源码：https://gitcode.net/KnowledgePlanet/chatgpt
作业仓库：https://gitcode.net/CreativeAlliance 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305071319262.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304301642131.png)
- 在基础设置的基本必备服务搭建后，会进入**接口鉴权的简单开发**，这个模块开发后，大家就可以简单的使用了**小傅哥提供的 OpenAI 接口**了。—— 当然你如果自己有 OpenAI 接口，也可以直接使用。https://huggingface.co/也提供了一些可以免费使用的简单 Open-API
- 有了这部分内容的使用，后续会进入 **API-SDK 的开发**，以及网页的简单开发。通过这样的开发构成一套基本的模块服务。ChatGPT-WEB-UI -> API-SDK -> 鉴权 -> OpenAI 的使用。
- ChatGPT-WEB-UI 流程🏃🏻跑通后，就可以**逐步扩展其他服务模块**——应用服务模块SDK开发。让业务与场景结合，如关注公众号、公众号回复、企业微信机器人、交易支付购买授权Token。这个过程可以让 ChatGPT-WEB-UI 与各个模块结合使用。由于日后可能将其扩展到  
# 1. 说明
围绕类似ChatGPT的生成式服务，构建微服务应用架构体系组件。包括
- 用户鉴权
- 公众号
- 多方支付
- 企业微信

等对接方式，满足不同诉求的引用。并以模块化设计、积木式构建应用，让不用的场景诉求都可以配置化对接。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304301606557.png)

# 2. 工程结构
本项目核心是**微服务应用体系的构建**，通过讲解配置Docker、Nginx、SSL等环境、以及开发出**鉴权、认证、微信公众号、企业微信、支付宝交易**等模块的方式，完善体系的物料服务。

而==ChatGPT只是其中的一种产品形态而已，这种产品形态通过API的方式、与具体的物料服务模块解耦==。这样做的方式是因为**基础的物料【物料指SDK和服务】并不会频繁变化**，而离业务最近的API会随业务变动发生较多的改动。所以这样的应用架构方式，在互联网大厂中也是非常常见和常用的。

这些东西的价值在于架构思维，而不是永远的在CV+CRUD。有了这样的学习，学习的就不只是这样一个项目，而是可以把这个项目中所涉及的组件开发，都能对「**任意物料模块**」与「**需要对接的服务**」进行关联打通使用。方便用到项目。

接下来我们再以工程拓扑的视角看下这套需要开发的系统；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304301636373.png)
如拓扑结构，系统从上到下以不同的产品形态，统一调用**封装的服务API**进行功能的流转。==API系统中所处理的核心动作，会以各个物料模块进行实现==。所以这里会拆分出标准的 **ChatGPT-API 业务系统**，之后再由各个模块系统支撑。到具体的模块中再进行详细的系统设计。
# 3. 技术说明
此项目使用到 SpringBoot、MyBatis、MySQL、Redis 等技术栈，但因**本项目主要以小成本，轻量维护的实际使用为主**，所以**不会过多引入分布式技术栈**。所以在设计实现上，主要以小而美、小而精、能匹配到真实场景的使用为主——==分布式技术栈是为了更大规模的体量使用，但也会为此付出运维和应用服务器成本==。所以一些中小厂的项目或者创业类型的项目，都会优先**更轻量级技术栈**使用，以此减少这部分成本。

除技术栈的使用外，涉及到开发工具包括；IntelliJ IDEA、**WebStorm**、Docker、Protainter、**Nginx**、Git、Maven、Navicat、**SSH工具等**，以满足开发代码中的使用。

