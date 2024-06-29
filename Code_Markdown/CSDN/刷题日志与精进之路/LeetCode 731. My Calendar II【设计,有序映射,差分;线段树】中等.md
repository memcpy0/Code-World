> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202309102022561.png)
实现一个 `MyCalendar` 类来存放你的日程安排。如果要添加的时间内不会导致三重预订时，则可以存储这个新的日程安排。

`MyCalendar` 有一个 `book(int start, int end)`方法。它意味着在 `start` 到 `end` 时间内增加一个日程安排，注意，这里的时间是半开区间，即 `[start, end)`, 实数 `x` 的范围为，  `start <= x < end`。

当三个日程安排有一些时间上的交叉时（例如三个日程安排都在同一时间内），就会产生三重预订。

每次调用 `MyCalendar.book`方法时，如果可以将日程安排成功添加到日历中而不会导致三重预订，返回 `true`。否则，返回 `false` 并且不要将该日程安排添加到日历中。

请按照以下步骤调用`MyCalendar` 类: `MyCalendar cal = new MyCalendar();` `MyCalendar.book(start, end)`

**示例：**
```java
MyCalendar();
MyCalendar.book(10, 20); // returns true
MyCalendar.book(50, 60); // returns true
MyCalendar.book(10, 40); // returns true
MyCalendar.book(5, 15); // returns false
MyCalendar.book(5, 10); // returns true
MyCalendar.book(25, 55); // returns true
解释：
前两个日程安排可以添加至日历中。 第三个日程安排会导致双重预订，但可以添加至日历中。
第四个日程安排活动（5,15）不能添加至日历中，因为它会导致三重预订。
第五个日程安排（5,10）可以添加至日历中，因为它未使用已经双重预订的时间10。
第六个日程安排（25,55）可以添加至日历中，因为时间 [25,40] 将和第三个日程安排双重预订；
时间 [40,50] 将单独预订，时间 [50,55）将和第二个日程安排双重预订。
```
**提示：**
- 每个测试用例，调用 `MyCalendar.book` 函数最多不超过 `1000`次。
- 调用函数 `MyCalendar.book(start, end)`时， `start` 和 `end` 的取值范围为 `[0, 10^9]`。

---
### 解法1 直接遍历
记录下所有**已经预定的课程安排区间**与**已经预定过两次的课程安排区间**，当我们预定新的区间 $[start, end)$ 时，此时检查当前已经预定过两次的每个日程安排是否与新日程安排冲突。若不冲突，则可以添加新的日程安排。
- 对于两个区间 $[s_1, e_1)$ 和 $[s_2, e_2)$ ，如果二者没有交集，则此时应当满足 $s_1 \ge e_2$ 或者 $s_2 \ge e_1$ ，这就意味着如果满足 $s_1 < e_2$ 并且 $s_2 < e_1$ 。
- 首先检测新加入的区间 $[start, end)$是否与已经预定过两次的区间有交集，如果没有冲突，则将新加入的区间与已经预定的区间进行检查，求出新增的预定两次的区间。对于两个区间 $[s_1, e_1)$ 和 $[s_2,e_2)$ ，则他们之间的交集为 $[\max(s_1,s_2), \min(e_1,e_2))$
​
```cpp
class MyCalendarTwo {
public:
    MyCalendarTwo() {}
    bool book(int start, int end) {
        for (auto &[l, r] : overlaps)
            if (l < end && start < r) return false;
        for (auto &[l, r] : booked)
            if (l < end && start < r)
                overlaps.emplace_back(max(l, start), min(r, end));
        booked.emplace_back(start, end);
        return true;
    }
private:
    vector<pair<int, int>> booked;
    vector<pair<int, int>> overlaps;
};
```
复杂度分析：
- 时间复杂度：$O(n^2)$ ，其中 $n$ 表示日程安排的数量。由于每次在进行预订时，都需要遍历所有已经预订的行程安排。
- 空间复杂度：$O(n)$ ，其中 $n$ 表示日程安排的数量。需要保存所有已经预订的行程。

