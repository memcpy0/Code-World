> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

 <p>Given an array <code>nums</code> containing <code>n</code> distinct numbers in the range <code>[0, n]</code>, return <em>the only number in the range that is missing from the array.</em></p>
 
<p><strong>Example 1:</strong></p>

```cpp
Input: nums = [3,0,1]
Output: 2
Explanation: n = 3 since there are 3 numbers, so all numbers are in the range [0,3]. 2 is the missing number in the range since it does not appear in nums. 
```

<p><strong>Example 2:</strong></p>

```cpp
Input: nums = [0,1]
Output: 2
Explanation: n = 2 since there are 2 numbers, so all numbers are in the range [0,2]. 2 is the missing number in the range since it does not appear in nums. 
```

<p><strong>Example 3:</strong></p>

```cpp
Input: nums = [9,6,4,2,3,5,7,0,1]
Output: 8
Explanation: n = 9 since there are 9 numbers, so all numbers are in the range [0,9]. 8 is the missing number in the range since it does not appear in nums.
```

<p><strong>Example 4:</strong></p>

```cpp
Input: nums = [0]
Output: 1
Explanation: n = 1 since there is 1 number, so all numbers are in the range [0,1]. 1 is the missing number in the range since it does not appear in nums.
```

 

 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>n == nums.length</code></li>
	<li><code>1 &lt;= n &lt;= 10<sup>4</sup></code></li>
	<li><code>0 &lt;= nums[i] &lt;= n</code></li>
	<li>All the numbers of <code>nums</code> are <strong>unique</strong>.</li>
</ul>
 
<p><strong>Follow up:</strong> Could you implement a solution using only <code>O(1)</code> extra space complexity and <code>O(n)</code> runtime complexity?</p>

题意：给定一个包含 `[0, n]` 中 `n` 个数的数组 `nums` ，找出 `[0, n]` 这个范围内没有出现在数组中的那个数。

---
本题是[136. Single Number](https://leetcode-cn.com/problems/single-number/)的又一个变形，因此沿用[136. Single Number【哈希表/位运算/数学】简单](https://memcpy0.blog.csdn.net/article/details/106988806)的做法即是正解。一种不考虑的方法是「排序然后从头遍历」。

### 解法1 哈希表（统计次数/标记出现/原地数组哈希）
使用哈希表或整型数组，统计 `nums[]` 中每个元素的出现次数（最多为一次）。如果出现次数为零，则为答案。这一做法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(n)$ 。
```java
//Java version
class Solution {
    public int missingNumber(int[] nums) {
        int n = nums.length; // n个数
        int[] cnt = new int[n + 1];
        for (int v : nums) ++cnt[v];
        for (int i = 0; i <= n; ++i)
            if (cnt[i] == 0) return i;
        return -1;
    }
}
//执行用时：0 ms, 在所有 Java 提交中击败了100.00% 的用户
//内存消耗：38.7 MB, 在所有 Java 提交中击败了67.63% 的用户
```
或者利用**只有一个值缺失、其他值都只出现一次**的特性，直接开一个 $n + 1$ 的布尔数组（更好的是位图，如Java的 `BitSet` ）充当哈希表，数字出现就标记对应位置，没被标记的数字就是答案。这一做法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(n)$ 。
```java
//Java version
class Solution {
    public int missingNumber(int[] nums) {
        int n = nums.length; // n个数
        BitSet vis = new BitSet(n + 1);
        for (int v : nums) vis.set(v);
        for (int i = 0; i <= n; ++i)
            if (vis.get(i) == false) return i;
        return -1;
    }
}
//执行用时：1 ms, 在所有 Java 提交中击败了41.56% 的用户
//内存消耗：38.7 MB, 在所有 Java 提交中击败了68.41% 的用户
```
或者将 `nums[]` 本身当做哈希表使用——先将 `nums[i]` 都放到应该出现的位置 `i` 上，然后检查 `nums[]` ，找到满足 `nums[i]` 不等于下标 `i` 的位置，即是答案。如果不存在这一位置，则 `n` 为答案。这一做法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ 。
```java
//Java version
class Solution {
    private void swap(int[] nums, int i, int j) {
        int c = nums[i];
        nums[i] = nums[j];
        nums[j] = c; 
    }
    public int missingNumber(int[] nums) {
        int n = nums.length; // n个数
        for (int i = 0; i < n; ++i) 
            if (nums[i] != i && nums[i] < n) 
                swap(nums, nums[i], i--); // 可能换过来一个新数,i先不动
        for (int i = 0; i < n; ++i)
            if (nums[i] != i) return i;
        return n;
    }
}
//执行用时：1 ms, 在所有 Java 提交中击败了41.56% 的用户
//内存消耗：38.5 MB, 在所有 Java 提交中击败了89.28% 的用户
```

---
### 解法2 数学
能想到的另一种简单方法是**求等差数列和**，然后和 `nums[]` 数组和对比，相差的数就是缺失的值。这一做法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ 。
```cpp
//C++ version
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int sum = 0, n = nums.size();
        for (int i = 0; i < nums.size(); ++i) 
            sum += nums[i]; 
        return n * (n + 1) / 2 - sum;
    }
};
//执行用时：20 ms, 在所有 C++ 提交中击败了51.48% 的用户
//内存消耗：17.6 MB, 在所有 C++ 提交中击败了40.21% 的用户
```
更好的写法是边加边减：
```cpp
//C++ version
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int sum = 0, n = nums.size();
        for (int i = 0; i < n; ++i) 
            sum += (i - nums[i]);
        return sum + n;
    }
};
//执行用时：16 ms, 在所有 C++ 提交中击败了76.62% 的用户
//内存消耗：17.5 MB, 在所有 C++ 提交中击败了82.90% 的用户
```

---
### 解法3 位运算（最优）
以前做过[136. Single Number](https://leetcode-cn.com/problems/single-number/)只出现一次的数字，这里可以借鉴它的做法。不难发现，只要我们让 `[0, n]` 的每个整数再出现一次，即可让 `[0, n]` 中仅有一个整数只出现一次，其他整数都出现两次，从而等价于136题。

对本题而言，这是最好的方法，时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ 。
```cpp
//C++ version
class Solution {
public:
	int missingNumber(vector<int>& nums) {
		int n = nums.size(), ans = n;
        for (int i = 0; i < n; ++i)
            ans ^= (nums[i] ^ i);
		return ans;
	}
};
//执行用时：16 ms, 在所有 C++ 提交中击败了76.62% 的用户
//内存消耗：17.5 MB, 在所有 C++ 提交中击败了76.36% 的用户
```
