
## 6.7． 图解es的并发问题

如同秒杀，多线程情况下，es同样会出现并发冲突问题。

## 6.8． 图解悲观锁与乐观锁机制

为控制并发问题，我们通常采用锁机制。分为悲观锁和乐观锁两种机制。

悲观锁：很悲观，所有情况都上锁。此时只有一个线程可以操作数据。具体例子为数据库中的行级锁. 表级锁. 读锁. 写锁等。

特点：优点是方便，直接加锁，对程序透明。缺点是效率低。

乐观锁：很乐观，对数据本身不加锁。提交数据时，通过一种机制验证是否存在冲突，如es中通过版本号验证。

特点：优点是并发能力高。缺点是操作繁琐，在提交数据时，可能反复重试多次。

## 6.9． 图解es内部基于_version乐观锁控制

#### 实验基于_version的版本控制

es对于文档的增删改都是基于版本号。

1新增多次文档：

```
PUT /test_index/_doc/3
{
  "test_field": "test"
}
```

返回版本号递增

2删除此文档

```
DELETE /test_index/_doc/3
```

返回

```
DELETE /test_index/_doc/3
{
  "_index" : "test_index",
  "_type" : "_doc",
  "_id" : "2",
  "_version" : 6,
  "result" : "deleted",
  "_shards" : {
    "total" : 2,
    "successful" : 1,
    "failed" : 0
  },
  "_seq_no" : 7,
  "_primary_term" : 1
}
```

3再新增

```
PUT /test_index/_doc/3
{
  "test_field": "test"
}
```

可以看到版本号依然递增，验证延迟删除策略。

如果删除一条数据立马删除的话，所有分片和副本都要立马删除，对es集群压力太大。

#### 图解es内部并发控制

es内部主从同步时，是多线程异步。乐观锁机制。

## 6.10． 演示客户端程序基于_version并发操作流程

java python客户端更新的机制。

#### 新建文档

```json
PUT /test_index/_doc/5
{
  "test_field": "itcast"
}
```

返回： 

```json
{
  "_index" : "test_index",
  "_type" : "_doc",
  "_id" : "3",
  "_version" : 1,
  "result" : "created",
  "_shards" : {
    "total" : 2,
    "successful" : 1,
    "failed" : 0
  },
  "_seq_no" : 8,
  "_primary_term" : 1
}
```

#### 客户端1修改。带版本号1。

首先获取数据的当前版本号

```
GET /test_index/_doc/5
```

更新文档

```
PUT /test_index/_doc/5?version=1
{
  "test_field": "itcast1"
}
PUT /test_index/_doc/5?if_seq_no=21&if_primary_term=1
{
  "test_field": "itcast1"
}
```

#### 客户端2并发修改。带版本号1。

```
PUT /test_index/_doc/5?version=1
{
  "test_field": "itcast2"
}
PUT /test_index/_doc/5?if_seq_no=21&if_primary_term=1
{
  "test_field": "itcast1"
}
```

报错。

#### 客户端2重新查询。得到最新版本为2。seq_no=22

```
GET /test_index/_doc/4
```

#### 客户端2并发修改。带版本号2。

```
PUT /test_index/_doc/4?version=2
{
  "test_field": "itcast2"
}
es7
PUT /test_index/_doc/5?if_seq_no=22&if_primary_term=1
{
  "test_field": "itcast2"
}
```

修改成功。

## 6.11． 演示自己手动控制版本号 external version

背景：已有数据是在数据库中，有自己手动维护的版本号的情况下，可以使用external version控制。hbase。

要求：修改时external version要大于当前文档的_version

对比：基于_version时，修改的文档version等于当前文档的版本号。

使用?version=1&version_type=external

#### 新建文档

```
PUT /test_index/_doc/4
{
  "test_field": "itcast"
}
```

更新文档：

#### 客户端1修改文档

```
PUT /test_index/_doc/4?version=2&version_type=external
{
  "test_field": "itcast1"
}
```

#### 客户端2同时修改

```
PUT /test_index/_doc/4?version=2&version_type=external
{
  "test_field": "itcast2"
}
```

返回：

```
{
  "error": {
    "root_cause": [
      {
        "type": "version_conflict_engine_exception",
        "reason": "[4]: version conflict, current version [2] is higher or equal to the one provided [2]",
        "index_uuid": "-rqYZ2EcSPqL6pu8Gi35jw",
        "shard": "1",
        "index": "test_index"
      }
    ],
    "type": "version_conflict_engine_exception",
    "reason": "[4]: version conflict, current version [2] is higher or equal to the one provided [2]",
    "index_uuid": "-rqYZ2EcSPqL6pu8Gi35jw",
    "shard": "1",
    "index": "test_index"
  },
  "status": 409
}
```

#### 客户端2重新查询数据

```
GET /test_index/_doc/4
```

#### 客户端2重新修改数据

```
PUT /test_index/_doc/4?version=3&version_type=external
{
  "test_field": "itcast2"
}
```

## 6.12．更新时 retry_on_conflict 参数

####  retry_on_conflict 

指定重试次数

```
POST /test_index/_doc/5/_update?retry_on_conflict=3
{
  "doc": {
    "test_field": "itcast1"
  }
}
```

#### 与 _version结合使用

```json
POST /test_index/_doc/5/_update?retry_on_conflict=3&version=22&version_type=external
{
  "doc": {
    "test_field": "itcast1"
  }
}
```
