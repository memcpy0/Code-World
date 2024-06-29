在前面领略了 Wireshark 的强大功能，不过这些都是由工具开发者设计好的功能，如果遇到了一些特殊情况，而这些情况又是开发者当时所没有想到的，这又该如何解决呢？其实很多网络安全方面的工具都考虑到了这个问题，例如Nmap、Metasploit 和 Wireshark 都给出了解决的方案，这也正是这些工具日益受到使用者欢迎的原因。它们的解决方案就是==在工具中提供编程的接口，只要使用者掌握一定的编程能力，就可以打造出符合自己需要的功能模块来==。这样一来，这些工具就不再只是功能模块的简单拼凑，而是变成了一个拥有无限潜力的开发工具。怎么样？是不是觉得很有意思？按照如下几个部分来介绍 Wireshark 的扩展开发：
- Wireshark 编程开发的基础；
- 使用 Lua 开发简单扩展功能；
- 在 Wireshark 开发**新的协议解析器**；
- **对新协议的测试**；
- 编写**恶意攻击数据包**检测模块。

Wireshark 的功能已经十分强大了，但鉴于在这个世界上新的协议不断产生，我们需要扩展它的功能。Wireshark 本身就是使用 C 语言开发出来的，所以它支持 C 语言编写扩展功能。虽然 C 语言在我国普通高校普及的范围很广泛，但是这门语言的难度也十分大。很多人在花费了很长时间来学习 C 语言之后，也无法用它写出一个实用的程序来。

相信 Wireshark 的开发团队也考虑到了这个问题，所以 Wireshark 还提供了对另一门语言 Lua 的支持。Lua 是一种轻量、小巧的脚本语言，学习起来将会十分简单。
## 1.1 Wireshark 中对 Lua 的支持
**Wireshark 的大部分版本中都内嵌了 Lua 语言的解释器**，在开始为 Wireshark 编写扩展功能之前需要检查当前的版本是否支持 Lua。检查的方法很简单，可以在启动 Wireshark 之后，依次单击菜单栏上的“帮助”→“关于 Wireshark”，打开“关于 Wireshark”对话框。

如图 1 所示，在这个对话框中显示了当前版本所支持的所有工具，如果在这个对话框中显示了“with Lua 5.x”的话，表示已经内嵌了 Lua 的解释环境。
图 1 Wireshark 中的 Lua 版本
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172304729.png)
如果在你的 Wireshark 版本中没有提供对 Lua 的支持，就需要在操作系统中安装它的支持环境。

同样 **TShark 中可以支持使用 Lua 语言编写扩展**，但也需要检查它是否内置了 Lua 的解释器，这一点可以通过在命令行中输入 TShark -v 来查看。如图 2 所示，看到这个 Tshark 和 Wireshark 一样都支持 Lua5.2.4。
图 2 TShark -v 来查看 Lua 版本
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172305695.png)
## 1.2 Wireshark 中 Lua 的初始化
init.lua 是 Wireshark 中启动的第一个 Lua 脚本，它位于 Wireshark 的 “global
configuration”目录中。这个目录会因为配置的不同而不同，在我使用的 Windows 7 操作系统中，这个文件位于 `C:\Program Files\Wireshark` 中。==在 init.lua 中可以开启和关闭对 Lua 的支持，同时也为 Wireshark 提供了安全检查==。

**当 init.lua 启动了之后，这个脚本中可以使用 dofile 函数来指定要执行的其他 Lua 脚本**。这个过程是在数据包捕获之前就完成的。
## 1.2 使用 Lua 开发简单扩展功能
在 Wireshark 中内置了一个简单的 Lua 编程环境，只需菜单栏上单击“工具”→“Lua”→“Evaluate”，帮助我们轻松实现使用 Lua 编程和调试。

