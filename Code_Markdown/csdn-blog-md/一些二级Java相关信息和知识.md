这篇文章是考二级java的过程中的一些信息。不是对二级Java知识的总结，因为也没什么需要太总结的，刷题就是了。**二级也没什么含金量，就是以考促学**。考过后多少也会给我这样的跨专业学习者一点信心。至少C/C++是的(当然，更多的是我在平时不断使用和看书学习带来的)，而VB...以后捡起来重新学(为了Excel)，也会方便一点。

另外，这里面也会顺便总结官方给的一些说明。
@[toc]
### 0. 考试报名(先报后学)
> 重点：自2019年3月考试起，二级语言类及数据库类科目（即除MS Office高级应用外的其他二级科目）调整获证条件为：**总分达到60分**且**选择题得分达到50%及以上**（即选择题得分要达到20分及以上）的考生方可取得合格证书。

我是打算报名明年三月的湖北的java、office和web。因此都会写一篇文章。说来office还有点麻烦...还有明年五月的初级会计和软考初级...
[湖北省教育考试院公告栏](http://www.hbea.edu.cn/html/zhks/index.shtml) 
[湖北省报名系统NCRE报名系统](http://ncre.hbea.edu.cn/NCRE_EMS/studentlogin.aspx) 
### 1. 考试环境->NetBeans IDE 中国教育考试版 (2007) 
多少有一点软件洁癖的我，一想到要下这种过时的软件...不过是官方指定的，又有什么办法呢？而且下下来放在C盘根目录(`将下载的压缩文件直接解压缩到C盘根目录下这一点是必须的`)，考完试后顺便删掉也方便(笑)。

下载地址：[1](http://www.neea.edu.cn)和[2](http://ncre.neea.edu.cn/)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115120852885.png)
直接运行C盘根目录下的nbncre.exe，如下所示即启动成功：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115113751546.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)这个软件的不同点在于，NetBeans IDE 中国教育考试版 (2007) 可以**创建为中国教育考试涉及的“NCRE项目”**， “NCRE项目”是**标准的Java SE项目**；使用上（编辑、编译、调试等）同标准的Java SE项目**完全相同**。

但是，NCRE项目必须导入现有的Java源代码？其实也好理解，因为考试的操作题都会给你代码的；项目的属性文件等会创建在于源代码目录同级的NCREProject目录中，想要再次**打开已有的项目**时在NCREProject目录下选择相应的项目名称即可，**删除本目录不会对源文件有任何影响**； 运行软件时会在C盘根目录下产生一个tmp目录，用来**存放操作记录文件**，再次启动软件会自动显示上次打开的项目及设置，**删除本目录不会对源文件有任何影响**；


![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115120127493.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 2. 考试环境使用示例
将一个名为Test.java的文件放置在C:\Space\java1\目录中。Test.java文件包含以下内容：
```java
public class Test
{
     public static void main(String[] args)
     {
         System.out.println(“Hello, NCRE!”); 
     }
}
```
#### a. 创建项目
然后创建一个新的 NCRE项目，包含我们指定的源代码，将其用作我们的主项目。
1）	选择 “文件”> “新建项目” 。在 “类别” 列表中选择 “常规”。在 “项目”列表下，选择 “NCRE项目” 并单击 “下一步”。 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115150907151.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
2）	在 “源代码路径”输入框旁边，点击 “浏览” 按钮，选择目录C:\Space\java1；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115151031867.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
3）	项目名称会自动变为子目录名称，如本例中，项目名称自动变为java1，也可以更改项目名称。 
4）	“项目路径”会自动更改，如本例中改为C:\mySpace\NCREProject\java1。IDE自动在java1的同级目录中创建NCREProject目录，在NCREProject下的java1（项目名称）目录中存放新建项目的项目信息。 同时在C盘根目录下产生一个tmp目录，存放操作记录文件。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019111515223121.png)
5）单击完成即可。
#### b. 编辑Java代码
在 “项目” 中展开“源代码包”节点，双击 Test.java。 Test.java文件在文本编辑窗口中打开。 然后就可以开始编辑代码了。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115151440957.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)在 main 方法中加入以下内容： 
```java
String netbeans = “the NetBeans world!”;
System.out.println("Welcome to " + netbeans);
```
CTRL+S保存文件内容。点击"运行"-"运行主项目"即可运行代码。或者使用F6快捷键。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115151615616.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
#### c. 设置主项目
 NetBeans IDE中“主项目”的概念是指， **在同时打开多个项目的时候，IDE的操作（编译、调试等）只对“主项目”有效**； 主项目的名称在“项目”窗口中**以黑体显示**；可以通过在项目名称上点击鼠标右键设置其他项目为主项目。 
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115151857343.png)
#### d. 打开已经有的项目
如果想打开已有的项目进行编辑，则应选择NCREProject下面对应的项目名称，然后选择“打开项目文件夹”即可。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115152130499.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
#### e. 编译和运行项目
运行通常的Java程序可以使用F6直接运行；如果是Applet，由于没有主类，所以不能用F6来运行。正确的操作是右键单击源文件选择“运行文件”（shift+F6）。
#### f. 调试项目
在 Test.java文件中，将光标放置在System.out.println("Welcome to " + netbeans);这一行，后按 Ctrl-F8设置断点。 或者**直接在该行前的边框处点击鼠标左键**。 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115152620749.png)1）	选择菜单 “运行” > “调试主项目(F5)”。IDE 将打开 “调试” 窗口并**在调试器中运行项目，直到到达断点**。 
2）	选择 “局部变量” 窗口，可以查看变量netbeans的当前值。 
3）	单击工具栏中的 “继续”(Ctrl-F5)   继续程序运行。 
4）	当程序到达结尾时，调试器窗口将关闭。 
### 3. 一些用Netbeans的示例
#### a. 目录判断的简单程序
这个简单，按照上面的步骤操作就行了。
```java
import java.io.*;

public class Folder {
    public static void main(String[] args) {
        File myDir = new File("C:\\Program Files");
        System.out.println(myDir + (myDir.isDirectory() ? "是" : "不是") + "一个目录");
    }
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115162829228.png)
#### b. 加法运算功能的Applet小程序
已有源码：sum.java，保存在C:\Space\applet1目录下:![在这里插入图片描述](https://img-blog.csdnimg.cn/2019111516295050.png)
预期结果：如图2-1所示。
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115163100733.png)
```java
import java.awt.Button;
import java.awt.Label;
import java.awt.TextField;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;


