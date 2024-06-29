@[toc]


---
# 1. Stack用户指南
## 1.1 关于Stack的一点认知

稍微回顾一下Stack的功能——它能够管理工具链（包括GHC和Windows用户的MSYS）、构建和注册库、创建构建工具的依赖 `building build tool dependencies` 等等，**Stack有能力提供你所需的所有Haskell工具**，即一站式服务 `one-stop shop` 。

要构建一个项目，Stack使用项目根目录中的 `stack.yaml` 文件作为蓝图，该文件包含一个引用，也称为解析器 `resolver` ，该引用指向将用来构建你的包的快照 `snapshot` 。

Stack还是隔离的，它不会在特定的Stack目录之外进行更改。Stack生成的文件通常位于**Stack根目录** `stack root directory` 中（默认在Windows上是 `%LOCALAPPDATA%\Programs\stack` ）或者每个项目的 `./.stack-work` 目录中。其中，Stack根目录保存着属于快照 `snapshots` 的包、通过Stack安装的各种版本的GHC `any stack-installed versions of GHC` 。Stack不会篡改GHC的任何系统版本，也不会干扰Cabal-install安装的软件包或其他任何构建工具。 


## 1.2 使用Stack构建一个Hello World项目
如何开始一个项目，用到的命令如下：
```bash
stack new my-project # 创建一个新目录(新项目),包含正确启动项目所需的所有文件
cd my-project 		 # 进入该项目
stack setup			 # 在一个隔离环境中下载GHC编译器(或者直接使用全局安装的GHC)和其他相关的库,不会干扰任何系统级安装
# stack path		 # 输出有关安装路径的许多信息
stack build			 # 构建这个项目,如果之前没有使用stack setup安装编译器的话,这里会由stack自动安装对应版本的GHC
stack exec my-project-exe # 执行程序
```
安装了Stack后，我们将创建一个新的Hello World项目（从一个模板），并且遍览大多数常用的Stack命令，具体介绍以Stack为中心 `Stack-centric` 的典型工作流。

