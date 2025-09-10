> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091742906.png)

Given a collection of intervals, merge all overlapping intervals.

**Example 1:**

```clike
Input: intervals = [[1,3],[2,6],[8,10],[15,18]]
Output: [[1,6],[8,10],[15,18]]
Explanation: Since intervals [1,3] and [2,6] overlaps, merge them into [1,6].
```

**Example 2:**

```clike
Input: intervals = [[1,4],[4,5]]
Output: [[1,5]]
Explanation: Intervals [1,4] and [4,5] are considered overlapping.
```

**NOTE**: input types have been changed on April 15, 2019. Please reset to default code definition to get new method signature.

**Constraints:** `intervals[i][0] <= intervals[i][1]`

题意：给出一个区间的集合，请合并所有重叠的区间。

--- 
①简单题;②sort(开头小排前面,否则结尾大排前面)+被覆盖时跳过,不重叠时入vector,否则直接修改ans.back()[1];③starts和ends分开排序,然后双指针i,j; starts[i+1]>ends[i],入数组{start[j],ends[i]},j=i+1;④位图;要区间全覆盖,记得*2,将某些区间分开;
### 解法1 排序
对区间数组，按照左端点从小到大排序，左端点相同则按照右端点从大到小排序（从大到小或从小到大都可以，这一点无所谓）。

然后从前往后扫描，如果**当前区间的左端点**大于结果数组**尾元素的右端点**，则不存在重叠，无需区间合并；否则比较大小，并用**当前区间的右端点**更新结果数组**尾元素的右端点**，进行区间合并。
```cpp
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end());
        vector<vector<int>> ans;
        for (auto &i : intervals) { 
            if (ans.empty() || i[0] > ans.back()[1]) ans.push_back(i); //不重叠
            else ans.back()[1] = max(i[1], ans.back()[1]); //i[0]<=ans.back()[1]
        }
        return ans;
    }
};
```
提交后效率如下：
```cpp
执行用时：60 ms, 在所有 C++ 提交中击败了49.30% 的用户
内存消耗：14 MB, 在所有 C++ 提交中击败了51.16% 的用户
```
