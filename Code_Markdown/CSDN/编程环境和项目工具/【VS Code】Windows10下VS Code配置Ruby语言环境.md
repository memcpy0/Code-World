@[toc]
> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。

不管怎样，[获取了VS Code编辑器之后](https://memcpy0.blog.csdn.net/article/details/117640795)，我们将在VS Code中配置Ruby的语言学习环境，目前还缺少Ruby的编译器和解释器，缺少支持插件和配置代码……不过，有关VSCode和Ruby的配合使用，我也不是太懂，慢慢学慢慢改吧……同样的，我会在个人代码库 `CodeWorld` 中建立 `Code_Ruby` 文件夹，里面的文件夹是 `Ruby_Single` 和 `Ruby_Multiple` ，分别存放不同的分类代码文件夹，即程序运行目标分别是**单个源代码文件**或**少数几个源代码文件的组合**。


---
# 0. Ruby官方文档
访问这一网站[https://ruby-doc.org/](https://ruby-doc.org/)，可以看到Ruby的核心API、标准库API文档、起步教程和其他乱七八糟的东西：
![在这里插入图片描述](https://img-blog.csdnimg.cn/ccb330f4e515465a8343344cf63ced9f.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/602b7d7bded7491796aac1a018164ed9.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
不过感觉排版不是很好，可以访问这一中文网站[http://www.ruby-lang.org/zh_cn/documentation/](http://www.ruby-lang.org/zh_cn/documentation/)作为代替/首选：
![在这里插入图片描述](https://img-blog.csdnimg.cn/5a5cc4aa6f764c9cb273852df9887910.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)



---
# 1. 下载安装Ruby
先打开Ruby中文官网[http://www.ruby-lang.org/zh_cn/downloads/](http://www.ruby-lang.org/zh_cn/downloads/)，点击红框位置：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630110819859.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
进入[https://rubyinstaller.org/](https://rubyinstaller.org/)网站：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630110950603.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

再点击 `Download` 进入下载界面，右边有详细的解释。如果不知道安装哪个版本的话，建议使用 `Ruby+Devkit 2.7.x (x64)` 安装包，它提供了最多数量的兼容gem，且和Ruby一起安装了 `MSYS2 Devkit` ，可以编译具有C扩展名的gem。仅当必须使用自定义32位本机DLL或者COM对象时，才建议使用32位x86版本。

可能你不是很了解MSYS2，但是作为一个程序员，你肯定知道MinGW，而MSYS2就集成了MinGW，同时还具有其他的一些特性，例如包管理器Pacman（用过ArchLinux的应该都知道Pacman）等等，这篇文章做了更详细的区分和总结：[Cygwin、MSYS、MinGW、MinGW64、MSYS2 区别](https://www.cnblogs.com/flyinggod/p/12888060.html)。总而言之，**MSYS2可以在Windows下搭建一个比较完美的类Linux环境**（虽然对我而言用处不是很大），包括Bash、Vim、gcc、make等工具，都可以通过包管理器来添加和卸载。  
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/2021063011122187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
我在这里选择的是最新版 `Ruby+Devkit 3.0.1-1 (x64)` 。由于官网下载太慢，很久都下载不下来，可以点击[此处](https://download.csdn.net/download/myRealization/19929682)下载我分享的安装包资源。下载完成之后，点击运行，先接受许可：
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/2021063011220378.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

自定义安装目录，然后勾选全部两项，点击 `Install` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630112533117.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

然后选择要安装的组件，默认全部勾选，点击 `Next` 即可：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630112735272.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

最后就开始了安装：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630112816349.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

前面默认选择了MSYS2，这里也勾选这个，然后点击 `Finish` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630115437485.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
出现如下界面，即使之前我在[配置C++语言环境](https://memcpy0.blog.csdn.net/article/details/117640795)时安装了MinGW-w64，还是保险起见，此处输入3：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630120105555.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
之后开始用GnuPG生成密钥，然后开始用 `pacman` 安装一系列乱七八糟的东西：
![在这里插入图片描述](https://img-blog.csdnimg.cn/353951e9644144099371c481cbf42942.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
最后出现下图时，按回车退出：
![在这里插入图片描述](https://img-blog.csdnimg.cn/10b21fd90fd5479a8fb502e57ed16c7e.png)
打开安装文件夹可以看到整套MSYS2开发环境和MinGW64工具链。MSYS2小巧玲珑，主要以基本的Linux工具为主，还有更优秀的包管理器 `pacman` ，不喜欢庞大的 `Cygwin` 的用户可以试试MSYS2。
![在这里插入图片描述](https://img-blog.csdnimg.cn/82651bc4ba3240598b6f2dc01d447a67.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
进入 `usr/bin` 目录，可以看到（随着Ruby下载安装的）MSYS2提供的包管理器 `pacman` ，要使用它需要将当前目录添加进环境变量：
![在这里插入图片描述](https://img-blog.csdnimg.cn/cbcd6cf89f3845f4b5574e932cd4173d.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
现在，打开你的命令行，输入 `ruby -v` 查看Ruby的版本号，如果成功就安装成功了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021063012144719.png)
顺便试一下Pacman：
![在这里插入图片描述](https://img-blog.csdnimg.cn/13587e5e49c64f47a24974839c99ddc5.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 2. 下载安装插件
打开VS Code，`Ctrl+Shift+X` 进入扩展界面，输入Ruby安装对应插件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630121807279.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
安装Ruby插件后，随手新建 `hello.rb` ：
```ruby
puts "Hello Ruby World!"
```
然后在VS Code的终端控制台中运行该程序：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630124559147.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

注意，如果报错说——无法将“ruby”项识别为 cmdlet、函数、脚本文件或可运行程序，可能是权限不够（为什么啊？），找到VS Ccode的 `Code.exe` ，右键选择属性，在兼容性中勾选【以管理员权限运行此程序】，然后重启VS Code即可。

---
# 3. 安装Ruby依赖

本节参考Ruby插件对应的文档：[https://github.com/rubyide/vscode-ruby/blob/main/docs/debugger.md](https://github.com/rubyide/vscode-ruby/blob/main/docs/debugger.md)。我们先要在控制台中输入下面几个命令，安装Ruby依赖：
```bash
gem install bundle
gem install bundler 
gem install debase-ruby_core_source
gem install debase
gem install ruby-debug-ide
```
一口气安装如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2b3b9f65df58489e8fbc8c7e4c90d40d.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
如果失败，可能需要修改gem的源：
```cpp
gem sources -r / --remove XXX  # 删除安装源 
gem sources -a / --add XXX	   # 添加源
```
再执行下面的两条命令，先删除原有sources 源，再添加新源，然后重新运行上述安装命令：
```cpp
gem sources --remove https://rubygems.org/
gem sources -a https://mirrors.ustc.edu.cn/rubygems/
```
执行 `rdebug-ide` 查看 `ruby-debug-ide` 的使用参数：
```bash
> rdebug-ide
Using ruby-debug-base 0.2.5.beta2
Usage: rdebug-ide is supposed to be called from RDT, NetBeans, RubyMine, or
       the IntelliJ IDEA Ruby plugin.  The command line interface to
       ruby-debug is rdebug.

Options:
    -h, --host HOST                  Host name used for remote debugging
    -p, --port PORT                  Port used for remote debugging
        --dispatcher-port PORT       Port used for multi-process debugging dispatcher
        --evaluation-timeout TIMEOUT evaluation timeout in seconds (default: 10)
        --evaluation-control         trace to_s evaluation
    -m, --memory-limit LIMIT         evaluation memory limit in mb (default: 10)
    -t, --time-limit LIMIT           evaluation time limit in milliseconds (default: 100)
        --stop                       stop when the script is loaded
    -x, --trace                      turn on line tracing
        --skip_wait_for_start        skip wait for 'start' command
    -l, --load-mode                  load mode (experimental)
    -d, --debug                      Debug self - prints information for debugging ruby-debug itself
        --xml-debug                  Debug self - sends information <message>s for debugging ruby-debug itself
    -I, --include PATH               Add PATH to $LOAD_PATH
        --attach-mode                Tells that rdebug-ide is working in attach mode
        --key-value                  Key/Value presentation of hash items
        --ignore-port                Generate another port
        --keep-frame-binding         Keep frame bindings
        --disable-int-handler        Disables interrupt signal handler
        --rubymine-protocol-extensions
                                     Enable all RubyMine-specific incompatible protocol extensions
        --catchpoint-deleted-event   Enable chatchpointDeleted event
        --value-as-nested-element    Allow to pass variable's value as nested element instead of attribute
        --socket-path PATH           Listen for debugger on the given UNIX domain socket path

Common options:
    -v, --version                    Show version

Must specify a script to run
```

如果 `gem install` 死活安装不上 `debase` 或者 `ruby-debug-ide` ，可以按照如下方法，从源码安装：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2992caf0c668410ca848d0b4c0f249cd.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/dd8dcd34336a47d197d9fb3dd076eab7.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

或者也有可能你把Ruby安装到了一个带有空格的路径下，**此时最好删除原有安装目录、删除Ruby的环境变量、重新下载RubyInstaller、重新安装Ruby**。要做到这一点，关键是抛弃**把所有编程软件安装在一个目录下**的强迫症……FxxK Ruby！！！
![在这里插入图片描述](https://img-blog.csdnimg.cn/96962d697f854c6f8bde8fb3f1e57367.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 4. 书写Ruby单文件下配置文件
如果想要添加全局配置，可以直接在**不打开文件夹**时**运行->添加配置**，然后会跳转到全局的配置文件 `settings.json` 中（关于用户配置和工作区配置，见[此处文章](https://memcpy0.blog.csdn.net/article/details/118362651)）——我的 $\LaTeX$ 配置代码，**目前**就在全局配置文件中（也许有一天我还会建立一个 `Code_LaTeX` 文件夹，然后把相关 $\TeX$ 文件都放进去，只是不一定转为工作区配置）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630125809708.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
不然就像我一样，建立一个 `CodeWorld/Code_Ruby` 文件夹，然后打开该文件夹。类似于[C/C++单文件语言学习环境的建立](https://memcpy0.blog.csdn.net/article/details/117640795)，这里先在其中创建一个名为 `Seven_Weeks_Seven_Languages` 的文件夹，用于[《七周七语言》一书的学习](https://memcpy0.blog.csdn.net/article/details/118170076)。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630210348708.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后使用**运行->添加配置**，（在打开文件夹且存在源文件时）自动创建 `.vscode` 文件夹和其中的 `launch.json` 文件。或者，可以点击下面的**创建 launch.json 文件**：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630210144411.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

VS Code会尝试自动检测你的调试环境，不过如果失败了，就需要手动选择，这里我们选择 `Ruby` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/202106302103196.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
接着需要选择生成的那一份 `configuration` 的 `name` ，这里选中 `Debug Local File` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630210749211.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

然后就生成了一份 `launch.json` 配置文件，用于Ruby的调试。现在回到文件浏览器，可以发现VS Code创建了一个 `.vscode` 文件夹，并且添加了 `launch.json` 文件到工作区。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210630211201671.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)其中 `launch.json` 文件的内容如下：
```cpp
{
    // 使用 IntelliSense 了解相关属性。 
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        { 
            "name": "Debug Local File", 
            "type": "Ruby", 
            "request": "launch", 
            "program": "${workspaceRoot}/main.rb"
        }
    ]
}
```
这只是 `configurations` 数组中的一个配置，**我们可以添加任意多个配置，只要它们的 `name` 都独一无二**。经过反复修改后，我在 `CodeWorld/Code_Ruby` 下的 `.vscode/launch.json` 文件内容如下，`configurations` 中的注释是我自己添加的：
```json
{ 
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [ 
        {
            //name是字符串形式的配置名称,显示在启动配置下拉菜单中,在配置数组中必须独一无二
            "name": "Debug Local File",
            //type是配置类型,必须是Ruby,告诉VS Code运行什么调试器
            "type": "Ruby",
            //request是请求配置类型,可以是launch(允许直接从VS Code中启动提供的程序),
            //或者是attach,允许你附加到远程调试会话(remote debug session).
            "request": "launch", 
            //cwd调试程序时的工作目录,需要是源码文件所在的文件夹位置(program字符串指出的位置)
            "cwd": "${fileDirname}",
            //program是将要进行调试的程序的完整路径,不应该依赖于相对路径.
            //如果文件在你的工作区,program通常是这样一个字符串"${workspaceRoot}/path/to/script.rb"
            //- ${workspaceRoot} 开启在VS Code中的文件夹的路径
            //- ${file} 当前打开的文件(的完整路径)
            //- ${fileBasename} 当前打开的文件的basename(即文件名.扩展名)
            //- ${fileDirname} 当前打开的文件的dirname
            //- ${fileExtname} 当前打开的文件的扩展名
            "program": "${fileDirname}/${fileBasename}",
            //stopOnEntry设为true时调试程序将暂停在程序入口处,相当于在main上打断点
            "stopOnEntry": false,
            //showDebuggerOutput展示一些额外的输出信息到调试终端,尤其是关于rdebug-ide的运行
            "showDebuggerOutput": true,
            //args数组是程序调试时提供给程序的命令行参数,里面每个字符串都被作为一个单独参数进行传送
            "args": [],
            //env提供要在启动程序之前设置的环境变量
            // "env": {
            //     "BASE": "${fileDirname}",
            //     "EXT": "${fileExtname}",
            //     "RAILS_ENV": "test"
            // },
            //如果不是'ruby',就是Ruby可执行文件的路径,用于运行程序(在没有调试器时) 
            //"pathToRuby": "ruby",
            //如果'rdebug-ide'不在PATH环境变量中,需要提供'rdebug-ide'的绝对路径值,比如"c:\ruby\rdebug-ide.bat" 
            "pathToRDebugIDE": "rdebug-ide",
            //"pathToBundler": "bundle"
        },
        {
            "name": "Minitest - current line",
            "type": "Ruby",
            "request": "launch", 
            "cwd": "${fileDirname}",
            "program": "${fileDirname}/bin/rails",
            "args": [
                "test",
                "${file}:${lineNumber}"
            ]
        },
        {   //仅对当前打开的测试文件运行Test::Unit
            "name": "Test::Unit - open test file",
            "type": "Ruby",
            "request": "launch",
            "cwd": "${fileDirname}",          
            "program": "${file}",
            "includes": ["test", "lib"] //使用includes在$LOAD_PATH中包含其他路径。
        },
        {   //在当前打开的文件中,对单个选定的测试运行Test::Unit 
            "name": "Test::Unit - single selected test",
            "type": "Ruby",
            "request": "launch",
            "cwd": "${fileDirname}",          
            "program": "${file}",
            "includes": ["test", "lib"],
            "args": [
                "-n",
                "${selectedText}"
            ]
        },
        {   //这将在源文件目录下运行rails服务器,假设binstubs已经通过bundler install --binstubs安装
            "name": "Rails server",
            "type": "Ruby",
            "request": "launch",
            "cwd": "${fileDirname}",
            "program": "${fileDirname}/bin/rails",
            "args": ["server"]
        },
        {   //使用传递过来的额外参数,运行所有的spec
            "name": "RSpec - all",
            "type": "Ruby",
            "request": "launch",
            "cwd": "${fileDirname}",
            "program": "${fileDirname}/bin/rspec",
            "args": [
                "-I", "${fileDirname}",
                "--require", "spec_helper",
                "--require", "rails_helper",
                "--format", "documentation",
            ]
        },
        {   //运行RSpec,不过只在当前打开的spec文件上
            "name": "RSpec - active spec file only",
            "type": "Ruby",
            "request": "launch",
            "cwd": "${fileDirname}",
            "program": "${fileDirname}/bin/rspec",
            "args": [
                "-I",
                "${fileDirname}",
                "${file}"
            ]
        }, 
        {   //运行所有cuke
            "name": "Cucumber",
            "type": "Ruby",
            "request": "launch",
            "cwd": "${fileDirname}",
            "program": "${fileDirname}/bin/cucumber"
        },
        // { //用于远程调试的配置,暂时用不到
        //     "name": "Listen for rdebug-ide",
        //     "type": "Ruby",
        //     "request": "attach",
        //     "remoteHost": "127.0.0.1",
        //     "remotePort": "1234",
        //     "remoteWorkspaceRoot": "${workspaceRoot}"
        // },
    ]  
}
```
---
# 5. 尝试调试Ruby代码
一份十分简单的Ruby代码：
```ruby
rec = {"name"=>"first", "label"=>"second"}
puts "#{rec}"
puts "#{rec.size()}"
puts "Hello Ruby World!"
```
在运行和调试图标下，选择 `Debug Local File` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021071419093761.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
点击这个绿色的小三角，开始调试程序： 
![](https://img-blog.csdnimg.cn/20210714191157789.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

然后前进一步：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210714191323156.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

前进到程序结束：
![](https://img-blog.csdnimg.cn/2021071419141324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

现在我们就可以运行和调试Ruby程序了！
