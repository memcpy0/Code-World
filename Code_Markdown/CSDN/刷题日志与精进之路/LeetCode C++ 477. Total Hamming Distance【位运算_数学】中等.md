
<div class="notranslate"><p>The <a href="https://en.wikipedia.org/wiki/Hamming_distance">Hamming distance</a> between two integers is the number of positions at which the corresponding bits are different.</p>

<p>Given an integer array <code>nums</code>, return <em>the sum of <strong>Hamming distances</strong> between all the pairs of the integers in</em> <code>nums</code>.</p>
 
<p><strong>Example 1:</strong></p>

```go
Input: nums = [4,14,2]
Output: 6
Explanation: In binary representation, the 4 is 0100, 14 is 1110, and 2 is 0010 (just
showing the four bits relevant in this case).
The answer will be:
HammingDistance(4, 14) + HammingDistance(4, 2) + HammingDistance(14, 2) = 2 + 2 + 2 = 6. 
```
<p><strong>Example 2:</strong></p>

```go
Input: nums = [4,14,4]
Output: 4
```

 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length &lt;= 10<sup>5</sup></code></li>
	<li><code>0 &lt;= nums[i] &lt;= 10<sup>9</sup></code></li>
</ul>
</div>


题意：两个整数的 **汉明距离** 指的是这两个数字的二进制数对应位不同的数量。计算一个数组中，任意两个数之间汉明距离的总和。


---
### 解法 数学/位运算
运用[461. Hamming Distance]()中写的求两个数之间汉明距离的函数：
```cpp
class Solution {
public:
    int hammingDistance(int x, int y) {
        int ans = 0;
        for (int i = 0; i < 32; ++i) 
            if (((x >> i) & 1) != ((y >> i) & 1)) ++ans;
        return ans;
    }
    int totalHammingDistance(vector<int>& nums) {
        int ans = 0;
        for (int i = 0, n = nums.size(); i < n; ++i) 
            for (int j = i + 1; j < n; ++j) 
                ans += hammingDistance(nums[i], nums[j]);
        return ans;
    }
};
```
上述代码超过时间限制。换一下汉明距离的函数：
```cpp
class Solution {
public:
    int hammingDistance(int x, int y) {
        int ans = 0, z = x ^ y;
        for (int i = 0; i < 32; ++i) if ((z >> i) & 1) ++ans; 
        return ans;
    }
    int totalHammingDistance(vector<int>& nums) {
        int ans = 0;
        for (int i = 0, n = nums.size(); i < n; ++i) 
            for (int j = i + 1; j < n; ++j) 
                ans += hammingDistance(nums[i], nums[j]);
        return ans;
    }
};
```
上述代码还是超过了时间限制，再换一个函数：
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
    int totalHammingDistance(vector<int>& nums) {
        int ans = 0;
        for (int i = 0, n = nums.size(); i < n; ++i) 
            for (int j = i + 1; j < n; ++j) 
                ans += hammingDistance(nums[i], nums[j]);
        return ans;
    }
};
```
上述代码继续超时，再换一个函数：
```cpp
int n1[256] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};
class Solution {
public:
    int hammingDistance(int x, int y) {
        int z = x ^ y; 
        return n1[z & 0xff] + n1[(z >> 8) & 0xff] + 
        	   n1[(z >> 16) & 0xff] + n1[(z >> 24) & 0xff];
    }
    int totalHammingDistance(vector<int>& nums) {
        int ans = 0;
        for (int i = 0, n = nums.size(); i < n; ++i) 
            for (int j = i + 1; j < n; ++j) 
                ans += hammingDistance(nums[i], nums[j]);
        return ans;
    }
};
```
继续超时……使用双重循环+汉明距离函数，$O(n^2)$ 的算法都过不了。不过通过巧妙的思索，可以将整个题的复杂度降低到 $O(n)$ 。

具体做法很简单，对每一个二进制位，统计整个数组 `nums[]` 中在这一位上为 `0` 和为 `1` 的数目 `n0, n1` ，可知整个数组 `nums[]` 形成的所有二元组 `(nums[i], nums[j]), i < j` 中，该位不同的二元组数目为 `n0 * n1` ，汉明距离总和就加上 `n0 * n1` 。
```cpp
class Solution {
public: 
    int totalHammingDistance(vector<int>& nums) {
        int ans = 0, n = nums.size();
        for (int i = 0; i < 32; ++i) {
            int n1 = 0;
            for (int j = 0; j < n; ++j) {
                n1 += (nums[j] & 1);
                nums[j] >>= 1;
            }
            ans += n1 * (n - n1);
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：60 ms, 在所有 C++ 提交中击败了40.29% 的用户
内存消耗：18.6 MB, 在所有 C++ 提交中击败了20.33% 的用户
```
