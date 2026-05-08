import React, { useState } from 'react';
import './SourceSymb.css';

interface Symb {
	name: string;
	desc: string;
	file: string;
	line: number;
};

const SourceSymb: React.FC = () => {
	const [symbs, setSymb] = useState<Symb[]>([{
		name: "main",
		desc: "This entry fucntion",
		file: "./helloworld.c",
		line: 2
	}, {
		name: "version",
		desc: "This is variable",
		file: "./helloworld.c",
		line: 1
	}]);

	return(<div>
	{symbs && symbs.length === 0 ? (
		<span>Nothing</span>
	): (
		<div className='list'>
			{ symbs.map((symb, idx) => (
				<div className="card" key={idx}>
					<span>{symb.name}</span>
					<span>{symb.file}</span>
				</div>
			))}
		</div>
	)}
	</div>)
};

export default SourceSymb;
