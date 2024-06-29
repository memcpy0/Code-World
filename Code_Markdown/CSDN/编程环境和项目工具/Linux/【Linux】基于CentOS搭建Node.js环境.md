@[toc]



# 实验内容
Node.js 是运行在服务端的 JavaScript，是基于 Chrome JavaScript V8 引擎建立的平台。软件环境是CentOS 7.2 64 位。

# 安装 Node.js 环境  
## 下载并安装 Node.js

下载最新的稳定版 v6.10.3 到本地

```bash
$ wget https://nodejs.org/dist/v6.10.3/node-v6.10.3-linux-x64.tar.xz
```

下载完成后, 将其解压

```bash
$ tar xvJf node-v6.10.3-linux-x64.tar.xz
$ ls
node-v6.10.3-linux-x64  node-v6.10.3-linux-x64.tar.xz
$ ls node-v6.10.3-linux-x64
bin  CHANGELOG.md  include  lib  LICENSE  README.md  share
```

将解压的 `Node.js` 目录移动到 `/usr/local` 目录下

```bash
$ mv node-v6.10.3-linux-x64 /usr/local/node-v6
$ ls
node-v6.10.3-linux-x64.tar.xz
```

配置 `node` 软链接到 `/bin` 目录

```bash
$ ln -s /usr/local/node-v6/bin/node /bin/node
$ ls /bin/node
/bin/node
```

# 配置和使用 npm
## 配置 npm
`npm` 是 `Node.js` 的包管理和分发工具。它可以让 `Node.js` 开发者能够更加轻松的共享代码和共用代码片段。

下载 `node` 的压缩包中已经包含了 `npm` , 我们只需要将其软链接到 `bin` 目录下即可

```bash
$ ln -s /usr/local/node-v6/bin/npm /bin/npm
$ ls /bin/npm
/bin/npm
```

## 配置环境变量

将 `/usr/local/node-v6/bin` 目录添加到 `$PATH` 环境变量中，可以方便地使用通过 `npm` 全局安装的第三方工具

```bash
$ echo 'export PATH=/usr/local/node-v6/bin:$PATH' >> /etc/profile
```

生效环境变量

```bash
$ source /etc/profile
```

## 使用 npm

通过 `npm` 安装进程管理模块 `forever`

```bash
$ npm install forever -g
```

# 完成实验

恭喜！您已经成功完成了搭建 Node.js 环境的实验任务。

