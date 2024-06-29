# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-13 10:56
program     : 学习CSS选择器和CSS属性
"""#

# https://www.cnblogs.com/liwenzhou/p/7999532.html
# 小米商城
# iconfont 字体图标
# http://dummyimage.com/
"""
CSS 层叠样式表
    改变HTML标签的样式
    步骤:
        1. 用选择器找到
        2. 定向修改
CSS语法
    1. CSS选择器
        1. 标签(元素)选择器 
            tag {}
        2. 类选择器
            .cl {}
        3. 通用选择器
            * {}
            标签中的class属性不要用数字开头
            class属性中可以用空格分隔 写多个class属性值
        4. ID选择器
            #id {}
        5. 组合选择器
            多种选择器可以组合
            1. 后代选择器
                tag1 tag2 是指从tag1的子子孙孙中选择tag2
                #id  tag2 是指从#id的标签的子子孙孙中选择tag2
            2. 毗邻选择器(紧挨着平行向下)
                tag1+tag2  是指选择紧挨tag1之后的tag2
            3. 弟弟选择器(隔着不隔着无所谓 平行向下同级)
                tag1-tag2  是指选择tag1向下平行的tag2标签, 可以是多个
            4. 儿子选择器
                tag1>tag2 是指从tag1的儿子中选择tag2
                #id>tag2  是指从#id的标签的儿子中选择tag2
        6. 属性选择器
            tag[attr] 选择所有的tag标签中有attr属性的标签
            tag[attr=value] 选择所有的tag标签中有attr属性且attr=value的标签
        7. 伪类选择器
            未访问的链接
            a:link {}
            已访问的链接
            a:visited {}
            
            鼠标移动到链接上
            a:hover  {}
            选定链接
            a:active {}
            
            获取焦点
            input:focus {} 
        8. 伪元素选择器
            给首字母设置特殊样式 
            p:first-letter {}
            在p标签中内容之前做一些工作
            p:before {}
            在p标签中内容之后做一些工作
            p:after {}
    2. 分组嵌套
        selector1, selector2, ... {} 多个选择器的标签都应用同样的样式
    3. css选择器的优先级
        1. 选择器相同 下面的优先级越高 覆盖
        2. 选择器不同 不同的选择器优先级不一样
        内联(1000)>ID选择器(100)>类选择器(10)>元素选择器(1)>继承的(0)
        选择器权重永不进位 可以相加 
        3. 无视权重的方法 在{}中加上!important; 最高位

