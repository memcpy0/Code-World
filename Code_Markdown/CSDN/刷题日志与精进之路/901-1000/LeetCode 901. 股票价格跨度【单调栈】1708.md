> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

设计一个算法收集某些股票的每日报价，并返回该股票当日价格的 **跨度** 。

当日股票价格的 **跨度** 被定义为股票价格小于或等于今天价格的最大连续日数（从今天开始往回数，包括今天）。
- 例如，如果未来 7 天股票的价格是 `[100,80,60,70,60,75,85]`，那么股票跨度将是 `[1,1,1,2,1,4,6]` 。
    

实现 `StockSpanner` 类：
- `StockSpanner()` 初始化类对象。
- `int next(int price)` 给出今天的股价 `price` ，返回该股票当日价格的 **跨度** 。

**示例：**
```js
输入：
["StockSpanner", "next", "next", "next", "next", "next", "next", "next"]
[[], [100], [80], [60], [70], [60], [75], [85]]
输出：
[null, 1, 1, 1, 2, 1, 4, 6]

解释：
StockSpanner stockSpanner = new StockSpanner();
stockSpanner.next(100); // 返回 1
stockSpanner.next(80);  // 返回 1
stockSpanner.next(60);  // 返回 1
stockSpanner.next(70);  // 返回 2
stockSpanner.next(60);  // 返回 1
stockSpanner.next(75);  // 返回 4 ，因为截至今天的最后 4 个股价 (包括今天的股价 75) 都小于或等于今天的股价。
stockSpanner.next(85);  // 返回 6
```
**提示：**
- `1 <= price <= 10^5`
- 最多调用 `next` 方法 `10^4` 次

---
### 解法 单调栈
```cpp
class StockSpanner {
private:
    // 单调栈(从栈顶向下单调递减)
    // index动态更新传入的价格下标，维护栈顶为最小的单调递减栈，栈存储<下标,价格>对，
    // 当栈为空(可在栈底预先加入哨兵节点<-1,MAX>避免判断空栈)或栈顶价格大于当前price时将price加入，此时向前的连续递减天数为1(price本身)，
    // 否则弹出价格小于等于当前price的栈顶，剩余栈顶价格就是之前大于当前price的最近元素，两下标相减即price前连续较小天数
    // 若栈内只剩哨兵节点，因为栈是单调递减，说明price之前元素都不大于本身，天数为当期那price下标index+1
    stack<pair<int, int>> st;
    int index = 0;
public:
    StockSpanner() { 
        // 预先在栈底埋入哨兵节点<-1,MAX>，后续不必再特判栈为空的情况
        st.push({INT_MAX, -1});
    }
    
    int next(int price) {
        while (price >= st.top().first) {
            st.pop();
        }
        int ans = index - st.top().second;
        st.push({price, index++});
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(n)$