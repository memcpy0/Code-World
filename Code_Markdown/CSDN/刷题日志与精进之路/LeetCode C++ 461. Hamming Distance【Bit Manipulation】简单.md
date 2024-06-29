<p>The <a href="https://en.wikipedia.org/wiki/Hamming_distance">Hamming distance</a> between two integers is the number of positions at which the corresponding bits are different.</p>

<p>Given two integers <code>x</code> and <code>y</code>, return <em>the <strong>Hamming distance</strong> between them</em>.</p>

 
<p><strong>Example 1:</strong></p>

```go
Input: x = 1, y = 4
Output: 2
Explanation:
1   (0 0 0 1)
4   (0 1 0 0)
       ↑   ↑
The above arrows point to positions where the corresponding bits are different.
```
 
<p><strong>Example 2:</strong></p>

```go
Input: x = 3, y = 1
Output: 1
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>0 &lt;=&nbsp;x, y &lt;= 2<sup>31</sup> - 1</code></li>
</ul>


题意：两个整数之间的汉明距离指的是这两个数字对应二进制位不同的位置的数目。给出两个整数 `x` 和 `y` ，计算它们之间的汉明距离。


---
### 解法1 暴力遍历
暴力遍历所有位，$O(32)$ ：
```cpp
class Solution {
public:
    int hammingDistance(int x, int y) {
        int ans = 0;
        for (int i = 0; i < 32; ++i) 
            if (((x >> i) & 1) != ((y >> i) & 1)) ++ans;
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了31.48% 的用户
内存消耗：5.9 MB, 在所有 C++ 提交中击败了5.79% 的用户
```
---
### 解法2 异或
将两个数异或得到新数，`x ^ y = z` ，`z` 的二进制形式中为 `1` 的位就是 `x, y` 不同的位。然后就转换为了求一个数字的二进制形式中有多少个 `1` 。

遍历 `z` 的所有位：
```cpp
class Solution {
public:
    int hammingDistance(int x, int y) {
        int ans = 0, z = x ^ y;
        for (int i = 0; i < 32; ++i) if ((z >> i) & 1) ++ans; 
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.9 MB, 在所有 C++ 提交中击败了9.28% 的用户
```
反复使用 `n & (n - 1)` 消去最低位的 `1` ：
```cpp
class Solution {
public:
    int hammingDistance(int x, int y) {
        int ans = 0, z = x ^ y;
        while (z) {
            z &= z - 1;
            ++ans;
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.9 MB, 在所有 C++ 提交中击败了5.79% 的用户
```
或者查表：
```cpp
int n1[256] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};
class Solution {
public:
    int hammingDistance(int x, int y) {
        int z = x ^ y; 
        return n1[z & 0xff] + n1[(z >> 8) & 0xff] + 
        	   n1[(z >> 16) & 0xff] + n1[(z >> 24) & 0xff];
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.8 MB, 在所有 C++ 提交中击败了60.38% 的用户
```
