@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。
 
[获取了VS Code编辑器之后](https://memcpy0.blog.csdn.net/article/details/117640795)，我们将在VS Code中配置Rust的语言学习环境。同样的，我会在个人代码库 `CodeWorld` 中建立 `Code_Rust` 文件夹，里面的文件夹是 `Rust_Single` 和 `Rust_Multiple` ，分别存放不同的分类代码文件夹，即程序运行目标分别是**单个源代码文件**或**少数几个源代码文件的组合**。

---
# 0. Rust官方资料
[Rust官网](https://www.rust-lang.org/)在此：
![在这里插入图片描述](https://img-blog.csdnimg.cn/9ad70d74c3ac4ae9b84813826c9e6fa2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
[Learn页面](https://www.rust-lang.org/learn)是Rust的文档，包括标准库API、Cargo文档、RustDoc文档、Rustc文档等等，这些文档可以在本地用 `rustup doc` 命令阅读：
![在这里插入图片描述](https://img-blog.csdnimg.cn/17a07b9b7c864e42bc9670de029df8fa.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

[Playground页面](https://play.rust-lang.org/)给出了一个Rust在线学习环境：
![在这里插入图片描述](https://img-blog.csdnimg.cn/91da609cf66e4b30b6cb8447569f6629.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

[Tools页面](https://www.rust-lang.org/tools)介绍了和Rust协作的编辑器或IDE：
![在这里插入图片描述](https://img-blog.csdnimg.cn/74acd5d9d59f46ff9e4522f543f4c966.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 1. 下载安装Rust
由于Rust的编译依赖于C/C++开发环境，所以在Windows下需要先配置C++编译工具链，即安装[Microsoft Visual C++ Build Tools 2015](http://go.microsoft.com/fwlink/?LinkId=691126)。之后下载Rust。Rustup是Rust的安装器，也是一个**版本管理工具**，我们进入[Install页面](https://www.rust-lang.org/tools/install)下载Rustup：
![在这里插入图片描述](https://img-blog.csdnimg.cn/8248dd565ce84b26ae1b43bc56a2e6b9.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
突然想起来以前安装过一次Rust，现在在命令行中卸载掉它：
```shell
C:\Users\21839>rustup self uninstall

Thanks for hacking in Rust!

This will uninstall all Rust toolchains and data, and remove
%USERPROFILE%\.cargo/bin from your PATH environment variable.

Continue? (y/N) y

info: removing rustup home
info: removing cargo home
info: removing rustup binaries
info: rustup is
```
下载后运行安装程序，仔细阅读说明，发现它默认会把Rust元数据和工具链安装在 `C:\Users\用户名\.rustup` 中，把依赖库等安装在 `C:\Users\用户名\.cargo` 中，把 `cargo, rustc, rustup` 和其他命令安装在 `C:\Users\用户名\.cargo\bin` 中：
![在这里插入图片描述](https://img-blog.csdnimg.cn/19ff810f668643e8b56e653f94a227b0.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
如果不想改变安装位置的话，就可以选择1进行直接安装了。不然，就要在下载安装程序之前，提早设置好环境变量 `RUSTUP_HOME, CARGO_HOME`（如 `C:\Rust\Rustup, C:\Rust\Cargo` ） ，再把 `%CARGO_HOME%\bin` 加入到环境变量PATH中。为了**统一各个编程语言软件的安装位置**，我选择修改环境变量，接着下载和运行安装程序，如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/d995af22e5804863a17131d140da18f2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
此外，如果安装在系统盘C盘，可能需要**以管理员身份运行**安装程序。下面是安装信息，下载了Rust的编译器Rustc，还有项目管理与构建工具Cargo、格式化工具Rustfmt等等：
```bash
info: profile set to 'default'
info: default host triple is x86_64-pc-windows-msvc
info: syncing channel updates for 'stable-x86_64-pc-windows-msvc'
679.1 KiB / 679.1 KiB (100 %) 213.2 KiB/s in  3s ETA:  0s
info: latest update on 2021-07-29, rust version 1.54.0 (a178d0322 2021-07-26)
info: downloading component 'cargo'
  3.6 MiB /   3.6 MiB (100 %) 537.6 KiB/s in  7s ETA:  0s
info: downloading component 'clippy'
  1.5 MiB /   1.5 MiB (100 %) 678.8 KiB/s in  2s ETA:  0s
info: downloading component 'rust-docs'
 16.7 MiB /  16.7 MiB (100 %)   1.6 MiB/s in 14s ETA:  0s
info: downloading component 'rust-std'
 19.4 MiB /  19.4 MiB (100 %)   1.8 MiB/s in 12s ETA:  0s
info: downloading component 'rustc'
 41.1 MiB /  74.3 MiB ( 55 %)   1.2 MiB/s in  1m 38s ETA: 26s
info: retrying download for 'https://static.rust-lang.org/dist/2021-07-29/rustc-1.54.0-x86_64-pc-windows-msvc.tar.xz'
253.5 KiB /  74.3 MiB (  0 %) 143.5 KiB/s in 58s ETA:  8m 48s
info: retrying download for 'https://static.rust-lang.org/dist/2021-07-29/rustc-1.54.0-x86_64-pc-windows-msvc.tar.xz'
 54.5 MiB /  74.3 MiB ( 73 %)   1.7 MiB/s in  1m 53s ETA: 11s
info: retrying download for 'https://static.rust-lang.org/dist/2021-07-29/rustc-1.54.0-x86_64-pc-windows-msvc.tar.xz'
 74.3 MiB /  74.3 MiB (100 %)   2.2 MiB/s in 45s ETA:  0s
info: downloading component 'rustfmt'
  2.1 MiB /   2.1 MiB (100 %)   1.7 MiB/s in  1s ETA:  0s
info: installing component 'cargo'
info: installing component 'clippy'
info: installing component 'rust-docs'
 16.7 MiB /  16.7 MiB (100 %)   1.2 MiB/s in 28s ETA:  0s
info: installing component 'rust-std'
 19.4 MiB /  19.4 MiB (100 %)   4.7 MiB/s in  4s ETA:  0s
info: installing component 'rustc'
 74.3 MiB /  74.3 MiB (100 %)   5.5 MiB/s in 13s ETA:  0s
info: installing component 'rustfmt'
info: default toolchain set to 'stable-x86_64-pc-windows-msvc'

  stable-x86_64-pc-windows-msvc installed - rustc 1.54.0 (a178d0322 2021-07-26)


Rust is installed now. Great!

To get started you may need to restart your current shell.
This would reload its PATH environment variable to include
Cargo's bin directory (C:\Program Files\Rust\.cargo\bin).
```

接着在命令行中检查安装成果。如果已经正确安装了Rust，`rustup --version` 会打印Rustup的版本信息，`rustc --version` 会打印当前Rust编译器的版本信息，`cargo --version` 会打印Cargo的版本信息：
```bash
C:\Users\21839>rustup --version
rustup 1.24.3 (ce5817a94 2021-05-31)
info: This is the version for the rustup toolchain manager, not the rustc compiler.
info: The currently active `rustc` version is `rustc 1.54.0 (a178d0322 2021-07-26)`

C:\Users\21839>rustc --version
rustc 1.54.0 (a178d0322 2021-07-26)

C:\Users\21839>cargo --version
cargo 1.54.0 (5ae8d74b3 2021-06-22)

C:\Users\21839>where rustup
C:\Program Files\Rust\.cargo\bin\rustup.exe

C:\Users\21839>where rustc
C:\Program Files\Rust\.cargo\bin\rustc.exe

C:\Users\21839>rustup show # 查看已有的工具链
Default host: x86_64-pc-windows-msvc
rustup home:  C:\Program Files\Rust\.rustup

installed toolchains
--------------------

stable-x86_64-pc-windows-msvc (default)
nightly-x86_64-pc-windows-msvc

active toolchain
----------------

stable-x86_64-pc-windows-msvc (default)
rustc 1.54.0 (a178d0322 2021-07-26) 

C:\Users\21839>rustc --print sysroot 
C:\Program Files\Rust\.rustup\toolchains\stable-x86_64-pc-windows-msvc

C:\Users\21839>rustup doc # 打开离线文档
```
---
# 3. VS Code中安装支持插件
在VS Code扩展商店中输入 `rls` ，选择Rust插件进行安装：
![在这里插入图片描述](https://img-blog.csdnimg.cn/013e0b9a52c047d1aa7bca2620d447c3.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这一插件提供了很多功能，但不包括调试：
- 代码补全；
- 跳转至定义，查看定义，符号搜索等；
- 类型、文档显示；
- 代码格式化；
- 自定义代码片段；
- 构建任务；
- ……

要注意的是，这一插件由单独的一个 `language server` 赋能，要么是 `Rust Language Server (RLS)` 要么是 `rust-analyzer` ，依据用户的个人偏好来选择。
![在这里插入图片描述](https://img-blog.csdnimg.cn/647b2c206b44408c893930d0edf0fb3a.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
与这一官方插件功能相似的，还有一个名为 `rust-analyzer` 的替代插件，它使用 `rust-analyzer server` 。不过这个插件 `may cause conflicts with the official Rust extension` ，如果要安装和使用这个插件，就不要安装官方插件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/0f1efdcd72fd4476ab7d8c0279181c36.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
https://zhuanlan.zhihu.com/p/76599587
https://www.tqwba.com/x_d/jishu/279387.html


Rust语言服务器：Rust Language Server

Rust 语言服务器（RLS）基于 LSP（Language Server Protocol），即语言服务器协议，LSP 由红帽、微软和 Codenvy 联合推出，可以让不同的程序编辑器与集成开发环境（IDE）方便地嵌入各种编程语言，允许开发人员在最喜爱的工具中使用各种语言来编写程序。

它通过用于开发工具和语言服务器间通信的 JSON-RPC 标准，能够让编程工具提供实时反馈的详细信息并以此实现多种强大功能，比如符号搜寻、语法分析、代码自动补全、移至定义、描绘轮廓与重构等。Rust 语言服务器集成了这些逻辑作为后端，并通过标准的 LSP 提供给前端工具，它被设计为与前端无关，可以被不同的编辑器和 IDE 广泛采用，但目前项目团队仅在 VS Code 中进行测试。用户可以在 VS Code 扩展商店中找到基于 RLS 实现的 Rust 官方插件。

这个就是 Rust 官方为 Visual Studio Code 提供的 Rust 插件，该插件最初是 Rust 官方语言服务器（RSL）的前端参考实现，现在也支持另一个来自社区维护的 Rust 服务器 rust-analyzer（RA），用户可根据个人偏好在 RSL 或者 RA 之间选择安装一个。该插件支持：

代码补全jump to definition、peek definition、find all references 与 symbol search类型和文档悬停提示代码格式化重构错误纠正并应用建议snippets构建任务值得一提的是 snippets（代码片段）和构建任务这两个特性。

snippets 是扩展为通用样板的代码模板。输入时，Intellisense 会将代码段名称包括在内作为选项，可供快捷键选中。提供了以下 snippets：

for - 创建循环unimplementedunreachableprint(ln)assert(_eq)macro_rules - 声明宏if let Option - 在某些情况下执行 if let 语句spawn - 创建线程extern crate - 插入 extern crate 语句此 Rust 插件提供了使用相关的 cargo 命令进行构建、运行和测试的任务，可以使用 ctrl + shift + b 进行构建，可通过命令面板中的“运行任务”查看其它任务。这些任务内容写在 task.json 中，插件不会覆盖现有任务，因此可以自定义任务。

社区驱动的Rust语言服务器rust-analyzer

rust-anlyzer（RA）是除了 RSL 以外的另一个 Rust 语言服务器，由社区驱动开发。自 2019 年末发布第一个预编译 Release 以来，RA 都会在每周二稳定发布新版本并保持至今。经过快速的迭代，目前 RA 各方面的优化和提升已经超越 RSL，所以也被官方纳入 RSL 2.0 计划的一部分。Rust 的 VS Code 官方插件也已经增加了对 RA 的支持。

RA 目前已经支持 Vim、Emac、Sublime Text 3 和 VSCode 等编辑器，可通过官方文档获取详细的安装指导。

Rust代码补全程序：Racer

前面提到 RSL 和 RA 都是 Rust 语言服务器，作为 IDE 与编辑器的后端提供通用能力。而 Racer 是 Rust 语言服务器中的一个静态库，它提供了代码自动补全功能。

许多 IDE 等工具可以轻松集成这样的底层功能，目前包括以下工具都有集成 Racer 的相关实现：

EclipseEmacsGeditGnome BuilderKateSublime TextVimVS CodeAtomKakoune

Rust 提供了三个版本渠道：nightly，beta，还有stable。不稳定特性只在 Nightly Rust 有效。这个过程的更多细节，

rust 切换到nightly
 rust default nightly
执行如下命令安装工具链：可以选择其它版本(如nightly-i686-pc-windows-msvc) 
rustup toolchain install nightly-x86_64-pc-windows-gnu　　

安装源码： 
	
rustup component add rust-src --toolchain nightly

　

 

完成之后要设置如下环境变量： 
	
RUST：D:\Program Files\RUST\.rustup\toolchains\nightly-i686-pc-windows-msvc
RUST_SRC_PATH：%RUST%\lib\rustlib\src\rust\src
RUSTBINPATH：%CARGO_PATH%\bin
 
下面两个是配置科大源要用到的：
RUSTUP_DIST_SERVER：https://mirrors.ustc.edu.cn/rust-static
RUSTUP_UPDATE_ROOT：https://mirrors.ustc.edu.cn/rust-static/rustup
 
并且在path里添加如下路径：
%CARGO_HOME%\bin　　

继续安装： 
cargo +nightly install racer    // 安装 racer
 
rustup component add rls-preview --toolchain nightly
rustup component add rust-analysis --toolchain nightly

　　

2. vscode配置信息:

    下载插件：在vscode上搜索插件 rust， 然后把排名的前两个给装上，然后还可以把 rustfmt 和 vscode-rust-syntax 装上，作用分别是代码格式化和语法高亮。。。
    File/Preferences/settings/下找到setings.json文件，向其中添加如下内容：
 
	
"rust.mode": "rls",
"rust.cargoHomePath": "%CARGO_HOME%",
"rust.cargoPath":"%RUSTBINPATH%\\cargo.exe",
"rust.racerPath":"%RUSTBINPATH%\\racer.exe",
"rust.rls":"%RUSTBINPATH%\\rls.exe",
"rust.rustfmtPath":"%RUSTBINPATH%\\rustfmt.exe",
"rust.rustup":"%RUSTBINPATH%\\rustup.exe",
"rust.rustLangSrcPath": "%RUST_SRC_PATH%",
"rust.executeCargoCommandInTerminal": true,

---
# 4. 配置支持插件
## 4.1 用 `cargo new` 创建项目
`rustup` 是Rust版本管理和包管理工具，用于管理（下载安装、升级 `rustup update` 、卸载 `rustup self uninstall` 等）不同平台下的多个Rust构建版本并使其相互兼容，也用于管理Rust工具链。

而 `cargo` 是Rust的构建工具，它将常用命令集于一身，无需引入其它命令。 Cargo的文档构建器让所有API都有文档，可以通过 `cargo doc` 在本地生成文档， 还可以通过 `docs.rs` 在线获取公开的Crate文档：
- `cargo new` 可以创建项目；
- `cargo build` 可以构建项目；
- `cargo run` 可以运行项目；
- `cargo test` 可以测试项目；
- `cargo doc` 可以为项目构建文档；
- `cargo publish` 可以将库发布到 `crates.io` 


现在打开VS Code终端，输入命令创建一个Rust项目 `hello-rust` ，用以存放我们的源代码：
```shell
$ cargo new hello-rust
     Created binary (application) `hello-rust` package
```
这将生成一个 `hello-rust` 的新目录，结构如下。其中的 **`Cargo.toml` 是Cargo的清单文件，包含了项目的元数据和依赖库**，`src/main.rs` 是编写应用代码的地方：
```shell
hello-rust
|- .gitignore
|- Cargo.toml
|- src
  |- main.rs
```
然后打开 `main.rs` 文件，VSCode会提示有一些Rust组件需要安装，点击 `Yes` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/204371b56eda4d2f994668263da2193b.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
结果遇到了两个报错：
![在这里插入图片描述](https://img-blog.csdnimg.cn/27b0e72f4cd0429a8d799ca0e7981deb.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 4.2 安装工具链和源码
为了解决这一安装错误，需要手动安装工具链和源码。首先安装 `nightly` 工具链，由于使用的是 `stable-x86_64-pc-windows-msvc` ，所以执行：

```bash
C:\WINDOWS\system32>rustup toolchain install nightly-x86_64-pc-windows-msvc
info: syncing channel updates for 'nightly-x86_64-pc-windows-msvc'
431.0 KiB / 702.7 KiB ( 61 %)  16.0 KiB/s in  1m 42s ETA: 16s
error: could not download file from 'https://static.rust-lang.org/dist/channel-rust-nightly.toml' 
to 'C:\Program Files\Rust\.rustup\tmp\rbkrylf5fq0jf0k4_file.toml': error decoding response body: operation timed out: operation timed out
```
又报错了……属实给人搞无语了，比安装Haskell的时候还麻烦。
 
![在这里插入图片描述](https://img-blog.csdnimg.cn/f024af2154284a4b8f510fba0813bcfc.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