CSS属性
    1. 宽度和高度
       只有给块级标签设置css样式的宽和高
       width height 才生效
       内联标签的宽度和高度有内容决定
    2. 字体属性
        1. 文字字体
            font-family: font1, font1, font2, ... font-family可以把多个字体名称作为一个回退系统来保存
        如果浏览器不支持 会向后选择知道有字体支持为止 
        2. 字体大小
            font-size: px;
        3. 字体字重(粗细) 
            font-weight
                normal 
                bold
                bolder
                lighter
                100~900
                inherit 
        4. 字体颜色 color: 可以是颜色名称;#16进制;rgb()值;rgba();
        5. 文字属性 
            1. 文字对齐 text-align
                left     左对齐，默认
                right    右对齐
                center   居中对齐
                justify  两端对齐
            2. 文字装饰 text-decoration
                none       默认，定义标准的文本  (常用的去掉a标签的下划线)
                underline  定义文本下的一条线
                overline   定义文本上的一条线
                line-through 定义穿过文本的一条线
                inherit      继承父标签的text-decoration的值
            3. 首行缩进 text-indent
                将段落的第一行缩进32像素
    3. 背景属性
        1. 背景颜色 background-color 
        2. 背景图片 background-image: url()
        3. 背景重复 background-repeat: repeat-x; 
                                      no-repeat; ...
        4. 背景位置 background-position: 200px 200px; -->(200, 200)
                                        right center left
                                        top center bottom
        5. 背景简写 background: color url() 重复 位置
        6. background-attachment:fixed 固定
    4. 边框属性
        1. 边框宽度 border-width: 3px;
        2. 边框样式 border-style: 
                    none;   无边框
                    solid;  实线边框
                    dashed;  矩形虚线边框
                    dotted;  点状虚线边框
        3. 边框颜色 border-color: pink;
        4. 边框圆角 border-radius 
        
    5. display属性
        用于控制HTML元素的显示效果。
        display:"none"	HTML文档中元素存在，但是在浏览器中不显示。一般用于配合JavaScript代码使用。
        display:"block"	默认占满整个页面宽度，如果设置了指定宽度，则会用margin填充剩下的部分。
        display:"inline"	按行内元素显示，此时再设置元素的width、height、margin-top、
    margin-bottom和float属性都不会有什么影响。
        display:"inline-block"	使元素同时具有行内元素和块级元素的特点
    
    6. 溢出属性 overflow
        1. visible 默认值 内容不会被修剪 会呈现在元素框之外
        2. hidden  内容会被修剪 并且其余内容是不可见的
        3. scroll  内容会被修剪 但是浏览器会显示滚动条以便查看其余的内容
        4. auto	   如果内容被修剪，则浏览器会显示滚动条以便查看其余的内容
        5. inherit 规定应该从父元素继承 overflow 属性的值
    
    7. 定位属性 position
        1. static
        默认值，无定位，不能当作绝对定位的参照物，并且设置标签对象的left、top等值是不起作用的
        2. relative（相对定位）
        相对定位是相对于该元素在文档流中的原始位置，即以自己原始位置为参照物。有趣的是，即使设定
        了元素的相对定位以及偏移值，元素还占有着原来的位置，即占据文档流空间。
        对象遵循正常文档流，但将依据top，right，bottom，left等属性在正常文档流中偏移位置。而其层叠通过z-index属性定义。
        注意：position：relative的一个主要用法：方便绝对定位元素找到参照物。
        
        3. absolute（绝对定位）
        设置为绝对定位的元素框从文档流完全删除，并相对于最近的已定位祖先元素定位，如果元素没有已定位的祖先元素，
        那么它的位置相对于最初的包含块（即body元素）。元素原先在正常文档流中所占的空间会关闭，就好像该元素原来不存在
        一样。元素定位后生成一个块级框，而不论原来它在正常流中生成何种类型的框。
        重点：如果父级设置了position属性，例如position:relative;，那么子元素就会以父级的左上角为原始点进行定位。
        这样能很好的解决自适应网站的标签偏离问题，即父级为自适应的，那我子元素就设置position:absolute;
        父元素设置position:relative;，然后Top、Right、Bottom、Left用百分比宽度表示。
        另外，对象脱离正常文档流，使用top，right，bottom，left等属性进行绝对定位。而其层叠通过z-index属性定义。

        4. fixed（固定）
        对象脱离正常文档流，使用top，right，bottom，left等属性以窗口为参考点进行定位，当出现滚动条时，对象不会随
        着滚动。而其层叠通过z-index属性定义。 
        注意点： 一个元素若设置了 position:absolute | fixed; 则该元素就不能设置float。
        这是一个常识性的知识点，因为这是两个不同的流，一个是浮动流，另一个是“定位流”。
        但是relative却可以。因为它原本所占的空间仍然占据文档流。
        在理论上，被设置为fixed的元素会被定位于浏览器窗口的一个指定坐标，不论窗口是否滚动，它都会固定在这个位置。
        5. 浮动 
            float: left right 
        浮动 固定 绝对 脱离文档标准流 就不是文档标准流下的标签 不符合行内和块标签的规则
        可以任意设置宽高
    
    8. z-index 仅能在定位元素上奏效
        设置对象的层叠顺序，数值大的会叠加到数值小的上面
        .cover {
            position: fixed;
            top: 0;
            bottom: 0;
            left: 0;
            right: 0;
            background-color: rgba(0, 0, 0, 0.3);
            z-index: 1;
        }
    9. opacity 0-1 0是完全透明 1是完全不透明
        
CSS注释机制
    /* 这是注释 */
CSS的几种引入方式
    1. 直接将样式表写在标签内部的style属性
    2. 在head标签中学style标签
    3. 将样式写在单独的CSS文件中，通过link标签的href导入(项目中多用这种)

CSS盒子模型
    内容
    padding 内填充
        padding-top padding-bottom padding-left padding-right
        padding: 20px 40px; # 上下20 左右30
        padding: 10px 20px 30px # 上 左右 下
        padding: 10px 20px 20px 20px # 上 右 下 左 顺时针 
    border 边框
    margin 外边距  * {margin: 0} 
"""
