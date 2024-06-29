<strong style="color: black;">本章重点</strong>：搭建ChatGPT-API工程，作为统一的后端访问入口，提供API能力。这些能力后随着章节的推进，不断地叠加，如；访问验证、OpenAI会话、公众号、企业微信等。
<strong>本章源码</strong>：<a href="https://gitcode.net/KnowledgePlanet/chatgpt/dev-ops/-/tree/230502-xfg-nginx-auth-request" target="_blank">https://gitcode.net/KnowledgePlanet/chatgpt/dev-ops/-/tree/230502-xfg-nginx-auth-request</a> <a href="https://gitcode.net/KnowledgePlanet/chatgpt/chatgpt-api/-/tree/230502-xfg-auth-request" target="_blank">https://gitcode.net/KnowledgePlanet/chatgpt/chatgpt-api/-/tree/230502-xfg-auth-request</a>  
<strong style="color: black;">课程视频</strong> ： https://t.zsxq.com/0dzm349us
<strong style="color: black;">作业仓库</strong>：https://gitcode.net/CreativeAlliance

---
# 一、本章诉求
一般研发人员需要完成一个完整需求前，最好是先梳理需求，并把「用于完成需求目标的各个功能节点」进行单个验证，以确保方案的可行性。有了这些基本功能模块的验证后，就可以逐步再把各个模块像乐高积木一样搭建起来，**搭建的过程就是架构和设计模式的运用**。

那么本章目标就是：==在 Nginx 访问接口时，做一些权限校验，只有校验通过才能访问接口，否则就直接返回失败==。

有了这样的控制，是不是能想象到，你在一些网站购买的一个月有效期的服务，过期就不能使用的场景。那么在本章节带着大家先搭建一个简单的 SpringBoot 工程，并在工程中提供用于**与 Nginx 的 `auth_request` 模块做验证处理**的访问接口。

本章节涉及到 Maven 工程的创建、代码提交、启动发布、**Nginx  auth 配置**等内容。
# 二、流程设计
整个流程为；**以用户视角访问API开始，进入 Nginx 的 auth 认证模块，调用 SpringBoot 提供的认证服务**。根据认证结果调用**重定向到对应的 API 接口或者 404 页面**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305201251001.png)
由于 OpenAI 或者本身自己训练的一套服务，都会有服务器成本。所以**基于这样一个模型结构，后续可以通过用户购买 Token 的时效性进行成本回收**。这也是其中一种商业变现的思路。
- 限频限次等

# 三、方案实现
# 0. 看有无http_auth模块
一开始在FinalShell中直接 `nginx -V` ，发现没有 `http_auth` 模块，吓到我了，还以为之前安装错了……后来想到，我之前宝塔面板安装了一个Nginx……
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305201321831.png)
可以自己添加模块：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305201817029.png)

## 1. API 工程
首先我们来创建一个 SpringBoot 工程，并在工程中提供简单的 API 接口。<strong>源码</strong>：<a href="https://gitcode.net/KnowledgePlanet/chatgpt/chatgpt-api" target="_blank">https://gitcode.net/KnowledgePlanet/chatgpt/chatgpt-api</a> 

导入依赖：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.memcpy0.chatgpt</groupId>
    <artifactId>chatgpt-api</artifactId>
    <version>1.0-SNAPSHOT</version>
    <packaging>jar</packaging>
    
    <properties>
        <maven.compiler.source>8</maven.compiler.source>
        <maven.compiler.target>8</maven.compiler.target>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    </properties>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.6.0</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>
    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-tx</artifactId>
            <version>5.3.13</version>
        </dependency>
        <!-- https://mvnrepository.com/artifact/com.alibaba/fastjson -->
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>fastjson</artifactId>
            <version>2.0.28</version>
        </dependency>
    </dependencies>
    <build>
        <finalName>chatgpt-api</finalName>
        <resources>
            <resource>
                <directory>src/main/resources</directory>
                <filtering>true</filtering>
                <includes>
                    <include>**/**</include>
                </includes>
            </resource>
        </resources>
        <testResources>
            <testResource>
                <directory>src/test/resources</directory>
                <filtering>true</filtering>
                <includes>
                    <include>**/**</include>
                </includes>
            </testResource>
        </testResources>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-surefire-plugin</artifactId>
                <version>2.12.4</version>
                <configuration>
                    <skipTests>true</skipTests>
                </configuration>
            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <configuration>
                    <source>8</source>
                    <target>8</target>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
