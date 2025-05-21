#include <stdio.h>
#include <time.h>
#include <string.h>

int main (int argc, char *argv[]) {
    char str[128] = {0};

    memcpy(str, "12345i\03232", 127);
    printf("(%d[%d])%s\n", sizeof(str), strlen(str), str);

    return 0;
}


/*

 CPU



CPI(Clock cycle Per Intruction, 每条指令的平均时钟周期)
MIPS(Million Instructions Per Second, 每秒执行的百万指令数)
DMIPS(Dhrystone MIPS, 每秒执行的整数运算)
FLOPS(Floating Point Operations Pre Second, 每秒的浮现运算)
CoreMark


架构 -
核心 - 核心数
线程 - 每核线程
主频 - 外频 *倍频， 时钟频率
功耗
制程

缓存
内存
接口



* 时钟周期： 时钟频率的倒数
* 机器周期： CPU周期
* 指令周期：一条是指令执行完成的周期， 取指、译码、执行



指令取指：依据PC寄存器读取指定内存的指令
指令译码：解析读取的指令
指令执行：
内存读取：
内存写入：
指令更新：



 */