### ① `stack new`
**创建新项目的起点是 `stack new` 命令**，项目会包含一个和项目同名的Haskell包（也是一个文件夹），因此首先要选择一个有效的包名。顺带一提，`stack new` 不接受像 `Learn_Haskell` 这样的名字：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210720111932145.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
有效包名的规则如下，要求 `one or more alphanumeric words separated by hyphens` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210720234035643.png)
我们将给项目起名为 `hello-world`（相应的包名和文件夹也名为 `hello-world` ） ，然后使用 `new-template` 项目模板。 从下面的安装过程可以发现，这个Stack命令需要进行相当多的初始设置——它下载了一个名为 `new-template` 的模板并以此来创建项目 `hello-world` ，然后寻找 `.cabal` 或 `package.yaml` 文件来初始化这个项目，之后搜寻最佳快照（匹配到镜像源的页面），从中选择了解析器，初始化使用解析器的配置并写入到 `hello-world\stack.yaml` 文件中：
```bash
C:\CodeWorld\Code_Haskell>stack new hello-world new-template
Downloading template "new-template" to create project "hello-world" in hello-world\ ...
Looking for .cabal or package.yaml files to use to init the project.
Using cabal packages:
- hello-world\

Selecting the best among 20 snapshots...

* Matches https://mirrors.tuna.tsinghua.edu.cn/stackage/stackage-snapshots/lts/18/3.yaml

Selected resolver: https://mirrors.tuna.tsinghua.edu.cn/stackage/stackage-snapshots/lts/18/3.yaml
Initialising configuration using resolver: https://mirrors.tuna.tsinghua.edu.cn/stackage/stackage-snapshots/lts/18/3.yaml
Total number of user packages considered: 1
Writing configuration to file: hello-world\stack.yaml
All done.
C:\sr\templates\new-template.hsfiles:    6.06 KiB downloaded...
```
### ② `stack build`
现在我们在 `hello-world` 文件夹中有了一个项目，开始运行最重要的Stack命令 `stack build` 。为了构建项目，Stack需要一个GHC，如果Stack发现你缺少了什么，就会为你进行安装。当然，也可以使用 `stack setup` 命令手动执行此操作。**`stack build` 一旦安装了GHC，就会开始构建项目**。需要注意的是，GHC会安装在你的全局Stack根目录 `stack root directory` 中，所以在CMD中使用 `ghc` 是行不通的。
```bash
C:\CodeWorld\Code_Haskell>cd hello-world

C:\CodeWorld\Code_Haskell\hello-world>stack build
Building all executables for `hello-world' once. After a successful build of all of them, only specified executables will be rebuilt.
hello-world> configure (lib + exe)
Configuring hello-world-0.1.0.0...
hello-world> build (lib + exe)
Preprocessing library for hello-world-0.1.0.0..
Building library for hello-world-0.1.0.0..
[1 of 2] Compiling Lib
[2 of 2] Compiling Paths_hello_world
Preprocessing executable 'hello-world-exe' for hello-world-0.1.0.0..
Building executable 'hello-world-exe' for hello-world-0.1.0.0..
[1 of 2] Compiling Main
[2 of 2] Compiling Paths_hello_world
Linking .stack-work\dist\274b403a\build\hello-world-exe\hello-world-exe.exe ...
hello-world> copy/register
Installing library in C:\CodeWorld\Code_Haskell\hello-world\.stack-work\install\a604d27e\lib\x86_64-windows-ghc-8.10.4\hello-world-0.1.0.0-3yJi0xF19Dm7oeCS9zApxI
Installing executable hello-world-exe in C:\CodeWorld\Code_Haskell\hello-world\.stack-work\install\a604d27e\bin
Registering library for hello-world-0.1.0.0..
```
毫不夸张的说，**构建命令是Stack的核心和灵魂所在，是为构建代码、测试代码、获取依赖等提供动力的引擎**。使用相同的选项和参数、运行两次构建命令通常应该是无操作的（除了重新运行测试套件这样的事情之外)，并且应该在不同的运行之间产生可重复的结果。

### ③ `stack exec` 
仔细看清楚前一个命令的输出，发现它构建了一个名为 `hello-world` 的库（`Building library`）和一个名为 `hello-world-exe` 的可执行文件（`Building executable`）。下一小节中会解释这一点，要注意的是可执行文件安装在项目根目录下 `./.stack-work` 文件夹中。现在使用 `stack exec` 来执行这一程序，结果只会输出 `someFunc` ：
```bash
C:\CodeWorld\Code_Haskell\hello-world>stack exec hello-world-exe
someFunc
```
`stack exec` 的工作原理是，为**正在运行的命令**提供相同的、可重复的、用于构建项目的环境。因此，它知道在哪里可以找到 `hello-world-exe`（即使隐藏在 `./.stack-work` 目录中）。

### ④ `stack test` 
最后，`hello-world` 也有一个测试套件 `test-suite` ，执行 `stack test` ，然后查看输出，会发现Stack首先构建测试套件，然后自动为我们运行它：
```bash
C:\CodeWorld\Code_Haskell\hello-world>stack test
hello-world-0.1.0.0: unregistering (components added: test:hello-world-test)
hello-world> configure (lib + exe + test)
Configuring hello-world-0.1.0.0...
hello-world> build (lib + exe + test)
Preprocessing library for hello-world-0.1.0.0..
Building library for hello-world-0.1.0.0..
Preprocessing executable 'hello-world-exe' for hello-world-0.1.0.0..
Building executable 'hello-world-exe' for hello-world-0.1.0.0..
Preprocessing test suite 'hello-world-test' for hello-world-0.1.0.0..
Building test suite 'hello-world-test' for hello-world-0.1.0.0..
[1 of 2] Compiling Main
[2 of 2] Compiling Paths_hello_world
Linking .stack-work\dist\274b403a\build\hello-world-test\hello-world-test.exe ...
hello-world> copy/register
Installing library in C:\CodeWorld\Code_Haskell\hello-world\.stack-work\install\a604d27e\lib\x86_64-windows-ghc-8.10.4\hello-world-0.1.0.0-3yJi0xF19Dm7oeCS9zApxI
Installing executable hello-world-exe in C:\CodeWorld\Code_Haskell\hello-world\.stack-work\install\a604d27e\bin
Registering library for hello-world-0.1.0.0..
hello-world> test (suite: hello-world-test)

