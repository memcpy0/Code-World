

求 1+2+...+n ，要求不能使用乘除法、for、while、if、else、switch、case等关键字及条件判断语句（A?B:C）。

 

**示例 1：**

```haskell
输入: n = 3
输出: 6
```

**示例 2：**

```haskell
输入: n = 9
输出: 45
```

**限制：** `1 <= n <= 10000`


---
### 解法1 直接乘除
```cpp
class Solution {
public:
    int sumNums(int n) {
        return n * (n + 1) / 2;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了30.48% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了37.91% 的用户
```
---
### 解法2 快速乘和右移
```cpp
class Solution {
public:
    int sumNums(int n) { //乘法改为快速乘函数,除法换为>>1
        int a = n, b = n + 1, ans = 0;
        while (b) {
            if (b & 1) ans += a;
            a <<= 1;
            b >>= 1;
        }
        return ans >> 1;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了30.48% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了28.12% 的用户
```