```

```java
package com.memcpy0.chatgpt;  
  
import org.slf4j.Logger;  
import org.slf4j.LoggerFactory;  
import org.springframework.boot.SpringApplication;  
import org.springframework.boot.autoconfigure.SpringBootApplication;  
import org.springframework.http.HttpStatus;  
import org.springframework.http.ResponseEntity;  
import org.springframework.web.bind.annotation.GetMapping;  
import org.springframework.web.bind.annotation.ResponseBody;  
import org.springframework.web.bind.annotation.RestController;  
  
/**  
 * @author memcpy0  
 * @description 启动入口  
 * 沉淀、分享、成长，让自己和他人都能有所收获！  
 */  
@SpringBootApplication  
@RestController  
public class Application {  
	private static Logger logger = LoggerFactory.getLogger(Application.class);  
	public static void main(String[] args) {  
	    logger.info("启动成功");  
	    SpringApplication.run(Application.class);  
	}
  
    @GetMapping("/verify")  
    public ResponseEntity<String> verify(String token) {  
        logger.info("验证 token: {}", token);  
        if ("success".equals(token)) {  
            return ResponseEntity.status(HttpStatus.OK).build();  
        } else {  
            return ResponseEntity.status(HttpStatus.BAD_REQUEST).build();  
        }  
    }  
      
    @GetMapping("/success")  
    public String success() {  
        return "test success by memcpy0";  
    }  
}
```

```java
package com.memcpy0.chatgpt;  
  
/**  
 * @author memcpy0  
 * @description 单元测试  
 * 沉淀、分享、成长，让自己和他人都能有所收获！  
 */  
public class ApiTest {  
}
```
application.yml：
```yml
server:
  port: 8080
```
目前的工程还非常简单，只是在 Application 中提供了2个接口，一个认证，一个成功。在 `verify` 接口中，如果 `token == success` 就返回 `HttpStatus.OK == 200` 的码，否则返回 `HttpStatus.BAD_REQUEST == 400`  错误码。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305201431812.png)
## 2. Nginx 配置
这里我们需要改动 Nginx `conf.d/default.conf` 文件，添加 `auth` 认证模块；
```bash
server {
    listen       80;
    listen  [::]:80;
    server_name  localhost;
    # 首页
    index index.html;

    #access_log  /var/log/nginx/host.access.log  main;

    location / {
        root   /usr/share/nginx/html;
        index  index.html index.htm;
    }
    location /api/ {
        auth_request /auth;
        # 鉴权通过后的处理方式
        proxy_pass http://112.124.203.218:8080/success;
    }
    # =是绝对匹配,没有=就是前缀匹配
    location = /auth {
        # 发送子请求到HTTP服务，验证客户端的凭证，返回响应码
        internal;
        # 设置参数
        set $query '';
        if ($request_uri ~* [^\?]+\?(.*)$) {  
            set $query $1;  
        }
        # 带着参数$query请求verify方法，如果验证成功，返回200 OK
        proxy_pass http://112.124.203.218:8080/verify?$query;
        # 发送原始请求
        proxy_pass_request_body off;
        # 清空Content-Type
        proxy_set_header Content-Type "";
    }
    #error_page  404              /404.html;
    error_page 404 /404.html;
    location = /40x.html {
   
    }
    # redirect server error pages to the static page /50x.html
    #
    error_page   500 502 503 504  /50x.html;
    location = /50x.html {
        root   /usr/share/nginx/html;
    }

    # proxy the PHP scripts to Apache listening on 127.0.0.1:80
    #
    #location ~ \.php$ {
    #    proxy_pass   http://127.0.0.1;
    #}

    # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
    #
    #location ~ \.php$ {
    #    root           html;
    #    fastcgi_pass   127.0.0.1:9000;
    #    fastcgi_index  index.php;
    #    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;
    #    include        fastcgi_params;
    #}

    # deny access to .htaccess files, if Apache's document root
    # concurs with nginx's one
    #
    #location ~ /\.ht {
    #    deny  all;
    #}
}
```
> 修改配置后，一开始访问不到Nginx，排除了端口是否开放后，我用 `docker ps` 发现Nginx一直处于 `Restarting` 状态，再用 `docker logs 5eb15239a1c0` 查看其日志，一直报如下错误，乱七八糟改了下就成功了：
> ```bash
> /docker-entrypoint.sh: /docker-entrypoint.d/ is not empty, will attempt to perform configuration
/docker-entrypoint.sh: Looking for shell scripts in /docker-entrypoint.d/
/docker-entrypoint.sh: Launching /docker-entrypoint.d/10-listen-on-ipv6-by-default.sh
10-listen-on-ipv6-by-default.sh: info: IPv6 listen already enabled
/docker-entrypoint.sh: Launching /docker-entrypoint.d/20-envsubst-on-templates.sh
/docker-entrypoint.sh: Launching /docker-entrypoint.d/30-tune-worker-processes.sh
/docker-entrypoint.sh: Configuration complete; ready for start up
nginx: [emerg] unknown "request_uri ~* "[^\?]+\?(.*)$"" variable
> ```

用户访问 http://localhost/api/参数 ，目标是到达成功页面 `http://192.168.1.101:8080/success`  但这里添加了 `auth` 模块，所以会先进入 auth 的处理。
- `= auth` 是绝对匹配，没有 `=` 号就是前缀匹配。
- 在 `auth` 中把**请求 api 的参数**获取到，再访问到验证地址 `http://192.168.1.101:8080/verify?$query` ，如果接口返回一个200的码就通过（并发送原始请求？？），其他码就失败。

