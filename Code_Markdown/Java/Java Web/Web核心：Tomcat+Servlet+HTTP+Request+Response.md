Java Web中用到的技术栈中，只有HTTP协议、Servlet、JSP以及Tomcat是没有学习过的，所以整个Web核心就是学习这些技术。整个Web核心，总共有如下学习内容：
* HTTP、Tomcat、Servlet
* Request(请求)、Response(响应)
* JSP、会话技术(Cookie、Session)
* Filter(过滤器，过滤掉某些请求)、Listener(监听器)
* Ajax、Vue、ElementUI
* 综合案例

`Request` 是从客户端向服务端发出的请求对象，`Response` 是从服务端响应给客户端的结果对象，JSP是动态网页技术，**会话技术用来存储客户端和服务端交互所产生的数据**，过滤器是用来拦截客户端的请求、监听器是用来监听特定事件，Ajax、Vue、ElementUI都是属于前端技术。

这些技术该如何来使用，后面会进行详细讲解。接下来学习HTTP、Tomcat和Servlet。

---
# 1. HTTP
## 1.1 简介
*HTTP, HyperText Transfer Protocol*  超文本传输协议，规定了浏览器和服务器之间==数据传输的规则==。所以，==学习HTTP主要就是学习请求和响应数据的具体格式内容==。
* 数据传输的规则指的是**请求数据和响应数据需要按照指定格式进行传输**。
* 如果想知道具体的格式，打开浏览器，打开开发者工具，点击 `Network` 来查看某一次请求的请求数据和响应数据具体的格式内容，如下图所示:![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302052201584.png)

HTTP协议有它自己的一些特点，分别是:
* 基于TCP协议：面向连接，安全。**TCP是一种面向连接的（建立连接之前是需要经过三次握手）、可靠的、基于字节流的传输层通信协议**，在数据传输方面更安全。
* 基于请求-响应模型的：**一次请求对应一次响应**，请求和响应是一一对应关系
* HTTP协议是无状态协议：对于事物处理没有记忆能力。**每次请求-响应都是独立的**
	无状态指的是客户端发送HTTP请求给服务端之后，服务端根据请求响应数据，响应完后，不会记录任何信息。这种特性有优点也有缺点，缺点是**多次请求间不能共享数据**，优点是**速度快**。请求之间无法共享数据会引发的问题，如：京东购物，加入购物车和去购物车结算是两次请求，
	- HTTP协议的无状态特性，加入购物车请求响应结束后，并未记录加入购物车是何商品
	- 发起去购物车结算的请求后，因为无法获取哪些商品加入了购物车，会导致此次请求无法正确展示数据

  具体使用时，我们发现京东是可以正常展示数据的，原因是Java早已考虑到这个问题，并提出了**使用会话技术(Cookie、Session)来解决这个问题**。具体如何来做，后面会详细讲到。刚提到HTTP协议是规定了请求和响应数据的格式，那具体的格式是什么呢？
## 1.2 请求数据格式 
请求数据总共分为三部分内容，分别是==请求行==、==请求头==、==请求体==。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302052225749.png)
请求行：HTTP请求中的第一行数据，请求行包含三块内容，分别是 `GET[请求方式] /[请求URL路径] HTTP/1.1[HTTP协议及版本]` 。请求方式有七种，最常用的是GET和POST。`/` 表示请求资源路径（这的 `/` 表示根目录），不同资源的路径不同。`HTTP/1.1` 表示协议版本。

请求头：第二行开始，格式为 `key: value` 形式。请求头中会包含若干个属性，常见的HTTP请求头有：
- `Host` ：表示请求的主机名
- `User-Agent` ：浏览器版本、用来处理浏览器兼容问题，例如Chrome浏览器的标识类似Mozilla/5.0 ...Chrome/79，IE浏览器的标识类似Mozilla/5.0 (Windows NT ...)like Gecko；
- `Accept` ：表示浏览器能接收的资源类型，如 `text/*` ，`image/*` 或者 `*/*` 表示所有都可以接收；
-  `Accept-Language` ：表示浏览器偏好的语言，服务器可以据此返回不同语言的网页；
-  `Accept-Encoding` ：表示浏览器可以支持的压缩类型，例如 `gzip, deflate` 等。

==这些数据有什么用处==？举例说明：服务端可以根据请求头中的内容来获取客户端的相关信息，有了这些信息服务端就可以处理不同的业务需求，比如:
* 不同浏览器解析HTML和CSS标签的结果会有不一致，所以就会导致相同代码在不同浏览器会出现不同的效果
* 服务端根据客户端请求头中的数据，获取到客户端的浏览器类型，就可以根据不同浏览器设置不同代码来达到一致的效果
* 这就是常说的浏览器兼容问题

请求体：POST请求特有的最后一部分，存储请求参数。如下图红线框内容就是请求体的内容，**请求体和请求头之间有一个空行隔开**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302052230977.png)
此时浏览器发送的是POST请求，为什么不能使用GET呢？这时就需要回顾GET和POST两个请求之间的区别了:
* **GET请求的请求参数在请求行中，没有请求体**，POST请求的请求参数在请求体中
* GET请求的请求参数大小有限制，POST没有

## 1.3 响应数据格式 
响应数据总共分为三部分内容，分别是==响应行==、==响应头==、==响应体==
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302052245919.png)
响应行：响应数据的第一行，响应行包含三块内容，分别是 `HTTP/1.1[HTTP协议及版本] 200[响应状态码] ok[状态码的描述]` 。关于响应状态码，有五类：

| 状态码分类 | 说明                                                         |
| ---------- | --------- |
| 1xx        | **响应中**（很少见）——临时状态码，表示请求已经接受，告诉客户端应该继续请求或者如果它已经完成则忽略它 |
| 2xx        | **成功**——表示请求已经被成功接收，处理已完成                 |
| 3xx        | **重定向**——重定向到其它地方：它让客户端再发起一个请求以完成整个处理。 |
| 4xx        | **客户端错误**——处理发生错误，责任在客户端，如：客户端的请求一个不存在的资源，客户端未被授权，禁止访问等 |
| 5xx        | **服务器端错误**——处理发生错误，责任在服务端，如：服务端抛出异常，路由出错，HTTP版本不支持等 |
状态码大全：https://cloud.tencent.com/developer/chapter/13553 

| 状态码 | 英文描述                               | 解释                                                         |
| ------ | -------------------------------------- | ------------------------------------------------------------ |
| 200    | **`OK`**                               | 客户端请求成功，即**处理成功**，这是我们最想看到的状态码     |
| 302    | **`Found`**                            | 指示所请求的资源已移动到由 `Location` 响应头给定的URL，浏览器会自动重新访问到这个页面 |
| 304    | **`Not Modified`**                     | 告诉客户端，你请求的资源至上次取得后，服务端并未更改，直接用你本地缓存吧。隐式重定向 |
| 400    | **`Bad Request`**                      | 客户端请求有**语法错误**，不能被服务器所理解                 |
| 403    | **`Forbidden`**                        | 服务器收到请求，但**拒绝提供服务**，比如：没有权限访问相关资源 |
| 404    | **`Not Found`**                        | **请求资源不存在**，一般是URL输入有误，或者网站资源被删除了  |
| 428    | **`Precondition Required`**            | **服务器要求有条件的请求**，告诉客户端要想访问该资源，必须携带特定的请求头 |
| 429    | **`Too Many Requests`**                | **太多请求**，可以限制客户端请求某个资源的数量，配合Retry-After(多长时间后可以请求)响应头一起使用 |
| 431    | **` Request Header Fields Too Large`** | **请求头太大**，服务器不愿意处理请求，因为它的头部字段太大。请求可以在减少请求头域的大小后重新提交。 |
| 405    | **`Method Not Allowed`**               | 请求方式有误，比如应该用GET请求方式的资源，用了POST     |
| 500    | **`Internal Server Error`**            | **服务器发生不可预期的错误**。服务器出异常了，赶紧看日志去吧 |
| 503    | **`Service Unavailable`**              | **服务器尚未准备好处理请求**，服务器刚刚启动，还未初始化好  |
| 511    | **`Network Authentication Required`**  | **客户端需要进行身份验证才能获得网络访问权限**              |

响应头：第二行开始，格式为 `key: value` 形式。响应头中会包含若干个属性，常见的HTTP响应头有:
- `Content-Type` ：表示该响应内容的类型，例如 `text/html, image/jpeg`  ，不同内容浏览器解析方法不同；
- `Content-Length` ：表示该响应内容的长度（字节数）；
- `Content-Encoding` ：表示该响应压缩算法，例如 `gzip` ；
- `Cache-Control` ：指示客户端应如何缓存，例如 `max-age=300` 表示可以最多缓存300秒

响应体： 最后一部分，存放响应数据。下图中 `<html>...</html>` 这部分内容就是响应体，它和响应头之间有一个空行隔开。
 
## 1.4 自定义服务器
在前面导入到IDEA中的HTTP项目中，有一个 `Server.java` 类，这里面就是自定义的一个服务器代码，主要使用到的是 `ServerSocket` 和 `Socket` 类。
```java
package com.itheima;

import sun.misc.IOUtils;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
/*
 * 自定义服务器
 */
public class Server {
    public static void main(String[] args) throws IOException {
        ServerSocket ss = new ServerSocket(8080); // 监听指定端口
        System.out.println("server is running...");
        while (true) {
            Socket sock = ss.accept();
            System.out.println("connected from " + sock.getRemoteSocketAddress());
            Thread t = new Handler(sock);
            t.start();
        }
    }
}

class Handler extends Thread {
    Socket sock;

    public Handler(Socket sock) {
        this.sock = sock;
    }

    public void run() {
        try (InputStream input = this.sock.getInputStream()) {
            try (OutputStream output = this.sock.getOutputStream()) {
                handle(input, output);
            }
        } catch (Exception e) {
            try {
                this.sock.close();
            } catch (IOException ioe) {
            }
            System.out.println("client disconnected.");
        }
    }

    private void handle(InputStream input, OutputStream output) throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(input, StandardCharsets.UTF_8));
        BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(output, StandardCharsets.UTF_8));
        // 读取HTTP请求
        boolean requestOk = false;
        String first = reader.readLine();
        if (first.startsWith("GET / HTTP/1.")) {
            requestOk = true;
        }
        for (;;) {
            String header = reader.readLine();
            if (header.isEmpty()) { // 读取到空行时, HTTP Header读取完毕
                break;
            }
            System.out.println(header);
        }
        System.out.println(requestOk ? "Response OK" : "Response Error");
        if (!requestOk) {
            // 发送错误响应:
            writer.write("HTTP/1.0 404 Not Found\r\n");
            writer.write("Content-Length: 0\r\n");
            writer.write("\r\n");
            writer.flush();
        } else {
            // 发送成功响应:
            //读取html文件，转换为字符串
            BufferedReader br = new BufferedReader(new FileReader("http/html/a.html"));
            StringBuilder data = new StringBuilder();
            String line = null;
            while ((line = br.readLine()) != null){
                data.append(line);
            }
            br.close();
            int length = data.toString().getBytes(StandardCharsets.UTF_8).length;

            writer.write("HTTP/1.1 200 OK\r\n");
            writer.write("Connection: keep-alive\r\n");
            writer.write("Content-Type: text/html\r\n");
            writer.write("Content-Length: " + length + "\r\n");
            writer.write("\r\n"); // 空行标识Header和Body的分隔
            writer.write(data.toString());
            writer.flush();
        }
    }
}
```
上面代码，大家不需要自己写，主要通过上述代码，只需要大家了解到：==服务器可以使用Java完成编写，是可以接受页面发送的请求、发送响应数据给前端浏览器的==。真正用到的Web服务器，我们不会自己写，都是使用目前比较流行的web服务器，比如Tomcat，它帮我们屏蔽了对HTTP协议进行解析等的代码。

---
# 2. Tomcat
## 2.1 简介
### 2.1.1 什么是Web服务器
Web服务器是一个应用程序（==软件==），**对HTTP协议的操作进行封装，使得程序员不必直接对协议进行操作，让Web开发更加便捷**。主要功能是"对外提供网上信息浏览服务（我们可以将Web项目部署到服务器中）。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302052350151.png)

Web服务器是安装在服务器端的一款软件，==将来我们把自己写的Web项目部署到Web Tomcat服务器软件中（Tomcat的webapp目录等），当Web服务器软件启动后，部署在Web服务器软件中的页面就可以直接通过浏览器来访问了==。

**Web服务器软件使用步骤**如下：
* 准备静态资源
* 下载安装Web服务器软件
* **将静态资源部署到Web服务器上**
* **启动Web服务器使用浏览器访问对应的资源**

上述内容在演示时，使用的是Apache下的Tomcat软件，至于Tomcat软件如何使用，后面会详细讲到。而对于Web服务器来说，实现的方案有很多，Tomcat只是其中的一种，除了Tomcat以外，还有很多优秀的Web服务器，比如：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302060014456.png)
### 2.1.2 Tomcat
Tomcat就是一款软件，主要是以学习如何去使用为主。具体从以下这些方向去学习：
1. 简介：初步认识下Tomcat
2. 基本使用：安装、卸载、启动、关闭、配置和项目部署，这些都是对Tomcat的基本操作
3. IDEA中如何创建Maven Web项目
4. IDEA中如何使用Tomcat，后面这两个都是以后开发经常会用到的方式

