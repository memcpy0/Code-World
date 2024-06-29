@[toc]
 
> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。
 
[获取了VS Code编辑器之后](https://memcpy0.blog.csdn.net/article/details/117640795)，我们将在VS Code中配置Prolog的语言学习环境。同样的，我会在个人代码库 `CodeWorld` 中建立 `Code_Prolog` 文件夹，里面的文件夹是 `Prolog_Single` 和 `Prolog_Multiple` ，分别存放不同的分类代码文件夹，即程序运行目标分别是**单个源代码文件**或**少数几个源代码文件的组合**。

---
# 1. 下载安装Prolog运行环境
Prolog是一门不走寻常路的语言，用来进行逻辑编程。学习之前要先安装Prolog的运行环境SWI-Prolog，我们进入它的[官网](https://www.swi-prolog.org/)：
![在这里插入图片描述](https://img-blog.csdnimg.cn/130d98f6ffcd4c6bb12acec4fd71ca66.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

点击Download进入下载页面，选择 `Development release` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/01cfc616110d487d8acb2769839c703c.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

选择合适的软件版本进行下载：
![在这里插入图片描述](https://img-blog.csdnimg.cn/62a595194c234081908df92de527b9d3.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/ac8b356772cc48f48325e84c32ad16af.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

运行安装程序，一路向前：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c17297cdcd724d8ea389fff9a8855248.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/514549e49c5b40e784fe3760f03a3eab.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

这里选择**将swipl加入到系统变量中**，勾选**创建swipl桌面图标**：
![在这里插入图片描述](https://img-blog.csdnimg.cn/df37b13f266a44368513cb0c9c8b29cc.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
安装目录和快捷方式就随便了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/98d748cc5d3343d3ab2ed3986948f18e.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/04bca557f7de4093a47b0af967bd3c89.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

把全部组件都装上，反正占用的空间很少：
![在这里插入图片描述](https://img-blog.csdnimg.cn/4cab922413ef4a5885c904aeb678bd7d.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

完成安装后如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e6b3ead12fc14ced90a80eea26c3a16a.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

想再确认一下，可以打开命令行，输入 `swipl` ，`?-` 相当于命令提示符：
![在这里插入图片描述](https://img-blog.csdnimg.cn/1fc1b7c774cd4b229ff80dc75115cbb4.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
Hello World的例子如下：

```puppet
1 ?- write("Hello World").
Hello World
true.
```


---
# 2. VS Code中安装支持插件
VS Code中和Prolog相关的插件少得可怜，在扩展商店里，要安装的插件只有下面一个：
![在这里插入图片描述](https://img-blog.csdnimg.cn/371fc9dde4394195a79f303d9a777c8a.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


---
# 3. 书写插件配置代码
根据这一插件的使用说明，在配置文件 `settings.json`（此处我是在工作区配置文件）中输入以下配置代码。如果有疑问或错误，可以查看这一插件的[Issue](https://github.com/arthwang/vsc-prolog/issues)：
```json
{
    "prolog.dialect": "swi", //prolog方言选择:swi(SWI-Prolog)或者ecl(ECLiPSe)
    "prolog.executablePath": "C:\\Program Files\\swipl\\bin\\swipl.exe", //指向prolog可执行文件
    "prolog.linter.run": "onSave", //保存文件时进行代码检查
    //"prolog.linter.delay": 500,  //使用"onType"时用到的选项
    "prolog.format.enabled": true, //允许format源代码
    "prolog.format.tabSize": 4,    //将tab替换为4个空格
    "prolog.format.insertSpaces": true, //更偏向使用空格
}
```
为了进行调试，还需要创建一个 `.vscode` 文件夹，在里面建立 `launch.json` 文件：
```json
{
    "configurations": [
        { //这个大括号里是我们的调试Debug配置
            "name": "Debug",        //配置名称
            "type": "prolog",       //配置类型
            "request": "launch",    //请求配置类型,可以是启动launch或附加attach
            "program": "${file}",   //将要进行调试的程序的完整路径
            "startupQuery": "start",
            "stopAtEntry": false,   //设为true时调试程序将暂停在程序入口处,否则将执行到第一个断点处
            "cwd": "${fileDirname}",//调试程序时的工作目录,此处为源码文件所在目录
            "env": {},              //传递给源文件的环境变量
            "runtimeExecutable": "C:\\Program Files\\swipl\\bin\\swipl.exe",  //调试进程的可执行文件  
            "runtimeArgs": [],      //传递给调试程序的命令行参数
            "args": [],             //程序调试时传递给程序的命令行参数,这里设为空即可
            "traceCmds": {          //介于vscode调试命令和prolog trace选项之间的映射
                "continue": ["leap", "l"], 
                "stepover": ["skip", "s"],
                "stepinto": ["creep", "c"], 
                "stepout": ["up", "u"]
            }, 
        }
    ]
}
```
说实在的，这个插件有很多Bugs，而且作者好像不更新了……如果以后有时间了，说不定我自己会写个支持插件。