可能还会涉及少部分[Next.js](http://Next.js)、Typescript 等前端知识，方便做 Web UI 的开发。

# 3. DEV-OPS：工程搭建
## 3.1 提交工程
在做作业仓库https://gitcode.net/CreativeAlliance，按照创建标准【代码仓库名称标准，项目-星球用户编号-作者ID】，例如：chatgpt-dev-ops-1-xfg】创建一个自己的代码仓库。把本地代码提交到仓库。只需要用CSDN的账户名和密码，即可完成提交。
## 3.2 Docker环境配置
### 3.2.1 本地安装
Docker的安装非常简单，只用选择适合的机器版本，直接一步步安装即可。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305071337293.png)
安装后，可以在Docker操作界面拉取、推送镜像，部署程序等，还可在CMD、Intellij IDEA Terminal中使用Docker命令，操作Docker。但它还提示：Docker Desktop requires a newer WSL kernel version，Update the WSL kernel by running "wsl --update" 。根据提示，我发现适用于Linux的Windows子系统已安装，但适用于Linux的Windows子系统没有已安装的分发版。
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305071345382.png)
直接wsl --update更新WSL，一开始失败了，好像是开了Clash的缘故。更新完成，再打开Docker Desktop就是：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305071356566.png)
### 3.2.2 云服务器上安装
我使用的是轻量应用服务器1核2G，以后肯定要换个更好的，下面的过程也还要执行一遍，现在先凑合着用吧。
#### 1. 查看系统的内核版本
x86 64系统，32位是不能安装Docker的：
```bash
[root@iZbp1b2iy0neesie59eprgZ ~]# uname -r
4.18.0-193.14.2.el8_2.x86_64
```
#### 2. yum更新到最新版本
显示Complete就代表完成了，整个过程需要5-10分钟左右。
```bash
[root@iZbp1b2iy0neesie59eprgZ ~]# sudo yum update
Complete!
```
#### 3. 安装Docker所需的依赖包
过程需要1-3分钟左右：
```bash
[root@iZbp1b2iy0neesie59eprgZ ~]# sudo yum install yum-utils device-mapper-persistent-data lvm2
```
安装软件出现“错误：为仓库 ‘appstream’ 下载元数据失败 : Cannot prepare internal mirrorlist: No URLs in mirrorlist“。出现这个错误提示的原因是在2022年1月31日，CentOS 团队从官方镜像中移除 CentOS 8 的所有包。因为 CentOS 8 已于2021年12月31日停止官方服务了，但软件包仍在官方镜像上保留了一段时间。现在他们被转移到 [https://vault.centos.org](https://links.jianshu.com/go?to=https%3A%2F%2Fvault.centos.org)。如果仍需要运行 CentOS 8，你可以在 /etc/yum.repos.d 中更新一下源。使用 [vault.centos.org](https://links.jianshu.com/go?to=http%3A%2F%2Fvault.centos.org) 代替 [mirror.centos.org](https://links.jianshu.com/go?to=http%3A%2F%2Fmirror.centos.org) 即可。打开终端，输入 su 命令切换 root 用户登录，然后使用下面的命令更新源：
```bash
sed -i -e "s|mirrorlist=|#mirrorlist=|g" /etc/yum.repos.d/CentOS-*

sed -i -e "s|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g" /etc/yum.repos.d/CentOS-*
```
再次安装就成功了。
#### 4. 设置Docker的yum的源/设置docker-ce在线存储库
第二个是阿里云的软件源：
```bash
[root@iZbp1b2iy0neesie59eprgZ ~]# sudo yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo

[root@iZbp1b2iy0neesie59eprgZ ~]# sudo yum-config-manager --add-repo https://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo

[root@iZbp1b2iy0neesie59eprgZ ~]# sudo sed -i 's/download.docker.com/mirrors.aliyun.com\/docker-ce/g' /etc/yum.repos.d/docker-ce.repo
```
#### 5. 查看仓库所有的Docker版本
```bash
[root@iZbp1b2iy0neesie59eprgZ ~]# sudo yum list docker-ce --showduplicates | sort -r
```
#### 6. 安装Docker
```bash
[root@iZbp1b2iy0neesie59eprgZ ~]# sudo yum install docker-ce
```
#### 7. 启动docker-ce并设置开机自启动
```bash
[root@iZbp1b2iy0neesie59eprgZ ~]# sudo systemctl start docker
[root@iZbp1b2iy0neesie59eprgZ ~]# sudo systemctl enable docker
```
#### 8. 查看Docker版本
```bash
[root@iZbp1b2iy0neesie59eprgZ ~]# docker --version
```
## 3.3 Portainer环境配置
### 3.3.1 基础安装
#### 1. 拉取最新的Portainer
```bash
[root@iZbp1b2iy0neesie59eprgZ ~]# docker pull portainer/portainer
```
#### 2. 安装和启动
```bash
[root@iZbp1b2iy0neesie59eprgZ ~]# docker run -d --restart=always --name portainer -p 9000:9000 -v /var/run/docker.sock:/var/run/docker.sock portainer/portainer
```
#### 3. 访问Portainer
地址：112.124.203.218:9000  如果没有访问到，可能是服务器这边没有配置防火墙的接口。如果出现下图Your Portainer instance timed out for security purposes，运行 `[root@iZbp1b2iy0neesie59eprgZ ~]# sudo docker restart portainer` 即可
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305080330987.png)
登录后设置用户名和密码，现在显示结果如下：
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305080334678.png)
设置本地Docker即可。
![800](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305080336979.png)
### 3.3.2 链接服务
http://112.124.203.218:9000/#!/wizard/endpoints/create?envType=dockerStandalone
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305080340696.png)