首先认识下Tomcat。
* Tomcat是Apache软件基金会一个核心项目，是一个开源免费的**轻量级**Web服务器，**支持Servlet/JSP少量JavaEE规范**。而WebLogic等重量级Web服务器，支持所有JavaEE规范，但收费。
* 概念中提到了JavaEE规范，那什么又是JavaEE规范呢—— *JavaEE, Java Enterprise Edition* ，Java企业版。指Java企业级开发的技术规范总和。包含13项技术规范：JDBC、JNDI、EJB、RMI、JSP、Servlet、XML、JMS、Java IDL、JTS、JTA、JavaMail、JAF。有些学过了，如JDBC、XML，有些要学如JSP、Servlet，还有些过时了如EJB（用Spring替代）。
* ==因为Tomcat支持Servlet/JSP规范，所以Tomcat也被称为Web容器、Servlet容器==。Servlet需要依赖Tomcat才能运行。
* Tomcat的官网：https://tomcat.apache.org/ ，从官网上可以下载对应的版本进行使用。

## 2.2 基本使用
Tomcat总共分两部分学习，先来学习Tomcat的基本使用，包括Tomcat的==下载、安装、卸载、启动和关闭==。
### 2.2.1 下载
直接从官网下载（不同的Tomcat要求不同的Java版本）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302060023382.png)

### 2.2.2 安装
Tomcat是绿色版，直接解压即可
* 将 `apache-tomcat-8.5.68-windows-x64.zip` 进行解压缩，会得到一个 `apache-tomcat-8.5.68` 的目录，Tomcat就已经安装成功。
  ==注意==，Tomcat在解压缩时，解压所在的目录可以任意，但最好解压到一个不包含中文和空格的目录，因为后期在部署项目时，如果路径有中文或者空格可能会导致程序部署失败。
* 打开 `apache-tomcat-8.5.68` 目录就能看到如下目录结构，每个目录中包含的内容需要认识下，
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302060037440.png)
  `bin` 目录下有两类文件，一种是以 `.bat` 结尾的，是Windows系统的可执行文件，一种是以 `.sh` 结尾的，是Linux系统的可执行文件。`webapps` 就是以后项目部署的目录

到此，Tomcat的安装就已经完成。
### 2.2.3 卸载
卸载比较简单，可以直接删除目录即可
### 2.2.4 启动
双击：`bin\startup.bat` 。启动后，通过浏览器访问  `http://localhost:8080` 能看到Apache Tomcat的内容，就说明Tomcat已经启动成功。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302060051452.png)
注意：启动的过程中，控制台有中文乱码，需要修改 `conf/logging.prooperties` 。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302060052024.png)
### 2.2.5 关闭
关闭有三种方式 
* 直接x掉运行窗口：强制关闭[不建议]
* `bin\shutdown.bat` ：正常关闭
* `ctrl+c` ： 正常关闭
### 2.2.6 配置
**修改端口**
* Tomcat默认的端口是8080，要想修改Tomcat启动的端口号，需要修改 `conf/server.xml` 。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302060106277.png)
	注：HTTP协议默认端口号为80，如果将Tomcat端口号改为80，则将来访问Tomcat时，将不用输入端口号。

**启动时可能出现的错误**
* Tomcat的端口号取值范围是「0-65535之间任意未被占用的端口」，如果设置的端口号被占用，启动时就会出现如下错误：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302060108747.png)
* Tomcat启动时，启动窗口一闪而过：需要检查JAVA_HOME环境变量是否正确配置。

### 2.2.7 部署
Tomcat部署项目：将项目拷贝到 `webapps` 目录下，即部署完成。再通过浏览器访问对应页面如 `http://localhost/hello/a.html`，能看到对应内容就说明项目已经部署成功。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302060110587.png)
但随着项目的增大，项目中的资源也会越来越多，项目在拷贝的过程中也会越来越费时间，该如何解决呢——一般来说，JavaWeb项目会被打包成WAR包，然后将WAR包放到 `Webapps` 目录下，Tomcat会自动解压缩WAR文件。

至此，Tomcat的部署就已经完成了，至于如何获得项目对应的WAR包，后期我们会借助于IDEA工具来生成。

### 2.3 Maven创建Web项目
介绍完Tomcat的基本使用后，学习在IDEA中创建Maven Web项目，学习这种方式的原因是：==以后Tomcat中运行的绝大多数都是Web项目，而使用Maven工具能更加简单快捷地把Web项目给创建出来==。

在真正创建Maven Web项目之前，我们先要知道Web项目长什么样子，具体的结构是什么。
### 2.3.1 Web项目结构
**Web项目的结构分为：开发中的项目和开发完可以部署的Web项目**，这两种项目的结构是不一样的，一一介绍:
* Maven Web项目结构：开发中的项目 
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302060122354.png)
* 开发完成部署的Web项目
	 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302060124234.png)
其中的变化是：
  * 开发项目通过执行Maven打包命令 `package` ，可以获取到部署的Web项目目录
  * 编译后的Java字节码文件和 `resources` 中的资源文件，会被放到 `WEB-INF` 下的 `classes` 目录下
  * `pom.xml` 中依赖坐标对应的JAR包，会被放入 `WEB-INF` 下的 `lib` 目录下

### 2.3.2 创建Maven Web项目
介绍完Maven Web的项目结构后，接下来使用Maven来创建Web项目，创建方式有两种：使用骨架 *archetype* 和不使用骨架。

**使用骨架**具体的步骤包含：
1.创建Maven项目：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302060130971.png)
2.选择使用Web项目骨架：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302060132739.png)
3.输入Maven项目坐标创建项目：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061204862.png)
4.确认Maven相关的配置信息后，完成项目创建：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061205852.png)
5.删除`pom.xml` 中多余内容，只留下面的这些内容，注意打包方式JAR和WAR的区别：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061210527.png)

6.补齐Maven Web项目缺失的目录结构，默认没有 `java` 和 `resources` 目录，需要手动完成创建补齐，最终的目录结果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061210232.png)

 
**不使用骨架**具体的步骤包含:
1.创建Maven项目：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061212084.png)
2.选择不使用Web项目骨架：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061213345.png)
3.输入Maven项目坐标创建项目：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061213791.png)
4.在pom.xml设置打包方式为 `war` ，默认是不写代表打包方式为 `jar` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061214887.png)
5.在Project Structure中，补齐Maven Web项目缺失 `webapp` 的目录结构：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061214216.png)
6.补齐Maven Web项目缺失的 `WEB-INF/web.xml` 的目录结构
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061217149.png)
7.补充完后，最终的项目结构如下:
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061223990.png)

上述两种方式，创建的web项目，都不是很全，需要手动补充内容，至于最终采用哪种方式来创建Maven Web项目，都是可以的，根据各自的喜好来选择使用即可。

**小结**
1.掌握Maven Web项目的目录结构
2.掌握使用骨架的方式创建Maven Web项目
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061229139.png)
3.掌握不使用骨架的方式创建Maven Web项目
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061232642.png)

## 2.4 IDEA使用Tomcat
Maven Web项目创建成功后，通过Maven的 `package` 命令可以将项目打包成WAR包，将WAR文件拷贝到Tomcat的 `webapps` 目录下，启动Tomcat就可以将项目部署成功，然后通过浏览器进行访问即可。

然而我们在开发的过程中，项目中的内容会经常发生变化，如果按照上面这种方式来部署测试，是非常不方便的。如何在IDEA中能快速使用Tomcat呢？在IDEA中集成使用Tomcat有两种方式，分别是==集成本地Tomcat==和==Tomcat Maven插件==。
### 2.4.1 集成本地Tomcat
目标：将刚才本地安装好的Tomcat8集成到IDEA中，完成项目部署，具体的实现步骤
1.打开添加本地Tomcat的面板：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061257678.png)
2.指定本地Tomcat的具体路径
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061259408.png)
3.修改Tomcat的名称，此步骤可以不改，只是让名字看起来更有意义，HTTP port中的端口也可以进行修改，比如把8080改成80（社区版要安装插件；经典白学，SpringBoot中也集成了）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061300435.png)
4.将开发项目部署项目到Tomcat中：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061302613.png)

扩展内容：`xxx.war` 和 `xxx.war exploded` 这两种部署项目模式的区别?
* `war` 模式是将WEB工程打成 `war` 包，把 `war` 包发布到Tomcat服务器上
* `war exploded` 模式是**将WEB工程以「当前文件夹的位置关系」发布到Tomcat服务器上**
* `war` 模式部署成功后，Tomcat的 `webapps` 目录下会有部署的项目内容
* `war exploded` 模式部署成功后，Tomcat的 `webapps` 目录下没有，而使用的是项目的 `target` 目录下的内容进行部署
* **建议大家都选war模式进行部署，更符合项目部署的实际情况**

5.部署成功后，就可以启动项目，为了能更好的看到启动的效果，可以在webapp目录下添加 `a.html` 页面：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061310426.png)

6.启动成功后，可以通过浏览器进行访问测试
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061312721.png)
7.最终的注意事项
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061313379.png)

至此，IDEA中集成本地Tomcat进行项目部署的内容我们就介绍完了，整体步骤如下，需要按照流程进行部署操作练习。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061315761.png)

### 2.4.2 Tomcat Maven插件
==在IDEA中使用本地Tomcat进行项目部署，相对来说步骤比较繁琐，所以我们需要一种更简便的方式来替换它==，那就是直接使用Maven中的Tomcat插件来部署项目，具体的实现步骤，只需要两步，分别是：
1. 在 `pom.xml` 中添加Tomcat插件
   ```xml
   <build>
       <plugins>
       	<!--Tomcat插件 -->
           <plugin>
               <groupId>org.apache.tomcat.maven</groupId>
               <artifactId>tomcat7-maven-plugin</artifactId>
               <version>2.2</version>
           </plugin>
       </plugins>
   </build>
   ```
2. 使用Maven Helper插件快速启动项目，选中项目，右键-->Run Maven --> tomcat7:run 
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061319704.png)
	==注意==：如果选中项目并右键点击后，看不到 `Run Maven` 和 `Debug Maven` ，这个时候就需要在IDEA中下载Maven Helper插件，具体的操作方式为：File --> Settings --> Plugins --> Maven Helper --> Install，安装完后按照提示重启IDEA，就可以看到了。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061334682.png)


Maven Tomcat插件目前只有Tomcat7版本，没有更高的版本可以使用。使用Maven Tomcat插件，要想修改Tomcat的端口和访问路径，可以直接修改 `pom.xml` ：
```xml
<build>
    <plugins>
    	<!--Tomcat插件 -->
        <plugin>
            <groupId>org.apache.tomcat.maven</groupId>
            <artifactId>tomcat7-maven-plugin</artifactId>
            <version>2.2</version>
            <configuration>
            	<port>80</port><!--访问端口号 -->
                <!--项目访问路径
					未配置访问路径: http://localhost:80/tomcat-demo2/a.html
					配置/后访问路径: http://localhost:80/a.html
					如果配置成 /hello,访问路径会变成什么?
						答案: http://localhost:80/hello/a.html
				-->
                <path>/</path>
            </configuration>
        </plugin>
    </plugins>
</build>
```

**小结**：通过这一节，要掌握在IDEA中使用Tomcat的两种方式，集成本地Tomcat和使用Maven的Tomcat插件。后者更简单，推荐大家使用，==但如果对于Tomcat的版本有比较高的要求，要在Tomcat7以上，这个时候就只能用前者了==。

---
# 3. Servlet
## 3.1 简介
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061353470.png)
Servlet是Java Web最为核心的内容，它是Java提供的一门==动态web资源开发技术==。使用Servlet就可以实现，根据不同的登录用户在页面上动态显示不同内容。==Servlet是JavaEE规范之一，其实就是一个接口==，将来**我们需要定义 `Servlet` 类实现 `Servlet` 接口**，并由Web服务器运行Servlet。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302061402093.png)

介绍完Servlet是什么以后，接下来按照快速入门->执行流程->生命周期->体系结构-> `urlPattern` 配置->XML配置的学习步骤，一步步完成对Servlet的知识学习，首先通过一个入门案例来快速把Servlet用起来。
## 3.2 快速入门
==需求分析：编写一个Servlet类，并使用IDEA中Tomcat插件进行部署，最终通过浏览器访问所编写的Servlet程序==。具体的实现步骤为：
1.创建Web项目 `web-demo` ，**导入Servlet依赖坐标**
```xml
<dependency>
    <groupId>javax.servlet</groupId>
    <artifactId>javax.servlet-api</artifactId>
    <version>3.1.0</version>
    <!--
      此处为什么需要添加该标签?
      provided指的是在编译和测试过程中有效,最后生成的war包时不会加入
	  因为Tomcat的lib目录中已经有servlet-api这个jar包，
	  如果在生成war包时生效、就会和Tomcat中的jar包冲突，导致报错
    -->
    <scope>provided</scope>
</dependency>
```
2.创建：定义一个类，实现 `Servlet` 接口，并**重写接口中所有方法，在 `service` 方法中输入一句话**：
```java
package com.itheima.web;

import javax.servlet.*;
import java.io.IOException;

public class ServletDemo1 implements Servlet {

    public void service(ServletRequest servletRequest, ServletResponse servletResponse) throws ServletException, IOException {
        System.out.println("servlet hello world~");
    }
    
    public void init(ServletConfig servletConfig) throws ServletException {
    }
    public ServletConfig getServletConfig() {
        return null;
    }
    public String getServletInfo() {
        return null;
    }
    public void destroy() {
    }
}
```
3.配置：在类上使用 `@WebServlet` 注解，**配置该Servlet的访问路径**：
```java
@WebServlet("/demo1")
```
4.访问：启动Tomcat，浏览器中输入URL地址，访问该Servlet：
```java
http://localhost:8080/web-demo/demo1
```
5.访问后，在控制台会打印 `servlet hello world~` ，说明servlet程序已经成功运行。

