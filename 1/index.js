(function() {
    const sidebar = document.getElementById('sidebar');
    const collapseBtn = document.getElementById('collapseBtn');
    const expandTrigger = document.getElementById('expandTrigger');  // 管理平台logo区域

    const menuItems = document.querySelectorAll('.menu-item[data-page]');
    const pages = document.querySelectorAll('.page');
    const titleIcon = document.getElementById('titleIcon');
    const titleText = document.getElementById('titleText');

    const pageConfig = {
        'dashboard': { icon: 'fa-chart-pie', name: '展示面板' },
        'user': { icon: 'fa-users', name: '用户管理' },
        'permission': { icon: 'fa-lock', name: '权限管理' },
        'doc': { icon: 'fa-file-alt', name: '文档管理' },
        'model': { icon: 'fa-brain', name: '模型管理' },
        'tech': { icon: 'fa-microchip', name: '技术管理' },
        'solution': { icon: 'fa-clipboard-list', name: '方案管理' },
        'feature': { icon: 'fa-puzzle-piece', name: '功能管理' },
        'template': { icon: 'fa-copy', name: '模板管理' },
        'product': { icon: 'fa-boxes', name: '产品管理' },
        'source': { icon: 'fa-code', name: '源码管理' },
        'issue': { icon: 'fa-exclamation-triangle', name: '问题管理' },
        'project': { icon: 'fa-tasks', name: '项目管理' }
    };

    // 展开/折叠函数
    function setSidebar(expand) {
        if (expand) {
            sidebar.classList.remove('collapsed');
            collapseBtn.title = '折叠菜单';
        } else {
            sidebar.classList.add('collapsed');
            collapseBtn.title = '展开菜单';
        }
    }

    function toggleSidebar() {
        sidebar.classList.toggle('collapsed');
        collapseBtn.title = sidebar.classList.contains('collapsed') ? '展开菜单' : '折叠菜单';
    }

    // 1. 折叠按钮：只负责折叠/展开（双向）
    collapseBtn.addEventListener('click', function(e) {
        e.stopPropagation();
        toggleSidebar();
    });

    // 2. 管理平台logo区域：点击时如果处于折叠状态则展开，否则不做操作（也可以不做任何事，但按需求“仅最上边的管理平台的icon展开菜单”，点击它只展开，不折叠）
    expandTrigger.addEventListener('click', function(e) {
        e.stopPropagation();
        if (sidebar.classList.contains('collapsed')) {
            setSidebar(true);  // 强制展开
        }
        // 如果已经展开，点击不做任何事 (不折叠)
    });

    // 页面切换逻辑 (不变)
    function deactivateAll() {
        menuItems.forEach(item => item.classList.remove('active'));
        pages.forEach(p => p.classList.remove('active-page'));
    }

    function switchPage(pageId) {
        deactivateAll();
        const menu = Array.from(menuItems).find(el => el.dataset.page === pageId);
        if (menu) menu.classList.add('active');
        const page = document.getElementById(pageId);
        if (page) page.classList.add('active-page');
        const cfg = pageConfig[pageId] || { icon: 'fa-circle', name: pageId };
        titleIcon.className = `fas ${cfg.icon}`;
        titleText.innerText = cfg.name;
    }

    menuItems.forEach(item => {
        item.addEventListener('click', function() {
            const pageId = this.dataset.page;
            if (pageId) switchPage(pageId);
        });
    });

    // 初始激活
    const activeMenuItem = document.querySelector('.menu-item.active');
    if (activeMenuItem) {
        const initPage = activeMenuItem.dataset.page;
        if (initPage) {
            const cfg = pageConfig[initPage] || { icon: 'fa-chart-pie', name: '展示面板' };
            titleIcon.className = `fas ${cfg.icon}`;
            titleText.innerText = cfg.name;
        }
    } else {
        switchPage('dashboard');
    }

    // 设置初始title
    collapseBtn.title = sidebar.classList.contains('collapsed') ? '展开菜单' : '折叠菜单';
})();
