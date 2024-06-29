Write a program to check whether a given number is an ugly number.

Ugly numbers are **positive numbers** whose prime factors only include `2, 3, 5`.

**Example 1:**

```swift
Input: 6
Output: true
Explanation: 6 = 2 × 3
```

**Example 2:**

```swift
Input: 8
Output: true
Explanation: 8 = 2 × 2 × 2
```

**Example 3:**

```swift
Input: 14
Output: false 
Explanation: 14 is not ugly since it includes another prime factor 7.
```

**Note:**
- `1` is typically treated as an ugly number.
- Input is within the 32-bit signed integer range: <code>[−2<sup>31</sup>,  2<sup>31</sup> − 1]</code>.


题意：编写一个程序判断给定的数是否为丑数。丑数就是只包含质因数 `2, 3, 5` 的正整数。


---
### 解法 
不断整除 `2, 3, 5` ，最后判断是否等于 `1` 。注意，负数和零都不是丑数，一是丑数。具体代码如下：
```cpp
class Solution {
public:
    bool isUgly(int num) {
        if (num <= 0) return false;
        while (num % 2 == 0) num /= 2;
        while (num % 3 == 0) num /= 3;
        while (num % 5 == 0) num /= 5;
        return num == 1;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.8 MB, 在所有 C++ 提交中击败了39.39% 的用户
```