至此，Servlet的入门案例就已经完成，可以按照上面的步骤进行练习了。
## 3.3 执行流程
Servlet程序已经能正常运行，但我们需要思考个问题：**我们并没有创建 `ServletDemo1` 类的对象，也没有调用对象中的 `service` 方法，为什么在控制台就打印了`servlet hello world~` 这句话呢**？要想回答上述问题，就需要对Servlet的执行流程进行学习。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302080127598.png)
浏览器发出 `http://localhost:8080/web-demo/demo1`请求，从请求中可以解析出三部分内容，分别是 `localhost:8080, web-demo, demo1` ：
* 根据 `localhost:8080` 可以**找到要访问的Tomcat Web服务器**
* 根据 `web-demo` 可以找到**部署在Tomcat服务器上的 `web-demo` 项目**
* 根据 `demo1` 可以找到要访问的是「项目中的哪个Servlet类」，根据 `@WebServlet` 后面的值进行匹配
* 找到 `ServletDemo1` 这个类后，**Tomcat Web服务器就会为 `ServletDemo1` 这个类创建一个对象，然后调用对象中的 `service` 方法**
* `ServletDemo1` 实现了 `Servlet` 接口，所以类中必然会重写 `service` 方法供Tomcat Web服务器进行调用
* service方法中有 `ServletRequest` 和 `ServletResponse` 两个参数，`ServletRequest` 封装的是请求数据，`ServletResponse` 封装的是响应数据，**后期可以通过这两个参数实现前后端的数据交互**

**小结**：介绍完Servlet的执行流程，需要大家掌握两个问题：
1. Servlet由谁创建？Servlet方法由谁调用？——==Servlet由Web服务器创建，Servlet方法由Web服务器调用==。
2. 服务器怎么知道Servlet中一定有 `service` 方法？——因为我们自定义的Servlet必须实现 `Servlet` 接口并覆写其方法，而 `Servlet` 接口中有 `service` 方法。

## 3.4 生命周期
介绍完Servlet的执行流程后，我们知道**Servlet是由Tomcat Web服务器帮我们创建的**。接下来再思考一个问题：==Tomcat什么时候创建的Servlet对象==？要想回答上述问题，就需要学习「Servlet的生命周期」。对象的生命周期指一个对象从被创建到被销毁的整个过程。

**Servlet运行在Servlet容器（Web服务器）中，其生命周期由容器来管理**，分为4个阶段：
1. ==加载和实例化==：默认情况下，**当Servlet第一次被访问时**，由容器创建Servlet对象——默认情况，Servlet会在第一次访问被容器创建，但如果创建Servlet比较耗时的话，那么第一个访问的人等待的时间就比较长，用户的体验就比较差，那么我们能不能把Servlet的创建放到服务器启动时来创建，具体如何来配置？
   ```java
   @WebServlet(urlPatterns = "/demo1", loadOnStartup = 1)
   ```
  `loadOnstartup` 的取值有两类情况：
	  - 负整数：第一次访问时创建Servlet对象
	  - 0或正整数：**服务器启动时创建Servlet对象，数字越小优先级越高**
2. ==初始化==：在Servlet实例化之后，容器将**调用Servlet的 `init()` 方法初始化这个对象**，完成一些如加载配置文件、创建连接等初始化的工作。该方法==只调用一次==
3. ==请求处理==：==每次==请求Servlet时，Servlet容器都会调用Servlet的 `service()` 方法对请求进行处理
4. ==服务终止==：当需要释放内存或者容器关闭时，容器就会调用Servlet实例的 `destroy()` 方法完成资源的释放。在 `destroy()` 方法调用之后，容器会释放这个Servlet实例，**该实例随后会被Java的垃圾收集器所回收**

通过案例演示下上述的生命周期：
  ```java
  package com.itheima.web;
  
  import javax.servlet.*;
  import javax.servlet.annotation.WebServlet;
  import java.io.IOException;
  /**
   * Servlet生命周期方法
   */
  @WebServlet(urlPatterns = "/demo2", loadOnStartup = 1)
  public class ServletDemo2 implements Servlet {
      /**
       *  初始化方法
       *  1.调用时机: 默认情况下，Servlet被第一次访问时，调用
       *    * loadOnStartup: 默认为-1，修改为0或者正整数，则会在服务器启动时调用
       *  2.调用次数: 1次
       * @param config
       * @throws ServletException
       */
      public void init(ServletConfig config) throws ServletException {
          System.out.println("init...");
      }
  
      /**
       * 提供服务
       * 1.调用时机: 每一次Servlet被访问时，调用
       * 2.调用次数: 多次
       * @param req
       * @param res
       * @throws ServletException
       * @throws IOException
       */
      public void service(ServletRequest req, ServletResponse res) throws ServletException, IOException {
          System.out.println("servlet hello world~");
      }
  
      /**
       * 销毁方法
       * 1.调用时机: 内存释放或者服务器关闭时，Servlet对象会被销毁，调用
       * 2.调用次数: 1次
       */
      public void destroy() {
          System.out.println("destroy...");
      }
      
      public ServletConfig getServletConfig() {
          return null;
      }
      public String getServletInfo() {
          return null;
      }
  }
  ```
注意：**如何才能让Servlet中的 `destroy` 方法被执行？**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302080137199.png)

在Terminal命令行中，先使用 `mvn tomcat7:run` 启动，然后再使用 `ctrl+c` 关闭Tomcat。

**小结**。这节中需要掌握的内容是:
1. Servlet对象在什么时候被创建的？——默认是第一次访问时被创建，可以使用 `@WebServlet(urlPatterns = "/demo2", loadOnStartup = 1)` 的 `loadOnStartup` 修改成在服务器启动时创建。
2. Servlet生命周期中涉及到的三个方法，这三个方法是什么？什么时候被调用？调用几次？涉及到三个方法，分别是 init()、service()、destroy()
	- `init` 方法在Servlet对象被创建时执行，只执行1次
	- `service` 方法在Servlet被访问时调用，每访问1次就调用1次
	- `destroy` 方法在Servlet对象被销毁时调用，只执行1次

## 3.5 方法介绍
Servlet中总共有5个方法，我们已经介绍过其中的三个，剩下的两个方法作用分别是什么？先来回顾下前面讲的三个方法，分别是：
* 初始化方法，在Servlet被创建时执行，只执行一次
   ```java
void init(ServletConfig config) 
   ```
* 提供服务方法， 每次Servlet被访问，都会调用该方法
   ```java
void service(ServletRequest req, ServletResponse res)
   ```
* 销毁方法，当Servlet被销毁时，调用该方法。在内存释放或服务器关闭时销毁Servlet
   ```java
void destroy() 
   ```

剩下的两个方法是：
* 获取Servlet信息
   ```java
String getServletInfo() 
//该方法用来返回Servlet的相关信息, 没有太大用处, 一般我们返回一个空字符串即可
public String getServletInfo() {
    return "";
}
   ```
* 获取 `ServletConfig` 对象
   ```java
ServletConfig getServletConfig()
   ```
	`ServletConfig` 对象，在 `init` 方法的参数中有，而Tomcat Web服务器在创建Servlet对象时会调用 `init` 方法，必定会传入一个 `ServletConfig` 对象，我们只需要将服务器传过来的 `ServletConfig` 进行返回即可。具体如何操作?

```java
package com.itheima.web;

import javax.servlet.*;
import javax.servlet.annotation.WebServlet;
import java.io.IOException;
/**
 * Servlet方法介绍
 */
@WebServlet(urlPatterns = "/demo3",loadOnStartup = 1)
public class ServletDemo3 implements Servlet {
    private ServletConfig servletConfig;
    /**
     *  初始化方法
     *  1.调用时机: 默认情况下,Servlet被第一次访问时，调用
     *      * loadOnStartup: 默认为-1, 修改为0或者正整数, 则会在服务器启动时调用
     *  2.调用次数: 1次
     * @param config
     * @throws ServletException
     */
    public void init(ServletConfig config) throws ServletException {
        this.servletConfig = config;
        System.out.println("init...");
    }
    public ServletConfig getServletConfig() {
        return servletConfig;
    }
    /**
     * 提供服务
     * 1.调用时机:每一次Servlet被访问时，调用
     * 2.调用次数: 多次
     * @param req
     * @param res
     * @throws ServletException
     * @throws IOException
     */
    public void service(ServletRequest req, ServletResponse res) throws ServletException, IOException {
        System.out.println("servlet hello world~");
    }
    /**
     * 销毁方法
     * 1.调用时机：内存释放或者服务器关闭时，Servlet对象会被销毁，调用
     * 2.调用次数: 1次
     */
    public void destroy() {
        System.out.println("destroy...");
    }
    public String getServletInfo() {
        return "";
    }
}
```
`getServletInfo()` 和 `getServletConfig()` 这两个方法使用的不是很多，大家了解下。
## 3.6 体系结构
通过上面的学习，我们知道要想编写一个Servlet就必须要实现Servlet接口，重写接口中的5个方法，虽然已经能完成要求，但编写起来还是比较麻烦的，因为更关注的其实只有 `service` 方法，那有没有更简单方式来创建Servlet呢？要想解决上面的问题，我们需要先对Servlet的体系结构进行下了解:
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302091521136.png)

因为我们将来开发B/S架构的web项目，都是针对HTTP协议，所以**我们会通过继承 `HttpServlet` 自定义Servlet** 。具体的编写格式如下：
```java
@WebServlet("/demo4")
public class ServletDemo4 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //TODO GET 请求方式处理逻辑
        System.out.println("get...");
    }
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //TODO Post 请求方式处理逻辑
        System.out.println("post...");
    }
}
```
要想发送一个GET请求，请求该Servlet，只需要通过浏览器发送 `http://localhost:8080/web-demo/demo4` ，就能看到 `doGet` 方法被执行了。==要想发送一个POST请求，请求该Servlet，单单通过浏览器是无法实现的==，这个时候就需要编写一个 `form` 表单来发送请求，在Maven项目的 `webapp` 文件夹下创建一个 `a.html` 页面，内容如下：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
    <form action="/web-demo/demo4" method="post">
        <input name="username"/><input type="submit"/>
    </form>
</body>
</html>
```
启动测试，即可看到 `doPost` 方法被执行了。

Servlet的简化编写就介绍完了，接着需要思考两个问题：
1. `HttpServlet` 中为什么要根据请求方式的不同，调用不同的方法？
2. 如何调用？

针对问题一，我们需要回顾之前的知识点——==前端发送GET和POST请求时，参数的位置不一致，GET请求参数在请求行中，POST请求参数在请求体中==，为了能处理不同的请求方式，我们**得在 `service` 方法中进行判断**，然后写不同的业务处理，这样能实现，但是每个Servlet类中都将有相似的代码，针对这个问题，有什么可以优化的策略么？
```java
package com.itheima.web;

import javax.servlet.*;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

@WebServlet("/demo5")
public class ServletDemo5 implements Servlet {
    public void init(ServletConfig config) throws ServletException {
    }
    public ServletConfig getServletConfig() {
        return null;
    }
    public void service(ServletRequest req, ServletResponse res) throws ServletException, IOException {
        //如何调用?
        //获取请求方式,根据不同的请求方式进行不同的业务处理
        HttpServletRequest request = (HttpServletRequest)req;
       //1. 获取请求方式
        String method = request.getMethod();
        //2. 判断
        if("GET".equals(method)){
            // get方式的处理逻辑
        }else if("POST".equals(method)){
            // post方式的处理逻辑
        }
    } 
    public String getServletInfo() {
        return null;
    } 
    public void destroy() {
    }
}
```
要解决上述问题，我们可以对Servlet接口进行继承封装，来简化代码开发。
```java
package com.itheima.web;

import javax.servlet.*;
import javax.servlet.http.HttpServletRequest;
import java.io.IOException;

public class MyHttpServlet implements Servlet {
    public void init(ServletConfig config) throws ServletException {

    }

    public ServletConfig getServletConfig() {
        return null;
    }

    public void service(ServletRequest req, ServletResponse res) throws ServletException, IOException {
        HttpServletRequest request = (HttpServletRequest)req;
        //1. 获取请求方式
        String method = request.getMethod();
        //2. 判断
        if("GET".equals(method)){
            // get方式的处理逻辑
            doGet(req,res);
        }else if("POST".equals(method)){
            // post方式的处理逻辑
            doPost(req,res);
        }
    }

    protected void doPost(ServletRequest req, ServletResponse res) {
    }

    protected void doGet(ServletRequest req, ServletResponse res) {
    }

    public String getServletInfo() {
        return null;
    }

