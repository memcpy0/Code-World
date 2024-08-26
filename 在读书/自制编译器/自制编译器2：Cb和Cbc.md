## Cb语言概要
C♭ 是 C 语言的简化版，省略了 C 语言中琐碎部分以及难以实现、容易混淆的功能，实现起来条理更加清晰。虽然如此，C♭ 仍保留了包括指针等在内的 C 语言的重要部分。因此，**理解 了 C♭ 的编译过程，也就相当于理解了 C 程序的编译过程**。

目的让读者理解“在现有的 OS 上，现有的程序是如何编译及运行的”。那些有着诸多不切实际的限制，仅能作为书中示例的“玩具”语言，对其进行编译丝毫没有意义。

删减的功能：
- 预处理器：认真制作预处理器花费时间太多。因为省略了预处理器，所以 C♭ 无法使用 `#define` 和 `#include` ，无法导入类型定义和函数原型。为了解决该问题，C♭ 使用了与 Java 类似的 `import` 关键字。
- 浮点数：页数限制
- `enum` ：C 语言的 enum 和生成名称连续的 int 型变量的功能本质上无太大区别
- 结构体（struct）的位域（bit field） ：考虑到编译器的复杂度，才删除了类似的使用频率不高或非核心的功能
- 结构体和联合体（union）的赋值：考虑到编译器的复杂度
- 结构体和联合体的返回值：考虑到编译器的复杂度
- 逗号表达式
- `const` 和 `volatile` ：比较常用；但因为 cbc 几乎不进行优化，所以 volatile 本 身并没有太大意义。const 可以有条件地用数字字面量和字符串字面量来实现
- `auto` 和 `register` ：使用频率低，而且并非必要

C♭ 提供了 `import` 关键字。`import` 的语法如下所示：
```c
import stdio; 
import sys.params;
```
导入文件类似于 C 语言中的头文件，记载了其他程序库中的**函数、变量以及类型的定义**。 cbc 中有 stdio.hb、stdlib.hb、sys/params.hb 等导入文件，当然也可以自己编写**导入文件**。

导入文件的ID是去掉文件名后的 `.hb` 并用 `.` 取代路径标识中的 `\` 得到的。如 `sys/params.hb` 的ID是 `sys.params` 。

导入文件的规范：
代码清单 2.2 导入文件 stdio.hb
```c
// stdio.hb

import stddef;    // for NULL and size_t
import stdarg;

typedef unsigned long FILE; // dummy
// 变量声明
extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;
// 函数声明
extern FILE* fopen(char* path, char* mode);
extern FILE* fdopen(int fd, char* mode);
extern FILE* freopen(char* path, char* mode, FILE* stream);
extern int fclose(FILE* stream);
...
```
只有下面这些声明能记录在导入文件中：
- `typedef`
 - 函数声明
 - 变量声明（不能包含初始值的定义）
 - 常量定义（这里必须有初始值）
 - 结构体定义
 - 联合体定义

**函数**及**变量**的声明必须添加关键字 `extern` 。并且在 C♭ 中，函数返回值的类型、参数的类 型、**参数名**均不能省略。

## C♭编译器 cbc 的构成
==阅读有一定数量的代码时，首先要做的就是把握代码目录以及文件的构成==。这一节将对制作的 C♭ 编译器 cbc 的代码构成进行说明。
### cbc中的包
cbc采用Java标准的目录结构：
![image.png](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202407020159515.png)
从 `asm` 到 `utils` 的 11 个目录，各自对应着同名的包。也就是说，cbc 有 11 个包，所 有 cbc 的类都属于这 11 个包中的某一个。
![image.png](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202407020200218.png)

cbc 不直接在 net.loveruby 和 net.loveruby.cflat 下面放置类。

这些包中，`entity, type, ast, asm, ir` 归为数据相关（被操作类），`compiler, parser, sysdep, sysdep.x86` 归为处理类。**把握代码整体结构时最重要的包是 compiler 包，其中基本收录了 cbc 编译器前端的所有内容**。例如，编译器程序的入口函数 main 就定义在 compiler 包的 `Compiler` 类中。

### compiler 包中的类群
`compiler` 包中主要的类如下：
![image.png](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202407020214209.png)
`Compiler` 类是统管 cbc 的整体处理的类。编译器的入口函数 main 也在 `Compiler` 类中定义。

从 `Visitor` 类到 `TypeResolver` 类都是语义分析相关的类。关于这些类的作用将在第 9 章详细说明。

IRGenerator 是将抽象语法树转化为中间代码的类，详情请参考第 11 章。

### main函数的实现

