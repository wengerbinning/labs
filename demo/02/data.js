const typespace = [{
	name: 'project',
	icon: '📁',
	nest: true,
	// fold: true,
}, {
	name: 'module',
	icon: '📦',
	nest: true,
}, {
	name: 'folder',
	icon: '📘',
	nest: true,
}, {
	name: 'file',
	icon: '📘',
	nest: true,
}, {
	name: 'group',
	icon:'⚙️',
	nest: true,
}, {
	name: 'symbol',
	icon: '🔒',
	nest: false,
}]

const namespace = [{
	name: 'src',
	type: 'folder',
	fold: true,
	data: [{}]
}, {
	name: '.gitignore',
	type: 'file',
	data: '123'
}, {
	name: 'README.md',
	type: 'file',
	data: '123'
}, {
	name: 'package.json',
	type: 'file',
	data: '123'
}, {
	name: 'README.md',
	type: 'file',
	data: '123'
}]




// 渲染文件树
function renderFileTree(nodes, container, level = 0) {
	const ul = document.createElement('ul');
	ul.style.listStyle = 'none';

	nodes.forEach(node => {
		const li = document.createElement('li');
		const div = document.createElement('div');
		li.className = 'tree-item';
		div.className = 'tree-item-content';
		const type = typespace.find(item => item.name === node.type);

		// 1. 展开/折叠图标
		if (type && type.nest) {
			const expandSpan = document.createElement('span');
			expandSpan.className = `icon expand-icon ${node.fold ? '': 'expanded'}`;
			expandSpan.textContent = node.fold ? '▶' : '▼';
			expandSpan.style.fontSize = '12px';
			expandSpan.onclick = (e) => {
				e.stopPropagation();
				node.fold = !node.fold;
				renderFullFileTree();
			};
			div.appendChild(expandSpan);
		} else {
			// 文件缩进占位
			const placeholder = document.createElement('span');
			placeholder.className = 'expand-icon';
			placeholder.style.opacity = '0';
			placeholder.textContent = '▶';
			div.appendChild(placeholder);
		}

		// 2. 文件/文件夹图标
		const iconSpan = document.createElement('span');
		iconSpan.className = 'icon';
		if (node.type === 'folder') {
			iconSpan.textContent = node.icon || '📁';
			iconSpan.classList.add('folder-icon');
		} else {
			const fileIcon = iconMap[node.fileType] || iconMap.default;
			iconSpan.textContent = fileIcon.icon;
			iconSpan.classList.add(fileIcon.class);
		}
		div.appendChild(iconSpan);

		// 3. 文件名
		const nameSpan = document.createElement('span');
		nameSpan.className = 'name';
		nameSpan.textContent = node.name;
		div.appendChild(nameSpan);

		// 4. 点击事件
		div.onclick = (e) => {
			e.stopPropagation();
			// 移除其他选中状态
			document.querySelectorAll('.tree-item-content.selected').forEach(el => {
				el.classList.remove('selected');
			});
			div.classList.add('selected');
			if (node.type === 'file') {
				currentSelectedFile = node;
				currentPath = getPath(node, mockFileSystem);
				updateBreadcrumb();
				renderFilePreview(node);
			} else {
				node.expanded = !node.expanded;
				renderFullFileTree();
			}
		};
		li.appendChild(div);

		// 子节点
		if (node.type === 'folder' && node.children && node.children.length > 0) {
			const childContainer = document.createElement('div');
			childContainer.className = `tree-children ${node.expanded ? 'expanded' : ''}`;
			renderFileTree(node.children, childContainer, level + 1);
			li.appendChild(childContainer);
		}
		ul.appendChild(li);
	});

	container.appendChild(ul);
}

// 完整渲染文件树
function renderFullFileTree() {
	const container = document.getElementById('fileTree');
	container.innerHTML = '';
	renderFileTree(mockFileSystem, container);
}
