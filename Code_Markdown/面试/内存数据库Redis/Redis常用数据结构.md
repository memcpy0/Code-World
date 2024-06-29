### Redis 有哪几种数据结构？
有五种常用数据类型：String、Hash、Set、List、SortedSet。以及三种特殊的数据类型：Bitmap、HyperLogLog、Geospatial ，其中 **HyperLogLog、Bitmap的底层都是 String 数据类型**，Geospatial 的底层是 Sorted Set 数据类型。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304201540398.png)
