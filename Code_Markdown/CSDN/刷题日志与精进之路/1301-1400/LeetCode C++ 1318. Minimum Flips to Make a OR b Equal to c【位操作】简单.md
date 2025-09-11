

Given 3 positives numbers `a`, `b` and `c`. Return the minimum flips required in some bits of `a` and `b` to make ( `a OR b == c` ). (bitwise OR operation).

Flip operation consists of change **any** single bit 1 to 0 or change the bit 0 to 1 in their binary representation.

 

**Example 1:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201106174832303.png#pic_center)

```swift
Input: a = 2, b = 6, c = 5
Output: 3
Explanation: After flips a = 1 , b = 4 , c = 5 such that (a OR b == c)
```

**Example 2:**

```swift
Input: a = 4, b = 2, c = 7
Output: 1
```

**Example 3:**

```swift
Input: a = 1, b = 2, c = 3
Output: 0
```

 

**Constraints:**
- `1 <= a <= 10^9`
- `1 <= b <= 10^9`
 - `1 <= c <= 10^9`

题意：给出三个正整数 `a`、`b` 和 `c` 。可以对 `a` 和 `b` 的二进制表示进行位翻转操作，返回能够使按位或运算 `a OR b == c` 成立的最小翻转次数。

---
### 解法 直接遍历每一位
由于都是正整数，所以可以不断 `>>` 直到变为零。接着判断是否需要翻转、翻转几位：
```cpp
class Solution {
public:
    int minFlips(int a, int b, int c) {
        int flips = 0;
        while (a || b || c) {
            int ta = a & 1, tb = b & 1, tc = c & 1;
            if (tc == 1 && ta + tb == 0) ++flips; //ta,tb都是0
            else if (tc == 0) flips += (ta + tb); //ta==tb==0时翻转0次; ta,tb有一个是1时翻转1次; ta==tb==1时翻转2次
            a >>= 1, b >>= 1, c >>= 1;
        }
        return flips;
    }
};
```
执行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.1 MB, 在所有 C++ 提交中击败了5.68% 的用户
```
