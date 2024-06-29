@[toc]




#  Linux 基础入门
## 0. Linux目录体系
![在这里插入图片描述](https://img-blog.csdnimg.cn/7a6adc9fe8a74f58a5697ffc64e79d52.png)
## 1. 目录操作
### 创建目录
使用 mkdir 命令创建目录 `mkdir /home/testFolder` 
```bash
mkdir $HOME/testFolder
```

### 切换目录
使用 cd 命令切换目录

```bash
cd $HOME/testFolder
```

使用 cd ../ 命令切换到上一级目录，或 `cd ..`

```bash
cd ../
```

### 移动目录
使用 `mv` 命令移动目录

```bash
mv $HOME/testFolder /var/tmp
```

### 删除目录
使用 `rm -rf` 命令删除目录

```bash
rm -rf /var/tmp/testFolder
```

### 查看当前目录
使用 `pwd` 命令查看当前目录，注意 `~` 是用户根目录 `root` ：
```bash
pwd
```
### 查看目录下的文件
使用 ls 命令查看 /etc目录下所有文件和文件夹

```bash
ls /etc
```

## 2. 文件操作
### 创建文件
使用 touch 命令创建文件

```bash
touch ~/testFile
```

执行 ls 命令, 可以看到刚才新建的 testFile 文件

```bash
ls ~
```

### 复制文件
使用 cp 命令复制文件

```bash
cp ~/testFile ~/testNewFile
```

### 删除文件
使用 rm 命令删除文件, 输入 y 后回车确认删除

```bash
rm ~/testFile
```

### 查看文件内容
使用 cat 命令查看 .bash_history 文件内容

```bash
cat ~/.bash_history
```

## 3. 过滤、管道与重定向
### 过滤
过滤出 /etc/passwd 文件中包含 root 的**记录**

```bash
grep 'root' /etc/passwd
```
递归地过滤出 /var/log/ 目录中包含 linux 的**记录**

```bash
grep -r 'linux' /var/log/
```

### 管道
简单来说, Linux 中管道的作用是将上一个命令的输出作为下一个命令的输入, 像 pipe 一样将各个命令串联起来执行, 管道的操作符是 |
比如, 我们可以将 cat 和 grep 两个命令用管道组合在一起

```bash
cat /etc/passwd | grep 'root'
```

过滤出 /etc 目录中名字包含 ssh 的目录(不包括子目录)

```bash
ls /etc | grep 'ssh'
```

### 重定向
可以使用 > 或 < 将命令的输出重定向到一个文件中

```bash
echo 'Hello World' > ~/test.txt
```

## 4. 运维常用命令
### ping命令
对 cloud.tencent.com 发送 4 个 ping 包, 检查与其是否联通

```bash
ping -c 4 cloud.tencent.com
```

### netstat命令
netstat 命令用于显示各种网络相关信息，如网络连接, 路由表, 接口状态等等
列出所有处于监听状态的tcp端口

```bash
netstat -lt
```

查看所有的端口信息, 包括 PID 和进程名称

```bash
netstat -tulpn
```

### ps命令
过滤得到当前系统中的 ssh 进程信息

```bash
ps aux | grep 'ssh'
```

为什么可以在HOME前写成 $HOME/... ，其他目录不能写
~文件夹在哪里？

韩顺平 一周学会Linux。

https://www.bilibili.com/read/cv5655041/

https://cloud.tencent.com/developer/labs/gallery?category=env_build

https://cloud.tencent.com/developer/labs/lab/10000

HOME 是个全局环境变量，意思是打印出当前用户环境的主目录路径 通常用波浪号“~”来表示当前用户的主目录(home目录)
```bash
[root@VM-189-93-centos /]$ $HOME
-bash: /root: Is a directory

[root@VM-189-93-centos /]$ echo "$HOME"
/root
```
---
# 文件服务器
[【Linux】基于CentOS搭建FTP文件服务器](https://memcpy0.blog.csdn.net/article/details/121019272) https://cloud.tencent.com/developer/labs/lab/10002
[【Linux】基于Ubuntu搭建FTP文件服务器](https://memcpy0.blog.csdn.net/article/details/121053775) https://cloud.tencent.com/developer/labs/lab/10123 失败了，不知道为什么无法连接
# 个人网盘
[【Linux】基于CentOS搭建个人网盘](https://memcpy0.blog.csdn.net/article/details/121111702) https://cloud.tencent.com/developer/labs/lab/10093 需要域名
[【Linux】基于Ubuntu搭建个人网盘]() https://cloud.tencent.com/developer/labs/lab/10134

# 个人博客
[【Linux】基于 CentOS 搭建 Hexo 个人博客](
[【Linux】基于 Ubuntu 搭建 Hexo 个人博客](
[【Linux】基于 CentOS 搭建 WordPress 个人博客](
[【Linux】基于 Ubuntu 搭建 WordPress 个人博客](

[【Linux】基于 CentOS 7 搭建 Halo 博客应用](
单机部署 WordPress 个人博客
基于 typecho 搭建个人博客系统
搭建 Hexo 个人博客 写作与部署
基于 ubuntu+jekyll 搭建个人博客
搭建 OpenLiteSpeed 安装 WordPress

# Django

[基于CentOS搭建Python的Django环境](https://memcpy0.blog.csdn.net/article/details/121020142) CentOS 7.2 64 位
Python+Django网站开发 
基于 CentOS7.2 的 Django 环境搭建

# 云服务体验
[【Linux】体验 CentOS 系统 CVM](https://memcpy0.blog.csdn.net/article/details/121051610)
[【Linux】体验 Ubuntu 系统 CVM](https://memcpy0.blog.csdn.net/article/details/121051627)




[体验 COS 与 CDN 服务 for PHP](https://editor.csdn.net/md?articleId=121023523)
[体验 COS 与 CDN 服务 for Node.js]()
[体验 COS 与 CDN 服务 for Python]()
体验云函数
使用 CMQ 和 SCF 实现邮件发送 
# 技术学习
[基于 Ubuntu 搭建微信小程序服务] 软件环境Ubuntu 16.04.1 LTS 64 位

微信小程序开发 Start-up

搭建微信 AI 机器人
# 云服务体验
[【Linux】基于CentOS与NodeJs打造Web在线聊天室](https://memcpy0.blog.csdn.net/article/details/121050887) https://cloud.tencent.com/developer/labs/lab/10454 失败了m，`node index.js` 时报错 `SyntaxError: Unexpected token >`

# 开发环境
[【Linux】基于CentOS搭建Node.js环境](https://memcpy0.blog.csdn.net/article/details/121055088) https://cloud.tencent.com/developer/labs/lab/10040/
[【Linux】部署与发布之Linux服务器]() https://cloud.tencent.com/developer/labs/lab/10376


---
 
