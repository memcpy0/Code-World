

Given two integer arrays `startTime` and `endTime` and given an integer `queryTime`.

The `ith` student started doing their homework at the time `startTime[i]` and finished it at time `endTime[i]`.

Return *the number of students* doing their homework at time `queryTime`. More formally, return the number of students where `queryTime` lays in the interval `[startTime[i], endTime[i]]` inclusive.

 

**Example 1:**

```swift
Input: startTime = [1,2,3], endTime = [3,2,7], queryTime = 4
Output: 1
Explanation: We have 3 students where:
The first student started doing homework at time 1 and finished at time 3 and wasn't doing anything at time 4.
The second student started doing homework at time 2 and finished at time 2 and also wasn't doing anything at time 4.
The third student started doing homework at time 3 and finished at time 7 and was the only student doing homework at time 4.
```

**Example 2:**

```swift
Input: startTime = [4], endTime = [4], queryTime = 4
Output: 1
Explanation: The only student was doing their homework at the queryTime.
```

**Example 3:**

```swift
Input: startTime = [4], endTime = [4], queryTime = 5
Output: 0
```

**Example 4:**

```swift
Input: startTime = [1,1,1,1], endTime = [1,3,2,4], queryTime = 7
Output: 0
```

**Example 5:**

```swift
Input: startTime = [9,8,7,6,5,4,3,2,1], endTime = [10,10,10,10,10,10,10,10,10], queryTime = 5
Output: 5
```
**Constraints:**
- `startTime.length == endTime.length`
- `1 <= startTime.length <= 100`
- `1 <= startTime[i] <= endTime[i] <= 1000`
 - `1 <= queryTime <= 1000`

题意：返回在查询时间 `queryTime` 时正在做作业的学生人数。

---
### 解法 简单遍历
```cpp
class Solution {
public:
    int busyStudent(vector<int>& startTime, vector<int>& endTime, int queryTime) {
        int stu = 0, n = startTime.size();
        for (int i = 0; i < n; ++i)
            if (startTime[i] <= queryTime && queryTime <= endTime[i])
                ++stu;
        return stu;
    }
};
```
执行效率见下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了47.55% 的用户
内存消耗：11.2 MB, 在所有 C++ 提交中击败了5.21% 的用户
```
