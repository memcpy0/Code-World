

Given an array `A` of positive integers, let `S` be the sum of the digits of the minimal element of `A`. Return `0` if `S` is odd, otherwise return `1` .

**Example 1:**
```haskell
Input: [34,23,1,24,75,33,54,8]
Output: 0
Explanation: 
The minimal element is 1, and the sum of those digits is S = 1 which is odd, so the answer is 0.
```

**Example 2:**

```haskell
Input: [99,77,33,66,55]
Output: 1
Explanation: 
The minimal element is 33, and the sum of those digits is S = 3 + 3 = 6 which is even, so the answer is 1.
```

 

**Constraints:**
- `1 <= A.length <= 100`
-  `1 <= A[i] <= 100`

题意：计算数组 `A` 当中最小的那个元素各个数位上数字之和。假如计算结果是**奇数**就返回 `0` ，否则返回 `1` 。

---
### 解法 顺序遍历
```cpp
class Solution {
public:
    int sumOfDigits(vector<int>& A) {
        int mn = *min_element(A.begin(), A.end()), s = 0; 
        do { s += mn % 10; mn /= 10; } while (mn);
        return !(s & 1);
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了86.96% 的用户
内存消耗：8.3 MB, 在所有 C++ 提交中击败了14.76% 的用户
```
