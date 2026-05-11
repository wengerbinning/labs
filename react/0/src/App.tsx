// src/App.tsx

import React from 'react';
import { useState, lazy, Suspense } from 'react';
import { BrowserRouter, Routes, Route, Link } from 'react-router-dom';

import SymbolManagement from './components/SymbolManagement';
import DisplayPanel from './components/DisplayPanel';

import FileImport from './components/FileImport';
import FileExport from './components/FileExport';

import DigitalCertificate from './components/DigitalCertificate';


import Project from './views/Project/Project';

import Applications from './views/Applications';
import SourceCode from './views/SourceCode/SourceCode';
import SourceFile from './views/SourceCode/SourceFile';
import SourceSymb from './views/SourceCode/SourceSymb';

import './App.css';

function App() {
	const headerHiddren = useState(false);
	const footerHiddren = useState(false);
	const [collapsedMenu, setCollapsedMenu] = useState(false);
	const [collapsedList, setCollapsedList] = useState(false);

	return (
		<div className="app">
			<Suspense fallback={<div>加载中...</div>}>
				<Routes>
					<Route path="/"  element={ <Project /> } />

					<Route path="/0" element={ <SourceCode /> } />
					<Route path="/1" element={ <SourceFile /> } />
					<Route path="/2" element={ <SourceSymb /> } />

					<Route path="/display" element={ <SymbolManagement /> } />

					<Route path="/file/export" element = { <FileExport /> } />
					<Route path="/file/import" element = { <FileImport /> } />

					<Route path="/model/certificate" element={ <DigitalCertificate /> } />
				</Routes>
			</Suspense>
		</div>
	);
}

export default App;
