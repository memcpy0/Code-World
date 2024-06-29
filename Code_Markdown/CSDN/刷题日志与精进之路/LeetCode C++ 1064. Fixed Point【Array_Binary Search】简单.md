Given an array `A` of distinct integers sorted in ascending order, return the smallest index `i` that satisfies `A[i] == i`.  Return `-1` if no such `i` exists.

 

**Example 1:**
```haskell
Input: [-10,-5,0,3,7]
Output: 3
Explanation: 
For the given array, A[0] = -10, A[1] = -5, A[2] = 0, A[3] = 3, thus the output is 3.
```

**Example 2:**
```haskell
Input: [0,2,5,8,17]
Output: 0
Explanation: 
A[0] = 0, thus the output is 0.
```

**Example 3:**
```haskell
Input: [-10,-5,3,4,7,9]
Output: -1
Explanation: 
There is no such i that A[i] = i, thus the output is -1.
```

**Note:**
- `1 <= A.length < 10^4`
- `-10^9 <= A[i] <= 10^9`

题意：给定已经按升序排列、由不同整数组成的数组 `A` ，返回满足 `A[i] == i` 的最小索引 `i` 。如果不存在这样的 `i` ，返回 `-1` 。

---
### 解法1 顺序遍历
```cpp
class Solution {
public:
    int fixedPoint(vector<int>& A) {
        int n = A.size();
        for (int i = 0; i < n; ++i) if (A[i] == i) return i;
        return -1;
    }
};
```
运行效率如下：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了80.22% 的用户
内存消耗：10.8 MB, 在所有 C++ 提交中击败了10.99% 的用户
```
---
### 解法2 二分
如果是寻常的有序数组，则不存在二分做法。但是给定的**升序数组** `A` 中所有整数都不同，也就是说**数组中不存在两个相同的整数**，因此我们可以得到一个 $O(\log n)$ 的做法：
- 如果 `A[mid] < mid` ，则左侧不可能存在 `A[i] == i` ，原因在于：假设左侧存在一个 `A[i] == i` ，则从 `i` 到 `mid` 这一区间中任意相邻的两个整数之间最小的差为 `1` ，`A[mid] >= A[i] + (mid - i) >= mid` ，与假设出现矛盾。此时只有右侧可能存在 `A[i] == i` ；
- 如果 `A[mid] > mid` ，则右侧不可能存在 `A[i] == i` ，同理可得：假设右侧存在一个 `A[i] == i` ，则从 `mid` 到 `i` 这一区间中任意相邻的两个整数之间最小的差为 `1` ，`A[mid] <= A[i] - (i - mid) = mid` ，与假设相矛盾。此时只有左侧可能存在 `A[i] == i` ；
- 如果 `A[mid] == mid` ，则 `mid` 可能是答案之一，往左侧继续寻找。直到得到结果，或者最后返回 `-1` 。

```cpp
class Solution {
public:
    int fixedPoint(vector<int>& A) {
        int n = A.size(), lo = 0, hi = n - 1;
        while (lo < hi) { //注意退出循环的条件
            int mid = lo + (hi - lo) / 2;
            if (A[mid] == mid) hi = mid;
            else if (A[mid] < mid) lo = mid + 1;
            else hi = mid - 1;
        }
        return A[lo] == lo ? lo : -1;
    }
};
```
提交后运行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：10.9 MB, 在所有 C++ 提交中击败了8.79% 的用户
```
