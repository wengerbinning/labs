这是一个用于测试与验证各种编程技术的实验平台.






## TODO Lists

### 2025-02-10 0000

实现一个展示版本信息的页面,信息包含:版本号,发布时间,版本说明,版本标签,版本问题列表,版本功能列表.

1. 实现版本信息展示: 默认按照版本轴展示
2. 实现版本信息管理

#### 第一阶段

基于Web技术栈实现版本信息展示

### 2025-02-10 0001

实现一个服务导航栏,用于展示当前设备提供的服务.


## Work Lists


## Notes

JTAG(Joint Test Action Group)
-----------------------------
* 电气特性
* 功能调试
* 在线编程

通过JTAG接口访问CPU的内部寄存器和CPU总线上的设备。

IEEE 1149.1

pins
---
* TCK - 输入时钟
* TMS - 测试模式
* TDI - 数据输入
* TDO - 数据输出
* RST - 复位测试

modules
-------
* TAP(Test Access Port)控制器（状态机）
* BSC(Boundary Scan Cell, 边界扫描单元)
* BSR(Boundary Scan Register, 边界扫描寄存器)
* 指令寄存器(IR)
* 数据寄存器

modes
* SAMPLE
* PRELOAD
* EXTEST
* BYPASS


BSC(Boundary Scan Cell, 边界扫描单元)
-----------------------------------

芯片每一个主要输入/输出信号都配备了BSC， BSC的集合被配置为一个BSR

* 正常模式
* 更新模式
* 捕捉模式
* 移位模式



EC:74:D7:6F:2D:B4


基础指令
* EXTEST
扩展指令






- <https://zhuanlan.zhihu.com/p/666604213>

