(function() {
	// ---------- 数据结构 ----------
	class TreeNode {
		constructor(id, name, type) {
			this.id = id;
			this.name = name;
			this.type = type;
			this.children = [];
			this.parent = null;
			this._symbols = [];
			this.content = '';
		}
		isDirectory() { return this.type !== 'file'; }
		getSymbols() { return this._symbols || []; }
		setSymbols(syms) { this._symbols = syms; }
	}

	// ---------- 全局状态 ----------
	let nextId = 5000;
	const rootNode = new TreeNode('root', 'root', 'root');
	let selectedNode = null;
	let openedFiles = [];
	let activeFileId = null;
	let collapsedGroups = new Set();
	let wordWrapEnabled = false;

	// 符号表多选状态
	let selectedSymbols = new Set();

	// 拖拽状态
	let dragSourceId = null;
	let dragSourceGroup = null;

	// 添加符号对话框状态
	let currentAddGroup = null;
	let selectedSymbolType = 'data';

	// 存储相关的配置文件名
	const STORAGE_KEY = 'file_editor_data';

	// 存储状态
	let saveTimeout = null;

	// 菜单状态
	let createMenuVisible = false;

	// ---------- 菜单控制 ----------
	function toggleCreateMenu() {
		const menu = document.getElementById('dropdownMenu');
		createMenuVisible = !createMenuVisible;
		if (createMenuVisible) {
			menu.classList.add('show');
		} else {
			menu.classList.remove('show');
		}
	}

	function hideCreateMenu() {
		const menu = document.getElementById('dropdownMenu');
		createMenuVisible = false;
		menu.classList.remove('show');
	}

	// ---------- 添加符号对话框 ----------
	function showAddSymbolDialog(groupName) {
		const activeFile = activeFileId ? findNodeById(rootNode, activeFileId) : null;
		if (!activeFile || activeFile.type !== 'file') {
			alert('请先选择一个文件');
			return;
		}

		currentAddGroup = groupName;
		document.getElementById('symbolNameInput').value = '';
		selectedSymbolType = 'data';

		// 更新类型按钮状态
		document.querySelectorAll('.dialog-type-btn').forEach(btn => {
			btn.classList.remove('selected');
			if (btn.dataset.type === 'data') {
				btn.classList.add('selected');
			}
		});

		document.getElementById('addSymbolDialog').style.display = 'block';
	}

	function hideAddSymbolDialog() {
		document.getElementById('addSymbolDialog').style.display = 'none';
		currentAddGroup = null;
	}

	function addSymbolToGroup() {
		const activeFile = activeFileId ? findNodeById(rootNode, activeFileId) : null;
		if (!activeFile || activeFile.type !== 'file') {
			alert('请先选择一个文件');
			hideAddSymbolDialog();
			return;
		}

		const symbolName = document.getElementById('symbolNameInput').value.trim();
		if (!symbolName) {
			alert('请输入符号名称');
			return;
		}

		// 创建新符号
		const newSymbol = {
			id: generateId(),
			name: symbolName,
			type: selectedSymbolType,
			group: currentAddGroup === '__ungrouped__' ? null : currentAddGroup
		};

		const symbols = activeFile.getSymbols();
		symbols.push(newSymbol);
		activeFile.setSymbols(symbols);

		// 如果添加到有名字的组，确保该组不在空组列表中
		if (currentAddGroup && currentAddGroup !== '__ungrouped__' && activeFile._emptyGroups) {
			activeFile._emptyGroups = activeFile._emptyGroups.filter(g => g !== currentAddGroup);
		}

		renderSymbolList();
		scheduleSave();
		hideAddSymbolDialog();
	}

	// ---------- 存储功能 ----------
	function updateStorageIndicator(saving = false) {
		const dot = document.getElementById('storageDot');
		const status = document.getElementById('storageStatus');
		if (saving) {
			dot.className = 'storage-dot saving';
			status.textContent = '保存中...';
		} else {
			dot.className = 'storage-dot';
			status.textContent = '已保存';
		}
	}

	function saveWorkspace() {
		if (saveTimeout) clearTimeout(saveTimeout);

		saveTimeout = setTimeout(() => {
			updateStorageIndicator(true);

			const workspaceData = {
				version: '1.0',
				lastSaved: new Date().toISOString(),
				root: serializeNode(rootNode),
				config: {
					collapsedGroups: Array.from(collapsedGroups),
					wordWrapEnabled: wordWrapEnabled,
					openedFiles: openedFiles.map(f => f.id),
					activeFileId: activeFileId
				}
			};

			try {
				localStorage.setItem(STORAGE_KEY, JSON.stringify(workspaceData));
				setTimeout(() => updateStorageIndicator(false), 300);
			} catch (e) {
				console.error('保存失败:', e);
				updateStorageIndicator(false);
			}
		}, 500);
	}

	function serializeNode(node) {
		const serialized = {
			id: node.id,
			name: node.name,
			type: node.type,
			content: node.content,
			_symbols: node._symbols.map(sym => ({ ...sym })),
			_emptyGroups: node._emptyGroups ? [...node._emptyGroups] : undefined,
			children: node.children.map(child => serializeNode(child))
		};
		return serialized;
	}

	function deserializeNode(data, parent = null) {
		const node = new TreeNode(data.id, data.name, data.type);
		node.content = data.content || '';
		node._symbols = (data._symbols || []).map(sym => ({ ...sym }));
		node._emptyGroups = data._emptyGroups ? [...data._emptyGroups] : [];

		if (data.children) {
			data.children.forEach(childData => {
				const childNode = deserializeNode(childData, node);
				node.children.push(childNode);
			});
		}

		node.parent = parent;
		return node;
	}

	function loadWorkspace() {
		try {
			const saved = localStorage.getItem(STORAGE_KEY);
			if (!saved) return false;

			const workspaceData = JSON.parse(saved);

			rootNode.children = [];

			if (workspaceData.root && workspaceData.root.children) {
				workspaceData.root.children.forEach(childData => {
					const node = deserializeNode(childData);
					rootNode.children.push(node);
				});
			}

			if (workspaceData.config) {
				collapsedGroups = new Set(workspaceData.config.collapsedGroups || []);
				wordWrapEnabled = workspaceData.config.wordWrapEnabled || false;

				if (workspaceData.config.openedFiles) {
					openedFiles = workspaceData.config.openedFiles
						.map(id => findNodeById(rootNode, id))
						.filter(node => node && node.type === 'file');
				}

				if (workspaceData.config.activeFileId) {
					activeFileId = workspaceData.config.activeFileId;
					const activeNode = findNodeById(rootNode, activeFileId);
					if (activeNode && activeNode.type === 'file') {
						if (!openedFiles.find(f => f.id === activeFileId)) {
							openedFiles.push(activeNode);
						}
					} else {
						activeFileId = openedFiles.length ? openedFiles[0].id : null;
					}
				}
			}

			updateStorageIndicator(false);
			return true;
		} catch (e) {
			console.error('加载失败:', e);
			return false;
		}
	}

	function scheduleSave() {
		saveWorkspace();
	}

	// ---------- 节点操作 ----------
	function findNodeById(node, id) {
		if (node.id === id) return node;
		if (node.children) {
			for (let child of node.children) {
				const found = findNodeById(child, id);
				if (found) return found;
			}
		}
		return null;
	}

	function generateId() {
		return (nextId++).toString(36) + '-' + Date.now().toString(36).slice(2);
	}

	function getDefaultName(type) {
		const map = { 'project':'项目','module':'模块','interface':'接口','model':'模型','file':'文件' };
		return map[type] || '未知';
	}

	function addDirectory(parentId, type) {
		const parent = findNodeById(rootNode, parentId);
		if (!parent?.isDirectory()) return null;
		const dir = new TreeNode(generateId(), getDefaultName(type), type);
		parent.children.push(dir);
		dir.parent = parent;
		renderTree();
		scheduleSave();
		return dir;
	}

	function addFile(parentId) {
		const parent = findNodeById(rootNode, parentId);
		if (!parent?.isDirectory()) return null;
		const file = new TreeNode(generateId(), getDefaultName('file'), 'file');
		file.setSymbols([
			{ id: generateId(), name: 'userData', type: 'data', group: '数据组' },
			{ id: generateId(), name: 'productData', type: 'data', group: '数据组' },
			{ id: generateId(), name: 'UserType', type: 'type', group: '类型组' },
			{ id: generateId(), name: 'ProductType', type: 'type', group: '类型组' },
			{ id: generateId(), name: 'UserInterface', type: 'interface', group: '接口组' },
			{ id: generateId(), name: 'tempVar', type: 'data', group: null },
		]);
		file.content = '// 新文件\n\n// 数据\nlet userData = { name: "John" };\nconst productData = { id: 1 };\n\n// 类型\ntype UserType = {\n  name: string;\n};\n\n// 接口\ninterface UserInterface {\n  getName(): string;\n}\n';
		parent.children.push(file);
		file.parent = parent;
		renderTree();
		scheduleSave();
		return file;
	}

	function deleteNode(nodeId) {
		const node = findNodeById(rootNode, nodeId);
		if (!node || node === rootNode) return;
		const parent = node.parent;
		if (parent) {
			const idx = parent.children.indexOf(node);
			if (idx !== -1) {
				parent.children.splice(idx, 1);
				if (selectedNode === node) selectedNode = null;
				openedFiles = openedFiles.filter(f => f.id !== node.id);
				if (activeFileId === node.id) {
					activeFileId = openedFiles.length ? openedFiles[0].id : null;
					collapsedGroups.clear();
					selectedSymbols.clear();
				}
				renderTree();
				renderTabsAndEditor();
				renderSymbolList();
				scheduleSave();
			}
		}
	}

	function openFile(fileNode) {
		if (!fileNode || fileNode.type !== 'file') return;
		if (!openedFiles.find(f => f.id === fileNode.id)) {
			openedFiles.push(fileNode);
		}
		if (activeFileId !== fileNode.id) {
			collapsedGroups.clear();
			selectedSymbols.clear();
		}
		activeFileId = fileNode.id;
		renderTabsAndEditor();
		renderSymbolList();
		scheduleSave();
	}

	function closeTab(fileId) {
		const idx = openedFiles.findIndex(f => f.id === fileId);
		if (idx === -1) return;
		openedFiles.splice(idx, 1);
		if (activeFileId === fileId) {
			activeFileId = openedFiles.length ? openedFiles[0].id : null;
			collapsedGroups.clear();
			selectedSymbols.clear();
		}
		renderTabsAndEditor();
		renderSymbolList();
		scheduleSave();
	}

	function toggleWordWrap() {
		wordWrapEnabled = !wordWrapEnabled;
		const cb = document.getElementById('wordWrapCheckbox');
		if (cb) cb.checked = wordWrapEnabled;
		renderTabsAndEditor();
		scheduleSave();
	}

	// ---------- 符号表操作 ----------
	function toggleSymbolSelection(symbolId, multiselect = false) {
		if (multiselect) {
			if (selectedSymbols.has(symbolId)) {
				selectedSymbols.delete(symbolId);
			} else {
				selectedSymbols.add(symbolId);
			}
		} else {
			if (selectedSymbols.has(symbolId) && selectedSymbols.size === 1) {
				selectedSymbols.clear();
			} else {
				selectedSymbols.clear();
				selectedSymbols.add(symbolId);
			}
		}
		renderSymbolList();
	}

	function createNewGroup() {
		const activeFile = activeFileId ? findNodeById(rootNode, activeFileId) : null;
		if (!activeFile || activeFile.type !== 'file') {
			alert('请先选择一个文件');
			return;
		}

		const groupInput = document.getElementById('groupNameInput');
		const groupName = groupInput.value.trim();

		if (!groupName) {
			alert('请输入组名称');
			return;
		}

		// 检查组名是否已存在
		const symbols = activeFile.getSymbols();
		const existingGroups = new Set(symbols.map(s => s.group).filter(g => g));
		if (existingGroups.has(groupName)) {
			alert(`组 "${groupName}" 已存在`);
			return;
		}

		// 创建空组
		if (!activeFile._emptyGroups) {
			activeFile._emptyGroups = [];
		}

		if (!activeFile._emptyGroups.includes(groupName)) {
			activeFile._emptyGroups.push(groupName);
		}

		// 如果有选中的符号，将它们移入新组
		if (selectedSymbols.size > 0) {
			symbols.forEach(sym => {
				if (selectedSymbols.has(sym.id)) {
					sym.group = groupName;
				}
			});

			// 如果新组有了符号，从空组列表中移除
			const hasSymbols = symbols.some(s => s.group === groupName);
			if (hasSymbols && activeFile._emptyGroups) {
				activeFile._emptyGroups = activeFile._emptyGroups.filter(g => g !== groupName);
			}
		}

		activeFile.setSymbols(symbols);

		// 清空输入框
		groupInput.value = '';

		renderSymbolList();
		scheduleSave();
	}

	function deleteGroup(groupName) {
		const activeFile = activeFileId ? findNodeById(rootNode, activeFileId) : null;
		if (!activeFile || activeFile.type !== 'file') return;

		if (confirm(`确定解散组 "${groupName}" 吗？组内所有符号将变为未分组`)) {
			const symbols = activeFile.getSymbols();
			symbols.forEach(sym => {
				if (sym.group === groupName) {
					sym.group = null;
				}
			});

			// 从空组列表中移除
			if (activeFile._emptyGroups) {
				activeFile._emptyGroups = activeFile._emptyGroups.filter(g => g !== groupName);
			}

			activeFile.setSymbols(symbols);
			selectedSymbols.clear();
			renderSymbolList();
			scheduleSave();
		}
	}

	function toggleGroupCollapse(groupName) {
		if (collapsedGroups.has(groupName)) {
			collapsedGroups.delete(groupName);
		} else {
			collapsedGroups.add(groupName);
		}
		renderSymbolList();
		scheduleSave();
	}

	// 渲染树
	function renderTree() {
		const container = document.getElementById('treeRoot');
		container.innerHTML = '';
		rootNode.children.forEach(c => container.appendChild(renderTreeNode(c)));
	}

	function renderTreeNode(node) {
		const li = document.createElement('li');
		li.className = 'tree-item';
		li.setAttribute('data-id', node.id);

		const div = document.createElement('div');
		div.className = `tree-node ${selectedNode?.id === node.id ? 'selected' : ''}`;
		const iconSpan = document.createElement('span'); iconSpan.className = 'icon';
		if (node.isDirectory()) {
			const icons = { 'project':'📁','module':'📦','interface':'🔌','model':'🧩' };
			iconSpan.textContent = icons[node.type] || '📂';
		} else { iconSpan.textContent = '📄'; }
		const typeSpan = document.createElement('span'); typeSpan.className = 'type-badge';
		typeSpan.textContent = node.type === 'file' ? '文件' : node.type.slice(0,2);
		const labelSpan = document.createElement('span'); labelSpan.className = 'label'; labelSpan.textContent = node.name;

		const actionsDiv = document.createElement('div'); actionsDiv.className = 'dir-actions';
		if (node.isDirectory()) {
			const addFileBtn = document.createElement('button'); addFileBtn.innerHTML = '➕'; addFileBtn.className = 'small-btn'; addFileBtn.title = '添加文件';
			addFileBtn.onclick = (e) => { e.stopPropagation(); addFile(node.id); };
			actionsDiv.appendChild(addFileBtn);
		}
		if (node !== rootNode) {
			const delBtn = document.createElement('button'); delBtn.innerHTML = '❌'; delBtn.className = 'small-btn'; delBtn.title = '删除';
			delBtn.onclick = (e) => { e.stopPropagation(); if (confirm(`删除“${node.name}”?`)) deleteNode(node.id); };
			actionsDiv.appendChild(delBtn);
		}

		div.appendChild(iconSpan); div.appendChild(typeSpan); div.appendChild(labelSpan); div.appendChild(actionsDiv);
		li.appendChild(div);

		div.addEventListener('click', (e) => {
			if (e.target.closest('.small-btn')) return;
			selectedNode = node;
			renderTree();
			if (!node.isDirectory()) { openFile(node); }
			else { activeFileId = null; collapsedGroups.clear(); selectedSymbols.clear(); renderSymbolList(); scheduleSave(); }
		});

		if (node.children.length) {
			const ul = document.createElement('ul'); ul.className = 'children';
			node.children.forEach(ch => ul.appendChild(renderTreeNode(ch)));
			li.appendChild(ul);
		}
		return li;
	}

	// 渲染标签 + 编辑器
	function renderTabsAndEditor() {
		const tabContainer = document.getElementById('tabContainer');
		const editorDiv = document.getElementById('editorContentArea');
		const toolbar = document.getElementById('editorToolbar');

		if (!openedFiles.length) {
			tabContainer.innerHTML = '<div class="placeholder-editor" style="padding:4px 0;">暂无打开文件</div>';
			editorDiv.innerHTML = '<div class="placeholder-editor">选择一个文件开始编辑</div>';
			toolbar.style.display = 'none';
			return;
		}

		toolbar.style.display = 'flex';
		let tabsHtml = '';
		openedFiles.forEach(file => {
			const activeClass = (activeFileId === file.id) ? 'active' : '';
			tabsHtml += `<div class="tab-item ${activeClass}" data-file-id="${file.id}">
				<span>📄 ${escapeHtml(file.name)}</span>
				<span class="tab-close" data-close-id="${file.id}">✕</span>
			</div>`;
		});
		tabContainer.innerHTML = tabsHtml;

		const activeFile = openedFiles.find(f => f.id === activeFileId);
		if (activeFile) {
			const lines = (activeFile.content || '').split('\n');
			const lineCount = Math.max(lines.length, 1);
			let lineNumbersHtml = '';
			for (let i = 1; i <= lineCount; i++) lineNumbersHtml += i + '\n';

			const wrapClass = wordWrapEnabled ? 'word-wrap' : '';
			editorDiv.innerHTML = `
				<div class="editor-with-line-numbers">
					<div class="line-numbers">${escapeHtml(lineNumbersHtml)}</div>
					<textarea class="editor-textarea ${wrapClass}" id="liveEditor" spellcheck="false">${escapeHtml(activeFile.content || '')}</textarea>
				</div>
			`;
			const editor = document.getElementById('liveEditor');
			const lineNumbersDiv = editorDiv.querySelector('.line-numbers');

			editor.addEventListener('input', (e) => {
				if (activeFile) activeFile.content = e.target.value;
				const newLines = activeFile.content.split('\n').length;
				let newLineNumbers = '';
				for (let i = 1; i <= newLines; i++) newLineNumbers += i + '\n';
				if (lineNumbersDiv) lineNumbersDiv.textContent = newLineNumbers;
				updateCursorPosition(editor);
				scheduleSave();
			});

			editor.addEventListener('scroll', () => {
				if (lineNumbersDiv) lineNumbersDiv.scrollTop = editor.scrollTop;
			});

			editor.addEventListener('click', () => updateCursorPosition(editor));
			editor.addEventListener('keyup', () => updateCursorPosition(editor));

			setTimeout(() => updateCursorPosition(editor), 50);
		} else {
			editorDiv.innerHTML = '<div class="placeholder-editor">文件未找到</div>';
		}

		document.querySelectorAll('.tab-item').forEach(tab => {
			const fileId = tab.dataset.fileId;
			tab.addEventListener('click', (e) => {
				if (e.target.classList.contains('tab-close')) return;
				if (fileId) {
					const fileNode = findNodeById(rootNode, fileId);
					if (fileNode) {
						activeFileId = fileId;
						selectedNode = fileNode;
						collapsedGroups.clear();
						selectedSymbols.clear();
						renderTree();
						renderTabsAndEditor();
						renderSymbolList();
						scheduleSave();
					}
				}
			});
		});
		document.querySelectorAll('.tab-close').forEach(btn => {
			btn.addEventListener('click', (e) => {
				e.stopPropagation();
				const closeId = btn.dataset.closeId;
				if (closeId) closeTab(closeId);
			});
		});

		const cb = document.getElementById('wordWrapCheckbox');
		if (cb) cb.checked = wordWrapEnabled;
	}

	function updateCursorPosition(editor) {
		if (!editor) return;
		const pos = editor.selectionStart;
		const text = editor.value;
		const lines = text.substring(0, pos).split('\n');
		const line = lines.length;
		const column = lines[lines.length - 1].length + 1;
		document.getElementById('cursorPos').textContent = `行 ${line}, 列 ${column}`;
	}

	function escapeHtml(str) {
		return str.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;');
	}

	// 符号表渲染
	function renderSymbolList() {
		const container = document.getElementById('symbolListContainer');
		const badge = document.getElementById('currentSymbolFile');
		const selectedCount = document.getElementById('selectedCount');

		let fileNode = activeFileId ? findNodeById(rootNode, activeFileId) : null;

		if (!fileNode || fileNode.type !== 'file') {
			badge.textContent = '未选中';
			container.innerHTML = '<div class="empty-symbol">← 选择文件查看符号</div>';
			selectedCount.textContent = '0';
			return;
		}

		badge.textContent = fileNode.name;
		selectedCount.textContent = selectedSymbols.size;

		const symbols = fileNode.getSymbols() || [];
		const emptyGroups = fileNode._emptyGroups || [];

		// 按组分类
		const groups = new Map();

		// 先添加有符号的组
		symbols.forEach(sym => {
			const g = sym.group || '__ungrouped__';
			if (!groups.has(g)) groups.set(g, []);
			groups.get(g).push(sym);
		});

		// 添加空组
		emptyGroups.forEach(groupName => {
			if (!groups.has(groupName)) {
				groups.set(groupName, []);
			}
		});

		// 按原始顺序排序组
		const groupOrder = [];
		symbols.forEach(sym => {
			const g = sym.group || '__ungrouped__';
			if (!groupOrder.includes(g)) groupOrder.push(g);
		});

		// 添加空组到末尾
		emptyGroups.forEach(g => {
			if (!groupOrder.includes(g)) {
				groupOrder.push(g);
			}
		});

		const sortedGroups = groupOrder.map(g => [g, groups.get(g) || []]);

		container.innerHTML = '';
		sortedGroups.forEach(([groupName, symList]) => {
			const isUngrouped = (groupName === '__ungrouped__');
			const displayGroupName = isUngrouped ? '未分组' : groupName;
			const isCollapsed = collapsedGroups.has(groupName);
			const isEmpty = symList.length === 0;

			const groupDiv = document.createElement('div');
			groupDiv.className = 'symbol-group';
			groupDiv.dataset.groupName = groupName;

			const header = document.createElement('div');
			header.className = 'group-header';
			header.draggable = !isUngrouped;
			header.dataset.groupName = groupName;

			header.innerHTML = `
				<span class="group-toggle">${isCollapsed ? '▶' : '▼'}</span>
				<span class="group-name">${displayGroupName}</span>
				<span class="group-count">${symList.length}</span>
				<span class="group-add-symbol" title="添加符号到该组">➕</span>
				${!isUngrouped ? '<span class="group-delete" title="删除组">🗑️</span>' : ''}
			`;

			// 组折叠事件
			header.querySelector('.group-toggle').addEventListener('click', (e) => {
				e.stopPropagation();
				toggleGroupCollapse(groupName);
			});

			// 组添加符号事件
			header.querySelector('.group-add-symbol').addEventListener('click', (e) => {
				e.stopPropagation();
				showAddSymbolDialog(groupName);
			});

			// 组删除事件
			if (!isUngrouped) {
				header.querySelector('.group-delete').addEventListener('click', (e) => {
					e.stopPropagation();
					deleteGroup(groupName);
				});
			}

			// 组拖拽事件（用于组间排序）
			header.addEventListener('dragstart', (e) => {
				if (isUngrouped) {
					e.preventDefault();
					return;
				}
				dragSourceGroup = groupName;
				header.classList.add('dragging');
				e.dataTransfer.setData('text/plain', groupName);
				e.dataTransfer.effectAllowed = 'move';
			});

			header.addEventListener('dragend', () => {
				dragSourceGroup = null;
				document.querySelectorAll('.group-header').forEach(h => h.classList.remove('drag-over'));
			});

			header.addEventListener('dragover', (e) => {
				e.preventDefault();
				if (!dragSourceGroup || dragSourceGroup === groupName || isUngrouped) return;
				header.classList.add('drag-over');
			});

			header.addEventListener('dragleave', () => {
				header.classList.remove('drag-over');
			});

			header.addEventListener('drop', (e) => {
				e.preventDefault();
				header.classList.remove('drag-over');
				if (!dragSourceGroup || dragSourceGroup === groupName || isUngrouped) return;

				// 重新排序组
				const activeFile = activeFileId ? findNodeById(rootNode, activeFileId) : null;
				if (!activeFile) return;

				const symbols = activeFile.getSymbols();
				const emptyGroups = activeFile._emptyGroups || [];

				// 获取当前组顺序
				const groupSet = new Set();
				symbols.forEach(s => groupSet.add(s.group || '__ungrouped__'));
				emptyGroups.forEach(g => groupSet.add(g));
				const groups = Array.from(groupSet);

				const sourceIndex = groups.indexOf(dragSourceGroup);
				const targetIndex = groups.indexOf(groupName);

				if (sourceIndex !== -1 && targetIndex !== -1) {
					// 移动组
					groups.splice(sourceIndex, 1);
					groups.splice(targetIndex, 0, dragSourceGroup);

					// 重新排序符号
					const newSymbols = [];
					groups.forEach(g => {
						if (g === '__ungrouped__') {
							const groupSymbols = symbols
								.filter(s => !s.group)
								.sort((a, b) => {
									return symbols.indexOf(a) - symbols.indexOf(b);
								});
							newSymbols.push(...groupSymbols);
						} else {
							const groupSymbols = symbols
								.filter(s => s.group === g)
								.sort((a, b) => {
									return symbols.indexOf(a) - symbols.indexOf(b);
								});
							newSymbols.push(...groupSymbols);
						}
					});

					activeFile.setSymbols(newSymbols);
					renderSymbolList();
					scheduleSave();
				}
			});

			const childrenDiv = document.createElement('div');
			childrenDiv.className = `group-children ${isCollapsed ? 'collapsed' : ''}`;

			if (isEmpty && !isUngrouped) {
				const emptyMessage = document.createElement('div');
				emptyMessage.className = 'empty-group-message';
				emptyMessage.textContent = '✨ 空组，点击➕添加符号';
				childrenDiv.appendChild(emptyMessage);
			}

			symList.forEach(sym => {
				const item = document.createElement('div');
				item.className = `symbol-item ${selectedSymbols.has(sym.id) ? 'selected' : ''}`;
				item.draggable = true;
				item.dataset.symbolId = sym.id;

				const left = document.createElement('div'); left.className = 'symbol-left';
				const icon = document.createElement('span'); icon.className = 'symbol-icon';

				if (sym.type === 'data') icon.textContent = '📊';
				else if (sym.type === 'type') icon.textContent = '📐';
				else if (sym.type === 'interface') icon.textContent = '🔌';
				else icon.textContent = '🔶';

				const nameSpan = document.createElement('span'); nameSpan.className = 'symbol-name';
				nameSpan.textContent = sym.name;
				nameSpan.title = sym.name;
				const typeSpan = document.createElement('span'); typeSpan.className = 'symbol-type';
				typeSpan.textContent = sym.type;
				left.appendChild(icon); left.appendChild(nameSpan); left.appendChild(typeSpan);

				item.appendChild(left);

				// 符号点击选择
				item.addEventListener('click', (e) => {
					e.stopPropagation();
					const multiselect = e.ctrlKey || e.metaKey;
					toggleSymbolSelection(sym.id, multiselect);
				});

				// 符号拖拽事件（用于跨组移动）
				item.addEventListener('dragstart', (e) => {
					dragSourceId = sym.id;
					dragSourceGroup = sym.group || '__ungrouped__';
					item.classList.add('dragging');
					e.dataTransfer.setData('text/plain', sym.id);
					e.dataTransfer.effectAllowed = 'move';
				});

				item.addEventListener('dragend', () => {
					dragSourceId = null;
					dragSourceGroup = null;
					document.querySelectorAll('.symbol-item, .group-header').forEach(el => el.classList.remove('drag-over'));
				});

				item.addEventListener('dragover', (e) => {
					e.preventDefault();
					if (dragSourceId && dragSourceId !== sym.id) {
						item.classList.add('drag-over');
					}
				});

				item.addEventListener('dragleave', () => {
					item.classList.remove('drag-over');
				});

				item.addEventListener('drop', (e) => {
					e.preventDefault();
					item.classList.remove('drag-over');
					if (!dragSourceId || dragSourceId === sym.id) return;

					// 移动符号到新组
					const activeFile = activeFileId ? findNodeById(rootNode, activeFileId) : null;
					if (!activeFile) return;

					const symbols = activeFile.getSymbols();
					const sourceIdx = symbols.findIndex(s => s.id === dragSourceId);
					const targetIdx = symbols.findIndex(s => s.id === sym.id);

					if (sourceIdx !== -1 && targetIdx !== -1) {
						const [moved] = symbols.splice(sourceIdx, 1);
						moved.group = sym.group;
						symbols.splice(targetIdx, 0, moved);
						activeFile.setSymbols(symbols);
						renderSymbolList();
						scheduleSave();
					}
				});

				childrenDiv.appendChild(item);
			});

			groupDiv.appendChild(header);
			groupDiv.appendChild(childrenDiv);
			container.appendChild(groupDiv);
		});
	}

	// 初始化示例数据
	function initMockData() {
		const proj = new TreeNode(generateId(), 'demo', 'project');
		const mod = new TreeNode(generateId(), 'core', 'module');
		const fileA = new TreeNode(generateId(), 'main.js', 'file');
		fileA.setSymbols([
			{ id: generateId(), name: 'userData', type: 'data', group: '数据组' },
			{ id: generateId(), name: 'productData', type: 'data', group: '数据组' },
			{ id: generateId(), name: 'UserType', type: 'type', group: '类型组' },
			{ id: generateId(), name: 'ProductType', type: 'type', group: '类型组' },
			{ id: generateId(), name: 'UserInterface', type: 'interface', group: '接口组' },
			{ id: generateId(), name: 'tempVar', type: 'data', group: null },
		]);
		fileA._emptyGroups = [];
		fileA.content = '// main.js\n\n// 数据\nlet userData = { name: "John" };\nconst productData = { id: 1 };\n\n// 类型\ntype UserType = {\n  name: string;\n};\n\n// 接口\ninterface UserInterface {\n  getName(): string;\n}\n';

		const fileB = new TreeNode(generateId(), 'types.ts', 'file');
		fileB.setSymbols([
			{ id: generateId(), name: 'Config', type: 'type', group: '配置组' },
			{ id: generateId(), name: 'State', type: 'type', group: '状态组' },
			{ id: generateId(), name: 'API', type: 'interface', group: '接口组' },
		]);
		fileB._emptyGroups = [];

		proj.children = [mod, fileB];
		mod.children = [fileA];
		[proj, mod, fileA, fileB].forEach(n => {
			if (n.children) n.children.forEach(c => c.parent = n);
		});
		rootNode.children = [proj];
	}

	// 启动
	window.addEventListener('load', () => {
		const loaded = loadWorkspace();

		if (!loaded) {
			initMockData();
		}

		renderTree();

		if (activeFileId) {
			const activeNode = findNodeById(rootNode, activeFileId);
			if (activeNode) {
				selectedNode = activeNode;
			}
		} else if (!selectedNode && rootNode.children.length > 0) {
			const firstFile = findNodeById(rootNode, rootNode.children[0]?.children[0]?.children[0]?.id);
			if (firstFile) {
				selectedNode = firstFile;
				openFile(firstFile);
			}
		}

		renderTree();
		renderTabsAndEditor();
		renderSymbolList();

		// 目录菜单事件
		const menuToggle = document.getElementById('menuToggle');
		menuToggle.addEventListener('click', (e) => {
			e.stopPropagation();
			toggleCreateMenu();
		});

		document.addEventListener('click', (e) => {
			if (!e.target.closest('#createMenu')) {
				hideCreateMenu();
			}
		});

		// 目录菜单项事件
		document.getElementById('menuProject').addEventListener('click', () => {
			addDirectory(rootNode.id, 'project');
			hideCreateMenu();
		});
		document.getElementById('menuModule').addEventListener('click', () => {
			addDirectory(rootNode.id, 'module');
			hideCreateMenu();
		});
		document.getElementById('menuInterface').addEventListener('click', () => {
			addDirectory(rootNode.id, 'interface');
			hideCreateMenu();
		});
		document.getElementById('menuModel').addEventListener('click', () => {
			addDirectory(rootNode.id, 'model');
			hideCreateMenu();
		});
		document.getElementById('menuFile').addEventListener('click', () => {
			let parentId = rootNode.id;
			if (selectedNode?.isDirectory()) parentId = selectedNode.id;
			else if (selectedNode?.parent) parentId = selectedNode.parent.id;
			addFile(parentId);
			hideCreateMenu();
		});

		// 创建组按钮事件
		document.getElementById('createGroupBtn').addEventListener('click', createNewGroup);

		// 输入框回车事件
		document.getElementById('groupNameInput').addEventListener('keypress', (e) => {
			if (e.key === 'Enter') {
				createNewGroup();
			}
		});

		document.getElementById('wordWrapToggle').addEventListener('click', toggleWordWrap);

		// 添加符号对话框事件
		document.getElementById('cancelAddSymbol').addEventListener('click', hideAddSymbolDialog);
		document.getElementById('confirmAddSymbol').addEventListener('click', addSymbolToGroup);
		document.getElementById('dialogOverlay').addEventListener('click', hideAddSymbolDialog);

		// 对话框类型选择
		document.querySelectorAll('.dialog-type-btn').forEach(btn => {
			btn.addEventListener('click', () => {
				document.querySelectorAll('.dialog-type-btn').forEach(b => b.classList.remove('selected'));
				btn.classList.add('selected');
				selectedSymbolType = btn.dataset.type;
			});
		});

		scheduleSave();
	});

	window.addEventListener('beforeunload', () => {
		if (saveTimeout) {
			clearTimeout(saveTimeout);
			saveWorkspace();
		}
	});
})();
