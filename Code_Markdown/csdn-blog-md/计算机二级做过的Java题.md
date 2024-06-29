@[toc]
计算机二级Java对我来说，最大的问题在于Applet、Swing以及一些类库、输入输出流没有使用过。所以把这些题汇总学一下。
### 题1 Applet
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191116145815207.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
Applet(小程序)：
```java
import java.applet.*;
import java.awt.Graphics;

//*********Found********
public class Java_1 extends Applet {  
   public void paint( Graphics g )
   {
//*********Found********
      g.drawString( "欢迎你来参加Java 语言考试!", 25, 25 );
   }
}
```
```html
<html>
<applet code="Java_1.class" width=300 height=45>
</applet>
</html>
```
### 题2 Swing
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019111614590690.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
Java的swing：
```java
import java.awt.*;
import java.awt.font.*;
import java.awt.geom.*;
import javax.swing.*;

public class Java_3
{
   public static void main(String[] args)
   {
      FontFrame frame = new FontFrame();
      frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      frame.setVisible(true);
   }
}

     //*********Found********
class FontFrame extends JFrame
{
   public FontFrame()
   {
      setTitle("沁园春.雪");
      setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
      FontPanel panel = new FontPanel();
      Container contentPane = getContentPane();
     //*********Found********
      contentPane.add(panel);
   }
   public static final int DEFAULT_WIDTH = 300;
   public static final int DEFAULT_HEIGHT = 200;
}

     //*********Found********
class FontPanel extends JPanel
{
   public void paintComponent(Graphics g)
   {
      super.paintComponent(g);
      Graphics2D g2 = (Graphics2D)g;
      String message = "数风流人物，还看今朝！";
      Font f = new Font("隶书", Font.BOLD, 24);
      g2.setFont(f);
      FontRenderContext context = g2.getFontRenderContext();
      Rectangle2D bounds = f.getStringBounds(message, context);
      double x = (getWidth() - bounds.getWidth()) / 2;
      double y = (getHeight() - bounds.getHeight()) / 2;
      double ascent = -bounds.getY();
      double baseY = y + ascent;
      g2.setPaint(Color.RED);
     //*********Found******** 
      g2.drawString(message, (int)x, (int)(baseY));
   }
}
```
### 题3 0-20间的随机整数的阶乘
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191116145937518.png)
Java类库使用：
```java
import java.util.Random;

public class Java_2
{
   public static void main(String args[]){
      Random random = new Random();
      float x = random.nextFloat();//产生0.0与1.0之间的一个符点数
      int n = Math.round(20*x);  //构造20以内的一个整数
      long f = 1 ;  //保存阶乘的结果
      int k = 1 ;  //循环变量
   //*********Found********
      do{f = f*k;
         k++;
   //*********Found********
      }while(k <= n);  	
      System.out.println(n+"!= "+f);
   }
}
```
### 题4 Swing
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191116234600248.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
```java
import javax.swing.JOptionPane;  //导入JOptionPane类

public class Java_1 {
   public static void main( String args[] )
   {
//*********Found********
      JOptionPane.showMessageDialog(
         null, "欢迎\n你\n参加\nJava\n考试!" );
      System.exit( 0 );  // 结束程序
   }
}
/* JOptionPane类的常用静态方法如下：
   showInputDialog()
   showConfirmDialog()
   showMessageDialog()
   showOptionDialog()
*/
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191116234941553.png)
### 题5 普通题 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191116235402215.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
```java
public class Java_2
{
    public static void main(String[] args) {
        int[][] aMatrix = {{1,1,1,1,1},{2,2,2,2,2},{3,3,3,3,3},{4,4,4,4,4}};
		int i = 0; //循环变量
		int j = 0; //循环变量
        //print matrix
        for (i = 0; i < aMatrix.length; i++) {
   //*********Found********
	       for ( j = 0; j < aMatrix[i].length; j++) {
   //*********Found********
	        System.out.print(aMatrix[i][j] + " ");
	       }
	    System.out.println();
        }
    }
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191116235535244.png)
### 题6 Swing
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191116235625780.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191116235639522.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)感觉二级中最难的应该是Java，毕竟Java要考那麽多内容：输入输出流、多线程、AWT和Swing、Applet和集合框架。

还有，这个未来教育的系统...好差。快点做完这12套然后换到无忧再认真刷一下**。最好将二级的精力更多放在Office上面**。
```java
import java.text.DecimalFormat;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

     //*********Found********
public class Java_3 extends JFrame implements ActionListener {
   private JTextField input1, input2, output;
   private int number1, number2;
   private double result;

   // 初始化
   public Java_3()
   {
     //*********Found********
      super( "示范异常" );

      Container c = getContentPane();
      c.setLayout( new GridLayout( 3, 2 ) );

      c.add( new JLabel( "输入分子",
                         SwingConstants.RIGHT ) );
      input1 = new JTextField( 10 );
      c.add( input1 );

      c.add(
         new JLabel( "输入分母和回车",
                     SwingConstants.RIGHT ) );
      input2 = new JTextField( 10 );
      c.add( input2 );
      input2.addActionListener( this );

      c.add( new JLabel( "计算结果", SwingConstants.RIGHT ) );
      output = new JTextField();
      c.add( output );

      setSize( 425, 100 );
      show();
   }

   //处理 GUI 事件
   public void actionPerformed( ActionEvent e )
   {
      DecimalFormat precision3 = new DecimalFormat( "0.000" );

      output.setText( "" ); // 空的JTextField输出

     //*********Found********
      try {         
         number1 = Integer.parseInt( input1.getText() );
         number2 = Integer.parseInt( input2.getText() );

         result = quotient( number1, number2 );
     //*********Found********
         output.setText(""+result);
      }
      catch ( NumberFormatException nfe ) {
         JOptionPane.showMessageDialog( this,
            "你必须输入两个整数",
            "非法数字格式",
            JOptionPane.ERROR_MESSAGE );
      }
      catch ( Exception dbze ) {
     //*********Found********
         JOptionPane.showMessageDialog( this, 
            "除法异常",
            "除数为零",
            JOptionPane.ERROR_MESSAGE );
      }
   }

   // 定义求商的方法，如遇除数为零时，能抛出异常。
     public double quotient( int numerator, int denominator )
      throws Exception
   {
      if ( denominator == 0 )
         throw new Exception();

      return ( double ) numerator / denominator;
   }

   public static void main( String args[] )
   {
      Java_3 app = new Java_3();

      app.addWindowListener(
         new WindowAdapter() {
            public void windowClosing( WindowEvent e )
            {
               e.getWindow().dispose();
               System.exit( 0 );
            }
         }
      );
   }
} 
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191117001201886.png)
