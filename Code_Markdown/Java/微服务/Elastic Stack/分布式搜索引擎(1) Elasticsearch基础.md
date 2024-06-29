日常用语词不算多。分词表数据不多
# 1. 初识Elastic Search
## 1.1 了解ES
### 1.1.1 Elastic Search的作用
**Elastic Search是一款非常强大的开源搜索引擎**，具备非常多强大功能，可以帮助我们从海量数据中快速找到需要的内容。例如：
- 在GitHub搜索代码
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071630791.png)
- 在电商网站搜索商品
- 在百度搜索答案
- 在打车软件搜索附近的车
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071634691.png)

### 1.1.2 ELK技术栈
Elastic Search结合kibana、Logstash、Beats，也就是Elastic Stack（ELK）。被广泛应用在日志数据分析、实时监控等领域：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071635047.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071636423.png)

而Elastic Search是Elastic stack的核心，负责存储、搜索、分析数据。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071638236.png)
 
### 1.1.3 Elastic Search和lucene
Elastic Search底层是基于**lucene**来实现的。**Lucene**是一个Java语言的搜索引擎类库，是Apache公司的顶级项目，由DougCutting于1999年研发。官网地址：https://lucene.apache.org/ 。Lucene的优势：
- 易扩展
- 高性能（基于倒排索引）

Lucene的缺点：
- 只限于Java语言开发
- 学习曲线陡峭
- 不支持水平扩展
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071641423.png)
**Elastic Search**的发展历史：
- 2004年Shay Banon基于Lucene开发了Compass
- 2010年Shay Banon 重写了Compass，取名为Elastic Search。

官网地址: https://www.elastic.co/cn/ 。目前最新的版本是：7.12.1。相比与lucene，elasticsearch具备下列优势：
- 支持分布式，可水平扩展
- 提供Restful接口，可被任何语言调用
![image-20210720195001221](assets/image-20210720195001221.png)
### 1.1.4 为什么不是其他搜索技术？
目前比较知名的搜索引擎技术排名：
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071643606.png)
虽然在早期，Apache Solr是最主要的搜索引擎技术，但随着发展Elastic Search已经渐渐超越了Solr，独占鳌头：
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071644468.png)
### 1.1.5 总结
什么是Elastic Search？
- 一个开源的分布式搜索引擎，可以用来实现搜索、日志统计、分析、系统监控等功能

什么是elastic stack（ELK）？
- 是以Elastic Search为核心的技术栈，包括beats、Logstash、kibana、Elastic Search

什么是Lucene？
- 是Apache的开源搜索引擎类库，提供了搜索引擎的核心API

## 1.2 倒排索引
倒排索引的概念是基于MySQL这样的正向索引而言的。
### 1.2.1 正向索引
那么什么是正向索引呢？例如给下表（tb_goods）中的id创建索引：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071645957.png)
如果是根据id查询，那么直接走索引，查询速度非常快。但如果是基于一个标题字段title做模糊查询，只能是逐行扫描数据，流程如下：
1）用户搜索数据，条件是title符合 `"%手机%"`
2）逐行获取数据，比如id为1的数据
3）判断数据中的title是否符合用户搜索条件
4）如果符合则放入结果集，不符合则丢弃。回到步骤1

逐行扫描，也就是全表扫描，随着数据量增加，其查询效率也会越来越低。当数据量达到数百万时，就是一场灾难。
### 1.2.2 倒排索引
倒排索引中有两个非常重要的概念：
- 文档（`Document`）：用来搜索的数据，其中的**每一条数据就是一个文档**。例如一个网页、一个商品信息
- 词条（`Term`）：对文档数据或用户搜索数据，**利用某种算法分词，得到的具备含义的词语就是词条**。例如：我是中国人，就可以分为：我、是、中国人、中国、国人这样的几个词条

==**创建倒排索引**是对正向索引的一种特殊处理==，流程如下：
- 将每一个文档的数据利用算法分词，得到一个个词条
- 创建表，每行数据包括**词条**、词条所在文档id、位置等信息
- ==因为词条唯一性，可以给词条创建索引，例如Hash表结构索引==