    public void destroy() {
    }
}
```
有了 `MyHttpServlet` 这个类，以后我们再编写Servlet类时，只需要继承 `MyHttpServlet` ，重写父类中的 `doGet` 和 `doPost` 方法，就可以用来处理GET和POST请求的业务逻辑。接下来，可以把 `ServletDemo5` 代码进行改造：
```java
@WebServlet("/demo5")
public class ServletDemo5 extends MyHttpServlet {
    @Override
    protected void doGet(ServletRequest req, ServletResponse res) {
        System.out.println("get...");
    }
    @Override
    protected void doPost(ServletRequest req, ServletResponse res) {
        System.out.println("post...");
    }
}
```
将来页面发送的是GET请求，则会进入到 `doGet` 方法中进行执行，如果是POST请求，则进入到 `doPost` 方法。这样**代码在编写时就相对来说更加简单快捷**。

类似 `MyHttpServlet` 这样的类Servlet中已经为我们提供好了，就是 `HttpServlet` 。翻开源码，大家可以搜索 `service()` 方法，你会发现 `HttpServlet` 做的事更多，不仅可以处理 `GET` 和 `POST` 还可以处理其他五种请求方式。
```java
protected void service(HttpServletRequest req, HttpServletResponse resp)
        throws ServletException, IOException
    {
        String method = req.getMethod(); 
        if (method.equals(METHOD_GET)) {
            long lastModified = getLastModified(req);
            if (lastModified == -1) {
                // servlet doesn't support if-modified-since, no reason
                // to go through further expensive logic
                doGet(req, resp);
            } else {
                long ifModifiedSince = req.getDateHeader(HEADER_IFMODSINCE);
                if (ifModifiedSince < lastModified) {
                    // If the servlet mod time is later, call doGet()
                    // Round down to the nearest second for a proper compare
                    // A ifModifiedSince of -1 will always be less
                    maybeSetLastModified(resp, lastModified);
                    doGet(req, resp);
                } else {
                    resp.setStatus(HttpServletResponse.SC_NOT_MODIFIED);
                }
            } 
        } else if (method.equals(METHOD_HEAD)) {
            long lastModified = getLastModified(req);
            maybeSetLastModified(resp, lastModified);
            doHead(req, resp);
        } else if (method.equals(METHOD_POST)) {
            doPost(req, resp); 
        } else if (method.equals(METHOD_PUT)) {
            doPut(req, resp); 
        } else if (method.equals(METHOD_DELETE)) {
            doDelete(req, resp); 
        } else if (method.equals(METHOD_OPTIONS)) {
            doOptions(req,resp); 
        } else if (method.equals(METHOD_TRACE)) {
            doTrace(req,resp); 
        } else {
            //
            // Note that this means NO servlet supports whatever
            // method was requested, anywhere on this server.
            // 
            String errMsg = lStrings.getString("http.method_not_implemented");
            Object[] errArgs = new Object[1];
            errArgs[0] = method;
            errMsg = MessageFormat.format(errMsg, errArgs); 
            resp.sendError(HttpServletResponse.SC_NOT_IMPLEMENTED, errMsg);
        }
    }
```

**小结**：通过这一节的学习，要掌握
1. `HttpServlet` 的使用步骤——继承 `HttpServlet` ；重写 `doGet` 和 `doPost` 方法。
2. `HttpServlet` 原理：获取请求方式，并根据不同的请求方式，调用不同的 `doXxx` 方法

## 3.7 `urlPattern` 配置
Servlet类编写好后，要想被访问到，就需要配置其访问路径 `urlPattern` 。**一个Servlet可以配置多个 `urlPattern`** ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302091537552.png)

```java
package com.itheima.web;

import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.annotation.WebServlet;

/**
* urlPattern: 一个Servlet可以配置多个访问路径
*/
@WebServlet(urlPatterns = {"/demo7", "/demo8"})
public class ServletDemo7 extends MyHttpServlet {
  @Override
  protected void doGet(ServletRequest req, ServletResponse res) {
	  System.out.println("demo7 get...");
  }
  @Override
  protected void doPost(ServletRequest req, ServletResponse res) {
  }
}
```
在浏览器上输入 `http://localhost:8080/web-demo/demo7, http://localhost:8080/web-demo/demo8` 这两个地址都能访问到 `ServletDemo7` 的 `doGet` 方法。

==urlPattern配置规则==：
* 精确匹配
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302091542799.png)
    ```java
    /**
     * UrlPattern:
     * * 精确匹配
     */
    @WebServlet(urlPatterns = "/user/select")
    public class ServletDemo8 extends MyHttpServlet {
        @Override
        protected void doGet(ServletRequest req, ServletResponse res) {
            System.out.println("demo8 get...");
        }
        @Override
        protected void doPost(ServletRequest req, ServletResponse res) {
        }
    }
    ```
    访问路径 `http://localhost:8080/web-demo/user/select` 。
* **目录匹配**
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302091542442.png)
    ```java
    package com.itheima.web;
    
    import javax.servlet.ServletRequest;
    import javax.servlet.ServletResponse;
    import javax.servlet.annotation.WebServlet;
    
    /**
     * UrlPattern:
     * * 目录匹配: /user/*
     */
    @WebServlet(urlPatterns = "/user/*")
    public class ServletDemo9 extends MyHttpServlet {
    
        @Override
        protected void doGet(ServletRequest req, ServletResponse res) {
    
            System.out.println("demo9 get...");
        }
        @Override
        protected void doPost(ServletRequest req, ServletResponse res) {
        }
    }
    ```
    访问路径 `http://localhost:8080/web-demo/user/任意` 。
    ==思考==：
    1. 访问路径 `http://localhost:8080/web-demo/user` 是否能访问到 `demo9` 的 `doGet` 方法？
    2. 访问路径 `http://localhost:8080/web-demo/user/a/b` 是否能访问到 `demo9` 的 `doGet` 方法？
    3. 访问路径 `http://localhost:8080/web-demo/user/select` 是否能访问到 `demo9` 还是 `demo8` 的 `doGet` 方法？
    答案是: 能、能、demo8，进而我们可以得到的结论是：**`/user/*` 中的 `/*` 代表的是零或多个层级访问目录**、同时**精确匹配优先级要高于目录匹配**。
* 扩展名匹配
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302091545247.png)

    ```java
    package com.itheima.web;
    
    import javax.servlet.ServletRequest;
    import javax.servlet.ServletResponse;
    import javax.servlet.annotation.WebServlet;
    /**
     * UrlPattern:
     * * 扩展名匹配: *.do
     */
    @WebServlet(urlPatterns = "*.do")
    public class ServletDemo10 extends MyHttpServlet {
        @Override
        protected void doGet(ServletRequest req, ServletResponse res) {
            System.out.println("demo10 get...");
        }
        @Override
        protected void doPost(ServletRequest req, ServletResponse res) {
        }
    }
    ```
    访问路径`http://localhost:8080/web-demo/任意.do` 。==注意==：
    1. **如果路径配置的不是扩展名，那么在路径的前面就必须要加 `/`** ，否则会报错：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302091549746.png)
    2. 如果路径配置的是 `*.do` ，那么在 `*.do` 的前面不能加 `/` ，否则会报错
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302091555150.png)
* 任意匹配
    ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302091556832.png)
    ```java
    package com.itheima.web;
    
    import javax.servlet.ServletRequest;
    import javax.servlet.ServletResponse;
    import javax.servlet.annotation.WebServlet;
    
    /**
     * UrlPattern:
     * * 任意匹配： /
     */
    @WebServlet(urlPatterns = "/")
    public class ServletDemo11 extends MyHttpServlet {
        @Override
        protected void doGet(ServletRequest req, ServletResponse res) {
            System.out.println("demo11 get...");
        }
        @Override
        protected void doPost(ServletRequest req, ServletResponse res) {
        }
    }
    ```
    访问路径 `http://localhost:8080/demo-web/任意` 。
    ```java
    package com.itheima.web;
    
    import javax.servlet.ServletRequest;
    import javax.servlet.ServletResponse;
    import javax.servlet.annotation.WebServlet;
    /**
     * UrlPattern:
     * * 任意匹配： /*
     */
    @WebServlet(urlPatterns = "/*")
    public class ServletDemo12 extends MyHttpServlet {
        @Override
        protected void doGet(ServletRequest req, ServletResponse res) {
            System.out.println("demo12 get...");
        }
        @Override
        protected void doPost(ServletRequest req, ServletResponse res) {
        }
    }
    
    ```
    访问路径 `http://localhost:8080/demo-web/任意` 。
    ==注意==：`/` 和 `/*` 的区别？
    1. 当我们的项目中的Servlet配置了 `"/"` ，**会覆盖掉Tomcat中的 `DefaultServlet`** ，当其他的 `url-pattern` 都匹配不上时都会走这个Servlet
    2. `DefaultServlet` 是用来处理静态资源的访问的（一个非常重要的功能），如果配置了 `"/"` 会把默认的覆盖掉，就会引发「请求静态资源时没有走默认处理、而是走自定义的Servlet类」，最终导致静态资源不能被访问。
    3. **当我们的项目中配置了 `"/*"` ，意味着匹配任意访问路径**，它的优先级高于 `/` 。
    4. 这两种路径不建议配置。

**小结**
1. `urlPattern` 总共有四种配置方式，分别是精确匹配、目录匹配、扩展名匹配、任意匹配
2. 五种配置的优先级为 精确匹配 > 目录匹配> 扩展名匹配 > `/*` > `/` ，无需记，以最终运行结果为准。
## 3.8 XML配置
前面对应Servlet的配置，我们都使用的是 `@WebServlet` ，这个是Servlet从3.0版本后开始支持注解配置，3.0版本前只支持XML配置文件的配置方法。对于XML的配置步骤有两步：
1.编写Servlet类
```java
package com.itheima.web;

import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.annotation.WebServlet;

public class ServletDemo13 extends MyHttpServlet {
    @Override
    protected void doGet(ServletRequest req, ServletResponse res) {
        System.out.println("demo13 get...");
    }
    @Override
    protected void doPost(ServletRequest req, ServletResponse res) {
    }
}
```
2.在 `webapp/WEB-INF/web.xml` 中配置该Servlet
```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-app_4_0.xsd"
         version="4.0">

    <!-- Servlet 全类名 -->
    <servlet>
        <!-- servlet的名称，名字任意-->
        <servlet-name>demo13</servlet-name>
        <!--servlet的类全名-->
        <servlet-class>com.itheima.web.ServletDemo13</servlet-class>
    </servlet>

    <!-- Servlet 访问路径 -->
    <servlet-mapping>
        <!-- servlet的名称，要和上面的名称一致-->
        <servlet-name>demo13</servlet-name>
        <!-- servlet的访问路径-->
        <url-pattern>/demo13</url-pattern>
    </servlet-mapping>
</web-app>
```
这种配置方式和注解比起来，确实麻烦很多，所以建议大家使用注解来开发。但是大家要认识上面这种配置方式，因为并不是所有的项目都是基于注解开发的。

---
# 4. `Request` 和 `Response` 对象
`Request` 是请求对象，`Response` 是响应对象。这两个对象在我们使用Servlet时有看到：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302091944467.png)

此时，我们就需要思考一个问题：`request` 和 `response` 这两个参数的作用是什么？
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302091945102.png)
`request` ：**获取请求数据** 
- 浏览器会发送HTTP请求到后台服务器[Tomcat] 
- HTTP的请求中会包含很多请求数据[请求行+请求头+请求体] 
- 后台服务器[Tomcat]会对HTTP请求中的数据进行解析，并把解析结果存入到一个对象中
- 所存入的对象即为 `request` 对象，所以我们可以从 `request` 对象中获取请求的相关参数
- 获取到数据后就可以继续后续的业务，比如获取用户名和密码就可以实现登录操作的相关业务 

`response` ：**设置响应数据**
- 业务处理完后，后台就需要给前端返回业务处理的结果，即响应数据
- 把响应数据封装到 `response` 对象中 
- 后台服务器[Tomcat]会解析 `response` 对象，按照[响应行+响应头+响应体]格式拼接结果 
- 浏览器最终解析结果，把内容展示在浏览器给用户浏览

对于上述所讲的内容，我们通过一个案例来初步体验下 `request` 和 `response` 对象的使用。
```java
@WebServlet("/demo3") 
public class ServletDemo3 extends HttpServlet { 
	@Override 
	protected void doGet(HttpServletRequest request, HttpServletResponse response) 
		throws ServletException, IOException {
		//使用request对象 获取请求数据
		String name = request.getParameter("name"); //url?name=zhangsan 
		//使用response对象 设置响应数据 
		response.setHeader("content-type", "text/html;charset=utf-8"); 
		response.getWriter().write("<h1>"+name+",欢迎您！</h1>"); 
	} 
	@Override 
	protected void doPost(HttpServletRequest request, HttpServletResponse response) 
		throws ServletException, IOException { 
		System.out.println("Post..."); 
	} 
}
```
启动成功后就可以通过浏览器来访问，并且根据传入参数的不同就可以在页面上展示不同的内容:
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302091951835.png)

**小结**：在这节中，我们主要认识了 `request` 对象和 `reponse` 对象：
* `request` 对象是用来封装请求数据的对象
* `response` 对象是用来封装响应数据的对象

目前只知道这两个对象是用来干什么的，那么它们具体是如何实现的，就需要继续深入的学习。接下来，就先从 `Request` 对象来学习，主要学习下面这些内容：
* `request` 继承体系
* `request` 获取请求参数
* `request` 请求转发

## 4.1 `Request` 对象
### 4.1.1 `Request` 继承体系
在学习这节内容前，先思考一个问题，前面在介绍 `Request` 和 `Reponse` 对象时，可能已经发现：
* 当我们的Servlet类实现的是 `Servlet` 接口时，`service` 方法中的参数是 `ServletRequest` 和 `ServletResponse`
* 当我们的Servlet类继承的是 `HttpServlet` 类时，`doGet` 和 `doPost` 方法中的参数就变成 `HttpServletRequest` 和 `HttpServletReponse`

那么，
* `ServletRequest` 和 `HttpServletRequest` 的关系是什么？
* `request` 对象由谁来创建的？
* `request` 提供了哪些API，这些API从哪里查？

首先，我们先来看下 `Request` 的继承体系:
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302091955282.png)

从上图中可以看出，`ServletRequest` 和 `HttpServletRequest` 接口都是Java提供的（没有提供实现类），所以我们可以打开Java EE提供的API文档，打开后可以看到:
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302091956660.png)
所以 `ServletRequest` 和 `HttpServletRequest` 是继承关系，并且两个都是接口，接口是无法创建对象，这个时候就引发了下面这个问题:
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092000758.png)
这个时候，我们就需要用到 `Request` 继承体系中的 `RequestFacade` 类:
* 该类实现了 `HttpServletRequest` 接口，也间接实现了 `ServletRequest` 接口。
* `Servlet` 类中的 `service` 方法、`doGet` 方法或是 `doPost` 方法最终都是由Web服务器[Tomcat]来调用的，所以**Tomcat提供了方法参数接口的具体实现类，并完成了对象的创建**
* 要想了解 `RequestFacade` 中都提供了哪些方法，我们可以直接查看Java EE的API文档中关于 `ServletRequest` 和 `HttpServletRequest` 的接口文档，因为 `RequestFacade` 实现了其接口、就需要重写接口中的方法

