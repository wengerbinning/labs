import React, { useState } from 'react';

function FileImport() {
	const [base64, setBase64] = useState('');

	const handleFileChange = (event:React.ChangeEvent<HTMLInputElement>) => {
		const file = event.target.files?.[0];
		if (!file)
			return;

		const reader = new FileReader();
		reader.onload = (e) => {
			setBase64(e.target?.result as string);
		};
		reader.onerror = (error) => {
			console.error('文件读取失败', error);
		};
		reader.readAsDataURL(file); // 读取为 DataURL (base64)
	};

	return (
		<div>
			<input type="file" onChange={handleFileChange} />
			{base64 && (
				<div>
					<p>Base64 {base64?.length} 预览（前100字符）：</p>

					<pre>{base64.substring(0, 100)}...</pre>
					{/* <p>{base64}</p> */}

					<textarea
						rows={10}
						cols={80}
						value={base64}
						// readOnly
						wrap="soft"   // 自动换行，不插入换行符；也可以使用 "hard"
						style={{
							width: '100%',
							fontFamily: 'monospace',
							fontSize: '10px',
							resize: 'vertical',
							whiteSpace: 'pre-wrap', // 确保长文本自动换行
						}}
					/>



					{base64.startsWith('data:image') && (
						<img src={base64} alt="预览" style={{ maxWidth: '200px' }} />
					)}

				</div>
			)}
		</div>
	);
}

export default FileImport;
