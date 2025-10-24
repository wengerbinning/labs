
功能说明
-------

该数据结构是基于标准库的bool类型的扩展， 由于在设置配置的场景中，设置开关类参数是存在修改场景，
因此会涉及是否修改，是否开启两类判断，而单独的bool类型需要每一次处理时需要读取新老配置并对比，
在参数传递中不太方便（例如在当前函数解析处理配置， 但是在上层函数需要知道开关状态并做相应处理，
此时再次解析配置比较繁琐）。因此， 通过该数据类型可以读取一次新老配置， 后续将开关参数的状态传递
到上层函数，上层函数直接处理数据。 该数据结构需要通过4个宏函数来使用：

1. `SWITCH_INIT_KEEP` - 初始化默认状态。
2. `SWITCH_SET_VALUE` - 设置新的状态。
3. `SWITCH_GET_VALUE` - 获取开关的状态。
4. `SWITCH_TEST_KEEP` - 检查开关状态是否发生变化。

注意事项：
* 该数据类型的基础是bool类型大小为1字节， 因此通过扩展该字节中最高7比特位来实现其他状态存储，
  因此不额外消耗内存。


* 设置参数

```c
switch_t enable;

SWITCH_INIT_KEEP(enable, false);

// ...

SWITCH_SET_VALUE(enable, true);
```

* 检查参数

```c
if (!SWITCH_TEST_KEEP(enable)) {
    bool val = SWITCH_GET_VALUE(enable);
    // TODO
}
```