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
			<Card>Test</Card>
			<Card>Test</Card>
			<Card>Test</Card>
			<Card>Test</Card>
		</div>
	);
};

export default DisplayPanel;
