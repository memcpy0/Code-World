
@[toc]

编辑图像、CSV/Excel、Word、PDF、JSON文件，运行程序，从互联网下载文件、发送电子邮件和短信等等，诸如此类的事情都离不开Python模块的支持，只是有时候就是没有对应模块，有时候就是需要鼠标或者键盘操作……也许可以用按键精灵控制鼠标和键盘，不过有一点学习成本，Python其实也能做到这一点。
> 在计算机上自动化任务的终极工具，就是用程序直接控制键盘和鼠标点击，就像自己坐在计算机前与应用交互一样，这种技术被称为**图形用户界面自动化**……请将GUI自动化看成是对一个机械臂编程。你可以对机械臂编程，让它敲键盘或移动鼠标。**对于涉及许多无脑点击或填表的任务，这种技术特别有用**。

当然，还是离不开Python模块，此处是 `pyautogui` ，它包含了一些能模拟鼠标移动、按键和滚动鼠标滚轮、截屏、定位/移动/最小化/最大化/关闭应用窗口、展示消息框的函数，在[http://pyautogui.readthedocs.org/](http://pyautogui.readthedocs.org/)可以找到完整的文档，有时间建议仔细阅读一遍：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e6142d959906413c84854fb55db4bff2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
Windows上直接安装就好了：![在这里插入图片描述](https://img-blog.csdnimg.cn/03a76700771f4204b37a985885a37d9d.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 1. 注意事项
开始GUI自动化之前，要注意的是，编程软件移动鼠标并击键的速度太快，甚至可能导致其他程序跟不上。一旦出了问题，而GUI自动化程序仍在到处移动鼠标……这将是一场灾难。此时停止它可能很难，做不到点击程序窗口来关闭它。不过仍然有好几种方法来**防止/恢复GUI自动化问题**。
- **通过注销关闭程序**，在Window上，注销的快捷键是 `Ctrl+Alt+Del` ，不过**这将让你丢失所有未保存的工作**，只是不需要等计算机完全重启。
- **暂停防故障设置**，做法是**告诉脚本在每次函数调用后等一会儿**，这段短暂的时间里可以自行控制鼠标和键盘。需要设置 `pyautogui.PAUSE` 变量为要暂停的秒数，如 `pyautogui.PAUSE = 1.5` ，此时每个 `PyAutoGUI` 函数调用后都会等待一秒半，非 `PyAutoGUI` 函数不会停顿。
- **自动防故障设置**，`pyautogui` 模块有自动防障碍设置，**当鼠标移动到屏幕的左上角，将导致 `pyautogui.FailSafeException` 异常**。可以用 `try, except` 语句来处理，或者 `let it crash` 任其崩溃。这两种情况下，如果尽可能快向左上角移动鼠标，自动防障碍功能都将停止程序。

因此在交互式环境输入：
```python
>>> import pyautogui
>>> pyautogui.PAUSE = 1 # 每次函数调用后暂停1秒
>>> pyautogui.FAILSAFE = True # 启动自动防障碍功能
```
---
# 2. 控制鼠标移动、追踪在屏幕的位置
`pyautogui` 对坐标的处理，和计算机屏幕的坐标系统与之前提到的图像坐标系统完全一致，原点 `(0, 0` 在屏幕左上角，坐标 `(x, y)` 向右 `x` 坐标增加，向下 `y` 坐标增加。所有坐标都是正整数，没有负数坐标。**分辨率指明屏幕的宽和高有多少像素**，如是 `1920 x 1080` ，则左上角坐标是 `(0, 0)` ，右下角坐标是 `(1919, 1079)` 。
![在这里插入图片描述](https://img-blog.csdnimg.cn/5337637e018541478a1649efdf10be67.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_Q1NETiBAbWVtY3B5MA==,size_26,color_FFFFFF,t_70,g_se,x_16)
`pyautogui.size()` 返回两个整数的元组，包含屏幕宽度和高度：
```python
>>> pyautogui.size()
Size(width=1920, height=1280)
>>> width, height = pyautogui.size()
```
## 2.1 移动鼠标
`pyautogui.moveTo()` 将鼠标立即移动到指定的屏幕 `(x, y)` 坐标，此处坐标分别作为第一、二个参数传入，可选的 `duration` 关键字参数是一个数字，指定将鼠标移到目的位置需要多少秒，不指定时默认是零、即立即移动（PyAutoGUI函数中，所有的 `duration` 参数都是可选的）。

以下代码将以正方形模式顺时针移动鼠标，循环10次，每次移动耗时0.25秒：
```python
>>> for i in range(10):
...     pyautogui.moveTo(100, 100, duration=0.25)
...     pyautogui.moveTo(200, 100, duration=0.25)
...     pyautogui.moveTo(200, 200, duration=0.25)
...     pyautogui.moveTo(100, 200, duration=0.25)
```
`pyautogui.moveRel()` 函数则相对于当前位置移动鼠标，接受的3个参数分别是：向右水平移动多少像素、向下垂直移动多少像素、（可选的）花多少时间移动。以下代码同样以正方形模式移动鼠标，只是它从代码执行时鼠标所在的位置开始，按正方形移动：
```bash
>>> for i in range(10):
...     pyautogui.moveRel(100, 0, duration=0.25)
...     pyautogui.moveRel(0, 100, duration=0.25)
...     pyautogui.moveRel(-100, 0, duration=0.25)
...     pyautogui.moveRel(0, -100, duration=0.25)
```

## 2.2 获取鼠标位置
通过调用 `pyautogui.position()` 函数，可以确定鼠标当前的位置，它返回调用时鼠标 `(x, y)` 坐标的元组：

```bash
>>> pyautogui.position()
Point(x=810, y=1168)
>>> pyautogui.position()
Point(x=790, y=148)
>>> pyautogui.position()
Point(x=125, y=103)
```

---
# 3. 练手：现在鼠标在哪里
知道鼠标现在的位置在哪是很重要的，尤其是在GUI自动化程序中。

---
# 4. 控制鼠标交互
## 4.1 点击鼠标
## 4.2 拖动鼠标
## 4.3 滚动鼠标


---
# 5. 处理屏幕
## 5.1 获取屏幕快照
## 5.2 分析屏幕快照


---
# 6. 练手：扩展 `mouseNow` 程序

---
# 7. 图像识别

---
# 8. 控制键盘
## 8.1 通过键盘发送一个字符串
## 8.2 键名
## 8.3 按下和释放键盘
## 8.4 热键组合

---
# 9. 复习PyAutoGUI的函数

---
# 10. 练手：自动填表程序

---
# 11. 小结

---
# 12. 习题

---
# 13. 实践项目
## 13.1 看起来很忙
## 13.2 即时通信机器人
## 13.3 玩游戏机器人指南



