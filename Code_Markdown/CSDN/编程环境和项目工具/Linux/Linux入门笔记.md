![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303180141051.png)
# 1. Linux简介
桌面操作系统：Windows，Mac OS，Linux
服务器操作系统：UNIX，Linux，Windows Server
移动操作系统：Android，iOS
嵌入式操作系统：Linux（机顶盒，路由器，交换机）

Linux分为内核版和发行版：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303180151014.png)

---
# 2. Linux安装
Linux系统的安装方式：
- 物理级安装：直接将操作系统安装在服务器硬件上
- 虚拟机安装：通过虚拟机软件安装

## 2.1 FinalShell
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303180203481.png)

## 2.2 Linux目录
Linux有根目录，相当于一棵树的根结点，而Windows则是多个盘符，相当于一个森林。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181118170.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181153695.png)
cd / 进入根目录。

---
# 3. Linux常用命令
## 3.0 常用命令
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181158264.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181200733.png)
Linux命令使用技巧：
- Tab自动补全
- 连续两次Tab键，给出操作提示

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181210644.png)

## 3.1 文件目录操作命令
-a ：all 。Linux系统中，以 `.` 开头的都是隐藏文件，隐藏文件都是以 `.` 开头的。多个选项可以连接起来写，比如 `-al` 。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181216611.png)
ls -al /etc

每个Linux用户都有自己的用户目录。root用户的Home目录就是/root。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181220064.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181222512.png)


![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181224994.png)
tail命令动态监控日志文件（可以这么做）
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181227946.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181231665.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181232502.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181239876.png)

## 3.2 拷贝移动命令
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181240270.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181246331.png)


## 3.3 打包压缩命令
-c -x互斥，打包/解包 -cvf -zcvf -xvf -zxvf
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181317186.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181323743.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181325892.png)
输入unzip命令解压zip压缩包。
出现inflating即表明解压成功。
## 3.4 文本编辑命令
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181327422.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181329814.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181330337.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181331571.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181332714.png)


## 3.5 查找命令
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181337098.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181340595.png)

## 3.5 服务管理
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181407416.png)


---
# 4. Linux软件安装
安装JDK，安装Tomcat，安装Tomcat，安装MySQL，安装lrzsz。不如Docker
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181354565.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181356821.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181400815.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181403293.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181416562.png)


![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181420441.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181422683.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181423382.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181426145.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181431697.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181433007.png)
冒号后面是密码，注意空格
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181435708.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181447257.png)


---
# 5. 项目部署
## 5.1 手工部署项目
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181451478.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181505510.png)
rz命令上传！

③ java -jar 启动SpringBoot程序。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181507041.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181509755.png)
 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181519491.png)

## 5.2 通过Shell脚本自动部署项目
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181810934.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181909320.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181914330.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181917450.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181918379.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181925962.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303182005107.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303182006793.png)
