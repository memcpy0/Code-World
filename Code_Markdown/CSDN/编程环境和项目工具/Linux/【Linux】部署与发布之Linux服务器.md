@[toc]

在 Linux 服务器上建设应用程序的部署环境。

---

# 1. 更新系统
## 更新操作系统
执行如下命令，将系统软件更新到最新的版本。

```bash
$ yum update -y
```

安装 Epel Release

安装 Epel Release 可以让我们拥有更加丰富的软件资源，比如后续我们要使用的 Nginx ，就是包含在 EPEL Release 中的。

执行如下命令来安装 Epel Release

yum install epel-release -y

# 2. 配置 Nginx

    任务时间：5min ~ 10min

安装 Nginx

在 Linux 下，我们安装 Nginx 非常的容易，只需要执行如下命令

yum install nginx -y

然后，我们可以执行 如下命令来启动 Nginx

systemctl start nginx.service
systemctl enable nginx.service

这个时候，你可以访问你的服务器IP地址，查看默认页面：http://<您的 CVM IP 地址>
使用 Nginx

如果你需要创建一个新的虚拟主机，可以在 /etc/nginx/conf.d/ 文件夹下创建新的配置文件，比如 test.conf

文件内容可以参考下面的代码

server {
    listen       80;
    server_name  test.com;
    root         /usr/share/nginx/html;

    location / {
    }
 }

将其中的 root 和 server_name 改为你自己的文件路径和域名即可。
卸载 Nginx

卸载 Nginx 时，只需要执行如下代码即可

yum remove nginx -y

# 3. 配置 Apache

    任务时间：5min ~ 10min

 安装 Apache

在 Linux 下，安装 Apache 非常简单，执行如下命令来安装 Apache

yum install httpd -y

安装完成后，可以执行如下命令，来启动 Apache 并设置 Apache 的开机自启动

systemctl enable httpd.service 
systemctl start httpd.service

这个时候，你可以访问你的服务器IP地址，查看默认页面：http://<您的 CVM IP 地址>
使用 Apache

如果你需要创建一个新的虚拟主机，可以在 /etc/httpd/conf.d/ 文件夹下创建新的配置文件，比如 test.conf

文件内容可以参考下面的代码

<VirtualHost *:80>
  DocumentRoot "/www/docs/host.example.com"
  ServerName host.example.com
</VirtualHost>

将其中的 DocumentRoot 和 ServerName 改为你自己的文件路径和域名即可。
卸载 Apache

卸载 Apache 时，只需要执行如下代码即可

yum remove httpd -y

# 4. 配置 MySQL

    任务时间：5min ~ 10min

安装 MySQL

执行如下命令，即可安装 MySQL ，这里我们安装的是 MySQL 的一个分支版本 MariaDB

yum install mariadb mariadb-server -y

安装完成后，执行命令来启动 MariaDB

systemctl start mariadb.service
systemctl enable mariadb.service

启动 MySQL 成功后，我们来配置 MySQL 的安全安装

mysql_secure_installation

执行命令后，我们照着命令的说明，一步步执行下去。

image image

执行完成后，我们就完成了 MySQL 数据库的配置。
创建一个数据库

如果你想要创建一个数据库，可以这样做。

首先，在命令行中输入 mysql -uroot -p，并根据提示输入密码

image

可以看到这样的界面

执行如下命令来创建数据库

create database testDb;

image
卸载 MySQL

如果需要卸载 MySQL ，只需要执行如下命令

yum remove mariadb mariadb-server - y

