Given an array of integers `arr`, and three integers `a`, `b` and `c`. You need to find the number of good triplets.

A triplet `(arr[i], arr[j], arr[k])` is **good** if the following conditions are true:

- `0 <= i < j < k < arr.length`
 -  `|arr[i] - arr[j]| <= a`
 - `|arr[j] - arr[k]| <= b`
 -  `|arr[i] - arr[k]| <= c`

Where `|x|` denotes the absolute value of `x` .

Return ***the number of good triplets.***

 

**Example 1:**

```swift
Input: arr = [3,0,1,1,9,7], a = 7, b = 2, c = 3
Output: 4
Explanation: There are 4 good triplets: [(3,0,1), (3,0,1), (3,1,1), (0,1,1)].
```

**Example 2:**

```swift
Input: arr = [1,1,2,2,3], a = 0, b = 0, c = 1
Output: 0
Explanation: No triplet satisfies all conditions.
```

 

**Constraints:**

- `3 <= arr.length <= 100`
 - `0 <= arr[i] <= 1000`
 -  `0 <= a, b, c <= 1000`

题意：给出一个整数数组 `arr` ，以及 `a`、`b` 、`c` 三个整数。统计其中好三元组的数量。

---
### 解法 暴力
暴力枚举即可：
```cpp
class Solution {
public:
    int countGoodTriplets(vector<int>& arr, int a, int b, int c) {
        int n = arr.size(), cnt = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                for (int k = j + 1; k < n; ++k) {
                    if (abs(arr[i] - arr[j]) <= a && abs(arr[j] - arr[k]) <= b && abs(arr[i] - arr[k]) <= c)
                        ++cnt;
                }
            }
        }
        return cnt;
    }
};
```
效率果然差：
```cpp
执行用时：60 ms, 在所有 C++ 提交中击败了24.33% 的用户
内存消耗：8.6 MB, 在所有 C++ 提交中击败了5.18% 的用户
```
稍微剪枝优化一下，**把好三元组的一个条件提前判断**：
```cpp
class Solution {
public:
    int countGoodTriplets(vector<int>& arr, int a, int b, int c) {
        int n = arr.size(), cnt = 0;
        for (int i = 0; i < n - 2; ++i) { 
            for (int j = i + 1; j < n - 1; ++j) {
                if (abs(arr[i] - arr[j]) > a) continue; //剪枝
                for (int k = j + 1; k < n; ++k) {
                    if (abs(arr[j] - arr[k]) <= b && abs(arr[i] - arr[k]) <= c)
                        ++cnt;
                }
            }
        }
        return cnt;
    }
};
```
效率提升如下：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了92.51% 的用户
内存消耗：8.5 MB, 在所有 C++ 提交中击败了5.18% 的用户
```
