为您的应用程序选择最佳索引必须考虑多个因素，包括**您预期的查询类型，读写比率**以及系统上的可用内存量。

在开发索引策略时，您应深入了解你的应用程序的查询。
在构建索引之前，绘制出将运行的查询类型，以便可以构建引用这些字段的索引。索引带有性能成本，但针对大型数据集上的频繁查询而言，其价值远远超过成本。考虑应用程序中每个查询的相对频率以及查询是否有足够的理由使用索引。

==设计索引的最佳策略是，使用与生产环境中将要运行的数据集类似的数据集配置进行分析==，以确定哪种配置性能最佳。检查您的集合所创建的当前索引，以确保它们支持您当前和计划中的查询。如果不再使用索引，请删除索引。

通常，MongoDB仅使用一个索引来满足大多数查询。但是，$or查询的每个子句可能使用不同的索引，并且此外，MongoDB可以使用多个索引的[intersection](https://www.mongodb.com/docs/manual/core/index-intersection/)。

以下文档介绍索引策略：
1. [Create Indexes to Support Your Queries](https://www.mongodb.com/docs/manual/tutorial/create-indexes-to-support-queries/#std-label-create-indexes-to-support-queries)
  当索引包含查询扫描的所有字段时，索引支持查询，查询会扫描索引而非集合。**创建支持查询的索引**可显著提高查询性能。
2. [Use the ESR (Equality, Sort, Range) Rule](https://www.mongodb.com/docs/manual/tutorial/equality-sort-range-rule/#std-label-esr-indexing-rule)
    ESR（相等性、排序、范围）规则是一种指导**创建支持查询的索引**的有效方法。
3. [Use Indexes to Sort Query Results](https://www.mongodb.com/docs/manual/tutorial/sort-results-with-indexes/#std-label-sorting-with-indexes)
    To support efficient queries, use the strategies here when you specify the sequential order and sort order of index fields 为了支持高效查询，请在**指定索引字段的顺序和排序顺序**时使用这里的策略。
4. [Ensure Indexes Fit in RAM](https://www.mongodb.com/docs/manual/tutorial/ensure-indexes-fit-ram/#std-label-indexes-ensure-indexes-fit-ram
    当**索引适合RAM**时，系统可以避免从磁盘读取索引，从而获得最快的处理速度。
5. [Create Queries that Ensure Selectivity](https://www.mongodb.com/docs/manual/tutorial/create-queries-that-ensure-selectivity/#std-label-index-selectivity)
    Selectivity is the ability of a query to narrow results using the index. Selectivity allows MongoDB to use the index for a larger portion of the work associated with fulfilling the query. **选择性是查询使用索引缩小结果的能力**。选择性允许MongoDB在执行查询相关工作的较大部分中使用索引。

## 1. 创建支持你查询的索引
### 1.1 如果所有查询使用相同的单个 key ，创建一个单键索引
If you only ever query on a single key in a given collection, then you need to create just one single-key index for that collection.
### 1.2 创建复合索引，支持多个不同查询
如果有时仅在一个键上进行查询，而其他时间则在该键与第二个键组合后进行查询，则创建复合索引比创建单键索引更有效。MongoDB将使用复合索引来处理这两个查询。例如，可以在类别和项目上创建索引。
```
db.products.createIndex( { "category": 1, "item": 1 } )
```
This allows you both options. You can query on just `category`, and you also can query on `category` combined with `item`. A single [compound index](https://www.mongodb.com/docs/manual/core/index-compound/#std-label-index-type-compound) on multiple fields can support all the queries that search a "prefix" subset of those fields.

例如：
The following index on a collection:
```
{ x: 1, y: 1, z: 1 }
```
Can support queries that the following indexes support:
```
{ x: 1 }
{ x: 1, y: 1 }
```
在某些情况下，**前缀索引可能提供更好的查询性能**，例如如果z是一个大数组。

`{x: 1, y: 1, z: 1}` 索引也可以支持与以下索引相同的许多查询：
```
{x: 1, z: 1}
```
此外，`{x: 1, z: 1}` 还有一个额外的用途。给定以下查询：
```js
db.collection.find( { x: 5 } ).sort( { z: 1} )
```
The `{ x: 1, z: 1 }` index supports both the query and the sort operation, while the `{ x: 1, y: 1, z: 1 }` index only supports the query. For more information on sorting, see [Use Indexes to Sort Query Results.](https://www.mongodb.com/docs/manual/tutorial/sort-results-with-indexes/#std-label-sorting-with-indexes)

Starting in version 2.6, MongoDB can use [index intersection](https://www.mongodb.com/docs/manual/core/index-intersection/) to fulfill queries. The choice between creating compound indexes that support your queries or relying on index intersection depends on the specifics of your system. See [Index Intersection and Compound Indexes](https://www.mongodb.com/docs/manual/core/index-intersection/#std-label-index-intersection-compound-indexes) for more details. ？？

#### Create Indexes to Support Text Search[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/tutorial/create-indexes-to-support-queries/#create-indexes-to-support-text-search "Permalink to this heading")

For data hosted on MongoDB Atlas, you can support full-text search with Atlas Search indexes. To learn more, see [Create an Atlas Search Index.](https://www.mongodb.com/docs/atlas/atlas-search/create-index/)

For self-managed (non-Atlas) deployments, MongoDB provides a `text` index type that supports searching for string content in a collection. To learn more about self-managed text indexes, see [Text Indexes.](https://www.mongodb.com/docs/manual/core/index-text/#std-label-index-feature-text)

#### Index Use and Collation[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/tutorial/create-indexes-to-support-queries/#index-use-and-collation "Permalink to this heading")

To use an index for string comparisons, an operation must also specify the same collation. That is, an index with a collation cannot support an operation that performs string comparisons on the indexed fields if the operation specifies a different collation.

For example, the collection `myColl` has an index on a string field `category` with the collation locale `"fr"`.

```
db.myColl.createIndex( { category: 1 }, { collation: { locale: "fr" } } )
```

The following query operation, which specifies the same collation as the index, can use the index:

```
db.myColl.find( { category: "cafe" } ).collation( { locale: "fr" } )
```

However, the following query operation, which by default uses the "simple" binary collator, cannot use the index:

```
db.myColl.find( { category: "cafe" } )
```

For a compound index where the index prefix keys are not strings, arrays, and embedded documents, an operation that specifies a different collation can still use the index to support comparisons on the index prefix keys.

For example, the collection `myColl` has a compound index on the numeric fields `score` and `price` and the string field `category`; the index is created with the collation locale `"fr"` for string comparisons:

```
db.myColl.createIndex(   { score: 1, price: 1, category: 1 },   { collation: { locale: "fr" } } )
```

The following operations, which use `"simple"` binary collation for string comparisons, can use the index:

```
db.myColl.find( { score: 5 } ).sort( { price: 1 } )db.myColl.find( { score: 5, price: { $gt: NumberDecimal( "10" ) } } ).sort( { price: 1 } )
```

The following operation, which uses `"simple"` binary collation for string comparisons on the indexed `category` field, can use the index to fulfill only the `score: 5` portion of the query:

```
db.myColl.find( { score: 5, category: "cafe" } )
```
## 2. 创建高效索引的ESR规则
对于大多数复合索引，遵循 [ESR(Equality, Sort, Range) rule](https://www.mongodb.com/docs/manual/tutorial/equality-sort-range-rule/#std-label-esr-indexing-rule) 来 arrange the index keys 帮助create高效索引。
##### 1. Equality
Equality 指的是**对单个值的精确匹配** an exact match on a single value 。以下精确匹配查询 exact match queries 扫描cars集合，查找model字段完全匹配 `Cordoba` 的文档。
```
db.cars.find( { model: "Cordoba" } )  
db.cars.find( { model: { $eq: "Cordoba" } } )
```
索引搜索有效利用了**精确匹配**来限制需要检查以满足查询的文档数量。将需要精确匹配的字段放在索引中的前面 *Place fields that require exact matches first in your index* 。

对于具有精确匹配的查询，一个索引可能存在多个键。Equality匹配的索引键可以以任何顺序出现。但为了满足索引的Equality匹配，==所有精确匹配的索引键必须出现在任何其他索引字段之前==。MongoDB的搜索算法消除了「任何要按特定顺序来（在查询时？？）排列精确匹配字段 arrange the exact match fields in a particular order」的需求。

精确匹配应该是有选择性的。为了减少扫描的索引键的数量，请确保 equality test 消除了至少90%的可能文档匹配。
##### 2. Sort
“排序”决定结果的顺序。**排序跟随等式匹配**，因为等式匹配减少了需要排序的文档数量。在等式匹配之后进行排序 Sorting after the equality matches 还允许MongoDB执行非阻塞排序 non-blocking sort ？。

==当查询字段是索引键的子集时，索引可以支持排序操作==。只有在查询包括在排序键之前的所有前缀键的**等式条件**时 if the query includes equality conditions for all of the prefix keys that precede the sort keys ，**才支持对索引键子集进行排序操作**。有关更多信息，请参见：[Sort and Non-prefix Subset of an Index](https://www.mongodb.com/docs/manual/tutorial/sort-results-with-indexes/#std-label-sort-index-nonprefix-subset) 。或看下面：
> 比如索引有 a,b,c,d 四个字段，按照 c 排序，此时查询中的字段必须先是索引键的子集，还必须包含 a,b 这两个在 c 排序键之前的前缀键的 等式条件，比如 find ( {a: 1} ).sort( { c: 1}) ，或者 find ( { a: 1, b: 4}}).sort(c: 1) 。

**Sort and Non-prefix Subset of an Index**
一个索引能支持 sort operations on *a non-prefix subset of the index key pattern* ，为此，查询必须 include **equality conditions on all the prefix keys that precede the sort keys** 。例如，集合数据具有以下索引：
```
{ a: 1, b: 1, c: 1, d: 1 }
```
以下操作可以使用索引获取排序顺序：  

|示例 | 索引前缀  |
|:---|:---|
| db.data.find( { a: 5 } ).sort( { b: 1, c: 1 } )  | { a: 1 , b: 1, c: 1 }  
| db.data.find( { b: 3, a: 4 } ).sort( { c: 1 } )   | { a: 1, b: 1, c: 1 }  
| db.data.find( { a: 5, b: { $lt: 3} } ).sort( { b: 1 } )   | { a: 1, b: 1 }  

正如最后一个操作所示，仅 sort subset 之前的索引字段必须在查询中**有**等式条件；**其他索引字段可以指定其他条件**。

如果查询在 an index prefix that precedes or overlaps with the sort specification 上**未指定等式条件**，则操作将无法有效地使用索引。例如，以下操作指定了一个排序文档 `{ c: 1 }` ，但*查询文档没有包含前面的索引字段a和b上的等式匹配* ：
```
db.data.find( { a: { $gt: 2 } } ).sort( { c: 1 } )  
db.data.find( { c: 5 } ).sort( { c: 1 } )
```
这些操作将无法 **will not** 有效地使用索引{ a: 1, b: 1, c: 1, d: 1 }，甚至可能无法使用索引检索文档。
##### 3. Range
“范围”过滤器扫描字段。扫描不需要精确匹配，这意味着范围过滤器与索引键松散绑定 *range filters are loosely bound to index keys* 。为了提高查询效率，要使范围边界尽可能紧 *make the range bounds as tight as possible* ，并使用等式匹配来限制必须扫描的文档数量。

范围过滤器类似于以下内容：
```
db.cars.find( { price: { $gte: 15000} } )  
db.cars.find( { age: { $lt: 10 } } )  
db.cars.find( { priorAccidents: { $ne: null } } )
```
**MongoDB无法对范围过滤器的结果进行索引排序 index sort** 。将范围过滤器放在排序谓词之后，以便MongoDB可以使用非阻塞索引排序 *non-blocking index sort* 。有关阻塞排序的更多信息，请参见 mongosh method [cursor.allowDiskUse()] 。
#### 其他考虑因素  
不等式运算符，如 `$ne` 或 `$nin` ，是范围运算符，而不是等式运算符。`$regex` 是一个范围运算符。

**`$in` 可以是等式运算符或范围运算符**。**当 `$in` 单独使用时，它是一个等式运算符，执行一系列等式匹配**。当 `$in` 与 `.sort()` 一起使用时，它的作用类似于一个范围运算符。
#### 示例
下列查询，要搜索 `cars` 集合（Ford制造的 cost 超过 `$15000` 的 vehicles），输出按 `model` 排序：
```js
db.cars.find(
    {
        manufacturer: 'Ford',
        cost: { $gt: 15000 }
    }
).sort( { model: 1} )
```
这个查询包含 ESR rule的所有元素：
- `manufacturer: 'Ford'` 是一个 equality based match
- `cost: { $gt: 15000 }` 是一个 range based match
- `model` 用于排序

遵循 ESR rule ，对示例查询最优的索引是：
```js
{ manufacturer: 1, model: 1, cost: 1}
```
#### Further Discussion[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/tutorial/equality-sort-range-rule/#further-discussion "Permalink to this heading")
A number of MongoDB conference presentations discuss the ESR rule in depth.
- [Tips and Tricks for Effective Indexing](https://www.slideshare.net/mongodb/mongodb-local-toronto-2019-tips-and-tricks-for-effective-indexing)
- [The Sights (and Smells) of a Bad Query](https://www.slideshare.net/mongodb/mongodb-world-2019-the-sights-and-smells-of-a-bad-query)
- [Tips and Tricks++ for Querying and Indexing MongoDB](https://www.slideshare.net/mongodb/mongodb-world-2019-tips-and-tricks-for-querying-and-indexing-mongodb)

> For more information on tuning queries, see [`explain`](https://www.mongodb.com/docs/manual/reference/command/explain/#mongodb-dbcommand-dbcmd.explain) and [Query Plans.](https://www.mongodb.com/docs/manual/core/query-plans/#std-label-query-plans-query-optimization)
>To force MongoDB to **use**(必须是已存在的) a particular index, use [cursor.hint()](https://www.mongodb.com/docs/manual/reference/method/cursor.hint/#std-label-cursor-hint) mongosh when testing indexes.

## Use Indexes to Sort Query Results
==Since indexes contain ordered records, MongoDB can obtain the results of a sort from an index that includes the sort fields==. 由于索引包含有序记录，因此MongoDB可以从包括排序字段的索引中获取排序结果。 MongoDB _may_ use multiple indexes to support a sort operation _if_ the sort uses the same indexes as the query predicate. 如果排序使用与查询谓词相同的索引

If MongoDB cannot use an index or indexes to obtain the sort order, MongoDB must perform a blocking sort operation on the data. A blocking sort indicates that MongoDB must consume and process all input documents to the sort before returning results. Blocking sorts do not block concurrent operations on the collection or database. 如果MongoDB无法使用索引来获取排序顺序，则必须对数据执行阻塞排序操作。阻塞排序表示MongoDB必须在返回结果之前消耗并处理所有输入文档。阻塞排序不会阻塞集合或数据库上的并发操作。

Starting in MongoDB 6.0, if the server requires more than 100 megabytes of memory for a pipeline execution stage, MongoDB automatically writes temporary files to disk unless that query specifies `{ allowDiskUse: false }`. In versions 4.4 and 5.0, if the server needs more than 100 megabytes of system memory for the blocking sort operation, MongoDB returns an error unless that query specifies [`cursor.allowDiskUse()`](https://www.mongodb.com/docs/manual/reference/method/cursor.allowDiskUse/#mongodb-method-cursor.allowDiskUse). For details, see [`allowDiskUseByDefault`.](https://www.mongodb.com/docs/manual/reference/parameters/#mongodb-parameter-param.allowDiskUseByDefault)

==Sort operations that use an index often have better performance than blocking sorts.==

？？
As a result of changes to sorting behavior on array fields in MongoDB 4.4, when you sort on an array indexed with a [multikey index](https://www.mongodb.com/docs/manual/core/index-multikey/#std-label-index-type-multikey), the query plan includes a [blocking sort](https://www.mongodb.com/docs/manual/reference/glossary/#std-term-blocking-sort) stage, unless:
- The index [boundaries](https://www.mongodb.com/docs/manual/core/multikey-index-bounds/#std-label-multikey-index-bounds-intersecting) for all sort fields are `[MinKey, MaxKey]`, _and_    
- No boundaries for any multikey-indexed field have the same path prefix as the sort pattern.

### Sort with a Single Field Index  
If an ascending or a descending index is on a single field, the sort operation on the field can be in either direction. 单个字段上创建升序或降序索引，排序操作在这个字段上的可以是任一方向。

For example, create an ascending index on the field a for a collection records:
```js
db.records.createIndex( { a: 1 } )
```
This index can support an ascending sort on a:
```js
db.records.find().sort( { a: 1 } )
```
The index can also support the following descending sort on a by traversing the index in reverse order:
```js
db.records.find().sort( { a: -1 } )
```

### Sort on Multiple Fields[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/tutorial/sort-results-with-indexes/#sort-on-multiple-fields "Permalink to this heading")

Create a [compound index](https://www.mongodb.com/docs/manual/core/index-compound/#std-label-index-type-compound) to support sorting on multiple fields.

**You can specify a sort on all the keys of the index or on a subset** 可指定所有索引键上的一个排序，或索引键子集的排序; however, the sort keys must be listed in the _same order_ as they appear in the index **排序用到的键 必须 和它们出现在索引中的顺序 一致**. For example, an index key pattern `{ a: 1, b: 1 }` can support a sort on `{ a: 1, b: 1 }` but _not_ on `{ b: 1, a: 1 }`.

For a query to use a compound index for a sort 排序中使用复合索引的查询, the specified sort direction for all keys in the [`cursor.sort()`](https://www.mongodb.com/docs/manual/reference/method/cursor.sort/#mongodb-method-cursor.sort) document must match the index key pattern _or_ match the inverse of the index key pattern ==在 cursor.sort()中的所有键 指定的排序方向 必须匹配 索引键的模式 或 匹配索引键模式的反向== . For example, an index key pattern `{ a: 1, b: -1 }` can support a sort on `{ a: 1, b: -1 }` and `{ a: -1, b: 1 }` but **not** on `{ a: -1, b: -1 }` or `{a: 1, b: 1}`.

### 排序和索引前缀 sort and index prefix
==如果排序键对应于索引键或索引前缀，则 MongoDB 可以使用索引来对查询结果进行排序==。复合索引的前缀是索引键模式开头的一个或多个键组成的子集。

例如，在数据集合上创建一个复合索引：
```js
TXTCopydb.data.createIndex( { a:1, b: 1, c: 1, d: 1 } )
```
然后，以下是该索引的前缀：
```js
TXTCopy{ a: 1 }
{ a: 1, b: 1 }
{ a: 1, b: 1, c: 1 }
```
以下查询和排序操作**使用索引前缀对结果进行排序**。这些操作==不需要在内存中对结果集进行排序==。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307281824637.png)
考虑以下示例，其中索引的前缀键出现在查询谓词和排序中 the prefix keys of the index appear in both the query predicate and the sort ：

```
TXTCopydb.data.find( { a: { $gt: 4 } } ).sort( { a: 1, b: 1 } )
```

在这种情况下，MongoDB 可以使用索引按照**排序指定的顺序**检索文档。正如示例所示，==在查询谓词中的索引前缀可以与排序中的前缀不同==。

### Sort and Non-prefix Subset of an Index
An index can support sort operations on a non-prefix subset of the index key pattern. To do so, the query must include **equality** conditions on all the prefix keys that precede the sort keys. 索引可以支持 索引键模式的**非前缀子集**的排序操作。为此，查询必须包括所有==在排序键之前的前缀键==的**相等性**条件。

例如，集合`data`有以下索引：
```
{ a: 1, b: 1, c: 1, d: 1 }
```
The following operations can use the index to get the sort order:

|Example|Index Prefix|
|---|---|
|`db.data.find( { a: 5 } ).sort( { b: 1, c: 1 } )`|`{ a: 1 , b: 1, c: 1 }`|
|`db.data.find( { b: 3, a: 4 } ).sort( { c: 1 } )`|`{ a: 1, b: 1, c: 1 }`|
|`db.data.find( { a: 5, b: { $lt: 3} } ).sort( { b: 1 } )`|`{ a: 1, b: 1 }`|

As the last operation shows, **only** the index fields  (a) **_preceding_** the sort subset (b) must have the equality conditions in the query document; the other index fields may **specify other conditions**.

If the query does **not** specify an equality condition on ==an index prefix that precedes or overlaps with the sort specification==, the operation will **not** efficiently use the index. For example, the following operations specify a sort document of `{ c: 1 }`, but the query documents ==do not contain equality matches on the preceding index fields== `a` and `b`:
```
db.data.find( { a: { $gt: 2 } } ).sort( { c: 1 } )db.data.find( { c: 5 } ).sort( { c: 1 } )
```
These operations **will not** efficiently use the index `{ a: 1, b: 1, c: 1, d: 1 }` and **may not even use the index to retrieve the documents**.

### Index Sort Order
A collection of indexed documents may have multiple data types in the key field. 在索引字段上可能有多个数据类型。
- When an index has a key with multiple data types, the index is sorted according to the [BSON type sort order.](https://www.mongodb.com/docs/manual/reference/bson-type-comparison-order/#std-label-bson-types-comparison-order)
- In array comparisons:    
    - A less-than comparison, or an ascending sort, compares the smallest elements of the array according to the BSON type sort order.
    - A greater-than comparison, or a descending sort, compares the largest elements of the array according to the reverse BSON type sort order.
    - When comparing a field whose value is a one element array (example, `[ 1 ]`) with non-array fields (example, `2`), the comparison is for `1` and `2`.
    - A comparison of an empty array (example, `[ ]`) considers the empty array as less than a `null` value or a missing field value.
       
See the [index sorting example.](https://www.mongodb.com/docs/manual/tutorial/sort-results-with-indexes/#std-label-ex-sort-index-types)

