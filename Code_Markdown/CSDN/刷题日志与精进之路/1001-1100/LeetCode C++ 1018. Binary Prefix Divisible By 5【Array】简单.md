Given an array `A` of `0`s and `1`s, consider `N_i`: the i-th subarray from `A[0]` to `A[i]` interpreted as a binary number (from most-significant-bit to least-significant-bit.)

Return a list of booleans `answer`, where `answer[i]` is `true` if and only if `N_i` is divisible by 5.

**Example 1:**

```cpp
Input: [0,1,1]
Output: [true,false,false]
Explanation: 
The input numbers in binary are 0, 01, 011; which are 0, 1, and 3 in base-10.  Only the first number is divisible by 5, so answer[0] is true.
```

**Example 2:**

```cpp
Input: [1,1,1]
Output: [false,false,false]
```

**Example 3:**

```cpp
Input: [0,1,1,1,1,1]
Output: [true,false,false,false,true,false]
```

**Example 4:**

```cpp
Input: [1,1,1,0,1]
Output: [false,false,false,false,false]
```
**Note:**
- `1 <= A.length <= 30000`
-  `A[i]` is `0` or `1`

题意：给定由若干 `0` 和 `1` 组成的数组 `A` 。定义 `N_i`：从 `A[0]` 到 `A[i]` 的第 `i` 个子数组被解释为一个二进制数（从最高有效位到最低有效位）。返回布尔值列表 `answer`，只有当 `N_i` 可以被 `5` 整除时，答案 `answer[i]` 为 `true`，否则为 `false` 。

---
### 解法 位运算
由于二进制数组太长，因此肯定会溢出。而能够被 `5` 整除的数字其十进制形式的最低位要么为 `0` ，要么为 `5` 。所以有如下代码：
```cpp
class Solution {
public:
    vector<bool> prefixesDivBy5(vector<int>& A) {
        vector<bool> ans;
        int val = 0;
        for (const int &v : A) {
            val = ((val << 1) ^ v) % 10; //val = (val * 2 + v) % 10;
            ans.push_back(val == 0 || val == 5);
        }
        return ans;
    }
};
```
运算效率如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了85.28% 的用户
内存消耗：13.3 MB, 在所有 C++ 提交中击败了62.37% 的用户
```
