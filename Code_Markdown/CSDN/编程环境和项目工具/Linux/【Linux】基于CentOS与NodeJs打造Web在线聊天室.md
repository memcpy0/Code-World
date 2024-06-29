@[toc]

介绍如何使用 Nodejs + Socket.IO 打造 Web 在线聊天室，旨在熟悉 Socket 的环境基础搭建与使用。

# Socket.IO 简介与基础环境搭建
## 关于 Socket.IO

[Socket.IO](https://github.com/orgs/socketio/repositories) 可以实现**在浏览器和服务器之间实时双向通信**，本节将详细介绍 Socket.IO 的搭建与部署，并在结束后完成 Socket.IO Web聊天室，帮助快速上手 Socket.IO。

![在这里插入图片描述](https://img-blog.csdnimg.cn/64239e389e3a4cd0a045d1d646cb4978.png)


## 安装 NodeJs

在 CentOS 环境中，可以直接使用 yum 安装 NodeJs（或者用 `wget` 下载，见[【Linux】基于CentOS搭建Node.js环境](https://memcpy0.blog.csdn.net/article/details/121055088)）
```bash
$ yum install nodejs -y
Loaded plugins: fastestmirror, security
Setting up Install Process
Determining fastest mirrors
epel                                                     | 4.7 kB     00:00     
epel/primary_db                                          | 6.1 MB     00:00     
extras                                                   | 3.4 kB     00:00     
extras/primary_db                                        |  29 kB     00:00     
os                                                       | 3.7 kB     00:00     
os/primary_db                                            | 4.7 MB     00:00     
updates                                                                               | 3.4 kB     00:00     
updates/primary_db                                                                    |  12 MB     00:00     
Resolving Dependencies
--> Running transaction check
---> Package nodejs.x86_64 0:0.10.48-3.el6 will be installed
--> Processing Dependency: libuv.so.0.10()(64bit) for package: nodejs-0.10.48-3.el6.x86_64
--> Running transaction check
---> Package libuv.x86_64 1:0.10.34-1.el6 will be installed
--> Finished Dependency Resolution

Dependencies Resolved

=============================================================================================================
 Package                Arch                   Version                            Repository            Size
=============================================================================================================
Installing:
 nodejs                 x86_64                 0.10.48-3.el6                      epel                 2.1 M
Installing for dependencies:
 libuv                  x86_64                 1:0.10.34-1.el6                    epel                  57 k

Transaction Summary
=============================================================================================================
Install       2 Package(s)

Total download size: 2.2 M
Installed size: 7.2 M
Downloading Packages:
(1/2): libuv-0.10.34-1.el6.x86_64.rpm                                                 |  57 kB     00:00     
(2/2): nodejs-0.10.48-3.el6.x86_64.rpm                                                | 2.1 MB     00:00     
-------------------------------------------------------------------------------------------------------------
Total                                                                         19 MB/s | 2.2 MB     00:00     
Running rpm_check_debug
Running Transaction Test
Transaction Test Succeeded
Running Transaction
  Installing : 1:libuv-0.10.34-1.el6.x86_64                                                              1/2 
  Installing : nodejs-0.10.48-3.el6.x86_64                                                               2/2 
  Verifying  : nodejs-0.10.48-3.el6.x86_64                                                               1/2 
  Verifying  : 1:libuv-0.10.34-1.el6.x86_64                                                              2/2 

Installed:
  nodejs.x86_64 0:0.10.48-3.el6                                                                              

Dependency Installed:
  libuv.x86_64 1:0.10.34-1.el6                                                                               

Complete!
```

# 安装 npm 包管理器

```bash
$ yum install npm -y
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/8765f85296f948aa8f77143e381b4c9a.png)

# 搭建 express 环境
## 创建目录

创建web环境目录

```bash
$ mkdir www
$ cd www
```
初始化环境目录（回车即可）——用来初始化项目，并生成一个 `package.json` 文件，该文件用来记录项目的一些基本配置信息，比如项目版本、项目依赖等等。该命令有一个可选参数 `-y` ，其含义是在项目初始化时，采用系统默认的基本配置信息。如果没有此参数，在 `npm init` 命令执行过程中，会有一系列参数让用户即时输入。

```bash
$ npm init -y
This utility will walk you through creating a package.json file.
It only covers the most common items, and tries to guess sane defaults.

See `npm help json` for definitive documentation on these fields
and exactly what they do.

Use `npm install <pkg> --save` afterwards to install a package and
save it as a dependency in the package.json file.

Press ^C at any time to quit.
name: (www) 
version: (0.0.0) 
description: 
entry point: (index.js) 
test command: 
git repository: 
keywords: 
author: 
license: (BSD) 
About to write to /root/www/package.json:

{
  "name": "www",
  "version": "0.0.0",
  "description": "",
  "main": "index.js",
  "scripts": {
    "test": "echo \"Error: no test specified\" && exit 1"
  },
  "author": "",
  "license": "BSD"
}


Is this ok? (yes) yes
```

关闭npm的https强制效验

```bash
$ npm config set strict-ssl false
```

## 安装 express

```bash
$ npm install express --save
npm WARN package.json www@0.0.0 No description
npm WARN package.json www@0.0.0 No repository field.
npm WARN package.json www@0.0.0 No README data
npm http GET https://registry.npmjs.org/express
npm http 200 https://registry.npmjs.org/express
npm http GET https://registry.npmjs.org/express/-/express-4.17.1.tgz
npm http 200 https://registry.npmjs.org/express/-/express-4.17.1.tgz
npm http GET https://registry.npmjs.org/accepts
npm http GET https://registry.npmjs.org/array-flatten/1.1.1
npm http GET https://registry.npmjs.org/body-parser/1.19.0
npm http GET https://registry.npmjs.org/content-disposition/0.5.3
npm http GET https://registry.npmjs.org/content-type
npm http GET https://registry.npmjs.org/cookie/0.4.0
......
```

# 搭建拉取 socket.io
## npm 拉取 socket.io

```bash
$ npm install socket.io  --save
......
```

## 安装 git

```bash
$ yum install git -y
Loaded plugins: fastestmirror,
              : security
Setting up Install Process
Loading mirror speeds from cached hostfile
Resolving Dependencies
--> Running transaction check
---> Package git.x86_64 0:1.7.1-10.el6_10 will be installed
--> Processing Dependency: perl-Git = 1.7.1-10.el6_10 for package: git-1.7.1-10.el6_10.x86_64
--> Processing Dependency: perl(Git) for package: git-1.7.1-10.el6_10.x86_64
--> Processing Dependency: perl(Error) for package: git-1.7.1-10.el6_10.x86_64
--> Running transaction check
---> Package perl-Error.noarch 1:0.17015-4.el6 will be installed
---> Package perl-Git.noarch 0:1.7.1-10.el6_10 will be installed
--> Finished Dependency Resolution

Dependencies Resolved

======================================
 Package
     Arch   Version          Repository
                                 Size
======================================
Installing:
 git x86_64 1.7.1-10.el6_10  updates
                                4.6 M
Installing for dependencies:
 perl-Error
     noarch 1:0.17015-4.el6  os  29 k
 perl-Git
     noarch 1.7.1-10.el6_10  updates
                                 29 k

Transaction Summary
======================================
Install       3 Package(s)

Total download size: 4.7 M
Installed size: 15 M
Downloading Packages:
(1/3): git-1.7 | 4.6 MB     00:00     
(2/3): perl-Er |  29 kB     00:00     
(3/3): perl-Gi |  29 kB     00:00     
--------------------------------------
Total          | 4.7 MB     00:00     
Running rpm_check_debug
Running Transaction Test
Transaction Test Succeeded
Running Transaction
  Installing : 1:perl-Error-0.1   1/3 
  Installing : perl-Git-1.7.1-1   2/3 
  Installing : git-1.7.1-10.el6   3/3 
  Verifying  : 1:perl-Error-0.1   1/3 
  Verifying  : perl-Git-1.7.1-1   2/3 
  Verifying  : git-1.7.1-10.el6   3/3 

Installed:
  git.x86_64 0:1.7.1-10.el6_10        

Dependency Installed:
  perl-Error.noarch 1:0.17015-4.el6   
  perl-Git.noarch 0:1.7.1-10.el6_10   

Complete!
```

## 拉取 socket demo

```bash
$ git clone https://github.com/socketio/chat-example.git
Initialized empty Git repository in /root/www/chat-example/.git/
remote: Enumerating objects: 83, done.
remote: Counting objects: 100% (7/7), done.
remote: Compressing objects: 100% (5/5), done.
remote: Total 83 (delta 1), reused 4 (delta 1), pack-reused 76
Unpacking objects: 100% (83/83), done.
$ mv chat-example/* /root/www/
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/0bee01e623ad4752a4fb7972a0915480.png)

## 修改 package.json 文件
定位到 `www/package.json` 文件

```bash
{
  "name": "socket-chat-example",
  "version": "0.0.1",
  "description": "my first socket.io app",
  "dependencies": {
    "express": "^4.15.2",
    "socket.io": "^1.7.3"
  },
  "scripts": {
    "start": "node index.js"
  }
}
```

## 安装 npm 依赖

```bash
$ npm install
......
socket.io@1.7.4 node_modules/socket.io
├── object-assign@4.1.0
├── socket.io-adapter@0.5.0
├── has-binary@0.1.7 (isarray@0.0.1)
├── debug@2.3.3 (ms@0.7.2)
├── socket.io-parser@2.3.1 (isarray@0.0.1, component-emitter@1.1.2, json3@3.3.2, debug@2.2.0)
├── socket.io-client@1.7.4 (component-emitter@1.2.1, to-array@0.1.4, indexof@0.0.1, component-bind@1.0.0, backo2@1.0.2, object-component@0.0.3, parseuri@0.0.5, engine.io-client@1.8.6)
└── engine.io@1.8.5 (base64id@1.0.0, cookie@0.3.1, engine.io-parser@1.3.2, ws@1.1.5, accepts@1.3.3)
```

## 运行 Socket.IO

```bash
$ ls
app.json      index.html  LICENSE       package.json       README.md
chat-example  index.js    node_modules  package-lock.json
$ node index.js

/root/www/index.js:6
app.get('/', (req, res) => {
                         ^
SyntaxError: Unexpected token >
    at Module._compile (module.js:439:25)
    at Object.Module._extensions..js (module.js:474:10)
    at Module.load (module.js:356:32)
    at Function.Module._load (module.js:312:12)
    at Function.Module.runMain (module.js:497:10)
    at startup (node.js:119:16)
    at node.js:945:3
```

## 访问 socket 并测试 demo
访问 <您的 CVM IP 地址>:3000

