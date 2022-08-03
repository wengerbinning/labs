



#### ANSI C

ANSI于1989年发布了标准(ANSI X3.159-1989), 该版本被称为为ANSI C；后来为区分C90, 该版本也被称为C89.

#### C90

ISO/IEC仅修改C89的格式后发布标准(ISO/IEC 9899:1990), 该版本被称为C90, 实质与C89属于同一标准。

#### C95

ISO/IEC于1995年发布了一个基于C90的修订标准(ISO/IEC 9899:1990/AMD1:1995), 被称为C95, 主要修正一些错误
与优化：

* 标准库中多字节与宽字节的支持， 引入了wchar.h与wctype.h以及多字节IO；
* 在语言中添加了有向图；
* 用于操作符代替规范的标准规范宏：`&&`等；
* 引入标准版本的宏`__STDC_VERSION_`；

除此之外还在1994发布技术更正(ISO/IEC 9899:1990/Cor 1:1994, TCOR1), 1996年发布技术更正
(ISO/IEC 9899:1990/Cor 2:1996)

#### C99

在2000年， ANSI采用新标准(ISO/IEC 9899:1999), 被称为C99, 在之前的基础上扩充了

* 引入新的内置数据类型：`long long`、`_Bool`、`_Complex`、`_Imaginary`;
* 新增语言特性：静态数组下标(Static Array Indices)、指定的初始化(Designated Initializers)、
  复合字面量(Compound Literals)、变长数组(Variable length arrays)、灵活的数组成员(Flexible  Array Members)、
  可变的宏(Variadic Macros)、restrict；
* 增加新的头文件:`stdint.h`、`tgmath.h`、`fenv.h`、`complex.h`;
* 改进了几个C++特性的兼容性：内联函数(inline functions)、单行注释`//`、混合声明与代码(Mixing Declarations and code)、标识符中的通用字符名(Univeral character name in identifiers);
* 移除一些C89的危险特性：隐式函数声明和隐式int.

C99标准存在一下技术更正：[ISO/IEC 9899:1999/Cor 1:2001(E)]、[ISO/IEC 9899:1999/Cor 2:2004(E)]、[ISO/IEC 9899:1999/Cor 3:2007(E)]

#### C11

在2011年12月， 新的标准(ISO/IEC 9899:2011)发布， 在该标准中：

* 支持Unicode；
* 引入`_Gerenic`关键字的类型泛型表达式；
* 新增一个跨平台的多线程API；
* 