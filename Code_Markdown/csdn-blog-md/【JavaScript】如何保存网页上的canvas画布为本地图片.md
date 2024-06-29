今天遇到了一个问题，我发现网页上的一张Gif图片很不错，想保存到本地。然而鼠标右键单击发现没有这个选项，F12打开源代码发现是canvas对象。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210708123340269.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

首先打开Console，找到canvas节点：
```js
var tCanvas = document.getElementById("shadows3D_canvas");
```
由于画布canvas有一个有用的方法 `toDataURL()` ，它能够把画布中的图案转变为Base64编码的PNG图片，返回 `Data URL` 数据。 我们运行以下代码：
```js
var image = new Image();
image.src = tCanvas.toDataURL("image/png");
var url = image.src.replace(/^data:image\/[^;]/, 'data:application/octet-stream');
window.open(url);
```
最后浏览器会自动下载一个没有扩展名的文件，添加扩展名 `.png` 即可得到图片。不过这只是静态图片，要想保存GIF图片，还要下一番功夫。
