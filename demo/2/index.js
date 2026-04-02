    // ---------- 全局状态 ----------
    let rawRecords = [];        // 存储 { name, version } 对象数组（原始顺序，从csv提取）
    let nameColumnKey = null;   // 实际检测到的名称列名
    let versionColumnKey = null; // 实际检测到的版本列名

    // UI 状态
    let currentFilter = "";      // 搜索关键词
    let currentSort = "none";    // 'name-asc', 'name-desc', 'version-asc', 'version-desc', 'none'

    // DOM 元素
    const fileInput = document.getElementById('fileInput');
    const dropZone = document.getElementById('dropZone');
    const listContainer = document.getElementById('listContainer');
    const columnMappingBadge = document.getElementById('columnMappingBadge');
    const searchInput = document.getElementById('searchInput');
    const resetFilterBtn = document.getElementById('resetFilterBtn');
    const recordCounterSpan = document.getElementById('recordCounter');
    const downloadSampleBtn = document.getElementById('downloadSampleBtn');

    // 辅助函数: 智能识别名称列和版本列 (基于表头数组)
    function detectNameAndVersionColumns(headers) {
        if (!headers || headers.length === 0) return { nameCol: null, versionCol: null };

        // 定义候选词（中英文常见，忽略大小写）
        const nameCandidates = ['名称', 'name', '产品名称', '商品名称', '标题', 'title', 'product', '名字', '软件名称'];
        const versionCandidates = ['版本', 'version', 'ver', '版本号', 'version number', 'release', 'edition'];

        let detectedName = null;
        let detectedVersion = null;

        // 转为小写比较提高容错
        const lowerHeaders = headers.map(h => (h || "").toString().toLowerCase().trim());

        // 优先精确匹配（原始大小写友好但用includes匹配候选）
        for (let i = 0; i < headers.length; i++) {
            const original = headers[i].toString().trim();
            const lowerVal = lowerHeaders[i];
            if (!detectedName) {
                if (nameCandidates.some(candidate => lowerVal === candidate.toLowerCase() || original === candidate)) {
                    detectedName = headers[i];
                }
            }
            if (!detectedVersion) {
                if (versionCandidates.some(candidate => lowerVal === candidate.toLowerCase() || original === candidate)) {
                    detectedVersion = headers[i];
                }
            }
        }
        // 若未精确命中，尝试包含匹配 (例如 "项目名称" 包含 "名称")
        if (!detectedName) {
            for (let i = 0; i < headers.length; i++) {
                const lowerVal = lowerHeaders[i];
                if (nameCandidates.some(cand => lowerVal.includes(cand.toLowerCase()) || cand.toLowerCase().includes(lowerVal))) {
                    detectedName = headers[i];
                    break;
                }
            }
        }
        if (!detectedVersion) {
            for (let i = 0; i < headers.length; i++) {
                const lowerVal = lowerHeaders[i];
                if (versionCandidates.some(cand => lowerVal.includes(cand.toLowerCase()) || cand.toLowerCase().includes(lowerVal))) {
                    detectedVersion = headers[i];
                    break;
                }
            }
        }
        // 兜底：如果还没找到名称，使用第一个列；版本使用第二个列，但会提示警告
        if (!detectedName && headers.length > 0) {
            detectedName = headers[0];
            console.warn("未明确识别名称列，使用第一列作为名称");
        }
        if (!detectedVersion && headers.length > 1) {
            detectedVersion = headers[1];
            console.warn("未明确识别版本列，使用第二列作为版本");
        } else if (!detectedVersion && headers.length === 1 && detectedName) {
            detectedVersion = headers[0]; // 极端情况只有一个列，同时当作版本
        }
        return { nameCol: detectedName, versionCol: detectedVersion };
    }

    // 从解析后的数据中提取 name & version 记录
    function extractRecordsFromData(dataArray, nameCol, versionCol) {
        if (!dataArray || !Array.isArray(dataArray)) return [];
        const records = [];
        for (let idx = 0; idx < dataArray.length; idx++) {
            const row = dataArray[idx];
            if (!row || typeof row !== 'object') continue;
            let nameVal = nameCol ? (row[nameCol] !== undefined && row[nameCol] !== null ? String(row[nameCol]).trim() : "") : "";
            let versionVal = versionCol ? (row[versionCol] !== undefined && row[versionCol] !== null ? String(row[versionCol]).trim() : "") : "";
            // 空值占位显示
            if (nameVal === "") nameVal = "(未命名)";
            if (versionVal === "") versionVal = "(无版本)";
            records.push({ name: nameVal, version: versionVal, rawIndex: idx });
        }
        return records;
    }

    // 渲染列表 (根据筛选和排序)
    function renderList() {
        if (!rawRecords.length) {
            listContainer.innerHTML = `<div class="empty-state">📭 暂无数据，请上传 CSV 文件 (需包含名称/版本列)</div>`;
            recordCounterSpan.innerText = `0 条记录`;
            return;
        }

        // 1. 筛选
        let filtered = [...rawRecords];
        if (currentFilter.trim() !== "") {
            const keyword = currentFilter.trim().toLowerCase();
            filtered = filtered.filter(record =>
                record.name.toLowerCase().includes(keyword) ||
                record.version.toLowerCase().includes(keyword)
            );
        }

        // 2. 排序
        if (currentSort !== "none") {
            filtered.sort((a, b) => {
                if (currentSort === "name-asc") return a.name.localeCompare(b.name, 'zh');
                if (currentSort === "name-desc") return b.name.localeCompare(a.name, 'zh');
                if (currentSort === "version-asc") return a.version.localeCompare(b.version, undefined, { numeric: true, sensitivity: 'base' });
                if (currentSort === "version-desc") return b.version.localeCompare(a.version, undefined, { numeric: true, sensitivity: 'base' });
                return 0;
            });
        }

        // 更新计数
        recordCounterSpan.innerText = `${filtered.length} / ${rawRecords.length} 条记录`;

        if (filtered.length === 0) {
            listContainer.innerHTML = `<div class="empty-state">🔎 没有匹配 “${escapeHtml(currentFilter)}” 的记录，试试其他关键词</div>`;
            return;
        }

        // 生成卡片HTML
        let cardsHtml = "";
        for (let rec of filtered) {
            cardsHtml += `
                <div class="record-card">
                    <div class="record-name">${escapeHtml(rec.name)}</div>
                    <div class="record-version">
                        <span>📌 版本 ·</span>
                        <strong>${escapeHtml(rec.version)}</strong>
                    </div>
                </div>
            `;
        }
        listContainer.innerHTML = cardsHtml;
    }

    // 简单的防XSS
    function escapeHtml(str) {
        if (!str) return '';
        return str.replace(/[&<>]/g, function(m) {
            if (m === '&') return '&amp;';
            if (m === '<') return '&lt;';
            if (m === '>') return '&gt;';
            return m;
        }).replace(/[\uD800-\uDBFF][\uDC00-\uDFFF]/g, function(c) {
            return c;
        });
    }

    // 更新UI上显示的列映射信息
    function updateMappingBadge() {
        if (nameColumnKey && versionColumnKey) {
            columnMappingBadge.innerHTML = `✅ 识别列: 名称 = 「${escapeHtml(nameColumnKey)}」 &nbsp;|&nbsp; 版本 = 「${escapeHtml(versionColumnKey)}」`;
            columnMappingBadge.style.background = "#e0f2fe";
        } else if (nameColumnKey && !versionColumnKey) {
            columnMappingBadge.innerHTML = `⚠️ 仅识别名称列: 「${escapeHtml(nameColumnKey)}」, 版本列缺失, 请确保CSV包含版本信息`;
        } else if (!nameColumnKey && versionColumnKey) {
            columnMappingBadge.innerHTML = `⚠️ 仅识别版本列: 「${escapeHtml(versionColumnKey)}」, 名称列缺失`;
        } else {
            columnMappingBadge.innerHTML = `❌ 未检测到有效的名称/版本列，请检查CSV表头`;
        }
    }

    // 重置所有筛选排序并刷新
    function resetFiltersAndSort() {
        currentFilter = "";
        currentSort = "none";
        if (searchInput) searchInput.value = "";
        // 清除按钮高亮样式
        document.querySelectorAll('.sort-btn').forEach(btn => btn.classList.remove('active'));
        renderList();
    }

    // 处理CSV文件 (核心)
    function processCSVFile(file) {
        if (!file) return;
        // 重置之前的筛选状态，保持视图干净
        resetFiltersAndSort();

        Papa.parse(file, {
            header: true,
            skipEmptyLines: true,
            encoding: "UTF-8",
            complete: function(results) {
                if (results.errors && results.errors.length > 0) {
                    console.warn("解析警告:", results.errors);
                }
                const data = results.data;
                const metaFields = results.meta.fields;
                if (!data || data.length === 0 || !metaFields || metaFields.length === 0) {
                    listContainer.innerHTML = `<div class="error-msg">❌ CSV 文件无有效数据或表头为空，请检查文件格式</div>`;
                    rawRecords = [];
                    nameColumnKey = null;
                    versionColumnKey = null;
                    updateMappingBadge();
                    recordCounterSpan.innerText = `0 条记录`;
                    return;
                }

                // 智能识别名称列和版本列
                const { nameCol, versionCol } = detectNameAndVersionColumns(metaFields);
                nameColumnKey = nameCol;
                versionColumnKey = versionCol;
                updateMappingBadge();

                if (!nameColumnKey && !versionColumnKey) {
                    listContainer.innerHTML = `<div class="error-msg">⚠️ 无法自动识别「名称」和「版本」列，请确保CSV表头包含 “名称/name” 及 “版本/version” 关键词</div>`;
                    rawRecords = [];
                    renderList();
                    return;
                }

                // 提取记录
                const extracted = extractRecordsFromData(data, nameColumnKey, versionColumnKey);
                if (extracted.length === 0) {
                    listContainer.innerHTML = `<div class="error-msg">⚠️ 没有提取到任何数据行，请确认CSV内容非空</div>`;
                    rawRecords = [];
                    renderList();
                    return;
                }

                rawRecords = extracted;
                // 重新应用当前筛选（刚重置为空），重新渲染
                renderList();
            },
            error: function(err) {
                console.error("解析错误:", err);
                listContainer.innerHTML = `<div class="error-msg">❌ 文件解析失败: ${err.message || "未知错误"}</div>`;
                rawRecords = [];
                renderList();
            }
        });
    }

    // 处理文件上传 (从input或拖拽)
    function handleFile(file) {
        if (!file) return;
        // 检查文件类型
        if (file.type && !file.type.includes('csv') && !file.name.endsWith('.csv')) {
            listContainer.innerHTML = `<div class="error-msg">📛 请上传 .csv 格式文件</div>`;
            return;
        }
        processCSVFile(file);
    }

    // ---------- 事件绑定 ----------
    // 点击上传
    fileInput.addEventListener('change', (e) => {
        if (e.target.files && e.target.files.length > 0) {
            handleFile(e.target.files[0]);
        }
        fileInput.value = ''; // 允许重复上传同一文件
    });

    // 拖拽区域逻辑
    ['dragenter', 'dragover', 'dragleave', 'drop'].forEach(eventName => {
        dropZone.addEventListener(eventName, preventDefaults, false);
    });
    function preventDefaults(e) {
        e.preventDefault();
        e.stopPropagation();
    }
    ['dragenter', 'dragover'].forEach(eventName => {
        dropZone.addEventListener(eventName, () => {
            dropZone.classList.add('drag-over');
        });
    });
    ['dragleave', 'drop'].forEach(eventName => {
        dropZone.addEventListener(eventName, () => {
            dropZone.classList.remove('drag-over');
        });
    });
    dropZone.addEventListener('drop', (e) => {
        const dt = e.dataTransfer;
        const files = dt.files;
        if (files && files.length > 0) {
            handleFile(files[0]);
        }
    });
    // 点击drop区域任意位置调起文件选择 (可选，但为了交互，允许点击区域内的隐藏input不方便，我们直接让点击dropZone触发fileInput点击)
    dropZone.addEventListener('click', (e) => {
        // 避免点到内部label重复触发
        if (e.target.tagName !== 'LABEL' && !e.target.closest('.file-input-label')) {
            fileInput.click();
        }
    });

    // 搜索框事件
    searchInput.addEventListener('input', (e) => {
        currentFilter = e.target.value;
        renderList();
    });

    // 重置筛选
    resetFilterBtn.addEventListener('click', () => {
        resetFiltersAndSort();
    });

    // 排序按钮事件
    const sortBtns = document.querySelectorAll('.sort-btn[data-sort]');
    sortBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            const sortValue = btn.getAttribute('data-sort');
            if (sortValue === 'name-asc') currentSort = 'name-asc';
            else if (sortValue === 'name-desc') currentSort = 'name-desc';
            else if (sortValue === 'version-asc') currentSort = 'version-asc';
            else if (sortValue === 'version-desc') currentSort = 'version-desc';
            // 高亮当前排序按钮
            sortBtns.forEach(b => b.classList.remove('active'));
            btn.classList.add('active');
            renderList();
        });
    });

    // 下载示例CSV
    function downloadSampleCSV() {
        const sampleHeaders = ["产品名称", "版本号", "描述(额外列)"];
        const sampleRows = [
            ["Vue 生态系统工具", "3.4.21", "渐进式JS框架"],
            ["React 开发库", "18.2.0", "UI库"],
            ["Python 数据分析包 Pandas", "2.1.4", "数据处理"],
            ["Docker 引擎", "24.0.7", "容器化平台"],
            ["Node.js 运行时", "20.11.0", "JS后端"],
            ["Git 版本控制", "2.43.0", "代码管理"],
        ];
        let csvContent = sampleHeaders.join(",") + "\n";
        sampleRows.forEach(row => {
            // 简单转义逗号/引号, 但示例中不包含特殊字符
            const escapedRow = row.map(cell => `"${cell.replace(/"/g, '""')}"`).join(",");
            csvContent += escapedRow + "\n";
        });
        const blob = new Blob(["\uFEFF" + csvContent], { type: "text/csv;charset=utf-8;" });
        const link = document.createElement("a");
        const url = URL.createObjectURL(blob);
        link.href = url;
        link.setAttribute("download", "sample_products.csv");
        document.body.appendChild(link);
        link.click();
        document.body.removeChild(link);
        URL.revokeObjectURL(url);
    }
    downloadSampleBtn.addEventListener('click', downloadSampleCSV);

    // 初始化一些空状态
    rawRecords = [];
    renderList();
