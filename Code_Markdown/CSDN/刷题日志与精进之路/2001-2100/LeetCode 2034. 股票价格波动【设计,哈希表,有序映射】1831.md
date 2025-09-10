> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一支股票价格的数据流。数据流中每一条记录包含一个 **时间戳** 和该时间点股票对应的 **价格** 。

不巧的是，由于股票市场内在的波动性，股票价格记录可能不是按时间顺序到来的。某些情况下，有的记录可能是错的。如果两个有相同时间戳的记录出现在数据流中，前一条记录视为错误记录，后出现的记录 **更正** 前一条错误的记录。

请你设计一个算法，实现：
- **更新** 股票在某一时间戳的股票价格，如果有之前同一时间戳的价格，这一操作将 **更正** 之前的错误价格。
- 找到当前记录里 **最新股票价格** 。**最新股票价格** 定义为时间戳最晚的股票价格。
- 找到当前记录里股票的 **最高价格** 。
- 找到当前记录里股票的 **最低价格** 。

请你实现 `StockPrice` 类：
- `StockPrice()` 初始化对象，当前无股票价格记录。
- `void update(int timestamp, int price)` 在时间点 `timestamp` 更新股票价格为 `price` 。
- `int current()` 返回股票 **最新价格** 。
- `int maximum()` 返回股票 **最高价格** 。
- `int minimum()` 返回股票 **最低价格** 。

**示例 1：**
```js
输入：
["StockPrice", "update", "update", "current", "maximum", "update", "maximum", "update", "minimum"]
[[], [1, 10], [2, 5], [], [], [1, 3], [], [4, 2], []]
输出：
[null, null, null, 5, 10, null, 5, null, 2]

解释：
StockPrice stockPrice = new StockPrice();
stockPrice.update(1, 10); // 时间戳为 [1] ，对应的股票价格为 [10] 。
stockPrice.update(2, 5);  // 时间戳为 [1,2] ，对应的股票价格为 [10,5] 。
stockPrice.current();     // 返回 5 ，最新时间戳为 2 ，对应价格为 5 。
stockPrice.maximum();     // 返回 10 ，最高价格的时间戳为 1 ，价格为 10 。
stockPrice.update(1, 3);  // 之前时间戳为 1 的价格错误，价格更新为 3 。
                          // 时间戳为 [1,2] ，对应股票价格为 [3,5] 。
stockPrice.maximum();     // 返回 5 ，更正后最高价格为 5 。
stockPrice.update(4, 2);  // 时间戳为 [1,2,4] ，对应价格为 [3,5,2] 。
stockPrice.minimum();     // 返回 2 ，最低价格时间戳为 4 ，价格为 2 。
```
**提示：**
- `1 <= timestamp, price <= 10^9`
- `update`，`current`，`maximum` 和 `minimum` **总** 调用次数不超过 `10^5` 。
- `current`，`maximum` 和 `minimum` 被调用时，`update` 操作 **至少** 已经被调用过 **一次** 。

---
### 解法 设计+哈希表+有序映射
容易想到要使用「哈希表」来记录 `{时间:价格}` 的映射关系。

关于 $current$ 操作，我们可以维护一个最大的时间戳 $cur$ ，在调用 $current$ 的时候直接 $O(1)$ 查得结果。

然后考虑解决 $update$ 操作中对相同时间点的更新问题，我们可以使用 `TreeMap`（红黑树）来解决该问题。以 `{价格:该价格对应的时间点数量}` 的 KV 形式进行存储，$key$ 按照「升序」进行排序。

对于更新操作，对传入的 $timestamp$ 是否已经被记录（是否已经存在哈希表中）进行分情况讨论：
- 传入的 $timestamp$ 未被记录，直接更新哈希表和 `TreeMap` ；
- 传入的 $timestamp$ 已被记录，此时需要先从哈希表取出旧价格 $old$ ，然后用旧价格对 `TreeMap` 进行修改（如果该价格只有一个时间点，将该价格直接从 `TreeMap` 中移除；**若有多个时间点，则对该价格对应的时间点数量进行减一操作**），然后再使用传入的新价格 $price$ 更新哈希表和 `TreeMap` 。

对于最大和最小价格的 $minimum$ 和 $maximum$ 操作，则只需要取得首尾的 $key$ 即可。
```java
class StockPrice {
    int maxTimestamp;
    HashMap<Integer, Integer> timePriceMap;
    TreeMap<Integer, Integer> prices;
    public StockPrice() {
        maxTimestamp = 0;
        timePriceMap = new HashMap<>();
        prices = new TreeMap<>();
    }
    
    public void update(int timestamp, int price) {
        maxTimestamp = Math.max(timestamp, maxTimestamp);
        if (timePriceMap.containsKey(timestamp)) { // 时间戳已存在
            int oldPrice = timePriceMap.get(timestamp); // 获取旧价格
            prices.put(oldPrice, prices.get(oldPrice) - 1); // 旧价格对应的时间戳数量-1
            if (prices.get(oldPrice) == 0) {
                prices.remove(oldPrice);
            }
        }
        timePriceMap.put(timestamp, price); // timestamp设置新价格
        prices.put(price, prices.getOrDefault(price, 0) + 1); // 新价格对应的时间戳数量+1
    }
    
    public int current() {
        return timePriceMap.get(maxTimestamp); // 获取股票最新价格
    }
    
    public int maximum() {
        return prices.lastKey();
    }
    
    public int minimum() {
        return prices.firstKey();
    }
}
```
复杂度分析：
- 时间复杂度：令 $n$ 为最大调用次数，$update$ 复杂度为 $O(\log{n})$ ；$current, maximum, minimum$ 操作复杂度为 $O(1)$
- 空间复杂度：$O(n)$