function func() {
	// 获取三个按钮和三个面板
	const tab1 = document.getElementById('tab-1');
	const tab2 = document.getElementById('tab-2');
	const tab3 = document.getElementById('tab-3');
	const pnl1 = document.getElementById('pnl-1');
	const pnl2 = document.getElementById('pnl-2');
	const pnl3 = document.getElementById('pnl-3');

	// 将所有tab和panel组合成数组方便管理
	const tabs = [tab1, tab2, tab3];
	const pnls = [pnl1, pnl2, pnl3];

	// 封装切换函数
	function switchTab(id) {
		// 根据选中的按钮id找到索引
		let index = 0;
		if (id === 'tab-2') 
			index = 1;
		else if (id === 'tab-3') 
			index = 2;
		
		// 1. 更新tab按钮 active 样式 + aria-selected
		tabs.forEach((tab, i) => {
			if (i === index) {
				tab.classList.add('active');
				tab.setAttribute('aria-selected', 'true');
			} else {
				tab.classList.remove('active');
				tab.setAttribute('aria-selected', 'false');
			}
		});

		// 2. 更新面板显示：移除所有active-panel，再给对应面板添加active
		pnls.forEach((pnl, i) => {
			if (i === index) {
				pnl.classList.add('active');
			} else {
				pnl.classList.remove('active');
			}
		});
	}

	switchTab('tab-1');

	// 为每个按钮绑定点击事件
	tab1.addEventListener('click', function(e) {
		switchTab('tab-1');
	});
	tab2.addEventListener('click', function(e) {
		switchTab('tab-2');
	});
	tab3.addEventListener('click', function(e) {
		switchTab('tab-3');
	});
}

// 可选：增加键盘可访问性——用左右箭头轻量提示（非必须，这里简单增强用户习惯）
// 默认状态已经是第一个打开，无额外动作
func();
