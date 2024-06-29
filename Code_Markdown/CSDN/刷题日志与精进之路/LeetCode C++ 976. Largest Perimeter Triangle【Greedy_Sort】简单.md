

Given an array `A` of positive lengths, return the largest perimeter of a triangle with **non-zero** area, formed from `3` of these lengths.

If it is impossible to form any triangle of non-zero area, return `0`.

 

**Example 1:**

```swift
Input: [2,1,2]
Output: 5
```

**Example 2:**

```swift
Input: [1,2,1]
Output: 0
```

**Example 3:**

```swift
Input: [3,2,3,4]
Output: 10
```

**Example 4:**

```swift
Input: [3,6,2,3]
Output: 8
```

 

**Note:**
- `3 <= A.length <= 10000`
- `1 <= A[i] <= 10^6`

题意：给定正整数数组 `A` ，返回由其中三个长度组成的、面积不为零的三角形的最大周长。如果不能形成任何面积不为零的三角形，返回 `0` 。

---
### 解法 贪心
数据范围比较大，暴力法绝对过不了，因此需要思考新的方法。由于要求最大周长，我们很容易想到，可以**将数组从大到小排序**，然后从其中选择**最大的**、**能够满足三角形条件**的（形成非零面积）三条边，计算其周长。

如何选择这三条边呢？或者说，使用怎样的**贪心策略**？我们的做法是，**选择从大到小的连续三条边**，如果无法形成非零面积，就继续选择新的三条边。直到求出最大周长，或者返回 `0` 。

为什么这样进行贪心选择？或者是，**为什么排序之后遍历相邻元素可行**，有没有可能最优解为非相邻元素？不可能。我们将排序后的数组设为 `A[0], A[1], ..., A[n - 1]` ，从其中选择 `A[i], A[i + 1], A[i + 2]` 三条边，如果满足**次大两边之和大于最大边**，则这三条边就能够形成三角形，于是得到最优解 `A[i] + A[i + 1] + A[i + 2]` 。

反证：假设后面存在 `A[j], A[k]` ，满足 `A[i] > A[j] > A[i + 1] > A[k] > A[i + 2]` ，即存在非相邻元素。
- 由于 `A[i + 1] + A[i + 2] > A[i]` ，且 `A[j] > A[i + 1]` ，则 `A[j] + A[i + 2] > A[i]` ，与最优解 `A[i], A[i + 1], A[i + 2]` 相冲突；
-  由于 `A[i + 1] + A[i + 2] > A[i]` ，且 `A[k] > A[i + 2]` ，则 `A[i + 1] + A[k] > A[i]` ，与最优解 `A[i], A[i + 1], A[i + 2]` 相冲突；

由此，最优解一定为**排序后相邻元素**。具体代码如下：
```cpp
class Solution {
public:
    int largestPerimeter(vector<int>& A) {
        sort(A.begin(), A.end(), greater<int>());
        int n = A.size();
        for (int i = 0; i < n - 2; ++i) 
            if (A[i] < A[i + 1] + A[i + 2]) 
                return A[i] + A[i + 1] + A[i + 2];
        return 0;
    }
};
```
执行效率如下：
```cpp
执行用时：100 ms, 在所有 C++ 提交中击败了90.85% 的用户
内存消耗：20.4 MB, 在所有 C++ 提交中击败了65.71% 的用户
```
