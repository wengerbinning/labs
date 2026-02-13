 // 初始化代码高亮
 document.addEventListener('DOMContentLoaded', () => {
	hljs.highlightAll();
  });

  // 移动端侧边栏切换
  const menuToggle = document.getElementById('menuToggle');
  const sidebar = document.getElementById('sidebar');

  if (menuToggle) {
	menuToggle.addEventListener('click', () => {
	  sidebar.classList.toggle('open');
	});
  }

  // 自动调整文本框高度
  const textarea = document.querySelector('textarea');
  if (textarea) {
	textarea.addEventListener('input', function() {
	  this.style.height = 'auto';
	  this.style.height = (this.scrollHeight) + 'px';
	});
  }

  // 发送消息（演示用）
  const sendButton = document.querySelector('.send');
  if (sendButton) {
	sendButton.addEventListener('click', () => {
	  const input = document.querySelector('textarea');
	  if (input && input.value.trim()) {
		const messagesContainer = document.getElementById('messagesContainer');

		// 创建用户消息
		const userMessage = document.createElement('div');
		userMessage.className = 'message user';
		userMessage.innerHTML = `
		  <div class="message-avatar">
			<i class="fas fa-user"></i>
		  </div>
		  <div class="message-content">
			<div class="message-meta">
			  <span class="message-sender">你</span>
			  <span class="message-time">刚刚</span>
			</div>
			<div class="message-text">
			  <p>${input.value.replace(/</g, '&lt;').replace(/>/g, '&gt;')}</p>
			</div>
			<div class="message-actions">
			  <span><i class="far fa-copy"></i> 复制</span>
			  <span><i class="fas fa-pencil"></i> 编辑</span>
			</div>
		  </div>
		`;

		messagesContainer.appendChild(userMessage);

		// 模拟AI回复
		setTimeout(() => {
		  const aiMessage = document.createElement('div');
		  aiMessage.className = 'message ai';
		  aiMessage.innerHTML = `
			<div class="message-avatar">
			  <i class="fas fa-robot"></i>
			</div>
			<div class="message-content">
			  <div class="message-meta">
				<span class="message-sender">DeepSeek</span>
				<span class="message-time">刚刚</span>
			  </div>
			  <div class="message-text">
				<p>我收到了你的消息："${input.value}"</p>
				<p>这是一个演示界面，实际对话需要连接到 DeepSeek API。你可以访问 platform.deepseek.com 获取 API 密钥并集成真实对话能力。</p>
			  </div>
			  <div class="message-actions">
				<span><i class="far fa-copy"></i> 复制</span>
				<span><i class="far fa-thumbs-up"></i> 有用</span>
				<span><i class="fas fa-rotate-right"></i> 重新生成</span>
			  </div>
			</div>
		  `;

		  // 移除打字指示器
		  const typingIndicator = messagesContainer.querySelector('.message.ai:last-child .typing-indicator');
		  if (typingIndicator) {
			typingIndicator.closest('.message').remove();
		  }

		  messagesContainer.appendChild(aiMessage);
		  messagesContainer.scrollTop = messagesContainer.scrollHeight;
		  hljs.highlightAll();
		}, 1500);

		// 添加打字指示器
		const typingDiv = document.createElement('div');
		typingDiv.className = 'message ai';
		typingDiv.innerHTML = `
		  <div class="message-avatar">
			<i class="fas fa-robot"></i>
		  </div>
		  <div class="message-content">
			<div class="typing-indicator">
			  <span class="typing-dot"></span>
			  <span class="typing-dot"></span>
			  <span class="typing-dot"></span>
			</div>
		  </div>
		`;
		messagesContainer.appendChild(typingDiv);

		// 清空输入框
		input.value = '';
		input.style.height = 'auto';
		messagesContainer.scrollTop = messagesContainer.scrollHeight;
	  }
	});
  }

  // 快捷键发送（Ctrl+Enter）
  if (textarea) {
	textarea.addEventListener('keydown', (e) => {
	  if (e.key === 'Enter' && !e.shiftKey) {
		e.preventDefault();
		sendButton?.click();
	  }
	});
  }

  // 点击空白处关闭侧边栏（移动端）
  document.addEventListener('click', (e) => {
	if (window.innerWidth <= 768) {
	  const isSidebar = sidebar.contains(e.target);
	  const isMenuToggle = menuToggle.contains(e.target);

	  if (!isSidebar && !isMenuToggle && sidebar.classList.contains('open')) {
		sidebar.classList.remove('open');
	  }
	}
  });

  // 提示词点击填充
  document.querySelectorAll('.suggestion-chip').forEach(chip => {
	chip.addEventListener('click', function() {
	  const textarea = document.querySelector('textarea');
	  if (textarea) {
		textarea.value = this.textContent.replace(/^[^ ]+ /, '');
		textarea.dispatchEvent(new Event('input'));
		textarea.focus();
	  }
	});
  });
