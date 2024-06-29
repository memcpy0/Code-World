@[toc]

# 1. 工作介绍
写一个脚本完成这一工作：调整最近抓取的[几百张二次元壁纸](https://memcpy0.blog.csdn.net/article/details/119887221)的大小，并在每张图片的角上添加一个黑猫图标水印，如下所示，图像其余部分实际上是透明的：
<img src="https://img-blog.csdnimg.cn/7d71751eed004d1d9262f02533e20e18.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="40%">

程序完成的事情具体如下：
- 载入标志图像 `catlogo.png` 作为 `Image` 对象，调整其宽度和高度以方便粘贴；
- 使用 `os.listdir()` ，循环遍历目标文件夹下的所有 `.png` 和 `.jpg` 文件，此处的目标文件夹将在脚本中给出；
- 通过 `size` 属性取得图像宽度和高度，检查图像是否宽于 `1920` ，过宽则减少至 `1920` ，并且按照比例计算缩小后的高度，用 `resize()` 方法进行调整；
- 在右下角用 `paste()` 粘贴图标图像；
- 保留原图片文件，调用 `save()` 将改变后的图像存入另一个文件夹。


---
# 2. 程序分解
## 2.1 打开标志图像、调整标志大小
在VS Code中建立一个名为 `resize-and-add-logo.py` 的文件，输入以下代码，其中定义了几个全局常量，载入图标图片并得到 `Image` 对象，鉴于标志图像过大，此处将调整标志的大小，其他部分则是用作说明的TODO注释：
```python
# resize-and-add-logo.py
# This script helps resize all images in the given directory to 
# fit in a 1920x1280 screen(especially the width), and add catlogo.png to the lower-right corner.
import os
from PIL import Image

FIT_WIDTH = 1920
LOG_WIDTH = 180
LOGO_FILENAME = 'catlogo.png' # in current working directory
GIVEN_DIR = '' # your image directory

logoIm = Image.open(LOGO_FILENAME)
logoWidth, logoHeight = logoIm.size # (808,768)
logoHeight, logoWidth = int((LOG_WIDTH / logoWidth) * logoHeight), LOG_WIDTH
logoIm = logoIm.resize((logoWidth, logoHeight))

# TODO: Loop over all image files in the given directory.

# TODO: Check if image needs to be resized.

# TODO: Calculate the new width and height to resize to.

# TODO: Resize the image.

# TODO: Add the logo.

# TODO: Save changes to another directory.
```
## 2.2 遍历所有文件并打开图像
首先在当前工作目录下，用 `os.makedirs()`（`os.mkdir()` 的超级加强版） 创建了一个文件夹 `withLogo` ，用于保存改变后的带有标志的图像，而非覆盖原始壁纸（免得费时重新下载），毕竟这只是一个练手脚本。关键字参数 `exist_ok=True` 避免该文件夹存在时爆出异常。然后，调用 `os.listdir()` 遍历给定目录，找到其中的所有PNG和JPG文件，将其载入为 `Image` 对象：
```python
# resize-and-add-logo.py
# This script helps resize all images in the given directory to 
# fit in a 1920x1280 screen(especially the width), and add catlogo.png to the lower-right corner.
import os
from PIL import Image

# ...
for filename in os.listdir(GIVEN_DIR):
    if not (filename.endswith('.png') or filename.endswith('.jpg')):
        continue # skip non-image files
    im = Image.open(os.path.join(GIVEN_DIR, filename))
    width, height = im.size
# ...
```
## 2.3 调整图像的大小
检查图像是否宽于 `1920` ，过宽则减少至 `1920` ，并且按照比例计算缩小后的高度，用 `resize()` 方法进行调整：
```python
# resize-and-add-logo.py
# This script helps resize all images in the given directory to 
# fit in a 1920x1280 screen(especially the width), and add catlogo.png to the lower-right corner.
import os
from PIL import Image

# ...
    if width > FIT_WIDTH:
        height = int((FIT_WIDTH / width) * height)
        width = FIT_WIDTH
    print('Resizing {}...'.format(filename)) 
    im = im.resize((width, height)) # resize the image
# ...
```
## 2.4 添加标志、保存更改
不管是否调整了图像的大小，标志仍应粘贴到右下角，不过确切位置则**取决于图像的大小和标志的大小**。如何计算粘贴的位置呢？简单，由于我们要让标志的右下角和图像右下角重合，因此粘贴标志的左顶坐标应该为图像的宽度/高度减去标志宽度/高度。此时先输出一条消息，指明标志已经加入到某张图片，然后执行粘贴、保存变更到 `withLogo` 目录中。**注意，此处如果不传入 `logoIm` 作为第三个参数，`paste()` 方法将粘贴不透明的矩形**，而非透明的像素。
```python
# resize-and-add-logo.py
# This script helps resize all images in the given directory to 
# fit in a 1920x1280 screen(especially the width), and add catlogo.png to the lower-right corner.
import os
from PIL import Image

# ...
    print('Adding logo to {}...'.format(filename))
    im.paste(logoIm, (width - logoWidth, height - logoHeight), logoIm) # add the logo
    im.save(os.path.join('withLogo', filename)) # save changes
# ...
```

## 2.5 完整程序
最后得到的完整程序如下所示：
```python
# resize-and-add-logo.py
# This script helps resize all images in the given directory to 
# fit in a 1920x1280 screen(especially the width), and add catlogo.png to the lower-right corner.
import os
from PIL import Image, ImageFile
ImageFile.LOAD_TRUNCATED_IMAGES = True # tolerate large image file

FIT_WIDTH = 1920
LOG_WIDTH = 180
LOGO_FILENAME = 'catlogo.png' # in current working directory
GIVEN_DIR = 'C:\\Users\\21839\\Pictures\\image'

logoIm = Image.open(LOGO_FILENAME)
logoWidth, logoHeight = logoIm.size # (808,768)
logoHeight, logoWidth = int((LOG_WIDTH / logoWidth) * logoHeight), LOG_WIDTH
logoIm = logoIm.resize((logoWidth, logoHeight))

os.makedirs('withLogo', exist_ok = True) # create a directory to save changed images
for filename in os.listdir(GIVEN_DIR):
    if not (filename.endswith('.png') or filename.endswith('.jpg')):
        continue # skip non-image files
    im = Image.open(os.path.join(GIVEN_DIR, filename))
    width, height = im.size
    if width > FIT_WIDTH:
        height = int((FIT_WIDTH / width) * height)
        width = FIT_WIDTH
    print('Resizing {}...'.format(filename)) 
    im = im.resize((width, height)) # resize the image
    print('Adding logo to {}...'.format(filename))
    im.paste(logoIm, (width - logoWidth, height - logoHeight), logoIm) # add the logo
    im.save(os.path.join('withLogo', filename)) # save changes
```
在开头添加了两句，导入了 `ImageFile` ，是因为在试验性的运行过程中报错 `Image file is truncated` 。不过这个问题很常见，之所以 `truncated image` 是因为图片太大了。在Pillow库中打开 `ImageFile.py` 这个文件，会发现在文件顶部有固定的一个数值：
```python
MAXBLOCK = xxxxxx
```
由于载入的图片已经超过了 `MAXBLOCK` 限制大小，PIL无法完整处理，必须将这个图片截断一部分，所以报告了 `Image file is truncated` 这个错误。要解决它需要在Python脚本里设置 `LOAD_TRUNCATED_IMAGES` 设为 `True` ，实际上将导致加载的图片少掉一部分……

最后的效果图大致如下，为什么CSDN上传图片的水印这么大[○･｀Д´･ ○]：
![在这里插入图片描述](https://img-blog.csdnimg.cn/3d49114cb0ee4fe19e6aedc5288257de.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_Q1NETiBAbWVtY3B5MA==,size_65,color_FFFFFF,t_70,g_se,x_16)
还有我的欧根老婆，AZUR LANE的图标说不定也是这样添加上去的呢：
![在这里插入图片描述](https://img-blog.csdnimg.cn/4f5e848dd84e4c3da17b6dde97abf0d8.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_Q1NETiBAbWVtY3B5MA==,size_73,color_FFFFFF,t_70,g_se,x_16#pic_center)
输出文件夹的具体状况如下，不管高度如何，宽度总算是适应了我的电脑屏幕，而且在调整大小的过程中大致保持了图像的纵横比、不至于使图像变形，这就足够了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/58ddfe87649545cd84284ce86524542d.gif#pic_center)




---

# 3. 类似程序的想法
批量合成图像或修改图像大小在许多应用中都有用。可以编写类似的程
序，完成以下任务：
- 为图像添加**文字或网站URL或时间戳**，就像CSDN做的一样，用到的是 `ImageDraw` 的 `text()` 方法；
- 为图像添加一个几乎透明的**水印**，防止他人复制，像本文一样操作即可；
- 根据图像的大小，将图像复制或移动到不同的文件夹中