这里有个细节就是：
- 如果是本地使用 docker 启动的 NGINX，default.conf配置文件中的 server_name、proxy_pass 不能写localhost 或者 127.0.0.1 ，而是需要写本地的实际 IP 地址。 写 localhost或者127.0.1 的话，访问 `http://localhost/api/?token=success` 会出现 404 。 127.0.0.1 这个地址是nginx容器中网关的地址，本地是 ping 不通的。

我是在云端使用Docker启动的Nginx，就把本地的chagpt-api工程打包为JAR，发到服务器端运行。现在直接访问 `http://112.124.203.218:8080/verify?token=success` 和 `http://112.124.203.218:8080/success` 都是可行的：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305201503662.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305201614045.png)

然后再访问 `http://112.124.203.218/api/?token=success` ，注意，这里的端口是80，由Nginx监听。而程序运行的端口是8080，请求由Nginx转发给程序。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305201618411.png)

---
# 四、功能验证
启动 chatgpt-api SpringBoot 服务<img src="https://article-images.zsxq.com/Fqzlb2YRT1i86iPrAhfv1zN1YaUU" style="display: block; margin: auto;" align="center" width="756">
## 1. 重启 Nginx 服务
命令：`docker restart Nginx`  或者在 portainer 页面重启 <a href="http://localhost:9000/#!/2/docker/containers" target="_blank">http://localhost:9000/#!/2/docker/containers</a><img src="https://article-images.zsxq.com/FvFAUx_-f3c4l7nAkuuY1o3F3DqX">
## 2. 访问测试
正确验证：`http://localhost/api/?token=success` 
<img src="https://article-images.zsxq.com/FhR5-LR61TjsBMgrFFox1q1yRt4L" style="display: block; margin: auto;" align="center">

错误验证：`http://localhost/api/?token=123` <img src="https://article-images.zsxq.com/FtVcjGX3P1ZrpMQt7ntvHJbR7TaH" style="display: block; margin: auto;" align="center">

# 五、读者作业
简单作业：完成本章节功能，**配置 Nginx auth，服务开发，验证调用**。
难度作业：域名、443、ssl、openai、aws﻿