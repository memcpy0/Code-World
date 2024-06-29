@[toc]

前面在【数据库基础】中总结了常用的SQL语句，这里要手动来实践一下，需要先安装好MySQL。为此写下这篇文章，记录安装过程。 

---
# 1. 下载 
先要在数据库的[官网](http://www.mysql.com)下载[MySQL](https://downloads.mysql.com/archives/community/)。 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212311912698.png)

选择和自己**系统位数**相对应的版本，点击右边的`Download`，此时会进到另一个页面，同样在接近页面底部的地方找到如下图所示的位置：
![img](https://img2018.cnblogs.com/blog/1556823/201812/1556823-20181220194715840-436169502.png)
不用理会上面的登录和注册按钮，直接点击`No thanks, just start my download` 就可以下载。

---
# 2. 安装 
下载完成后我们得到的是一个压缩包，将其解压，我们就可以得到MySQL 的软件本体了（一个文件夹），可以把它放在你想安装的位置。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212311914179.png)
如果下载的是一个安装器，按照如下步骤：
- 选择 I accept 然后点击next进入下一步。
- 选择Developer Default，然后点击next进入下一步。
	![](https://img-blog.csdnimg.cn/20200419094613383.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
- 这一步是检查安装条件，直接点击next进入下一步就可以了。
	![](https://img-blog.csdnimg.cn/2020041909463574.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
- 直接点击execute执行就可以了，执行完后点击next进入下一步。	
	![](https://img-blog.csdnimg.cn/20200419095112900.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
- 继续点击next进入下一步。	
	![](https://img-blog.csdnimg.cn/20200419094657588.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
	![](https://img-blog.csdnimg.cn/20200419094728517.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
- 选择第一个然后点击next进入下一步。		
	![](https://img-blog.csdnimg.cn/20200419095745217.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- 直接点击next进入下一步就可以了。	
	![](https://img-blog.csdnimg.cn/20200419094808740.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
	![](https://img-blog.csdnimg.cn/20200419094825294.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
- 设置root密码然后点击next进入下一步。	
	![](https://img-blog.csdnimg.cn/20200419095203626.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
- 点击next进入下一步。
	![](https://img-blog.csdnimg.cn/20200419094841196.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
- 点击execute执行。
	![](https://img-blog.csdnimg.cn/20200419095236700.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- 点击next。	
	![](https://img-blog.csdnimg.cn/20200419095309456.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
	![](https://img-blog.csdnimg.cn/2020041909532362.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- 点击finish。
	![](https://img-blog.csdnimg.cn/2020041909534876.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 3. 配置
## 3.1 配置环境变量
环境变量里面有很多选项，这里我们只用到 `Path` 这个参数。为什么在初始化的开始要添加环境变量呢？**在CMD中输入一个可执行程序的名字，Windows会先在「环境变量中的 `Path` 所指的路径」中寻找一遍，如果找到了就直接执行，没找到就在当前工作目录找，如果还没找到，就报错**。我们添加环境变量的目的，就是能在任意一个黑框直接调用MySQL中的相关程序、而不用总是修改工作目录，大大简化操作。

在系统变量中新建 `MYSQL_HOME` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212311949127.png)
在系统变量中找到并**双击** `Path` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212311951551.png)
点击新建：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212311951049.png)
最后点击确定。

**如何验证是否添加成功**？右键开始菜单（屏幕左下角），选择命令提示符(管理员)，打开黑框，敲入 `mysql` ，回车。 如果提示 `Can't connect to MySQL server on 'localhost'` 则证明添加成功； 如果提示 `mysql不是内部或外部命令，也不是可运行的程序或批处理文件` 则表示添加添加失败，请重新检查步骤并重试。
## 3.2 新建配置文件
（这是对MySQL5.7.24的，不一定现在还需要）新建一个文本文件，内容如下：
```java
[mysql]
default-character-set=utf8

[mysqld]
character-set-server=utf8
default-storage-engine=INNODB
sql_mode=STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION
```
把上面的文本文件另存为，在保存类型里选所有文件(*.*)，文件名叫 `my.ini` ，存放的路径为MySQL的根目录（例如 `D:\software\mysql-5.7.24-winx64` ，根据自己的MySQL目录位置修改）。上面代码意思就是配置数据库的默认编码集为utf-8和默认存储引擎为INNODB。
## 3.3 初始化MySQL
（这是对MySQL5.7.24的，不一定现在还需要）在刚才的黑框中敲入 `mysqld --initialize-insecure` ，回车，稍微等待一会，如果出现没有出现报错信息（如下图）则证明 `data` 目录初始化没有问题，此时再查看MySQL目录下已经有 `data` 目录生成。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212312004940.png)
## 3.4 注册并启动MySQL服务
在黑框里敲入 `mysqld -install` ，回车。现在你的计算机上已经安装好了MySQL服务了。
> 安装MySQL时，需要将它注册为一个服务，然后启动服务（可以设置为自动或手动类型）。==安装了MySQL，你的机器就可以称为MySQL服务器了，可以对外提供数据库服务==。**所谓服务器，就是安装了某种服务软件、可以对外提供服务的机器**。

在黑框里敲入 `net start mysql` ，回车。
```bash
net start mysql  // 启动mysql服务
net stop mysql  // 停止mysql服务
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212312007766.png)

## 3.5 修改默认账户密码
在黑框里敲入 `mysqladmin -u root password 1234` ，这里的`1234`就是指默认管理员（即root账户）的密码，可以自行修改成你喜欢的。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212312008456.png)

**至此，MySQL解压版安装完毕**！安装完成后进入MySQL的安装目录，其目录下的文件如下： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212312014960.png)
`bin` 目录下保存了MySQL常用的命令工具以及管理工具，`data` 目录是MySQL默认用来保存数据文件以及日志文件的地方，`docs` 目录下是MySQL的帮助文档，`include` 目录和 `lib` 目录是MySQL所依赖的头文件以及库文件（MySQL是C编写的），`share` 目录下保存目录文件以及日志文件。

---
# 4. 登录MySQL
右键开始菜单，选择命令提示符，打开黑框。 在黑框中输入，`mysql -uroot -p1234` ，回车，出现下图且左下角为 `mysql>` ，则登录成功。**到这里你就可以开始你的MySQL之旅了**！退出mysql可以用 `exit` 或 `quit` 命令。登陆参数：
```bash
mysql -u用户名 -p密码 -h要连接的mysql服务器的ip地址(默认127.0.0.1) -P端口号(默认3306)
```

---
# 5. 卸载MySQL

如果你想卸载MySQL，也很简单。右键开始菜单，选择命令提示符(管理员)，打开黑框。敲入 `net stop mysql`，回车。再敲入`mysqld -remove mysql`，回车。最后删除MySQL目录及相关的环境变量。**至此，MySQL卸载完成**！

---
# 6. 关于修改数据库名字
在低版本的MySQL中提供了一个命令rename来修改数据库的名称（注意不是数据库的表），不过这个命令会造成数据的丢失，所以这个命令已经被删掉，现在如果想要重命名一个数据库名称的话，需要先导出原有数据库中的所有数据，然后新建一个数据库，然后把导出的数据导入到新的数据库中，最后删掉原来的数据库。整个过程比较繁琐，数据量大的时候会非常浪费时间，所以不推荐修改数据库的名字，因此在给数据库命名的时候要考虑好应该给数据库起一个合理的名字。

---
# 7. 拓展：pycharm操作MySQL
打开pycharm，按照图片上的指示点击绿色“+”然后选择MySQL，输入数据库名， 然后设置一下名字、密码，最后测试连接成功后，点击apply就可以了。 连接后就可以在pycharm里面写MySQL的命令了，同理Java的Intellij IDEA也可以这样设置。 

