> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个区间数组 `intervals` ，其中 `intervals[i] = [starti, endi]` ，且每个 `starti` 都 **不同** 。

区间 `i` 的 **右侧区间** 可以记作区间 `j` ，并满足 `startj` `>= endi` ，且 `startj` **最小化** 。注意 `i` 可能等于 `j` 。

返回一个由每个区间 `i` 的 **右侧区间** 在 `intervals` 中对应下标组成的数组。如果某个区间 `i` 不存在对应的 **右侧区间** ，则下标 `i` 处的值设为 `-1` 。

**示例 1：**
```js
输入：intervals = [[1,2]]
输出：[-1]
解释：集合中只有一个区间，所以输出-1。
```
**示例 2：**
```js
输入：intervals = [[3,4],[2,3],[1,2]]
输出：[-1,0,1]
解释：对于 [3,4] ，没有满足条件的“右侧”区间。
对于 [2,3] ，区间[3,4]具有最小的“右”起点;
对于 [1,2] ，区间[2,3]具有最小的“右”起点。
```
**示例 3：**
```js
输入：intervals = [[1,4],[2,3],[3,4]]
输出：[-1,2,-1]
解释：对于区间 [1,4] 和 [3,4] ，没有满足条件的“右侧”区间。
对于 [2,3] ，区间 [3,4] 有最小的“右”起点。
```
**提示：**
- `1 <= intervals.length <= 2 * 10^4`
- `intervals[i].length == 2`
- `-10^6 <= starti <= endi <= 10^6`
- 每个间隔的起点都 **不相同**

---
### 解法1 排序+二分
最简单的解决方案是对于集合中的每个区间，我们**扫描所有区间**找到**其起点大于当前区间的终点的区间**（具有最小差值），时间复杂度为 $O(n^2)$ ，在此不详细描述。

对于一个特定的 $its[i]$ 而言，其右端点固定，并且**我们只关心目标位置的左端点**。
1. 首先将**每个 $its[i][0]$ 与其对应的索引 $i$** 存储在数组 $itsStart$ 中，并**对其按 $its[i][0]$ 的大小进行排序**；
2. 然后枚举每个区间 $its[i]$ 的右端点 $its[i][1]$，利用二分查找在 $itsStart$ 中找到大于等于 $its[i][1]$ 的最小值 $val$ ，此时区间 $its[i]$ 对应的右侧区间即为 **$val$ 对应的索引**。
```cpp
class Solution {
public:
    vector<int> findRightInterval(vector<vector<int>>& intervals) {
        int n = intervals.size();
        vector<pair<int, int>> itsStart;
        for (int i = 0; i < n; ++i) itsStart.emplace_back(intervals[i][0], i);
        sort(itsStart.begin(), itsStart.end());
        vector<int> ans(n);
        for (int i = 0; i < n; ++i) {
            int l = 0, r = n;
            while (l < r) {
                int mid = (l + r) >> 1;
                if (itsStart[mid].first >= intervals[i][1]) r = mid;
                else l = mid + 1;
            }
            ans[i] = l == n ? -1 : itsStart[l].second;
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n \log n)$，其中 $n$ 为区间数组的长度。排序的时间为 $O(n \log n)$，每次进行二分查找花费的时间为 $O(\log n)$，一共需要进行 $n$ 次二分查找，因此总的时间复杂度为 $O(n \log n)$ 。
- 空间复杂度：$O(n)$，其中 $n$ 为区间数组的长度。$itsStart$ 一共存储了 $n$ 个元素，因此空间复杂度为 $O(n)$。

---
### 解法2 排序+双指针（莫队思想）
更进一步，在解法一中我们并没有对**求解询问的顺序**进行调整，这导致了**我们不得不每次都在整个左端点序列中进行二分**。朴素处理询问的方式，需要每次对整个序列进行双重扫描，复杂度为 $O(n^2)$ 。

但实际上，==如果我们按照「右端点从小到大」的顺序处理询问，其每个询问对应的「最右区间的左端点」也具有单调特性==。具体进行说明。

开辟两个新的数组：
- $startIntervals$，基于所有区间的起始点和下标从小到大排序。
- $endIntervals$，基于所有区间的结束点和下标从小到大排序。

我们从 $endIntervals$ 数组中取出第 $i$ 个区间，从左到右扫描 $startIntervals$ 数组中的**区间起点**来找到满足右区间条件的区间。设 **$endIntervals$ 数组中第 $i$ 个元素的右区间为 $startIntervals$ 数组中的第 $j$ 个元素**，此时可以知道： 
$$\begin{aligned}
\textit{startIntervals}[j-1][0] < \textit{endIntervals}[i][0],\\ \textit{startIntervals}[j][0] \ge \textit{endIntervals}[i][0] \end{aligned}$$

当我们遍历 $endIntervals$ 数组中第 $i+1$ 个元素时，**不需要从第一个索引开始扫描 $startIntervals$ 数组**，可以直接从第 $j$ 个元素开始扫描 $startIntervals$ 数组。由于**数组中所有的元素都是已排序**，因此可以知道 
$$\textit{startIntervals}[j-1][0] < \textit{endIntervals}[i][0] \le \textit{endIntervals}[i+1][1]$$
所以数组 $startIntervals$ 的前 $j−1$ 的元素的起始点都小于 $endIntervals[i+1][1]$ ，因此**可以直接跳过前 $j-1$ 个元素，只需要从 $j$ 开始搜索即可**。
 
因此，我们可以运用莫队思想：==通过调整询问的处理顺序，来减少扫描目标位置的指针移动次数==。将其从「必然进行 $n^2$ 次移动」优化为「最多不超过 $n$ 次移动」，从而将 构造答案 的复杂度从 $O(n^2)$ 优化为 $O(n)$ 。

最后，由于**每个 $intervals[i]$ 只关心目标位置的「左端点」**，因此我们无须对某一端进行分块，而直接使用双指针实现即可。
```cpp
class Solution {
public:
    vector<int> findRightInterval(vector<vector<int>>& intervals) {
        vector<pair<int, int>> startIntervals;
        vector<pair<int, int>> endIntervals;
        int n = intervals.size();
        for (int i = 0; i < n; i++) {
            startIntervals.emplace_back(intervals[i][0], i);
            endIntervals.emplace_back(intervals[i][1], i);
        }
        sort(startIntervals.begin(), startIntervals.end());
        sort(endIntervals.begin(), endIntervals.end());

        vector<int> ans(n, -1);
        for (int i = 0, j = 0; i < n && j < n; i++) {
            while (j < n && endIntervals[i].first > startIntervals[j].first)
                j++;
            if (j < n)
                ans[endIntervals[i].second] = startIntervals[j].second;
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：排序复杂度为 $O(n\log n)$ ；双指针构造答案的复杂度为 $O(n)$ 。整体复杂度为 $O(n\log{n})$
- 空间复杂度：$O(n)$