
import { useState } from 'react';

import './Project.css';

interface ProType {
	uuid: number;
	name: string;
	resv: Boolean;

	// attr: string[];
}

interface Project {
	uuid: string;
	name: string;
	desc: string;
}

const projectType:ProType[]  = [{
	uuid: 0,
	name: "通用",
	resv: false
}, {
	uuid: 1,
	name: "文档",
	resv: false
}, {
	uuid: 2,
	name: "硬件",
	resv: false
}, {
	uuid: 3,
	name: "软件",
	resv: false
}, {
	uuid: 99,
	name: "Reserved",
	resv: true
}]




const Project: React.FC = (proj) => {
	const [projs, setProj] = useState<Project[]>([{
		uuid: '0',
		name: 'A',
		desc: 'This is a test Priject'
	}, {
		uuid: '1',
		name: 'B',
		desc: 'This is a test Priject'
	}, {
		uuid: '1',
		name: 'a',
		desc: 'This is a test Priject'
	}, {
		uuid: '1',
		name: 'b',
		desc: 'This is a test Priject'
	}, {
		uuid: '1',
		name: 'A1',
		desc: 'This is a test Priject'
	}, {
		uuid: '1',
		name: 'B1',
		desc: 'This is a test Priject'
	}]);

	const [curProj, setCurProj] = useState<string>('A');

	const headerHiddren = useState(false);
	const footerHiddren = useState(false);
	const [collapsedMenu, setCollapsedMenu] = useState(false);
	const [collapsedList, setCollapsedList] = useState(false);

	return(<div className="page">
		{/* Header */}
		<header className="header"><div className='header-container'>
			<div className="demo">
				<svg viewBox='0 0 50 50' height='100%' width='50' stroke-width='1' fill="blue" >
					{/* <circle cx='50' cy='100' r='50' fill='red' /> */}
					<rect width="100%" height="100%" fill="#f0f0f0" />
				</svg>
			</div>
			<div className="filter">
				<select value={curProj} onChange={(e) => setCurProj(e.target.value)}>
				{projectType.map((p) => { if (!p.resv) {
					return <option key={p.uuid} value={p.name}>{p.name}</option>
				}})}
				</select>
				<input list="projects" placeholder="请选择项目" />
				<datalist id="projects"> {projs.map((p) => (
					<option key={p.uuid} value={p.name}>{p.name}</option>
				))} </datalist>
			</div>
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
				{/* <div className="list">List</div> */}
			</div>
		</div></header>




		{/* Center */}
		<center className="center">
			{/* right aside */}
			<aside className={`aside menu${collapsedMenu ? ' collapsed' : ''}`}>
				{!collapsedMenu ? (<h2>功能菜单</h2>) : (<h2>Menu</h2>)}
					<button className="toggle-btn" onClick={() => setCollapsedMenu(!collapsedMenu)}>
						{collapsedMenu ? '→' : '←'}
					</button>
			</aside>

			<main className="main">

			</main>

			{/* left aside */}
			<aside className={`aside stat${collapsedList ? ' collapsed' : ''}`}>
			{!collapsedList ? (<h2>内容列表</h2>) : (<h2>List</h2>)}
				<button className="toggle-btn" onClick={() => setCollapsedList(!collapsedList)}>
					{collapsedList ? '←' : '→'}
				</button>
			</aside>
		</center>

		{/* Footer */}
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
	</div>);
}

export default Project;
