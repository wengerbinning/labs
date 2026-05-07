
db_type_t
db_config_t
db_context_t

db_init
db_exit




* 配置(conf)
* 节点(node)
* 用户(user)





* 环境创建
* 环境清理
* 配置加载
* 服务连接
* 服务注销

* 数据用户创建
* 数据用户删除

* 数据库创建
* 数据库删除
* 数据库修改
* 数据库查询
* 数据库备份
* 数据库选择

* 数据表创建
* 数据表删除
* 数据表修改
* 数据表查询

* 数据项添加
* 数据项删除
* 数据项修改
* 数据项查询


注销 - unregister
创建 - created
销毁 - destroy
修改 - modify
更新 - update
搜索 - search
选择 - select
导入 - import
导出 - export
查询 - query
刷新 - flush
注册 - register
登陆 - login
退出 - quit

释放 - free





root - root root
user

admin - root root


权限管理()
用户管理(用户名称,用户密码,用户类型，用户权限)
数据管理(基础数据，用户数据)




gcc -o main -Iinclude test/main.c libs/database.c -L/usr/lib/ -lmariadb