倒排索引中包含两部分内容：
- 词条词典 *Term Dictionary* ：记录所有词条，以及词条与倒排列表 *Posting List* 之间的关系，会**给词条创建索引**，提高查询和插入效率
- 倒排列表 *Posting List* ：记录**词条所在的文档ID、词条出现频率 、词条在文档中的位置等**信息
	- 文档ID：用于快速获取文档
	- 词条频率（TF）：文档在词条出现的次数，用于评分
	
如图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071649099.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071655923.png)

倒排索引的**搜索流程**如下（以搜索"华为手机"为例），如图：
1）用户输入条件 `"华为手机"` 进行搜索。
2）对用户输入内容**分词**，得到词条：`华为`、`手机`。
3）拿着词条在倒排索引中查找，可以得到包含词条的文档id：1、2、3。
4）**拿着文档id到正向索引中查找具体文档**。
![600](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071651545.png)

虽然要==先查询倒排索引，再查询正向索引==，但无论是词条、还是文档id都建立了索引，查询速度非常快！无需全表扫描。
### 1.2.3 正向和倒排
那么为什么一个叫做正向索引，一个叫做倒排索引呢？
- **正向索引**是最传统的，根据ID索引的方式。但根据词条查询时，必须先逐条获取每个文档，然后判断文档中是否包含所需要的词条，是**根据文档找词条的过程**。
- 而**倒排索引**则相反，是先找到用户要搜索的词条，根据词条得到保护词条的文档的ID，然后根据ID获取文档。是**根据词条找文档的过程**。

是不是恰好反过来了？那么两者方式的优缺点是什么呢？

**正向索引**：
- 优点：
  - **可以给多个字段创建索引**
  - 根据索引字段搜索、排序速度非常快
- 缺点：
  - 根据非索引字段，或者索引字段中的部分词条查找时，只能全表扫描。

**倒排索引**：
- 优点：
  - **根据词条搜索、模糊搜索时，速度非常快**
- 缺点：
  - **只能给词条创建索引**，而不是字段
  - **无法根据字段做排序**

## 1.3 ES的一些概念
Elastic Search中有很多独有的概念，与MySQL中略有差别，但也有相似之处。
### 1.3.1 文档和字段
Elastic Search是面向**文档**（Document）存储的，可以是数据库中的一条商品数据，一个订单信息。文档数据会被序列化为JSON格式后存储在Elastic Search中：
![660](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071657457.png)
而JSON文档中往往包含很多的**字段（Field）**，类似于数据库中的列。
### 1.3.2 索引和映射
**索引（Index）**，就是**相同类型的文档的集合**。例如：
- 所有用户文档，可以组织在一起，称为用户的索引；
- 所有商品文档，可以组织在一起，称为商品的索引；
- 所有订单文档，可以组织在一起，称为订单的索引；
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071659444.png)

因此，我们可以==把索引当做是数据库中的表==。数据库的表会有约束信息，用来定义表的结构、字段的名称、类型等信息。因此，==索引库中就有**映射（mapping）**，是索引中文档的字段约束信息==，类似表的结构约束。
### 1.3.3 MySQL与Elastic Search
我们统一的把MySQL与Elastic Search的概念做一下对比：
| **MySQL** | **Elastic Search** | **说明**                                                     |
| --------- | ----------------- | ------------------------------------------------------------ |
| Table     | Index             | 索引（index），就是同类型文档的集合，类似数据库的表(table)           |
| Row       | Document          | 文档（Document），就是一条条的数据，类似数据库中的行（Row），文档都是JSON格式 |
| Column    | Field             | 字段（Field），就是JSON文档中的字段，类似数据库中的列（Column） |
| Schema    | Mapping           | Mapping（映射）是索引中文档的约束，例如字段类型约束。类似数据库的表结构（Schema） |
| SQL       | DSL               | DSL是Elastic Search提供的JSON风格的请求语句，用来操作Elastic Search，实现CRUD |