在图 3 所示的“Evaluate Lua”对话框中输入 Lua 编写的代码，编写的代码会自动载入 Wireshark 所提供的库文件。下面写一个简单的 Lua 程序，它的作用是弹出一个显示“hello world！”的窗口。输入的程序代码如下：
```java
local newwindow = TextWindow.new("hello world!")
```
图 3 Wireshark 中的 Lua 编程环境
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172307969.png)
然后单击“Evaluate”，就会弹出一个标题栏为“hello world！”窗口，如图 4 所示。
图 4 弹出一个标题栏为“hello world！”窗口
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172307006.png)
除此之外，在 Wireshark 中还可以**使用 Lua 编写两种类型的插件，协议解析器用于解析报文，监听器用来收集解析后的信息**。
## 1.3 用 Wireshark 开发新的协议解析器
在学习任何一种语言时，除了需要了解该语言的语法之外，还需要做的就是了解系统所提供的 API。有了这些 API 的帮助，编写程序时可以节省大量的时间和精力。

Wireshark 中就提供了很多高效的函数，Wireshark 主页提供了这些函数的详细说明。下面利用这些 API 来编写一个协议解析器。
### 1.3.1 新协议的注册
先来查看一下 Wireshark 在解析协议时的原理。==一个协议在进行解析时需要考虑两个方面，第一是这个协议所使用的端口，第二是这个协议中数据的格式==。首先看一下 Wireshark 中所支持的全部协议信息，单击菜单栏上的“视图”→“内部”→“解析器表”，就可以打开一个 Wireshark 的解析器表，如图 5 所示。
图 5 Wireshark 中的解析器表
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172308668.png)
这个表中一共分成了 4 个子表，我们选中其中的第 3 个子表“Interger Tables”。在其中可以看到 **Wireshark 可解析的应用层协议**对应的端口，如果这个应用层协议使用 TCP 协议进行传输的话，可以在“Interger Tables”中找到“TCP port”选项（见图 6），会列出这些应用层协议所对应的端口。
图 6 应用层协议所对应的端口列表
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172309871.png)
==所有可以解析的协议都需要先在 Wireshark 中注册，成功注册的协议会显示在“解析器表”中==，在这个注册过程中我们需要完成如下的 3 项工作。
（1）添加一个协议。
（2）添加这个协议的解析器。
（3）将这个协议注册到 Wireshark 中。
这里面需要用到几个 Wireshark 提供的函数，首先查看用于实现添加协议的类，使用这个类实例化一个对象的方法为：
```java
proto:__call(name, desc)
```
这里面包含两个参数，name 表示新协议的名称，desc 表示新协议的描述。使用如下命令来初始化一个新的协议：
```java
local foo = Proto(“foo”, "Foo Protocol")
```
接下来添加这个协议的解析器，这需要使用到 proto.dissector()函数，这个函数的形式为：
```java
dissector:call(tvb, pinfo, tree)
```
需要用到 3 个参数：tvb、pinfo、tree。其中 tvb 表示要处理的报文缓存，pinfo 表示报文，tree 表示报文解析树。这里我们暂时先不添加这个函数的内容：
```js
function foo.dissector (tvb, pinfo, tree)
end
```
现在我们将这个协议添加到“Interger Tables”的 tcp.port 中。`DissectorTable.get(tablename)` 函数会将协议添加到 tablename 中，这个参数 tablename 就是表的名字，**这里可以是 tcp.port 或者是 udp.port**。

`dissectortable:add(pattern, dissector)`这里面的 pattern 可以是整数，整数区间或者字符串，这个是由前面所选的 tablename 所决定的。dissector 可以是一个协议或者解析器。
```js
DissectorTable.get("tcp.port"):add(10001, foo)
```
完整的代码很简单为：
```js
local foo = Proto("foo", "Foo Protocol")
function foo.dissector (tvb, pinfo,tree)
end
DissectorTable.get("tcp.port"):add(10001, foo)
```
好了到此为止，已经创建了一个 Lua 的框架文件，这里面包含了解析器的创建，解析器函数，解析器注册的功能。==将这个文件以 foo.lua 为名保存到 plugin 目录中。Wireshark 在启动时，就会自动加载这个文件==。

