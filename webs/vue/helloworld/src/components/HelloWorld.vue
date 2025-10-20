<script setup>
import { ref } from 'vue'

const conf = ref([
  { id: 1, name: 'Alice' },
  { id: 2, name: 'Bob' },
  { id: 3, name: 'Charlie' }
])



function parseData (name, data) {
	console.log(name);
	// console.log(data);
	let cnt = 0;

	conf.value.length = 0;
	// conf.value.push({ id:0, name: "" });
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

			conf.value.push({ id:cnt, name: key[2] })
			break;
		}
	});

	console.log(conf);

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
	<div>
		<div v-if="conf && conf.length">
			<div v-for="(each, index) in conf" :key="each.id" class="card">
				<p> {{ each.name }}</p>
			</div>
		</div>
		<div v-else>
			<p>暂无有效数据</p>
		</div>
	</div>
</template>



<style scoped>
.upload-box {
  width: 300px;
}
</style>
