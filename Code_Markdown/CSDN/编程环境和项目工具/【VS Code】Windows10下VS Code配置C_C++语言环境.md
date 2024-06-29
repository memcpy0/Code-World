@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。

由于DevC++太过落后和老旧，而C/C++是本人最经常使用的语言，于是下定决心转向VS Code。[现在有了VS Code编辑器](https://memcpy0.blog.csdn.net/article/details/117640795)，为了完成相关语言环境的配置，我们需要做只有下载编译套装、安装支持插件和书写配置文件三件事……听起来好多啊。

---
# 1. 下载安装MinGW-w64
**共通的第一步已经完成，即获取编辑器**。接下来要做的就是获取C/C++编译套装。常见的C/C++编译工具有GCC（`GNU Compiler Collection` ，即GNU编译器套件，GCC过去代表GNU C编译器，但是由于编译器支持除C之外的其他几种语言，它现在代表GNU编译器套件集合，[GCC官网在此处](https://gcc.gnu.org/)）、Clang（`C language family frontend for LLVM` ，提供兼容GCC的编译器驱动程序 `clang.exe` 和兼容MSVC的编译器驱动程序 `clang-cl.exe` ，[Clang官网在此](https://clang.llvm.org/)）、MSVC（`Microsoft C++` 编译器工具集）三巨头，个人觉得Clang的架构更优雅、优化更惊艳，然而姜还是老的辣……在对C++ 20的语核支持“比赛”中，GCC首先接近了终点——虽然MSVC一个月前宣布自己冲过了终点。
> MSVC compiler toolset is now C++20 feature-complete
> -- C++20 immediate functions & constinit support added in 16.10


<img src="https://img-blog.csdnimg.cn/20210607201748902.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">
<img src="https://img-blog.csdnimg.cn/2021060720175922.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">
<img src="https://img-blog.csdnimg.cn/20210607201804696.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">

此处要使用的GCC是[MinGW](http://www.mingw-w64.org/doku.php)（`Minimalist GNU for Windows`），MinGW是一个Windows特定头文件和GNU工具集的集合，是为了那些**不喜欢工作在Linux而留在Windows的人**提供的一套符合标准的GNU工作环境。MinGW实际上就是将GCC移植到了Windows平台上，并且包含了WIN32 API，可以将源代码编译为能在Windows中运行的可执行程序，还可以使用一些Linux下的开发工具。

MinGW目前可以分为MinGW和MinGW-w64，前者只能够编译32位可执行程序，已经停止更新，后者可以编译32位或64位可执行程序，是MinGW的标准版本。最终要下载的是**MinGW-w64**，[下载地址点此处](](http://www.mingw-w64.org/doku.php))。找到下面这个并下载：
<img src="https://img-blog.csdnimg.cn/20210607204658506.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="40%">

下载可能比较慢，耐心等待中……下载完成后还需要解压得到mingw64文件夹，将该文件夹拖到一个合适的、不包含中文地址的位置，如 `C:\Program Files` 。打开 `C:\Program Files\mingw64\bin` ，里面有许多可执行程序，`gcc.exe` 是C程序的编译器，`g++.exe` 是C++的编译器，`gdb.exe` 是调试程序的Debug工具：
<img src="https://img-blog.csdnimg.cn/20210608003302461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">

为了让程序能够访问这些可执行程序，需要将目录 `C:\Program Files\mingw64\bin` 添加到环境变量中。这里可以看到，本人已经将这一目录添加到系统变量的Path中（**用户变量只对当前用户有效，系统变量对所有用户有效**）：
<img src="https://img-blog.csdnimg.cn/20210608004438482.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">

现在进行验证，在CMD中输入 `gcc --version` ，可以看到如下版本信息，说明GCC已经安装成功：
<img src="https://img-blog.csdnimg.cn/20210608004725473.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">

现在电脑里面有了编辑器VS Code和编译套装GCC，它们下载自不同的网站，安装在不同的位置，彼此之间没有任何联系。而下面，我们将建立这一条道路，将两者关联起来。

--- 
# 2. 语言环境的文件结构
不过在此之前，一个题外但又很重要的事情是，我们需要辨析**语言学习环境和实际项目开发环境**这两者之间的区别。一个关键性的不同是二者的文件结构——或者说，组织和决定文件夹与文件之间的嵌套从属关系的方法。

在学习一门语言时，无论是C/C++/Haskell/Java/Python/Racket/Rust/Scala，我们多数情况下有的都是**单个文件**，了不起会有一些简单的小项目，涉及到**几个简单头文件和源文件的组合**。实际的项目开发就复杂多了，可能会有 `build, lib, makefile, CMakeLists` 等文件/文件夹，以及一系列的音频、视频和图片资源文件。由于此处介绍的是语言学习环境，我们的文件结构必须**方便新建单个语言文件、编译调试、管理查看等**，而不是学一点什么知识就要新建一个项目。

因为本人打算用VS Code统一组织和管理**各个语言学习环境**，所以新建了一个名为 `C:\CodeWorld` 的文件夹，将所有语言学习代码放在这里面。接着在里面新建了 `Code_C, Code_Cpp, Code_Haskell, Code_Java, Code_Python, Code_Rust` 等文件夹，在 `Code_Cpp` 中新建了 `Cpp_Single, Cpp_Multiple` 两个文件夹作为工作区文件夹。

其后开发实际项目时，也可以同样在 `CodeWorld` 文件夹下进行组织，比如从Github克隆项目时，建立一个 `Github` 文件夹；自己做项目时，建立一个 `Projects` 文件夹……

下面看一下语言学习环境下的工作区文件夹，以 `C_Single` 为例组织文件结构。**首先要有一个 `.vscode`  文件夹，存储相应的配置文件**。然后要有源文件，这些源文件可以**分类进不同的文件夹**，比如按照章节分或者按照类型分。我个人的分类比较细致，包括不同OJ的文件夹、不同技术书的文件夹等等，这里不一一列出。这些分类文件夹中都会有一系列的单个源文件，分别编译出的 `.exe` 文件一多，就容易杂乱，因此我们统一将这些可执行文件放到**每个分类文件夹下的 `bin` 文件夹**。一个示例如下：
<img src="https://img-blog.csdnimg.cn/20210624124401853.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="20%">
`C_Multiple` 类似于此，也有 **`.vscode` 文件夹**，有**分类文件夹**，但是每个分类文件夹中都是一系列的**小项目文件夹**，小项目文件夹中包含**一组源文件和编译出的 `.exe` 文件**，不需要单独的 `bin` 文件夹。一个示例如下：
<img src="https://img-blog.csdnimg.cn/20210624124755202.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="20%">

这就是一个合格的**语言学习环境**需要的**文件结构**，下面可以开始安装对应插件、书写相关配置文件。

---
# 3. 安装C/C++支持插件、书写配置文件
为什么要安装插件、配置文件呢？我们都知道，通过命令行使用 `gcc` 命令，能够编译生成可执行文件，但是每次都用命令行就太麻烦了。VS Code通过安装的插件和 `.vscode` 文件夹下的JSON配置文件（JSON文件的写法是VS Code官方规定的，可以看相关文档），帮我们实现了这些功能——一个是 `tasks.json` 文件，编译和执行就是我们想要VS Code执行的任务，因此 `tasks.json` 中要写两个 `task` ：`Build, Run` ，**构建** `Build` 任务包括预编译、编译 `compile` 、链接等过程。除了**构建**和**执行**外，还需要进行**调试** `Debug` ，这就需要通过另一个 `launch.json` 文件与调试插件来实现。注意，**`tasks.json` 中的任务 `Build` 应在 `launch.json` 中的 `Debug` 之前执行**。此外还有一个 `c_cpp_properties.json` 文件，用于实现某些关于C/C++语言特性的功能。

听上去很麻烦是吧？不过这还没到时候呢，本部分内容只是入门引导，用于C/C++语言学习。真要实打实开发C/C++项目，还需要先学GCC、Makefile、CMake，学习如何链接静态/动态库，如何引入头文件、解决文件冲突等等。这些知识又是进阶学习必不可少的，**不会系统构建就等于C/C++停留在入门水平，只能做些玩具项目**。

然而一个很现实的问题是，关于C/C++构建系统的资料太少了，C/C++书籍大多也不讲这些生态；Visual Studio那一套系统构建还和Unix/Linux环境不兼容……种种原因导致新手入门十分困难。可以预见的是，这些内容都学完、这些坑都踩完还要很久，不过这个时候回头再去用IDE，就会有一种了然于心的透彻感。
 
 
 
## 3.1 安装C/C++支持插件

在[官网](https://code.visualstudio.com/docs/editor/debugging#_launch-configurations)的 `Debugger extensions` 一栏中已经说明，VS Code有对 `Node.js` 运行时的内建支持，因此可以调试JavaScript、TypeScript和其他能够转写到JavaScript的编程语言。对于其他语言，就需要安装 `Debugger extensions` 了。首先下载C/C++的支持插件（一个技巧是：在扩展搜索栏中输入 `@category:"programming languages" @sort:installs ` ，就会列出所有语言相关的扩展）：
<img src="https://img-blog.csdnimg.cn/20210625062304728.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">

可以看到，这一插件支持的编译套件和平台包括 `Windows+MSVC/GCC and Mingw-w64, Windows Subsystem for Linux(WSL)+GCC, Linux+GCC, macOS+Clang` 几种，它的功能包括，可以仔细看一下这些链接：
<img src="https://img-blog.csdnimg.cn/20210625062909838.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="35%">

然后是 `Native Debug` 插件，方便调试：
<img src="https://img-blog.csdnimg.cn/20210713210144386.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">


## 3.2 书写C/C++单文件下配置文件，尝试构建、运行与调试
安装后重启VS Code，在文件->打开文件夹下，打开 `C_Single` 文件夹，此时VS Code中打开的**根目录**即是 `C_Single`（如果不额外设置JSON配置文件中的 `options` ，`C_Single` 作为工作区根目录，也是已执行程序的**当前工作目录**），由于是新建的文件夹，里面没有文件：
<img src="https://img-blog.csdnimg.cn/20210625064759287.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">

方法有很多种，这里我们新建一个名为 `t.c` 的文件，输入如下代码：
```cpp
#include <stdio.h>
int main() {
    printf("Hello VS Code!");
    return 0;
}
```
然后点击运行，弹出的界面如下：
<img src="https://img-blog.csdnimg.cn/20210625065536912.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="20%">

点击**启动调试**：
<img src="https://img-blog.csdnimg.cn/20210625065953722.png" width="40%">

需要选择 `C++ (GDB/LLDB)`（`Native Debug` 插件支持）：
<img src="https://img-blog.csdnimg.cn/20210625070155861.png" width="50%">

- 如果选择了**默认配置**，会生成 `.vscode` 文件夹和其中的 `launch.json` 文件用于调试，JSON文件内容如下，还需要自行修改：
	```json
	{
	    // 使用 IntelliSense 了解相关属性。 
	    // 悬停以查看现有属性的描述。
	    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
	    "version": "0.2.0",
	    "configurations": [
	        {
	            "name": "(gdb) 启动",
	            "type": "cppdbg",
	            "request": "launch",
	            "program": "输入程序名称，例如 ${workspaceFolder}/a.exe",
	            "args": [],
	            "stopAtEntry": false,
	            "cwd": "${fileDirname}",
	            "environment": [],
	            "externalConsole": false,
	            "MIMode": "gdb",
	            "miDebuggerPath": "/path/to/gdb",
	            "setupCommands": [
	                {
	                    "description": "为 gdb 启用整齐打印",
	                    "text": "-enable-pretty-printing",
	                    "ignoreFailures": true
	                }
	            ]
	        }
	    ]
	}
	```
- 如果选择了**生成和调试活动文件**，同样会生成 `.vscode` 文件夹和其中的`tasks.json, launch.json` 文件，JSON文件内容如下，此时不需要修改，可以直接进行**构建、运行和调试（先打断点）**：
```json
	//tasks.json
	{
	    "tasks": [
	        {
	            "type": "cppbuild",
	            "label": "C/C++: gcc.exe 生成活动文件",
	            "command": "C:\\Program Files\\mingw64\\bin\\gcc.exe",
	            "args": [
	                "-g",
	                "${file}",
	                "-o",
	                "${fileDirname}\\${fileBasenameNoExtension}.exe"
	            ],
	            "options": {
	                "cwd": "${fileDirname}"
	            },
	            "problemMatcher": [
	                "$gcc"
	            ],
	            "group": {
	                "kind": "build",
	                "isDefault": true
	            },
	            "detail": "调试器生成的任务。"
	        }
	    ],
	    "version": "2.0.0"
	}
	//launch.json
	{
	    // 使用 IntelliSense 了解相关属性。 
	    // 悬停以查看现有属性的描述。
	    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
	    "version": "0.2.0",
	    "configurations": [
	        {
	            "name": "gcc.exe - 生成和调试活动文件",
	            "type": "cppdbg",
	            "request": "launch",
	            "program": "${fileDirname}\\${fileBasenameNoExtension}.exe",
	            "args": [],
	            "stopAtEntry": false,
	            "cwd": "${fileDirname}",
	            "environment": [],
	            "externalConsole": false,
	            "MIMode": "gdb",
	            "miDebuggerPath": "C:\\Program Files\\mingw64\\bin\\gdb.exe",
	            "setupCommands": [
	                {
	                    "description": "为 gdb 启用整齐打印",
	                    "text": "-enable-pretty-printing",
	                    "ignoreFailures": true
	                }
	            ],
	            "preLaunchTask": "C/C++: gcc.exe 生成活动文件"
	        }
	    ]
	}
```

或者点击**以非调试模式运行**：
<img src="https://img-blog.csdnimg.cn/20210625065953722.png" width="50%">
还是需要选择 `C++ (GDB/LLDB)` ：
<img src="https://img-blog.csdnimg.cn/20210625070155861.png" width="50%">
- 如果选择了**默认配置**，会生成 `.vscode` 文件夹和其中的 `launch.json` 文件用于调试，JSON文件内容同上面的默认配置，还需要自行修改。 
- 如果选择了**生成和调试活动文件**，同样会生成 `.vscode` 文件夹和其中的 `tasks.json, launch.json` 文件，JSON文件内容同上，此时不需要修改，可以直接进行**构建、运行和调试（先打断点）**。

总的来说，**以非调试模式运行**感觉和**启动调试**没有什么区别，在存在程序源文件时**添加配置**也是一样的（**打开文件夹**但是**不存在源文件**时只会创建 `lauch.json` 文件；如果**没有打开文件夹**，添加配置会打开**全局配置 `settings.json` 文件**）。不过这三者都不太符合我们的预期文件结构。

我们可以自行在 `C_Single` 下创建 `.vscode` 文件夹以及其中的 `tasks.json, launch.json` 文件，文件具体内容如下，注释比较详细，一些路径可能需要修改，以符合上一节中提到的工作区文件结构：
```json
//tasks.json
{   
    //欲了解更多信息，请访问:
    //https://code.visualstudio.com/docs/editor/tasks#vscode
    "tasks": [
        { //这个大括号里是构建Build任务
            //label是任务的用户界面标签,可以更改
            "label": "build", 
            //type定义任务是被作为进程process运行(VS Code会把预定义变量和转义解析后全部传给command) 
            //还是在shell中作为命令运行(相当于先打开shell再输入命令,args会经过shell再解析一遍) 
            "type": "shell", //cppbuild 
            //command是要执行的(编译)命令,可以是外部程序或者shell命令
            //没有添加环境变量时,类似于在命令行执行"C:\\Program Files\\mingw64\\bin\\gcc.exe" 其他参数
            //添加了环境变量后,可以只写"gcc"
            "command": "gcc", //编译C++需要换成g++
            //args是调用此任务(即label所标明的任务)时需要传递给命令(即command指定的命令)的系列参数,以实现功能
            "args": [
                "-g",       //生成和调试相关的信息
                "${file}",  //承接上一步的-g,指定要编译的是当前正在编辑的文件,${file}包括绝对路径/文件名.文件后缀名
                "-o",       //指定输出文件的路径和名称
                "${fileDirname}\\bin\\${fileBasenameNoExtension}.exe", //承接上一步的-o,
                //让可执行文件输出到源码文件所在的文件夹下的bin文件夹内，并且让它的名字和源码文件相同
                //${fileDirname}是当前打开文件所在的文件夹绝对路径,不包括文件名
                //${fileBasename}是当前打开的文件名.后缀名,不包括路径
                //${fileBasenameNoExtension}是当前打开文件的文件名,不包括路径和后缀名
                //${fileExtname}是当前打开文件的后缀名
                //即 ${fileDirname}/${fileBasename}
                //                  |{fileBasenameNoExtension}.${fileExtname}|
                //   |--------${file}-------------|
                "-Wall",                //开启额外警告
                "-static-libgcc",       //静态链接libgcc
                //"-fexec-charset=GBK", //生成的程序使用GBK编码,不加此条可能导致Win下输出中文乱码
                "-std=c11",             //语言标准,根据需要进行修改,写C++时可以换成C++的语言标准,比如C++17
            ],  
            //group表示组,我们可以有很多的task,然后把它们放到一个组里面
            //此处定义此任务属于执行组,它支持"build"以将其添加到生成组,也支持"test"以将其添加到测试组
            "group": { 
                "kind": "build",  //表示这一组的任务属于执行组
                "isDefault": true //表示此任务是当前这组任务中的默认任务
            },
            //problemMatcher是要使用的问题匹配程序,
            //可以是一个字符串或一个问题匹配程序定义,也可以是一个字符串数组和多个问题匹配程序
            //捕捉编译时编译器在终端里显示的报错信息,将其显示在vscode的'问题'面板里
            "problemMatcher": [ 
                "$gcc"
            ], 
            //presentation配置用于显示任务输出并读取其输入的面板
            "presentation": {
                "echo": true,       //控制是否将执行的命令显示到终端面板中,默认值为true
                "reveal": "always", //控制运行任务的终端是否显示,默认设置为always,总是在此任务执行时显示终端;或者为silent/never
                "focus": false,     //控制终端面板是否获取焦点,默认值为false;如果设置为true,面板也会显示,但对编译来说设为true没有意义,因为运行时才涉及到输入
                "panel": "new"      //控制是否在任务间共享终端面板,同一个任务使用相同面板shared(会出现"任务被终端重用"),还是每次运行时新创建一个面板new
            },
            "detail": "调试器生成的任务。"
        },
        { //这个大括号里面是运行Run任务,一些设置与上面的构建任务性质相同
            "label": "run",
            "type": "shell",
            "dependsOn": "build", //此任务依赖的另一任务,因为必须先构建然后运行,所以执行此任务前必须先执行build任务
            "command": "${fileDirname}\\bin\\${fileBasenameNoExtension}.exe", //执行exe文件,只需要指定这个exe文件在哪里
            "group": {
                "kind": "test", //表示这一组是测试组,将run任务放在test组里方便我们用快捷键执行
                "isDefault": true //表示此任务是当前这组任务中的默认任务
            },
            "presentation": {
                "echo": true,
                "reveal": "always",
                "focus": true, //运行run任务后将焦点聚集到终端,方便进行输入
                "panel": "new"
            }
        }
    ],
    "version": "2.0.0"
}
```
```json
//launch.json
{
    //使用 IntelliSense 了解相关属性。 
    //悬停以查看现有属性的描述。
    //欲了解更多信息，请访问:
    //https://code.visualstudio.com/docs/editor/debugging#_launch-configurations
    "version": "0.2.0",
    "configurations": [ 
        { //这个大括号里是我们的调试Debug配置
            "name": "Debug",        //配置名称
            "type": "cppdbg",       //配置类型,cppdbg对应cpptools提供的调试功能,可以认为此处只能是cppdbg
            "request": "launch",    //请求配置类型,可以是启动launch或附加attach
            "program": "${fileDirname}\\bin\\${fileBasenameNoExtension}.exe", //将要进行调试的程序可执行文件的完整路径
            "args": [],             //程序调试时传递给程序的命令行参数,这里设为空即可
            "stopAtEntry": false,   //设为true时调试程序将暂停在程序入口处,相当于在main上打断点;如果传递了processId则不起任何作用
            "cwd": "${fileDirname}",//调试程序时的工作目录,此处为源码文件所在目录
            "environment": [],      //要添加到程序环境的环境变量,这里设为空即可
            "externalConsole": false, //为true时使用单独CMD窗口,跳出小黑框;设为false则用VS Code的内置终端,建议用内置终端
            "internalConsoleOptions": "neverOpen", //如果不设为neverOpen,调试时会跳到“调试控制台”选项卡,新手调试用不到
            "MIMode": "gdb",        //指定MIDebugEngine连接的控制台调试程序,gdb是minGW中的调试程序
            "miDebuggerPath": "C:\\Program Files\\mingw64\\bin\\gdb.exe", //MI调试程序(如gdb)的路径;如果未指定,将首先在路径中搜索调试程序
            "setupCommands": [ //为了安装基础调试程序而执行的一个或多个GDB/LLDB命令
                {
                    "description": "为 gdb 启用整齐打印",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "build" //调试开始前执行的任务,我们在调试前要编译构建,与tasks.json的label相对应,名字要一样
        }
    ]
}
```
到目前为止，差不多完成了 `C_Single` 下语言学习环境的配置。先来一个简单的Hello程序，尝试一下编译、运行、调试。首先要在 `C_Single` 中新建一个分类文件夹，假设为 `temp` ，再在 `temp` 文件夹下建立 `bin` 文件夹，然后在 `temp` 下新建 `hello.c` 文件，`hello.c` 中输入代码如下：
<img src="https://img-blog.csdnimg.cn/20210625103909812.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">

现在可以通过 **终端-运行生成任务…** 或者 `Ctrl+Shift+B` ，来仅编译构建 `Build` 。此时终端面板打开并显示如下，没有报错，而且在 `C_Single/temp/bin` 下多了 `hello.exe` ，构建成功：
<img src="https://img-blog.csdnimg.cn/20210625103004932.png" width="60%">
再尝试一下编译构建+运行，使用 `Ctrl+K Ctrl+S` 打开键盘快捷方式，搜索**任务**两个字，找到**运行测试任务**（默认没有快捷键，先前我们在 `tasks.json` 中将 `run` 任务设为测试组，正是为了此处），点击左侧的加号添加快捷键绑定。由于启动调试设为 `F5` ，这里将运行测试任务设置为 `F4` ：
<img src="https://img-blog.csdnimg.cn/20210625103828662.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">
然后回到 `hello.c` 界面，按下F4，结果如下，输入名字并按下回车，运行成功：
<img src="https://img-blog.csdnimg.cn/2021062510502698.png" width="60%">
最后是调试，我们在第一个 `printf` 前打上断点，然后要么使用调试快捷键 `F5` ，要么通过**运行-启动调试**：
<img src="https://img-blog.csdnimg.cn/20210625105723495.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">

点击单步调试按钮，或者使用快捷键 `F11` ，向下执行程序，**黄色箭头的那一行是现在未执行、下一步将要执行的语句**，执行到输入语句时黄色箭头消失，需要在终端面板中**输入+回车**，接着黄色箭头出现，可以继续向下执行，直到调试成功。

现在我们已经可以新建C源文件写程序，`F4` 一键编译构建+运行，`F5` 一键开始调试——和别的IDE的体验是不是差不多了？！类似的，我们在 `Code_Cpp/Cpp_Single` 中添加 `.vscode` 文件夹和 `tasks.json, launch.json` 文件（只需要修改前面给出的相关JSON代码，命令中 `gcc` 改为 `g++` ，添加一个静态链接到 `libstdc++` ，标准改为 `-std=c++17` ，注意 `"problemMatcher"` 仍为 `["$gcc"]` ），也尝试一下构建、运行和调试。


## 3.3 书写C/C++多文件下配置文件，尝试编译、运行和调试
搞定了 `C_Single` ，简单多文件的 `C_Multiple` 也就类似了。只是需要修改一下两个JSON文件中涉及到路径的部分。新文件如下：
```json
//tasks.json
{
   "version": "2.0.0",
   "tasks": [
       {
           "label": "build",
           "type": "shell",
           "command": "gcc",  
           "args": [
               "${fileDirname}\\*.c", //***
               "-o",
               "${fileDirname}\\${fileBasenameNoExtension}.exe", //***
               "-g",
               "-Wall", 
               "-static-libgcc",   
               "-fexec-charset=GBK",
               "-std=c11",   
           ],
           "group": {
               "kind": "build",
               "isDefault": true
           },
           "presentation": {
               "echo": true,
               "reveal": "always",
               "focus": false,
               "panel": "new"
           },
           "problemMatcher": "$gcc"
       },
       {
           "label": "run",
           "type": "shell",
           "dependsOn": "build",
           "command": "${fileDirname}\\${fileBasenameNoExtension}.exe", //***
           "group": {
               "kind": "test",
               "isDefault": true
           },
           "presentation": {
               "echo": true,
               "reveal": "always",
               "focus": true,
               "panel": "new"
           }
       }

   ]
}
```
```json
//launch.json
{
    "version": "0.2.0",
    "configurations": [{
        "name": "Debug", 
        "type": "cppdbg", 
        "request": "launch",
        "program": "${fileDirname}\\${fileBasenameNoExtension}.exe", //***
        "args": [],
        "stopAtEntry": false, 
        "cwd": "${fileDirname}", 
        "environment": [], 
        "externalConsole": false,
        "internalConsoleOptions": "neverOpen",
        "MIMode": "gdb", 
        "miDebuggerPath": "C:\\Program Files\\mingw64\\bin\\gdb.exe", 
        "preLaunchTask": "build" 
    }]
}
```
注意，`C_Single` 和 `C_Multiple` 文件夹的文件结构是不同的，写单文件时用前者（要建立一个 `bin` 文件夹），写多文件时用后者。

类似的，我们在 `Code_Cpp/Cpp_Multiple` 中添加 `.vscode` 文件夹和 `tasks.json, launch.json` 文件（只需要修改前面给出的相关JSON代码，命令中 `gcc` 改为 `g++` ，添加一个静态链接到 `libstdc++` ，标准改为 `-std=c++17` ，注意 `"problemMatcher"` 仍为 `["$gcc"]` ），然后写一个简单的多文件C++项目，尝试一下构建、运行和调试。
 
值得一提的是，VS Code在针对C++多文件编译时，可能不是那么友好，需要再搭配上CMake来使用，以后会介绍这方面的知识。

 
