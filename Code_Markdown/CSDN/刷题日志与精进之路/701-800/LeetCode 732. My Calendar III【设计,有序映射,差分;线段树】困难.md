> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202309101949969.png)
当 `k` 个日程安排有一些时间上的交叉时（例如 `k` 个日程安排都在同一时间内），就会产生 `k` 次预订。

给你一些日程安排 `[start, end)` ，请你在每个日程安排添加后，返回一个整数 `k` ，表示所有先前日程安排会产生的最大 `k` 次预订。

实现一个 `MyCalendarThree` 类来存放你的日程安排，你可以一直添加新的日程安排。
- `MyCalendarThree()` 初始化对象。
- `int book(int start, int end)` 返回一个整数 `k` ，表示日历中存在的 `k` 次预订的最大值。

**示例：**
```java
输入：
["MyCalendarThree", "book", "book", "book", "book", "book", "book"]
[[], [10, 20], [50, 60], [10, 40], [5, 15], [5, 10], [25, 55]]
输出：
[null, 1, 1, 2, 3, 3, 3]
解释：
MyCalendarThree myCalendarThree = new MyCalendarThree();
myCalendarThree.book(10, 20); // 返回 1 ，第一个日程安排可以预订并且不存在相交，所以最大 k 次预订是 1 次预订。
myCalendarThree.book(50, 60); // 返回 1 ，第二个日程安排可以预订并且不存在相交，所以最大 k 次预订是 1 次预订。
myCalendarThree.book(10, 40); // 返回 2 ，第三个日程安排 [10, 40) 与第一个日程安排相交，所以最大 k 次预订是 2 次预订。
myCalendarThree.book(5, 15); // 返回 3 ，剩下的日程安排的最大 k 次预订是 3 次预订。
myCalendarThree.book(5, 10); // 返回 3
myCalendarThree.book(25, 55); // 返回 3
```
**提示：**
- `0 <= start < end <= 10^9`
- 每个测试用例，调用 `book` 函数最多不超过 `400`次

---
### 解法 有序映射+差分
可以参考「731. 我的日程安排表 II」的解法二，我们可以采用同样的思路即可。利用差分数组的思想，每当我们预定一个新的日程安排 $[start, end)$ ，在 $start$ 计数 $cnt$ 加 $1$ ，表示从 $start$ 预定的数目加 $1$ ；从 $\textit{end}$ 计数 $\textit{cnt}[\textit{end}]$ 减 $1$ ，表示从 $\textit{end}$ 开始预定的数目减 $1$ 。此时以起点 $x$ 开始的预定的数目 $\textit{book}_x = \sum_{y \le x}\textit{cnt}[y]​$ ，我们对计数进行累加依次求出最大的预定数目即可。由于本题中 $\textit{start}, \textit{end}$ 数量较大，我们利用 $\texttt{TreeMap}$ 计数即可。而且由于 `book()` 函数调用次数很少，所以 $O(n)$ 查询差分数组的前缀和的最大值也是可行的：
```cpp
class MyCalendarThree {
private:
    map<int, int> d;
public:
    MyCalendarThree() {}
    
    int book(int start, int end) {
        ++d[start];
        --d[end];
        int sum = 0, ans = 0;
        for (auto &it : d) {
            sum += it.second; 
            if (sum > ans) ans = sum;
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n^2)$ ，其中 $n$ 为日程安排的数量。每次求的最大的预定需要遍历所有的日程安排。
- 空间复杂度：$O(n)$ ，其中 $n$ 为日程安排的数量。需要空间存储所有的日程安排计数，需要的空间为 $O(n)$ 。

---
### 解法2 动态开点线段树+懒标记
利用线段树，假设我们开辟了数组 $\textit{arr}[0,\cdots, 10^9]$ ，初始时每个元素的值都为 $0$ ，对于每次行程预定的区间 $[\textit{start}, \textit{end})$ ，则我们将区间中的元素 $\textit{arr}[\textit{start},\cdots,\textit{end}-1]$ 中的每个元素加 $1$ ，最终只需要求出数组 $arr$ 的最大元素即可。实际我们不必实际开辟数组 $\textit{arr}$ ，可采用动态线段树，懒标记 $\textit{lazy}$ 标记区间 $[l,r]$ 进行累加的次数，$\textit{tree}$ 记录区间 $[l,r]$ 的最大值，最终返回区间 $[0,10^9]$ 中的最大元素即可。
```cpp
class MyCalendarThree {
public:
    unordered_map<int, pair<int, int>> tree;
    MyCalendarThree() { }
    void update(int s, int e, int l, int r, int idx) {
        if (r < s || e < l) return;
        if (s <= l && r <= e) {
            tree[idx].first++;
            tree[idx].second++; // 懒标记
        } else {
            int mid = (l + r) >> 1;
            update(s, e, l, mid, 2 * idx);
            update(s, e, mid + 1, r, 2 * idx + 1);
            tree[idx].first = tree[idx].second + max(tree[2 * idx].first, tree[2 * idx + 1].first);
        }
    }
    int book(int sTime, int eTime) {
        update(sTime, eTime - 1, 0, 1e9, 1);
        return tree[1].first;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n \log C)$ ，其中 $n$ 为日程安排的数量。由于使用了线段树查询，线段树的最大深度为 $\log C$ ，每次最多会查询 $\log C$ 个节点，每次求最大的预定需的时间复杂度为 $O(\log C + \log C)$ ，因此时间复杂度为 $O(n \log C)$ ，在此 $C$ 取固定值即为 $10^9$ 
- 空间复杂度：$O(n \log C)$ ，其中 $n$ 为日程安排的数量。由于该解法采用的为动态线段树，线段树的最大深度为 $\log C$ ，每次预定最多会在线段树上增加 $\log C$ 个节点，因此空间复杂度为 $O(n \log C)$ ，在此 $C$ 取固定值即为 $10^9$ 
 