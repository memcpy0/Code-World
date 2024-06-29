Given an array `A` of integers, for each integer `A[i]` we may choose any `x` with `-K <= x <= K`, and add `x` to `A[i]`. After this process, we have some array `B`.

Return **the smallest possible difference** between the maximum value of `B` and the minimum value of `B`.

 

**Example 1:**

```swift
Input: A = [1], K = 0
Output: 0
Explanation: B = [1]
```

**Example 2:**

```swift
Input: A = [0,10], K = 2
Output: 6
Explanation: B = [2,8]
```

**Example 3:**

```swift
Input: A = [1,3,6], K = 3
Output: 0
Explanation: B = [3,3,3] or B = [4,4,4]
```

 

**Note:**
- `1 <= A.length <= 10000`
 - `0 <= A[i] <= 10000`
 -  `0 <= K <= 10000`

题意：给定一个整数数组 `A` ，把数组中的每个元素 `A[i]` 都加上一个任意值 `x (-K <= x <= K)` ，从而得到一个新数组。返回新数组 `B` 的最大值和最小值之间，可能存在的最小差值。

---
### 解法 找规律
对于一个数组，其中必然存在最大值和最小值。要得到新数组的最小差值，就要对原来的最大值减去一个值，对最小值加上一个值，其他值呢？不需要考虑吗？不需要，**因为我们可以加减值使得它们落在新的最大值和最小值之间。** 于是，如果原本的最大值和最小值之差 `<= 2K` 时，最小差值可以为零；否则，原本的最大值 `- K` 、原本的最小值 `+ K` ，得到新的最小绝对差值。具体代码如下：
```cpp
class Solution {
public:
    int smallestRangeI(vector<int>& A, int K) {
        if (A.size() <= 1) return 0;
        int mn = 11111, mx = -1;
        for (int i = 0; i < A.size(); ++i) {
            mn = min(mn, A[i]);
            mx = max(mx, A[i]);
        }  
        return abs(mx - mn) <= 2 * K ? 0 : abs((mx - K) - (mn + K)); 
    }
};
```
效率如下：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了99.48% 的用户
内存消耗：14.9 MB, 在所有 C++ 提交中击败了5.59% 的用户
```
