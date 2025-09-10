> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091744290.png)

Given a set of ***non-overlapping intervals***, insert a new interval into the intervals (**merge** if necessary).

You may assume that the intervals were **initially sorted** according to their start times.

 

**Example 1:**

```swift
Input: intervals = [[1,3],[6,9]], newInterval = [2,5]
Output: [[1,5],[6,9]]
```

**Example 2:**

```swift
Input: intervals = [[1,2],[3,5],[6,7],[8,10],[12,16]], newInterval = [4,8]
Output: [[1,2],[3,10],[12,16]]
Explanation: Because the new interval [4,8] overlaps with [3,5],[6,7],[8,10].
```

**Example 3:**

```swift
Input: intervals = [], newInterval = [5,7]
Output: [[5,7]]
```

**Example 4:**

```swift
Input: intervals = [[1,5]], newInterval = [2,3]
Output: [[1,5]]
```

**Example 5:**

```swift
Input: intervals = [[1,5]], newInterval = [2,7]
Output: [[1,7]]
```
 
**Constraints:** 
 -  <code>0 <= intervals.length <= 10<sup>4</sup></code>
 -  `intervals[i].length == 2`
  -  <code>0 <= intervals[i][0] <= intervals[i][1] <= 10<sup>5</sup></code>
  - `intervals` is sorted by `intervals[i][0]` in **ascending** order.
 -  `newInterval.length == 2`
 -  <code>0 <= newInterval[0] <= newInterval[1] <= 10<sup>5</sup></code>
 
题意：给出一个无重叠的、按照区间起始端点排序的区间列表。在列表中插入一个新的区间，确保列表中的区间仍然有序且不重叠（如果有必要的话，可以合并区间）。

---
### 解法 排序后合并区间
注意区间合并的写法即可：
```cpp
class Solution {
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        intervals.push_back(newInterval);
        sort(intervals.begin(), intervals.end(), [&](const vector<int>& a, const vector<int>& b) {
            return a[0] != b[0] ? a[0] < b[0] : a[1] < b[1]; //按照左端点从小到大排序; 否则按照右端点从小到大排序;
        });
        vector<vector<int>> ans;
        for (const vector<int>& interval : intervals) {
            if (ans.empty() || ans.back()[1] < interval[0]) ans.push_back(interval); //加入新区间
            else ans.back()[1] = max(ans.back()[1], interval[1]); //此时ans.back()[1] >= interval[0],重叠区间时进行合并 
        }
        return ans;
    }
};
```
提交结果如下：
```cpp
执行用时：76 ms, 在所有 C++ 提交中击败了14.98% 的用户
内存消耗：17 MB, 在所有 C++ 提交中击败了22.24% 的用户
```
