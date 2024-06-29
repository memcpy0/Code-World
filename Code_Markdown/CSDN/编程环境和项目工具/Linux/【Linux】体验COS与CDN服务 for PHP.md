@[toc]

# 实验内容
**云对象存储**（`Cloud Object Storage`）是稳定安全的云端存储服务，可以存放任意数量和形式的非结构化内容。结合**内容分发网络**（`Content Delivery Network`），将内容分发至加速节点，利用**全球调度系统**使用户在就近节点获取内容。

本实验介绍如何在PHP应用中使用COS与CDN服务。

软件环境：CentOS 7.2 64 位

# CDN 服务申请
## 实名认证与申请服务
腾讯云提供了 [CDN 的加速服务](https://cloud.tencent.com/product/cdn?fromSource=lab)，但使用服务前您需要实名认证，点击下面的视频了解如何申请该服务

如果您已经实名认证，可以直接跳过这一步。
## 使用 COS 服务
### 开通 COS 并上传文件
CDN 开通后，我们既可以使用 **CDN 全站加速**的功能，也可以使用 **CDN 部署资源文件**的功能，本实验使用 COS/CDN 超出免费配额可能产生小额费用。

在使用 **CDN 部署资源文件**的功能前，我们需要开通腾讯云的 **COS 服务**，点击下面的视频了解如何申请该服务


### COS 文件地址与 CDN 加速地址

文件通过腾讯云官网控制台web页面上传后，通过查看上传文件信息可以得到形如 Bucket名称-id.cos地区缩写.myqcloud.com/文件名 的 URL 地址，将该地址中的 cos地区缩写 改为 file 即可得到 CDN 的加速地址.

使用该加速地址来访问的资源文件可以得到更快的响应，达到加速的目的. 如:

文件地址:

http://cloud-1251435248.costj.myqcloud.com/hello.txt

对应的 CDN 地址:

http://cloud-1251435248.file.myqcloud.com/hello.txt

cos地址与对应园区的关系

    costj -- 华北(天津园区)
    cossh -- 华东(上海园区)
    cosgz -- 华南(广州园区)
    cossgp -- 新加坡园区

使用 COS SDK for PHP

    任务时间：30min ~ 60min

准备工作

上面介绍了使用浏览器上传文件到 COS 并得到 CDN 地址的方式，下面介绍如何通过 SDK 的方式使用 COS. 在这之前，我们需要做一些准备工作
创建相关目录

mkdir -p /data/upload

准备相关的文件

执行下面的命令，在 /data 目录下创建一个名叫upload_example.txt的文件

echo 'Hello World' > /data/upload/upload_example.txt

查看appid，secret_id 和 secret_key 信息

    如果您 Bucket 的默认域名中包含125开头的 APPID，请使用“API密钥”， 点击前往
    如果您 Bucket 的默认域名中包含100开头的项目 ID，请使用“项目密钥”， 点击前往

查看 bucket 属于哪个园区

    打开 bucket列表
    在列表中点击将要上传的 bucket，进入详情页
    选择 基础配置 tab
    查看 基本信息 - 所属地区
    华北地区对应 tj，华南地区对应 gz，华东地区对应 sh

安装 Git 与 PHP 环境

yum install -y git php php-common php-devel

安装 COS 的 PHP SDK

git clone https://github.com/tencentyun/cos-php-sdk-v4 /data/cos-php-sdk-v4

修改 SDK 配置

修改 /data/cos-php-sdk-v4/qcloudcos/conf.php 内的 APPID、SECRET_ID、SECRET_KEY 为您的配置。
使用 SDK 上传文件到 COS

在 /data 目录下创建 cos_php_upload.php 文件，并编辑内容如下
示例代码：/data/cos_php_upload.php

<?php
// 包含cos-php-sdk-v4/include.php文件
require('./cos-php-sdk-v4/include.php');
use qcloudcos\Cosapi;

// 设置COS所在的区域，对应关系如下：
//     华南  -> gz
//     华中  -> sh
//     华北  -> tj
Cosapi::setRegion('');

$bucket = '';                            // 填写你的bucket名
$src = './upload/upload_example.txt';    // 填写要上传到 cos 的文件(含地址)，如 ./upload/upload_example.txt
$dst = 'upload_sample_php.txt';          // 填写上传到 cos 后的文件名称，如 upload_sample_php.txt

// 上传文件
$ret = Cosapi::upload($bucket，$src，$dst);
var_dump($ret);

将你的 bucket 所属的地区码，bucket 名，需要上传的文件，上传后的文件名 依次填入后执行

cd /data && php /data/cos_php_upload.php

如果调用成功，会返回类似如下结构的信息:

array(4) {
  ["code"]=>
  int(0)
  ["message"]=>
  string(7) "SUCCESS"
  ["request_id"]=>
  string(36) "NTkyZmM2NzFfOGVhMDY4XzgzMmVfODU1Yjc="
  ["data"]=>
  array(5) {
    ["access_url"]=>
    string(64) "http://detect-1253675457.file.myqcloud.com/upload_sample_php.txt"
    ["resource_path"]=>
    string(40) "/1253675457/detect/upload_sample_php.txt"
    ["source_url"]=>
    string(65) "http://detect-1253675457.cossh.myqcloud.com/upload_sample_php.txt"
    ["url"]=>
    string(76) "http://sh.file.myqcloud.com/files/v2/1253675457/detect/upload_sample_php.txt"
    ["vid"]=>
    string(42) "23c8c295f62f8639cd4dc5cda26b1b4b1496303217"
  }
}

点击这里 了解返回的各个字段的含义

在 bucket列表 中选择你刚才填入的 bucket 并查看文件列表，会发现新增了 upload_sample_php.txt 文件，证明确实已经成功上传到 COS

如果调用失败，会返回类似如下的信息:

array(3) {
  ["code"]=>
  int(-62)
  ["message"]=>
  string(22) "ERROR_PROXY_AUTH_APPID"
  ["request_id"]=>
  string(36) "NTkyZmM2ZjhfOGRhMDY4XzViMzdfODc4Njg="
}

结合 cos错误码说明 和message字段，您可以知道发生错误的原因
使用 SDK 移动 COS 文件

COS 提供通过 SDK，移动同一 bucket 下的文件位置的能力

在 /data 目录下创建 cos_php_move.php 文件，并编辑内容如下
示例代码：/data/cos_php_move.php

<?php
// 包含cos-php-sdk-v4/include.php文件
require('./cos-php-sdk-v4/include.php');
use qcloudcos\Cosapi;

// 设置COS所在的区域，对应关系如下：
//     华南  -> gz
//     华中  -> sh
//     华北  -> tj
Cosapi::setRegion('gz');

$bucket = '';                   // 填写你的bucket名
$src = '/upload_sample_php.txt'; // 填写要移动的 cos 文件的名称，如 /upload_sample_php.txt
$dst = '';                      // 填写移动后 cos 文件的名称，如 /upload_sample_php_move.txt

// 上传文件
$ret = Cosapi::moveFile($bucket，$src，$dst);
var_dump($ret);

将你的 bucket 所属的地区码，bucket 名，需要移动的文件名，移动后的文件名 依次填入后执行

cd /data && php /data/cos_php_move.php

如果调用成功，会返回类似如下结构的信息:

array(3) {
  ["code"]=>
  int(0)
  ["message"]=>
  string(7) "SUCCESS"
  ["request_id"]=>
  string(36) "NTkyZmNhMDlfOGVhMDY4XzgzMzdfODViMDI="
}

在 bucket列表 中选择你刚才填入的 bucket 并查看文件列表，会发现upload_sample_php.txt文件名字变成了 sample_file_move_php.txt 文件，证明文件移动成功
使用 SDK 创建 COS 目录

COS 提供通过 SDK，在 bucket 下创建目录的能力

在 /data 目录下创建 cos_php_create_folder.py 文件，并编辑内容如下
示例代码：/data/cos_php_create_folder.php

<?php
// 包含cos-php-sdk-v4/include.php文件
require('./cos-php-sdk-v4/include.php');
use qcloudcos\Cosapi;

// 设置COS所在的区域，对应关系如下：
//     华南  -> gz
//     华中  -> sh
//     华北  -> tj
Cosapi::setRegion('gz');

$bucket = '';                   // 填写你的bucket名
$folderName = '/sample_folder_php'; // 填写要创建的目录的名字

// 上传文件
$ret = Cosapi::createFolder($bucket，$folderName);
var_dump($ret);

将你的 bucket 所属的地区码，bucket 名，要创建的目录的名字 依次填入后执行

cd /data && php /data/cos_php_create_folder.php

如果调用成功，会返回类似如下结构的信息:

array(4) {
  ["code"]=>
  int(0)
  ["message"]=>
  string(7) "SUCCESS"
  ["request_id"]=>
  string(32) "NTkyZmNiNmFfZjFlNjlfYmVhXzgxNzRl"
  ["data"]=>
  array(1) {
    ["ctime"]=>
    int(1496304490)
  }
}

在 bucket列表 中选择你刚才填入的 bucket 并查看文件列表，会发现新增了一个sample_folder_php目录，证明目录创建成功
实验完成

恭喜您已经完成了 CDN 与 COS for PHP 的学习
