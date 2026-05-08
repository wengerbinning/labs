
import React, { useState } from 'react';
import './SourceCode.css';

interface Code {
	name: string;
	desc: string;
	stat: string;
	attr: string[];
	vers: string[];
};

const SourceCode: React.FC = () => {
	const [codes, setCode] = useState<Code[]>([
		// {
		// 	name: "binutil",
		// 	desc: "This is Basic Toolkit",
		// 	stat: "develop",
		// 	attr: ["fork"],
		// 	vers: ["1.0.0"]
		// },
		// {
		// 	name: "linux",
		// 	desc: "This is Linux Kernel",
		// 	stat: "develop",
		// 	attr: ["fork"],
		// 	vers: ["1.0.0"]
		// }
	]);

	// const importCSV = (event: React.ChangeEvent<HTMLInputElement>) => {
	// 	const file = event.target.files?.[0];
	// 	const reader = new FileReader();

	// 	if (!file)
	// 		return;

	// 	reader.onload = (e) => {
	// 		const text = e.target?.result as string;
	// 		const lines = text.trim().split(/\r?\n/);
	// 		const headers = lines[0].split(',');
	// 		// const entries = lines.slice(1).map(line => {
	// 		// 	const vals = line.split(',');
	// 		// 	let attr: Attr = {};
	// 		// 	headers.forEach((key, idx) => {
	// 		// 		attr[key] = vals[idx] || '';

	// 		// 	});
	// 		// 	return attr;
	// 		// });
	// 		const apps = lines.slice(1).map(line => {
	// 			const vals = line.split(',');
	// 			let app: App = {} as App;
	// 			headers.forEach((key, idx) => {
	// 				switch (key) {
	// 				case 'Name':
	// 					app.name = vals[idx] || '';
	// 					break;
	// 				case 'Version':
	// 					app.version = vals[idx] || '';
	// 					break;
	// 				}
	// 			});
	// 			return app;
	// 		});
	// 		// setHeaders(headers);
	// 		// setEntries(entries);
	// 		setApps(apps);
	// 	};

	// 	reader.readAsText(file, 'UTF-8');
	// }


	return(<div>
	{ codes && codes.length === 0 ? (
		<span>Nothing</span>
	):(
		<div className='list'>
			{ codes.map((code, idx) => (
				<div className="card" key={idx}>
					<span>{code.name}</span>
					<span>{code.desc}</span>
				</div>
			))}
		</div>
	)}
	</div>)
};

export default SourceCode;