如果放置在其他目录下，那么 Wireshark 将不会自动加载这个插件，要修改Wireshark 根目录下面的 init.lua 文件，在文件尾部追加下面一行代码，这里我们的 Lua 解析文件名为 foo.lua，保存在 C 盘下，添加的代码为：
```js
dofile("c:/foo.lua")
```
重新启动 Wireshark 之后，单击菜单栏上的“视图”→“内部”→“解析器表”，就可
以在解析器表中“Interger Tables”中找到“TCP port”选项，可以看到 10001 号端口对应着协议 foo（见图 7）。
图 7 FOO 协议对应的 10001 端口
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172314127.png)
### 1.3.2 解析器的编写
上一节已经提到了**解析器的主体部分就是 dissector 函数**，它将决定一个数据包在进行解析时，显示在 Wireshark 中信息面板中的树状结构（见图 8）。
图 8 Wireshark 中信息面板中的树状结构
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172315856.png)
在开始编写这个函数之前，先回忆一下网络协议的一些基本知识。首先协议规定的**数据包的长度不应该是任意的**，这是因为每个字节的长度是 8 位，通常的网络协议是以 4 字节（32 位）或者 8 字节（64 位）为单位的。另外==当数据通过网络传播时，采用的大端模式==。表 1 给出了一个极为简单的协议（这个协议是不存在的，为了方便学习，协议内容得到了最大的简化）。
表 1
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172316802.png)
假设这个协议使用 2 字节表示传输的序号 Trans ID，使用另外 2 字节表示传输的消息类型 Msg Type，使用 4 字节来存储 Msg data。

好了现在我们虚拟了一个协议（但它和真实的协议是一样的），下面使用 Lua 语言来构造这个协议，==这里需要使用 ProtoField 对象，它表示协议字段，一般用于解析字段后往解析树上添加节点==。添加的函数格式为：
```js
ProtoField.{type} (abbr, [name], [desc],[base], [valuestring], [mask])
```
可以使用的 type 类型有：uint8、uint16、uint24、uint32、uint64、framenum。
例如我们虚构的协议中 Trans ID 的长度为 2 字节（16 位），这里就选用 unit16 来定义它，只赋值前两个参数：
```js
Trans_ID=ProtoField.uint16("foo.ID","ID")
```
同样的方法来定义 Msg Type 和 Msg data。
```js
Msg_Type=ProtoField.uint16("foo.Type","Type")
Msg_Data=ProtoField.uint32("foo.Data","Data")
```
接下来可以将这几个字段合并成一个协议。
```js
foo.fields={Trans_ID,Msg_Type,Msg_Data}
```
下面利用这个协议来定义协议树的结构：
```js
function foo.dissector(tvb,pinfo,tree)
```
把 Wireshark 报文列表上的“Protocol”列的文本置为 foo 协议名称“Foo”：`pinfo.cols.protocol = foo.name`
- 往协议解析树上添加一个新节点 subtree：
    `local subtree=tree:add(foo,tvb(0))`
- 将 Trans_ID 的信息加入到协议解析树：
    `subtree:add(Trans_ID,tvb(0, 2))`
- 将 Msg_Type 的信息加入到协议解析树：
    `subtree:add(Msg_Type,tvb(2, 2))`
- 将 Msg_data 的信息加入到协议解析树：
    `subtree:add(Msg_Data,tvb(4, 4))`