public class sum extends java.applet.Applet implements ActionListener {
    Label label1 = new Label("+");
    Label label2 = new Label("=");
    
    TextField field1 = new TextField(6);
    TextField field2 = new TextField(6);
    TextField field3 = new TextField(6);
    
    Button button1 = new Button("相加");
    
    public void init() {
        add(field1);
        add(label1);
        add(field2);
        add(label2);
        add(field3);
        add(button1);
        button1.addActionListener(this);
    }
    
    public void actionPerformed(ActionEvent e) {
        int x = Integer.parseInt(field1.getText()) + Integer.parseInt(field2.getText());
        field3.setText(Int
        eger.toString(x));
    }
}
```
运行文件：在“项目”面板中，鼠标右键单击 sum.java，选择“运行文件”；或直接按快捷键 Shift + F6。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115163855814.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115164023427.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
#### c. 一个简单的命令行程序
开发一个计算年龄的简单命令行程序。
1. 建立项目和文件
(1) 启动NetBeans IDE 中国教育考试版 (2007) ，选择菜单“文件 - 新建项目”。 
(2) 在“新建项目”对话框中，“类别”列表选择“常规”，“项目”列表**选择“Java 应用程序”** 。
(3) 在“新建 Java 应用程序”对话框中，输入“项目名称”：Age，并选择“项目位置” 所在路径。选中“设置为主项目”和“**创建主类**”，如下图所示，单击“完成”。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115164426435.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)(4) 现在，在左上方的“项目”面板中，可以看到当前项目的所有内容。“Age包”中的“Main.java”是NetBeans为我们创建的主类。接下来可以直接编写Main.java的代码。(主类文件名Main，主类名为Main，age是包名，Age是项目名)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115164536577.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
2. 在Main.java中已经自动创建了主方法和构造函数，我们可以直接写了：
	```java
	/*
	 * Main.java
	 *
	 * Created on 2019年11月15日, 下午4:44
	 *
	 * To change this template, choose Tools | Template Manager
	 * and open the template in the editor.
	 */
	
	package age;
	
	import java.util.Calendar;
	
	public class Main {
	    private int year;
	    private int month;
	    private int day;
	    private int age;
	    
	    /** Creates a new instance of Main */
	    public Main(int y, int m, int d) {
	        year = y;
	        month = (((m >= 1) && (m <= 12)) ? m : 1);
	        day = (((d >= 1) && (d <= 31)) ? d : 1);
	        age = Calendar.getInstance().get(Calendar.YEAR) - year;
	    }
	    
	    /**
	     * @param args the command line arguments
	     */
	    public static void main(String[] args) {
	        // TODO code application logic here
	        Main main = new Main(1984, 11, 2);
	        System.out.println("生日" + main.year + "年" + main.month + "月" + main.day 
	        + "日, 今年" + main.age + "岁");
	    }   
	}
	```
一个很有用的功能：`通过快捷键 Alt + Shift + F 自动修复导入包`。代码提示功能快捷键为：`Ctrl + \`，自动代码格式化的快捷键为`Ctrl + Shift + F`。

3. 编译和运行文件
(1) 编译文件：在“项目”面板中，鼠标右键单击 Main.java，选择“编译文件”；或直接按快捷键 F9。
(2) 运行文件：在“项目”面板中，鼠标右键单击 Main.java，选择“运行文件”；或直接按快捷键 Shift + F6。
(3) 查看运行结果：在下方的“输入”面板中，会显示最终运行结果。当然，最简单的还是直接**运行主项目**。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115165848668.png)
#### d. 一个简单的图形界面程序
NetBeans中提供了功能非常强大的可视化开发工具：Matisse，下面就开发一个使用文本输入框的简单图形界面程序。

(1) 启动NetBeans IDE 中国教育考试版 (2007) ，选择菜单“文件 - 新建项目” 。在“新建项目”对话框中，“类别”列表选择“常规”，“项目”列表选择“Java 应用程序” ，单击“下一步”。 
(2) 在“新建 Java 应用程序”对话框中，输入“项目名称”：GUI，并选择“项目位置” 所在路径。选中“设置为主项目”，**取消选择“创建主类”**，单击“完成”。
(3) 在项目面板中，选择项目根节点“GUI”，单击鼠标右键，选择“新建 - JFrame 窗体”。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115191832264.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
(4) 在“新建 JFrame 窗体”对话框中，输入类名： MyFrame，输入包名：gui，单击“完成”。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115191850767.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)(5) 在“组件面板”中，选择“JLabel”， 将其拖入 MyFrame.java的设计视图中，其名称为：jLabel1；选择“JTextField”， 将其拖入 MyFrame.java的设计视图中，其名称为：jTextField1； 用鼠标直接调整MyFrame面板大小，并调整jLabel1和jTextField1的位置和大小：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115192112738.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)(6) 单击MyFrame面板，在右下方“属性”面板中修改MyFrame的属性，将其Title修改为：GUI 示例。用同样的方法修改jTextField1的Text属性为空。
(7) 鼠标右键单击jLabel1，在弹出菜单中选择“更改变量名称”，输入：jLabelInput。用同样的方法修改jTextField1的变量名称为jTextFieldOutput。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019111519231935.png)
(8) 选择jTextFieldOutput，在右下方面板中选择“事件”，双击第一个事件“ActionPerformed”，加入处理代码：
```java
private void jTextFieldOutputActionPerformed(java.awt.event.ActionEvent evt) {
        String getMessage = this.jTextFieldOutput.getText().trim();
        if (getMessage.equals("")) {
            this.jLabelInput.setText("你没有输入任何内容！");
        } else {
            this.jLabelInput.setText("你输入的内容是:" + getMessage);
        }
    }
