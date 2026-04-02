<template>
  <div class="container">
    <h1>✨ Vue 最小实例 (npm 运行)</h1>

    <!-- 双向绑定演示 -->
    <div class="input-area">
      <label>📝 输入文字：</label>
      <input v-model="message" type="text" placeholder="试试修改我" autofocus>
    </div>

    <div class="display-area">
      <p>⚡ 实时响应输出：</p>
      <div class="message">{{ message || '等待输入...' }}</div>
    </div>

    <div class="note">
      💡 数据驱动视图 · 完全响应式
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      message: '你好，Vue！🚀'
    }
  }
}
</script>

<style scoped>
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}
.container {
  max-width: 600px;
  margin: 3rem auto;
  padding: 2rem;
  font-family: 'Segoe UI', Roboto, sans-serif;
  background: #ffffffdd;
  backdrop-filter: blur(2px);
  border-radius: 2rem;
  box-shadow: 0 20px 35px -12px rgba(0, 0, 0, 0.15);
  border: 1px solid rgba(255,255,255,0.6);
  background: white;
}
h1 {
  font-size: 1.8rem;
  background: linear-gradient(135deg, #1e2a3a, #2c5f7a);
  background-clip: text;
  -webkit-background-clip: text;
  color: transparent;
  margin-bottom: 1.8rem;
  text-align: center;
}
.input-area {
  margin-bottom: 1.8rem;
}
label {
  font-weight: 600;
  color: #2c3e4e;
  display: block;
  margin-bottom: 0.5rem;
}
input {
  width: 100%;
  padding: 0.8rem 1rem;
  font-size: 1rem;
  border: 2px solid #e2e8f0;
  border-radius: 2rem;
  outline: none;
  transition: 0.2s;
  font-family: inherit;
}
input:focus {
  border-color: #42b883;
  box-shadow: 0 0 0 3px rgba(66, 184, 131, 0.2);
}
.display-area {
  background: #f8fafc;
  border-radius: 1.5rem;
  padding: 1.2rem;
  text-align: center;
  margin-top: 1rem;
}
.display-area p {
  font-weight: 500;
  color: #4a6a7e;
  margin-bottom: 0.5rem;
}
.message {
  font-size: 1.8rem;
  font-weight: 600;
  color: #1e4663;
  word-break: break-word;
  background: linear-gradient(120deg, #2c5f7a, #1d3e53);
  background-clip: text;
  -webkit-background-clip: text;
  color: transparent;
  padding: 0.5rem;
}
.note {
  margin-top: 1.5rem;
  font-size: 0.75rem;
  text-align: center;
  color: #6b8a9c;
  border-top: 1px solid #e2edf2;
  padding-top: 1rem;
}
</style>
