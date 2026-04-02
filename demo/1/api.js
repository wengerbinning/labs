
/**
 * 生成8位不重复的随机十六进制字符串
 * 方法一：使用时间戳 + 随机数（适用于大多数场景）
 */
function genFeatureId_v1() {
	const timestamp= Date.now().toString(16);
	const random = Math.floor(Math.random() * 0x1000000).toString(16).padStart(6, '0');
	return (timestamp + random).slice(-8).toUpperCase();
}

/**
 * 生成8位不重复的随机十六进制字符串
 * 方法二：使用Crypto API（更随机，适合高安全性要求）
 */
function genFeatureId_v2() {
	if (window.crypto && window.crypto.getRandomValues) {
		const array = new Uint8Array(4);
		window.crypto.getRandomValues(array);
		return Array.from(array)
			.map(b => b.toString(16).padStart(2, '0'))
			.join('').toUpperCase();
	} else {
		return genFeatureId_v1();
	}
}

// 使用示例
console.log('方法一生成:', genFeatureId_v1()); // 例如：A3F9B2C1
// console.log('方法二生成:', genFeatureId_v2()); // 例如：4E8D1F2A