```
(9) 运行：在“项目”面板中，鼠标右键单击 MyFrame.java，选择“运行文件”；或直接按快捷键 Shift + F6。弹出标题为“GUI 示例”的窗口。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115192820584.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)(10) 测试：在文本框中输入：Hello NetBeans，按回车，上方标签将显示“您输入的内容是：Hello NetBeans”。如果在文本框中没有输入任何内容，直接按回车，上方标签将显示“您没有输入任何内容”。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115192906736.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115192916642.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
#### e. 一个简单的Applet小程序
这个程序的功能在于绘制一条余弦曲线：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115193102599.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115193242177.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115193308938.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)然后编辑代码，使用快捷键“Ctrl + Shift + F”自动格式化代码，并使用快捷键“Alt + Shift + F”自动修复包：
```cpp
public void paint(Graphics g) {
       super.paint(g);
       double d, tx;
       int x, y, x0, y0;
       d = Math.PI / 100;
       x0 = y0 = 0;
       
       for (tx = 0, x = 20; tx <= 2 * Math.PI; tx += d, x++) {
           y = 120 - (int)(Math.cos(tx) * 50 + 60);
           if (x > 20) g.drawLine(x0, y0, x, y);
       
           x0 = x;
           y0 = y;
           g.drawString("y = cos(x)", 10, 70);
       }
   }
```
点击Shift+F6运行，这里不可以使用“运行主项目”：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115194139567.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这样一番使用下来，发现还是满顺手的，剩下的就在不断刷题中了解吧。
### 4. 模拟考试软件使用
未来教育考试系统：[链接](https://pan.baidu.com/s/1MMFnktdPILE0pwb4WZPw7w)，提取码：kpn8。

我打算用它考Java和Office。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115153550760.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115194623636.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)发现它的操作题不评分？（不知道Office是不是还是这样)。做我还是要做，只有12套，刷完不要多久。不过还是要结合一下**考试酷**。它还提供有软考的考题。明年我打算一起考。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191115212052872.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)无忧模拟软件有更全的题库。还是主要用无忧吧。另外web二级不考了，还不如把时间用在更有意义的事情上面，比如**PAT和软考**。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191116000808381.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 5. 一些错题
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191116224706291.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/2019111700162513.png)
java.awt.event。
