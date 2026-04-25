// react-papaparse

import React, { useState } from 'react';
import { useCSVReader } from 'react-papaparse';

const ImportCSV: React.FC = () => {
	const { CSVReader } = useCSVReader();
	const [data, setData] = useState<RowData[]>([]);

	return (
		<CSVReader onUploadAccepted={(results: any) => {
			setData(results.data);
		}}>
		{({ getRootProps, acceptedFile, ProgressBar, getRemoveFileProps }: any) => (
			<>
				<div {...getRootProps()} style={{ border: '1px dashed gray', padding: '20px', textAlign: 'center' }}>
					{acceptedFile ? acceptedFile.name : '拖拽或点击上传 CSV 文件'}
				</div>
				<ProgressBar />
				<button {...getRemoveFileProps()}>移除文件</button>
			</>
		)}
		</CSVReader>
	);
};
