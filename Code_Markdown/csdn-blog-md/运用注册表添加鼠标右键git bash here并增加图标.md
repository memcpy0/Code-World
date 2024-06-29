 
突然发现自己<strong>鼠标右键没有git bash here</strong>，或者安装之后就没有git bash here。后面那种情况多半是没有默认装在C盘。我们装在其他盘的时候就需要自己去配置。git gui目前用不上，这里只讲git bash here。网上一堆教程，说法不同大多不能用要么就很繁琐。<strong>反复测试下面方法最有效。</strong></p> 
<p><strong>1、win+R打开运行界面输入regedit并回车打开注册表，或者直接笔记本左下角搜索框搜索注册表。</strong></p> 
<p><img alt="" src="https://img-blog.csdnimg.cn/img_convert/a358c119cec76cc5702ce7c03442c92e.png" width="513" height="272"></p> 
<p>2、按照路径打开<strong>计算机\HKEY_CLASSES_ROOT\Directory\Background\shell\，</strong>切记是<strong>红2箭头标注的Background目录</strong>而<strong>不是绿色箭头标注的shell目录</strong>。网上说在shell目录里面搞事的我都没搞成功。</p> 
<p><img alt="" src="https://img-blog.csdnimg.cn/img_convert/2d113be32393ca2ce7dde0251c7416de.png" width="1200" height="739"></p> 
<p>3、进入后发现没有shell目录就新建一个，有的话就在shell上<strong>右键新建项，取名Git Bash Here</strong>，这就是鼠标邮件会显示的名字，后面不用再去配置，减少麻烦。<strong>点击Git Bash Here右键新建项取名command</strong> 如下图：</p> 
<p><img alt="" src="https://img-blog.csdnimg.cn/img_convert/a4c715c9038e97ba30ed94c97cccdc45.png" width="688" height="301"></p> 
<p>4、在红箭头表制标注的默认地方右键修改，值为你的git安装目录加上git-bash.exe。我的安装目录在下面第二张图，绿色箭头bin目录里面也有git-bash.exe，网上说配置那个的别看了，有毒，没有用就是最外面红色箭头指向的就可以。</p> 
<p><img alt="" src="https://img-blog.csdnimg.cn/img_convert/b47d3c12bd02af1c715e1fe7c60c51c2.png" width="1200" height="457"></p> 
<p>我的安装目录是：D:\Program Files\Git\git-bash.exe。确定即可</p> 
<p><img alt="" src="https://img-blog.csdnimg.cn/img_convert/5892c607991e83bd1127d77bccd9c5aa.png" width="898" height="623"></p> 
<p>5、点击<strong>Git Bash Here</strong>，<strong>右键新建字符串值，取名Icon,</strong>目的是为了给Git Bash Here加一个图标好看一些。修改值为安装路径D:\Program Files\Git加上\<strong>mingw64\share\git\git-for-windows.ico</strong>。每个人安装路径不一样自己找一下即可。</p> 
<p><img alt="" src="https://img-blog.csdnimg.cn/img_convert/8677a5ccb4e4def73563dec3aac53bb3.png" width="997" height="442"></p> 
<p><img alt="" src="https://img-blog.csdnimg.cn/img_convert/d18dadf4009095efcac8296c21ab5e79.png" width="569" height="249"></p> 
<p>点击确定即可。右键你就可以看到下图你要的东西。</p> 
<p><img alt="" src="https://img-blog.csdnimg.cn/img_convert/b0624e526c7a18bd34cfe566caed80c9.png" width="249" height="319"></p> 
<p>多次测试，希望可以帮助到你。</p>
                
