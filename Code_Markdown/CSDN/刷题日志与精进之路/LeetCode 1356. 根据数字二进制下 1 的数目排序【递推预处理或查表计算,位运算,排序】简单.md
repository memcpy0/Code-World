> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an integer array `arr`. You have to sort the integers in the array in ascending order by the number of **1's** in their binary representation and in case of two or more integers have the same number of **1's** you have to sort them in ascending order.

*Return the sorted array.*

**Example 1:**
```swift
Input: arr = [0,1,2,3,4,5,6,7,8]
Output: [0,1,2,4,8,3,5,6,7]
Explantion: [0] is the only integer with 0 bits.
[1,2,4,8] all have 1 bit.
[3,5,6] have 2 bits.
[7] has 3 bits.
The sorted array by bits is [0,1,2,4,8,3,5,6,7]
```
**Example 2:**
```swift
Input: arr = [1024,512,256,128,64,32,16,8,4,2,1]
Output: [1,2,4,8,16,32,64,128,256,512,1024]
Explantion: All integers have 1 bit in the binary representation, you should just sort them in ascending order.
```
**Example 3:**
```swift
Input: arr = [10000,10000]
Output: [10000,10000]
```
**Example 4:**
```swift
Input: arr = [2,3,5,7,11,13,17,19]
Output: [2,3,5,17,7,11,13,19]
```
**Example 5:**
```swift
Input: arr = [10,100,1000,10000]
Output: [10,100,10000,1000]
```
**Constraints:**
- `1 <= arr.length <= 500`
- `0 <= arr[i] <= 10^4`

题意：给出一个整数数组 `arr` ，将数组中的元素按照其二进制表示中数字 `1` 的数目升序排序。如果存在多个数字二进制中 `1` 的数目相同，则必须将它们按照数值大小升序排列。返回排序后的数组。

---
### 解法 递推+预处理
这道题的关键点在于，如何获取数字二进制表示中的置位位数，一种方法是计数每个二进制位；**另一种方法是反复 `n &= (n - 1)`** 。但这两种方法，在求多个数的二进制中 $1$ 的个数时，都比较慢。

我们定义 $bit[i]$ 为数字 $i$ 二进制表示下数字 $1$ 的个数，则可以列出递推式（这里的递推式有好多种，比如减去最低置位的 $1$ ……）：
$$bit[i] = bit[i>>1] + (i \& 1)$$
所以我们线性预处理 $bit$ 数组然后去排序即可。
```cpp
class Solution {
public:
    vector<int> sortByBits(vector<int>& arr) {
        vector<int> bit(10001, 0);
        // for (int i = 1; i <= 10000; ++i) bit[i] = bit[i >> 1] + (i & 1); 
        for (int i = 1; i <= 10000; ++i) bit[i] = bit[i & (i - 1)] + 1;
        sort(arr.begin(), arr.end(), [&](int x, int y) { return bit[x] != bit[y] ? bit[x] < bit[y] : x < y; });
        return arr;
    }
};
```
---
### 解法2 查表计算1的个数
这里使用介绍过的**查表方法**：
```cpp
class Solution {
private: 
    int record[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
    int getOnesOfNumber(int n) {
        if (n == 0) return 0;
        return getOnesOfNumber(n >> 4) + record[n & 0xf];
    }
public:
    vector<int> sortByBits(vector<int>& arr) {
        sort(arr.begin(), arr.end(), [&](const int &a, const int &b) {
            int a1 = getOnesOfNumber(a), b1 = getOnesOfNumber(b);
            return a1 != b1 ? a1 < b1 : a < b;
        });
        return arr;
    }
};
```
每次比较两个元素时，都要调用 `getOnesOfNumber()` 方法，稍微慢一点：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了71.43% 的用户
内存消耗：10.3 MB, 在所有 C++ 提交中击败了28.03% 的用户
```
 ---
### 解法 不查表而通过二进制计算获取1的个数
计算 `the number of 1 bits` 和**计算汉明距离**（其实本题就是计算数 $n$ 与数 $0$ 的汉明距离）一样 ，可以通过二进制计算完成，时间复杂度是 $O(1)$ 。Java 的 `Integer::bitCount` 方法也是采用这个方法来计算二进制中 1 的个数的。[https://stackoverflow.com/questions/109023/count-the-number-of-set-bits-in-a-32-bit-integer/109025#](https://stackoverflow.com/questions/109023/count-the-number-of-set-bits-in-a-32-bit-integer/109025#)这里展示了更多的讨论。
```cpp
int getHammingWeight(int n) {
    n = (n&0x55555555) + ((n>>1)&0x55555555); // 每两个bit
    n = (n&0x33333333) + ((n>>2)&0x33333333); // 每4个bit
    n = (n&0x0f0f0f0f) + ((n>>4)&0x0f0f0f0f); // 每8个bit
    n = (n&0x00ff00ff) + ((n>>8)&0x00ff00ff); // 每16个bit
    n = (n&0x0000ffff) + ((n>>16)&0x0000ffff); // 每32个bit中1的个数
    return n;
}
```
或者：
```cpp
class Solution {
private:
    uint32_t bitCount(uint32_t v) {
        v = v - ((v >> 1) & 0x55555555); // put count of each 2 bits into those 2 bits
        v = (v & 0x33333333) + ((v >> 2) & 0x33333333); // put count of each 4 bits into those 4 bits
        // (v + (v >> 4) & 0x0F0F0F0F) 高4位+低4位中1的个数
        // *0x01010101 = A+B+C+D B+C+D C+D D
        return ((v + (v >> 4) & 0x0F0F0F0F) * 0x01010101) >> 24;
    }
public:
    vector<int> sortByBits(vector<int>& arr) {
        vector<int> bit(10001, 0);
        sort(arr.begin(), arr.end(), [&](int x, int y) { 
            int bx = bitCount(x), by = bitCount(y);
            return bx != by ? bx < by : x < y; 
        });
        return arr;
    }
};
```