完整的代码如下所示：
```js
local foo=Proto("foo","Foo Protocol")
Trans_ID=ProtoField.uint16("foo.ID","ID")
Msg_Type=ProtoField.uint16("foo.Type","Type")
Msg_Data=ProtoField.uint32("foo.Data","Data")
foo.fields={Trans_ID,Msg_Type,Msg_Data}

function foo.dissector(tvb,pinfo,tree)
    pinfo.cols.protocol="foo"
    local subtree=tree:add(foo,tvb(0))
    subtree:add(Trans_ID,tvb(0, 2))
    subtree:add(Msg_Type,tvb(2, 2))
    subtree:add(Msg_Data,tvb(4, 4))
end
DissectorTable.get("tcp.port"):add(10001,foo)
```
将这段代码保存到 Wireshark 的 plugins 目录中，重新启动 Wireshark 就可以加载这个插件。
## 1.4 测试新协议
接下来，使用任何一种发包工具产生一个 TCP 协议数据包，其中的数据部分为 64 位的 0，目标端口为 10001。这里==使用了 xcap 工具（见图 9）来发送这个数据包，可以到 xcap 网站下载这个工具，网站提供了这个工具和它的使用方法==。
图 9 xcap 工具
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172322585.png)
使用 xcap 发包工具的方法很简单，大体步骤就是**创建报文、添加报文内容和发送报文**。我们以构造一个上例中的 foo 数据包为例。
（1）创建报文组，可点击菜单“报文组/创建报文组”，输入报文组名称，再单击“确定”按钮（见图 10）。
图 10 在 xcap 中创建报文组
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172323526.png)
（2）在左侧窗口中选择已创建的报文组，右侧窗口显示该报文组的界面（见图 11）。
图 11 在 xcap 中选中报文组
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172323903.png)
（3）在右侧界面中点击“+”按钮，创建一个报文，并输入名称（见图 12）。
图 12 在 xcap 中创建一个报文
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172323286.png)
（4）**双击已创建的报文，出现报文配置向导**，首页为以太网头部（见图 13）。这里的内容与我们的实验无关，无需进行改变。
图 13 修改报文的以太网头部
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172324947.png)
（5）点击下一步，出现 Ipv4 头部页面，==这里要将 Protocol 选中为 0x06（TCP）（见图14），因为我们的这个 foo 协议是以 TCP 作为下层协议的==，然后单击“下一步”。
图 14 修改报文的 Ipv4 头部
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172325845.png)

（6）此时出现 TCP 页面，==目的端口填写“10001”（见图 15），这是关键的一步，因为只有**来自 10001 端口的数据包**才会被当做 foo 协议来解析==。
图 15 修改报文的 TCP 头部
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172326108.png)
（7）如图 16 所示，在 Data 部分添加 64 位的 0。
图 16 修改报文的 Data 部分
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172326045.png)
（8）单击下方的“保存和关闭”按钮，报文创建成功。
（9）获取接口列表。点击主界面工具栏中的“刷新列表”按钮（或对应菜单“接口”→“刷新接口”），**所有接口会列在左侧窗口中**（见图 17），选中要使用的接口。单击工具栏中的“启动接口”按钮，之后接口启动成功。
图 17 在 xcap 中启动接口
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172327822.png)

（10）如图 18 所示，用鼠标选中要发送的报文（如果要发送多个，可使用 Ctrl 键选中多个），然后单击“发送”按钮，报文即被发送。
图 18 在 xcap 中发送报文
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172327913.png)
另外，==在发送的同时使用 Wireshark 捕获数据包，可以看到 Wireshark 已经可以正确地解析这个数据包的格式了==（见图 19）。
图 19 Wireshark 中解析的数据包格式
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172327888.png)

## 1.5 编写恶意攻击数据包检测模块
现在已经掌握了在 Wireshark 编程的基础知识。那么接下来就开始编写一个==可以找出攻击者发送数据包的插件==。这里就**以 SQL 注入攻击**为例，这是黑客对数据库进行攻击的一种常用手段。现在采用 B/S 模式的应用程序越来越多，但是开发这些应用的程序员水平参差不齐。很多人在编写程序时没有考虑到对用户输入的内容进行合法性判断，从而可能导致数据库内容的泄露，这种攻击方式就是 SQL 注入。

下面以一个实例来简单了解一下这种攻击方式。==许多网页链接有类似的结构
http://xxx.com/xxx.php?id=1 基于此种形式的注入，一般被叫作数字型注入点==，缘由是其注入点 id 类型为数字，在大多数的网页中，诸如查看用户个人信息、查看文章等，大都会使用这种形式的结构传递 id 等信息，交给后端查询出数据库中对应的信息，再将信息返回给前台。

