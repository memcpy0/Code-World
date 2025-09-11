Given a collection of intervals, find the **minimum** number of intervals you need to remove to make the rest of the intervals **non-overlapping.**

 

**Example 1:**
```cpp
Input: [[1,2],[2,3],[3,4],[1,3]]
Output: 1
Explanation: [1,3] can be removed and the rest of intervals are non-overlapping.
```
**Example 2:**
```cpp
Input: [[1,2],[1,2],[1,2]]
Output: 2
Explanation: You need to remove two [1,2] to make the rest of intervals non-overlapping.
```
**Example 3:**
```cpp
Input: [[1,2],[2,3]]
Output: 0
Explanation: You don't need to remove any of the intervals since they're already non-overlapping.
```
**Note:**
- You may assume the interval's end point is always bigger than its start point.
- Intervals like `[1,2]` and `[2,3]` have borders "touching" but they don't overlap each other.

题意：给出一个区间集合，移除最少数量的区间，使得剩余区间不重合。

---
### 解法 贪心
本题等同于——**最多保留多少个区间**，让它们之间互相不重叠。关键是按照**区间右端点**从小到大来进行排序。具体代码如下：
```cpp
class Solution {
public:
    int eraseOverlapIntervals(vector<vector<int>>& intervals) {
        if (intervals.empty()) return 0; 
        sort(intervals.begin(), intervals.end(), [&](const vector<int> &a, const vector<int> &b) {
            return a[1] != b[1] ? a[1] < b[1] : a[0] < b[0];
        });
        int nonOverlapping = 0, n = intervals.size(), end = INT_MIN;
        for (int i = 0; i < n; ++i) {
            if (intervals[i][0] >= end) {
                ++nonOverlapping;
                end = intervals[i][1]; 
            } 
        }
        return n - nonOverlapping;
    }
};
```
运行效率如下：
```cpp
执行用时：32 ms, 在所有 C++ 提交中击败了69.63% 的用户
内存消耗：9.3 MB, 在所有 C++ 提交中击败了78.73% 的用户
```




