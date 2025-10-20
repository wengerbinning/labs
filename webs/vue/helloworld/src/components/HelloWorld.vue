<script setup>






function parseData (name, data) {
	console.log(name);
	// console.log(data);
	let cnt = 0;

	data.split(/\r?\n/).forEach((each, idx) => {
		const line = each.trimStart();
		const key = line.split(" ");
		if (line.startsWith("#")) {
			return;
		}
		// console.log(`${idx++}: ${key[0]}`);

		switch (key[0]) {
		case 'config':
			cnt ++;
			console.log(`${cnt}: ${key[2]}`);
			break;
		}
	});

}


function handleFile (event) {
	const name = event.target.value;
	const reader = new FileReader();
	const file = event.target.files[0];

	reader.onload = function(event) {
		const text = event.target.result;
		parseData(name, text);
	};

	console.log(name);
	reader.readAsText(file, 'utf-8');
}
</script>

<template>
  <div>
	<input type="file" @input="handleFile"/>
  </div>
</template>

<style scoped>
.upload-box {
  width: 300px;
}
</style>