是不是说，我们学习了Elastic Search就不再需要MySQL了呢？并不是如此，两者各自有自己的擅长支出：
- MySQL：**擅长事务类型操作**，可以确保数据的安全和一致性
- Elastic Search：**擅长海量数据的搜索、分析、计算**

因此在企业中，往往是两者结合使用：
- **对安全性要求较高的写操作**，使用MySQL实现
- **对查询性能要求较高的搜索需求**，使用Elastic Search实现
- 两者再基于某种方式，实现数据的同步，保证一致性
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071703410.png)
## 1.4 安装es、kibana
### 1.4.1 安装
参考课前资料。
### 1.4.2 分词器
参考课前资料。
### 1.4.3 总结
分词器的作用是什么？
- 创建倒排索引时对文档分词
- 用户搜索时，对输入的内容分词

IK分词器有几种模式？
- ik_smart：智能切分，粗粒度
- ik_max_word：最细切分，细粒度

IK分词器如何拓展词条？如何停用词条？
- 利用config目录的IkAnalyzer.cfg.xml文件添加拓展词典和停用词典
- 在词典中添加拓展词条或者停用词条

---
# 2. 索引库操作
索引库就类似数据库表，mapping映射就类似表的结构。我们要向es中存储数据，必须先创建“库”和“表”。
## 2.1 mapping映射属性
mapping是对索引库中文档的约束，常见的mapping属性包括：
- type：字段数据类型，常见的简单类型有：
  - 字符串：text（可分词的文本）、keyword（精确值，例如：品牌、国家、ip地址）
  - 数值：long、integer、short、byte、double、float、
  - 布尔：boolean
  - 日期：date
  - 对象：object
- index：是否创建索引，默认为true
- analyzer：使用哪种分词器
- properties：该字段的子字段

例如下面的JSON文档：
```json
{
    "age": 21,
    "weight": 52.1,
    "isMarried": false,
    "info": "黑马程序员Java讲师",
    "email": "zy@itcast.cn",
    "score": [99.1, 99.5, 98.9],
    "name": {
        "firstName": "云",
        "lastName": "赵"
    }
}
```
对应的每个字段映射（mapping）：
- age：类型为 integer；参与搜索，因此需要index为true；无需分词器
- weight：类型为float；参与搜索，因此需要index为true；无需分词器
- isMarried：类型为boolean；参与搜索，因此需要index为true；无需分词器
- info：类型为字符串，需要分词，因此是text；参与搜索，因此需要index为true；分词器可以用ik_smart
- email：类型为字符串，但是不需要分词，因此是keyword；不参与搜索，因此需要index为false；无需分词器
- score：虽然是数组，但是我们只看元素的类型，类型为float；参与搜索，因此需要index为true；无需分词器
- name：类型为object，需要定义多个子属性
	- name.firstName；类型为字符串，但是不需要分词，因此是keyword；参与搜索，因此需要index为true；无需分词器
	- name.lastName；类型为字符串，但是不需要分词，因此是keyword；参与搜索，因此需要index为true；无需分词器

## 2.2 索引库的CRUD
这里我们统一使用Kibana编写DSL的方式来演示。
### 2.2.1 创建索引库和映射
#### 基本语法：
- 请求方式：PUT
- 请求路径：/索引库名，可以自定义
- 请求参数：**mapping映射**

格式：
```json
PUT /索引库名称
{
  "mappings": {
    "properties": {
      "字段名":{
        "type": "text",
        "analyzer": "ik_smart"
      },
      "字段名2":{
        "type": "keyword",
        "index": "false"
      },
      "字段名3":{
        "properties": {
          "子字段": {
            "type": "keyword"
          }
        }
      },
      // ...略
    }
  }
}
```
#### 示例：
```sh
PUT /heima
{
  "mappings": {
    "properties": {
      "info":{
        "type": "text",
        "analyzer": "ik_smart"
      },
      "email":{
        "type": "keyword",
        "index": "falsae"
      },
      "name":{
	    "type": "object",
        "properties": {
          "firstName": {
            "type": "keyword"
          }
        }
      },
      // ... 略
    }
  }
}
```
### 2.2.2 查询索引库
**基本语法**：
- 请求方式：GET
- 请求路径：/索引库名
- 请求参数：无

