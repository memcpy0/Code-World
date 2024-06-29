@[toc]

# 实验内容和软件环境
FTP 是一个很实用的文件传输协议，方便在客户端和服务器之间进行文件的传输。本实验在Ubuntu 16.04.1 LTS 64 位上，使用 `vsftpd` 来搭建一个 FTP 服务，并且创建专有的 FTP 登录账户，保障服务器安全。



# 安装并启动 FTP 服务
## 安装 VSFTPD
使用 `apt-get` 安装 `vsftpd` 。`vsftpd` 是在 Linux 上被广泛使用的 FTP 服务器，根据其官网介绍，它可能是 UNIX-like 系统下最安全和快速的 FTP 服务器软件。

```bash
ubuntu@VM-22-216-ubuntu:~$ sudo apt-get install vsftpd -y
Reading package lists... Done
Building dependency tree       
Reading state information... Done
The following packages were automatically installed and are no longer required:
  amd64-microcode crda intel-microcode iucode-tool iw libc-dev-bin
  libnl-3-200 libnl-genl-3-200 wireless-regdb
Use 'sudo apt autoremove' to remove them.
The following additional packages will be installed:
  ssl-cert
Suggested packages:
  openssl-blacklist
The following NEW packages will be installed:
  ssl-cert vsftpd
0 upgraded, 2 newly installed, 0 to remove and 52 not upgraded.
Need to get 132 kB of archives.
After this operation, 398 kB of additional disk space will be used.
Get:1 http://mirrors.tencentyun.com/ubuntu xenial/main amd64 ssl-cert all 1.0.37 [16.9 kB]
Get:2 http://mirrors.tencentyun.com/ubuntu xenial/main amd64 vsftpd amd64 3.0.3-3ubuntu2 [115 kB]
Fetched 132 kB in 0s (2,737 kB/s)
Preconfiguring packages ...
Selecting previously unselected package ssl-cert.
(Reading database ... 60490 files and directories currently installed.)
Preparing to unpack .../ssl-cert_1.0.37_all.deb ...
Unpacking ssl-cert (1.0.37) ...
Selecting previously unselected package vsftpd.
Preparing to unpack .../vsftpd_3.0.3-3ubuntu2_amd64.deb ...
Unpacking vsftpd (3.0.3-3ubuntu2) ...
Processing triggers for man-db (2.7.5-1) ...
Processing triggers for ureadahead (0.100.0-19.1) ...
Processing triggers for systemd (229-4ubuntu21.29) ...
Setting up ssl-cert (1.0.37) ...
Setting up vsftpd (3.0.3-3ubuntu2) ...
Processing triggers for ureadahead (0.100.0-19.1) ...
Processing triggers for systemd (229-4ubuntu21.29) ...
```

    

## 启动 VSFTPD

安装完成后 VSFTPD 会自动启动，通过 `netstat` 命令可以看到系统已经监听了 21 端口——FTP 协议默认使用 21 端口作为服务端口：

```bash
ubuntu@VM-22-216-ubuntu:~$ sudo netstat -nltp | grep 21
tcp6       0      0 :::21                   :::*                    LISTEN      1964/vsftpd    
```

如果没有启动，可以手动开启 VSFTPD 服务：

```bash
ubuntu@VM-22-216-ubuntu:~$ sudo systemctl start vsftpd.service
```

    

# 配置用户访问目录

## 新建用户主目录

```bash
ubuntu@VM-22-216-ubuntu:~$ sudo mkdir /home/uftp
```

执行完后，在 `/home/uftp` 就能看到新建的文件夹 uftp 了。

创建登录欢迎文件，**方便用户登录后可以看到欢迎信息，并且确定用户确实登录到了主目录上**（用户的主目录是用户通过 FTP 登录后看到的根目录）。

```bash
ubuntu@VM-22-216-ubuntu:~$ sudo touch /home/uftp/welcome.txt
```
## 新建用户 uftp 并设置密码
创建一个用户 uftp ：

```bash
ubuntu@VM-22-216-ubuntu:~$ sudo useradd -d /home/uftp -s /bin/bash uftp
```

为用户 uftp 设置密码：

```bash
ubuntu@VM-22-216-ubuntu:~$ sudo passwd uftp
Enter new UNIX password: 
Retype new UNIX password: 
passwd: password updated successfully
```

删除掉 `pam.d` 中 `vsftpd` ，因为该配置文件会导致使用用户名登录 ftp 失败：

