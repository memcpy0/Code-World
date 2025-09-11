Write a function that takes an unsigned integer and return the number of '1' bits it has (also known as the **Hamming weight**).

**Example 1:**

```swift
Input: 00000000000000000000000000001011
Output: 3
Explanation: The input binary string 00000000000000000000000000001011 has a total of three '1' bits.
```

**Example 2:**

```swift
Input: 00000000000000000000000010000000
Output: 1
Explanation: The input binary string 00000000000000000000000010000000 has a total of one '1' bit.
```

**Example 3:**

```swift
Input: 11111111111111111111111111111101
Output: 31
Explanation: The input binary string 11111111111111111111111111111101 has a total of thirty one '1' bits.
```

 

**Note:**
 - Note that in some languages such as Java, there is no unsigned integer type. In this case, the input will be given as **signed integer type** and should not affect your implementation, as the internal binary representation of the integer is the same whether it is signed or unsigned.
 - In Java, the compiler represents the signed integers using **2's complement notation**. Therefore, in **Example 3** above the input represents the signed integer `-3` .

**Follow up:** If this function is called many times, how would you optimize it?

题意：编写一个函数，输入是一个无符号整数，返回其二进制表示中的置位位数。

---
### 思路1 `n & (n - 1)`
代码如下：
```cpp
class Solution {
public:
    int hammingWeight(uint32_t n) {
        int ans = 0;
        while (n) {
            n = n & (n - 1);
            ++ans;
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了47.21% 的用户
内存消耗：5.7 MB, 在所有 C++ 提交中击败了94.34% 的用户
```
---
### 思路2 查表
如果这一函数需要多次调用，就应该事先打表，然后直接查询。代码如下：
```cpp
class Solution {
public:
    int hammingWeight(uint32_t n) {
        int setBitsOfNums[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
        int ans = 0;
        while (n) {
            ans += setBitsOfNums[n & 0xf];
            n >>= 4;
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了47.21% 的用户
内存消耗：5.7 MB, 在所有 C++ 提交中击败了87.08% 的用户
```
或者换成八位查表：
```cpp
class Solution {
public:
    int setBitsOfNums[256] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};  
    int hammingWeight(uint32_t n) {  
        return setBitsOfNums[n & 0xff] + setBitsOfNums[(n >> 8) & 0xff] + setBitsOfNums[(n >> 16) & 0xff] + setBitsOfNums[(n >> 24) & 0xff];
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了47.21% 的用户
内存消耗：5.9 MB, 在所有 C++ 提交中击败了21.95% 的用户
```
