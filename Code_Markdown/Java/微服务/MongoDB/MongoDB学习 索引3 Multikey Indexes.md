- [Create Multikey Index](https://www.mongodb.com/docs/manual/core/index-multikey/#create-multikey-index)
- [Index Bounds](https://www.mongodb.com/docs/manual/core/index-multikey/#index-bounds)
- [Unique Multikey Index](https://www.mongodb.com/docs/manual/core/index-multikey/#unique-multikey-index)
- [Limitations](https://www.mongodb.com/docs/manual/core/index-multikey/#limitations)
- [Examples](https://www.mongodb.com/docs/manual/core/index-multikey/#examples)

为了为**包含数组值的字段**创建索引，MongoDB会==为数组中的每个元素创建一个索引键==。这些多键索引 *multikey indexes* **支持针对数组字段的高效查询**。可以在包含标量值[1]（例如字符串、数字）和嵌套文档的数组上构建多键索引。
[1] 标量值是指既不是嵌入式文档也不是数组的值。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307281440457.png)
多键索引的示意图，索引位于“addr.zip”字段上。 “addr”字段包含一个地址文档数组。地址文档包含“zip”字段。  

### 创建多键索引
要创建多键索引，用 `db.collection.createIndex()` 方法：
```js
db.coll.createIndex( { <field>: < 1 or -1 > } )
```
**如果任何索引字段是数组，则MongoDB会自动创建多键索引**；不需要显式指定多键类型。
> 注意：仅适用于WiredTiger和In-Memory存储引擎，对于多键索引，MongoDB会跟踪哪些索引字段导致索引成为多键索引。跟踪此信息允许MongoDB查询引擎使用更紧密的**索引边界** index bound 。

### 索引边界
如果索引是多键索引，则计算索引边界遵循特殊规则。有关多键索引边界的详细信息，请参见[多键索引边界](https://www.mongodb.com/docs/manual/core/multikey-index-bounds/)。

### 唯一多键索引  
对于唯一索引，唯一约束适用于 applies across 集合中的单独文档，而不是单个文档内部。

因为唯一约束适用于单独的文档，所以对于[唯一多键索引](https://www.mongodb.com/docs/manual/core/index-unique/#std-label-unique-separate-documents)，文档可能具有导致重复索引键值的数组元素，只要该文档的索引键值不重复于另一个文档的索引键值即可。？？？

有关更多信息，请参见[跨单独文档的唯一约束](https://www.mongodb.com/docs/manual/core/index-unique/#std-label-unique-separate-documents)。
### 限制  
#### 复合多键索引  
对于复合多键索引，**每个索引文档最多可以有一个被索引字段其值为数组**。也就是说：如果文档的多个待索引字段是数组，则无法创建复合多键索引。例如，考虑包含以下文档的集合：
```
{ _id: 1, a: [ 1, 2 ], b: [ 1, 2 ], category: "AB - both arrays" }
```
无法在集合上创建复合多键索引{ a: 1, b: 1 }，因为a和b字段都是数组。或者，**如果已经存在复合多键索引，则不能插入违反此限制的文档**。

考虑包含以下文档的集合：
```
{ _id: 1, a: [1, 2], b: 1, category: "A array" }  
{ _id: 2, a: 1, b: [1, 2], category: "B array" }
```
可以使用复合多键索引{ a: 1, b: 1 }，因为==对于每个文档，仅有一个由复合多键索引索引的字段是数组== *one field indexed by the compound multikey index* ；即没有文档包含a和b字段的数组值。但，在创建复合多键索引之后，如果尝试插入同时具有a和b字段的数组的文档，则MongoDB将无法插入。

如果字段是文档数组，则可以索引嵌入字段以创建复合索引。例如，考虑包含以下文档的集合：
```js
{ _id: 1, a: [ { x: 5, z: [ 1, 2 ] }, { z: [ 1, 2 ] } ] }  
{ _id: 2, a: [ { x: 5 }, { z: 4 } ] }
```
可以在{“a.x”: 1，“a.z”: 1}上创建复合索引。最多一个索引字段为数组的限制也适用。有关示例，请参见使用[Index Arrays with Embedded Documents](https://www.mongodb.com/docs/manual/core/index-multikey/#std-label-multikey-embedded-documents)。

### See also:
- [Unique Constraint Across Separate Documents](https://www.mongodb.com/docs/manual/core/index-unique/#std-label-unique-separate-documents)
- [Unique Index on a Single Field](https://www.mongodb.com/docs/manual/core/index-unique/#std-label-index-unique-index)

### Sorting
由于MongoDB 4.4中对数组字段排序行为的更改，当您对使用[多键索引](https://www.mongodb.com/docs/manual/core/index-multikey/#std-label-index-type-multikey)索引的数组进行排序时，查询计划会包括一个[阻塞排序](https://www.mongodb.com/docs/manual/reference/glossary/#std-term-blocking-sort)阶段，除非：
- 所有排序字段的索引边界均为[MinKey，MaxKey]，且
- 任何多键索引字段的边界都没有与排序模式具有相同的路径前缀。- No boundaries for any multikey-indexed field have the same path prefix as the sort pattern.

> 分片键：不能将多键索引指定为分片键索引。
> 但是，如果分片键索引是复合索引的前缀，如果其中另一个键（即不属于分片键的键）索引了一个数组，则复合索引被允许变为复合多键索引，，则可以对性能产生影响。

> 哈希索引：哈希索引不能是多键的。

> 覆盖查询：多键索引不能覆盖数组字段的查询。如果索引跟踪导致索引成为多键的哪个字段或多个字段，则多键索引可以覆盖对非数组字段的查询。

### 作为整体查询数组字段  
当查询过滤器指定数组作为整体的精确匹配时，MongoDB可以使用多键索引查找查询数组的第一个元素，但无法使用多键索引扫描来查找整个数组。相反，在使用多键索引查找查询数组的第一个元素后，MongoDB检索关联的文档并过滤其数组与查询中的数组匹配的文档。

例如，考虑包含以下文档的 `inventory` 集合：
```js
{ _id: 5, type: "food", item: "aaa", ratings: [ 5, 8, 9 ] }
{ _id: 6, type: "food", item: "bbb", ratings: [ 5, 9 ] }
{ _id: 7, type: "food", item: "ccc", ratings: [ 9, 5, 8 ] }
{ _id: 8, type: "food", item: "ddd", ratings: [ 9, 5 ] }
{ _id: 9, type: "food", item: "eee", ratings: [ 5, 9, 5 ] }
```
这个集合有一个 `ratings` 字段的多键索引。
```
db.inventory.createIndex( { ratings: 1 } )
```
下列查询寻找文档，其 `ratings` 字段是数组 `[5, 9]` ：
```js
db.inventory.find( { ratings: [ 5, 9 ] } )
```
MongoDB可以使用多键索引来查找处于 `ratings` 数组中任何位置的5的文档。然后，MongoDB检索这些文档，并过滤 `ratings` 数组等于查询数组[5,9]的文档。

### 示例
```js
db.survey.insertOne(
    { _id: 1, item: "ABC", ratings: [2, 5, 9] }
)
```
创建一个 `ratings` 的索引：
```js
db.survey.createIndex( { ratings: 1 })
```
因为 `ratings` 字段包含一个数组，所以在 `ratings` 上的索引是 multikey ，这个多键索引包含以下三个 索引键 ，每个都指向同样的文档 *each pointing to the same document* ：
```js
2, 5, and 9
```

### Index Arrays with Embedded Documents[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/core/index-multikey/#index-arrays-with-embedded-documents "Permalink to this heading")

You can create multikey indexes on array fields that contain nested objects.

Consider an `inventory` collection with documents of the following form:

```
{
  _id: 1,
  item: "abc",
  stock: [
    { size: "S", color: "red", quantity: 25 },
    { size: "S", color: "blue", quantity: 10 },
    { size: "M", color: "blue", quantity: 50 }
  ]
}
{
  _id: 2,
  item: "def",
  stock: [
    { size: "S", color: "blue", quantity: 20 },
    { size: "M", color: "blue", quantity: 5 },
    { size: "M", color: "black", quantity: 10 },
    { size: "L", color: "red", quantity: 2 }
  ]
}
{
  _id: 3,
  item: "ijk",
  stock: [
    { size: "M", color: "blue", quantity: 15 },
    { size: "L", color: "blue", quantity: 100 },
    { size: "L", color: "red", quantity: 25 }
  ]
}

...
```

The following operation creates a multikey index on the `stock.size` and `stock.quantity` fields:

```
db.inventory.createIndex( { "stock.size": 1, "stock.quantity": 1 } )
```

The compound multikey index can support queries with predicates that include both indexed fields as well as predicates that include only the index prefix `"stock.size"`, as in the following examples:

```
db.inventory.find( { "stock.size": "M" } )
db.inventory.find( { "stock.size": "S", "stock.quantity": { $gt: 20 } } )
```

For details on how MongoDB can combine multikey index bounds, see [Multikey Index Bounds](https://www.mongodb.com/docs/manual/core/multikey-index-bounds/). For more information on behavior of compound indexes and prefixes, see [compound indexes and prefixes.](https://www.mongodb.com/docs/manual/core/index-compound/#std-label-compound-index-prefix)

The compound multikey index can also support sort operations, such as the following examples:

```
db.inventory.find( ).sort( { "stock.size": 1, "stock.quantity": 1 } )
db.inventory.find( { "stock.size": "M" } ).sort( { "stock.quantity": 1 } )
```

For more information on behavior of compound indexes and sort operations, see [Use Indexes to Sort Query Results.](https://www.mongodb.com/docs/manual/tutorial/sort-results-with-indexes/)

### Multikey Index Bound
==The bounds of an index scan define the portions of an index to search during a query==. 索引扫描的边界定义了查询期间要搜索的索引部分 When multiple predicates over an index exist, MongoDB will attempt to combine the bounds for these predicates, by either intersection or compounding, in order to produce a scan with smaller bounds. 以产生较小边界的扫描
#### MultiKey Index
Bound Intersection: a logical conjunction (i.e. AND) of multiple bounds . 例如，两个 bounds `[ [ 3, Infinity ] ]` and `[ [ -Infinity, 6 ] ]` 的交集为 `[ [3, 6] ]` 。

给出一个 indexed array 字段，考虑一个在数组上指定多个谓词并且可以使用多键索引的查询。如果 `$elemMatch` 连接谓词，MongoDB可以交集多键索引边界。

例如，创建包含具有字段 `item` 和数组字段 `ratings` 的文档的 `survey` 集合：
```js
db.survey.insertMany(
    [
        { _id: 1, item: "ABC", ratings: [2, 9] },
        { _id: 2, item: "XYZ", ratings: [4, 3] }
    ]
)
```
在 `ratings` 上创建一个多键索引：
```js
db.survey.createIndex( { ratings: 1} )
```
使用 `$elemMatch` 获取：数组包含至少 one single element ，匹配以下两个条件：
```js
db.survey.find( { ratings : { $elemMatch: { $gte: 3, $lte: 6 } } } )
```
分开看谓词：
- >= 3 predicate 的 bounds 是 `[ [3, Infinity] ]`
- `$lte: 6` predicate 的 bounds 是 `[ [-Infinity, 6] ]` 

因为查询使用了 `$elemMatch` 来 join 这些谓词，MongoDB能 intersect the bounds 为： `ratings: [ [ 3, 6 ] ]` 。

如果查询没有使用 `$elemMatch` 连接数组字段的条件，MongoDB无法交集多键索引边界。考虑以下查询：
```
db.survey.find( { ratings : { $gte: 3, $lte: 6 } } )
```
该查询搜索 `ratings` 数组，寻找至少一个元素大于或等于3，以及至少一个元素小于或等于6。由于单个元素不需要同时满足两个条件，MongoDB不会交集边界，而是使用 [ [ 3, Infinity ] ] 或 [ [ -Infinity, 6 ] ] 中的一个。MongoDB不保证选择哪个边界。


---
## Compound Bounds for Multikey Index[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/core/multikey-index-bounds/#compound-bounds-for-multikey-index "Permalink to this heading")

Compounding bounds refers to using bounds for multiple keys of [compound index](https://www.mongodb.com/docs/manual/core/index-compound/#std-label-index-type-compound). For instance, given a compound index `{ a: 1, b: 1 }` with bounds on field `a` of `[ [ 3, Infinity ] ]` and bounds on field `b` of `[ [ -Infinity, 6 ] ]`, compounding the bounds results in the use of both bounds:

```
{ a: [ [ 3, Infinity ] ], b: [ [ -Infinity, 6 ] ] }
```

If MongoDB cannot compound the two bounds, MongoDB always constrains the index scan by the bound on its leading field, in this case, `a: [ [ 3, Infinity ] ]`.

### Compound Index on an Array Field[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/core/multikey-index-bounds/#compound-index-on-an-array-field "Permalink to this heading")

Consider a compound multikey index; i.e. a [compound index](https://www.mongodb.com/docs/manual/core/index-compound/#std-label-index-type-compound) where one of the indexed fields is an array. For example, create a `survey` collection that contains documents with a field `item` and an array field `ratings`:

```
db.survey.insertMany(
   [
      { _id: 1, item: "ABC", ratings: [ 2, 9 ] },
      { _id: 2, item: "XYZ", ratings: [ 4, 3 ] }
   ]
)
```

Create a [compound index](https://www.mongodb.com/docs/manual/core/index-compound/#std-label-index-type-compound) on the `item` field and the `ratings` field:

```
db.survey.createIndex( { item: 1, ratings: 1 } )
```

The following query specifies a condition on both keys of the index:

```
db.survey.find( { item: "XYZ", ratings: { $gte: 3 } } )
```
Taking the predicates separately:
- the bounds for the `item: "XYZ"` predicate are `[ [ "XYZ", "XYZ" ] ]`;    
- the bounds for the `ratings: { $gte: 3 }` predicate are `[ [ 3, Infinity ] ]`.

MongoDB can compound the two bounds to use the combined bounds of:
```
{ item: [ [ "XYZ", "XYZ" ] ], ratings: [ [ 3, Infinity ] ] }
```

#### Range Queries on a Scalar Indexed Field (WiredTiger)[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/core/multikey-index-bounds/#range-queries-on-a-scalar-indexed-field--wiredtiger- "Permalink to this heading")

_For the WiredTiger and In-Memory storage engines only_,

In multikey index, MongoDB keeps track of which indexed field or fields cause an index to be a multikey index. Tracking this information allows the MongoDB query engine to use tighter index bounds.

The aforementioned [compound index](https://www.mongodb.com/docs/manual/core/index-compound/#std-label-index-type-compound) is on the scalar field [[1]](https://www.mongodb.com/docs/manual/core/multikey-index-bounds/#footnote-scalar) `item` and the array field `ratings`:

```
db.survey.createIndex( { item: 1, ratings: 1 } )
```

For the WiredTiger and the In-Memory storage engines, if a query operation specifies multiple predicates on the indexed scalar field(s) of a compound multikey index, MongoDB will intersect the bounds for the field.

For example, the following operation specifies a range query on the scalar field as well as a range query on the array field:

```
db.survey.find( {   item: { $gte: "L", $lte: "Z"}, ratings : { $elemMatch: { $gte: 3, $lte: 6 } }} )
```

MongoDB will intersect the bounds for `item` to `[ [ "L", "Z" ] ]` and ratings to `[[3.0, 6.0]]` to use the combined bounds of:

```
"item" : [ [ "L", "Z" ] ], "ratings" : [ [3.0, 6.0] ]
```

For another example, consider where the scalar fields belong to a nested document. For instance, create a `survey` collection that contains the following documents:

```
db.survey.insertMany(   [      { _id: 1, item: { name: "ABC", manufactured: 2016 }, ratings: [ 2, 9 ] },      { _id: 2, item: { name: "XYZ", manufactured: 2013 },  ratings: [ 4, 3 ] }   ])
```

Create a compound multikey index on the scalar fields `"item.name"`, `"item.manufactured"`, and the array field `ratings` :

```
db.survey.createIndex( { "item.name": 1, "item.manufactured": 1, ratings: 1 } )
```

Consider the following operation that specifies query predicates on the scalar fields:

```
db.survey.find( {   "item.name": "L" ,   "item.manufactured": 2012} )
```

For this query, MongoDB can use the combined bounds of:

```
"item.name" : [ ["L", "L"] ], "item.manufactured" : [ [2012.0, 2012.0] ]
```

Earlier versions of MongoDB cannot combine these bounds for the scalar fields.

|   |   |
|---|---|
|[[1](https://www.mongodb.com/docs/manual/core/multikey-index-bounds/#ref-scalar-id1)]|A scalar field is a field whose value is neither a document nor an array; e.g. a field whose value is a string or an integer is a scalar field.A scalar field can be a field nested in a document, as long as the field itself is not an array or a document. For example, in the document `{ a: { b: { c: 5, d: 5 } } }`, `c` and `d` are scalar fields where as `a` and `b` are not.|

### Compound Index on Fields from an Array of Embedded Documents[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/core/multikey-index-bounds/#compound-index-on-fields-from-an-array-of-embedded-documents "Permalink to this heading")

If an array contains embedded documents, to index on fields contained in the embedded documents, use the [dotted field name](https://www.mongodb.com/docs/manual/core/document/#std-label-document-dot-notation) in the index specification. For instance, given the following array of embedded documents:

```
ratings: [ { score: 2, by: "mn" }, { score: 9, by: "anon" } ]
```

The dotted field name for the `score` field is `"ratings.score"`.

#### Compound Bounds of Non-array Field and Field from an Array[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/core/multikey-index-bounds/#compound-bounds-of-non-array-field-and-field-from-an-array "Permalink to this heading")

Consider a collection `survey2` contains documents with a field `item` and an array field `ratings`:

```
{  _id: 1,  item: "ABC",  ratings: [ { score: 2, by: "mn" }, { score: 9, by: "anon" } ]}{  _id: 2,  item: "XYZ",  ratings: [ { score: 5, by: "anon" }, { score: 7, by: "wv" } ]}
```

Create a [compound index](https://www.mongodb.com/docs/manual/core/index-compound/#std-label-index-type-compound) on the non-array field `item` as well as two fields from an array `ratings.score` and `ratings.by`:

```
db.survey2.createIndex( { "item": 1, "ratings.score": 1, "ratings.by": 1 } )
```

The following query specifies a condition on all three fields:

```
db.survey2.find( { item: "XYZ",  "ratings.score": { $lte: 5 }, "ratings.by": "anon" } )
```

Taking the predicates separately:

- the bounds for the `item: "XYZ"` predicate are `[ [ "XYZ", "XYZ" ] ]`;
    
- the bounds for the `score: { $lte: 5 }` predicate are `[ [ -Infinity, 5 ] ]`;
    
- the bounds for the `by: "anon"` predicate are `[ "anon", "anon" ]`.
    

MongoDB can compound the bounds for the `item` key with _either_ the bounds for `"ratings.score"` or the bounds for `"ratings.by"`, depending upon the query predicates and the index key values. MongoDB makes no guarantee as to which bounds it compounds with the `item` field. For instance, MongoDB will either choose to compound the `item` bounds with the `"ratings.score"` bounds:

```
{  "item" : [ [ "XYZ", "XYZ" ] ],  "ratings.score" : [ [ -Infinity, 5 ] ],  "ratings.by" : [ [ MinKey, MaxKey ] ]}
```

Or, MongoDB may choose to compound the `item` bounds with `"ratings.by"` bounds:

```
{  "item" : [ [ "XYZ", "XYZ" ] ],  "ratings.score" : [ [ MinKey, MaxKey ] ],  "ratings.by" : [ [ "anon", "anon" ] ]}
```

However, to compound the bounds for `"ratings.score"` with the bounds for `"ratings.by"`, the query must use [`$elemMatch`](https://www.mongodb.com/docs/manual/reference/operator/query/elemMatch/#mongodb-query-op.-elemMatch). See [Compound Bounds of Index Fields from an Array](https://www.mongodb.com/docs/manual/core/multikey-index-bounds/#std-label-compound-fields-from-array) for more information.

#### Compound Bounds of Index Fields from an Array[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/core/multikey-index-bounds/#compound-bounds-of-index-fields-from-an-array "Permalink to this heading")

To compound together the bounds for index keys from the same array:

- the index keys must share the same field path up to but excluding the field names, and
    
- the query must specify predicates on the fields using [`$elemMatch`](https://www.mongodb.com/docs/manual/reference/operator/query/elemMatch/#mongodb-query-op.-elemMatch) on that path.
    

For a field in an embedded document, the [dotted field name](https://www.mongodb.com/docs/manual/core/document/#std-label-document-dot-notation), such as `"a.b.c.d"`, is the field path for `d`. To compound the bounds for index keys from the same array, the [`$elemMatch`](https://www.mongodb.com/docs/manual/reference/operator/query/elemMatch/#mongodb-query-op.-elemMatch) must be on the path up to _but excluding_ the field name itself; i.e. `"a.b.c"`.

For instance, create a [compound index](https://www.mongodb.com/docs/manual/core/index-compound/#std-label-index-type-compound) on the `ratings.score` and the `ratings.by` fields:

```
db.survey2.createIndex( { "ratings.score": 1, "ratings.by": 1 } )
```

The fields `"ratings.score"` and `"ratings.by"` share the field path `ratings`. The following query uses [`$elemMatch`](https://www.mongodb.com/docs/manual/reference/operator/query/elemMatch/#mongodb-query-op.-elemMatch) on the field `ratings` to require that the array contains at least one _single_ element that matches both conditions:

```
db.survey2.find( { ratings: { $elemMatch: { score: { $lte: 5 }, by: "anon" } } } )
```

Taking the predicates separately:

- the bounds for the `score: { $lte: 5 }` predicate are `[ [ -Infinity, 5 ] ]`;
    
- the bounds for the `by: "anon"` predicate are `[ [ "anon", "anon" ] ]`.
    

MongoDB can compound the two bounds to use the combined bounds of:

```
{ "ratings.score" : [ [ -Infinity, 5 ] ], "ratings.by" : [ [ "anon", "anon" ] ] }
```

#### Query Without `$elemMatch`[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/core/multikey-index-bounds/#query-without--elemmatch "Permalink to this heading")

If the query does _not_ join the conditions on the indexed array fields with [`$elemMatch`](https://www.mongodb.com/docs/manual/reference/operator/query/elemMatch/#mongodb-query-op.-elemMatch), MongoDB _cannot_ compound their bounds. Consider the following query:

```
db.survey2.find( { "ratings.score": { $lte: 5 }, "ratings.by": "anon" } )
```

Because a single embedded document in the array does not need to meet both criteria, MongoDB does _not_ compound the bounds. When using a compound index, if MongoDB cannot constrain all the fields of the index, MongoDB always constrains the leading field of the index, in this case `"ratings.score"`:

```
{  "ratings.score": [ [ -Infinity, 5 ] ],  "ratings.by": [ [ MinKey, MaxKey ] ]}
```

#### `$elemMatch` on Incomplete Path[![](https://www.mongodb.com/docs/manual/assets/link.svg)](https://www.mongodb.com/docs/manual/core/multikey-index-bounds/#-elemmatch-on-incomplete-path "Permalink to this heading")

If the query does not specify [`$elemMatch`](https://www.mongodb.com/docs/manual/reference/operator/query/elemMatch/#mongodb-query-op.-elemMatch) on the path of the embedded fields, up to but excluding the field names, MongoDB **cannot** compound the bounds of index keys from the same array.

For example, a collection `survey3` contains documents with a field `item` and an array field `ratings`:

```
{  _id: 1,  item: "ABC",  ratings: [ { scores: [ { q1: 2, q2: 4 }, { q1: 3, q2: 8 } ], loc: "A" },             { scores: [ { q1: 2, q2: 5 } ], loc: "B" } ]}{  _id: 2,  item: "XYZ",  ratings: [ { scores: [ { q1: 7 }, { q1: 2, q2: 8 } ], loc: "B" } ]}
```

Create a [compound index](https://www.mongodb.com/docs/manual/core/index-compound/#std-label-index-type-compound) on the `ratings.scores.q1` and the `ratings.scores.q2` fields:

```
db.survey3.createIndex( { "ratings.scores.q1": 1, "ratings.scores.q2": 1 } )
```

The fields `"ratings.scores.q1"` and `"ratings.scores.q2"` share the field path `"ratings.scores"` and the [`$elemMatch`](https://www.mongodb.com/docs/manual/reference/operator/query/elemMatch/#mongodb-query-op.-elemMatch) must be on that path.

The following query, however, uses an [`$elemMatch`](https://www.mongodb.com/docs/manual/reference/operator/query/elemMatch/#mongodb-query-op.-elemMatch) but not on the required path:

```
db.survey3.find( { ratings: { $elemMatch: { 'scores.q1': 2, 'scores.q2': 8 } } } )
```

As such, MongoDB **cannot** compound the bounds, and the `"ratings.scores.q2"` field will be unconstrained during the index scan. To compound the bounds, the query must use [`$elemMatch`](https://www.mongodb.com/docs/manual/reference/operator/query/elemMatch/#mongodb-query-op.-elemMatch) on the path `"ratings.scores"`:

```
db.survey3.find( { 'ratings.scores': { $elemMatch: { 'q1': 2, 'q2': 8 } } } )
```

←  [Multikey Indexes](https://www.mongodb.com/docs/manual/core/index-multikey/ "Previous Section")[Text Indexes](https://www.mongodb.com/docs/manual/core/index-text/ "Next Section") →