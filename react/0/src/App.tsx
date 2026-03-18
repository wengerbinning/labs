// src/App.tsx

import React from 'react';
import { useState, lazy, Suspense } from 'react';
import { BrowserRouter, Routes, Route, Link } from 'react-router-dom';

import SymbolManagement from './components/SymbolManagement';
import DisplayPanel from './components/DisplayPanel';
import './App.css';

function App() {
	const headerHiddren = useState(false);
	const footerHiddren = useState(false);
	const [collapsedMenu, setCollapsedMenu] = useState(false);
	const [collapsedList, setCollapsedList] = useState(false);

	return (
		<div className="app">
			<header className="header">
				<div className='header-container'>
					<div className="logo">⸻  minimal  ⸻</div>
					<div className="title">WebSite</div>
					<div className="nav">
						<nav aria-label="主导航">
							<ul>
								<li><a href="#">首页</a></li>
								<li><a href="#">关于</a></li>
								<li><a href="#">作品</a></li>
								<li><a href="#">联系</a></li>
								<li><a href="#">插件</a></li>
							</ul>
						</nav>
						<div className="list">List</div>
					</div>
				</div>
			</header>
			<center className="center">
				<aside className={`sidebar-menu sidebar ${collapsedMenu ? 'collapsed' : ''}`}>
					{!collapsedMenu ? (<h2>功能菜单</h2>) : (<h2>Menu</h2>)}
					<button className="toggle-btn" onClick={() => setCollapsedMenu(!collapsedMenu)}>
						{collapsedMenu ? '→' : '←'}
					</button>
				</aside>
				<main className="main">
					<Suspense fallback={<div>加载中...</div>}>
					<Routes>
						<Route path="/" element={<DisplayPanel /> } />
						<Route path="/display" element={<SymbolManagement /> } />
					</Routes>
					</Suspense>
				</main>
				<aside className={`sidebar-list sidebar ${collapsedList ? 'collapsed' : ''}`}>
					{!collapsedList ? (<h2>内容列表</h2>) : (<h2>List</h2>)}
					<button className="toggle-btn" onClick={() => setCollapsedList(!collapsedList)}>
						{collapsedList ? '←' : '→'}
					</button>
				</aside>
			</center>
			<footer className="footer">
				<div className='footer-container'>
					<div className="copyright">&copy; 2025 minimal. 未经许可 不得复制</div>
					<div className="footer-links">
						<ul>
							<li><a href="#">隐私条款</a></li>
							<li><a href="#">使用条款</a></li>
							<li><a href="#">GitHub</a></li>
						</ul>
					</div>
				</div>
			</footer>
		</div>
	);
}

export default App;
