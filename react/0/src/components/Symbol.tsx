import React, { useState, useMemo } from 'react';
import './FileList.css';
import { SymbolType, Symbol,
    MacroSwitch, MacroConstant, MacroFunction,
    DataSymbol, TypeSymbol, FunctionSymbol,
} from '../types/symbol';



const MAX_ITEMS: number = 1024;

// 初始数据
const SymbolDefault: Symbol[] = [
	{ uuid: '0', name: 'Symbol 1' },
	{ uuid: '1', name: 'Symbol 2' },
	{ uuid: '2', name: 'Symbol 3' },
];

const FileList: React.FC = () => {
	const [items, setItems] = useState<Symbol[]>(SymbolDefault);
	const [selectedIds, setSelectedIds] = useState<Set<number>>(new Set());

	// 添加项
	const handleAdd = (): void => {
		const name: string | null = prompt('请输入文件名称：');
		if (name && name.trim()) {
			const newItem: Symbol = {
				id: Date.now(),
				name: name.trim(),
			};
			setItems([...items, newItem]);
	}
	};

	// 编辑项
	const handleEdit = (id: number, currentName: string): void => {
		const newName: string | null = prompt('编辑名称：', currentName);
		if (newName && newName.trim()) {
			setItems(items.map(item =>
				item.id === id ? { ...item, name: newName.trim() } : item
			));
		}
	};

	// 删除项
	const handleDelete = (id: number): void => {
		if (window.confirm('确定删除此项吗？')) {
			setItems(items.filter(item => item.id !== id));
			const newSelected = new Set(selectedIds);
			newSelected.delete(id);
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
			setItems(items.filter(item => !selectedIds.has(item.id)));
			setSelectedIds(new Set());
		}
	};

	// 全选 / 取消全选
	const handleSelectAll = (): void => {
		if (selectedIds.size === items.length) {
			setSelectedIds(new Set());
		} else {
		const allIds = new Set(items.map(item => item.id));
		setSelectedIds(allIds);
		}
	};

	// 切换单个项的选中状态
	const toggleSelect = (id: number): void => {
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
          ➕ 添加 ({items.length}/{MAX_ITEMS})
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
          {items.map((item: Symbol) => (
            <li key={item.uuid} className="file-item">
              <label className="checkbox-wrapper">
                <input
                  type="checkbox"
                  checked={selectedIds.has(item.uuid)}
                  onChange={() => toggleSelect(item.uuid)}
                />
              </label>
              <span className="file-name">{item.name}</span>
              <div className="item-actions">
                <button
                  onClick={() => handleEdit(item.uuid, item.name)}
                  className="btn-edit"
                >
                  ✏️ 编辑
                </button>
                <button
                  onClick={() => handleDelete(item.uuid)}
                  className="btn-delete"
                >
                  🗑️ 删除
                </button>
              </div>
            </li>
          ))}
        </ul>
      )}
    </div>
  );
};

export default FileList;
