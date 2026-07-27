渲染(render):

* 性能
* 交互
* 索引

关键阶段
* 构建模型: DOM，CSSOM
* 渲染合成:
* 位置布局:
* 图形绘制:
* 图层合成:


渲染模型
* CSR (客户端渲染)
* SSR (服务端渲染)
* ISR (增量静态再生)
* DPR (分布式持久渲染)
* SSG (静态站点生成)

指标
* FCP (First Contentful Paint，首次内容绘制)
* LCP (Largest Contentful Paint，最大内容绘制)
* TTI (Time to Interactive，可交互时间)


模型生成 - DOM(document)
-------------
* 字节 -> 字符 -> 字元 -> 节点 -> 树
* DOMContentLoaded事件
* 遇到script标签， 暂停并执行(可以async，defer)

关键渲染路径(CRP):
* CSSOM 构建：强制阻塞渲染



HTML
HTML + JS
HTML + CSS
HTML + CSS +JS


DOM解析器
COM解析器

* 首先解析DOM树，遇到link标签，委托COM解析器进行异步工作。




DOM与COM模型建立完毕后，开始生成渲染树


交互进程:
渲染进程: 交互任务、计算任务
绘制进程:
网络进程:
插件进程:
