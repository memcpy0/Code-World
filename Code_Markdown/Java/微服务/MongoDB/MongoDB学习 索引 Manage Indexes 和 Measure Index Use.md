如何管理已经存在的索引。
## 查看存在的索引
查看一个 Collection 中的所有 indexes ：
```js
db.collection.getIndexes()
```
查看一个数据库中的所有索引，mongosh命令：
```js
db.getCollectionNames().forEach(function(collection) {
    indexes = db[collection].getIndexes();
    print("Indexes for " + collection + ":");
    printjson(indexes);
});
```
列出特定类型的索引（所有数据库中所有集合的特定类型）：
```js
// The following finds all hashed indexes

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
### 删除索引
隐藏索引，在删除前：如果删除了正在生产环境中使用的索引，应用会 incur a performance degradation. 在丢弃索引前，可以估算潜在的 drop 影响，通过 [hiding the index](https://www.mongodb.com/docs/manual/core/index-hidden/#std-label-hide-existing-index) 。**隐藏的索引不用于支持查询**。如果您隐藏了一个索引并观察到了显著的负面性能影响，请考虑保留并取消隐藏该索引，以便查询可以恢复使用它。

移除特定索引：`db.collection.dropIndex()` 。下面移除了一个升序的索引 `tax-id` 在 `accounts` 集合中。
```js
db.accounts.dropIndex( { "tax-id": 1 } )
```
这个操作返回一个文档，包括操作的执行状态：
```js
{ "nIndexesWas" : 3, "ok" : 1 }
```
其中nIndexesWas的值反映了删除该索引前的索引数量。

For [text](https://www.mongodb.com/docs/manual/core/index-text/) indexes, pass the index name to the [`db.collection.dropIndex()`](https://www.mongodb.com/docs/manual/reference/method/db.collection.dropIndex/#mongodb-method-db.collection.dropIndex) method. See [Use the Index Name to Drop a `text` Index](https://www.mongodb.com/docs/manual/tutorial/avoid-text-index-name-limit/#std-label-drop-text-index) for details.
> Starting in MongoDB 4.2, [`db.collection.dropIndexes()`](https://www.mongodb.com/docs/manual/reference/method/db.collection.dropIndexes/#mongodb-method-db.collection.dropIndexes) can accept an array of index names.
> 
> Starting in MongoDB 4.4, [`db.collection.dropIndexes()`](https://www.mongodb.com/docs/manual/reference/method/db.collection.dropIndexes/#mongodb-method-db.collection.dropIndexes) can stop in-progress index builds. See [Stop In-Progress Index Builds](https://www.mongodb.com/docs/manual/reference/method/db.collection.dropIndexes/#std-label-dropIndexes-method-index-builds) for more information.

移除所有索引：`db.collection.dropIndexes` ，除了不移除 `_id` index 。下面移除了所有索引，从 `accounts` 中：
```js
db.accounts.dropIndexes();
```
### 修改一个索引
To modify an existing index in the MongoDB Shell, you need to drop and recreate the index. The exception to this rule is [TTL indexes](https://www.mongodb.com/docs/manual/core/index-ttl/), which can be modified via the [`collMod`](https://www.mongodb.com/docs/manual/reference/command/collMod/#mongodb-dbcommand-dbcmd.collMod) command in conjunction with the [`index`](https://www.mongodb.com/docs/manual/reference/command/collMod/#mongodb-collflag-index) collection flag.

修改索引需要 drop 并 recreate 索引。为了最小化 drop 的性能影响：
### Minimize Performance Impact With a Temporary Index[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/tutorial/manage-indexes/#minimize-performance-impact-with-a-temporary-index "Permalink to this heading")
If you drop an index that is actively used in production, your application may incur a performance degradation. To ensure queries can still use an index during modification, you can ==create a temporary, redundant index that contains the same fields as the modified index.==
示例：https://www.mongodb.com/docs/manual/tutorial/manage-indexes/#example

## 4.3 索引的管理操作
### 4.3.1 索引的查看
说明：返回一个集合中的所有索引的数组。语法：
```js
db.collection.getIndexes()
```
提示：该语法命令运行要求是MongoDB 3.0+

【示例】查看comment集合中所有的索引情况
```js
db.comment.getIndexes() [
	{
		"v" : 2,
			"key" : {
			"_id" : 1
		},
		"name" : "_id_",
		"ns" : "articledb.comment"
	}
]
```
结果中显示的是默认 `_id` 索引。

默认 `_id` 索引：MongoDB在创建集合的过程中，在_id字段上创建一个唯一的索引，默认名字为 `_id` ，该索引可**防止客户端插入两个具有相同值的文档**，您**不能在 `_id` 字段上删除此索引**。

注意：**该索引是唯一索引，因此值不能重复，即_id值不能重复**。在分片集群中，通常使用_id作为片键。
### 4.3.2 索引的创建
说明：在集合上创建索引。语法：
```js
db.collection.createIndex(keys, options)
```

 | Parameter  | Type | Description |
 |:----|:----|:---|
 | keys  | document | 包含字段和值对的文档，其中字段是索引键，值描述该字段的索引类型。
 |   |   |  对于字段上的升序索引，请指定值 1 ；对于降序索引，请指定值-1。
 |   |    | 比如：{字段: 1 或- 1 }，其中 1 为指定按升序创建索引，如果你想按降序来创建索引指定为-1即可。
 |  |  | 另外，MongoDB支持几种不同的索引类型，包括文本、地理空间和哈希索引。
 | options | document |  可选。包含一组控制索引创建的选项的文档。有关详细信息，请参见选项详情列表。

options（更多选项）列表：

| Parameter | Type | Description |
|:---|:---|:---|
| background | Boolean | **建索引过程会阻塞其它数据库操作**，background可指定以后台方式创建索引，即增加"background" 可选参数。 "background" 默认值为false。
| unique | Boolean | 建立的索引是否唯一。指定为true创建唯一索引。默认值为false.
| name | string | 索引的名称。如果未指定，MongoDB的通过连接索引的字段名和排序顺序生成一个索引名称。
| dropDups | Boolean | **3.0+版本已废弃**。在建立唯一索引时是否删除重复记录,指定 true 创建唯一索引。默认值为false.
| sparse | Boolean | 对文档中不存在的字段数据不启用索引；这个参数需要特别注意，如果设置为true的话，在索引字段中不会查询出不包含对应字段的文档.。默认值为 false.
|  expireAfterSeconds | integer | 指定一个以秒为单位的数值，完成 TTL设定，设定集合的生存时间。
| v | index version | **索引的版本号**。默认的索引版本取决于mongod创建索引时运行的版本。
| weights | document |  索引权重值，数值在 1 到 99,999 之间，表示该索引相对于其他索引字段的得分权重。
| default_language | string | 对于文本索引，该参数决定了停用词及词干和词器的规则的列表。 默认为英语
| language_override | string| 对于文本索引，该参数指定了包含在文档中的字段名，语言覆盖默认的language，默认值为language.

提示：注意在3.0.0版本前创建索引方法为 `db.collection.ensureIndex()` ，之后的版本使用了  `db.collection.createIndex()` 方法，`ensureIndex()` 还能用，但只是 createIndex()的别名。

【示例】
（1）单字段索引示例：对userid字段建立索引：
```js
> db.comment.createIndex({userid:1}) 
{
	"createdCollectionAutomatically" : false,
	"numIndexesBefore" : 1,
	"numIndexesAfter" : 2,
	"ok" : 1
}
```
参数 1 ：按升序创建索引。可以查看一下：
```js
> db.comment.getIndexes()
[
	{
		"v" : 2,
		"key" : {
			"_id" : 1
		},
		"name" : "_id_",
		"ns" : "articledb.comment"
	},
	{
		"v" : 2,
		"key" : {
			"userid" : 1
		},
		"name" : "userid_1",
		"ns" : "articledb.comment"
	}
]
```
索引名字为 `userid_1` 。Compass查看：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303091100251.png)

（2）复合索引：对 `userid` 和 `nickname` 同时建立复合（Compound）索引：
```js
> db.comment.createIndex({userid:1,nickname:-1})
{
	"createdCollectionAutomatically" : false,
	"numIndexesBefore" : 2,
	"numIndexesAfter" : 3,
	"ok" : 1
}
```
查看一下索引：
```js
> db.comment.getIndexes()
[
	{
		"v" : 2,
		"key" : {
			"_id" : 1
		},
		"name" : "_id_",
		"ns" : "articledb.comment"
	},
	{
		"v" : 2,
		"key" : {
			"userid" : 1
		},
		"name" : "userid_1",
		"ns" : "articledb.comment"
	},
	{
		"v" : 2,
		"key" : {
			"userid" : 1,
			"nickname" : -1
		},
		"name" : "userid_1_nickname_-1",
		"ns" : "articledb.comment"
	}
]
```
Compass中：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303091102184.png)
### 4.3.3 索引的移除
说明：可以移除指定的索引，或移除所有索引
#### 1. 指定索引的移除
语法：
```js
db.collection.dropIndex(index)
```
参数：
| Parameter | Type | Description
| index | string or document | 指定要删除的索引。可以通过索引名称或索引规范文档指定索引。**若要删除文本索引，请指定索引名称**。

【示例】删除comment集合中userid字段上的升序索引：
```js
> db.comment.dropIndex({userid:1})
{ "nIndexesWas" : 3, "ok" : 1 }
```
 查看已经删除了。

#### 2. 所有索引的移除
语法：
```js
db.collection.dropIndexes()
```
【示例】删除spit集合中所有索引。
```js
> db.comment.dropIndexes()
{
"nIndexesWas" : 2,
"msg" : "non-_id indexes dropped for collection",
"ok" : 1
}
```
 提示：`_id` 的字段的索引是无法删除的，只能删除非 `_id` 字段的索引。

---
### 找到跨分片的不一致索引
==如果该集合没有完全一样的索引（包括索引选项）在每个分片（包含这个集合的数据 chunks ），那么该分片集合 sharded collection 有一个 inconsistent index ==。虽然在正常操作期间不应发生不一致的索引，但可能会发生不一致的索引，例如：
- 当用户使用唯一键约束创建索引，且一个分片包含具有重复文档的块时，创建索引操作可能在没有重复项的分片上成功，但在具有重复项的分片上失败。
- 当用户以滚动方式 [rolling manner](https://www.mongodb.com/docs/manual/tutorial/build-indexes-on-sharded-clusters/) 在分片之间创建索引（即手动逐个在分片之间构建索引），但未能为关联的分片构建索引或使用不同的规范不正确地构建索引时，也可能出现不一致的索引。

从MongoDB 4.4（和4.2.6）开始，默认情况下，**配置服务器主机会检查分片集合 sharded collections 的索引不一致性**，命令 [`serverStatus`](https://www.mongodb.com/docs/manual/reference/command/serverStatus/#mongodb-dbcommand-dbcmd.serverStatus) 在配置服务器主机上运行时，返回字段 `shardedIndexConsistency` 以报告存在索引不一致性的分片集合数量。

如果 `shardedIndexConsistency` 报告任何索引不一致性，您可以针对您的分片集合运行以下管道，直到找到不一致性为止。
```js
const pipeline = [
    // Get indexes and the shards that they belong to.
    {$indexStats: {}},
    // Attach a list of all shards which reported indexes to each document from $indexStats.
    {$group: {_id: null, indexDoc: {$push: "$$ROOT"}, allShards: {$addToSet: "$shard"}}},
    // Unwind the generated array back into an array of index documents.
    {$unwind: "$indexDoc"},
    // Group by index name.
    {
        $group: {
            "_id": "$indexDoc.name",
            "shards": {$push: "$indexDoc.shard"},
            // Convert each index specification into an array of its properties
            // that can be compared using set operators.
            "specs": {$push: {$objectToArray: {$ifNull: ["$indexDoc.spec", {}]}}},
            "allShards": {$first: "$allShards"}
        }
    },
    // Compute which indexes are not present on all targeted shards and
    // which index specification properties aren't the same across all shards.
    {
        $project: {
            missingFromShards: {$setDifference: ["$allShards", "$shards"]},
            inconsistentProperties: {
                 $setDifference: [
                     {$reduce: {
                         input: "$specs",
                         initialValue: {$arrayElemAt: ["$specs", 0]},
                         in: {$setUnion: ["$$value", "$$this"]}}},
                     {$reduce: {
                         input: "$specs",
                         initialValue: {$arrayElemAt: ["$specs", 0]},
                         in: {$setIntersection: ["$$value", "$$this"]}}}
                 ]
             }
        }
    },
    // Only return output that indicates an index was inconsistent, i.e. either a shard was missing
    // an index or a property on at least one shard was not the same on all others.
    {
        $match: {
            $expr:
                {$or: [
                    {$gt: [{$size: "$missingFromShards"}, 0]},
                    {$gt: [{$size: "$inconsistentProperties"}, 0]},
                ]
            }
        }
    },
    // Output relevant fields.
    {$project: {_id: 0, indexName: "$$ROOT._id", inconsistentProperties: 1, missingFromShards: 1}}
];
```
1. Run the aggregation pipeline for the sharded collection to test. For example, to test if the sharded collection `test.reviews` has inconsistent indexes across its associated shards:
    
    ```
    db.getSiblingDB("test").reviews.aggregate(pipeline)
    ```
    
    If the collection has inconsistent indexes, the aggregation for that collection returns details regarding the inconsistent indexes:
    
    ```
    { "missingFromShards" : [ "shardB" ], "inconsistentProperties" : [ ], "indexName" : "page_1_score_1" }{ "missingFromShards" : [ ], "inconsistentProperties" : [ { "k" : "expireAfterSeconds", "v" : 60 }, { "k" : "expireAfterSeconds", "v" : 600 } ], "indexName" : "reviewDt_1" }
    ```
    
    The returned documents indicate two inconsistencies for the sharded collection `test.reviews`:
    1. An index named `page_1_score_1` is missing from the collection on `shardB`.
    2. An index named `reviewDt_1` has inconsistent properties across the collection's shards, specifically, the `expireAfterSeconds` properties differ.

To resolve the inconsistency where an index is missing from the collection on a particular shard(s),

You can either:
- Perform a [rolling index build](https://www.mongodb.com/docs/manual/tutorial/build-indexes-on-sharded-clusters/) for the collection on the affected shard(s).
    -OR-
- Issue an index build [`db.collection.createIndex()`](https://www.mongodb.com/docs/manual/reference/method/db.collection.createIndex/#mongodb-method-db.collection.createIndex) from a [`mongos`](https://www.mongodb.com/docs/manual/reference/program/mongos/#mongodb-binary-bin.mongos) instance. The operation only builds the collection's index on the shard(s) missing the index.
    

To resolve where the index properties differ across the shards,

Drop the incorrect index from the collection on the affected shard(s) and rebuild the index. To rebuild the index, you can either:
- Perform a [rolling index build](https://www.mongodb.com/docs/manual/tutorial/build-indexes-on-sharded-clusters/) for the collection on the affected shard.
    -OR-
- Issue an index build [`db.collection.createIndex()`](https://www.mongodb.com/docs/manual/reference/method/db.collection.createIndex/#mongodb-method-db.collection.createIndex) from a [`mongos`](https://www.mongodb.com/docs/manual/reference/program/mongos/#mongodb-binary-bin.mongos) instance. The operation only builds the collection's index on the shard(s) missing the index.

Alternatively, if the inconsistency is the `expireAfterSeconds` property, you can run the [`collMod`](https://www.mongodb.com/docs/manual/reference/command/collMod/#mongodb-dbcommand-dbcmd.collMod) command to update the number of seconds instead of dropping and rebuilding the index.


---
## 4.4 索引的使用
### Get Index Access Information
使用 [`$indexStats`](https://www.mongodb.com/docs/manual/reference/operator/aggregation/indexStats/#mongodb-pipeline-pipe.-indexStats) aggregation stage 去获取关于 一个集合中每个索引的使用的数据。例如，下列的 aggregation 操作返回 `orders` 集合的索引使用数据。
```js
db.orders.aggregate( [ { $indexStats: { } } ] )
```

### 4.4.1 执行计划
分析查询性能（Analyze Query Performance）通常使用执行计划（解释计划、Explain Plan）来查看查询的情况，如查询耗费的时间、是否基于索引查询等。那么，通常我们想知道，建立的索引是否有效，效果如何，都需要通过执行计划查看。

语法：Use the [`db.collection.explain()`](https://www.mongodb.com/docs/manual/reference/method/db.collection.explain/#mongodb-method-db.collection.explain) or the [`cursor.explain()`](https://www.mongodb.com/docs/manual/reference/method/cursor.explain/#mongodb-method-cursor.explain) method in [executionStats](https://www.mongodb.com/docs/manual/reference/method/db.collection.explain/#std-label-explain-method-executionStats) mode to return statistics about the query process, including the index used, the number of documents scanned, and the time the query takes to process in milliseconds.
```js
db.collection.find(query,options).explain(options)
```
【示例】查看根据userid查询数据的情况：
```js
> db.comment.find({userid:"1003"}).explain()
{
	"queryPlanner" : {
		"plannerVersion" : 1,
		"namespace" : "articledb.comment",
		"indexFilterSet" : false,
		"parsedQuery" : {
			"userid" : {
				"$eq" : "1003"
			}
		},
		"winningPlan" : {
			"stage" : "COLLSCAN",
			"filter" : {
				"userid" : {
					"$eq" : "1003"
				}
			},
			"direction" : "forward"
		},
		"rejectedPlans" : [ ]
	},
	"serverInfo" : {
		"host" : "9ef3740277ad",
		"port" : 27017,
		"version" : "4.0.10",
		"gitVersion" : "c389e7f69f637f7a1ac3cc9fae843b635f20b766"
	},
	"ok" : 1
}
```
关键点看："stage" : "COLLSCAN",表示全集合扫描
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303091137637.png)

下面对userid建立索引
```js
> db.comment.createIndex({userid:1})
{
	"createdCollectionAutomatically" : false,
	"numIndexesBefore" : 1,
	"numIndexesAfter" : 2,
	"ok" : 1
}
```
再次查看执行计划：
```js
> db.comment.find({userid:"1013"}).explain()
{
	"queryPlanner" : {
		"plannerVersion" : 1,
		"namespace" : "articledb.comment",
		"indexFilterSet" : false,
		"parsedQuery" : {
			"userid" : {
				"$eq" : "1013"
			}
		},
		"winningPlan" : {
			"stage" : "FETCH",
			"inputStage" : {
				"stage" : "IXSCAN",
				"keyPattern" : {
					"userid" : 1
				},
				"indexName" : "userid_1",
				"isMultiKey" : false,
				"multiKeyPaths" : {
					"userid" : [ ]
				},
				"isUnique" : false,
				"isSparse" : false,
				"isPartial" : false,
				"indexVersion" : 2,
				"direction" : "forward",
				"indexBounds" : {
					"userid" : [
					"[\"1013\", \"1013\"]"
					]
				}
			}
		},
		"rejectedPlans" : [ ]
	},
	"serverInfo" : {
		"host" : "9ef3740277ad",
		"port" : 27017,
		"version" : "4.0.10",
		"gitVersion" : "c389e7f69f637f7a1ac3cc9fae843b635f20b766"
	},
	"ok" : 1
}
```
关键点看："stage" : "IXSCAN",基于索引的扫描

Compass查看：
![450](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303091139281.png)

### 4.4.2 涵盖的查询
Covered Queries：当查询条件和查询的投影仅包含索引字段时，MongoDB直接从索引返回结果，而不扫描任何文档或将文档带入内存。 这些覆盖的查询可以非常有效。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303091140804.png)

更多：https://docs.mongodb.com/manual/core/query-optimization/#read-operations-covered-query

【示例】
```js
> db.comment.find({userid:"1003"},{userid:1,_id:0})
{ "userid" : "1003" }
{ "userid" : "1003" }
> db.comment.find({userid:"1003"},{userid:1,_id:0}).explain()
{
	"queryPlanner" : {
		"plannerVersion" : 1,
		"namespace" : "articledb.comment",
		"indexFilterSet" : false,
		"parsedQuery" : {
			"userid" : {
				"$eq" : "1003"
			}
		},
		"winningPlan" : {
			"stage" : "PROJECTION",
			"transformBy" : {
				"userid" : 1,
				"_id" : 0
			},
			"inputStage" : {
				"stage" : "IXSCAN",
				"keyPattern" : {
					"userid" : 1
				},
				"indexName" : "userid_1",
				"isMultiKey" : false,
				"multiKeyPaths" : {
					"userid" : [ ]
				},
				"isUnique" : false,
				"isSparse" : false,
				"isPartial" : false,
				"indexVersion" : 2,
				"direction" : "forward",
				"indexBounds" : {
					"userid" : [
						"[\"1003\", \"1003\"]"
					]
				}
			}
		},
		"rejectedPlans" : [ ]
	},
	"serverInfo" : {
		"host" : "bobohost.localdomain",
		"port" : 27017,
		"version" : "4.0.10",
		"gitVersion" : "c389e7f69f637f7a1ac3cc9fae843b635f20b766"
	},
	"ok" : 1
}
``` 
Compass中：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303091143322.png)

### ## Control Index Use with `hint()`[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/tutorial/measure-index-use/#control-index-use-with-hint-- "Permalink to this heading")

To _force_ MongoDB to use a particular index for a [`db.collection.find()`](https://www.mongodb.com/docs/manual/reference/method/db.collection.find/#mongodb-method-db.collection.find) operation, specify the index with the [`hint()`](https://www.mongodb.com/docs/manual/reference/method/cursor.hint/#mongodb-method-cursor.hint) method. Append the [`hint()`](https://www.mongodb.com/docs/manual/reference/method/cursor.hint/#mongodb-method-cursor.hint) method to the [`find()`](https://www.mongodb.com/docs/manual/reference/method/db.collection.find/#mongodb-method-db.collection.find) method. Consider the following example:

```
db.people.find(   { name: "John Doe", zipcode: { $gt: "63000" } }).hint( { zipcode: 1 } )
```

To view the execution statistics for a specific index, append to the [`db.collection.find()`](https://www.mongodb.com/docs/manual/reference/method/db.collection.find/#mongodb-method-db.collection.find) the [`hint()`](https://www.mongodb.com/docs/manual/reference/method/cursor.hint/#mongodb-method-cursor.hint) method followed by [`cursor.explain()`](https://www.mongodb.com/docs/manual/reference/method/cursor.explain/#mongodb-method-cursor.explain), e.g.:

```
db.people.find(   { name: "John Doe", zipcode: { $gt: "63000" } }).hint( { zipcode: 1 } ).explain("executionStats")
```

Or, append [`hint()`](https://www.mongodb.com/docs/manual/reference/method/cursor.hint/#mongodb-method-cursor.hint) method to [`db.collection.explain().find()`:](https://www.mongodb.com/docs/manual/reference/method/db.collection.explain/#mongodb-method-db.collection.explain)

```
db.people.explain("executionStats").find(   { name: "John Doe", zipcode: { $gt: "63000" } }).hint( { zipcode: 1 } )
```

Specify the `$natural` operator to the [`hint()`](https://www.mongodb.com/docs/manual/reference/method/cursor.hint/#mongodb-method-cursor.hint) method to prevent MongoDB from using _any_ index:

```
db.people.find(   { name: "John Doe", zipcode: { $gt: "63000" } }).hint( { $natural: 1 } )
```

## Index Metrics[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/tutorial/measure-index-use/#index-metrics "Permalink to this heading")
In addition to the [`$indexStats`](https://www.mongodb.com/docs/manual/reference/operator/aggregation/indexStats/#mongodb-pipeline-pipe.-indexStats) aggregation stage, MongoDB provides various index statistics that you may want to consider when analyzing index use for your database:
- In the output of [`serverStatus`:](https://www.mongodb.com/docs/manual/reference/command/serverStatus/#mongodb-dbcommand-dbcmd.serverStatus)
    - [`metrics.queryExecutor.scanned`](https://www.mongodb.com/docs/manual/reference/command/serverStatus/#mongodb-serverstatus-serverstatus.metrics.queryExecutor.scanned)        
    - [`metrics.operation.scanAndOrder`](https://www.mongodb.com/docs/manual/reference/command/serverStatus/#mongodb-serverstatus-serverstatus.metrics.operation.scanAndOrder)
        
- In the output of [`collStats`:](https://www.mongodb.com/docs/manual/reference/command/collStats/#mongodb-dbcommand-dbcmd.collStats)
    - [`totalIndexSize`](https://www.mongodb.com/docs/manual/reference/command/collStats/#mongodb-data-collStats.totalIndexSize)
    - [`indexSizes`](https://www.mongodb.com/docs/manual/reference/command/collStats/#mongodb-data-collStats.indexSizes)
        
- In the output of [`dbStats`:](https://www.mongodb.com/docs/manual/reference/command/dbStats/#mongodb-dbcommand-dbcmd.dbStats)
    - [`dbStats.indexes`](https://www.mongodb.com/docs/manual/reference/command/dbStats/#mongodb-data-dbStats.indexes)
    - [`dbStats.indexSize`](https://www.mongodb.com/docs/manual/reference/command/dbStats/#mongodb-data-dbStats.indexSize)
