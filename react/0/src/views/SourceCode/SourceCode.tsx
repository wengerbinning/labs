
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
		{
			name: "binutil",
			desc: "This is Basic Toolkit",
			stat: "develop",
			attr: ["fork"],
			vers: ["1.0.0"]
		},
		{
			name: "linux",
			desc: "This is Linux Kernel",
			stat: "develop",
			attr: ["fork"],
			vers: ["1.0.0"]
		}
	]);

	return(<div>
	{ codes && codes.length === 0 ? (
		<span>Nothing</span>
	):(
		<div className='cards'>
			{ codes.map((code, idx) => (
				<div className="card" key={idx}>
					<span>{code.name}</span>
					<div className='card-detail'>
						<span>{code.desc}</span>
					</div>
					<span>{code.vers[0]}</span>
				</div>

			))}
		</div>
	)}
	</div>)
};

export default SourceCode;
