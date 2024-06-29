# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-11 22:40
program     : 学习HTML与Python结合
"""#

"""
<!DOCTYPE html>         文档类型
<html lang="zh_CN">     语言

head  -->               不是给人看的
    <meta charset="UTF-8">  字符集
    <title></title>         网页标题
    
    <meta></meta>           定义网页原信息
        * charset 
        * name="keywords" content="……"
          name="description" content="……"
        * content 
        * http-equiv
    <style></style>         CSS定义内部样式表
    <link>                  CSS引入外部样式表
        * rel="stylesheet" type="text/css" href="…….css"
    <scrpit></script>       定义内部JS代码或JS文件
        * type="text/javascript" src="…….js"

body  -->               给用户看的内容
属性
    * id     制定ID
    * class  css属性类
    * style  定义样式
    * width height 很少使用的标签的默认选项
注释
    <!-- -->
锚点
    页面内的跳转 href="#id" 
转义符
    如<p>a>b</p>等可能需要转义
    可以用b&lt;a  b<a
         b&gt;a  b>a
    空格 转义  &nbsp;
    元        &yen;
    版权      &copy;
    注册      &reg;
    &        &amp;
    
换行空格 HTML作为一门标记语言会把HTML文件中的换行和空格解析成一个空格
    The spacing between code in an HTML file doesn't affect the positioning of
    elements in the browser. If you are interested in modifying the spacing 
    in the browser, you can use HTML's line break element: <br>.
    * 使用特定的标签 <br> 或 <br/>
    
    <a href="http://……" target="_blank"></a> 
        * target默认为self; _blank指的是新建页面跳转
    
<hr>                    一条横线
<b>加粗</b>
<i>斜体</i>
<u>下划线</u>
<s>删除</s>
<em></em>         will generally render as italic emphasis.
<strong></strong> will generally render as bold emphasis.
 
图片   
    <img src="……" alt="图片未加载时显示" title="鼠标移上去的提示">
        * 通常不会用width和height来设置属性 而是用css
    * The alt attribute also serves the following purposes:
    If an image fails to load on a web page, a user can mouse over the area originally intended for the image and read 
a brief description of the image. This is made possible by the description you provide in the alt attribute.
    Visually impaired users often browse the web with the aid of screen reading software. When you include the alt
attribute, the screen reading software can read the image's description out loud to the visually impaired user.
    The alt attribute also plays a role in Search Engine Optimization (SEO), because search engines cannot "see" the 
images on websites as they crawl the internet. Having descriptive alt attributes can improve the ranking of your site.
    
    <h1></h1> --> <h6></h6> 多级标题
    
    <p></P>               定义一个段

专门为定义CSS样式而生:
    <div></div>           用来定义一个块级标签 通过CSS渲染样式
    <span></span>         用来定义一个行内标签(内联) 通过CSS渲染样式
    
行内标签 在一行显示 行内元素则不需另起一行 行内标签不能包含块级标签 它只能包含其他内联元素
    1. <a>
    2. <span>
    3. <img>
    4. <b/i/u/s>
    5. th/td
    
块级标签 在另一行显示 另起一行开始渲染的元素 如div可以任意嵌套块级标签和行内标签
    1. <h1> -> <h6> 
    2. <p>
    3. <div>
    4. <hr>
    5. li
    6. tr
    7. dd/dt
p标签不能套p标签和div标签
 
列表   
    ul 无序列表 套li标签 
       type = disc 实心圆点
              None 
              circle 空心圆圈
              square 实心方块
    ol 有序列表 套li标签
       type = A(大写字母)/a(小写字母)/1(数字列表)
       start = 3(数字 1-)
    dl 标题列表 套dt(标题) dd(内容)标签
            
表格 
table border="1" 边框
      cellpadding="10" 内边距
      cellspacing="10" 外边距
      rowspan    单元格竖跨多少行
      colspan    单元格横跨多少列
    thead  表头 加粗处理 
        tr 
           <th>
    tbody  表体
        tr
           <td>
        tr
           <td>
"""

"""
获取用户输入的标签 form input 
    1. form标签必须把获得用户输入的标签包起来
    2. form标签必须要有action属性和method方法
            * action 控制往哪里发送数据
            * method get/post HTTP请求的两种方法
            * enctype --> 当form表单中哟文件类型的数据需要上传时 
                          multipart/form-data
    3. form标签必须要有name属性
       所有的form输入会用name:value拼成一个字典
    <form action="">
        <input type="text">
            * type= text;
                    button;  # 普通按钮 通常是用JS给它绑定事件
                    submit;  # 提交按钮 默认将form表单的数据提交
                    reset;   # 重置按钮 将form表单的数据清空
                    checkbox; radio; # 通过value属性设置value；通过checked设置默认选中
                    color;
                    date;
                    email; file; hidden;
            * placeholder 占位符 背景文本
            * name value --> name=value&name=value 可能是默认值
                    指定form表单提交数据的key，用户填写的是value
    </form>

        <select name="city" multiple>
            * multiple=multiple 简写为 multiple
            <option value="010">北京</option>
                - 设置value属性
            * 通过selected设置默认选中
            * 分组
                optgroup
        </select>
        
        <textarea>
            * cols 列数
            * rows 行数
            * name属性
        <label> # 通常与输入框搭配使用

"""

import socket
sk = socket.socket()
sk.bind(("127.0.0.1", 8080))
sk.listen(5)

connection, address = sk.accept()
while True:
    data = connection.recv(4096)
    print(data)
    connection.send(b'HTTP/1.1 200 OK\r\n\r\n')
    # with open('LocalHtml.html', 'rb') as f:
    #     connection.send(f.read())
connection.close()
sk.close()
