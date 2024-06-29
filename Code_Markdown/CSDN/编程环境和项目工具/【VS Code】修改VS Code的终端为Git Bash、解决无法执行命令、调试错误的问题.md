> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。



VS Code的默认终端为PowerShell，如下图所示： 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715144255793.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
为了更加熟练使用Git  Bash，我决定更换终端。有关Git Bash的安装，见本人的[这篇文章](https://memcpy0.blog.csdn.net/article/details/108431631)。先前我看到知乎上一篇文章的做法是，打开设置搜索 `shell:windows` ，点击 `setting.json` 中编辑：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715144458468.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
在此处输入Git Bash的可执行文件 `bash.exe` 路径： 
```json
{
    "terminal.integrated.automationShell.windows": "D:\\Program Files\\Git\\bin\\bash.exe",
}
```
最后重启，发现好像没有效果……再一看时间点，竟然是2020年的文章。2021年6月更新VS Code后，相关配置似乎已经弃用了？配置默认终端的推荐做法见这篇官方文档——[详细了解如何配置shell](https://code.visualstudio.com/docs/editor/integrated-terminal#_configuration)。新的方法是，修改配置文件，将下面的配置加到用户设置对应的 `setting.json` 文件中：
```json
{
    "terminal.integrated.profiles.windows": {
        "PowerShell -NoProfile": {
            "source": "PowerShell",
            "args": [
                "-NoProfile"
            ]
        },
        "Git-Bash": {
            "path": "D:\\Program Files\\Git\\bin\\bash.exe",
            "args": []
        }
    },
    "terminal.integrated.defaultProfile.windows": "Git-Bash",
}
```
重启VS Code即自动选择Git Bash终端：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210715160842332.png)
这也允许我们同时使用PowerShell、CMD、Git Bash作为终端（以后还可以添加别的终端程序）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718123325919.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

当然问题也来了，我发现外部Git Bash、CMD能执行的 `irb, java, javac, python, ruby -v` 等命令，在VS Code集成终端中无法使用，对此的解决方案是：
1. 右击VS Code图标，选择以管理员身份运行，或者设置属性->快捷方式->高级->用管理员身份运行：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021071812421057.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718124148874.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

2. 在外部的PowerShell中运行代码 `set-ExectionPolicy RemoteSigned` ，允许在终端运行命令，之后使用 ` get-ExecutionPolicy` 显示为 `RemoteSigned` 而非 `Restricted`（意为禁止终端使用命令）。执行策略可能的值有 `Unrestricted, RemoteSigned, AllSigned, Restricted, Default, Bypass, Undefined` ，可以通过 ` get-help ExecutionPolicies` 查询策略命令：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718124640445.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
3. 现在在VS Code内部终端Git Bash，输入命令 `irb` 打开Ruby交互式程序：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718172728135.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


---
**20210923**更新：使用Git Bash作为默认终端后，在编译C/C++程序时，可能出现 `g++.exe: error: C:CodeWorldCodeCppCpp_Singlesdf.cpp: No such file or directory` 这种错误，其实应该是 `C:\\CodeWorld\Code_Cpp\Cpp_Single\sdf.cpp` ，只是被解析为没有斜杠的地址，导致编译器找不到该文件。

造成这个错误的原因是，不同的终端对斜杠和反斜杠的解析规则不同。在VS Code中打开终端，点击右侧的下拉箭头，可以选择默认打开的终端：
![在这里插入图片描述](https://img-blog.csdnimg.cn/af1ea1ed6853461bb95b4471ef64c7cc.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_10,color_FFFFFF,t_70,g_se,x_16)
点击 `Select Default Shell` 后，界面上方会弹出环境中所有的Shell以供选择。选择PowerShell或者Command Prompt之后，就可以正常调试了。与此同时，配置代码也会做相应改变：
```cpp
{
	"terminal.integrated.defaultProfile.windows": "PowerShell",
}
```
