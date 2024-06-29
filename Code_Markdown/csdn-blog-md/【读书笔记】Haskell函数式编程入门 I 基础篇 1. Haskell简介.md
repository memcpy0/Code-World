@[toc]
# 1. Haskell简介
这一节太简单了，放在本文吧。本章内容只有4个：
- 介绍从Lisp诞生，到多种函数式编程语言的出现，到Haskell的出现和发展；
- 安装Haskell编译器，构建相应语言环境；
- Haskell的两种源代码文件，它们的区别；
- 编写一个HelloWorld程序

## 1.1 Haskell的由来
暂时不写，日后总结。

## 1.2 Haskell编译器的安装以及编写环境
书上写的太简略了，Haskell环境可不是那么好安装的，具体可以看本人写的[Windows10下VS Code配置Haskell语言环境](https://memcpy0.blog.csdn.net/article/details/118878150)这篇文章。

然后学习GHCi的使用方法。**GHCi是一个测试和调试的工具**，能导入Haskell源文件和库，调用其中的函数、查看数据和函数的信息等等。

在VS Code终端中，（由于我们使用的是Stack+GHC）运行 `stack exec ghci` 打开GHCi，发现出现了一个 `Prelude>` 提示符，其中 `Prelude` 指的是**GHCi在运行一个默认的初始环境**，导入了一个定义了很多类型与函数的库。我们启动GHCi后，不做任何设置就可以直接使用其中定义的内容：
```haskell
$ stack exec ghci
WARNING: GHCi invoked via 'ghci.exe' in *nix-like shells (cygwin-bash, in particular)
         doesn't handle Ctrl-C well; use the 'ghcii.sh' shell wrapper instead
GHCi, version 8.0.2: http://www.haskell.org/ghc/  :? for help
Prelude> 
```
GHCi中常用的命令有：
- `:load, :l` 导入当前路径或指定路径下的代码文件，Windows下使用转义的反斜杠。
- `:module` 导入和移除库，简写为 `:m` 。使用 `:m + <module1> <module2> ...` 和 `:m -<module1> <module2> ...` 来导入和移除不同的模块，只输入 `:m` 会移除所有导入的模块。
- `:reload, :r` 重新导入当前的源代码文件，一般适用于重新导入修改后的源文件（GHCi不会自动导入）。
- `:cd` 改变当前GHCi的路径，就不用每次输入绝对路径来导入文件了。
- `:edit` 用默认的文本编辑器编辑当前导入的文件，使用时GHCi会读取系统环境变量中的 `EDITOR` ，启动对应的编辑器。不过，这个命令不太常用。
- `:! COMMAND` 在GHCi中执行操作系统的一些命令，比如Windows下 `:! dir` 和Linux下 `:! ls` 会返回当前目录下的文件和文件夹。
- `:quit` 退出GHCi。
- `:?` 输出GHCi帮助信息。

`Prelude` 初始环境中定义了许多数值、四则运算、自然对数、三角函数、$\pi$ 相关的函数，还定义了数字的类型、布尔类型（只有 `True, False` 两个值）等等，以及非常实用的列表容器：
```haskell
Prelude> 4 + 6 * 7 / 3
18.0     
Prelude> log 2.71828
0.999999327347282
Prelude> sin (pi / 3) / cos (pi / 3)
1.7320508075688767 
Prelude> tan (pi / 3)
1.7320508075688767
Prelude> True && False
False
Prelude> True || False
True
Prelude> [1..4]
[1,2,3,4]
```
## 1.3 `.hs` 文件和 `.lhs` 文件、注释与库函数
GHCi可以解析 `.hs, .lhs` 两种文件，它们所写程序的语法完全相同。不过 `.lhs (Literate Haskell Script)` 文件用于从Haskell代码生成精美的PDF。如果 `.lhs` 文件遵守一定的个数，就可以使用 `lhs2tex` 生成 $\LaTeX$ ……本文不必深入了解这个。

不需要多行注释、生成文档时用 `.hs` 文件，里面全局函数要起头写；单行注释用 `--` 开头，多行注释用 `{- ... -}` 形式；对GHC和GHCi声明语言扩展和编译器选项/参数（GHC在Haskell标准之外定义的特性）时，需要在文件首部进行声明，以 `{-#` 开头，以 `#-}` 结尾。

Haskell的函数库相当强大，可以在[http://www.haskell.org/ghc/docs/latest/html/libraries](http://www.haskell.org/ghc/docs/latest/html/libraries)在线浏览。
## 1.4 第1个Haskell程序——HelloWorld
在个人的语言学习环境中，写第一个Haskell程序 `1.helloworld.hs` ：

```haskell
main = putStrLn "Hello, World!"
```
可见Haskell和C/C++、Java一样，**以一个 `main` 函数作为程序开始运行的入口**。

然后在VS Code终端中输入 `stack exec ghc 1.helloworld.hs` ，得到 `1.helloworld.hi, 1.helloworld.o` 文件和一个可执行的 `1.helloworld.exe` 文件，然后运行该文件即可：
```haskell
$ stack exec ghc 1.helloworld.hs
[1 of 1] Compiling Main             ( 1.helloworld.hs, 1.helloworld.o )
Linking 1.helloworld.exe ...
$ ./1.helloworld
Hello, World!
```
如果只想在命令行中运行程序、而不像进行编译，可以使用 `stack exec runghc` 或 `stack exec runhaskell` 命令，或者使用VS Code中安装的Code Runner插件的命令、还有Haskelly插件的命令：
```haskell
$ stack exec runghc 1.helloworld.hs
Hello, World!
$ stack exec runhaskell 1.helloworld.hs
Hello, World!
```
还可以进入GHCi并导入这个文件，输入 `main` 也可以运行这段程序：
```haskell
$ stack exec ghci
WARNING: GHCi invoked via 'ghci.exe' in *nix-like shells (cygwin-bash, in particular)
         doesn't handle Ctrl-C well; use the 'ghcii.sh' shell wrapper instead     
GHCi, version 8.0.2: http://www.haskell.org/ghc/  :? for help
Prelude> :l 1.helloworld 
[1 of 1] Compiling Main             ( 1.helloworld.hs, interpreted )
Ok, modules loaded: Main.
*Main> main
Hello, World!
```
因为**Haskell处理输入和输出的特殊性**（IO Monod），这本书前10章都在讲纯函数，里面的所有函数均需要在GHCi中运行，到了第11章才可以写和操作系统互动的程序！因此，学会使用Stack、GHC和GHCi在现阶段是很重要的！
