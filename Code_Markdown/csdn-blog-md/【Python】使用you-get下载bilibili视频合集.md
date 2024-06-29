@[toc]

今天，为了一口气下载完哔哩哔哩上的某个视频合集（一百多P），我在网上四处搜索，这才发现了 `you-get` 这个有用的工具。因此想在这里做个记录，以后需要用的时候方便查看。

---

# 1. `you-get` 说明
`you-get` 是一个开源的命令行下载工具，能够下载众多网站上的多媒体资源，包括腾讯视频、哔哩哔哩等网站的视频资源，抖音上的短视频，网易云的音乐，某些网站上的图片等。

如要详细了解，请查看对应的[文档](https://github.com/soimort/you-get)和[中文Wiki](https://github.com/soimort/you-get/wiki/%E4%B8%AD%E6%96%87%E8%AF%B4%E6%98%8E)（话说Github是被墙了吗，访问怎么这么慢……）

---
# 2. `you-get` 安装
使用Python3环境和 `pip3` ，执行如下语句：
```bash
> pip install ffmpeg
Collecting ffmpeg
  Downloading ffmpeg-1.4.tar.gz (5.1 kB)
Using legacy 'setup.py install' for ffmpeg, since package 'wheel' is not installed.
Installing collected packages: ffmpeg
    Running setup.py install for ffmpeg ... done
Successfully installed ffmpeg-1.4
> pip install you-get
Collecting you-get
  Downloading you_get-0.4.1500-py3-none-any.whl (217 kB)
     |████████████████████████████████| 217 kB 469 kB/s
Installing collected packages: you-get
```
运行时遇到的一个小问题是：
```bash
> you-get -i
'you-get' 不是内部或外部命令，也不是可运行的程序
或批处理文件。
```
这不难解决，我们运行 `pip3 show you-get` 找到 `you-get` 的安装位置（包括代码和程序）：
```bash
> pip3 show you-get
Name: you-get
Version: 0.4.1500
Summary: Dumb downloader that scrapes the web
Home-page: https://you-get.org/
Author: Mort Yao
Author-email: mort.yao@gmail.com
License: MIT
Location: c:\users\21839\appdata\local\packages\pythonsoftwarefoundation.python.3.9_qbz5n2kfra8p0\localcache\local-packages\python39\site-packages
Requires:
Required-by:
```
其中的 `Location` 是 `you-get` 代码文件的位置，复制 `Location` 。然后编辑系统环境变量，进入环境变量-（用户变量）Path-编辑-新建，把刚刚复制出来的 `Location` 粘贴进去，注意要将 `Location` 中最后一处 `site-packages` 替换成 `Scripts` ，也即是 `you-get` 程序所在位置。而后点击确定，重载CMD即可。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210122223524826.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
现在输入 `you-get` ，显示结果如下：
```bash
> you-get
usage: you-get [OPTION]... URL...

A tiny downloader that scrapes the web

optional arguments:
  -V, --version         Print version and exit
  -h, --help            Print this help message and exit

Dry-run options:
  (no actual downloading)

  -i, --info            Print extracted information
  -u, --url             Print extracted information with URLs
  --json                Print extracted URLs in JSON format

Download options:
  -n, --no-merge        Do not merge video parts
  --no-caption          Do not download captions (subtitles, lyrics, danmaku, ...)
  -f, --force           Force overwriting existing files
  --skip-existing-file-size-check
                        Skip existing file without checking file size
  -F STREAM_ID, --format STREAM_ID
                        Set video format to STREAM_ID
  -O FILE, --output-filename FILE
                        Set output filename
  -o DIR, --output-dir DIR
                        Set output directory
  -p PLAYER, --player PLAYER
                        Stream extracted URL to a PLAYER
  -c COOKIES_FILE, --cookies COOKIES_FILE
                        Load cookies.txt or cookies.sqlite
  -t SECONDS, --timeout SECONDS
                        Set socket timeout
  -d, --debug           Show traceback and other debug info
  -I FILE, --input-file FILE
                        Read non-playlist URLs from FILE
  -P PASSWORD, --password PASSWORD
                        Set video visit password to PASSWORD
  -l, --playlist        Prefer to download a playlist
  -a, --auto-rename     Auto rename same name different files
  -k, --insecure        ignore ssl errors

Proxy options:
  -x HOST:PORT, --http-proxy HOST:PORT
                        Use an HTTP proxy for downloading
  -y HOST:PORT, --extractor-proxy HOST:PORT
                        Use an HTTP proxy for extracting only
  --no-proxy            Never use a proxy
  -s HOST:PORT or USERNAME:PASSWORD@HOST:PORT, --socks-proxy HOST:PORT or USERNAME:PASSWORD@HOST:PORT
                        Use an SOCKS5 proxy for downloading
```


---
# 3. `you-get` 使用
## （1）查看视频源信息 `--info/-i` 和下载视频
以[【MV】Garnidelia--grilletto【魔法科高校的劣等生】OP 完整版（中文字幕）](https://www.bilibili.com/video/BV1jx411j7MB?t=34)为例，可以使用 `--info/-i` 查看其所有可用画质与格式，：
```bash
> you-get -i https://www.bilibili.com/video/BV1jx411j7MB?t=34
site:                Bilibili
title:               【MV】Garnidelia--grilletto【魔法科高校的劣等生】OP 完整版（中文字幕）
streams:             # Available quality and codecs
    [ DASH ] ____________________________________
    - format:        dash-flv720
      container:     mp4
      quality:       高清 720P
      size:          42.4 MiB (44428363 bytes)
    # download-with: you-get --format=dash-flv720 [URL]

    - format:        dash-flv480
      container:     mp4
      quality:       清晰 480P
      size:          33.9 MiB (35582112 bytes)
    # download-with: you-get --format=dash-flv480 [URL]

    - format:        dash-flv360
      container:     mp4
      quality:       流畅 360P
      size:          12.4 MiB (13049847 bytes)
    # download-with: you-get --format=dash-flv360 [URL]

    [ DEFAULT ] _________________________________
    - format:        flv720
      container:     flv
      quality:       高清 720P
      size:          52.9 MiB (55469642 bytes)
    # download-with: you-get --format=flv720 [URL]

    - format:        flv480
      container:     flv
      quality:       清晰 480P
      size:          27.5 MiB (28867098 bytes)
    # download-with: you-get --format=flv480 [URL]

    - format:        flv360
      container:     flv
      quality:       流畅 360P
      size:          31.5 MiB (33072563 bytes)
    # download-with: you-get --format=flv360 [URL]
```
上面标有 `DEFAULT` 的是下载默认画质。为什么下载B站视频会默认是720P呢？原因很简单，B站总是自动播放720P的视频：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210122224518932.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

如认同这一默认画质，可直接下载。注意，命令中的URL地址没有引号：
```bash
> you-get https://www.bilibili.com/video/BV1jx411j7MB?t=34
site:                Bilibili
title:               【MV】Garnidelia--grilletto【魔法科高校的劣等生】OP 完整版（中文字幕）
stream:
    - format:        flv720
      container:     flv
      quality:       高清 720P
      size:          52.9 MiB (55469642 bytes)
    # download-with: you-get --format=flv720 [URL]

Downloading 【MV】Garnidelia--grilletto【魔法科高校的劣等生】OP 完整版（中文字幕）.flv ...
 100% ( 52.9/ 52.9MB) ├████████████████████████████████████████┤[1/1]    4 MB/s

Downloading 【MV】Garnidelia--grilletto【魔法科高校的劣等生】OP 完整版（中文字幕）.cmt.xml ...
```
可以发现，这里的 `stream` 下的 `format, container, quality, size` 都和前面的默认选项相一致。然后打开文件夹，可以看到两个文件，其中 `.xml` 是B站的弹幕文件，可以使用工具将它转换为 `.ass` 格式并导入到播放器中。![在这里插入图片描述](https://img-blog.csdnimg.cn/20210122225151486.png)
如希望下载其他格式的文件，可以使用其他提示选项。

## （2）下载其他内容
运行如下命令将下载一张百度的Logo，不过不是所有图片都能通过 `you-get` 下载。
```bash
> you-get https://dgss2.bdstatic.com/5eR1dDebRNRTm2_p8IuM_a/her/static/indexnew/container/search/baidu-logo.ba9d667.png
Site:       bdstatic.com
Title:      baidu-logo.ba9d667
Type:       Portable Network Graphics (image/png)
Size:       0.0 MiB (3537 Bytes)

Downloading baidu-logo.ba9d667.png ...
 100% (  0.0/  0.0MB) ├████████████████████████████████████████┤[1/1]   10 kB/s
```
图片如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210122230153698.png#pic_center)
文档中还提到，可以让 `you-get` 自动检查网页，下载一切有可能感兴趣的内容：
```bash
> you-get http://kopasas.tumblr.com/post/69361932517
you-get: [error] oops, something went wrong.
you-get: don't panic, c'est la vie. please try the following steps:
you-get:   (1) Rule out any network problem.
you-get:   (2) Make sure you-get is up-to-date.
you-get:   (3) Check if the issue is already known, on
you-get:         https://github.com/soimort/you-get/wiki/Known-Bugs
you-get:         https://github.com/soimort/you-get/issues
you-get:   (4) Run the command with '--debug' option,
you-get:       and report this issue with the full output.
```
出现错误，可能是GFW导致的，可以使用 `-x` 选项设置代理。另外，这里的 `don't panic` 是在玩《银河系漫游指南》的梗吗……


## （3）下载视频合集 `--playlist/-l` 和修改存储路径 `--output-dir/-o` 
以[【谜之声实况】扣押 DISTRAINT （三p完结）](https://www.bilibili.com/video/BV1Ws411s7ye?from=search&seid=8670069290811596358)为例：
```bash
> you-get -l https://www.bilibili.com/video/BV1Ws411s7ye?from=search&seid=8670069290811596358 -o 'C:\Users\21839\Desktop'
site:                Bilibili
title:               【谜之声实况】扣押 DISTRAINT （三p完结） (P1. 一纸文书扣房产，老头老太往外赶)
stream:
    - format:        flv720
      container:     flv
      quality:       高清 720P
      size:          162.5 MiB (170374077 bytes)
    # download-with: you-get --format=flv720 [URL]

you-get: Skipping .\【谜之声实况】扣押 DISTRAINT （三p完结） (P1. 一纸文书扣房产，老头老太往外赶).flv: file already exists

Downloading 【谜之声实况】扣押 DISTRAINT （三p完结） (P1. 一纸文书扣房产，老头老太往外赶).cmt.xml ...

site:                Bilibili
title:               【谜之声实况】扣押 DISTRAINT （三p完结） (P2. 孤苦长辈谁去看，潦倒房客何人管)
stream:
    - format:        flv720
      container:     flv
      quality:       高清 720P
      size:          163.8 MiB (171710921 bytes)
    # download-with: you-get --format=flv720 [URL]

Downloading 【谜之声实况】扣押 DISTRAINT （三p完结） (P2. 孤苦长辈谁去看，潦倒房客何人管).flv ...
89.6% (146.8/163.8MB) ├████████████████████████████████████────┤[1/1]  172 kB/s
```