**格式**：
```
GET /索引库名
```
**示例**：
![600](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071957422.png)
### 2.2.3 修改索引库
倒排索引结构虽然不复杂，但==一旦数据结构改变（比如改变了分词器），就需要重新创建倒排索引，这简直是灾难==。因此索引库**一旦创建，无法修改mapping**。索引库无法修改。

虽然无法修改 `mapping` 中已有的字段，但却允许添加新的字段到 `mapping` 中，因为不会对倒排索引产生影响。

**语法说明**：
```json
PUT /索引库名/_mapping
{
  "properties": {
    "新字段名": {
      "type": "integer"
    }
  }
}
```
**示例**：
![650](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303072000064.png)
### 2.2.4 删除索引库
**语法：**
- 请求方式：DELETE
- 请求路径：/索引库名
- 请求参数：无

**格式：**
```
DELETE /索引库名
```
在kibana中测试：
![700](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303072001700.png)
### 2.2.5 总结
索引库操作有哪些？
- 创建索引库：PUT /索引库名
- 查询索引库：GET /索引库名
- 删除索引库：DELETE /索引库名
- 添加字段：PUT /索引库名/_mapping

---
# 3. 文档操作
## 3.1 新增文档
**语法：**
```json
POST /索引库名/_doc/文档id
{
    "字段1": "值1",
    "字段2": "值2",
    "字段3": {
        "子属性1": "值3",
        "子属性2": "值4"
    },
    // ...
}
```
**示例：**
```json
POST /heima/_doc/1
{
    "info": "黑马程序员Java讲师",
    "email": "zy@itcast.cn",
    "name": {
        "firstName": "云",
        "lastName": "赵"
    }
}
```
**响应：**
![700](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303072005922.png)
## 3.2 查询文档
根据rest风格，新增是post，查询应该是get，不过查询一般都需要条件，这里我们把文档ID带上。**语法：**
```json
GET /{索引库名称}/_doc/{id}
```
**通过kibana查看数据：**
```js
GET /heima/_doc/1
```
**查看结果：**
![700](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303072006834.png)

## 3.3 删除文档
删除使用DELETE请求，同样，需要根据ID进行删除。**语法：**
```js
DELETE /{索引库名}/_doc/id值
```
**示例：**
```json
# 根据id删除数据
DELETE /heima/_doc/1
```
**结果：**
![700](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303072007895.png)
## 3.4 修改文档
修改有两种方式：
- 全量修改：直接覆盖原来的文档
- 增量修改：修改文档中的部分字段

### 3.4.1 全量修改
**全量修改是覆盖原来的文档**，其本质是：
- 根据指定的ID删除文档
- 新增一个相同ID的文档

**注意**：如果根据ID删除时，ID不存在，第二步的新增也会执行，也就从修改变成了新增操作了。

**语法：**
```json
PUT /{索引库名}/_doc/文档id
{
    "字段1": "值1",
    "字段2": "值2",
    // ... 略
}

```
**示例：**
```json
PUT /heima/_doc/1
{
    "info": "黑马程序员高级Java讲师",
    "email": "zy@itcast.cn",
    "name": {
        "firstName": "云",
        "lastName": "赵"
    }
}
```
### 3.4.2 增量修改
增量修改是只修改指定id匹配的文档中的部分字段。**语法：**
```json
POST /{索引库名}/_update/文档id
{
    "doc": {
         "字段名": "新的值",
    }
}
```
**示例：**
```json
POST /heima/_update/1
{
  "doc": {
    "email": "ZhaoYun@itcast.cn"
  }
}
```
## 3.5 总结
文档操作有哪些？
- 创建文档：`POST /{索引库名}/_doc/文档id   { json文档 }`
- 查询文档：`GET /{索引库名}/_doc/文档id`
- 删除文档：`DELETE /{索引库名}/_doc/文档id`
- 修改文档：
	- 全量修改：`PUT /{索引库名}/_doc/文档id { json文档 }`
	- 增量修改：`POST /{索引库名}/_update/文档id { "doc": {字段}}`
