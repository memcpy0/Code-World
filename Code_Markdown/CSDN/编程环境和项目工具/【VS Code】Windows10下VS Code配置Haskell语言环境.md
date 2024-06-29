@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。

Haskell好用的IDE很少，连官网Wiki都说 `The IDE world in Haskell is incomplete, but is in motion`。于是我打起了VS Code的主意。还有一个很重要的原因是，VS Code上面有几个好用的Haskell扩展和工具，比如 `Haskero` 、`Haskelly` 、`Haskell Language Server` 等等。不过由于Haskell相关的中文资料比较少，网上的教程多是几年前的，配置Haskell语言学习环境会比较麻烦……~~不，是相当地麻烦，配置过程中我连杀人的❤都有了~~。

不管怎样，[获取了VS Code编辑器之后](https://memcpy0.blog.csdn.net/article/details/117640795)，我们将在VS Code中配置Haskell的语言学习环境。同样的，我会在个人代码库 `CodeWorld` 中建立 `Code_Haskell` 文件夹，里面是文件夹 `Haskell_Single` 和 `Haskell_Multiple` ，分别存放不同的分类代码文件夹，即程序运行目标分别是**单个源代码文件**或**少数几个源代码文件的组合**。

---
# 0. Haskell官方资料
Haskell的官网是：[https://www.haskell.org/](https://www.haskell.org/)：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021071815595867.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
Haskell的Wiki：[https://wiki.haskell.org/Haskell](https://wiki.haskell.org/Haskell)，一个网页版的Haskell交互式命令行：[https://tryhaskell.org/](https://tryhaskell.org/)，Haskell的安装文档：[https://www.haskell.org/downloads/](https://www.haskell.org/downloads/)。

从安装文档中可以看到，要想完成Haskell工具链的配置，就需要有以下工具：
- GHC `the Glasgow Haskell Compiler` ：格拉斯哥Haskell编译器；
- Cabal-install `the Cabal installation tool` ：用于管理Haskell软件包的Cabal安装工具；
- Stack：一个用于开发Haskell项目的跨平台程序；
- Haskell Language Server(HLS)：可选，`A language server for developers to integrate with their editor/IDE` 

不过安装文档会让Windows下的用户比如我们，先安装一个名叫 `Chocolatey` 的软件，然后通过这个软件来安装GHC和Cabal-install。关于这个巧克力味ψ(｀∇´)ψ软件的用法，见[这篇文章](https://memcpy0.blog.csdn.net/article/details/118879435)。一言以概之，这就是Windows下的yum或apt-get，是一个集中打包和分发软件的包管理工具，使用它可以快速下载安装、管理、卸载软件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718162513723.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
下载这个软件有没有必要呢？如果你只是想完成Haskell语言环境的安装，不想节外生枝，**可以不下**。如果爱折腾，想要用Chocolatey来管理软件包，可以下载下来玩一下，**但是不要按照这个步骤安装Haskell环境**！

而且一个很重要、但是少有相关安装类文章说明的区别是，Cabal和Cable-install是不同的！Cabal是 `The Common Architecture for Building Applications and Libraries` 即用于构建应用程序和库的通用架构，是一个构建系统，仅仅参与包的创建和应用构建，**并不实际管理软件包**。Cabel-install则负责**获取、配置、编译和安装**Cabal构建的Haskell包，而且**它不是一个全功能的包管理器**，它不能安装非Cabal打包的依赖、不能卸载包、也不能自动升级安装包。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021072021491460.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/2021072021492054.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
听起来Cabal-install不是那么不可或缺啊。确实，Stack就是一个可以替代Cabal-install的工具，而且Stack也是建立在Cabal构建系统之上的（`stack is built on top of the Cabal build system`）。我们将在后文看到这一点。

因此，本文中我们直接安装和使用Stack，然后通过 `stack new` 创建新项目，并在 `stack build` 编译该项目时，由Stack自动安装对应版本的GHC编译器（或者提前执行 `stack setup` 命令），最后使用VS Code和相关插件来书写Haskell程序。

不要遵循Haskell.org里面的说明，来安装Haskell Platform，原因在此，[Haskell Platform as the default recommendation considered harmful](https://mail.haskell.org/pipermail/haskell-community/2015-September/000014.html)。太长不看版：Haskell中很多基础的包都不在 `Prelude` 里面，都需要自己下载，而且经常出现某个库觉得你的GHC版本太新而无法完成安装的荒谬情况，本文中就有这种例子。使用Haskell Platform更加剧了这种风险，当全局安装的软件包过时后，更多的冲突将爆发，而新手完全没有能力应对和解决。不像Stack，我们可以随时降级或升级到其他GHC版本，区分使用全局和局部安装的软件包。
 
---
# 1. 下载安装Stack
**共通的第一步已经完成，即获取VS Code编辑器**。对症下药，要配置Haskell语言学习环境，第二步就是下载Stack。我们打开[Stack官方文档](https://docs.haskellstack.org/en/stable/README/)，建议仔细阅读一遍：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718155441874.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

Stack拥有丰富的特性，提供了更加便利的管理功能：
- 自动在隔离环境/沙盒环境 `isolated location` 中安装和管理GHC `The Glasgow Haskell Compiler` ；
- 支持对不同的GHC发行版本进行本地管理，通过配置文件选择和使用不同版本的GHC；
- 提供沙盒机制来进行隔离，从而使得不同版本的应用之间不会冲突；
- 安装和管理项目所需要的Haskell包，对包的依赖和不同版本的包，均提供了不错的支持；
- 构建、调试、基准测试 `benchmark` 你的项目；
- 具有可重复构建 `reproducible builds` 的特性，可以多次运行 `stack build` 并得到同样的结果（关于这一概念见[这一网站](https://reproducible-builds.org/)）

在[下载页面](https://docs.haskellstack.org/en/stable/install_and_upgrade/#windows)中点击下载Windows 64-bit Installer，然后开始安装，为了安装到C盘自定义路径，我使用了管理员身份（？记不清了）运行安装程序：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021071819270497.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
安装过程中，还会自动将Stack添加到用户环境变量 `PATH` 中，同时会设置 `STACK_ROOT=C:\sr`（安装程序的默认选项）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210718192733944.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
在命令行中，运行一下 `stack --help` 查看Stack的选项和命令，好杂乱的 `options` 啊：
```bash
C:\CodeWorld\Code_Haskell>stack --version
Version 2.7.1, Git revision 8afe0c2932716b0441cf4440d6942c59568b6b19 x86_64 hpack-0.34.4

C:\CodeWorld\Code_Haskell>stack --help
stack - The Haskell Tool Stack

Usage: stack [--help] [--version] [--numeric-version] [--hpack-numeric-version]
             [--docker*] [--nix*]
             [--verbosity VERBOSITY | (-v|--verbose) | --silent]
             [--[no-]time-in-log] [--stack-root STACK-ROOT]
             [--work-dir WORK-DIR] [--[no-]system-ghc] [--[no-]install-ghc]
             [--arch ARCH] [--ghc-variant VARIANT] [--ghc-build BUILD]
             [-j|--jobs JOBS] [--extra-include-dirs DIR] [--extra-lib-dirs DIR]
             [--custom-preprocessor-extensions EXT] [--with-gcc PATH-TO-GCC]
             [--with-hpack HPACK] [--[no-]skip-ghc-check] [--[no-]skip-msys]
             [--local-bin-path DIR] [--setup-info-yaml URL]
             [--[no-]modify-code-page] [--[no-]allow-different-user]
             [--[no-]dump-logs] [--color|--colour WHEN]
             [--snapshot-location-base URL] [--resolver RESOLVER]
             [--compiler COMPILER] [--[no-]terminal]
             [--stack-colors|--stack-colours STYLES] [--terminal-width INT]
             [--stack-yaml STACK-YAML] [--lock-file ARG] COMMAND|FILE

Available options:
  --help                   Show this help text
  --version                Show version
  --numeric-version        Show only version number
  --hpack-numeric-version  Show only hpack's version number
  --docker*                Run 'stack --docker-help' for details
  --nix*                   Run 'stack --nix-help' for details
  --verbosity VERBOSITY    Verbosity: silent, error, warn, info, debug
  -v,--verbose             Enable verbose mode: verbosity level "debug"
  --silent                 Enable silent mode: verbosity level "silent"
  --[no-]time-in-log       Enable/disable inclusion of timings in logs, for the
                           purposes of using diff with logs (default: enabled)
  --stack-root STACK-ROOT  Absolute path to the global stack root directory
                           (Overrides any STACK_ROOT environment variable)
  --work-dir WORK-DIR      Relative path of work directory (Overrides any
                           STACK_WORK environment variable, default is
                           '.stack-work')
  --[no-]system-ghc        Enable/disable using the system installed GHC (on the
                           PATH) if it is available and its version matches.
                           Disabled by default.
  --[no-]install-ghc       Enable/disable downloading and installing GHC if
                           necessary (can be done manually with stack setup)
                           (default: enabled)
  --arch ARCH              System architecture, e.g. i386, x86_64
  --ghc-variant VARIANT    Specialized GHC variant, e.g. integersimple
                           (incompatible with --system-ghc)
  --ghc-build BUILD        Specialized GHC build, e.g. 'gmp4' or 'standard'
                           (usually auto-detected)
  -j,--jobs JOBS           Number of concurrent jobs to run
  --extra-include-dirs DIR Extra directories to check for C header files
  --extra-lib-dirs DIR     Extra directories to check for libraries
  --custom-preprocessor-extensions EXT
                           Extensions used for custom preprocessors
  --with-gcc PATH-TO-GCC   Use gcc found at PATH-TO-GCC
  --with-hpack HPACK       Use HPACK executable (overrides bundled Hpack)
  --[no-]skip-ghc-check    Enable/disable skipping the GHC version and
                           architecture check (default: disabled)
  --[no-]skip-msys         Enable/disable skipping the local MSYS installation
                           (Windows only) (default: disabled)
  --local-bin-path DIR     Install binaries to DIR
  --setup-info-yaml URL    Alternate URL or relative / absolute path for stack
                           dependencies
  --[no-]modify-code-page  Enable/disable setting the codepage to support UTF-8
                           (Windows only) (default: enabled)
  --[no-]allow-different-user
                           Enable/disable permission for users other than the
                           owner of the stack root directory to use a stack
                           installation (POSIX only) (default: true inside
                           Docker, otherwise false)
  --[no-]dump-logs         Enable/disable dump the build output logs for local
                           packages to the console (default: dump warning logs)
  --color,--colour WHEN    Specify when to use color in output; WHEN is
                           'always', 'never', or 'auto'. On Windows versions
                           before Windows 10, for terminals that do not support
                           color codes, the default is 'never'; color may work
                           on terminals that support color codes
  --snapshot-location-base URL
                           The base location of LTS/Nightly snapshots
  --resolver RESOLVER      Override resolver in project file
  --compiler COMPILER      Use the specified compiler
  --[no-]terminal          Enable/disable overriding terminal detection in the
                           case of running in a false terminal
  --stack-colors,--stack-colours STYLES
                           Specify stack's output styles; STYLES is a
                           colon-delimited sequence of key=value, where 'key' is
                           a style name and 'value' is a semicolon-delimited
                           list of 'ANSI' SGR (Select Graphic Rendition) control
                           codes (in decimal). Use 'stack ls stack-colors
                           --basic' to see the current sequence. In shells where
                           a semicolon is a command separator, enclose STYLES in
                           quotes.
  --terminal-width INT     Specify the width of the terminal, used for
                           pretty-print messages
  --stack-yaml STACK-YAML  Override project stack.yaml file (overrides any
                           STACK_YAML environment variable)
  --lock-file ARG          Specify how to interact with lock files. Default:
                           read/write. If resolver is overridden: read-only

Available commands:
  build                    Build the package(s) in this directory/configuration
  install                  Shortcut for 'build --copy-bins'
  uninstall                DEPRECATED: This command performs no actions, and is
                           present for documentation only
  test                     Shortcut for 'build --test'
  bench                    Shortcut for 'build --bench'
  haddock                  Shortcut for 'build --haddock'
  new                      Create a new project from a template. Run `stack
                           templates' to see available templates. Note: you can
                           also specify a local file or a remote URL as a
                           template.
  templates                Show how to find templates available for `stack new'.
                           `stack new' can accept a template from a remote
                           repository (default: github), local file or remote
                           URL. Note: this downloads the help file.
  init                     Create stack project config from cabal or hpack
                           package specifications
  setup                    Get the appropriate GHC for your project
  path                     Print out handy path information
  ls                       List command. (Supports snapshots, dependencies and
                           stack's styles)
  unpack                   Unpack one or more packages locally
  update                   Update the package index
  upgrade                  Upgrade to the latest stack
  upload                   Upload a package to Hackage
  sdist                    Create source distribution tarballs
  dot                      Visualize your project's dependency graph using
                           Graphviz dot
  ghc                      Run ghc
  hoogle                   Run hoogle, the Haskell API search engine. Use the
                           '-- ARGUMENT(S)' syntax to pass Hoogle arguments,
                           e.g. stack hoogle -- --count=20, or stack hoogle --
                           server --local.
  exec                     Execute a command. If the command is absent, the
                           first of any arguments is taken as the command.
  run                      Build and run an executable. Defaults to the first
                           available executable if none is provided as the first
                           argument.
  ghci                     Run ghci in the context of package(s) (experimental)
  repl                     Run ghci in the context of package(s) (experimental)
                           (alias for 'ghci')
  runghc                   Run runghc
  runhaskell               Run runghc (alias for 'runghc')
  script                   Run a Stack Script
  eval                     Evaluate some haskell code inline. Shortcut for
                           'stack exec ghc -- -e CODE'
  clean                    Delete build artefacts for the project packages.
  purge                    Delete the project stack working directories
                           (.stack-work by default). Shortcut for 'stack clean
                           --full'
  query                    Query general build information (experimental)
  list                     List package id's in snapshot (experimental)
  ide                      IDE-specific commands
  docker                   Subcommands specific to Docker use
  config                   Subcommands for accessing and modifying configuration
                           values
  hpc                      Subcommands specific to Haskell Program Coverage

stack's documentation is available at https://docs.haskellstack.org/
```

---
# 2. 安装GHC前的准备活动
Stack可以用于管理Haskell编译器GHC和Haskell库，我们将使用Stack安装GHC。这一步也是本文最麻烦的一步，迈过了这道坎，后面似乎就是一片坦途。

由于Haskell的生态和版本支持不是很好，如果你直接安装而不固定版本号，软件包的安装就容易引起冲突，导致各种依赖项间的不兼容。访问[这个网站](https://www.stackage.org/)，查看Haskell最新的LTS，然后锁定在这个LTS上。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210719221830752.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
可能还有些概念需要解释，比如 `Hackage, Stackage, LTS` 等等：
- **Hackage**是Haskell社区的开源软件包仓库 `The Haskell Package Repository`  ，可以发布库和程序，或者使用Cabal-install这样的软件包管理器下载和安装软件包；
- **Stackage**是 `Stable Haskell package sets` 的缩写，即稳定一致的Haskell包集合。它是一个Haskell软件包子集的发行版，来自**Hackage**并和它一同被社区构建起来。**Stackage**存在的意义在于，它提供了兼容一致且稳定的Haskell包集，每个包都选择了一个特定版本以满足软件包集合的自洽性，从而解决包版本的冲突问题、顺利构建和运行项目；
- **Stackage**提供和维护了一堆不同的大版本，每个大版本都对应一个兼容一致稳定的Haskell包集，或者说是一个**LTS** `Long Term Service` ，被社区长期支持。我们选定了一个**LTS**之后，在下载安装时只可能发生某个包不存在的情况，就算出现冲突也能通过Stack来轻松解决，不会埋下一大堆“地雷”。

我选择的是当前最新的LTS 18.2版本，对应的GHC是8.10.4，执行命令将它设置到Stack配置中：
```bash
stack config set resolver lts-18.2
```
选择完LTS之后，我们要了解如何安装GHC、如何完成一个项目。一般来说，我们按照如下的步骤完成项目：
```bash
stack new my-project # 创建一个新目录(新项目),包含正确启动项目所需的所有文件
cd my-project 		 # 进入该项目
stack setup			 # 在一个隔离环境中下载GHC编译器(或者直接使用全局安装的GHC)和其他相关的库,不会干扰任何系统级安装
# stack path		 # 输出有关安装路径的许多信息
stack build			 # 构建这个项目,如果之前没有使用stack setup安装编译器的话,这里会由stack自动安装对应版本的GHC
stack exec my-project-exe # 执行程序
```
不过很大可能你不太想直接开启一个新项目，或者你尚未接触到这些内容，那就直接使用下列命令安装GHC……如果能够成功执行这个命令，那么本节和第3节的内容你都可以跳过去，然而一般来说事情不会这么如意：
```bash
stack --resolver lts-18.2 setup
```
下面给出的这两种方法，我都尝试过，不过一般来说选一种就可以了。

## (1) 直接下载法
这一节的内容无法审核通过，见本人的[个人博客](http://112.124.203.218/archives/4.html)。

## (2) 配置镜像源法
如果你在墙内，且不能、不会get outside of the wall或者不想为这种软件付费（可能这是国内的普遍情况），那么可以使用这种方法，配置清华的镜像源——先看完[TUNA Hackage](https://mirrors.tuna.tsinghua.edu.cn/help/hackage/)和[TUNA Stackage](https://mirrors.tuna.tsinghua.edu.cn/help/stackage/)的说明。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210719224540297.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210719224652206.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
在2021年7月，我按照上述步骤安装的Stack版本是2.7.1，应该对应 `stack >= v2.1.1` 和 `stack >= v2.5.1` 两节的说明来修改配置文件。不过TUNA中给出的配置文件路径是 `%APPDATA%\stack\config.yaml`（看起来麻烦，不过这个文件很好找，在资源管理器地址栏输入 `%APPDATA%` ，再打开子目录 `stack` 即可找到 `config.yaml` ；如果文件不存在，也可以手动创建）……然而，由于我们修改了 `STACK_ROOT` ，要修改的配置文件实际上在 `%STACK_ROOT%\config.yaml`（**也可以在资源管理器地址栏中查询**），按照默认的 `STACK_ROOT=C:\sr` ，**全局配置文件**的路径是 `C:\sr\config.yaml` ，如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210719230054751.gif)
打开这个文件，附加下面的文本并保存，用来配置镜像源：
```yaml
package-indices:
  - download-prefix: http://mirrors.tuna.tsinghua.edu.cn/hackage/
    hackage-security:
        keyids:
        - 0a5c7ea47cd1b15f01f5f51a33adda7e655bc0f0b0615baa8e271f4c3351e21d
        - 1ea9ba32c526d1cc91ab5e5bd364ec5e9e8cb67179a471872f6e26f0ae773d42
        - 280b10153a522681163658cb49f632cde3f38d768b736ddbc901d99a1a772833
        - 2a96b1889dc221c17296fcc2bb34b908ca9734376f0f361660200935916ef201
        - 2c6c3627bd6c982990239487f1abd02e08a02e6cf16edb105a8012d444d870c3
        - 51f0161b906011b52c6613376b1ae937670da69322113a246a09f807c62f6921
        - 772e9f4c7db33d251d5c6e357199c819e569d130857dc225549b40845ff0890d
        - aa315286e6ad281ad61182235533c41e806e5a787e0b6d1e7eef3f09d137d2e9
        - fe331502606802feac15e514d9b9ea83fee8b6ffef71335479a2e68d84adc6b0
        key-threshold: 3 # number of keys required

        # ignore expiration date, see https://github.com/commercialhaskell/stack/pull/4614
        ignore-expiry: no


setup-info-locations: ["http://mirrors.tuna.tsinghua.edu.cn/stackage/stack-setup.yaml"]
urls:
  latest-snapshot: http://mirrors.tuna.tsinghua.edu.cn/stackage/snapshots.json

snapshot-location-base: https://mirrors.tuna.tsinghua.edu.cn/stackage/stackage-snapshots/
```
然后不要忘记手动下载 `https://raw.githubusercontent.com/fpco/stackage-content/master/stack/global-hints.yaml` 到 `%APPDATA%\stack\pantry\global-hints-cache.yaml` ，实际的文件夹路径是 `C:\sr\pantry\global-hints-cache.yaml` 。每当第一次用到新版本GHC时，都需要更新该文件。

---
# 3. 实际安装GHC
在完成这一系列的准备工作后，不免让人心神疲惫，是时候检验我们的成果了。直接运行 `stack --resolver lts-18.2 setup` ，观摩一下安装的过程（由于我尝试过好多次，GHC包已经下载了，只是后面的安装过程失败了，所以说 `Already download` ）：
```bash
C:\CodeWorld\Code_Haskell>stack --resolver lts-18.2 setup
Preparing to install GHC to an isolated location.
This will not interfere with any system-level installation.
Already downloaded.
Downloaded 7z.dll.
Downloaded 7z.exe.
Decompressing ghc-8.10.4.tar.xz...

7-Zip 9.20  Copyright (c) 1999-2010 Igor Pavlov  2010-11-18

Processing archive: C:\Users\21839\AppData\Local\Programs\stack\x86_64-windows\ghc-8.10.4.tar.xz

Extracting  ghc-8.10.4.tar

Everything is Ok

Size:       2754344960
Compressed: 431240772
Extracting ghc-8.10.4.tar...
Extracted total of 10335 files from ghc-8.10.4.tar
GHC installed to C:\Users\21839\AppData\Local\Programs\stack\x86_64-windows\ghc-8.10.4\
msys2-20210604:    2.59 MiB / 95.20 MiB (  2.72%) downloaded...

Decompressing msys2-20210604.tar.xz...

7-Zip 9.20  Copyright (c) 1999-2010 Igor Pavlov  2010-11-18

Processing archive: C:\Users\21839\AppData\Local\Programs\stack\x86_64-windows\msys2-20210604.tar.xz

Extracting  msys2-20210604.tar

Everything is Ok

Size:       390021120
Compressed: 99822772
Extracting msys2-20210604.tar...
Extracted total of 17688 files from msys2-20210604.tar
Copying skeleton files.
These files are for the users to personalise their msys2 experience.

They will never be overwritten nor automatically updated.

'./.bashrc' -> '/home/21839/.bashrc'
'./.bash_logout' -> '/home/21839/.bash_logout'
'./.bash_profile' -> '/home/21839/.bash_profile'
'./.inputrc' -> '/home/21839/.inputrc'
'./.profile' -> '/home/21839/.profile'
[0mstack will use a sandboxed GHC it installed[0m
[0mFor more information on paths, see 'stack path' and 'stack exec env'[0m
[0mTo use this GHC and packages outside of a project, consider using:[0m
[0mstack ghc, stack ghci, stack runghc, or stack exec[0m
```
不难发现，Stack安装GHC的方法是下载一个 `ghc-8.10.4.tar.xz` 压缩包，然后下载 `7z.dll, 7z.exe` 来解压文件，最后下载一个 `msys2-20210604`（这篇文章做了更详细的区分和总结：[Cygwin、Msys、MinGW、Msys2 区别](https://www.cnblogs.com/flyinggod/p/12888060.html)）。

现在测试一下吧，看看我们到底成功安装了GHC与否。首先测试一下GHC（是8.10.4没错）和GHCi的版本：
```bash
C:\>stack exec -- ghc --version
The Glorious Glasgow Haskell Compilation System, version 8.10.4
C:\>stack exec -- ghci
GHCi, version 8.10.4: https://www.haskell.org/ghc/  :? for help
Prelude> :quit
Leaving GHCi.
```
然后是 `stack ghci, stack repl` ，这两个命令都会运行GHCi：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210719234144899.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
综上，我们已经有了一个可用的Stack和GHC。也许你发现无法直接使用 `ghc / ghci` 命令启动GHC/GHCi，要做到这一点，需要使用命令 `stack exec -- where.exe ghc` 或者 `stack path` 得到GHC的所在路径，然后将提示的路径加入到环境变量中：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210719234544749.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20210719234635821.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
不过本人**不太推荐**这样的做法。因为Stack可以管理多个版本的GHC，从环境变量直接使用GHC/GHCi可能造成冲突。为了避免冲突，我们还可以使用命令行选项来指定GHC版本，比如：
```bash
C:\CodeWorld\Code_Haskell>stack --compiler ghc-8.10.4 exec ghci
GHCi, version 8.10.4: https://www.haskell.org/ghc/  :? for help
Prelude>
```
截止目前为止，我们终于在Windows10上获得了一个可用的Haskell开发环境。但是需要注意的是，**这里安装的Haskell环境是隔离在Stack中的**，有点类似于Python的虚拟环境。接下来似乎我们要进入VS Code的配置过程了，不过在此之前，我还想先用一节讲一下Stack的具体使用。

另外也要注意的是，使用 `stack install` 命令安装的GHC和其他Haskell库，都是**全局安装** `global installation` ，安装到Stack工作根目录（默认在Windows上是 `%LOCALAPPDATA%\Programs\stack` ； 区别于 `STACK_ROOT=C:\sr` Stack全局配置目录）；而在项目文件夹中执行 `stack setup, stack build` 安装的GHC和库，是**局部安装** `local installation` ，安装到项目根目录下的  `.stack-work` 目录中。例如：
```bash
stack install intero QuickCheck stack-run  # for a global installation
stack build intero QuickCheck stack-run # for a local installation
```

---
# 4. Stack的具体使用
Stack的许多知识和用法，见[这篇文章](https://memcpy0.blog.csdn.net/article/details/118967134)，不过现在我还没有写完，可能要写蛮久。



---
# 5. 安装低版本的GHC、在不同版本GHC间切换
本应该进入VS Code的配置过程，但是在下载Stack端的插件时，发现有些插件会出现版本冲突，比如安装 `ghc-mod` 就会报错。我学编程这么久遇到的错误，唯独Haskell这么多ORZ……属实蚌埠住了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210720123046458.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

除了 `ghc-mod` 外还有几个插件会出现版本冲突……毕竟我下载的是LTS 18.2版，我看到不少文章用的是LTS 9，四年前的版本了，好像没出现冲突。因此这里执行如下命令，下载一个低版本的GHC 8.0.2:
```bash
C:\Users\21839>stack --resolver lts-9.21 setup
Preparing to install GHC to an isolated location.
This will not interfere with any system-level installation.
Downloaded ghc-8.0.2.
Already downloaded.
Already downloaded.
Decompressing ghc-8.0.2.tar.xz...

7-Zip 9.20  Copyright (c) 1999-2010 Igor Pavlov  2010-11-18

Processing archive: C:\Users\21839\AppData\Local\Programs\stack\x86_64-windows\ghc-8.0.2.tar.xz

Extracting  ghc-8.0.2.tar

Everything is Ok

Size:       1435248640
Compressed: 152282428
Extracting ghc-8.0.2.tar...
Extracted total of 9239 files from ghc-8.0.2.tar
GHC installed to C:\Users\21839\AppData\Local\Programs\stack\x86_64-windows\ghc-8.0.2\
stack will use a sandboxed GHC it installed
For more information on paths, see 'stack path' and 'stack exec env'
To use this GHC and packages outside of a project, consider using:
stack ghc, stack ghci, stack runghc, or stack exec
```
现在同样进行测试，看看成功安装了GHC 8.0.2与否（指定编译器版本，执行命令 `ghc --version` ）：
```bash
C:\Users\21839>stack --compiler ghc-8.0.2 exec -- ghc --version
The Glorious Glasgow Haskell Compilation System, version 8.0.2
C:\Users\21839>stack --compiler ghc-8.0.2 exec ghci
GHCi, version 8.0.2: http://www.haskell.org/ghc/  :? for help
Prelude> :q
Leaving GHCi.
C:\Users\21839>stack --compiler ghc-8.0.2 exec -- ghci
GHCi, version 8.0.2: http://www.haskell.org/ghc/  :? for help
Prelude> :q
Leaving GHCi.

C:\Users\21839>stack exec ghci
GHCi, version 8.10.4: https://www.haskell.org/ghc/  :? for help
Prelude> :q
Leaving GHCi.
C:\Users\21839>stack exec -- ghci
GHCi, version 8.10.4: https://www.haskell.org/ghc/  :? for help
Prelude> :q
Leaving GHCi.
```
如果要从LTS 18.2、GHC 8.10.4切换到LTS 9.21、GHC 8.0.2，只需要执行 `stack config set resolver` 命令，这将修改**全局工程配置文件** `C:\sr\global-project\stack.yaml` 中的LTS版本：
```bash
C:\Users\21839>stack config set resolver lts-9.21
C:\sr\global-project\stack.yaml has been updated.

C:\Users\21839>stack exec -- ghci
GHCi, version 8.0.2: http://www.haskell.org/ghc/  :? for help
Prelude> :q
Leaving GHCi.
```

---
# 6. 安装Haskell扩展插件（VS Code和Stack）
安装VS Code的Haskell扩展时，我们需要同时在VS Code和Stack两端安装依赖项，才能使二者协调工作。安装之前顺带一提，有很多文章都介绍了 `Haskell ghc-mod` 这个插件，为了配合LTS 9.21和GHC 8.0.2，我会使用这个插件。但是后来的读者要注意，`ghc-mod` 的作者说，他去搞 `haskell-ide-engine` 了，不再支持和维护这个插件，日后这个插件不一定可用：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210720123234695.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
我们要安装的插件有：
- `Haskell ghc-mod` 和 `ghc-mod` ：类型查看等功能；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210720100925954.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- `Haskell-linter` 和 `hlint` ：代码语法检查器；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210720101025432.png)
-  `Haskell Syntax Highlighting` ：提供语法高亮等功能：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210720100950761.png)
- `Haskelly` 和 `intero, QuickCheck, stack-run` ：提供**快速检查、集成REPL、构建、测试和运行**等功能；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210720101129556.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
-  `Haskell` ：VS Code 中的 `Haskell` 插件自带 `Haskell Language Server` 的binary版本（`automatically download and install haskell-language-server for you`），能提供相当完善的Haskell语言支持
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210720100815505.png)
我们看一下[Haskell Language Server](https://github.com/haskell/haskell-language-server#prerequisites)的GitHub页面，它还集成了 `ghcide` 和 `haskell-ide-engine` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210720124834423.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

在VS Code中安装完上述插件后，根据扩展插件的说明，执行如下命令：
```bash
C:\Users\21839>stack --resolver lts-9.21 install ghc-mod
C:\Users\21839>stack --resolver lts-9.21 install hlint
C:\Users\21839>stack --resolver lts-9.21 install intero QuickCheck stack-run
```
在执行过程中，又又又报错了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210721200739325.png)
查了查问题，发现使用GHC编译的很多程序，需要配置支持UTF-8的语言环境，否则遇到非ASCII字符时，它们将因此类错误而失败。要解决此问题，可以在CMD窗口运行 `chcp 65001` ，临时修改当前CMD为UTF-8编码格式，再执行安装。要永久修改编码格式，可以自己百度，不过感觉这样做波及太大了……

之后要安装的是 `Code Runner` 扩展插件，这一插件主要用来运行代码片段和单个代码文件。平心而论，我一开始是不愿意装这个插件的，宁可每一种语言都安装对应的插件，都在用户配置或者语言学习环境中 `.vscode` 文件夹下的项目配置里面书写相应的JSON配置代码，也不想下一个什么都能干的插件，避免插件冲突是一部分原因，也有一部分原因是想要维持配置代码的互不干扰、干干净净……只不过为了打造一个舒适的Haskell语言学习环境，最后我还是真香了。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021072121515748.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 7. 配置Haskell语言学习环境
经过一段时间的等待后，我们完成了安装。观察安装输出可以发现，Stack安装的所有扩展应该都在这个路径：`C:\Users\username\AppData\Roaming\local\bin` 中，它还提示我们这一路径 `not found on the PATH environment variable` 。嘛，无所谓，只要我们能够通过Stack使用这些扩展就行了。

漫长的路途至此为止，终于可以大松一口气了，先在VS Code中创建对应的文件夹结构，然后进行相关配置。依照配置C/C++环境时的惯例，我进入了文件夹 `CodeWorld/Code_Haskell` ，然后创建两个文件夹 `Haskell_Single, Haskell_Multiple` ，`Haskell_Single` 中的每个**分类文件夹**里面都是单个可以运行的Haskell源文件，`Haskell_Multiple` 中的每个**分类文件夹**里面都是一组小的项目文件夹。 

## 7.1 配置 `ghc-mod` 和 `hlint` 
遵循插件的说明，我们打开用户配置 `settings.json` ，加入下面两行代码：
```json
{ 
    /* Haskell ghc-mod配置 */ 
    "haskell.ghcMod.executablePath": "stack",
    /* Haskell-linter配置 */
	"haskell.hlint.executablePath": "C:\\Users\\username\\AppData\\Roaming\\local\\bin\\hlint.exe", 
}
```
## 7.2 配置 `Haskell` 
由于使用的是Stack，下列配置项要么添加到全局配置文件 `$STACK_ROOT\config.yaml` 中，要么添加到项目配置文件 `stack.yaml` 中：
```yaml
ghc-options:
  '$everything': -haddock # 一个从带注释的Haskell源代码自动生成文档的工具
```
## 7.3 配置 `Code Runner` 
在用户配置 `settings.json` 中输入以下代码……我可能也只会用它运行Haskell：
```json
{
	"code-runner.executorMap": {
		"haskell": "cd $dir && stack runhaskell $fileName"
	},
}
```
现在点击右上的小三角形就可以运行Haskell文件了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210721220059318.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 7.4 如何使用 `Haskelly`
除了 `Code Runner` 外，不要忘记我们下载过 `Haskelly` ，如果说 `Code Runner` 可以用于运行单文件，那么 `Haskelly` 就可以和Stack配合来构建、运行和测试一个项目：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210721221059996.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
在底部的工具栏看到这些按钮：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210721221124914.png)
现在完全可以在VS Code中编写Haskell项目了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210721221437998.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
 
---
# 8. 一段日子后……
## 8.1 实际使用体验和可改造之处

上面的配置我用了一段时间后，发现有点问题，或者说，是我对Stack和某些插件的理解和使用有一点问题，总之就是用着不太爽快：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c99381cc6b354d058d3500c781a07585.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
首先，我卸载了 `Haskell GHC-mod` ，它只适用于低LTS版本的GHC，比如之前的LTS 9.21 GHC 8.0.2，如果使用这个插件，就无法使用Haskell插件的HLS，这也是为什么前文的配置不起作用。两相权衡之下，我毫不犹豫卸载前者，并且更新到了最新的LTS版本，现在在VS Code中就可以看到如下的信息，证明我们下载了 `Haskell Language Server` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/5ccd3fb07d3c43cd911b6be2f14390a2.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/4cb6247a18b94ae19b4193a3bfa77955.png)

再者的是 `Haskelly` ，这个插件不能说没用，它有代码补全、许多快捷按钮 `GHCi, Run file, QuickCheck, Stack build, Stack run, Stack test` 、自定义代码片段、重用终端功能：
![在这里插入图片描述](https://img-blog.csdnimg.cn/9b8b221b51a146f48af9f89e5a6a68f5.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
只是现在，为了使用HLS、以及追逐Haskell的最新版本，我更新到了LTS 18.6 GHC 8.10.4，（前文中在LTS 18.2版本中也）无法下载安装 `Haskelly` 要求的几个插件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/66ee5007e7194d3f884e93b119a058ea.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/436469b2bdbc40c980d7d80626180413.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
虽然可以按照建议进行规避，但是仔细一想，`Haskelly` 插件的功能也不是多么特殊，我就禁用了 `Haskelly` 插件。另外还有 `haskell-linter` 插件，和 `Haskell` 插件功能重合了，没必要安装，直接禁用或卸载。

综上所述，目前有两种使用VS Code+Haskell的方式，任选其一即可：
1. 遵照前文我的安装设置，当然要卸载Haskell插件，使用 `Haskell GHC-mod` 提供类型查看、悬停文档等功能， `Haskell Syntax Highlighting` 提供语法高亮，`haskell-linter` 提供实时检查等功能，然后用 `Code Runner` 运行单个文件，或者在终端用 `stack ghci filename` 导入源代码文件来运行；用 `Haskelly` 来构建、运行和测试基于Stack的整个工程，用终端Stack命令作为补充或优化。就是LTS和GHC的版本有点老，运行在LTS 9.21 GHC 8.0.2上。
2. 直接来最新版的GHC 8.10.4，安装 `Haskell, Haskell Syntax Highlighting, Code Runner` 三个插件，同样用 `Code Runner` 运行单个文件，或者在终端用 `stack ghci filename` 导入源代码文件来运行；使用 `Haskell` 为单个文件提供类型检查、代码求值、悬停文档等功能，不过这要求GHC在环境变量PATH中，或者设置 `"haskell.serverExecutablePath": "~/.local/bin/haskell-language-server"` ；直接在终端使用Stack命令运行基于Stack的项目，此时 `Haskell` 插件只要求Stack在环境变量PATH中。


尽管目前我是可以写Haskell了，但是不代表读者也能配置成功，如果有问题，可以在评论区提出来……

## 8.2 为什么是 `Haskell` 插件
关键原因之前就提到了—— `Haskell` 插件自带 `Haskell Language Server` 的binary版本，具体使用示例可见这一视频（来源是[https://twitter.com/i/status/1286046745076670465](https://twitter.com/i/status/1286046745076670465)）：
[video(video-uvunUmkw-1629607908903)(type-bilibili)(url-https://player.bilibili.com/player.html?aid=377556752)(image-https://ss.csdn.net/p?http://i2.hdslb.com/bfs/archive/051b312c9a480aed4866dda422e31bb8bf24dcbf.jpg)(title-使用Haskell Language Server)]

 
这一插件的特性十分丰富酷炫，稍微一看就能做出决定：
- 来自GHC的**警告和错误诊断**
- **在悬停时显示类型信息和文档**
 - **代码补全**
- **跳转到定义**
- **代码求值**（Haskell Language Server），参见[官方教程](https://github.com/haskell/haskell-language-server/blob/master/plugins/hls-eval-plugin/README.md)
- **多根目录的工作区支持** `Multi-root workspace support` ：先了解[什么是多根目录工作区](https://code.visualstudio.com/docs/editor/multi-root-workspaces)，即同时在多个不同的Haskell项目中工作，它们具有不同的GHC版本或Stackage LTS版本。现在插件会为你在多根工作区中的每个工作区文件夹启动语言服务器，并且这些配置位于文件夹范围，而非全局范围 
- `Code lenses for explicit import lists` 
- 从类型签名生成函数，并使用 [Wingman (tactics)](https://github.com/haskell/haskell-language-server/tree/master/plugins/hls-tactics-plugin) 智能补全漏洞
- 文档符号 `Document symbols` 
- 突出显示文档中的引用
- 通过 `Brittany, Floskell, Fourmolu, Ormolu` 或 `Stylish Haskell` 进行格式化
- 与 [retrie](https://hackage.haskell.org/package/retrie) 集成
- 与 [hlint](https://github.com/ndmitchell/hlint) 集成以显示诊断，并通过 [apply-refact](https://github.com/mpickering/apply-refact) 应用 `hlint` 
- 用于插入或更正的模块名称建议 

另外，想了解什么是语言服务器可以看一下[这篇文章]() 施工中……。



---
# 9. 卸载重装Stack
无论是对操作系统，还是编程软件，只会下载却不会卸载和重装，是不完整的（指人生）。运行 `stack path` 命令，发现Stack主要安装在 `C:\sr` 文件夹和 `C:\Users\21839\AppData\Local\Programs\stack` 文件夹：
![在这里插入图片描述](https://img-blog.csdnimg.cn/1e1a65bb6af74756bb06fae0f50aab21.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
还有Haskell插件所在的文件夹 `C:\Users\21839\AppData\Roaming\local\bin` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/ceaa2bdc8300462eb680da54e72baf05.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
以及Stack的安装文件夹：
![在这里插入图片描述](https://img-blog.csdnimg.cn/eebfdb6f0ace45cc82067382a7b735e6.png)

全部删除，删个干干净净的，就卸载了Haskell和Stack。建议试一下（逃
