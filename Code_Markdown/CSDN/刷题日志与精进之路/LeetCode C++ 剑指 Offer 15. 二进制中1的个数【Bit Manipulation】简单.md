

请实现一个函数，输入一个整数，输出该数二进制表示中 `1` 的个数。例如，把 `9` 表示成二进制是 `1001`，有 `2` 位是 `1`。因此，如果输入 `9`，则该函数输出 `2`。

**示例 1：**

```swift
输入：00000000000000000000000000001011
输出：3
解释：输入的二进制串 00000000000000000000000000001011 中，共有三位为 '1'。
```

**示例 2：**

```swift
输入：00000000000000000000000010000000
输出：1
解释：输入的二进制串 00000000000000000000000010000000 中，共有一位为 '1'。
```

**示例 3：**

```swift
输入：11111111111111111111111111111101
输出：31
解释：输入的二进制串 11111111111111111111111111111101 中，共有 31 位为 '1'。
```

---
### 解法 位操作
这个题做过太多次了：
```cpp
class Solution {
public:
    int hammingWeight(uint32_t n) { //注意是无符号数
        int setbits = 0;
        while (n) {
            n = n & (n - 1);
            ++setbits;
        }
        return setbits;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了5.15% 的用户
```
