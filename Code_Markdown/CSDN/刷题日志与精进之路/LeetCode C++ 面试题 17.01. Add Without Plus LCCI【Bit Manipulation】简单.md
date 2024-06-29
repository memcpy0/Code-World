Write a function that adds two numbers. You should not use `+` or any arithmetic operators.

**Example:**
```swift
Input: a = 1, b = 1
Output: 2
```

 

**Note:**
-  `a` and `b` may be `0` or negative.
 - The result fits in 32-bit integer.

题意：设计一个函数把两个数字相加。不得使用 `+` 或者其他算术运算符。注意，`a, b` 均可能是负数或 `0` ，结果不会溢出 `32` 位整数。

---
### 解法 位运算
这是一道做过的题目，思想很简单。十进制的加法是 `+` ，二进制的加法是 `xor` 异或运算。首先，考虑 `a, b` 二进制表示中对应为 `1` 的部分，它们相加后一定需要进位，因此使用 `a & b` 得到这一部分，然后对其左移一位（相当于对其中的每个二进制 `1` 都加一进位），结果为 `ta` ；同时，`a ^ b` 得到相加后不需要进位的部分 `tb` 。递归加 `ta, tb` ，直到 `ta = 0` 为止。具体代码如下：
```cpp
class Solution {
public:
    int add(int a, int b) { 
        if (!a) return b; //使用(uint32_t)类型转换是为了避免负数左移溢出的UB行为
        return add((uint32_t)(a & b) << 1, a ^ b); 
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6 MB, 在所有 C++ 提交中击败了5.47% 的用户
```
迭代版的代码如下：
```cpp
class Solution {
public:
    int add(int a, int b) {  
        uint32_t ka = a, kb = b;
        do {
            uint32_t ta = (ka & kb) << 1, tb = ka ^ kb; 
            ka = ta, kb = tb;    
        } while (ka);
        return kb;
    }
};
```
同样的效率：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6 MB, 在所有 C++ 提交中击败了6.14% 的用户
```
