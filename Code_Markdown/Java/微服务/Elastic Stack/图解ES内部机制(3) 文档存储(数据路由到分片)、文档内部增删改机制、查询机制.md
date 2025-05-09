
# 9． 图解文档存储机制

## 9.1． 数据路由

### 9.1.1文档存储如何路由到相应分片

一个文档，最终会落在主分片的一个分片上，到底应该在哪一个分片？这就是数据路由。

### 9.1.2路由算法

```
shard = hash(routing) % number_of_primary_shards
```

哈希值对主分片数取模。

 

举例：

对一个文档经行crud时，都会带一个路由值 routing number。默认为文档_id（可能是手动指定，也可能是自动生成）。

存储1号文档，经过哈希计算，哈希值为2,此索引有3个主分片，那么计算2%3=2，就算出此文档在P2分片上。

决定一个document在哪个shard上，最重要的一个值就是routing值，默认是_id，也可以手动指定，相同的routing值，每次过来，从hash函数中，产出的hash值一定是相同的

无论hash值是几，无论是什么数字，对number_of_primary_shards求余数，结果一定是在0~number_of_primary_shards-1之间这个范围内的。0,1,2。

### 9.1.3手动指定 routing number

```
PUT /test_index/_doc/15?routing=num
{
  "num": 0,
  "tags": []
}
```

场景：在程序中，架构师可以手动指定已有数据的一个属性为路由值，好处是可以定制一类文档数据存储到一个分片中。缺点是设计不好，会造成数据倾斜。

所以，不同文档尽量放到不同的索引中。剩下的事情交给es集群自己处理。

### 9.1.4主分片数量不可变

涉及到以往数据的查询搜索，所以一旦建立索引，主分片数不可变。

## 9.2． 图解文档的增删改内部机制

增删改可以看做update,都是对数据的改动。一个改动请求发送到es集群，经历以下四个步骤：

（1）客户端选择一个node发送请求过去，这个node就是coordinating node（协调节点）

（2）coordinating node，对document进行路由，将请求转发给对应的node（有primary shard）

（3）实际的node上的primary shard处理请求，然后将数据同步到replica node。

（4）coordinating node，如果发现primary node和所有replica node都搞定之后，就返回响应结果给客户端。

## 9.3．图解文档的查询内部机制

1. 客户端发送请求到任意一个node，成为coordinate node

2. coordinate node对document进行路由，将请求转发到对应的node，此时会使用round-robin随机轮询算法，在primary shard以及其所有replica中随机选择一个，让读请求负载均衡

3. 接收请求的node返回document给coordinate node

4. coordinate node返回document给客户端

5. 特殊情况：document如果还在建立索引过程中，可能只有primary shard有，任何一个replica shard都没有，此时可能会导致无法读取到document，但是document完成索引建立之后，primary shard和replica shard就都有了。

## 9.4．bulk api奇特的json格式

```json
POST /_bulk
{"action": {"meta"}}\n
{"data"}\n
{"action": {"meta"}}\n
{"data"}\n

[
    {
        "action":{
            "method":"create"
        },
        "data":{
            "id":1,
            "field1":"java",
            "field1":"spring",
        }
    },
      {
        "action":{
            "method":"create"
        },
        "data":{
            "id":2,
            "field1":"java",
            "field1":"spring",
        }
    }       
]

```

1. bulk中的每个操作都可能要转发到不同的node的shard去执行

2. 如果采用比较良好的json数组格式

允许任意的换行，整个可读性非常棒，读起来很爽，es拿到那种标准格式的json串以后，要按照下述流程去进行处理

（1）将json数组解析为JSONArray对象，这个时候，整个数据，就会在内存中出现一份一模一样的拷贝，一份数据是json文本，一份数据是JSONArray对象

（2）解析json数组里的每个json，对每个请求中的document进行路由

（3）为路由到同一个shard上的多个请求，创建一个请求数组。100请求中有10个是到P1.

（4）将这个请求数组序列化

（5）将序列化后的请求数组发送到对应的节点上去

3. 耗费更多内存，更多的jvm gc开销

我们之前提到过bulk size最佳大小的那个问题，一般建议说在几千条那样，然后大小在10MB左右，所以说，可怕的事情来了。假设说现在100个bulk请求发送到了一个节点上去，然后每个请求是10MB，100个请求，就是1000MB = 1GB，然后每个请求的json都copy一份为jsonarray对象，此时内存中的占用就会翻倍，就会占用2GB的内存，甚至还不止。因为弄成jsonarray之后，还可能会多搞一些其他的数据结构，2GB+的内存占用。

占用更多的内存可能就会积压其他请求的内存使用量，比如说最重要的搜索请求，分析请求，等等，此时就可能会导致其他请求的性能急速下降。

另外的话，占用内存更多，就会导致java虚拟机的垃圾回收次数更多，跟频繁，每次要回收的垃圾对象更多，耗费的时间更多，导致es的java虚拟机停止工作线程的时间更多。

4. 现在的奇特格式

```
POST /_bulk
{ "delete": { "_index": "test_index",  "_id": "5" }} \n
{ "create": { "_index": "test_index",  "_id": "14" }}\n
{ "test_field": "test14" }\n
{ "update": { "_index": "test_index",  "_id": "2"} }\n
{ "doc" : {"test_field" : "bulk test"} }\n
```

（1）不用将其转换为json对象，不会出现内存中的相同数据的拷贝，直接按照换行符切割json

（2）对每两个一组的json，读取meta，进行document路由

（3）直接将对应的json发送到node上去

5. 最大的优势在于，不需要将json数组解析为一个JSONArray对象，形成一份大数据的拷贝，浪费内存空间，尽可能地保证性能。