对于上述结论，要想验证，可以编写一个Servlet，在方法中把 `request` 对象打印下，就能看到最终的对象是不是 `RequestFacade` ，代码如下:
```java
@WebServlet("/demo2")
public class ServletDemo2 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println(request);
    } 
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
    }
}
```
启动服务器，运行访问 `http://localhost:8080/request-demo/demo2` ，得到运行结果:
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092002883.png)

**小结**：
* `Request` 的继承体系为 `ServletRequest-->HttpServletRequest-->RequestFacade`
* **Tomcat需要解析请求数据，封装为 `request` 对象**，并创建 `request` 对象传递到 `service` 方法
* 使用 `request` 对象，可以查阅JavaEE API文档的 `HttpServletRequest` 接口中的方法说明

### 4.1.2 `Request` 获取请求数据
HTTP请求数据总共分为三部分内容，分别是==请求行、请求头、请求体==，对于这三部分内容的数据，分别该如何获取，首先来学习请求行数据如何获取。
#### 1. 获取请求行数据
请求行包含三块内容，分别是 `请求方式` 、`请求资源路径` 、`HTTP协议及版本`
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092004041.png)
对于这三部分内容，`request` 对象都提供了对应的API方法来获取，具体如下:
* 获取请求方式 `GET`
   ```java
   String getMethod()
   ```
* 获取**虚拟目录**（项目访问路径，通过动态方式获取，改了原配置以后就不会有影响）`/request-demo`
   ```java
   String getContextPath()
   ```
* 获取URL（统一资源定位符，作权限管理用）`http://localhost:8080/request-demo/req1`
   ```java
   StringBuffer getRequestURL()
   ```
 * 获取URI（统一资源标识符，作权限管理用，判断用户有无权限访问该资源）`/request-demo/req1`
    ```java
   String getRequestURI()
   ```
 * 获取请求参数（GET方式）`username=zhangsan&password=123`
   ```java
   String getQueryString()
   ```
 
介绍完上述方法后，咱们通过代码把上述方法都使用下：
```java
/**
 * request 获取请求数据
 */
@WebServlet("/req1")
public class RequestDemo1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        // String getMethod()：获取请求方式： GET
        String method = req.getMethod();
        System.out.println(method); //GET
        
        // String getContextPath() 获取虚拟目录(项目访问路径) /request-demo
        String contextPath = req.getContextPath();
        System.out.println(contextPath);
        
        // StringBuffer getRequestURL() 获取URL(统一资源定位符) http://localhost:8080/request-demo/req1
        StringBuffer url = req.getRequestURL();
        System.out.println(url.toString());
    
        // String getRequestURI() 获取URI(统一资源标识符)： /request-demo/req1
        String uri = req.getRequestURI();
        System.out.println(uri);
        
        // String getQueryString() 获取请求参数(GET方式) username=zhangsan
        String queryString = req.getQueryString();
        System.out.println(queryString);
    }
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
    }
}
``` 
启动服务器，访问 `http://localhost:8080/request-demo/req1?username=zhangsan&passwrod=123` ，获取的结果如下:
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092011942.png)

#### 2. 获取请求头数据
对于请求头的数据，格式为 `key: value` ，所以根据请求头名称获取对应值的方法为：
```java
String getHeader(String name)
```
接下来，在代码中如果想要获取客户端浏览器的版本信息，则可以使用：
```java
/**
 * request 获取请求数据
 */
@WebServlet("/req1")
public class RequestDemo1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //获取请求头: user-agent: 浏览器的版本信息
        String agent = req.getHeader("user-agent");
		System.out.println(agent);
    }
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
    }
}
```
重新启动服务器后，`http://localhost:8080/request-demo/req1?username=zhangsan&passwrod=123` ，获取的结果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092024179.png)

#### 3. 获取请求体数据
浏览器在发送GET请求时是没有请求体的，所以需要把请求方式变更为POST，请求体中的数据格式如下：
```java
username=superbaby&password=123
```
对于请求体中的数据，`Request` 对象提供了如下两种方式来获取其中的数据，分别是：
* 获取字节输入流，如果前端发送的是字节数据，比如传递的是文件数据，则使用该方法
  ```java
  ServletInputStream getInputStream()
  ```
* 获取字符输入流，如果前端发送的是纯文本数据，则使用该方法：
  ```java
  BufferedReader getReader()
  ```

接下来，大家需要思考，要想获取到请求体的内容该如何实现？具体实现的步骤如下。

1.在项目的 `webapp` 目录下添加一个html页面，名称为 `req.html` ，在页面中添加 `form` 表单，用来发送 `post` 请求
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<!-- 
    action:form表单提交的请求地址
    method:请求方式，指定为post
-->
<form action="/request-demo/req1" method="post">
    <input type="text" name="username">
    <input type="password" name="password">
    <input type="submit">
</form>
</body>
</html>
```
2.在Servlet的 `doPost` 方法中获取请求体数据
```java
/**
 * request 获取请求数据
 */
@WebServlet("/req1")
public class RequestDemo1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
    }
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //在此处获取请求体中的数据
    }
}
```

3.在 `doPost` 方法中使用 `request` 的 `getReader()` 或 `getInputStream()` 来获取，因为目前前端传递的是纯文本数据，所以我们采用 `getReader()` 方法来获取：
```java
/**
 * request 获取请求数据
 */
@WebServlet("/req1")
public class RequestDemo1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
    }
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
         //获取post 请求体：请求参数
        //1. 获取字符输入流
        BufferedReader br = req.getReader();
        //2. 读取数据
        String line = br.readLine();
        System.out.println(line);
    }
}
```
==注意==：`BufferedReader` 流是通过 `request` 对象来获取的，一次请求完成后 `request` 对象就会被销毁，`request` 对象被销毁后，`BufferedReader` 流就会自动关闭，所以**此处就不需要手动关闭流了**。

4.访问测试。启动服务器，通过浏览器访问 `http://localhost:8080/request-demo/req.html` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092126029.png)
点击提交按钮后，就可以在控制台看到前端所发送的请求数据：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092125231.png)

**小结**：HTTP请求数据中包含了 `请求行` 、`请求头` 和 `请求体` ，针对这三部分内容，Request对象都提供了对应的API方法来获取对应的值：
* 请求行
  * `getMethod()` 获取请求方式
  * `getContextPath()` 获取项目访问路径
  * `getRequestURL()` 获取请求URL
  * `getRequestURI()` 获取请求URI
  * `getQueryString()` 获取GET请求方式的请求参数
* 请求头
  * `getHeader(String name)` 根据请求头名称获取其对应的值
* 请求体
  * 注意：==浏览器发送的POST请求才有请求体==
  * 如果是纯文本数据：`getReader()`
  * 如果是字节数据如文件数据：`getInputStream()`

#### 4. 获取请求参数的通用方式
在学习下面内容之前，先提出两个问题：
* 什么是请求参数？
* 请求参数和请求数据的关系是什么？

为了能更好的回答上述两个问题，我们拿用户登录的例子来说明
1. 想要登录网址，需要进入登录页面
2. 在登录页面输入用户名和密码
3. 将用户名和密码提交到后台
4. 后台校验用户名和密码是否正确
5. 如果正确，则正常登录，如果不正确，则提示用户名或密码错误

上述例子中，用户名和密码其实就是我们所说的请求参数。**请求数据则是包含请求行、请求头和请求体的所有数据**。

请求参数和请求数据的关系是什么？
1. **请求参数是请求数据中的部分内容**
2. 如果是GET请求，请求参数在请求行中
3. 如果是POST请求，请求参数一般在请求体中

对于请求参数的获取,常用的有以下两种：
* GET方式 `String getQueryString()`
* POST方式：`BufferedReader getReader()`

有了上述的知识储备，我们来实现一个案例需求:
1. 发送一个GET请求并携带用户名，后台接收后打印到控制台
2. 发送一个POST请求并携带用户名，后台接收后打印到控制台

此处大家需要注意的是**GET请求和POST请求接收参数的方式不一样**，具体实现的代码如下：
```java
@WebServlet("/req1")
public class RequestDemo1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String result = req.getQueryString();
        System.out.println(result);
    }
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        BufferedReader br = req.getReader();
        String result = br.readLine();
        System.out.println(result);
    }
}
```
对于上述的代码，会存在什么问题呢？
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092134464.png)
如何解决上述重复代码的问题呢?
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092134322.png)
当然，也可以在 `doGet` 中调用 `doPost` ，在 `doPost` 中完成参数的获取和打印。另外需要注意的是，**`doGet` 和 `doPost` 方法都必须存在，不能删除任意一个**。

不同类型的请求，获取其请求参数的方法不同——==GET请求和POST请求获取请求参数的方式不一样，在获取请求参数这块该如何实现呢==？要想实现，我们就需要==思考==：**是否可以提供一种统一获取请求参数的方式**，从而统一 `doGet` 和 `doPost` 方法内的代码？

解决方案一：
```java
@WebServlet("/req1")
public class RequestDemo1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //获取请求方式
        String method = req.getMethod();
        //获取请求参数
        String params = "";
        if("GET".equals(method)){
            params = req.getQueryString();
        }else if("POST".equals(method)){
            BufferedReader reader = req.getReader();
            params = reader.readLine();
        }
        //将请求参数进行打印控制台
        System.out.println(params); 
    }
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        this.doGet(req, resp);
    }
}
```
使用 `request` 的 `getMethod()` 来获取请求方式，根据请求方式的不同分别获取请求参数值，这样就可以解决上述问题。但以后每个Servlet都需要这样写代码，实现起来比较麻烦，**这种方案我们不采用**。

解决方案二：`request` 对象已经将上述获取请求参数的方法进行了封装，并且 `request` 提供的方法实现的功能更强大，以后只需要调用 `request` 提供的方法即可，在 `request` 的方法中都实现了哪些操作？
1. 根据不同的请求方式获取请求参数，获取的内容：`username=zhangsan&hobby=1&hobby=2` 
2. 把获取到的内容进行分割，内容如下： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092139001.png)

3. 把分割后端数据，存入到一个 `Map` 集合中。**注意**：因为参数的值可能是一个，也可能有多个，所以 `Map` 的值的类型为 `String` 数组。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092139660.png)

基于上述理论，`request` 对象为我们提供了如下方法：
* 获取包含所有参数的 `Map` 集合
  ```java
  Map<String, String[]> getParameterMap()
  ```
* 根据名称获取参数值（数组）
  ```java
  String[] getParameterValues(String name)
  ```
* 根据名称获取参数值（单个值）
  ```java
  String getParameter(String name)
  ```

接下来，我们通过案例来把上述的三个方法进行实例演示：
1.修改 `req.html` 页面，添加爱好选项，爱好可以同时选多个
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<form action="/request-demo/req2" method="get">
    <input type="text" name="username"><br>
    <input type="password" name="password"><br>
    <input type="checkbox" name="hobby" value="1"> 游泳
    <input type="checkbox" name="hobby" value="2"> 爬山 <br>
    <input type="submit"> 
</form>
</body>
</html>
```

2.在Servlet代码中，获取页面传递的GET请求的参数值
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092256834.png)
3.获取GET方式的所有请求参数
```java
/**
 * request 通用方式获取请求参数
 */
@WebServlet("/req2")
public class RequestDemo2 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //GET请求逻辑
        System.out.println("get....");
        //1. 获取所有参数的Map集合
        Map<String, String[]> map = req.getParameterMap();
        for (String key : map.keySet()) {
            // username:zhangsan lisi
            System.out.print(key + ":"); 
            //获取值
            String[] values = map.get(key);
            for (String value : values) {
                System.out.print(value + " ");
            }
            System.out.println();
        }
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
    }
}
```
获取的结果为：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092257701.png)
4.获取GET请求参数中的爱好，结果是数组值
```java
/**
 * request 通用方式获取请求参数
 */
@WebServlet("/req2")
public class RequestDemo2 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //GET请求逻辑
        //...
        System.out.println("------------");
        String[] hobbies = req.getParameterValues("hobby");
        for (String hobby : hobbies) {
            System.out.println(hobby);
        }
    }
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
    }
}
```
获取的结果为：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092257495.png)
5.获取GET请求参数中的用户名和密码，结果是单个值
```java
/**
 * request 通用方式获取请求参数
 */
@WebServlet("/req2")
public class RequestDemo2 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //GET请求逻辑
        //...
        String username = req.getParameter("username");
        String password = req.getParameter("password");
        System.out.println(username);
        System.out.println(password);
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
    }
}
```
获取的结果为：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092258145.png)
6.在Servlet代码中获取页面传递POST请求的参数值。将 `req.html` 页面 `form` 表单的提交方式改成 `post` ，将 `doGet` 方法中的内容复制到 `doPost` 方法中即可。

**小结**——`req.getParameter()` 方法使用的频率会比较高；以后我们再写代码时，就只需要按照如下格式来编写：
```java
public class RequestDemo1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
       //采用request提供的获取请求参数的通用方式来获取请求参数
       //编写其他的业务代码...
    }
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        this.doGet(req, resp);
    }
}
```

### 4.1.3 请求参数中文乱码问题
先展示：将 `req.html` 页面的请求方式修改为 `get` 。
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<form action="/request-demo/req2" method="get">
    <input type="text" name="username"><br>
    <input type="password" name="password"><br>
    <input type="checkbox" name="hobby" value="1"> 游泳
    <input type="checkbox" name="hobby" value="2"> 爬山 <br>
    <input type="submit">
</form>
</body>
</html>
```
在Servlet方法中获取参数，并打印：
```java
/**
 * 中文乱码问题解决方案
 */
@WebServlet("/req4")
public class RequestDemo4 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
       //1. 获取username
       String username = request.getParameter("username");
       System.out.println(username);
    } 
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
启动服务器，页面上输入中文参数
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092309586.png)

查看控制台打印内容： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092349313.png)

把 `req.html` 页面的请求方式改成 `post` ，再次发送请求和中文参数，查看控制台打印内容，依然为乱码。

通过上面的案例，会发现，不管是GET还是POST请求，在发送的请求参数中如果有中文，在后台接收时，都会出现中文乱码的问题。具体该如何解决呢？
#### 1. POST请求解决方案
分析出现中文乱码的原因：
* POST的请求参数是通过 `request` 的 `getReader()` 来获取流中的数据
* TOMCAT在获取流时采用的编码是ISO-8859-1
* **ISO-8859-1编码是不支持中文的，所以会出现乱码**

解决方案：
* HTML页面设置的编码格式为UTF-8
* 把TOMCAT在获取流数据之前的编码设置为UTF-8
* 通过 `request.setCharacterEncoding("UTF-8")` 设置输入流的编码，UTF-8也可以写成小写

修改后的代码为：
```java
/**
 * 中文乱码问题解决方案
 */
