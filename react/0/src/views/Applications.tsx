import React, { useState } from 'react';
import './Applications.css';

// interface Attr {
// 	[key: string]: string;
// };

interface App {
	uuid: string;
	group: string;
	name: string;
	version: string;
	attributes: string[];
};

const Applications: React.FC = () => {
	// const [headers, setHeaders] = useState<string[]>([]);
	// const [entries, setEntries] = useState<Attr[]>([]);
	const [apps, setApps] = useState<App[]>([]);

	const importCSV = (event: React.ChangeEvent<HTMLInputElement>) => {
		const file = event.target.files?.[0];
		const reader = new FileReader();

		if (!file)
			return;

		reader.onload = (e) => {
			const text = e.target?.result as string;
			const lines = text.trim().split(/\r?\n/);
			const headers = lines[0].split(',');
			// const entries = lines.slice(1).map(line => {
			// 	const vals = line.split(',');
			// 	let attr: Attr = {};
			// 	headers.forEach((key, idx) => {
			// 		attr[key] = vals[idx] || '';

			// 	});
			// 	return attr;
			// });
			const apps = lines.slice(1).map(line => {
				const vals = line.split(',');
				let app: App = {} as App;
				headers.forEach((key, idx) => {
					switch (key) {
					case 'Name':
						app.name = vals[idx] || '';
						break;
					case 'Version':
						app.version = vals[idx] || '';
						break;
					}
				});
				return app;
			});
			// setHeaders(headers);
			// setEntries(entries);
			setApps(apps);
		};

		reader.readAsText(file, 'UTF-8');
	}


	return(<div>
		<label>CSV文件</label>
		<input type="file" accept=".csv" onChange={importCSV} />

		<div className='app-list'>
			{ apps.map((app, idx) => (
				<div className="app-card" key={idx}>
						<span>{app.name}</span>
						<span>{app.version}</span>
				</div>
			))}
		</div>
	</div>)
};

export default Applications;
