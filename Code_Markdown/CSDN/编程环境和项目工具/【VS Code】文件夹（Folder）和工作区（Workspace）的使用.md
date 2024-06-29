在VS Code的**文件**菜单中，有两个容易让人迷惑的操作：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210701140437539.png)
不难发现，打开文件夹 `Open Folder` 指的是**打开项目文件夹并将其作为资源管理器中的根目录**，这一项目文件夹 `Project Folder` 包含一个或多个与项目相关的文件夹，以及该项目的VS Code配置文件夹 `.vscode` （其中包含 `settings.json, tasks.json, launch.json` 等配置文件）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021070115031971.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
打开工作区 `Open Workspace` ，其中的工作区指的是后缀为 `.code-workspace` 的文件，也是一个**JSON With Comments**格式的文件，里面包含了**所有与该工作区相关联的文件夹**，以及相关的VS Code配置信息。双击 `*.code-workspace` 文件/启动VS Code打开工作区，与启动VS Code打开文件夹，区别是，前者的文件夹名字后面有一个**工作区 `WORKSPACE`** 的后缀。


什么时候用文件夹？**在一个项目上工作，使用文件夹就已经足够**。针对该项目设置完VS Code后，会自动在该项目文件夹下创建一个 `.vscode` 文件夹，用于存储VS Code配置文件。

什么时候用工作区？**当且仅当需要同时在多个项目上工作的时候，才需要创建工作区**，工作区中有多个项目文件夹的根目录，即 `multiroot workspace` 。