@WebServlet("/req4")
public class RequestDemo4 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        //1. 解决乱码: POST getReader()
        //设置字符输入流的编码，设置的字符集要和页面保持一致
        request.setCharacterEncoding("UTF-8");
       //2. 获取username
       String username = request.getParameter("username");
       System.out.println(username);
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```

重新发送POST请求，就会在控制台看到正常展示的中文结果。至此POST请求中文乱码的问题就已经解决，但这种方案不适用于GET请求，这个原因是什么呢，咱们下面再分析。
#### 2. GET请求解决方案
刚才提到一个问题是，POST请求的中文乱码解决方案为什么不适用GET请求？
* GET请求获取请求参数的方式是 `request.getQueryString()`
* POST请求获取请求参数的方式是 `request.getReader()`
* `request.setCharacterEncoding("utf-8")` 是设置 `request` 处理流的编码
* **`getQueryString` 方法并没有通过流的方式获取数据**

所以GET请求不能用设置编码的方式来解决中文乱码问题，那问题又来了，如何解决GET请求的中文乱码呢？首先我们需要先分析下GET请求出现乱码的原因：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092353655.png)
浏览器通过HTTP协议发送请求和数据给后台服务器（Tomcat）；浏览器在发送HTTP的过程中会对中文数据进行URL==编码==。**在进行URL编码时，会采用页面 `<meta>` 标签指定的UTF-8进行编码**，`张三` 编码后的结果为 `%E5%BC%A0%E4%B8%89` 。后台服务器（Tomcat）接收到 `%E5%BC%A0%E4%B8%89` 后会默认按照 `ISO-8859-1` 进行URL==解码==。由于前后编码与解码采用的格式不一样，就会导致后台获取到的数据为乱码。

思考：如果把 `req.html` 页面的 `<meta>` 标签的 `charset` 属性改成 `ISO-8859-1` ，后台不做操作，能解决中文乱码问题么？答案是否定的，因为 **`ISO-8859-1` 本身是不支持中文展示的，所以改了 `<meta>` 标签的 `charset` 属性后，会导致页面上的中文内容都无法正常展示**。

分析完上面的问题后，我们会发现，其中有两个我们不熟悉的内容就是==URL编码==和==URL解码==，什么是URL编码，什么又是URL解码呢？

对于**URL编码**这块知识，只需要了解下即可。具体编码过程分两步，分别是：
1. 将字符串按照编码方式转为二进制
2. 每个字节转为2个16进制数并在前边加上 `%`

`张三` 按照UTF-8的方式转换成二进制的结果为：
```
1110 0101 1011 1100 1010 0000 1110 0100 1011 1000 1000 1001
```
这个结果是如何计算的？使用 `http://www.mytju.com/classcode/tools/encode_utf8.asp` ，输入 `张三` ： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092357625.png)
就可以获取张和三分别对应的10进制，然后在使用计算器，选择程序员模式，计算出对应的二进制数据结果： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092358190.png)
在计算的十六进制结果中，每两位前面加一个 `%` ，就可以获取到 `%E5%BC%A0%E4%B8%89`。当然，从上面所提供的网站中就已经能看到编码16进制的结果了。 

对于上面的计算过程，如果没有工具，纯手工计算的话，相对来说还是比较复杂的，我们也不需要进行手动计算，在Java中已经为我们提供了编码和解码的API工具类，可以让我们更快速的进行编码和解码。
```java
// 编码
java.net.URLEncoder.encode("需要被编码的内容","字符集(UTF-8)")
// 解码 
java.net.URLDecoder.decode("需要被解码的内容","字符集(UTF-8)")
```
接下来咱们对 `张三` 来进行编码和解码：
```java
public class URLDemo {
	public static void main(String[] args) throws UnsupportedEncodingException {
        String username = "张三";
        //1. URL编码
        String encode = URLEncoder.encode(username, "utf-8");
        System.out.println(encode); //打印:%E5%BC%A0%E4%B8%89
       //2. URL解码
       //String decode = URLDecoder.decode(encode, "utf-8"); //打印:张三
       String decode = URLDecoder.decode(encode, "ISO-8859-1"); //打印:`å¼ ä¸ 
       System.out.println(decode);
    }
}
```

到这，我们就可以分析出GET请求中文参数出现乱码的原因了，
* 浏览器把中文参数按照 `UTF-8` 进行URL编码
* Tomcat对获取到的内容进行了 `ISO-8859-1` 的URL解码
* 在控制台就会出现类上 `å¼ ä¸` 的乱码，最后一位是个空格

清楚了出现乱码的原因，接下来我们就需要想办法进行解决：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100006670.png)
从上图可以看出，在进行编码和解码时，不管使用的是哪个字符集，他们对应的 `%E5%BC%A0%E4%B8%89` 是一致的。那他们对应的二进制值也是一样的，为：
```
1110 0101 1011 1100 1010 0000 1110 0100 1011 1000 1000 1001
```
所以我们可以考虑把 `å¼ ä¸` 转换成字节，再把字节转换成 `张三` ，在转换的过程中它们的编码一致，就可以解决中文乱码问题。具体的实现步骤为：
1. 按照ISO-8859-1编码获取乱码 `å¼ ä¸` 对应的字节数组
2. 按照UTF-8编码获取字节数组对应的字符串

实现代码如下：
```java
public class URLDemo {
	public static void main(String[] args) throws UnsupportedEncodingException {
        String username = "张三";
        //1. URL编码
        String encode = URLEncoder.encode(username, "utf-8");
        System.out.println(encode);
        //2. URL解码
        String decode = URLDecoder.decode(encode, "ISO-8859-1");

        System.out.println(decode); //此处打印的是对应的乱码数据

        //3. 转换为字节数据,编码
        byte[] bytes = decode.getBytes("ISO-8859-1");
        for (byte b : bytes) {
            System.out.print(b + " ");
        }
		//此处打印的是: -27 -68 -96 -28 -72 -119
        //4. 将字节数组转为字符串，解码
        String s = new String(bytes, "utf-8");
        System.out.println(s); //此处打印的是张三
    }
}
```
**说明**：在第18行中打印的数据是 `-27 -68 -96 -28 -72 -119` 和`张三`转换成的二进制数据 `1110 0101 1011 1100 1010 0000 1110 0100 1011 1000 1000 1001` 为什么不一样呢？其实打印出来的是十进制数据，我们只需要使用计算机换算下就能得到他们的对应关系，如下图： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100009268.png)

至此对于GET请求中文乱码的解决方案，我们就已经分析完了，最后在代码中去实现下：
```java
/**
 * 中文乱码问题解决方案
 */
@WebServlet("/req4")
public class RequestDemo4 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        //1. 解决乱码：POST getReader()
        //request.setCharacterEncoding("UTF-8"); //设置字符输入流的编码
        //2. 获取username
        String username = request.getParameter("username");
        System.out.println("解决乱码前："+username);
        
        //3. GET获取参数的方式：getQueryString
        // 乱码原因: tomcat进行URL解码，默认的字符集ISO-8859-1
       /* 
        //3.1 先对乱码数据进行编码: 转为字节数组
        byte[] bytes = username.getBytes(StandardCharsets.ISO_8859_1);
        //3.2 字节数组解码
        username = new String(bytes, StandardCharsets.UTF_8);
        */
        username  = new String(username.getBytes(StandardCharsets.ISO_8859_1), StandardCharsets.UTF_8);
        System.out.println("解决乱码后:" + username);
    }
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
**注意**：
* 把 `request.setCharacterEncoding("UTF-8")` 代码注释掉后，会发现「GET请求参数乱码解决方案」同时也可也把「POST请求参数乱码的问题」解决了
* 只不过对于POST请求参数一般都会比较多，采用这种方式解决乱码起来比较麻烦，所以对于POST请求还是建议使用设置编码的方式进行。

另外需要说明一点的是，==Tomcat8.0之后，已将GET请求乱码问题解决，设置默认的解码方式为UTF-8==。

**小结**
1. 中文乱码解决方案：POST请求和GET请求的参数中如果有中文，后台接收数据就会出现中文乱码问题。其中，GET请求在Tomcat8.0以后的版本就不会出现乱码问题了。POST请求解决方案是：设置输入流的编码。
   ```java
   request.setCharacterEncoding("UTF-8");
   // 注意: 设置的字符集要和页面保持一致
   ```
2. 通用方式（GET/POST）：需要先解码，再编码
   ```java
   new String(username.getBytes("ISO-8859-1"),"UTF-8");
   ```

## 4.2 IDEA快速创建Servlet
**使用通用方式获取请求参数后，屏蔽了GET和POST的请求方式代码的不同**，则代码可以定义如下格式：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092306685.png)

由于格式固定，所以我们可以使用IDEA提供的模板来制作一个Servlet的模板，这样我们后期在创建Servlet时就会更高效，具体如何实现：
1. 按照自己的需求，修改Servlet创建的模板内容
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092307935.png)
2. 使用Servlet模板创建Servlet类
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302092307190.png)

## 4.3 `Request` 请求转发
==请求转发 *forward* 是一种**在服务器内部的资源跳转方式**==。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100016016.png)
1. 浏览器发送请求给服务器，服务器中对应的资源A接收到请求
2. 资源A处理完请求后将请求发给资源B
3. 资源B处理完后将结果响应给浏览器
4. 请求从资源A到资源B的过程就叫==请求转发==

请求转发的实现方式： 
```java
req.getRequestDispatcher("资源B路径").forward(req, resp);
```
具体如何来使用，我们先来看下需求：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100019018.png)

针对上述需求，具体的实现步骤如下。
1.创建一个 `RequestDemo5` 类，接收 `/req5` 的请求，在 `doGet` 方法中打印 `demo5` ：
```java
/**
 * 请求转发
 */
@WebServlet("/req5")
public class RequestDemo5 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("demo5...");
    } 
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
2.创建一个 `RequestDemo6` 类，接收 `/req6` 的请求，在 `doGet` 方法中打印 `demo6`
```java
/**
 * 请求转发
 */
@WebServlet("/req6")
public class RequestDemo6 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("demo6...");
    } 
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
3.在 `RequestDemo5` 的 `doGet` 方法中进行请求转发，使用 `req.getRequestDispatcher("/req6").forward(req,resp)` 进行请求转发：
```java
/**
 * 请求转发
 */
@WebServlet("/req5")
public class RequestDemo5 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("demo5...");
        //请求转发
        request.getRequestDispatcher("/req6").forward(request,response);
    } 
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
4.启动测试。访问 `http://localhost:8080/request-demo/req5` ，就可以在控制台看到如下内容，说明请求已经转发到了 `/req6` ：
```java
demo5...
demo6...
```

**请求转发资源间共享数据：使用的是 `Request` 对象**。此处主要解决的问题是，在把请求从 `/req5` 转发到 `/req6` 时，如何传递数据给 `/req6` 。需要使用 `Request` 对象提供的三个方法：
* 存储数据到 `request` 域（范围，数据是存储在 `request` 对象）中
   ```java
   void setAttribute(String name, Object o);
   ```
* 根据 `key` 获取值
   ```java
   Object getAttribute(String name);
   ```
* 根据 `key` 删除该键值对
   ```java
   void removeAttribute(String name);
   ```

接着上个需求来：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100026919.png)
1.修改 `RequestDemo5` 中的方法。在 `RequestDemo5` 的 `doGet` 方法中转发请求之前，将数据存入 `request` 域对象中：
```java
@WebServlet("/req5")
public class RequestDemo5 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("demo5...");
        //存储数据
        request.setAttribute("msg","hello");
        //请求转发
        request.getRequestDispatcher("/req6").forward(request,response);
    }
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
2.修改 `RequestDemo6` 中的方法。在 `RequestDemo6` 的 `doGet` 方法中，从 `request` 域对象中获取数据，并将数据打印到控制台：
```java
/**
 * 请求转发
 */
@WebServlet("/req6")
public class RequestDemo6 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("demo6...");
        //获取数据
        Object msg = request.getAttribute("msg");
        System.out.println(msg); 
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
3.启动测试。访问 `http://localhost:8080/request-demo/req5` ，就可以在控制台看到如下内容：
```java
demo5...
demo6...
hello
```
此时就可以实现在转发多个资源之间共享数据。

请求转发的特点：
* **浏览器地址栏路径不发生变化**——虽然后台从 `/req5` 转发到 `/req6` ，但是浏览器的地址一直是 `/req5` ，未发生变化：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100031046.png)
* **只能转发到当前服务器的内部资源**，不能从一个服务器通过转发访问另一台服务器
* **一次请求**，可以在转发资源间使用 `request` 共享数据——虽然后台从 `/req5` 转发到 `/req6` ，但这个==只有一次请求==。

