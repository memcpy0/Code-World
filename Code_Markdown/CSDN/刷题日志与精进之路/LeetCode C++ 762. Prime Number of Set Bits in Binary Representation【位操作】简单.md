<div class="notranslate"><p>
Given two integers <code>L</code> and <code>R</code>, find the count of numbers in the range <code>[L, R]</code> (inclusive) having a prime number of set bits in their binary representation.
</p><p>
(Recall that the number of set bits an integer has is the number of <code>1</code>s present when written in binary.  For example, <code>21</code> written in binary is <code>10101</code> which has 3 set bits.  Also, 1 is not a prime.)
</p><p>

</p><p><b>Example 1:</b><br></p>

```cpp
Input: L = 6, R = 10
Output: 4
Explanation:
6 -> 110 (2 set bits, 2 is prime)
7 -> 111 (3 set bits, 3 is prime)
9 -> 1001 (2 set bits , 2 is prime)
10->1010 (2 set bits , 2 is prime)
```

<p><b>Example 2:</b><br></p>

```cpp
Input: L = 10, R = 15
Output: 5
Explanation: 
10 -> 1010 (2 set bits, 2 is prime)
11 -> 1011 (3 set bits, 3 is prime)
12 -> 1100 (2 set bits, 2 is prime)
13 -> 1101 (3 set bits, 3 is prime)
14 -> 1110 (3 set bits, 3 is prime)
15 -> 1111 (4 set bits, 4 is not prime)
```
<p><b>Note:</b><br></p><ol>
<li><code>L, R</code> will be integers <code>L &lt;= R</code> in the range <code>[1, 10^6]</code>.</li>
<li><code>R - L</code> will be at most 10000.</li>
</ol><p></p></div>


题意：给定两个整数 `L` 和 `R` ，找到闭区间 `[L, R]` 范围内，置位位数为质数的整数个数。注意，计算置位代表二进制表示中 `1` 的个数 。

---
### 思路1 
虽然 `L, R` 范围在百万以内，但是 `R - L` 不会超过 `10000` ，所以可以采用稍微暴力一点的方法——计算 `[L, R]` 区间内每个数的置位位数，如果是 `2, 3, 5, 7, 11, 13, 17, 19`（这样做就**不用写判断质数的函数**），则置位位数为质数的整数个数加一。代码如下：
```cpp
class Solution {
private:
    int countSetBits(int n) {
        int ans = 0;
        while (n) {
            n = n & (n - 1);
            ++ans;
        }
        return ans;
    }
public:
    int countPrimeSetBits(int L, int R) {
        int num = 0;
        for (int i = L; i <= R; ++i) {
            int setBits = countSetBits(i);
            switch (setBits) {
                case 2:
                case 3:
                case 5:
                case 7:
                case 11:
                case 13:
                case 17:
                case 19: ++num; break;
            }
        }
        return num;
    }
};
```
效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了82.98% 的用户
内存消耗：5.7 MB, 在所有 C++ 提交中击败了80.56% 的用户
```
---
### 思路2
虽然上述代码已经够用了，但是如果 `L, R` 之间的范围再大一点，就需要更快的计算置位位数的函数。下面使用打表方法完成这一点：
```cpp
class Solution {
private:
    int setBitsOfNums[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
    int countSetBits(int n) {
        int ans = 0;
        while (n) {
            ans += setBitsOfNums[n & 0xf];
            n >>= 4;
        }
        return ans;
    }
public:
    int countPrimeSetBits(int L, int R) {
        int num = 0;
        for (int i = L; i <= R; ++i) {
            int setBits = countSetBits(i);
            switch (setBits) {
                case 2:
                case 3:
                case 5:
                case 7:
                case 11:
                case 13:
                case 17: 
                case 19: ++num; break;
            }
        }
        return num;
    }
};
```
虽然提交后的情况如下，似乎没有多少效率变化：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了82.98% 的用户
内存消耗：5.7 MB, 在所有 C++ 提交中击败了92.68% 的用户
```
如果进一步，使用 `8` 位查表的方式，代码如下：
```cpp
class Solution {
private:
    int setBitsOfNums[256];
    void prepare() {
        setBitsOfNums[0] = 0;
        setBitsOfNums[1] = 1;
        for (int i = 2; i < 256; ++i) {
            if (i & 1) setBitsOfNums[i] = setBitsOfNums[i >> 1] + 1;
            else setBitsOfNums[i] = setBitsOfNums[i >> 1];
        }
    }
    int countSetBits(int n) {
        return setBitsOfNums[n & 0xff] + setBitsOfNums[(n >> 8) & 0xff] + setBitsOfNums[(n >> 16) & 0xff];
    }
public:
    int countPrimeSetBits(int L, int R) {
        int num = 0;
        prepare(); //初始化setBitsOfNums数组
        for (int i = L; i <= R; ++i) {
            int setBits = countSetBits(i);
            switch (setBits) {
                case 2:
                case 3:
                case 5:
                case 7:
                case 11:
                case 13:
                case 17: 
                case 19: ++num; break;
            }
        }
        return num;
    }
};
```
时间效率稍有提升：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了92.26% 的用户
内存消耗：5.7 MB, 在所有 C++ 提交中击败了78.03% 的用户
```
