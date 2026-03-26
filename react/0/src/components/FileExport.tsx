import React, { useState } from 'react';

function FileExport() {
	const [base64, setBase64] = useState('');

	const handleFileChange = (event:React.ChangeEvent<HTMLInputElement>) => {
    const file = event.target.files?.[0];
    if (!file) return;

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
					<p>Base64 预览（前100字符）：</p>
					<pre>{base64.substring(0, 100)}...</pre>
					{base64.startsWith('data:image') && (
						<img src={base64} alt="预览" style={{ maxWidth: '200px' }} />
					)}
				</div>
			)}
		</div>
	);
}

export default FileExport;
