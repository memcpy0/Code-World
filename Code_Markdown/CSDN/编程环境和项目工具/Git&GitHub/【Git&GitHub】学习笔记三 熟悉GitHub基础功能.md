@[toc]


> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「玩转Git/GitHub」系列文章之一，将随着时间不断修改完善，目的在于为读者提供有关Git/GitHub使用的大部分参考信息。由于内容随时可能发生更新变动，欢迎关注和收藏[玩转Git/GitHub系列文章目录](https://memcpy0.blog.csdn.net/article/details/119684105)以作备忘。

下面详细介绍GitHub的丰富功能。

---
# 1. 键盘快捷键
GitHub中，很多都可以使用键盘快捷键。熟悉键盘操作，能够让GitHub更加便捷。不用死记硬背，在各个页面按下 `shift + /` ，都可以打开键盘快捷键一览表，查看当前页面的快捷键：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200908104522676.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

---
# 2. 工具栏
工具栏常驻于各个页面的上端，先来讲解它：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200908112712593.png#pic_center)
- ① LOGO：点击它会进入控制面板；
- ② 搜索窗口：输入要找的用户或代码片段，就可以搜索到与之相关的信息；
- ③ Pull Request：点击查看用户已进行过的Pull Request，通过这里，开发者可以很方便地追踪Pull Request的后续情况；
- ④ Issues：查看用户拥有权限的仓库或者分配给自己的Issue，当用户同时进行多个项目时，可以在这里一并查看Issue；
- ⑤ Marketplace：相当于GitHub的开放平台或者扩展市场，提供两类扩展——`Apps`（应用，授权后使用用户的身份信息调用GitHub的开放接口，替用户完成代码检查、持续集成等）和`Actions`（一套持续集成的服务脚本，来帮助开发者自动构建和部署）；
- ⑥ Explore：从各个角度介绍GitHub上的热门软件，它会按照语言筛选本日/周/月的热门仓库/开发者；
- ⑦ Notifications：提示用户是否有新的通知。用户在新建Issue、被评论、进行Pull Request等时都会收到通知。按照默认设置，用户在GitHub收到的通知会同时发送到注册邮箱中；
- ⑧ 一些快捷功能的入口：包括 `New repository, New gist, New organization, New project` 等用来创建新的Git仓库、Organization、Project、Gist。其中Gist功能主要用来管理及发布一些没有必要保存在仓库中的代码，比如小的代码片段、随便编写的脚本代码等，系统会自动管理更新历史，并且提供了Fork功能；
- ⑨ 头像、用户名位置：点击会弹出如下图的下拉菜单。其中，点击 `Your profile` 会进入到用户的个人信息页面；点击 `Your repositories, Your projects` 查看拥有的个人仓库和创建的个人项目；`Help` 是GitHub相关的帮助，遇到不懂的可以在这里查一下；点击 `Settings` ，打开账户设置页面，这里可以进行个人信息、安全管理等的设置；……

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200909203832666.png#pic_center)

---
# 3. 控制面板
登录GitHub时，最先显示在我们眼前的是控制面板。由于GitHub的UI已经改版很多次了，所以和书上展示的大不一样：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200909233347375.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

- ① **News Feed**：显示当前已经 `Follow` 的用户和已经 `Watch` 的项目的活动信息，用户可以在这里查看最新动向。
- ② **Recent Activity**：展示用户最近发起的Pull Request、评论和其他动作；
- ③ **Explore Repositories**：部分展示最近的热点仓库；
- ④ **Repositories**：按照更新时间顺序显示用户的仓库。其中标有钥匙图案的是非公开仓库，标有类似字母 $\text{Y}$ 图案的是用户Fork过的仓库。

---
# 4. 个人信息
访问类似这样的URL，就可以看到各个开发者的个人信息：`https://github.com/用户名` 。我们以轮子哥的个人信息页为例进行讲解。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200910005447681.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
 

- ① **用户信息**：这里显示注册用户的基本信息，包括姓名、所属公司、邮箱地址、已加入的Organization等。如果对该用户有兴趣，可以点击 `Follow` 按钮，这样该用户在GitHub上的活动都会显示在我们的News Feed中。
- ② **Pinned/Popular repositories**：显示公开仓库中受到欢迎的、拥有大量Star的部分热门仓库。
- ③ **Contribution in the last year**：一格表示一天，记录当日用户对拥有读写权限的仓库的大致贡献度。贡献度的标准包括：发送Pull Request的次数、写Issue的次数、进行提交的次数等。颜色越深表示贡献度越高。
- ④ **时间栏**：点击可以查看不同时间段的贡献情况、活动情况等。
- ⑤ **Activity Overview**：显示该用户的公开活动的概览。
- ⑥ **Contribution activity**：按照时间顺序显示具体贡献活动的链接。
- ⑦ **Repositories**：显示该用户公开的仓库和Fork来的仓库。每个仓库，都会显示仓库名称、简要说明、使用的语言、Star的人数、Fork的次数、使用的许可协议和最新更新日期，旁边的图标则表示这个仓库的更新频率。
- ⑧、⑨ **Projects、Packages**：显示用户的项目和GitHub Packages。

---
# 5. 仓库
仓库的URL形式为：`https://github.com/用户名/仓库名` 。如vczh的gac：`https://github.com/vczh/gac` 。仓库的页面是各个软件的目录，通过目录找下去可以查阅所需的文件。如果有权限，还可以对文件的内容直接编辑和提交。
