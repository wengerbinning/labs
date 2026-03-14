import React, { useState } from 'react';
import {
  Card, Form, Input, Select, Button, Space, Table, Tag,
  Modal, Switch, Tabs, Collapse, message, Tooltip } from 'antd';
import {
  PlusOutlined, EditOutlined, DeleteOutlined, CopyOutlined,
  ExportOutlined, ImportOutlined, SearchOutlined } from '@ant-design/icons';
import type { ColumnsType } from 'antd/es/table';
import {
  Symbol, SymbolType, MacroSwitch, MacroConstant, MacroFunction,
  DataSymbol, TypeSymbol, FunctionSymbol } from '../types/symbol';

import './DisplayPanel.css';


const { Option } = Select;
const { TextArea } = Input;
const { TabPane } = Tabs;
const { Panel } = Collapse;

const DisplayPanel: React.FC = () => {
	const [collapsedMenu, setCollapsedMenu] = useState(false);
	const [collapsedList, setCollapsedList] = useState(false);


	return (
		<div className="panel">
			<aside className={`sidebar menu ${collapsedMenu ? 'collapsed' : ''}`}>
				{!collapsedMenu ? (
					<h2>功能菜单</h2>
				) : (
					<h2>Menu</h2>
				)}
				<button className="toggle-btn" onClick={() => setCollapsedMenu(!collapsedMenu)}>
					{collapsedMenu ? '→' : '←'}
				</button>
			</aside>
			<main className="main">
				<Card></Card>
			</main>
			<aside className={`sidebar list ${collapsedList ? 'collapsed' : ''}`}>
				{!collapsedList ? (
					<h2>内容列表</h2>
				) : (
					<h2>List</h2>
				)}
				<button className="toggle-btn" onClick={() => setCollapsedList(!collapsedList)}>
					{collapsedList ? '←' : '→'}
				</button>
			</aside>
		</div>
	);
};

export default DisplayPanel;
