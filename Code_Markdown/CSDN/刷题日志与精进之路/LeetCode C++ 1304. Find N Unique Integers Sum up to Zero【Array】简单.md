

Given an integer `n` , return **any** array containing `n` **unique** integers such that they add up to 0.

 

**Example 1:**

```swift
Input: n = 5
Output: [-7,-1,1,3,4]
Explanation: These arrays also are accepted [-5,-1,1,2,3] , [-3,-1,2,-2,4].
```

**Example 2:**

```swift
Input: n = 3
Output: [-1,0,1]
```

**Example 3:**

```swift
Input: n = 1
Output: [0]
```

 

**Constraints:** `1 <= n <= 1000`

题意：给出一个整数 `n`，返回任意一个由 `n` 个**各不相同**的整数组成的数组，并且这 `n` 个数相加和为 `0` 。

---
### 解法 分奇数和偶数
返回一个数组，其中的整数值是对称的，比如 `(-x, +x)` 。如果 `n` 是奇数，就添加一个零：
```cpp
class Solution {
public:
    vector<int> sumZero(int n) {
        vector<int> ans;
        if (n & 1) { ans.push_back(0); --n; }
        int x = 1;
        while (n > 0) {
            ans.push_back(x);
            ans.push_back(-x);
            n -= 2;
            ++x;
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了43.75% 的用户
内存消耗：7 MB, 在所有 C++ 提交中击败了17.19% 的用户
```
