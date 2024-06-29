@[toc]

## 安装 VSFTPD
使用 yum 安装 [vsftpd]：

```bash
$ yum install vsftpd -y
```

vsftpd 是在 Linux 上被广泛使用的 FTP 服务器，根据其官网介绍，它可能是 UNIX-like 系统下最安全和快速的 FTP 服务器软件。
![在这里插入图片描述](https://img-blog.csdnimg.cn/87b2840885e94413a4c3c8ed391d4de2.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

## 启动 VSFTPD

安装完成后，启动 FTP 服务：

```bash
$ service vsftpd start
Redirecting to /bin/systemctl start  vsftpd.service
```

启动后，可以看到系统已经监听了 21 端口（ FTP 协议默认使用 21 端口作为服务端口）：

```bash
$ netstat -nltp | grep 21
tcp6       0      0 :::21                   :::*                    LISTEN      6969/vsftpd 
```

此时，访问 `ftp://<您的CVM IP地址>` 可浏览机器上的 `/var/ftp` 目录了。
 
## 配置 FTP 权限 
目前 FTP 服务登陆允许匿名登陆，也无法区分用户访问，我们需要配置 FTP 访问权限
### 了解 VSFTP 配置
`vsftpd` 的配置目录为 `/etc/vsftpd` ，包含下列的配置文件：
- `vsftpd.conf` 为主要配置文件
- `ftpusers` 配置禁止访问 FTP 服务器的用户列表
 - `user_list` 配置用户访问控制
![在这里插入图片描述](https://img-blog.csdnimg.cn/e9637b1533944c7eb94888912c20e207.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
阅读上述配置以了解更多信息。如果您准备好了，点击下一步开始修改配置来设置权限。

### 阻止匿名访问和切换根目录

匿名访问和切换根目录都会给服务器带来安全风险（匿名访问让所有人都可以上传文件到服务器上而无需鉴权，而允许切换根目录则可能产生越权访问问题），我们把这两个功能关闭。

编辑 `/etc/vsftpd/vsftpd.conf` ，找到下面两处配置并修改为：
```bash
# 禁用匿名用户
anonymous_enable=NO

# 禁止切换根目录
chroot_local_user=YES
```

编辑完成后，按 Ctrl + S 保存配置，重新启动 FTP 服务，如：
```bash
$ service vsftpd restart
```
 
### 创建 FTP 用户
创建一个用户 `ftpuser` ：
```bash
$ useradd ftpuser
```
为用户 `ftpuser` 设置密码：
```bash
$ echo "Password" | passwd ftpuser --stdin
Changing password for user ftpuser.
passwd: all authentication tokens updated successfully.
```

### 限制该用户仅能通过 FTP 访问

限制用户 `ftpuser` 只能通过 FTP 访问服务器，而不能直接登录服务器：

```bash
$ usermod -s /sbin/nologin ftpuser
```

### 为用户分配主目录
为用户 `ftpuser` 创建主目录（用户的主目录是用户通过 FTP 登录后看到的根目录）并约定：
- `/data/ftp` 为主目录, 该目录不可上传文件
- `/data/ftp/pub` 文件只能上传到该目录下

```bash
$ mkdir -p /data/ftp/pub
```

创建登录欢迎文件，方便用户登录后可以看到欢迎信息，并且确定用户确实登录到了主目录上：
```bash
$ echo "Welcome to use FTP service." > /data/ftp/welcome.txt
```

设置访问权限：

```bash
$ chmod a-w /data/ftp && chmod 777 -R /data/ftp/pub
```

设置为用户的主目录：
```bash
$ usermod -d /data/ftp ftpuser
```

   

## 准备域名和证书
注：如果您不需要通过域名访问 FTP 服务器则可以直接点击“已完成，下一步”跳过域名和证书的准备环节

### 域名注册

如果您还没有域名，可以[在腾讯云上选购](https://dnspod.cloud.tencent.com/?fromSource=lab)，过程可以参考[视频](https://dnspod.cloud.tencent.com/?fromSource=lab)。 
 
### 域名解析
域名购买完成后, 需要将域名解析到实验云主机上，实验云主机的 IP 为：
```bash
<您的 CVM IP 地址>
例如:
118.89.43.134
```

在腾讯云购买的域名，可以[到控制台添加解析记录](https://console.cloud.tencent.com/domain)，过程可参考下面的[视频](https://v.qq.com/x/page/t0507ps9kxo.html)。

域名设置解析后需要过一段时间才会生效，通过 ping 命令检查域名是否生效，如：

```bash
$ ping www.yourdomain.com
```
如果 ping 命令返回的信息中含有你设置的解析的 IP 地址，说明解析成功。

 

## 访问 FTP 服务
 
FTP 服务已安装并配置完成，下面我们来使用该 FTP 服务。根据您个人的工作环境，选择一种方式来访问已经搭建的 FTP 服务。

### 通过 Windows 资源管理器访问
Windows 用户可以复制下面的链接，到资源管理器的地址栏访问：
```bash
ftp://ftpuser:Password@<您的 CVM IP 地址>
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/31d0acad20924d51a7aff2b0766b54b0.png)

### 通过 FTP 客户端工具访问
FTP 客户端工具众多，下面推荐两个常用的：
- [WinSCP]() - Windows 下的 FTP 和 SFTP 连接客户端
- [FileZilla]() - 跨平台的 FTP 客户端，支持 Windows 和 Mac

下载和安装 FTP 客户端后，使用下面的凭据进行连接即可：

主机（如果您申请了域名，可以将IP地址替换为对应的域名作为访问凭据）：
```bash
<您的 CVM IP 地址>
```

用户：

```bash
ftpuser
```

密码：
```bash
Password
```

如果能够正常连接，那么大功告成，您可以开始使用属于您自己的 FTP 服务器了！

接下来，请上传任意一张图片到您的 FTP 服务器上的pub目录下，然后，就可以在 `/data/ftp/pub` 中看到了。注意，请不要直接上传文件到根目录下，否则会如下所示报错：
![在这里插入图片描述](https://img-blog.csdnimg.cn/431c444fd31343e3850641c6c530d50b.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
您应该选择上传到 pub 目录下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/90f07a7ac9304111b87837e4c786abfd.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/7372403a3e1a438d8dab991c34a38b54.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)


## 大功告成
恭喜！现在已经成功完成了搭建 FTP 服务器的实验任务。
