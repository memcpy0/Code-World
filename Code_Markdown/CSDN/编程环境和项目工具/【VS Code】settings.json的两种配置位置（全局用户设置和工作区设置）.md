@[toc]

众所周知，VS Code存在两种设置 `settings.json` 的方式：
- `User Settings` 用户设置：`Settings that apply globally to any instance of VS Code you open.` 该设置会创建一个 `settings.json` 文件，**关联于用户区**，即Windows的某个用户下，打开任意VS Code界面都会用此配置，或者说**应用于该用户打开的所有工程**。
- `Workspace Settings` 工作区设置：`Settings stored inside your workspace and only apply when the workspace is opened.` 该设置指的是，使用VS Code打开某个**文件夹**或者**工作区**（关于文件夹和工作区的区别，见[这篇文章](https://memcpy0.blog.csdn.net/article/details/118385489)），在该文件夹下创建一个名为 `.vscode` 的隐藏文件夹，文档位置为 `根目录/.vscode/settings.json`（可自行决定是否创建），里面的设置**仅适用于当前目录下的VS Code**，且**工作区设置会覆盖用户设置**。


---
# 1. 创建与设置 `User Settings` 中 `settings.json` 的多种方法

## 1.1 使用菜单栏中的运行->添加配置（不太常用）
菜单栏中的运行->添加配置，其作用分为多种情况：
- 在**打开文件夹或者工作区**时，如果存在 `launch.json` ，就会跳转到 `launch.json` 文件中。否则：
	- 在**存在程序源文件**时，**添加配置**会先要求选择环境 `C++ (GDB/LLDB), C++ (Windows), Node.js...` 等等……最后生成 `.vscode` 文件夹，创建其中的 `tasks.json, launch.json` 文件（对于C和C++来说是如此）；
	- **打开文件夹**但是**不存在源文件**时，只会创建 `lauch.json` 文件；
- 如果**没有打开文件夹**，添加配置会打开**用户配置 `settings.json` 文件**。

## 1.2 使用UI设置界面
使用 `Ctrl+,` 或者点击左下角齿轮图标并选择设置。然后在文本编辑器中找到 `settings.json` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630160955412.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 1.3 使用命令面板
使用 `Ctrl+Shift+P` 或者点击左下角齿轮图标，选择命令面板。然后输入 `settings.json` 来搜索，点击 `Open Settings (JSON)` 即可进入用户设置 `settings.json` 文件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630161141173.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
如果搜索的是 `settings` ，会弹出多个选项：
- `Open User Settings` 会打开UI设置界面；
- `Open Workspace Settings` 也会打开UI设置界面；
- `Open Settings (JSON)` 会打开用户设置 `settings.json` 文件；
- `Open Workspace Settings (JSON)` 会打开工作区设置 `settings.json` 文件
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210701152336829.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)



---
# 2. 创建与设置 `Workspace Settings` 中 `settings.json` 的多种方法
## 2.1 手动创建 
在**打开文件夹或者工作区**时，手动创建 `.vscode` 文件夹，并在其中创建 `settings.json` 文件。
## 2.2 使用命令面板
使用 `Ctrl+Shift+P` 或者点击左下角齿轮图标，选择命令面板。然后输入 `settings.json` 来搜索。这一方法仅适用于**打开文件夹或工作区**时：
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630161517331.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


