<template> <div class="container">
	<div class="list-card">
		<div class="card-header">
			<h1>动态一维列表</h1>
			<div class="sub">📌 增·删·改·查 | 响应式数组驱动视图</div>
		</div>

		<!-- 新增栏位 -->
		<div class="add-panel">
			<div class="input-group">
				<input
					type="text"
					v-model="newItemText"
					placeholder="例如: 阅读源码 / 学习组合式API ..."
					@keyup.enter="addItem"
					autofocus
				/>
				<button class="btn-primary" @click="addItem"> <i>+</i> 添加项目 </button>
				<button class="btn-outline" @click="resetList" title="恢复默认列表"> 🔄 重置 </button>
			</div>
		</div>
	</div>

	<!-- 列表主体 -->
	<div class="list-container">
		<div class="list-header">
			<span class="badge">✨ 共 {{ totalCount }} 项</span>
			<button class="btn-warning" @click="clearAll" style="padding:0.3rem 1rem; font-size:0.75rem;"> 🗑️ 清空全部 </button>
		</div>

		<!-- 动态列表渲染 -->
		<ul class="list-ul" v-if="list.length">
			<li v-for="(item, idx) in list" :key="item.id" class="list-item">
				<div class="item-content">
					<span class="item-index">{{ idx + 1 }}</span>
					<span class="item-name">{{ item.name }}</span>
				</div>
				<div class="item-actions">
					<button class="icon-btn edit" @click="editItem(item.id)" title="编辑内容">编辑</button>
					<button class="icon-btn delete" @click="removeItem(item.id)" title="删除该项">🗑️ 删除</button>
				</div>
			</li>
		</ul>

		<!-- 空状态占位 -->
		<div v-else class="empty-state">
		🌱 列表空空如也，点击上方「添加项目」或「重置」开始~
		</div>

		<!-- 附加快捷说明（动态微交互） -->
		<div class="footer-actions">
			<span style="font-size:0.7rem; color:#6b8eab;">💡 支持回车添加 / 编辑内联修改</span>
		</div>
	</div>
</div> </template>

<script>
export default {
	data() {
		return {
			// 输入框新内容
			newItemText: '',
			// 一维列表核心数据 (每个元素包含唯一id和展示名称)
			list: [
				{ id: 1, name: '📖 学习Vue响应式原理' },
				{ id: 2, name: '🎯 动态渲染v-for列表' },
				{ id: 3, name: '🧩 实现增删改查交互' },
			]
		};
	},

	computed: {
		// 计算属性用于实时统计列表项目数量
		totalCount() {
			return this.list.length;
		}
	},

	methods: {
		// 添加新项目
		addItem() {
			const trimmedName = this.newItemText.trim();
			if (trimmedName === '') {
			alert('❓ 内容不能为空，请输入有效文字');
			return;
			}
			// 生成新id: 基于当前列表最大id + 1 (保证唯一且递增)
			const maxId = this.list.length > 0 ? Math.max(...this.list.map(item => item.id)) : 0;
			const newId = maxId + 1;
			this.list.push({
			id: newId,
			name: trimmedName
			});
			// 清空输入框，方便继续添加
			this.newItemText = '';
		},
		// 删除项目 (增加二次确认，防止误删)
		removeItem(id) {
			const targetItem = this.list.find(item => item.id === id);
			if (!targetItem) return;
			const confirmDelete = confirm(`确定要删除项目「${targetItem.name}」吗？`);
			if (confirmDelete) {
			// 使用filter生成新数组，触发响应式更新
			this.list = this.list.filter(item => item.id !== id);
			}
		},
		// 编辑项目 (使用原生prompt简单直接，体现动态修改能力)
		editItem(id) {
			const item = this.list.find(item => item.id === id);
			if (!item) return;
			let newName = prompt('✏️ 编辑项目名称', item.name);
			if (newName !== null && newName !== undefined) {
			newName = newName.trim();
			if (newName === '') {
				alert('名称不能为空，编辑未生效');
				return;
			}
			// 直接修改对象属性，Vue 3 可侦测深层变更
			item.name = newName;
			}
		},
		// 重置为初始列表 (完全替换数组，响应式更新)
		resetList() {
			const confirmed = confirm('重置将恢复为默认的三个示例项目，确定重置吗？');
			if (confirmed) {
			this.list = [
				{ id: 1, name: '📖 学习Vue响应式原理' },
				{ id: 2, name: '🎯 动态渲染v-for列表' },
				{ id: 3, name: '🧩 实现增删改查交互' },
			];
			// 重置同时清空输入栏，提升体验
			this.newItemText = '';
			}
		},
		// 清空所有列表项 (动态极致)
		clearAll() {
			if (this.list.length === 0) {
			alert('列表已经是空的啦～');
			return;
			}
			const confirmed = confirm('⚠️ 警告：将清空所有项目，无法撤销！确定清空吗？');
			if (confirmed) {
			this.list = [];
			this.newItemText = '';
			}
		}
	}
}
</script>


<style scoped>
.container {
	margin: 3rem auto;
	padding: 2rem;
	border: 1px solid rgba(255,255,255,0.6);
	border-radius: 2rem;
	box-shadow: 0 20px 35px -12px rgba(0, 0, 0, 0.15);

	min-width: 100vw;
	min-height: 100vh;

	font-family: 'Segoe UI', Roboto, sans-serif;

	background: #ffffffdd;
	background: white;
	backdrop-filter: blur(2px);
}

.input-area {
	margin-bottom: 1.8rem;
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
</style>
