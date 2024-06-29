@[toc]


现在的我们，几乎每时每刻都在遇到数字图片文件，处理这些文件当然可以用诸如Adobe Photoshop之类的专业软件。只是如果遇到了大量图像的编辑工作，尤其是某些比较简单粗暴的操作，手动编辑将是一次漫长而枯燥的苦旅。

不过我们可以使用Python和第三方模块Pillow，该模块包含了一些函数，如裁剪图像、调整大小、编辑图像等。**有了这种能力，我们就可以轻松处理成千上万张图片文件**。不过开始本文之前，记得用 `pip install pillow` 安装这一模块（要是安装Haskell包时也能这么轻松就好了┏(゜ロ゜;)┛）：

```bash
$ pip install pillow
Collecting pillow
  Downloading Pillow-8.3.1-1-cp39-cp39-win_amd64.whl (3.2 MB)
     |████████████████████████████████| 3.2 MB 356 kB/s 
Installing collected packages: pillow
Successfully installed pillow-8.3.1
```

---
# 1. 计算机图像基础
## 1.1 颜色和RGBA值
**像素是计算机屏幕上能够显示一种颜色的最小点**，计算机程序通常把图像分解为一系列**像素点**，因此**图像就是像素的集合**。图像中的每种颜色都被表示为一个RGBA值，或者说一组四个取值范围为 `[0, 255]` 的整数，分别指定颜色中的红、绿、蓝和 `alpha` 透明度的值。图像中的每个像素都有一个表示颜色的RGBA值，用以准确告诉计算机屏幕（上对应坐标的像素）该显示哪种颜色，`alpha` 值则决定了，当图像显示在屏幕上、遮住了背景图像或桌面壁纸时，透过这张图像我们可以看到多少背景。

>屏幕分辨率是指纵横向上的像素点数，单位是px。**屏幕分辨率是确定计算机屏幕上显示多少信息的设置**，以水平和垂直像素来衡量。就相同大小的屏幕而言，当屏幕分辨率低时（例如 640 x 480），在屏幕上显示的像素少，单个像素尺寸比较大。屏幕分辨率高时（例如 1600 x 1200），在屏幕上显示的像素多，单个像素尺寸比较小。
**显示分辨率就是屏幕上显示的像素个数**，分辨率160×128的意思是水平方向含有像素数为160个，垂直方向像素数128个。屏幕尺寸一样的情况下，分辨率越高，显示效果就越精细和细腻。

**Pillow中的RGBA值是一个四个整数的元组**，如红色表示为 `(255, 0, 0, 255)`（即颜色中红色的值最大、没有绿和蓝，`alpha` 值最大即完全不透明），绿色表示为 `(0, 255, 0, 255)` ，蓝色是 `(0, 0, 255, 255)` ，白色是 `(255, 255, 255, 255)` 即各种颜色的组合，黑色没有任何颜色，是 `(0, 0, 0, 255)` 。
> 提示1：如果一种颜色的 `alpha` 值为 `0` ，则不论RGB值是什么，该颜色都不可见，是透明的。透明的红色等同于透明的黑色。计算机体系结构中的**透明性**概念，是否取自于此呢？