Test suite not yet implemented

hello-world> Test suite hello-world-test passed
Completed 2 action(s).
```
对于 `build` 和 `test` 命令，已经构建的组件不会再次构建。我们可以通过第二次运行 `stack build` 和 `stack test` 来看到这一点：
```bash
C:\CodeWorld\Code_Haskell\hello-world>stack build

C:\CodeWorld\Code_Haskell\hello-world>stack test
hello-world> test (suite: hello-world-test)

Test suite not yet implemented

hello-world> Test suite hello-world-test passed
```
## 1.3 Stack的内部工作原理
这一小节将深入剖析 `hello-world` 示例的细节。
### (1) `hello-world` 项目的文件结构
首先进一步理解项目的文件夹结构：
```bash
C:\CodeWorld\Code_Haskell\hello-world>tree
卷 Local Disk 的文件夹 PATH 列表
卷序列号为 BA58-00DD
C:.
├─.stack-work
│  ├─dist
│  │  └─274b403a
│  │      ├─build
│  │      │  ├─autogen
│  │      │  ├─hello-world-exe
│  │      │  │  ├─autogen
│  │      │  │  └─hello-world-exe-tmp
│  │      │  └─hello-world-test
│  │      │      ├─autogen
│  │      │      └─hello-world-test-tmp
│  │      ├─package.conf.inplace
│  │      └─stack-build-caches
│  │          └─894b3ee87540e92fece6dae9016be3a62c5796717db6b56d68b0355e69d3db20
│  ├─ghci
│  │  └─d0f3bd92
│  └─install
│      └─a604d27e
│          ├─bin
│          ├─doc
│          │  └─hello-world-0.1.0.0
│          ├─lib
│          │  └─x86_64-windows-ghc-8.10.4
│          │      └─hello-world-0.1.0.0-3yJi0xF19Dm7oeCS9zApxI
│          └─pkgdb
├─app
├─src
└─test
```
然后是重要的文件：
```bash
LICENSE
README.md
hello-world.cabal
package.yaml
stack.yaml
stack.yaml.lock
Setup.hs
app/Main.hs
src/Lib.hs
test/Spec.hs
```
其中 `app/Main.hs, src/Lib.hs, test/Spec.hs` 文件都是Haskell源代码文件，实现项目的实际功能；`LICENSE, README.md` 文件对构建没有影响；`hello-world.cabal` 文件作为 `stack build` 的一部分会自动更新，**不应该被修改**；`stack.yaml.lock` 由Stack自动生成，记录一些关于快照 `Snapshots` 的信息，**不应该被手动编辑修改**；有趣的文件只剩下了 `Setup.hs, stack.yaml, package.yaml` 。
- `Setup.hs` 文件是Stack使用的Cabal构建系统的一个组成部分。从技术上讲，Stack并不需要它，但在Haskell世界中，将它包含进来仍然被认为是一个很好的实践。我们使用的 `Setup.hs` 是简单的样板文件：
	```haskell
	import Distribution.Simple
	main = defaultMain
	```
- **`stack.yaml` 文件给出了我们的项目级设置** `project-level settings` ：
	- `resolver` 是第一个关键参数，它告知Stack如何构建你的包，使用哪个GHC版本，包依赖的版本等等。`stack setup` 会安装这个版本的GHC。
	- `packages` 键告诉Stack要构建哪一个本地包。由于这一简单示例中，项目只有一个包，位于相同的目录，所以 `.` 就足够了；对于多包项目 `multi-package project` ，Stack也有强大的支持，在后文中会加以说明。
	- `flags, extra-deps` 键为空值，而且被注释掉了；
- `package.yaml` ：从Stack 1.6.1开始， `package.yaml` 是首选的包格式，被Stack通过hpack工具内置地提供。**默认的行为是从 `package.yaml` 生成 `.cabal` 文件**。因此你不该修改 `.cabal` 文件。
由于Stack也是建立在Cabal构建系统之上的，了解Cabal的活动部分是必要的。在Cabal中，我们会有独立的包，每个包含有一个单独的 `.cabal` 文件，`.cabal` 文件能定义一个或多个 `components` ：库、可执行文件、测试套装、基准测试，它还指定了额外的信息，例如库依赖关系、默认的语言语法等等。
### (2) `stack setup` 命令
正如上面看到的，`stack build` 为我们自动安装了GHC，现在让我们手动执行 `stack setup` 命令。值得庆幸的是，该命令足够智能，知道不需要也不应该执行两次安装，下面的命令输出中还说可以使用 `stack path` 获取更多的路径信息：
```bash
C:\CodeWorld\Code_Haskell\hello-world>stack setup
stack will use a sandboxed GHC it installed
For more information on paths, see 'stack path' and 'stack exec env'
To use this GHC and packages outside of a project, consider using:
stack ghc, stack ghci, stack runghc, or stack exec
```
现在来查看GHC到底安装在哪里。从下面输出的路径中所看到的（和前文强调的）是，安装的位置是为了不干扰任何其他GHC安装，无论是系统范围内的，还是通过Stack安装的不同版本GHC：
```bash
C:\CodeWorld\Code_Haskell\hello-world>stack exec -- which ghc # 搜索命令存放位置
/c/Users/21839/AppData/Local/Programs/stack/x86_64-windows/ghc-8.10.4/bin/ghc
C:\CodeWorld\Code_Haskell\hello-world>stack exec -- where ghc # 搜索二进制程序名
C:\Users\21839\AppData\Local\Programs\stack\x86_64-windows\ghc-8.10.4\bin\ghc.exe
```

---
## 1.4 清理你的项目
我们可以使用 `stack clean, stack purge` 命令，清理项目的构建工作。
- `stack clean` 会删除包含编译器输出的本地工作目录 `local working directories` ，默认的是 `.stack-work/dist` 文件夹的内容，而且是一个项目中的所有 `.stack-work` 目录。使用 `stack clean <specific-package>` 只删除特定包的输出。
- `stack purge` 会删除本地Stack工作目录 `local stack working directories` ，包括 `extra-deps, git dependencies, compiler output(including logs)` 。它不会删除任何快照包 `snapshot packages` 、编译器使用 `stack install` 安装的程序。本质上是将项目还原为一个 `completely fresh state` ，好像从未构建过一样。实质上，**`stack purge` 是 `stack clean --full` 的快捷命令**。

---
## 1.5 添加依赖
[](http://blog.remi.moe/tags/stack/)……

---
目前Stack最常用的命令就这几个：
- `stack new` 创建一个工程的脚手架。生成两个配置在项目目录下，一个是 `stack.yaml` ，里面可以配置LTS等项目设置；另一个是 `package.yaml` ，里面的 `dependencies` 填写工程需要安装的包。文件中 `import` 后面跟的名字与这里添加的包名是不同的，`import Data.Aeson` 但包名叫做 `aeson` 。
- `stack build` 编译你的工程。
- `stack exec xxx-exe` 运行编译出的可执行程序。 `xxx-exe` 是目标可执行文件的默认名字，可以在 `stack.yaml` 中配置。
 - `stack install` 这个命令看上去像是安装一个包的常用命令，其实并不是真正的安装。在工程中需要使用某个包的可执行程序，那么用 `stack build` 即可，使用时用 `stack exec` 即可执行。
    而 `stack install` 仅仅是将包拷贝到全局可访问的路径中，让你在工程以外也能使用它。因此在不同的工程下和不同的LTS中，安装出来的包版本也有差异？

# 2. 工作中……
我对Stack的使用还有很多认知不到位的地方……
![在这里插入图片描述](https://img-blog.csdnimg.cn/8b662d2fe1af4e119c1938a0d93def83.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/8faf21195b94412aab09fd74b0cf2edf.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/0fd76360d9f34c3b9527f07b6f5276b8.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/9cded92536a04279a91cfaf7f014d813.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

