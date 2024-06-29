输入数字 `n` ，按顺序打印出从 `1` 到最大的 `n` 位十进制数。比如输入 `3` ，则打印出 `1、2、3` 一直到最大的 `3` 位数 `999` 。

**示例 1:**

```swift
输入: n = 1
输出: [1,2,3,4,5,6,7,8,9] 
```

说明：
-  用返回一个整数列表来代替打印
-  `n` 为正整数
 
---
### 解法 顺序打印
这道题和书上的完全不同，不考大数就毫无意义：
```cpp
class Solution {
public:
    vector<int> printNumbers(int n) {
        int maxNum = pow(10, n);
        vector<int> ans;
        for (int i = 1; i < maxNum; ++i) ans.push_back(i);
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了71.11% 的用户
内存消耗：11.7 MB, 在所有 C++ 提交中击败了8.51% 的用户
```