> 提示2：小时候肯定学过三原色，混合红黄蓝三种颜色可以得到其他颜色，比如混合黄色和蓝色可以得到绿色。不过这是一种不严谨的说法。
> <b></b>
> 准确来说，要区分**色光三原色**（红 `red` 、绿 `green` 、蓝 `blue`）和**色料三原色**（红黄蓝或者更准确的是，品红 `magenta` 、黄 `yellow` 、青 `cyan`），两者分别对应**色光加色模型**和**色料减色模型**：
![在这里插入图片描述](https://img-blog.csdnimg.cn/760c4b4b5a864fb8ab87c16d781f0a01.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
> 由于光的物理使用加色模型，在计算机程序中也使用RGB值表示颜色，这样计算机屏幕发出的红光、绿光和蓝光就可以组合形成其他颜色。

 
下表是一些标准颜色名称和对应的RGBA值：
| 名称 | RGBA值 | 名称 | RGBA值
|:---|:---|:---|:---
| `White` | `(255, 255, 255, 255)` | `Black` | `(0, 0, 0, 255)`
| `Red` | `(255, 0, 0, 255)` | `Gray` | `(128, 128, 128, 255)` 
| `Green` | `(0, 255, 0, 255)` | `Yellow` | `(255, 255, 0, 255)`
| `Blue` | `(0, 0, 255, 255)` | `Purple` | `(128, 0, 128, 255)`

为了减少记忆负担，不用记住那麽多颜色的RGBA值，Pillow提供了 `ImageColor.getcolor()` 函数，接受一个**不区分大小写**的颜色名称字符串作为第一个参数（Pillow支持超过100种标准颜色名称），字符串 `'RGBA'` 作为第二个参数，返回一个RGBA四元组：

```python
>>> from PIL import ImageColor
>>> ImageColor.getcolor('red', 'RGBA')
(255, 0, 0, 255)
>>> ImageColor.getcolor('green', 'RGBA') 
(0, 128, 0, 255)
>>> ImageColor.getcolor('BLUE', 'RGBA')  
(0, 0, 255, 255)
>>> ImageColor.getcolor('White', 'RGBA') 
(255, 255, 255, 255)
>>> ImageColor.getcolor('bLack', 'RGBA') 
(0, 0, 0, 255)
>>> ImageColor.getcolor('grey', 'RGBA')  
(128, 128, 128, 255) 
>>> ImageColor.getcolor('yellow', 'RGBA') 
(255, 255, 0, 255)
>>> ImageColor.getcolor('chocolate', 'RGBA') 
(210, 105, 30, 255)
>>> ImageColor.getcolor('CornflowerBlue', 'RGBA') 
(100, 149, 237, 255)
```
为什么是从PIL中导入而不是从Pillow？因为Pillow的模块名称就是PIL，以保持与老模块 `Python Imaging Library` 的向后兼容。同时由于Pillow作者的设计方式，我们**必须**使用 `from PIL import xxx` 这种形式的 `import` 语句。所以，代码中不会出现 `import Pillow` 、`from Pillow import xxx` 或者简单地 `import PIL` 。


## 1.2 坐标和Box元组
图像像素的位置用 `(x, y)` 坐标指定，分别表示像素在图像中的水平和垂直位置。**原点位于图像最左上角处**，用 `(0, 0)` 指定。`x` 坐标以原点处为零，从左至右不断增加；`y` 坐标以原点处为零，从上到下不断增加：
![在这里插入图片描述](https://img-blog.csdnimg.cn/f7ad4a99ab2d450d92b964d0d3c3c75c.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

很多Pillow函数或方法需要一个矩形元组参数，即需要一个四个整数坐标的元组 `(leftmost, topmost, rightmost, downmost)`，以表示图像中的一个矩形区域。更准确的说是表示矩形区域 `leftmost <= x < rightmost, topmost <= y < downmost` ，左闭右开，包括左和顶坐标、但不包括右和底坐标。如矩形元组 `(3, 1, 9, 6)` 表示下图黑色矩形的所有像素：
![在这里插入图片描述](https://img-blog.csdnimg.cn/61665a4043914dab9fedf8d6ea25e016.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 2. 用Pillow操作图像
现在知道了Pillow中颜色和坐标的定义，就可以开始用Pillow处理图像了。以下图 `Zophie.png` 为例：
<img src="https://img-blog.csdnimg.cn/6622552aa70f487184a27c64d652611d.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" width="55%">

本节只是这些函数的简单介绍，具体使用方法可以用 `help(ImageObject.method)` 或 `help(Image.method)` 进行查看。
## 2.1 加载图像、保存图像
加载图像需要从Pillow中导入 `Image` ，调用 `Image.open()` 并传入图像路径字符串，再保存到类似 `catIm` 这样的变量中：

```python
>>> from PIL import Image
>>> catIm = Image.open('sources/zophie.png') 
```
如果图像文件不在当前工作目录，可以像上面一样传入路径字符串，也可以调用 `os.chdir()` 函数，将工作目录转为包含图像文件的文件夹：

```python
>>> import os 
>>> os.chdir('C:\\folder_with_image_file')
```
`Image.open()` 函数将返回一个 `Image` 对象数据模型，是Pillow将图像表示为Python值的中介方法。调用 `Image.open()` 并传入文件路径字符串，就可以从一个**任意格式的图像文件**加载一个 `Image` 对象。对图像进行的所有修改，如旋转、调整大小、裁剪、绘画和其他图像操作，均能通过调用这一 `Image` 对象上的方法来完成。之后通过 `save()` 方法，将对 `Image` 对象的所有更改保存到**任意格式的图像文件**中。

## 2.2 处理 `Image` 数据类型、创建空白图像
在文件资源管理器中，我们可以看到一张图像文件的诸多信息：
<img src="https://img-blog.csdnimg.cn/35536aefcdcb435a8311ecbbec0d654b.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="75%">

加载得到的 `Image` 对象也有这些有用的属性，以提供加载的图像文件的基本信息，如宽度、高度、分辨率、文件名和图像格式。下面的代码中，能看到该 `Image` 对象的大小 `size` 属性是一个二元组，包含该图像的宽度和高度像素数，将其分别赋给 `width` 和 `height` 变量，以便分别进行访问；文件名 `filename` 属性描述了传入文件的名称；格式 `format` 和 `format_description` 属性是字符串，描述了原始文件的图像格式。最后调用 `Image.save()` 方法，将图像保存为 `zophie.jpg` ——**Pillow看到文件扩展名是 `.jpg` ，会自动使用JPEG图像格式保存图像**。
```python
>>> catIm.size
(816, 1088)
>>> width, height = catIm.size
>>> width
816
>>> height
1088
>>> catIm.filename
'sources/zophie.png'
>>> catIm.format
'PNG'
>>> catIm.format_description
'Portable network graphics'
>>> catIm.save('zophie.jpg')
```
Pillow还提供了 `Image.new()` 函数，返回一个表示空白图像的 `Image` 对象，具体方法参数如下：
- 字符串 `'RGBA'` ，将颜色模式设置为RGBA（或者其他模式）；
- 表示大小的两个整数的元组，作为新图像的宽度和高度；
- 图像开始使用的背景颜色，一个表示RGBA值的四整数元组，用 `ImageColor.getcolor()` 的返回值作为这一参数是很合适的，或者也可以使用标准颜色名称字符串。如果不指定背景色，默认颜色为不可见的黑色 `(0, 0, 0, 0)` ，即透明背景的图像。

以下代码分别创建了一个大小为 `100x200` 、红色背景的 `Image` 图像和一个大小为 `20x20` 、无背景色的图像，并分别保存到文件中：
```python
>>> im = Image.new('RGBA', (100, 200), 'red')
>>> im.save('redImage.png')
>>> im2 = Image.new('RGBA', (20, 20))
>>> im2.save('transparentImage.png')
```

## 2.3 裁剪图像、展示图像
在图像内选择一个矩形区域、并删除矩形外的一切，就是**裁剪图像**。`Image` 对象的 `crop()` 方法接受一个矩形元组，返回一个 `Image` 对象表示裁剪后的图像。裁剪不会修改原图像，而是返回一个新的 `Image` 对象。

以下代码向 `crop()` 传入一个矩形元组，指定 `zophie.png` 中的一个矩形区域，包含这只猫的脸，裁剪 `catIm` 后得到一个新的 `Image` 对象，表示 `230x215` 的裁剪区域，保存在 `croppedIm` 中，最后调用 `croppedIm.save()` 存入文件 `cropped.png` ：
```python
>>> croppedIm = catIm.crop((335, 345, 565, 560))
>>> croppedIm.save('cropped.png')
```
下面调用 `Image` 对象的 `show()` 方法，展示 `catIm, croppedIm` 这两张图像：
<img src="https://img-blog.csdnimg.cn/b155699313324396a12969a2a83fdfaa.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="55%">

## 2.4 复制、粘贴图像到其他图像
`copy()` 方法返回一个和原先的 `Image` 对象一样的新的 `Image` 对象。如果需要修改图像、同时保持原有版本不变，这就很有用了。以下代码复制 `catIm` 对象得到了 `catCopyIm` ，两者是独立的 `Image` 对象、只是图像相同。可以随意修改 `catCopyIm` ，将它存入一个新的文件，而 `zophie.png` 不会改变：

```python
>>> catCopyIm = catIm.copy()
```
`Image` 对象的 `paste()` 方法，能将另一个图像粘贴在它身上的某个位置，它的两个参数分别是源 `Image` 对象和一个 `(x, y)` 坐标元组，坐标元组指明源 `Image` 对象粘贴到主 `Image` 对象时左上角的位置。注意，**`paste()` 方法会在原图 `Image` 对象上进行修改，不会返回粘贴后图像的 `Image` 对象**。我们尝试粘贴一个较小的图像到 `catCopyIm` ：

```python
>>> faceIm = catIm.crop((335, 345, 565, 560))
>>> faceIm.size  
(230, 215)
>>> catCopyIm.paste(faceIm, (0, 0))
>>> catCopyIm.paste(faceIm, (400, 500))
>>> catCopyIm.show()                   
>>> catCopyIm.save('pasted.png')
```
调用 `show()` 方法展示的新图像如下：
<img src="https://img-blog.csdnimg.cn/1c21bc46196846298264bafcac835562.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="55%">
以下代码用 `Zophie` 的头平铺满整个图像
```python
>>> catImWidth, catImHeight = catIm.size  
>>> faceImWidth, faceImHeight = faceIm.size
>>> catCopy2 = catIm.copy()
>>>> for left in range(0, catImWidth, faceImWidth): 
...     for top in range(0, catImHeight, faceImHeight):
...         print(left, top)
...         catCopy2.paste(faceIm, (left, top))
... 
0 0
0 215
0 430
0 645
0 860
......
690 860
690 1075
>>> catCopy2.show()
>>> catCopy2.save('tiled.png')
```
粘贴完成后进行展示和保存：
<img src="https://img-blog.csdnimg.cn/e9098e14fa1b4e5caa04aaa16f733b52.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="55%">

这一粘贴函数还有需要注意的地方，一般而言，**调用 `paste()` 会将透明像素作为白色像素进行粘贴**；如果要粘贴的图像有透明像素，要再将该 `Image` 对象作为第三个参数传入，这样就不会粘贴一个不透明的矩形。这就是所谓的**遮罩** `Image` 对象，**遮罩**是一个 `Image` 对象、**只有 `alpha` 值是有效的、红绿蓝值将被忽略**，遮罩告诉 `paste()` 函数哪些像素应该复制、哪些应该保持透明。

 
## 2.5 调整图像大小
`resize()` 方法在 `Image` 对象上调用，返回指定宽度和高度的一个新 `Image` 对象，它接受二整数元组作为参数。注意，**这一方法的元组参数中只允许整数**，所以以下代码中要取整。
```python
>>> quartersizedIm = catIm.resize((int(width / 2), int(height / 2)))
>>> svelteIm = catIm.resize((width, height + 300))
>>> quartersizedIm.save('quartersized.png') 
>>> svelteIm.save('svelte.png') 
```

## 2.6 旋转和翻转图像
用 `rotate()` 方法可以旋转图像，返回旋转后的一个新 `Image` 对象，它接受一个整数或浮点数，表示图像逆时针旋转的度数。以下代码将其简单旋转了90度、180度、270度：
```python
>>> catIm.rotate(90).show()
>>> catIm.rotate(180).show() 
>>> catIm.rotate(270).show() 
```
图像依次展示如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/fac8b7edce3445c9932185f6eac117d9.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
不知道注意了没有，在 `Image` 对象上调用 `rotate(angle)` ，**当图像本身旋转90度或270度时，宽度和高度发生变化；如果旋转其他角度如6度时，图像的原始尺寸会保持**，只是有些边角会被边缘截断，并且会用黑色的背景（Windows上）/透明像素（OS X上）来填补旋转造成的缝隙。然而无论如何，**保存为图片文件后，文件大小不会变化，仍然是原始文件大小**。不过凡事皆有例外，**将 `expand` 参数设置为 `True` 之后，就会放大图像的尺寸以适应非180度旋转后的新图像**，从而保持图片边角可见，如果有空隙也会用黑色背景填充，**保存为图片文件后文件尺寸将变大**：
```python
>>> catIm.rotate(6).show()
>>> catIm.rotate(6, expand=True).show()
```
展示的图像如下（为适应博客展示进行了一定的缩放）：
<img src="https://img-blog.csdnimg.cn/c3a9a653fc9b4f79986283331b67c20f.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="45%"><img src="https://img-blog.csdnimg.cn/f375864cb1154e8ab551985002d57091.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="47.5%">


调用 `transpose()` 方法，将返回镜像翻转后或逆时针旋转特定角度（90度、180度、270度）的一个新 `Image` 图像，只是必须向 `transpose()` 方法传入 `Image.FLIP_TOP_BOTTOM` 或 `Image.FLIP_LEFT_RIGHT` 或其他参数：
```python
>>> help(catIm.transpose)
Help on method transpose in module PIL.Image:

transpose(method) method of PIL.PngImagePlugin.PngImageFile instance
    Transpose image (flip or rotate in 90 degree steps)

    :param method: One of :py:data:`PIL.Image.FLIP_LEFT_RIGHT`,
      :py:data:`PIL.Image.FLIP_TOP_BOTTOM`, :py:data:`PIL.Image.ROTATE_90`,
      :py:data:`PIL.Image.ROTATE_180`, :py:data:`PIL.Image.ROTATE_270`,
      :py:data:`PIL.Image.TRANSPOSE` or :py:data:`PIL.Image.TRANSVERSE`.
    :returns: Returns a flipped or rotated copy of this image.
```
建议自己一一试一下这些代码，体验一下和 `rotate()` 特定角度的不同——**没有用黑色背景填充**（指得是90度和270度旋转时），**保存为图片文件后文件大小要么不变、要么宽度和高度交换**：
```python
>>> catIm.transpose(Image.FLIP_LEFT_RIGHT).show()
>>> catIm.transpose(Image.FLIP_TOP_BOTTOM).show() 
>>> catIm.transpose(Image.TRANSPOSE).show()       
>>> catIm.transpose(Image.TRANSVERSE).show() 
>>> catIm.transpose(Image.ROTATE_90).show()  
>>> catIm.transpose(Image.ROTATE_180).show() 
>>> catIm.transpose(Image.ROTATE_270).show()
```
## 2.7 更改单个像素
对于单个像素的颜色，可以用 `getpixel(), putpixel()` 方法取得和设置，它们都接受一个坐标 `(x, y)` 元组（表示像素的坐标），`putpixel()` 方法还接受一个元组作为该像素的颜色，可以是四整数RGBA元组或三整数RGB元组：
```python
>>> im  = Image.new('RGBA', (100, 100))
>>> im.getpixel((0, 0))
(0, 0, 0, 0)
>>> for x in range(100):
...     for y in range(50):
...         im.putpixel((x, y), (210, 210, 210))
... 
>>> from PIL import ImageColor
>>> for x in range(100):
...     for y in range(50, 100):
...         im.putpixel((x, y), ImageColor.getcolor('darkgrey', 'RGBA'))
...
>>> im.getpixel((0, 0))
(210, 210, 210, 255)
>>> im.getpixel((0, 50))
(169, 169, 169, 255)
>>> im.show()
```
以上代码中先得到一个 `100x100` 的透明正方形，对一些坐标调用 `getpixel()` 返回 `(0, 0, 0, 0)` 。之后使用嵌套循环遍历图像上半部分的所有像素，用 `putpixel()` 设置每个像素的颜色 `(210, 210, 210)` ，即浅灰色。然后，由于 `putpixel()` 方法不接受标准颜色名称，所以必须使用 `ImageColor.getcolor()` 来获得 `'darkgray'` 的颜色元组，再循环遍历图像的下半部分像素，设置为 `'darkgray'` 深灰色。如下图所示：
<img src="https://img-blog.csdnimg.cn/0d0ff9dd19f24e84a1bbc17d0709a391.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">

这样一个个绘制像素实在辛苦，如果要绘制形状，之后会使用 `ImageDraw()` 函数。

---
# 3. 练手：用Python添加标志


具体见这篇文章：[【Python】用Pillow库为几百张二次元壁纸添加徽标](https://memcpy0.blog.csdn.net/article/details/119920016)。


---
# 4. 在图像上绘画
如果需要在图像上进行绘画，如划线、矩形或其他简单形状，要用Pillow的 `ImageDraw` 。首先导入 `Image, ImageDraw` ，然后创建一个新的 `200x200` 的白色图像，将其传入 `ImageDraw.Draw()` 函数，得到一个 `ImageDraw` 对象，调用这一新对象的方法，就可以在 `Image` 对象上绘制形状和文字。

```bash
>>> from PIL import Image, ImageDraw
>>> im = Image.new('RGBA', (200, 200), 'white')
>>> draw = ImageDraw.Draw(im)
```
`ImageDraw` 对象的完整文档在：[http://pillow. readthedocs.org/en/latest/reference/ImageDraw.html](http://pillow.readthedocs.org/en/latest/reference/ImageDraw.html)，除了这里提到的还有其他绘制方法。

## 4.1 绘制形状
`ImageDraw` 对象上有如下方法，用于绘制形状，其中的 `fill, outline` 两个参数是可选的，未指定时默认为白色：
- **点**：`point(xy, fill)` 方法绘制单个像素，`xy` 表示要画的点的列表，可以是 `(x, y)` 坐标的元组的列表，或者是没有元组的 `x` 和 `y` 坐标的列表，如 `[(x1, y1), (x2, y2), ...]` 或 `[x1, y1, x2, y2, ...]` 。可选的 `fill` 参数是点的填充颜色，要么是一个 `'RGBA'` 元组，要么是颜色名称的字符串。
- **线**：`line(xy, fill, width)` 方法绘制一条线或一系列的线，`xy` 要么是一个元组的列表，要么是一个整数列表，如 `[(x1, y1), (x2, y2), ...]` 或 `[x1, y1, x2, y2, ...]` 。每个点都是正在绘制的线上的一个连接点。可选的 `fill` 参数是线的颜色，用法同上。可选的 `width` 参数是线的宽度，默认是 `1` 。
- **矩形**：`rectangle(xy, fill, outline)` 方法绘制一个矩形，`xy` 参数是一个矩形元组，形式为 `(left, top, right, bottom)` 。可选的 `fill` 参数是矩形内部的填充颜色，用法同上。可选的 `outline` 参数是矩形轮廓的颜色。
- **椭圆**：`ellipse(xy, fill, outline)` 方法绘制一个椭圆，`xy` 参数是一个矩形元组 `(left, top, right, bottom)` ，表示正好包含该椭圆的矩形。可选的 `fill` 参数是椭圆内的填充色。可选的 `outline` 参数是椭圆轮廓的颜色。
- **多边形**：`polygon(xy, fill, outline)` 方法绘制任意的多边形，`xy` 参数是一个元组的列表，或是一个整数列表，如 `[(x1, y1), (x2, y2), ...]` 和 `[x1, y1, x2, y2, ...]` ，表示多边形边的连接点。最后一个坐标自动连接到第一对坐标。可选的 `fill` 参数是多边形内部的颜色。可选的 `outline` 参数是多边形轮廓的颜色。

示例代码如下：
```bash
>>> from PIL import Image, ImageDraw
>>> im = Image.new('RGBA', (200, 200), 'white')
>>> draw = ImageDraw.Draw(im)
>>> draw.line([(0, 0), (199, 0), (199, 199), (0, 199), (0, 0)], fill = 'black') 
>>> draw.rectangle((20, 30, 60, 60), fill = 'blue')
>>> draw.ellipse((120, 30, 160, 60), fill = 'red') 
>>> draw.polygon([(57, 87), (79,62), (94, 85), (120, 90), (103, 113)], fill = 'brown')
>>> for i in range(100, 200, 10):
...     draw.line([(i, 0), (200, i - 100)], fill = 'green')
...
>>> im.save('drawing.png')
>>> im.show()
```
先生成 `200x200` 的白色 `Image` 图像，然后传入 `ImageDraw.Draw()` 获得 `ImageDraw` 对象，保存在 `draw` 中。然后在图像边缘画上黑色正方形轮廓；在里面画上一个蓝色的矩形，左上角是 `(20, 30)` 、右下角是 `(60, 60)` ；还有一个红色的椭圆，由 `(120, 30), (160, 60)` 的矩形来定义；一个棕色的多边形，五个顶点；还有一些绿线，用 `for` 循环绘制：
<img src="https://img-blog.csdnimg.cn/36441849028c4c64879eae3b54709535.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_Q1NETiBAbWVtY3B5MA==,size_25,color_FFFFFF,t_70,g_se,x_16" width="40%">

## 4.2 绘制文本
`ImageDraw` 对象的 `text()` 方法可以在图像上绘制文本，四个参数如下：
-  `xy` ：两个整数的元组，指定文本区域的左上角；
- `text` ：想写入的文本字符串；
- `fill` ：可选参数，文本的颜色；
- `font` ：可选参数，是一个 `ImageFont` 对象，用于设置文本的字体和大小

由于一块文本在给定字体下的大小难以预先知道，所以 `ImageDraw` 提供了 `textsize()` 方法，第一个参数是 `text` 文本字符串，第二个参数是可选的 `ImageFont` 对象，它返回一个二整数元组，表示以指定的字体写入图像后文本 `text` 的宽度和高度。利用其返回值，就可以精确计算文本放在图像上的位置。

关键是 `ImageFont` 对象，要创建这一对象，需要 `from PIL import ImageFont` ，导入后就可以调用 `ImageFont.truetype()` 函数：
- 参数一是字符串，表示字体的 `TrueType` 文件（具有 `.TTF` 文件扩展名，能在Windows的 `C:\Windows\Fonts` 中找到），即硬盘上实际的字体文件。实际上代码中并不需要输入这一路径作为字符串的一部分（非要输入也没辙），因为Python会自动在这一目录中搜索字体，找不到就报错。
以下就是字体库。需要注意的是，由于一种字体对应多种字体文件，比如粗体、斜体等等，**实际的字体文件英文名称需要选中相应字体、右击属性项才能找到**。如果只使用表面的字体名，就等着爆出 `OSError: cannot open resource` 错误吧。
<img src="https://img-blog.csdnimg.cn/68504d0643ac483cb5107ea868569b56.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_Q1NETiBAbWVtY3B5MA==,size_44,color_FFFFFF,t_70,g_se,x_16" width="60%">

- 参数二是一个整数，表示字体大小的点数（非像素！）。Pillow创建的PNG图像默认是**每英寸72像素**，**一点是 `1/72` 英寸**。

输入以下代码：

```bash
>>> from PIL import Image, ImageDraw, ImageFont
>>> import os
>>> im = Image.new('RGBA', (200, 200), 'white')
>>> draw = ImageDraw.Draw(im)
>>> draw.text((20, 150), 'Hello', fill='blue')
>>> fontsFolder = 'C:\\Windows\\Fonts'
>>> arialFont = ImageFont.truetype(os.path.join(fontsFolder, 'arial.ttf'), 32) # 使用完整路径
>>> draw.text((100, 150), 'Howdy', fill='grey', font=arialFont) # 绘制'Howdy',采用灰色、32点Arial字体
>>> im.show()
>>> arialFont = ImageFont.truetype('arial.ttf', 32) # 只使用字体文件英文名称
>>> draw.text((50, 50), 'Hey!', fill='chocolate', font=arialFont)
>>> im.show()
>>> consolaFont = ImageFont.truetype('consolaz.ttf', 32)
>>> draw.text((70, 120), 'Sunshine', fill='red', font=consolaFont)
>>> im.show()
```
最后展示的图片如下所示：
<img src="https://img-blog.csdnimg.cn/aa133e839ffa4064b3e92994fab7c904.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_Q1NETiBAbWVtY3B5MA==,size_16,color_FFFFFF,t_70,g_se,x_16" width="35%">




---
# 5. 实践项目
有几句槽不吐不快，我看的是书的中文版，可能是没有勘误过的缘故？光这一章就有好几个错字，习题的题目和答案都有错的，也是搞无语了，就该直接看英文原版的。
## 5.1 扩展和修正本章项目的程序
## 5.2 在硬盘上识别照片文件夹
## 5.3 定制的座位卡

 
