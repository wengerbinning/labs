// src/App.tsx

import React from 'react';
import { lazy, Suspense } from 'react';
import { BrowserRouter, Routes, Route, Link } from 'react-router-dom';

import SymbolManagement from './components/SymbolManagement';
import DisplayPanel from './components/DisplayPanel';
import './App.css';

function App() {
	return (
		<Suspense fallback={<div>加载中...</div>}>
			<BrowserRouter>
				<Routes>
					<Route path="/" element={<SymbolManagement /> } />
					<Route path="/display" element={<DisplayPanel /> } />
				</Routes>
			</BrowserRouter>
		</Suspense>
		// <div className="App"> </div>
	);
}

export default App;
