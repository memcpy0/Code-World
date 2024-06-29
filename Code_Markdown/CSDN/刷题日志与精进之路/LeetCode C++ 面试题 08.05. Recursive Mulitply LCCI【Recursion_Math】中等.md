

Write a recursive function to multiply two **positive integers** without using the `*` operator. You can use addition, subtraction, and bit shifting, but you should minimize the number of those operations.

**Example 1:**

```swift
Input: A = 1, B = 10
Output: 10
```

**Example 2:**

```swift
Input: A = 3, B = 4
Output: 12
```

**Note:** The result will not overflow.

题意： 写一个递归函数，不使用 `*` 运算符， 实现两个正整数的相乘。可以使用加号、减号、位移，但要吝啬一些。

-----
### 解法1 乘法
```cpp
class Solution {
public:
    int multiply(int A, int B) {
        return A * B;
    }
};
```
效率毋庸置疑：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了6.04% 的用户
```
---
### 解法2 直接加法
递归版：
```cpp
class Solution {
public:
    int multiply(int A, int B) {
        if (B == 0) return 0;
        if (B == 1) return A;
        return A + multiply(A, B - 1);
    }
};
```
迭代版：
```cpp
class Solution {
public:
    int multiply(int A, int B) {
        int ans = 0;
        for (int i = 0; i < B; ++i) ans += A;
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6 MB, 在所有 C++ 提交中击败了24.56% 的用户
```

---
### 解法3 快速乘
递归版：
```cpp
class Solution {
public:
    int multiply(int A, int B) {
        if (B == 0) return 0;
        if (B == 1) return A;
        if (B & 1) return multiply(A, B - 1) + A;
        return multiply(A, B >> 1) * 2;
    }
};
```
迭代版：
```cpp
class Solution {
public:
    int multiply(int A, int B) {
        int ans = 0;
        while (B) {
            if (B & 1) ans += A;
            B >>= 1;
            A <<= 1; //A*=2,A+=A可能溢出
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6 MB, 在所有 C++ 提交中击败了24.56% 的用户
```