## 4.4 `Response` 对象
前面讲解完 `Request` 对象，接下来我们回到刚开始的那张图： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100032304.png)
* `Request` ：使用 `request` 对象来==获取==请求数据
* `Response` ：使用 `response` 对象来==设置==响应数据

`Reponse` 的继承体系和 `Request` 的继承体系也非常相似： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100035854.png)

介绍完 `Response` 的相关体系结构后，对于 `Response` 需要学习如下内容：
* `Response` 设置响应数据的功能介绍
* `Response` 完成**重定向**
* `Response` 响应字符数据
* `Response` 响应字节数据

### 4.4.1 `Response` 设置响应数据功能介绍
HTTP响应数据总共分为三部分内容，分别是==响应行、响应头、响应体==，对于这三部分内容的数据，`response` 对象都提供了哪些方法来进行设置？
1. 响应行 
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100038304.png)
	对于响应头，比较常用的就是设置响应状态码：
   ```java
   void setStatus(int sc);
   ```
2. 响应头。设置响应头键值对：
   ```java
   void setHeader(String name,String value);
   ```
3. 响应体：如 `<html><head></head><body></body></html>` 。对于响应体，是通过字符、字节输出流的方式往浏览器写，获取字符输出流：
   ```java
   PrintWriter getWriter();
   ```
	获取字节输出流：
   ```java
   ServletOutputStream getOutputStream();
   ```

介绍完这些方法后，后面会通过案例把这些方法都用一用，首先先来完成下重定向的功能开发。
### 4.4.2 `Response` 请求重定向
`Response` 重定向 *redirect* 也是==一种资源跳转方式==。 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100047997.png)
1. 浏览器发送请求给服务器，服务器中对应的资源A接收到请求
2. 资源A现在无法处理该请求，就会给浏览器**响应一个302的状态码** + **`location` 的一个访问资源B的路径**
3. 浏览器接收到响应状态码为302，就会重新发送请求到「 `location` 对应的访问地址」去访问资源B
4. 资源B接收到请求后进行处理，并最终给浏览器响应结果，这整个过程就叫==重定向==

重定向的实现方式：
```java
resp.setStatus(302);
resp.setHeader("location", "资源B的访问路径");
```
具体如何来使用，我们先来看下需求： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100051998.png)
1.创建一个 `ResponseDemo1` 类，接收 `/resp1` 的请求，在 `doGet` 方法中打印 `resp1....`
```java
@WebServlet("/resp1")
public class ResponseDemo1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("resp1....");
    }
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
2.创建一个 `ResponseDemo2` 类，接收 `/resp2` 的请求，在 `doGet` 方法中打印 `resp2....`
```java
@WebServlet("/resp2")
public class ResponseDemo2 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("resp2....");
    }
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
3.在 `ResponseDemo1` 的 `doGet` 方法中，使用 `response.setStatus(302); response.setHeader("Location","/request-demo/resp2")` 来给前端响应结果数据。
```java
@WebServlet("/resp1")
public class ResponseDemo1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("resp1....");
        //重定向
        //1.设置响应状态码 302
        response.setStatus(302);
        //2. 设置响应头 Location
        response.setHeader("Location", "/request-demo/resp2");
    }
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
4.启动测试。访问 `http://localhost:8080/request-demo/resp1` ，就可以在控制台看到如下内容： 
```java
resp1....
resp2....
```
说明 `/resp1` 和 `/resp2` 都被访问到了。到这重定向就已经完成了。

虽然功能已经实现，但从设置重定向的两行代码来看，会发现除了重定向的地址不一样，其他的内容都是一模一样，所以 `request` 对象给我们提供了简化的编写方式为：
```java
resposne.sendRedirect("/request-demo/resp2")
```
所以第3步中的代码就可以简化为：
```java
@WebServlet("/resp1")
public class ResponseDemo1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("resp1....");
        //重定向
        resposne.sendRedirect("/request-demo/resp2")；
    }
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```

重定向的特点：
* **浏览器地址栏路径发送变化**。当进行重定向访问时，由于是由浏览器发送的两次请求，所以地址会发生变化： 
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100059854.png)
* **可以重定向到任何位置的资源**（服务器内部、外部均可）。因为第一次响应结果中包含了**浏览器下次要跳转的路径**，所以这个路径是可以任意位置资源。
* **两次请求**，不能在多个资源使用 `request` 共享数据。因为浏览器发送了两次请求，是两个不同的 `request` 对象，就无法通过 `request` 对象进行共享数据。

介绍完==请求重定向==和==请求转发==以后，接下来需要把这两个放在一块对比下： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100108206.png)
以后到底用哪个，还是需要根据具体的业务来决定。
### 4.4.3 路径问题
问题1：转发时路径上没有加 `/request-demo`而重定向加了，那么到底什么时候需要加，什么时候不需要加呢？
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100109933.png)

其实判断的依据很简单，只需要记住下面的规则即可：
* 浏览器使用：需要加虚拟目录（项目访问路径）
* 服务端使用：不需要加虚拟目录

对于转发来说，因为是在服务端进行的，所以不需要加虚拟目录。==对于重定向来说，路径最终是由浏览器来发送请求，就需要添加虚拟目录==。掌握了这个规则，接下来就通过一些练习来强化下知识的学习：
* `<a href='路径'>`
* `<form action='路径'>`
* `req.getRequestDispatcher("路径")`
* `resp.sendRedirect("路径")`

答案：
```java
1.超链接，从浏览器发送，需要加
2.表单，从浏览器发送，需要加
3.转发，是从服务器内部跳转，不需要加
4.重定向，是由浏览器进行跳转，需要加。
```

问题2：在重定向的代码中，`/request-demo` 是固定编码的，如果后期通过Tomcat插件**配置了项目的访问路径**，那么所有需要**重定向**的地方都需要重新修改，该如何优化？
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100115595.png)
答案也比较简单，我们可以**在代码中动态去获取项目访问的虚拟目录**，具体如何获取，可以借助前面所学习的 `request` 对象中的 `getContextPath()` 方法，修改后的代码如下：
```java
@WebServlet("/resp1")
public class ResponseDemo1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        System.out.println("resp1....");
        //简化方式完成重定向
        //动态获取虚拟目录
        String contextPath = request.getContextPath();
        response.sendRedirect(contextPath + "/resp2");
    }
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
重新启动访问测试，功能依然能够实现，此时就可以**动态获取项目访问的虚拟路径**，从而降低代码的耦合度。

### 4.4.4 `Response` 响应字符数据
要想将字符数据写回到浏览器，我们需要两个步骤：
* 通过 `Response` 对象获取字符输出流：`PrintWriter writer = resp.getWriter();`
* 通过字符输出流写数据：`writer.write("aaa");`

接下来，我们实现通过些案例把响应字符数据给实际应用下：
1.返回一个简单的字符串 `aaa`
```java
/**
 * 响应字符数据：设置字符数据的响应体
 */
@WebServlet("/resp3")
public class ResponseDemo3 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        response.setContentType("text/html;charset=utf-8");
        //1. 获取字符输出流
        PrintWriter writer = response.getWriter();
		writer.write("aaa");
    }
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100120014.png)

2.返回一串HTML字符串，并且能被浏览器解析：
```java
PrintWriter writer = response.getWriter();
//content-type, 告诉浏览器返回的数据类型是HTML类型数据, 这样浏览器才会解析HTML标签
response.setHeader("content-type","text/html");
writer.write("<h1>aaa</h1>");
```
![img|550x420](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100120721.png)

==注意==：**一次请求响应结束后，`response` 对象就会被销毁掉**，所以不要手动关闭流。

3.返回一个中文的字符串 `你好` ，需要注意设置响应数据的编码为 `utf-8` ，否则会乱码：
```java
//设置响应的数据格式及数据的编码
response.setContentType("text/html;charset=utf-8");
writer.write("你好");
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302100122530.png)

### 4.4.5 `Response` 响应字节数据
要想将字节数据写回到浏览器，我们需要两个步骤：
- 通过 `Response` 对象获取字节输出流：`ServletOutputStream outputStream = resp.getOutputStream();`
- 通过字节输出流写数据：`outputStream.write(字节数据);`

接下来，我们实现通过案例把响应字节数据给实际应用。
1.返回一个图片文件到浏览器
```java
/**
 * 响应字节数据：设置字节数据的响应体
 */
@WebServlet("/resp4")
public class ResponseDemo4 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        //1. 读取文件
        FileInputStream fis = new FileInputStream("d://a.jpg");
        //2. 获取response字节输出流
        ServletOutputStream os = response.getOutputStream();
        //3. 完成流的copy
        byte[] buff = new byte[1024];
        int len = 0;
        while ((len = fis.read(buff)) != -1){
            os.write(buff, 0, len);
        }
        fis.close();
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302101552731.png)
 上述代码中，对于流的 `copy` 的代码还是比较复杂的，所以我们可以使用别人提供好的方法来简化代码的开发，具体的步骤是：先在 `pom.xml` 添加依赖
```xml
<dependency>
    <groupId>commons-io</groupId>
    <artifactId>commons-io</artifactId>
    <version>2.6</version>
</dependency>
```
再调用工具类方法：
```java
//fis:输入流
//os:输出流
IOUtils.copy(fis, os);
```
优化后的代码：
```java
/**
 * 响应字节数据：设置字节数据的响应体
 */
@WebServlet("/resp4")
public class ResponseDemo4 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        //1. 读取文件
        FileInputStream fis = new FileInputStream("d://a.jpg");
        //2. 获取response字节输出流
        ServletOutputStream os = response.getOutputStream();
        //3. 完成流的copy
      	IOUtils.copy(fis, os);
        fis.close();
    }
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```

## 4.5 用户注册登录案例
接下来通过两个比较常见的案例，一个是==注册==，一个是==登录==、来对学习内容进行实战演练，首先来实现用户登录。
### 4.5.1 用户登录
#### 1. 需求分析 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302101553025.png)

1. 用户在登录页面输入用户名和密码，提交请求给 `LoginServlet`
2. 在 `LoginServlet` 中接收请求和数据[用户名和密码]
3. 在 `LoginServlt` 中通过Mybatis实现调用 `UserMapper` 、来根据用户名和密码查询数据库表
4. 将查询的结果封装到 `User` 对象中进行返回
5. 在 `LoginServlet` 中判断返回的 `User` 对象是否为 `null`
6. 如果为 `null` ，说明根据用户名和密码没有查询到用户，则登录失败，返回"登录失败"数据给前端
7. 如果不为 `null` ，则说明用户存在并且密码正确，则登录成功，返回"登录成功"数据给前端

#### 2. 环境准备
1.复制资料中的静态页面到项目的 `webapp` 目录下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302101556208.png)
这是 `login.html` ：
```html
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <title>login</title>
    <link href="css/login.css" rel="stylesheet">
</head>

<body>
<div id="loginDiv">
    <form action="/request-demo/loginServlet" method="post" id="form">
        <h1 id="loginMsg">LOGIN IN</h1>
        <p>Username:<input id="username" name="username" type="text"></p>

        <p>Password:<input id="password" name="password" type="password"></p>

        <div id="subDiv">
            <input type="submit" class="button" value="login up">
            <input type="reset" class="button" value="reset">&nbsp;&nbsp;&nbsp;
            <a href="register.html">没有账号？点击注册</a>
        </div>
    </form>
</div>

</body>
</html>
```
`login.css` ：
```css
* {
    margin: 0;
    padding: 0;
}

html {
    height: 100%;
    width: 100%;
    overflow: hidden;
    margin: 0;
    padding: 0;
    background: url(../imgs/Desert.jpg) no-repeat 0px 0px;
    background-repeat: no-repeat;
    background-size: 100% 100%;
    -moz-background-size: 100% 100%;
}

body {
    display: flex;
    align-items: center;
    justify-content: center;
    height: 100%;
}

#loginDiv {
    width: 37%;
    display: flex;
    justify-content: center;
    align-items: center;
    height: 300px;
    background-color: rgba(75, 81, 95, 0.3);
    box-shadow: 7px 7px 17px rgba(52, 56, 66, 0.5);
    border-radius: 5px;
}

#name_trip {
    margin-left: 50px;
    color: red;
}

p {
    margin-top: 30px;
    margin-left: 20px;
    color: azure;
}

input {
    margin-left: 15px;
    border-radius: 5px;
    border-style: hidden;
    height: 30px;
    width: 140px;
    background-color: rgba(216, 191, 216, 0.5);
    outline: none;
    color: #f0edf3;
    padding-left: 10px;
}
#username{
    width: 200px;
}
#password{
    width: 202px;
}
.button {
    border-color: cornsilk;
    background-color: rgba(100, 149, 237, .7);
    color: aliceblue;
    border-style: hidden;
    border-radius: 5px;
    width: 100px;
    height: 31px;
    font-size: 16px;
}

#subDiv {
    text-align: center;
    margin-top: 30px;
}
#loginMsg{
    text-align: center;color: aliceblue;
}
```
然后是 `register.html` ：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>欢迎注册</title>
    <link href="css/register.css" rel="stylesheet">
</head>
<body>
	<div class="form-div">
	    <div class="reg-content">
	        <h1>欢迎注册</h1>
	        <span>已有帐号？</span> <a href="login.html">登录</a>
	    </div>
	    <form id="reg-form" action="/request-demo/registerServlet" method="post">
	
	        <table> 
	            <tr>
	                <td>用户名</td>
	                <td class="inputs">
	                    <input name="username" type="text" id="username">
	                    <br>
	                    <span id="username_err" class="err_msg" style="display: none">用户名不太受欢迎</span>
	                </td> 
	            </tr> 
	            <tr>
	                <td>密码</td>
	                <td class="inputs">
	                    <input name="password" type="password" id="password">
	                    <br>
	                    <span id="password_err" class="err_msg" style="display: none">密码格式有误</span>
	                </td>
	            </tr> 
	        </table> 
	        <div class="buttons">
	            <input value="注 册" type="submit" id="reg_btn">
	        </div>
	        <br class="clear">
	    </form> 
	</div>
