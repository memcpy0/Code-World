> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


Given an array of positive integers `arr` , calculate the sum of all possible odd-length subarrays.

A subarray is a contiguous subsequence of the array.

Return **the sum of all odd-length subarrays** of `arr` .

**Example 1:**

```cpp
Input: arr = [1,4,2,5,3]
Output: 58
Explanation: The odd-length subarrays of arr and their sums are:
[1] = 1
[4] = 4
[2] = 2
[5] = 5
[3] = 3
[1,4,2] = 7
[4,2,5] = 11
[2,5,3] = 10
[1,4,2,5,3] = 15
If we add all these together we get 1 + 4 + 2 + 5 + 3 + 7 + 11 + 10 + 15 = 58
```

**Example 2:**

```cpp
Input: arr = [1,2]
Output: 3
Explanation: There are only 2 subarrays of odd length, [1] and [2]. Their sum is 3.
```

**Example 3:**
```cpp
Input: arr = [10,11,12]
Output: 66
```

 

**Constraints:**
- `1 <= arr.length <= 100`
- `1 <= arr[i] <= 1000`

 
题意：统计正整数数组中，所有长度为奇数的子数组的和。

---
### 解法1 暴力
暴力做法是，枚举每个子区间的左右端点 `[i, j]` ，如果长度 `j - i + 1` 为奇数，则又用一次循环计算它的元素和。或者稍微优化一下，**枚举左端点和计算右端点（使其长度为奇数），从而去掉了一个条件判断**。这样时间复杂度就来到了 $O(n^3)$ ，空间复杂度为 $O(1)$ ，不过考虑到数据范围只有 $100$ ，也是能够通过的：
```cpp
//C++ version
class Solution {
public:
    int sumOddLengthSubarrays(vector<int>& arr) {
        if (arr.size() == 1) return arr[0];
        int ans = 0, n = arr.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; j += 2) { //遍历奇数长度
                int sum = 0;
                for (int k = i; k <= j; ++k) sum += arr[k];
                ans += sum;
            }
        }
        return ans;
    }   
};
//执行用时：8 ms, 在所有 C++ 提交中击败了24.31% 的用户
//内存消耗：8.1 MB, 在所有 C++ 提交中击败了51.04% 的用户
```


---
### 解法2 前缀和
第二重循环沿用了上面的优化，再用前缀和优化掉内部循环的**区间求和**。时间复杂度为 $O(n^2)$ ，空间复杂度为 $O(n)$ ：
```cpp
//C++ version
class Solution {
public:
    int sumOddLengthSubarrays(vector<int>& arr) {
        if (arr.size() == 1) return arr[0];
        int ans = 0, n = arr.size();
        vector<int> presum(n + 1);
        for (int i = 0; i < n; ++i) presum[i + 1] = presum[i] + arr[i];
        for (int i = 0; i < n; ++i)
            for (int j = i; j < n; j += 2) //遍历奇数长度
                ans += presum[j + 1] - presum[i];
        return ans;
    }   
};
//执行用时：4 ms, 在所有 C++ 提交中击败了74.42% 的用户
//内存消耗：8.3 MB, 在所有 C++ 提交中击败了10.95% 的用户
```
---
### 解法3 模拟
其实完全可以不用前缀和，直接模拟就可以了，**同样的枚举左端点和计算右端点（使其长度为奇数）**，注意**每次要加上前两个元素**。时间复杂度为 $O(n^2)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int sumOddLengthSubarrays(vector<int>& arr) {
        if (arr.size() == 1) return arr[0];
        int ans = 0, n = arr.size();  
        for (int i = 0; i < n; ++i) {
            int sum = arr[i];
            ans += sum;
            for (int j = i + 2; j < n; j += 2) { //遍历奇数长度
                sum += arr[j - 1] + arr[j];
                ans += sum;
            }
        }
        return ans;
    }   
};
//执行用时：4 ms, 在所有 C++ 提交中击败了74.42% 的用户
//内存消耗：8 MB, 在所有 C++ 提交中击败了90.09% 的用户
```
---
### 解法4 数学
事实上，通过运用数学解法，我们可以得到时间复杂度为 $O(n)$ 、空间复杂度为 $O(1)$ 的解法。原理很简单，我们要做的就是**计算每个元素在所有奇数子数组中出现的次数，然后加总得到结果**。所以，又是一道组合数学的题目。

具体来说，对数组 `arr` 的任意元素 `arr[i]` 来说，其左边有 $i$ 个数，右边有 $n - i - 1$ 个数。从而很容易想到，**`arr[i]` 出现在某个奇数子数组中当且仅当其所在奇数子数组的左右两边元素个数的奇偶性相同**。即，`arr[i]` 左边的元素个数和右边的元素个数，要么都为偶数个、要么都为奇数个，这样才能保证**结果子数组长度为奇数**。

于是问题转换为，如何求 `arr[i]` 在原数组左边（以及右边）连续一段元素为奇数个的方案数，以及 `arr[i]` 在原数组右边（以及左边）连续一段元素为偶数个的方案数：
- 位置 `i` 左边连续一段元素为奇数个数的方案数为 $(i + 1) / 2$ ，右边连续一段元素为奇数个数的方案数为 $(n - i) / 2$ 。一个例子是 `[1, 4, 2, 5, 3]` ，对于 `i = 2` 来说，左边连续一段元素为奇数个只有 `1` 个方案，对于 `i = 3` 来说则有 `2` 个方案；
- 位置 `i` 左边连续一段元素为偶数个数的方案数为 $i / 2$ ，右边连续一段元素为偶数个数的方案数为 $(n - i - 1) / 2$ 。一个例子是 `[1, 4, 2, 5, 3]` ，对于 `i = 2` 来说，左边连续一段元素为偶数个只有 `1` 个方案，对于 `i = 3` 来说也只有 `1` 个方案？是这样吗？
考虑到左右两边不进行选择也是合法的偶数个数，因此左边、右边连续一段元素为偶数个数的方案数，分别还要加上 $1$ 。


现在我们各自得到了 `arr[i]` 左右两边连续一段元素分别为奇数个、偶数个的方案数，根据 乘法原理，就能知道 `arr[i]` 在所有奇数子数组中出现了多少次，次数乘以 `arr[i]` 就是对答案 `ans` 的贡献。
```cpp
//C++ version
class Solution {
public:
    int sumOddLengthSubarrays(vector<int>& arr) {
        int ans = 0;  
        for (int i = 0, n = arr.size(); i < n; ++i) {
            int l1 = (i + 1) / 2, r1 = (n - i) / 2; //奇数个数
            int l2 = i / 2 + 1, r2 = (n - i - 1) / 2 + 1; //偶数个数
            ans += arr[i] * (l1 * r1 + l2 * r2);
        }
        return ans;
    }   
};
//执行用时：4 ms, 在所有 C++ 提交中击败了74.42% 的用户
//内存消耗：8.1 MB, 在所有 C++ 提交中击败了51.04% 的用户
```
