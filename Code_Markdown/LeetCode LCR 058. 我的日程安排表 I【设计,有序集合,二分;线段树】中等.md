> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202309111940235.png)

---
类似处理区间模型的题有：
- [715. Range 模块](https://leetcode.cn/problems/range-module/)
- [2276. 统计区间中的整数数目](https://leetcode.cn/problems/count-integers-in-intervals/)
- [731. My Calendar II](https://leetcode.cn/problems/my-calendar-ii/)
- [732. My Calendar III](https://leetcode.cn/problems/my-calendar-iii/)
- [LCR 058. 我的日程安排表 I](https://leetcode.cn/problems/fi9suh/)
### 解法1 直接遍历
我们记录下所有已经预订的课程安排区间，当我们预订新的区间 $[\textit{start}, \textit{end})$ 时，此时检查当前已经预订的每个日程安排是否与新日程安排冲突。若不冲突，则可以添加新的日程安排。
- 对于两个区间 $[s_1, e_1)$ 和 $[s_2, e_2)$ ，如果二者没有交集，则此时应当满足 $s_1 \ge e_2$ 或者 $s_2 \ge e_1$ ，这就意味着如果满足 $s_1 < e_2$ 并且 $s_2 < e_1$ 。
​
```cpp
class MyCalendar {
private:
    vector<pair<int, int>> booked;
public:
    bool book(int start, int end) {
        for (auto &[l, r] : booked)
            if (l < end && start < r) return false;
        booked.emplace_back(start, end);
        return true;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n^2)$ ，其中 $n$ 表示日程安排的数量。由于每次在进行预订时，都需要遍历所有已经预订的行程安排。
- 空间复杂度：$O(n)$ ，其中 $n$ 表示日程安排的数量。需要保存所有已经预订的行程。

---
### 解法2 二分查找
如果我们按时间顺序维护日程安排，则可以**通过二分查找日程安排的情况**来检查新日程安排是否可以预订，若可以预订则在排序结构中更新插入日程安排。

需要一个数据结构能够**保持元素排序和支持快速插入**，可以用 $\texttt{TreeSet}$ 来构建。对于给定的区间 $[start,end)$ ，我们每次查找起点大于等于 $\textit{end}$ 的第一个区间 $[l_1,r_1)$ ，同时紧挨着 $[l_1,r_1)$ 的前一个区间为 $[l_2,r_2)$ ，此时如果满足 $r_2 \le \textit{start} < \textit{end} \le l_1$ ，则该区间可以预订。
```cpp
class MyCalendar {
    set<pair<int, int>> booked;
public:
    bool book(int start, int end) {
        auto it = booked.lower_bound({end, 0});
        if (it == booked.begin() || (--it)->second <= start) {
            booked.emplace(start, end);
            return true;
        }
        return false;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n\log n)$ ，其中 $n$ 表示日程安排的数量。由于每次在进行预订时，都需要进行二分查找，需要的时间为 $O(\log n)$ 。
- 空间复杂度：$O(n)$ ，其中 $n$ 表示日程安排的数量。需要保存所有已经预订的行程。

---
### 解法3 线段树
利用线段树，假设我们开辟了数组 $\textit{arr}[0,\cdots, 10^9]$ ，初始时每个元素的值都为 $0$ ，对于每次行程预订的区间 $[start, end)$ ，则我们将区间中的元素 $\textit{arr}[\textit{start},\cdots,\textit{end}-1]$ 中的每个元素都标记为 $1$ ，每次调用 $\texttt{book}$ 时，我们只需要检测 $\textit{arr}[\textit{start},\cdots,\textit{end}-1]$ 区间内是否有元素被标记为 $1$ 。实际我们不必实际开辟数组 $\textit{arr}$ ，可采用动态线段树，懒标记 $\textit{lazy}$ 标记区间 $[l,r]$ 已经被预订，$\textit{tree}$ 记录区间 $[l,r]$ 的是否存在标记为 $1$ 的元素。

每次进行 $\texttt{book}$ 操作时，首先判断区间 $[\textit{start},\cdots,\textit{end}-1]$ 是否存在元素被标记，如果存在被标记为 $1$ 的元素，则表明该区间不可预订；否则，则将可以预订。预订完成后，将 $\textit{arr}[\textit{start},\cdots,\textit{end}-1]$ 进行标记为 $1$ ，并同时更新线段树。
```cpp
class MyCalendar {
    unordered_set<int> tree, lazy;
public:
    bool query(int start, int end, int l, int r, int idx) {
        if (r < start || end < l) {
            return false;
        }
        /* 如果该区间已被预订，则直接返回 */
        if (lazy.count(idx)) {
            return true;
        }
        if (start <= l && r <= end) {
            return tree.count(idx);
        }
        int mid = (l + r) >> 1;
        return query(start, end, l, mid, 2 * idx) ||
               query(start, end, mid + 1, r, 2 * idx + 1);
    }

    void update(int start, int end, int l, int r, int idx) {
        if (r < start || end < l) {
            return;
        }
        if (start <= l && r <= end) {
            tree.emplace(idx);
            lazy.emplace(idx);
        } else {
            int mid = (l + r) >> 1;
            update(start, end, l, mid, 2 * idx);
            update(start, end, mid + 1, r, 2 * idx + 1);
            tree.emplace(idx);
            if (lazy.count(2 * idx) && lazy.count(2 * idx + 1)) {
                lazy.emplace(idx);
            }
        }
    }

    bool book(int start, int end) {
        if (query(start, end - 1, 0, 1e9, 1)) {
            return false;
        }
        update(start, end - 1, 0, 1e9, 1);
        return true;
    }
};
```
或者：
```cpp
class MyCalendar {
public:
    MyCalendar() {

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
        if (tree[1].first > 1) {
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
- 时间复杂度：$O(n \log C)$ ，其中 $n$ 为日程安排的数量。由于使用了线段树查询，线段树的最大深度为 $\log C$ ，每次最多会查询 $\log C$ 个节点，每次求预定需时间复杂度为 $O(\log C + \log C)$ ，因此时间复杂度为 $O(n \log C)$ ，在此 $C$ 取固定值即为 $10^9$ 
- 空间复杂度：$O(n \log C)$ ，其中 $n$ 为日程安排的数量。由于该解法采用的为动态线段树，线段树的最大深度为 $\log C$ ，每次预定最多会在线段树上增加 $\log C$ 个节点，因此空间复杂度为 $O(n \log C)$ ，在此 $C$ 取固定值即为 $10^9$ 
 
