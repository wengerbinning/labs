import React, { useState } from 'react';
import './SourceFile.css';

interface File {
	name: string;
	path: string;
	lang: string;
	size: number;
	line: number;
};

const SourceFile: React.FC = () => {
	const [files, setFile] = useState<File[]>([
		{
			name: "README.md",
			path: ".",
			lang: 'markdown',
			size: 0,
			line: 0,
		},
		{
			name: "helloworld.c",
			path: ".",
			lang: "c",
			size: 0,
			line: 0,
		}
	]);

	return(<div>
	{ files && files.length === 0 ? (
		<span>Nothing</span>
	):(
		<div className='lists'>
			{ files.map((file, idx) => (
				<div className="list" key={idx}>
					<span>{file.name}</span>
					<span>{file.path}</span>
				</div>
			))}
		</div>
	)}
	</div>)
};

export default SourceFile;
