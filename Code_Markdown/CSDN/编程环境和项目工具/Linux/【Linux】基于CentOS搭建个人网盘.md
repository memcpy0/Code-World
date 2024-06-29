@[toc]

# 1. 准备域名
## 域名注册
如果您还没有域名，可以[在腾讯云上选购](https://dnspod.cloud.tencent.com/?fromSource=lab)。
##  域名解析
域名购买完成后, 需要将域名解析到实验云主机上，实验云主机的 IP 为：

```cpp
42.193.188.174
```

 在腾讯云购买的域名，可以[到控制台添加解析记录](https://console.cloud.tencent.com/domain)。域名设置解析后需要过一段时间才会生效，通过 ping 命令检查域名是否生效，如：

```cpp
ping www.yourdomain.com
```
如果 ping 命令返回的信息中含有你设置的解析的 IP 地址，说明解析成功。

# 2. 安装 Seafile
## 安装依赖环境

使用 yum 安装 Python 及 MySQL：

```bash
$ yum install python python-setuptools python-imaging python-ldap python-memcached MySQL-python mariadb mariadb-server
```

启动 MariaDB 服务：

```bash
$ sudo systemctl start mariadb.service
$ sudo systemctl enable mariadb.service
Created symlink from /etc/systemd/system/multi-user.target.wants/mariadb.service to /usr/lib/systemd/system/mariadb.service.
```

配置 MySQL：

```bash
$ /usr/bin/mysql_secure_installation

NOTE: RUNNING ALL PARTS OF THIS SCRIPT IS RECOMMENDED FOR ALL MariaDB
      SERVERS IN PRODUCTION USE!  PLEASE READ EACH STEP CAREFULLY!

In order to log into MariaDB to secure it, we'll need the current
password for the root user.  If you've just installed MariaDB, and
you haven't set the root password yet, the password will be blank,
so you should just press enter here.

Enter current password for root (enter for none): 
OK, successfully used password, moving on...

Setting the root password ensures that nobody can log into the MariaDB
root user without the proper authorisation.

Set root password? [Y/n]  
New password: 
Re-enter new password: 
Password updated successfully!
Reloading privilege tables..
 ... Success!


By default, a MariaDB installation has an anonymous user, allowing anyone
to log into MariaDB without having to have a user account created for
them.  This is intended only for testing, and to make the installation
go a bit smoother.  You should remove them before moving into a
production environment.

Remove anonymous users? [Y/n] 
 ... Success!

Normally, root should only be allowed to connect from 'localhost'.  This
ensures that someone cannot guess at the root password from the network.

Disallow root login remotely? [Y/n] 
 ... Success!

By default, MariaDB comes with a database named 'test' that anyone can
access.  This is also intended only for testing, and should be removed
before moving into a production environment.

Remove test database and access to it? [Y/n] 
 - Dropping test database...
 ... Success!
 - Removing privileges on test database...
 ... Success!

Reloading the privilege tables will ensure that all changes made so far
will take effect immediately.

Reload privilege tables now? [Y/n] 
 ... Success!

Cleaning up...

All done!  If you've completed all of the above steps, your MariaDB
installation should now be secure.

Thanks for using MariaDB!
```

配置过程输入参数如截图所示，其中 New password 和 Re-enter new password 字段都设置为 a7eKyAvK，其他字段一路回车使用默认值。
## 安装 Seafile
下载 Seafile 安装包：

```bash
$ wget https://mc.qcloudimg.com/static/archive/3d8addbe52be88df4f6139ec7e35b453/seafile-server_5.1.4_x86-64.tar.gz
```

解压 Seafile 安装包：

```bash
$ tar -zxvf seafile-server_5.1.4_x86-64.tar.gz
```
安装 Seafile 安装包：

```bash
$ sudo mkdir -p /opt/seafile/installed
$ sudo mv seafile-server_5.1.4_x86-64.tar.gz /opt/seafile/installed
$ sudo mv seafile-server-5.1.4/ /opt/seafile
$ cd /opt/seafile/seafile-server-5.1.4
$ sudo ./setup-seafile-mysql.sh
```
执行过程输入参数如截图所示，`server name` 字段输入 Seafile，`This server's ip or domain` 字段输入教程第一步申请的域名，`1 or 2` 字段选择 1，mysql 的 `root password` 字段输入 `a7eKyAvK` ，其他字段一路回车使用默认值：
