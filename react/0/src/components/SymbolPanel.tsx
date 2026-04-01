import './SymbolPanel.css';
import React, { useState, useMemo } from 'react';
import { SymbolType, Symbol,
    MacroSwitch, MacroConstant, MacroFunction,
    DataSymbol, TypeSymbol, FunctionSymbol,
} from '../types/symbol';

// 初始数据
const SymbolDefault: Symbol[] = [
	{ uuid: '0', name: 'Symbol 1' },
	{ uuid: '1', name: 'Symbol 2' },
	{ uuid: '2', name: 'Symbol 3' },
];

const SymbolPanel: React.FC = () => {
	const [items, setItems] = useState<Symbol[]>(SymbolDefault);
	const [selectedIds, setSelectedIds] = useState<Set<string>>(new Set());

	// 添加项
	const handleAdd = (): void => {
		const name: string | null = prompt('请输入文件名称：');
		if (name && name.trim()) {
			const newItem: Symbol = {
				uuid: Date.now().toString(),
				name: name.trim(),
			};
			setItems([...items, newItem]);
	}
	};

	// 编辑项
	const handleEdit = (uuid: string, currentName: string): void => {
		const newName: string | null = prompt('编辑名称：', currentName);
		if (newName && newName.trim()) {
			setItems(items.map(item =>
				item.uuid === uuid ? { ...item, name: newName.trim() } : item
			));
		}
	};

	// 删除项
	const handleDelete = (uuid: string): void => {
		if (window.confirm('确定删除此项吗？')) {
			setItems(items.filter(item => item.uuid!== uuid));
			const newSelected = new Set(selectedIds);
			newSelected.delete(uuid);
			setSelectedIds(newSelected);
		}
	};

	// 批量删除
	const handleBatchDelete = (): void => {
		if (selectedIds.size === 0) {
			alert('没有选中任何项');
			return;
		}
		if (window.confirm(`确定删除选中的 ${selectedIds.size} 项吗？`)) {
			setItems(items.filter(item => !selectedIds.has(item.uuid)));
			setSelectedIds(new Set());
		}
	};

	// 全选 / 取消全选
	const handleSelectAll = (): void => {
		if (selectedIds.size === items.length) {
			setSelectedIds(new Set());
		} else {
		const allIds = new Set(items.map(item => item.uuid));
		setSelectedIds(allIds);
		}
	};

	// 切换单个项的选中状态
	const toggleSelect = (id: string): void => {
	const newSelected = new Set(selectedIds);
	if (newSelected.has(id)) {
		newSelected.delete(id);
	} else {
		newSelected.add(id);
	}
	setSelectedIds(newSelected);
	};

	// 判断是否全选（缓存计算结果）
	const isAllSelected: boolean = useMemo(() => {
		return items.length > 0 && selectedIds.size === items.length;
	}, [items.length, selectedIds.size]);


	return (
	<div className="file-list-container">
		<div className="toolbar">
			<button onClick={handleAdd} className="btn-add">
				➕ 添加 ({items.length})
			</button>
			<button onClick={handleSelectAll} className="btn-select-all">
				{isAllSelected ? '取消全选' : '全选'}
			</button>
			<button onClick={handleBatchDelete} className="btn-batch-delete">
				🗑️ 批量删除 ({selectedIds.size})
			</button>
		</div>

		{items.length === 0 ? (
			<div className="empty-message">暂无文件，点击“添加”创建</div>
		) : (
			<ul className="file-list">
				{items.map((sym: Symbol) => (
					<li key={sym.uuid} className="file-item">
						<label className="checkbox-wrapper">
							<input type="checkbox"
								checked={selectedIds.has(sym.uuid)}
								onChange={() => toggleSelect(sym.uuid)}
							/>
						</label>
						<span className="file-name">{sym.name}</span>
						<div className="item-actions">
							<button className="btn-edit"
								onClick={() => handleEdit(sym.uuid, sym.name)}
							>编辑</button>
							<button className="btn-delete"
								onClick={() => handleDelete(sym.uuid)}
							>🗑️ 删除</button>
						</div>
					</li>
				))}
			</ul>
		)}
	</div>
  );
};

export default SymbolPanel;
