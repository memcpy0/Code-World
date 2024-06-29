# 4. 索引-Index
## 4.1 概述
索引支持在MongoDB中高效地执行查询。如果没有索引，MongoDB必须执行全集合扫描 collection scan ，即扫描集合中的每个文档，以选择与查询语句匹配的文档。这种扫描全集合的查询效率是非常低的，特别在处理大量的数据时，查询可以要花费几十秒甚至几分钟，这对网站的性能是非常致命的。==如果查询存在**适当的索引**，MongoDB可以使用该索引限制 limit 必须检查的文档数==。

索引是特殊的数据结构，它以易于遍历的形式、存储集合数据集的**一小部分** *store a small portion of the collection's data set in an easy to traverse form* 。索引存储特定字段或一组字段的值，**按字段值排序**。
- 索引项的排序支持**有效的相等匹配**和**基于范围的查询操作** *The ordering of the index entries supports efficient equality matches and range-based query operations* 。
- 此外，==MongoDB还可以使用索引中的排序返回排序结果==。

官网文档：https://www.mongodb.com/docs/manual/indexes/

*下图说明了，一个查询使用一个索引来 选择 selects 和 排序 orders 匹配的文档，原先的聚簇索引（存储方式）如下面的白色格子，在 score 上是无序的。对 score 建立索引后，可以直接找到最后一个 <= 30 的位置，然后往前扫描就可获取目的文档。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307280935875.png)

MongoDB中的索引类似其他数据库系统，**定义索引在 collection level** ，并且支持在 一个MongoDB集合的文档中任一字段或子字段 *indexes on any field or sub-field of the documents in a MongoDB collection* 的索引。

