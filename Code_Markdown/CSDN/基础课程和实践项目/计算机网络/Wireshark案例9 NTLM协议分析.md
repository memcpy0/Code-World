读者做项目时遇到了麻烦，就抓了个网络包来找我分析。我粗略一看，身份验证协议用的竟然是NTLM，**便建议他改用Kerberos**。没想到对方说NTLM目前在中国还是用得很多的，不想改。我将信将疑，咨询了几位在一线做实施的工程师才确认，据说连某大银行内部的文件服务器都完全靠NTLM做身份验证。既然如此，我就来写一篇NTLM的工作原理吧，对中国读者应该会有好处。

NTLM的全称是NT LAN Manager，是WindowsNT时代就出现的身份验证协议。**大多数人都用到过NTLM，却没有意识到它的存在**。我们可以做个简单的实验来演示。
1．如图1所示，我在电脑上输入了一个网络共享的路径，再按一下回车。
图1
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307071741232.png)

2．图2的对话框跳出来了，要求身份验证，即输入用户名和密码。
图2
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307071741860.png)
3．完成了图2这一步，网络共享就打开了，我们就可以访问里面的文件。

在此过程中，实际上就用到了NTLM来验证身份。我们来看看在Wireshark中是怎样体现的。
1．**客户端向服务器发送了一个NTLM协商请求，然后服务器立即回复一个随机字符串作为Challenge**（见图3）。这个Challenge有什么用呢？后面会详细讲到。注意服务器的回复虽然是“Error:STATUS_MORE_PROCESSING_REQUIRED”，看上去好像是出错了，但实际上这个所谓的Error是正常流程的一部分。
图3
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307071743189.png)
2．客户端收到Challenge之后，向服务器回复了图2中输入的那个用户名VNX\Administrator，以及两个Response值（见图4）。这两个Response是哪来的呢？它们都是用hash过的用户密码对Challenge所进行的加密，两种不同的加密方式产生了两个不同的Response。加密过程就不细说了，绝大多数读者并不需要知道（其实是因为作者自己也了解不深）。
图4
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307071744090.png)

3．服务器收到了之后，是不知道该怎样验证这些Response的真假的。因此它就把Challenge和两个Response都转发给域控（DomainController），让域控去帮忙验证真假。在图5中可以看到，转发给域控的Challenge和Response和图3、图4里的是一样的。
图5
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307071745699.png)
4．**域控收到之后，也用hash过的用户密码对该Challenge进行加密。如果加密结果和这两个Response一致，就说明密码正确，身份验证通过**。在响应时，**域控还会把该用户所属的群组信息告知服务器**，见图6底部所示。
图6
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307071745510.png)
5．于是服务器就可以告诉客户端，“你的身份验证通过了。”见图7的289号包。如果失败的话，你看到的就不是这么简单的“SessionSetupResponse”了，而是某个Error。
图7
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307071746885.png)
这个过程总结下来就如图8所示，比起Kerberos还是比较简单的。
图8
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307071746303.png)
明白了原理，我们就可以理解NTLM的很多特征了。
1．由于从包里能看到Challenge和Response，算法也是固定的，所以密码存在被破解的可能。因此微软并不推荐NTLM，从官网就可以看到“Therefore, applications are g enerally advised not to use NTLM（因此，一般不建议应用程序使用NTLM）”。

2．客户端每访问一次服务器，就得让域控帮忙验证一次身份，增加了域控的负担。**如果改用Kerberos就能使用缓存的ticket，减少访问域控的次数**。

3．有些场合也能体现出先进性，比如当一个用户不属于某群组，所以访问不了资源时。解决方式是**在域控上为该用户账号添加群组**，这时如果用Kerberos就得重新登录来获得新的群组信息，用NTLM则不用。

当我们遇到NTLM问题的时候，用Wireshark来排查是最合适不过的，**客户端、服务器和域控的问题都能发现**。比如客户端有时会在图4的包中使用一个空的用户名，或者其它出人意料的账号来验证身份，这种现象在Wireshark中一目了然。域控导致的问题可能我们自己解决不了，需要联系微软技术支持，**但是在此之前用Wireshark定位会快很多**。

比如不久前我遇到过一个身份验证失败的案例，在Wireshark中看到的域控报错如图9所示，我们据此就能把问题定位到域控上。微软的工程师也是根据这个报错修改了GroupPolicy来解决的。这种问题如果没有用到Wireshark，可能连判断是哪一方导致的都不容易。
图9
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307071748636.png)
