@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「玩转Git/GitHub」系列文章之一，将随着时间不断修改完善，目的在于为读者提供有关Git/GitHub使用的大部分参考信息。由于内容随时可能发生更新变动，欢迎关注和收藏[玩转Git/GitHub系列文章目录](https://memcpy0.blog.csdn.net/article/details/119684105)以作备忘。

---
# 1. 为什么要使用emoji 
不经意发现大佬的GitHub上，Git的提交信息竟然有emoji前缀，相当鹤立鸡群、别具一格，感觉挺有趣的，就想着自己也使用一番。个人认为这样做有三点好处，后两点尤为重要：
- emoji比较呆萌，能美化提交记录和GitHub页面；
- emoji作为标签，能很好的对提交记录分门别类，方便整理；
- **emoji蕴含的丰富语义和情绪，能提高提交信息的可读性、可理解性，增强提交历史的阅读体验**。

一个示例如下所示，提交新内容，更新属性、贡献者、证明，消除Bug等等，各有各的emoji：
![在这里插入图片描述](https://img-blog.csdnimg.cn/7deb9cae251e4da297e391426852f286.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

# 2. emoji来源、用法和个人统一规范

这些emoji来自哪里？有什么统一规范来避免误解吗？有的，像[https://gitmoji.dev/](https://gitmoji.dev/)上就整理了一套emoji及其用法（如想下载，可见[这篇文章](https://memcpy0.blog.csdn.net/article/details/119716359)），[https://www.webfx.com/tools/emoji-cheat-sheet/](https://www.webfx.com/tools/emoji-cheat-sheet/)上的表情更多。用法也很简单，在 `git commit` 时如下书写提交信息，用 `:smile:` 来插入一个笑脸emoji：
```bash
$ git commit -m ":smile:表示一个笑脸"
```
除了**在Git提交时使用**，我们还可以**在 `README.md` 和GitHub Wiki中直接使用emoji**。这里整理和列出要使用的emoji，日后会慢慢增加或修改：
```py 

# ====常用颜文字====
# 🎉 :tada: Begin a project.                                开始一个项目
# 🚧 :construction: Work in progress.                       工作进行中
# ✨ :sparkles: Introduce new features.                     添加新功能
# 📝 :memo: Add or update documentation.                    增加或更新文档
# 🔧 :wrench: Add or update configuration files.            增加/更新配置文件
# 💄 :lipstick: Add or update the UI and style files.       增加/更新UI和样式文件
# 💡 :bulb: Add or update comments in source code.          增加/更新源代码中的注释
# 🚚 :truck: Move or rename resources (e.g.: files, paths, routes). 移动/重命名文件/路径
# 🔥 :fire: Remove code or files.                           移除代码/文件
# 🐛 :bug: Fix a bug.                                       修复bug
# 🚑️ :ambulance: Critical hotfix.                           紧急修复
# ⏪️ :rewind: Revert changes.                               回退
# ✏️ :pencil2: Fix typos.                                   修改错别字
# 🔀 :twisted_rightwards_arrows: Merge branches.            合并分支
# ⚡️ :zap: Improve performance.                             性能优化
# 🎨 :art: Improve structure / format of the code.          改进代码结构或格式
# ✅ :white_check_mark: Add, update, or pass tests.         增加/更新测试用例
# 📦️ :package: Add or update compiled files or packages.    增加/更新编译后文件/包
# 🙈 :see_no_evil: Add or update a .gitignore file.         增加/更新.gitignore文件
# 📄 :page_facing_up: Add or update license.                增加/更新LICENSE
# 🔊 :loud_sound: Add or update logs.                       增加/更新日志
# 🔇 :mute: Remove logs.                                    移除日志
# 👥 :busts_in_silhouette: Add or update contributor(s).    增加/更新贡献者
# ♻️ :recycle: Refactor code.                               重构代码
# 📈 :chart_with_upwards_trend: Add or update analytics or track code. 增加/更新分析/跟踪代码
# 🚸 :children_crossing: Improve user experience / usability. 增强用户体验/可用性
# 🛂 :passport_control: Work on code related to authorization, roles and permissions. 处理与授权、身份和权限相关的代码

# ====附加颜文字====
# ➕ :heavy_plus_sign: Add a dependency.                            增加一个依赖
# ➖ :heavy_minus_sign: Remove a dependency.                        移除一个依赖
# ⬇️ :arrow_down: Downgrade dependencies.                           降级依赖
# ⬆️ :arrow_up: Upgrade dependencies.                               升级依赖
# 📌 :pushpin: Pin dependencies to specific versions.               固定依赖到特定版本
# 🚀 :rocket: Deploy stuff.                                         部署功能
# 👽️ :alien: Update code due to external API changes.               由于外部API更改而更新代码
# 🔖 :bookmark: Release / Version tags.                             发布/版本标签
# 🔍️ :mag: Improve SEO.                                             提高SEO
# 🥅 :goal_net: Catch errors.                                       捕捉错误
# 🔒️ :lock: Fix security issues.                                    修复安全问题
# 🗑️ :wastebasket: Deprecate code that needs to be cleaned up.      废弃代码,需要清理
# ⚰️ :coffin: Remove dead code.                                     移除无用代码
# 🧪 :test_tube: Add a failing test.                                增加一个失败的测试
# 🏷️ :label: Add or update types.                                   增加/更新类型
# 💫 :dizzy: Add or update animations and transitions.              增加/更新动画和过渡
# 🚩 :triangular_flag_on_post: Add, update, or remove feature flags.增加/更新/移除功能标识
# 🔨 :hammer: Add or update development scripts.                    增加/更新开发脚本
# 👔 :necktie: Add or update business logic                         增加/更新业务逻辑
# 🍱 :bento: Add or update assets.                                  增加/更新assets
# 🌱 :seedling: Add or update seed files.                           增加/更新种子文件
# 📸 :camera_flash: Add or update snapshots.                        增加/更新快照
# 👷 :construction_worker: Add or update CI build system.           增加/更新CI构建系统
# 💚 :green_heart: Fix CI Build.                                    修复CI构建系统
# 💩 :poop: Write bad code that needs to be improved.               代码很烂需要改进
# 🚨 :rotating_light: Fix compiler / linter warnings.               修复compiler/linter的警告
# ♿️ :wheelchair: Improve accessibility.                            改善可访问性
# ⚗️ :alembic: Perform experiments.                                 进行实验
# 💥 :boom: Introduce breaking changes.                             引入破坏性改变
# 🗃️ :card_file_box: Perform database related changes.              执行数据库相关的改变
# 🏗️ :building_construction: Make architectural changes.            执行架构层次的改变
# 🩹 :adhesive_bandage: Simple fix for a non-critical issue.        非关键问题的简单修复
# 🧐 :monocle_face: Data exploration/inspection.                    数据探查/检查

# ====其他颜文字====
# 🌐 :globe_with_meridians: Internationalization and localization.  国际化与本地化
# 🍻 :beers: Write code drunkenly.                                  醉酒写码
# 💬 :speech_balloon: Add or update text and literals.              增加/更新文本和文字
# 📱 :iphone: Work on responsive design.                             致力于响应式设计
# 🥚 :egg: Add or update an easter egg.                             增加/更新复活节蛋
# 🤡 :clown_face: Mock things.                                      
```

# 3. VS Code中提交时使用emoji
这么多emoji，死记硬背不太科学，而且在Git Bash中使用起来也不是很方便……还是配置一下VS Code吧。经过反复比对，我找到了下面这个插件，能够在VS Code内嵌的Git支持中（即点击源代码管理图标后的左侧界面）添加一个花朵按钮，输入相应字符串并点击花朵按钮就会附加emoji（或者通过 `Ctrl+Shift+I` 快捷键来使用它）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/71c12153fea74229b471b453d1c23811.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![请添加图片描述](https://img-blog.csdnimg.cn/82661f986427488a9135ccd3275515d8.gif)
具体用法见其[中文文档页面](https://github.com/fz6m/individual-emoji-commit/blob/HEAD/README-CN.md)。我的配置如下：
```json
{
    "individualEmoji.specified.enable": true,
    "individualEmoji.specified.typeConfig": [
        ["begin", ["🎉"]], //:tada: Begin a project. 开始一个项目
        ["work", ["🚧"]],  //:construction: Work in progress. 工作进行中
        ["feat", ["✨"]],  //:sparkles: Introduce new features. 添加新功能
        ["docs", ["📝"]],  //:memo: Add or update documentation. 增加或更新文档
        ["config", ["🔧"]], //:wrench: Add or update configuration files.
        ["comment", ["💡"]], //:bulb: Add or update comments in source code. 增加/更新源代码中的注释
        ["rename", ["🚚"]], //:truck: Move or rename resources (e.g.: files, paths, routes). 移动/重命名文件/路径
        ["chore", ["🔥"]],  //:fire: Remove code or files.
        ["fix", ["🐛"]],   //:bug: Fix a bug. 修复bug
        ["revert", ["⏪️"]], //:rewind: Revert changes. 回退
        ["typos", ["✏️"]], //:pencil2: Fix typos. 修改错别字
        ["merge", ["🔀"]], //:twisted_rightwards_arrows: Merge branches. 合并分支
        ["pref", ["⚡️"]],  //:zap: Improve performance. 性能优化
        ["style", ["🎨"]], //:art: Improve structure / format of the code. 改进代码结构或格式
        ["test", ["✅"]], //:white_check_mark: Add, update, or pass tests.         增加/更新测试用例
        ["package", ["📦️"]], //:package: Add or update compiled files or packages.    增加/更新编译后文件/包
        ["log", ["🔊"]], //:loud_sound: Add or update logs. 增加/更新日志
        ["contributor", ["👥"]], //:busts_in_silhouette: Add or update contributor(s). 增加/更新贡献者
        ["refactor", ["♻️"]], //:recycle: Refactor code. 重构代码
        ["build", ["🚀"]], //:rocket: Deploy stuff. 构建和部署功能
        ["release", ["🔖"]], //:bookmark: Release / Version tags. 发行版本标签
    ],
    "individualEmoji.pos": "start",
}
```
这个插件支持自定义emoji和对应的字符串，只是emoji的位置太死板，只能出现在下面两个位置：
```bash
# start
feat(scope): some text => feat(scope) 🌈: some text

# end![请添加图片描述](https://img-blog.csdnimg.cn/d70dfc19075a4e018904b002dab6b57c.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

feat: some text => feat: some text 🌈
```
我不太喜欢这两个位置，还是认为把emoji图标统一放到提交信息第一列是最好的、最有辨识度的。为此安装了下面这个插件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/bade8ad2dcf643459f9a6ec18919d8a8.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
它的用法如图，虽然要点击按钮，而且也不能自定义……嘛，暂且忍一下吧：
![请添加图片描述](https://img-blog.csdnimg.cn/ac7c276b6c9e480687f949d9d7131f2f.gif)