了解：MongoDB索引使用B树数据结构（确切的说是B-Tree，MySQL是B+Tree）
## Default `_id` 索引 ※
MongoDB会在 [`_id`](https://www.mongodb.com/docs/manual/core/document/#std-label-document-id-field) 上创建一个 [*unique index*](https://www.mongodb.com/docs/manual/core/index-unique/#std-label-index-type-unique) 唯一索引（在一个 collection 创建时），`_id` 预防客户端插入 两个有相同 `_id` 字段的文档。**你无法删除 `_id` 字段上的索引**。
> 在 [sharded clusters](https://www.mongodb.com/docs/manual/reference/glossary/#std-term-sharded-cluster) 中，如果你不使用 `_id` 字段作为 [shard key](https://www.mongodb.com/docs/manual/reference/glossary/#std-term-shard-key)，那么你的应用 **必须** 保证 **the uniqueness of the values in the `_id` field** 来预防错误。This is most-often done by 使用一个 standard auto-generated [ObjectId](https://www.mongodb.com/docs/manual/reference/glossary/#std-term-ObjectId)。？？

## 创建一个索引
在 Mongo Shell 中，使用 `db.collection.createIndex()` ，这个命令只会创建 an index if an index of the same specification 没有已经存在：
```shell
db.collection.createIndex( <key and index type specification>, <options> )
```
如下创建一个 single key descending index 在 `name` 字段上：
```
db.collection.createIndex( { name: -1 } )
```

Java (Async)
To create an index using the [Async Java driver](http://mongodb.github.io/mongo-java-driver/3.0/driver-async/), use [com.mongodb.async.client.MongoCollection.createIndex](http://mongodb.github.io/mongo-java-driver/3.4/javadoc/?com/mongodb/async/client/MongoCollection.html#createIndex-org.bson.conversions.Bson-com.mongodb.async.SingleResultCallback-).
```
collection.createIndex( <key and index type specification>, <options>, <callbackFunction>)
```
The following example creates a single key descending index on the `name` field:
```
collection.createIndex(Indexes.descending("name"), someCallbackFunction());
```

Java (Sync)
To create an index using the [Java driver](https://mongodb.github.io/mongo-java-driver/), use [com.mongodb.client.MongoCollection.createIndex](http://mongodb.github.io/mongo-java-driver/3.4/javadoc/?com/mongodb/client/MongoCollection.html#createIndex-org.bson.conversions.Bson-).

```
collection.createIndex( <key and index type specification>, <options> )
```
The following example creates a single key descending index on the `name` field:
```
collection.createIndex(Indexes.descending("name"));
```
上面两个都只在 索引不存在时 创建。
## 索引 Names
一个索引的默认名称是 concatenation of the indexed keys and each key's direction in the index (1 or -1) using underscores as a separator. 例如，一个被创建的索引 `{ item: 1, quantity: -1 }` 有名字 `item_1_quantity_-1` 。

创建时刻自定义索引名，更加 human-readable 。考虑一个应用 频繁查询 `products` 集合来 populate data on existing inventory ，下面创建索引的方法，创建一个在 `item` 和 `quantity` 上的索引，名为 `query for inventory` ：
```shell
db.products.createIndex (
    { item: 1, quantity: -1},
    { name : "query for inventory" }
)
```
## 查看索引名称
使用 `db.collection.getIndexes` 方法，cannot rename an index once created. 反而，必须 drop and re-create 一个索引来使用新名字。

## 索引的类型
MongoDB提供一系列不同的 索引类型 ，支持特定类型的 数据和查询。
### 4.2.1 单字段索引 Single Field
除了 MongoDB定义的 `_id` 索引，MongoDB支持==在 [文档的任一单个字段](https://www.mongodb.com/docs/manual/core/index-single/)上创建用户定义的升序/降序索引==，称为单字段索引（Single Field Index）。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303091048443.png)
对于**单个字段的索引和排序操作**，索引键的排序顺序（即升序或降序）并不重要，因为MongoDB可以在任何方向 *either direction* 上遍历索引。
看 [Single Field Indexes](https://www.mongodb.com/docs/manual/core/index-single/) 和 [Sort with a Single Field Index](https://www.mongodb.com/docs/manual/tutorial/sort-results-with-indexes/#std-label-sort-results-single-field) 获取更多信息。没有更多信息……
#### 创建一个 升序索引 在一个 Single Field 上
考虑一个 `schools` 集合，维护下列的文档：
```
db.schools.insertOne(
   {
      "_id": ObjectId("570c04a4ad233577f97dc459"),
      "studentsEnrolled": 1034,
      "location": { state: "NY", city: "New York" }
   }
)
```
下列操作创建一个 升序索引 在 `studentsEnrolled` 字段上，index specification 中字段的值 指明索引的类型 ，1表示 orders items in a ascending order ，-1 表示 orders items in descending order 。查看[index type](https://www.mongodb.com/docs/manual/indexes/#std-label-index-types)：
```
db.schools.createIndex( { studentsEnrolled: 1} )
```
被创建的索引，支持 queries that select on the field `studentsEnrolled` ，例如：
```js
db.schools.find( { studentsEnrolled: 1034 })
db.schools.find( { studentsEnrolled: { $gt: 500 }} )
```
#### 为一个 Embedded Field创建索引
可以为 fields within embedded documents 创建索引，就像你能 index top-level fields in documents. 然而，indexes on embedded fields 不同于 [indexes on embedded documents](https://www.mongodb.com/docs/manual/core/index-single/#std-label-index-embedded-documents) ，后者include full content up to the maximum [index size](https://www.mongodb.com/docs/manual/reference/limits/#mongodb-limit-Index-Key-Limit) of the embedded document in the index 嵌入式字段上的索引与嵌入式文档上的索引不同，后者包括索引中嵌入文档的完整内容，直到嵌入式文档的最大索引大小。相反，indexes on embedded fields 允许你使用一个 dot notation 来 introspect into embedded documents.

考虑一个 `records` 集合，存储文档类似：
```
{
  "_id": ObjectId("570c04a4ad233577f97dc459"),
  "studentsEnrolled": 1034,
  "location": { state: "NY", city: "New York" }
}
```
下列操作创建一个索引，在 `location.state` 字段上：
```
db.schools.createIndex( { "location.state": 1 } )
```
创建的索引支持 queries that select on the field `location.state` ：
```
db.schools.find( { "location.state": "CA" } )
db.schools.find( { "location.city": "Albany", "location.state": "NY" }) // 可以命中索引
```
a,b,c -> c,b,a  c,a  a 都可命中索引，与查询时字段的顺序无关，只与字段是否最左前缀匹配有关！和MySQL一样
a and b or c 不会使用
#### 为一个 Embedded Document 创建索引
为一个 embedded document as a whole 创建一个索引，还是上面的集合：
```
{
  "_id": ObjectId("570c04a4ad233577f97dc459"),
  "studentsEnrolled": 1034,
  "location": { state: "NY", city: "New York" }
}
```
`location` 字段是一个嵌入式文档，包含嵌入式字段 `city, state` ，下列创建索引 在 `location` 字段上 as a whole:
```
db.schools.createIndex( { location: 1 } )
```
下列的查询能使用 `location` 字段上的索引：
```
db.schools.find( { location: { 
    city: "New York",
    state: "NY"
}})
```
虽然查询可以使用索引，但结果集不包括上面的示例文档。==在对嵌入式文档进行相等匹配  equality matches on embedded documents 时，字段顺序很重要，而且嵌入式文档必须完全匹配 *field order matters and the embedded documents must match exactly*== 。见[Query Embedded Documents](https://www.mongodb.com/docs/manual/reference/method/db.collection.find/#query-embedded-documents "Permalink to this heading")] 以及  [Query on Embedded/Nested Documents](https://www.mongodb.com/docs/manual/tutorial/query-embedded-documents/)。

#### 额外的考虑：
在索引构建过程中，应用程序可能会遇到性能降低或对「正在索引的集合」的读写访问受到限制。有关索引构建过程的更多信息，请参见 [Index Builds on Populated Collections](https://www.mongodb.com/docs/manual/core/index-creation/#std-label-index-operations) ，特别是其中的 [Index Builds in Replicated Environments]() 部分。TODO


## 4.2.2 复合索引
MongoDB还支持多个字段的用户定义索引，即复合索引 [*Compound Index*](https://www.mongodb.com/docs/manual/core/index-compound/)，where a single index structure holds reference to multiple fields within a collection's documents 单个索引结构保存了集合文档内多个字段的引用，支持 queries that match on multiple fields。MongoDB imposes a [limit of 32 fields for any compound index.](https://www.mongodb.com/docs/manual/reference/limits/#mongodb-limit-Number-of-Indexed-Fields-in-a-Compound-Index)

#### 创建一个复合索引
使用下面的操作创建：
```
db.collection.createIndex( { <field1>: <type>, <field2>: <type2>, ... } )
```
（创建时）**复合索引中列出的字段顺序 *the order of the indexed fields listed in a compound index* 具有重要意义**。例如，如果复合索引由{ userid: 1 , score: - 1 }组成，则索引首先按userid正序排序，然后在每个userid的值内，再在按score倒序排序。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307281100202.png)
> MongoDB 4.4: 复合索引可以包含 *a single [hashed index field](https://www.mongodb.com/docs/manual/core/index-hashed/)* ，如果在包含超过1个 hashed index field 上创建复合查询会报错。
> MongoDB 4.2 及以前：may not contain a hashed index field
 
考虑一个集合 `products` ，包含如下文档：
```js
{
 "_id": ObjectId(...),
 "item": "Banana",
 "category": ["food", "produce", "grocery"],
 "location": "4th Street Store",
 "stock": 4,
 "type": "cases"
}
```
下列的操作创建一个升序索引在 `item` 和 `stock` 上：
```js
db.products.createIndex( { "item": 1, "stock": 1 } )
```
复合索引中列出的字段的顺序是很重要的！该索引将包含**对文档的引用**，首先按照 `item` 字段的值排序，然后在每个 `item` 字段的值中，按照 `stock` 字段的值排序。有关更多信息，请参见[Sort Order]。

除了支持[匹配所有索引字段] match on all the index fields 的查询外，复合索引还支持在索引字段的前缀上匹配的查询 queries that match on the prefix of the index fields 。也就是说，该索引支持对 `item` 字段以及 `item` 和 `stock` 字段同时进行查询：
```js
db.products.find( { item: "Banana" } )
db.products.find( { item: "Banana", stock: { $gt: 5 } } )
```
更多看[Prefixes](https://www.mongodb.com/docs/manual/core/index-compound/#std-label-compound-index-prefix)。

对于复合的索引和排序操作，**索引键的排序顺序**（即升序或降序）可以确定索引是否支持排序操作。有关索引的顺序对复合索引结果的影响的更多信息，请参见 [Sort Order](https://www.mongodb.com/docs/manual/core/index-compound/#std-label-index-ascending-and-descending) 。

字段顺序，字段的排序顺序
### Sort Order
索引按升序（1）或降序（-1）排序存储字段的引用。**对于单字段索引，键的排序顺序无关紧要，因为MongoDB可以沿着任何方向遍历索引**。但对于复合索引，排序顺序可能会影响确定索引是否支持排序操作。

考虑一个包含 `username` 和 `date` 字段的文档的集合 `events` 。应用程序可以发出按升序 `username` 值排序，然后按降序（即从最近到最后）`date` 值排序的查询，例如：
```js
db.events.find().sort( { username: 1, date: -1 } )
```
或者先降序 `username` 然后升序 `date` ：
```js
db.events.find().sort( { username: -1, date: 1 } )
```
下列的索引，能支持以上两个 sort 操作：
```js
db.events.createIndex( { "username" : 1, "date" : -1 } )
```
然而，以上的索引**无法支持** 升序 `username` 接着升序 `date` ，例如：
```js
db.events.find().sort( { username: 1, date: 1 } )
```
更多信息见 [Use Indexes to Sort Query Results](https://www.mongodb.com/docs/manual/tutorial/sort-results-with-indexes/) 。
### Prefixes
索引前缀是 the beginning **subsets** of indexed fields ，例如，考虑下列复合索引：
```js
{ "item": 1, "location": 1, "stock": 1 }
```
这个索引有下列的 index prefixes：
- `{ item: 1}`
- `{ item: 1, location: 1}`

对于一个复合索引，MongodDB
### 4.2.3 其他索引
地理空间索引（Geospatial Index）、文本索引（Text Indexes）、哈希索引（Hashed Indexes）。
1. 地理空间索引（Geospatial Index）
	为了支持对地理空间坐标数据的有效查询，MongoDB提供了两种特殊的索引：返回结果时使用平面几何的二维索引和返回结果时使用球面几何的二维球面索引。
2. 文本索引（Text Indexes）
	MongoDB提供了一种文本索引类型，支持在集合中搜索字符串内容。这些文本索引不存储特定于语言的停止词（例如“the”、“a”、“or”），而将集合中的词作为词干，只存储根词。
3. 哈希索引（Hashed Indexes）
	

## 哈希索引 Hashed Indexes
MongoDB提供了散列索引类型，它对字段值的散列进行索引 *maintain entries with hashes of the values of the indexed field* 。
1. 哈希索引使用 hashed shard key 支持 [sharding](https://www.mongodb.com/docs/manual/sharding/)。[Hashed sharding](https://www.mongodb.com/docs/manual/core/hashed-sharding/#std-label-sharding-hashed-sharding)使用 字段的 hashed index 作为 shard key 来在 sharded cluster 上划分数据。使用一个 hashed shard key 来对一个集合进行分片 shard 会导致 更均匀的数据分布，见 [Hashed sharding](https://www.mongodb.com/docs/manual/core/hashed-sharding/#std-label-sharding-hashed-sharding) 。
2. 这些索引在其范围内的值分布更加随机，**但只支持相等匹配**，不支持基于范围的查询。
#### hashing function
哈希索引使用哈希函数计算索引字段值的哈希值[1]。哈希函数会折叠嵌入式文档 collapses embedded documents 并计算整个值的哈希值，**但不支持多键 multi-key（即数组）索引**。具体而言，在包含数组的字段上创建哈希索引、或尝试将数组插入到哈希索引字段中都会返回错误。
> MongoDB在使用哈希索引解析查询时会自动计算哈希值。**应用程序无需计算哈希值**。

> 1: 从版本4.0开始，mongosh提供了 `convertShardKeyToHashed()` 方法。该方法使用与哈希索引相同的哈希函数，可用于查看密钥的哈希值。

#### 创建哈希索引
要创建哈希索引，请将hashed指定为索引键 index key 的值，如以下示例所示：
```
db.collection.createIndex( { _id: "hashed" } )
```
#### 创建复合哈希索引 compound hashed index
从MongoDB 4.4开始，MongoDB支持创建**包括单个哈希字段** a single hashed field 的复合索引。==要创建复合哈希索引，请在创建索引时将hashed指定为任何单个索引键的值==：
```
db.collection.createIndex( { "fieldA" : 1, "fieldB" : "hashed", "fieldC" : -1 } )
```
复合哈希索引需要将featureCompatibilityVersion设置为4.4。
#### Considerations
唯一约束  
**MongoDB不支持在哈希索引上指定唯一约束**。您可以创建一个额外的非哈希索引，并在该字段上设置唯一约束。MongoDB可以使用**该非哈希索引**强制执行该字段上的唯一性。

**MongoDB哈希索引在哈希之前将浮点数截断为64位整数**。例如，哈希索引将为包含值2.3、2.2和2.9的字段存储相同的值。为了防止碰撞，不要对无法可靠转换为64位整数（然后再转换为浮点数）的浮点数使用哈希索引。MongoDB哈希索引不支持大于2的53次方的浮点值。要列出部署中所有集合的所有哈希索引，可以在mongosh中使用以下操作：
```js
db.adminCommand("listDatabases").databases.forEach(function(d){
   let mdb = db.getSiblingDB(d.name);
   mdb.getCollectionInfos({ type: "collection" }).forEach(function(c){
      let currentCollection = mdb.getCollection(c.name);
      currentCollection.getIndexes().forEach(function(idx){
        let idxValues = Object.values(Object.assign({}, idx.key));

        if (idxValues.includes("hashed")) {
          print("Hashed index: " + idx.name + " on " + d.name + "." + c.name);
          printjson(idx);
        };
      });
   });
});
```
To check if the indexed field contains the value 2 63, run the following operation for the collection and the indexed field:
- If the indexed field type is a scalar and never a document:
    ```
    // substitute the actual collection name for <collection>
    // substitute the actual indexed field name for <indexfield>
    db.<collection>.find( { <indexfield>: Math.pow(2,63) } );
    ```
- If the indexed field type is a document (or a scalar), you can run:
    ```JS
    // substitute the actual collection name for <collection>
    // substitute the actual indexed field name for <indexfield>
    
    db.<collection>.find({
        $where: function() {
            function findVal(obj, val) {
                if (obj === val)
                    return true;
    
                for (const child in obj) {
                    if (findVal(obj[child], val)) {
                        return true;
                    }
                }
                return false;
            }
            return findVal(this.<indexfield>, Math.pow(2, 63));
        }
    })
    ```