## 3.4 Nginx环境配置
### 3.4.1 常用命令
```bash
停止
docker stop Nginx
重启
docker restart Nginx
删除服务
docker rm Nginx
删除镜像
docker rmi Nginx
进入服务
docker exec -it Nginx /bin/bash
```
### 3.4.2 基础安装
**restart 是重启策略，always是一直保持重启**。如果不设置可以把这条删掉。never\always
第1个80-容器端口，第2个80 -服务器端口，这样外部通过80端口即可访问
```bash
# docker run \
	--restart always \
	--name Nginx \
	-d \
	-p 80:80 \
	nginx
```
`Error starting userland proxy: listen tcp4 0.0.0.0:80: bind: address already in use.` 这个端口号被占用了。可以 `netstat -tanlp | grep 80` 找出占用80端口的进程PID，`kill pid` 杀死该进程，然后 `docker restart Nginx` 。
> 发现我之前宝塔面板就安装了LNMP环境（包含Nginx），不得不干掉它了：
> ```bash
> [root@iZbp1b2iy0neesie59eprgZ ~]# netstat -tunlp
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name    
tcp        0      0 0.0.0.0:9000            0.0.0.0:*               LISTEN      2018478/docker-prox 
tcp        0      0 0.0.0.0:80              0.0.0.0:*               LISTEN      813/nginx: master p 
tcp        0      0 0.0.0.0:21              0.0.0.0:*               LISTEN      777/pure-ftpd (SERV 
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN      1496/sshd           
tcp        0      0 0.0.0.0:8888            0.0.0.0:*               LISTEN      1033/python         
tcp        0      0 0.0.0.0:888             0.0.0.0:*               LISTEN      813/nginx: master p 
tcp        0      0 127.0.0.1:25            0.0.0.0:*               LISTEN      1477/master         
tcp6       0      0 127.0.0.1:8005          :::*                    LISTEN      154357/java         
tcp6       0      0 :::9000                 :::*                    LISTEN      2018482/docker-prox 
tcp6       0      0 :::3306                 :::*                    LISTEN      2211/mysqld         
tcp6       0      0 :::8080                 :::*                    LISTEN      154357/java         
tcp6       0      0 :::21                   :::*                    LISTEN      777/pure-ftpd (SERV 
tcp6       0      0 :::8088                 :::*                    LISTEN      1581168/java        
tcp6       0      0 ::1:25                  :::*                    LISTEN      1477/master         
udp        0      0 127.0.0.1:323           0.0.0.0:*                           780/chronyd         
udp6       0      0 ::1:323                 :::*                                780/chronyd 
> ``` 
 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305082112106.png)
Portainer 其实可以通过点击端口号就能直接访问相应的容器WEB界面的。但这时点击80:80是无法访问Nginx的，因为它默认的服务器地址为0.0.0.0，点开时候会提示无法访问。需要我们去设置local的ip地址，选择“环境”，点击local（本地），将Public IP后面的IP地址改成我们服务器的本地IP，完成后点击“更新环境”。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305082127467.png)

### 3.4.3 管理配置
Nginx使用包括的配置有：配置、首页、日志、SSL等，而**这些内容在默认基础安装的情况下，都是在Docker容器中的，不容易被管理和使用**。所以==一般我们在安装Nginx的时候，需要做映射处理==。这时，我们可以把在Docker上安装的Nginx配置信息拷贝到本地/宿主机环境中，之后做一些修改后，再去重新执行Nginx的安装【**重新安装前记得把之前的Nginx删掉**】。