</body>
</html>
```
`register.css` ：
```css
```css
* {
    margin: 0;
    padding: 0;
    list-style-type: none;
}
.reg-content{
    padding: 30px;
    margin: 3px;
}
a, img {
    border: 0;
}

body {
    background-image: url("../imgs/reg_bg_min.jpg") ;
    text-align: center;
}

table {
    border-collapse: collapse;
    border-spacing: 0;
}

td, th {
    padding: 0;
    height: 90px;

}
.inputs{
    vertical-align: top;
}

.clear {
    clear: both;
}

.clear:before, .clear:after {
    content: "";
    display: table;
}

.clear:after {
    clear: both;
}

.form-div {
    background-color: rgba(255, 255, 255, 0.27);
    border-radius: 10px;
    border: 1px solid #aaa;
    width: 424px;
    margin-top: 150px;
    margin-left:1050px;
    padding: 30px 0 20px 0px;
    font-size: 16px;
    box-shadow: inset 0px 0px 10px rgba(255, 255, 255, 0.5), 0px 0px 15px rgba(75, 75, 75, 0.3);
    text-align: left;
}

.form-div input[type="text"], .form-div input[type="password"], .form-div input[type="email"] {
    width: 268px;
    margin: 10px;
    line-height: 20px;
    font-size: 16px;
}

.form-div input[type="checkbox"] {
    margin: 20px 0 20px 10px;
}

.form-div input[type="button"], .form-div input[type="submit"] {
    margin: 10px 20px 0 0;
}

.form-div table {
    margin: 0 auto;
    text-align: right;
    color: rgba(64, 64, 64, 1.00);
}

.form-div table img {
    vertical-align: middle;
    margin: 0 0 5px 0;
}

.footer {
    color: rgba(64, 64, 64, 1.00);
    font-size: 12px;
    margin-top: 30px;
}

.form-div .buttons {
    float: right;
}

input[type="text"], input[type="password"], input[type="email"] {
    border-radius: 8px;
    box-shadow: inset 0 2px 5px #eee;
    padding: 10px;
    border: 1px solid #D4D4D4;
    color: #333333;
    margin-top: 5px;
}

input[type="text"]:focus, input[type="password"]:focus, input[type="email"]:focus {
    border: 1px solid #50afeb;
    outline: none;
}

input[type="button"], input[type="submit"] {
    padding: 7px 15px;
    background-color: #3c6db0;
    text-align: center;
    border-radius: 5px;
    overflow: hidden;
    min-width: 80px;
    border: none;
    color: #FFF;
    box-shadow: 1px 1px 1px rgba(75, 75, 75, 0.3);
}

input[type="button"]:hover, input[type="submit"]:hover {
    background-color: #5a88c8;
}

input[type="button"]:active, input[type="submit"]:active {
    background-color: #5a88c8;
}
.err_msg{
    color: red;
    padding-right: 170px;
}
#password_err,#tel_err{
    padding-right: 195px;
}

#reg_btn{
    margin-right:50px; width: 285px; height: 45px; margin-top:20px;
}
```
`reg_bg_min.jpg` 和 `Desert.jpg` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302101635736.jpg)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302101635420.jpg)
2.创建 `db1` 数据库，创建 `tb_user` 表，创建 `User` 实体类。将如下SQL语句执行下：
```sql
-- 创建用户表
CREATE TABLE tb_user(
	id int primary key auto_increment,
	username varchar(20) unique,
	password varchar(32)
);

-- 添加数据
INSERT INTO tb_user(username,password) values('zhangsan','123'),('lisi','234');

SELECT * FROM tb_user;
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302101604178.png)
再将 `User.java` 拷贝到 `com.itheima.pojo` ：
```java
package com.itheima.pojo;

public class User {

    private Integer id;
    private String username;
    private String password;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    @Override
    public String toString() {
        return "User{" +
                "id=" + id +
                ", username='" + username + '\'' +
                ", password='" + password + '\'' +
                '}';
    }
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302101604520.png)
3.在项目的 `pom.xml` 导入Mybatis和MySQL驱动坐标
```xml
<dependency>
	<groupId>org.mybatis</groupId>
	<artifactId>mybatis</artifactId>
	<version>3.5.5</version>
</dependency>

<dependency>
	<groupId>mysql</groupId>
	<artifactId>mysql-connector-java</artifactId>
	<version>5.1.34</version>
</dependency>
```

4.创建 `mybatis-config.xml` 核心配置文件，`UserMapper.xml` 映射文件，`UserMapper` 接口
4.1 将 `mybatis-config.xml` 拷贝到 `resources` 目录下
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
        PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <!--起别名-->
    <typeAliases>
        <package name="com.itheima.pojo"/>
    </typeAliases>

    <environments default="development">
        <environment id="development">
            <transactionManager type="JDBC"/>
            <dataSource type="POOLED">
                <property name="driver" value="com.mysql.jdbc.Driver"/>
                <!--
                    useSSL:关闭SSL安全连接 性能更高
                    useServerPrepStmts:开启预编译功能
                    &amp; 等同于 &, xml配置文件中不能直接写 &符号
                -->
                <property name="url" value="jdbc:mysql:///db1?useSSL=false&amp;useServerPrepStmts=true"/>
                <property name="username" value="root"/>
                <property name="password" value="1234"/>
            </dataSource>
        </environment>
    </environments>
    <mappers>
        <!--扫描mapper-->
        <package name="com.itheima.mapper"/>
    </mappers>
</configuration>
```
4.2 在 `com.itheima.mapper` 包下创建 `UserMapper` 接口：
```java
public interface UserMapper {

}
```
4.3 将 `UserMapper.xml` 拷贝到 `resources` 目录下。==注意：在 `resources`下创建 `UserMapper.xml` 的目录时，要使用 `/` 分割==。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302101608030.png)
至此我们所需要的环境就都已经准备好了，具体该如何实现？
#### 3. 代码实现
1.在 `UserMapper` 接口中提供一个根据用户名和密码查询用户对象的方法：
```java
/**
     * 根据用户名和密码查询用户对象
     * @param username
     * @param password
     * @return
     */
    @Select("select * from tb_user where username = #{username} and password = #{password}")
    User select(@Param("username") String username,@Param("password")  String password);
```

**说明**：`@Param` 注解的作用：用于传递参数，使方法的参数可以与SQL中的字段名相对应。

2.修改 `loign.html`
```html
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <title>login</title>
    <link href="css/login.css" rel="stylesheet">
</head>

<body>
<div id="loginDiv">
    <form action="/request-demo/loginServlet" method="post" id="form">
        <h1 id="loginMsg">LOGIN IN</h1>
        <p>Username:<input id="username" name="username" type="text"></p>

        <p>Password:<input id="password" name="password" type="password"></p>

        <div id="subDiv">
            <input type="submit" class="button" value="login up">
            <input type="reset" class="button" value="reset">&nbsp;&nbsp;&nbsp;
            <a href="register.html">没有账号？点击注册</a>
        </div>
    </form>
</div>
</body>
</html>
```
3.编写 `LoginServlet` ：
```java
@WebServlet("/loginServlet")
public class LoginServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        //1. 接收用户名和密码
        String username = request.getParameter("username");
        String password = request.getParameter("password");

        //2. 调用MyBatis完成查询
        //2.1 获取SqlSessionFactory对象
        String resource = "mybatis-config.xml";
        InputStream inputStream = Resources.getResourceAsStream(resource);
        SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
        //2.2 获取SqlSession对象
        SqlSession sqlSession = sqlSessionFactory.openSession();
        //2.3 获取Mapper
        UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
        //2.4 调用方法
        User user = userMapper.select(username, password);
        //2.5 释放资源
        sqlSession.close();


        //获取字符输出流，并设置content type
        response.setContentType("text/html;charset=utf-8");
        PrintWriter writer = response.getWriter();
        //3. 判断user释放为null
        if(user != null){
            // 登陆成功
            writer.write("登陆成功");
        }else {
            // 登陆失败
            writer.write("登陆失败");
        }
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
4.启动服务器测试。如果用户名和密码输入错误，则 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302101640803.png)
如果用户名和密码输入正确，则
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302101641089.png)
至此用户的登录功能就已经完成了。
### 4.5.2 用户注册
#### 1. 需求分析
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302101642099.png)
1. 用户在注册页面输入用户名和密码，提交请求给 `RegisterServlet`
2. 在 `RegisterServlet` 中接收请求和数据[用户名和密码]
3. 在 `RegisterServlet` 中通过Mybatis实现、调用 `UserMapper` 来根据用户名查询数据库表
4. 将查询的结果封装到 `User` 对象中进行返回
5. 在 `RegisterServlet` 中判断返回的 `User` 对象是否为 `null`
6. 如果为 `null` ，说明根据用户名可用，则调用 `UserMapper` 来实现添加用户
7. 如果不为 `null` ，则说明用户不可用，返回"用户名已存在"数据给前端

#### 2. 代码编写
1. 编写 `UserMapper` 提供根据用户名查询用户数据方法和添加用户方法
```java
/**
 * 根据用户名查询用户对象
 * @param username
 * @return
 */
@Select("select * from tb_user where username = #{username}")
User selectByUsername(String username);

/**
 * 添加用户
 * @param user
 * id为null,实际上数据库内会自动增长
 */
@Insert("insert into tb_user values(null,#{username},#{password})")
void add(User user);
```
2.修改 `register.html`
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>欢迎注册</title>
    <link href="css/register.css" rel="stylesheet">
</head>
<body>
<div class="form-div">
    <div class="reg-content">
        <h1>欢迎注册</h1>
        <span>已有帐号？</span> <a href="login.html">登录</a>
    </div>
    <form id="reg-form" action="/request-demo/registerServlet" method="post">
        <table> 
            <tr>
                <td>用户名</td>
                <td class="inputs">
                    <input name="username" type="text" id="username">
                    <br>
                    <span id="username_err" class="err_msg" style="display: none">用户名不太受欢迎</span>
                </td>
            </tr>
            <tr>
                <td>密码</td>
                <td class="inputs">
                    <input name="password" type="password" id="password">
                    <br>
                    <span id="password_err" class="err_msg" style="display: none">密码格式有误</span>
                </td>
            </tr>
        </table>
        <div class="buttons">
            <input value="注 册" type="submit" id="reg_btn">
        </div>
        <br class="clear">
    </form>
</div>
</body>
</html>
```
3.创建 `RegisterServlet` 类
```java
@WebServlet("/registerServlet")
public class RegisterServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        //1. 接收用户数据
        String username = request.getParameter("username");
        String password = request.getParameter("password");

        //封装用户对象
        User user = new User();
        user.setUsername(username);
        user.setPassword(password);

        //2. 调用mapper 根据用户名查询用户对象
        //2.1 获取SqlSessionFactory对象
        String resource = "mybatis-config.xml";
        InputStream inputStream = Resources.getResourceAsStream(resource);
        SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
        //2.2 获取SqlSession对象
        SqlSession sqlSession = sqlSessionFactory.openSession();
        //2.3 获取Mapper
        UserMapper userMapper = sqlSession.getMapper(UserMapper.class);

        //2.4 调用方法
        User u = userMapper.selectByUsername(username);

        //3. 判断用户对象释放为null
        if( u == null){
            // 用户名不存在，添加用户
            userMapper.add(user); 
            // 提交事务
            sqlSession.commit();
            // 释放资源
            sqlSession.close();
        }else {
            // 用户名存在，给出提示信息
            response.setContentType("text/html;charset=utf-8");
            response.getWriter().write("用户名已存在");
        } 
    }
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
4.启动服务器进行测试
4.1 如果测试成功，则在数据库中就能查看到新注册的数据
4.2 如果用户已经存在，则在页面上展示 `用户名已存在` 的提示信息
## 4.6 `SqlSessionFactory` 工具类抽取
上面两个功能已经实现，但在写Servlet时，因为需要使用Mybatis来完成数据库的操作，所以对于Mybatis的基础操作就出现了些重复代码，如下
```java
String resource = "mybatis-config.xml";
InputStream inputStream = Resources.getResourceAsStream(resource);
SqlSessionFactory sqlSessionFactory = new 
	SqlSessionFactoryBuilder().build(inputStream);
```
有了这些重复代码就会造成一些问题：
* 重复代码不利于后期的维护
* `SqlSessionFactory` 工厂类进行重复创建——就相当于每次买手机都需要重新创建一个手机生产工厂来给你制造一个手机一样，资源消耗非常大但性能却非常低。所以这么做是不允许的。

那如何来优化呢？
* 代码重复可以抽取工具类
* **对指定代码只需要执行一次可以使用静态代码块**

有了这两个方向后，代码具体该如何编写?
```java
public class SqlSessionFactoryUtils {
    private static SqlSessionFactory sqlSessionFactory;
    static {
        //静态代码块会随着类的加载而自动执行,且只执行一次
        try {
            String resource = "mybatis-config.xml";
            InputStream inputStream = Resources.getResourceAsStream(resource);
            sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static SqlSessionFactory getSqlSessionFactory() {
        return sqlSessionFactory;
    }
}
```
工具类抽取以后，以后在对Mybatis的 `SqlSession` 进行操作时，就可以直接使用
```java
SqlSessionFactory sqlSessionFactory = SqlSessionFactoryUtils.getSqlSessionFactory();
```
这样就可以很好的解决上面所说的「代码重复和重复创建工厂导致性能低的问题」了。