```bash
ubuntu@VM-22-216-ubuntu:~$ sudo rm /etc/pam.d/vsftpd
```
## 限制该用户仅能通过 FTP 访问

限制用户 uftp 只能通过 FTP 访问服务器，而不能直接登录服务器：

```bash
ubuntu@VM-22-216-ubuntu:~$ sudo usermod -s /sbin/nologin uftp
```

## 修改 vsftpd 配置

```bash
ubuntu@VM-22-216-ubuntu:~$ sudo chmod a+w /etc/vsftpd.conf
```

修改 `/etc/vsftpd.conf` 文件中的配置（直接将如下配置添加到配置文件最下方）：

```bash
# 限制用户对主目录以外目录访问
chroot_local_user=YES

# 指定一个 userlist 存放允许访问 ftp 的用户列表
userlist_deny=NO
userlist_enable=YES

# 记录允许访问 ftp 用户列表
userlist_file=/etc/vsftpd.user_list

# 不配置可能导致莫名的530问题
seccomp_sandbox=NO

# 允许文件上传
write_enable=YES

# 使用utf8编码
utf8_filesystem=YES
```

新建文件 `/etc/vsftpd.user_list` ，用于存放允许访问 ftp 的用户：

```bash
ubuntu@VM-22-216-ubuntu:~$ sudo touch /etc/vsftpd.user_list
ubuntu@VM-22-216-ubuntu:~$ sudo chmod a+w /etc/vsftpd.user_list
```

修改 `/etc/vsftpd.user_list` ，加入刚刚创建的用户：

```bash
uftp
```

## 设置访问权限

设置主目录访问权限（只读）：

```bash
ubuntu@VM-22-216-ubuntu:~$ sudo chmod a-w /home/uftp
```

新建公共目录，并设置权限（读写）：

```bash
ubuntu@VM-22-216-ubuntu:~$ sudo mkdir /home/uftp/public && sudo chmod 777 -R /home/uftp/public
```

重启vsftpd 服务：

```bash
ubuntu@VM-22-216-ubuntu:~$ sudo mkdir /home/uftp/publicubuntu@VM-22-216-ubuntu:~$ sudo systemctl restart vsftpd.service
```

# 准备域名和证书
注：如果您不需要通过域名访问 FTP 服务器则可以直接点击“已完成，下一步”跳过域名和证书的准备环节
## 域名注册

如果您还没有域名，可以[在腾讯云上选购](https://dnspod.cloud.tencent.com/?fromSource=lab)。
 
## 域名解析
域名购买完成后, 需要将域名解析到实验云主机上，实验云主机的 IP 为：`<您的 CVM IP 地址>`

在腾讯云购买的域名，可以[到控制台添加解析记录](https://console.cloud.tencent.com/domain)。

域名设置解析后需要过一段时间才会生效，通过 ping 命令检查域名是否生效，注意替换下面命令中的 `www.yourmpdomain.com` 为您自己的注册的域名：

```bash
$ ping www.yourdomain.com
```

如果 ping 命令返回的信息中含有你设置的解析的 IP 地址，说明解析成功。

    

# 访问 FTP 服务 
FTP 服务已安装并配置完成，下面我们来使用该 FTP 服务

根据您个人的工作环境，选择一种方式来访问已经搭建的 FTP 服务
## 通过 FTP 客户端工具访问
FTP 客户端工具众多，下面推荐两个常用的：
- FileZilla - 跨平台的 FTP 客户端，支持 Windows 和 Mac
-  WinSCP - Windows 下的 FTP 和 SFTP 连接客户端

下载和安装 FTP 客户端后，使用下面的凭据进行连接即可：

主机 `<您的 CVM IP 地址>` ，用户 `uftp` 。如果您申请了域名，可以将Ip 地址替换为对应的域名作为访问凭据。

输入密码后，如果能够正常连接，那么大功告成，可以开始使用属于自己的 FTP 服务器了！

接下来，上传任意一张图片到 FTP 服务器上的 `uftp` 的 `public` 目录下，然后，就可以在 `/home/uftp/public` 中看到了。
## 通过 Windows 资源管理器访问
Windows 用户可以复制下面的链接到资源管理器的地址栏访问——如果您申请了域名，可以将链接中的 Ip 地址替换为对应的域名访问 FTP 服务：
```bash
ftp://uftp:你的密码@<您的 CVM IP 地址>
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/6e716a7ea630465bacc1490eb69c53ae.png)

# 大功告成

恭喜！您已经成功完成了搭建 FTP 服务器的实验任务。