而某个程序员在编写应用程序登录验证的 SQL 查询代码时，写成了以下形式：
```sql
select * from 表名 where id=1
```
如果攻击者在 id =1 后面添加“and 1=1”就可以构造出类似与如下的 SQL 注入语句从而完成对数据库的爆破：
```sql
select * from 表名 where id=1 and 1=1
```
这里我们可以知道，==通常一个攻击者在对应用程序进行 SQL 注入时会先添加“and 1=1”==，所以先编写一个插件来检测在指定数据包的内容是否存在这个字段。这个插件应该包含以下 3 个部分：
- URL 解码部分；
- 数据包内容检测部分；
- 攻击数据包显示部分。

> # SQL注入篇——sqli-labs最详细1-40闯关指南

首先来编写 URL 解码部分，==解码的原因是 HTTP 协议来用发送 URL 中的参数时会进行编码==。这种编码**将一些特殊字符（例如‘=’、‘&’、‘+’）转换为“%XX”形式的编码**，其中 XX 是**字符的十六进制表示**，然后**将空白转换成 `'+'`** 。比如，将字符串“a+b = c”编码为“a%2Bb+%3D+c”。

所以在解码时需要将其重新转换回来。这里需要使用函数 `string.gsub (s, pattern, repl [,m])` ，这个函数会返回一个替换后的副本，原串中所有的符合参数 `pattern` 的子串都将被参数 `repl` 所指定的字符串所替换。如果指定了参数 m，那么只替换查找过程的前 m 个匹配的子串，参数 repl 可以是一个字符串、表或者函数，并且函数可以将匹配的次数作为函数的第二个参数返回。这个 URL 解码的程序如下所示：
```js
function unescape (s)
    s = string.gsub (s, "+", " ")
    s= string.gsub (s, "%%(%x%x)",function(h) return string.char(tonumber(h,16)) end)
    s= string.gsub (s, "\r\n", "\n")
    return s
end
```
然后**编写一个检查数据包中是否包含特定字段的程序**，首先需要将数据包使用
`unescape()` 函数解码，然后再使用 string.match()函数进行查找。这个检查的程序如下所示：
```js
local function check(packet)
    local result = unescape (tostring(packet))
    result = string.match(result, " and 1=1")
    if result ~= nil then
        return true
    else
        return false
    end
end
```
最后将检测到的结果显示在数据包层次表中，这部分的内容如下所示：
```js
local function SQLInject_postdissector()
local proto = Proto('suspicious', 'suspicious dissector')
    exp_susp = ProtoExpert.new('suspicious.expert','Potential SQLInject',expert.group.SECURITY, expert.severity.WARN)
    proto.experts = {exp_susp}
    function proto.dissector(buffer, pinfo, tree)
        local range = buffer:range()
        if check(range:string()) then
            local stree = tree:add(proto, 'Suspicious')
            stree:add_proto_expert_info(exp_susp)
        end
    end
    register_postdissector(proto)
end
SQLInject_postdissector ()
```
同样，==如果这个编写好的插件没有放置在 plugin 目录中的话，那么 Wireshark 将不会自动加载这个插件==。我们需要修改 Wireshark 根目录下面的 init.lua 文件，在文件尾部追加一行代码，假设这里我们的 Lua 解析文件名为SQLInject_postdissector.lua，保存在 C 盘下，那么添加的代码为：
```js
dofile("c:/SQLInject_postdissector.lua")
```
现在使用这个插件来分析一个数据包文件中是否含有 SQL 注入攻击的数据包，通过 Wireshark 对 sql.pcap 文件进行分析，图 20 给出了一个已找到了具有 SQL 注入攻击的数据包。
图 20 使用这个插件分析 SQL 注入攻击的数据包
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172338901.png)
## 1.6 小结
介绍了如何在 Wireshark 中编写插件，这个功能在实际应用中相当有用，==尤其软件开发人员在设计新的应用时。之前很多的网络分析工具只能解析内置的协议，因此一旦出现新的协议时就毫无用武之地==。而 Wireshark 则弥补了这个缺陷，它提供的扩展功能已经成为当前流行工具的共同特点，例如 Nmap 和 Metasploit 都提供了开发功能。

Wireshark 中允许用户使用 C 语言和 Lua 语言进行新功能的开发，虽然大多数人可能对 Lua 并不熟悉，但还是强烈推荐这个语言，它十分简单易用。