我们所有配置都配置在 /etc/nginx/conf.d/default.conf，因为这个文件会被 /etc/nginx/nginx.conf文件包含进去：`include /etc/nginx/conf.d/*.conf;/` 。

#### 1. 进入Nginx
conf.d是个文件夹，所有这个文件夹下的Nginx配置文件default.conf都会被nginx.conf加载。所以后面的Nginx配置，我们也主要是修改这个文件。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305082150958.png)

#### 2. 拷贝Nginx
在云服务端，通过命令创建文件夹或SFTP工具手动创建。-p一次创建多个目录结构。
```bash
[root@iZbp1b2iy0neesie59eprgZ ~]# mkdir -p /data/nginx/conf
[root@iZbp1b2iy0neesie59eprgZ ~]# mkdir -p /data/nginx/html
[root@iZbp1b2iy0neesie59eprgZ ~]# mkdir -p /data/nginx/conf/conf.d
[root@iZbp1b2iy0neesie59eprgZ ~]# mkdir -p /data/nginx/logs
[root@iZbp1b2iy0neesie59eprgZ ~]# mkdir -p /data/nginx/ssl

[root@iZbp1b2iy0neesie59eprgZ ~]# docker container cp Nginx:/etc/nginx/nginx.conf /data/nginx/conf
[root@iZbp1b2iy0neesie59eprgZ ~]# docker container cp Nginx:/etc/nginx/conf.d/default.conf /data/nginx/conf/conf.d
[root@iZbp1b2iy0neesie59eprgZ ~]# docker container cp Nginx:/usr/share/nginx/html/index.html /data/nginx/html
```
#### 3. 部署Nginx
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305082205338.png)

```bash
[root@iZbp1b2iy0neesie59eprgZ ~]# docker stop Nginx
Nginx
[root@iZbp1b2iy0neesie59eprgZ ~]# docker rm Nginx
Nginx
[root@iZbp1b2iy0neesie59eprgZ ~]# docker run --restart always --name Nginx -d -p 80:80 -v /data/nginx/conf/conf.d:/etc/nginx/conf.d -v /data/nginx/conf/nginx.conf:/etc/nginx/nginx.conf -v /data/nginx/html:/usr/share/nginx/html -v /data/nginx/logs:/var/log/nginx -v /data/nginx/ssl:/ect/nginx/ssl --privileged=true nginx
```
如果再修改其他配置，直接重启：sudo service nginx restart 或在portainer页面操作。
- -v是映射配置，后面除了html、conf外还有其他配置需要映射

