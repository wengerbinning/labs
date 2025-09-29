#!/usr/bin/env node

const fs = require('fs');
const readline = require('readline');

const name = 'source.txt';
const mode = 'utf8';
// const chunk = 1 * 1024;
const chunk = 11;
let size = 0;
let len = 0;

fs.stat(name, (err, stats) => {
	if (err) throw err;
	size = stats.size;
	// console.log(`文件 ${name} 大小: ${stats.size} 字节`);
});

// const stream = fs.createReadStream(name, {encoding: mode, highWaterMark: chunk});
const stream = fs.createReadStream(name, {highWaterMark: chunk});

stream.on('data', chunk => {
	len += chunk.length;
	const hexStr = chunk.toString('hex');
	console.log('收到数据块:', chunk.length);
	console.log(hexStr);

	let buffer = []


	for (let i = 0; i < chunk.length; i++) {
		const b = chunk[i];
		// console.log(i, b);

		if ( 0x23 == b ) {
			while (chunk[i++] != 0x0a) {
				;
			}
			continue;
		} else if ( 0x30 <= b && b <= 0x39 ) {
			// console.log(i, b, b.toString(16).padStart(2, '0'), '#');
		} else if ( 0x41 <= b && b <= 0x4b ) {
			console.log(i.toString().padStart(2, '0'), b, b.toString(16).padStart(2, '0'));
		} else  if ( 0x0a == b ) {
			// console.log(i, b, b.toString(16).padStart(2, '0'), 'LF');
		}
	}


	// 在这里处理 chunk
});

stream.on('end', () => {
	console.log(`文件 ${name} 读取完毕, 文件大小 ${len}/${size} 字节`);
});

stream.on('error', err => {
	console.error('读取错误:', err);
});