---
### 解法2 差分数组
利用差分数组的思想，每当我们预定一个新的日程安排 $[start, end)$，在 $start$ 计数 $cnt[start]$ 加 $1$ ，表示从 $start$ 预定的数目加 $1$ ；从 $end$ 计数 $cnt[end]$ 减 $1$ ，表示从 $end$ 开始预定的数目减 $1$ 。此时以起点 $x$ 开始的预定的数目 $\textit{book}_x = \sum_{y \le x}\textit{cnt}[y]$ ，当我们将 $[start, end)$ 加入后，如果发现存在区间的预定数目大于 $2$ 时，此时为非法，应去除新加入的区间 $[start, end)$ 。由于本题中 $\textit{start}, \textit{end}$ 数量较大，我们利用 $\texttt{TreeMap}$ 计数即可。
```cpp
class MyCalendarTwo {
public:
    MyCalendarTwo() {

    }
    
    bool book(int start, int end) {
        int maxBook = 0;
        ++cnt[start];
        --cnt[end];
        for (auto &[_, freq] : cnt) {
            maxBook += freq;
            if (maxBook > 2) {
                --cnt[start];
                ++cnt[end];
                return false;
            }
        }
        return true;
    }
private:
    map<int, int> cnt;
};
```
复杂度分析：
- 时间复杂度：$O(n^2)$ ，其中 $n$ 为日程安排的数量。每次求的最大的预定需要遍历所有的日程安排。
- 空间复杂度：$O(n)$ ，其中 $n$ 为日程安排的数量。需要空间存储所有的日程安排计数，需要的空间为 $O(n)$ 。

---
### 解法3 线段树
利用线段树，假设我们开辟了数组 $\textit{arr}[0,\cdots, 10^9]$ ，初始时每个元素的值都为 $0$ ，对于每次行程预定的区间 $[start, end)$ ，则我们将区间中的元素 $\textit{arr}[\textit{start},\cdots,\textit{end}-1]$ 中的每个元素加 $1$ ，如果数组 $arr$ 的最大元素大于 $2$ 时，此时则出现某个区间被安排了 $2$ 次上，此时返回 $\texttt{false}$ ，同时将数组区间 $\textit{arr}[\textit{start},\cdots,\textit{end}-1]$ 进行减 $1$ 即可恢复。

实际我们不必实际开辟数组 $\textit{arr}$ ，**可采用动态线段树**，**懒标记 $\textit{lazy}$ 标记区间 $[l,r]$ 进行累加的次数**，$\textit{tree}$ 记录区间 $[l,r]$ 的**最大值**，每次动态更新线段树即可。
```cpp
class MyCalendarTwo {
public:
    MyCalendarTwo() {

    }
    void update(int s, int e, int val, int l, int r, int idx) {
        if (r < s || l > e) return;
        if (s <= l && r <= e) {
            tree[idx].first += val;
            tree[idx].second += val;
        } else {
            int mid = (l + r) >> 1;
            update(s, e, val, l, mid, 2 * idx);
            update(s, e, val, mid + 1, r, 2 * idx + 1);
            tree[idx].first = tree[idx].second + max(tree[2 * idx].first, tree[2 * idx + 1].first);
        }
    }    
    bool book(int start, int end) {
        update(start, end - 1, 1, 0, 1e9, 1);
        if (tree[1].first > 2) {
            update(start, end - 1, -1, 0, 1e9, 1);
            return false;
        }
        return true;
    }
private:
    unordered_map<int, pair<int, int>> tree;
};
```
复杂度分析：
- 时间复杂度：$O(n \log C)$ ，其中 $n$ 为日程安排的数量。由于使用了线段树查询，线段树的最大深度为 $\log C$ ，每次最多会查询 $\log C$ 个节点，每次求最大的预定需的时间复杂度为 $O(\log C + \log C)$ ，因此时间复杂度为 $O(n \log C)$ ，在此 $C$ 取固定值即为 $10^9$ 
- 空间复杂度：$O(n \log C)$ ，其中 $n$ 为日程安排的数量。由于该解法采用的为动态线段树，线段树的最大深度为 $\log C$ ，每次预定最多会在线段树上增加 $\log C$ 个节点，因此空间复杂度为 $O(n \log C)$ ，在此 $C$ 取固定值即为 $10^9$ 
 