### 3.4.4 证书安装【扩展】
#### 1. 创建证书
SSL 免费的证书，在各个云服务厂商都有提供，可以自己申请。这里以阿里云举例；
阿里云免费域名证书：https://yundun.console.aliyun.com/?p=cas#/certExtend/free/cn-hangzhou
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305082217441.png)
步骤1；通过免费的方式创建 SSL，之后通过引导的 DNS 方式进行验证。其实就是在你的域名里配置下验证信息。
步骤2；申请后，3-5分钟左右 DNS 会验证通过，这个时候你直接下载 Nginx 的 SSL 包即可。里面有2个文件【x.key、x.pem】
#### 2. 准备内容
##### 单个证书
把下载好的 SSL 文件解压到桌面，你会得到一个文件夹，里面含有 x.key、x.pem 两个文件。
创建一个default.conf 这个文件配置的 SSL 信息
```conf
server {    
	listen       80;    
	listen  [::]:80;    
	server_name  openai.xfg.im;    
	rewrite ^(.*) https://$server_name$1 permanent;
}
server {    
	listen       443 ssl;    
	server_name  openai.xfg.im;    
	ssl_certificate      /etc/nginx/ssl/9740289_openai.xfg.im.pem;    
	ssl_certificate_key  /etc/nginx/ssl/9740289_openai.xfg.im.key;   
	ssl_session_cache    shared:SSL:1m;    
	ssl_session_timeout  5m;    
	ssl_ciphers  HIGH:!aNULL:!MD5;    
	ssl_prefer_server_ciphers  on;    
	location / {        
		proxy_set_header   X-Real-IP         $remote_addr; 
		proxy_set_header   Host              $http_host;        
		proxy_set_header   X-Forwarded-For   $proxy_add_x_forwarded_for; 
		root   /usr/share/nginx/html;        
		index  index.html index.htm;    
	}    
	error_page   500 502 503 504  /50x.html;    
	location = /50x.html {        
		root   /usr/share/nginx/html;    
	}
}
```
你可以复制这份文件，在自己本地创建。注意**修改域名和SSL文件路径**。
##### 多个证书
如果你需要给1个以上的域名配置SSL，那么可以配置多组 server 如下；
```conf
server {    
	listen       80;    
	listen  [::]:80;    
	server_name  itedus.cn;    
	rewrite ^(.*) https://$server_name$1 permanent;
}
server {    
	listen       443 ssl;    
	server_name  itedus.cn;    
	ssl_certificate      /etc/nginx/ssl/9740289_itedus.cn.pem;    
	ssl_certificate_key  /etc/nginx/ssl/9740289_itedus.cn.key;   
	ssl_session_cache    shared:SSL:1m;    
	ssl_session_timeout  5m;    
	ssl_ciphers  HIGH:!aNULL:!MD5;    
	ssl_prefer_server_ciphers  on;    
	
	location / {        
		proxy_set_header   X-Real-IP         $remote_addr; 
		proxy_set_header   Host              $http_host;        
		proxy_set_header   X-Forwarded-For   $proxy_add_x_forwarded_for; 
		root   /usr/share/nginx/html;        
		index  index.html index.htm;    
	}    
	error_page   500 502 503 504  /50x.html;    
	location = /50x.html {        
		root   /usr/share/nginx/html;    
	}
}

server {    
	listen       80;    
	listen  [::]:80;    
	server_name  chatgpt.itedus.cn;    
	rewrite ^(.*) https://$server_name$1 permanent;
}
server {    
	listen       443 ssl;    
	server_name  chatgpt.itedus.cn;    
	ssl_certificate      /etc/nginx/ssl/9740289_chatgpt.itedus.cn.pem;    
	ssl_certificate_key  /etc/nginx/ssl/9740289_chatgpt.itedus.cn.key;   
	ssl_session_cache    shared:SSL:1m;    
	ssl_session_timeout  5m;    
	ssl_ciphers  HIGH:!aNULL:!MD5;    
	ssl_prefer_server_ciphers  on;    
	
	location / {        
		proxy_pass http://180.76.119.100:3002;
		proxy_http_version 1.1;
		chunked_transfer_encoding off;
		proxy_buffering off;
		proxy_cache off;    
	}    
	error_page   500 502 503 504  /50x.html;    
	location = /50x.html {        
		root   /usr/share/nginx/html;    
	}
}
```
#### 3. 上传文件
你可以通过 SFTP 工具或者 mkdir -p、touch 命令创建一些服务器本地用于映射的文件夹和文件，这里小傅哥使用了Termius工具进行创建操作。
![sdf](https://article-images.zsxq.com/FjjLOkAA7vX0bDLP-18ObC4T1uu3)
- 文件1；html 
- 文件2；ssl - 把本地的 ssh 文件上传进来
- 文件3；conf - 在 conf 下有个 conf.d 的文件夹，把 <a href="http://default.conf" target="_blank">default.conf</a> 上传进去。而 <a href="http://nginx.conf" target="_blank">nginx.conf</a> 传到 conf 中。
- 文件4；logs - 创建日志

#### 4. 启动服务
 在  nginx.conf  的配置文件有这么一句；`include /etc/nginx/conf.d/*.conf;` 那么只要是 conf.d 文件夹下的文件都会被加载。所以直接在 `conf.d/default.conf` 配置 SSL 就会被加载。接下来重新安装 Nginx 即可。安装前记得删除 Nginx 。你可以用命令 `docker stop Nginx、docker rm Nginx` 或者在 Portainer 中操作即可：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305082227526.png)

[Nginx简明教程](https://dunwu.github.io/nginx-tutorial/#/nginx-quickstart)
## 3.5 服务镜像构建和容器部署