Git：代码回溯；版本切换；多人协作；远程备份
# 1. 在IDEA中配置Git
安装IDEA后，如果Git安装在默认路径下，则IDEA会自动找到Git位置；如果更改了Git安装位置，则需要手动配置Git的路径，选择File->Settings打开设置窗口，Version Control下的Git选项：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302170104970.png)

---
# 2. 在IDEA中使用Git
## 2.1 初始化仓库
在菜单栏VCS -> Import into Version Control -> Create Git Repository
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302170113779.png)
然后选择目录：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302170114991.png)
点绿色√，写Commit Message，再提交：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302170115512.png)
还可以看到提交记录：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302170116651.png)
推送到远端，VCS -> Git -> Push：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302170118424.png)
## 2.2 分支操作
在Version Control -> Log中可以创建分支，切换分支等：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302170127268.png)

IDEA安装lombk、free mybatis插件