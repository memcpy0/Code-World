> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an array of integers `nums` containing `n + 1` integers where each integer is in the range `[1, n]` inclusive.

There is only one duplicate number in nums, return this duplicate number.

**Follow-ups:**
- How can we prove that at least one duplicate number must exist in nums? 
- Can you solve the problem without modifying the array nums?
- Can you solve the problem using only constant, $O(1)$ extra space?
- Can you solve the problem with runtime complexity less than $O(n^2)$?

 

**Example 1:**

```swift
Input: nums = [1,3,4,2,2]
Output: 2
```

**Example 2:**

```swift
Input: nums = [3,1,3,4,2]
Output: 3
```

**Example 3:**

```swift
Input: nums = [1,1]
Output: 1
```

**Example 4:**

```swift
Input: nums = [1,1,2]
Output: 1
```

 

**Constraints:**
- <code>2 <= n <= 3 * 10<sup>4</sup>
-  `nums.length == n + 1`
-   `1 <= nums[i] <= n`
- All the integers in `nums` appear only **once** except for **precisely one integer** which appears **two or more times**.

题意：给定一个包含 `n + 1` 个整数的数组 `nums`，其数字都在 `1` 到 `n` 之间（包括 `1` 和 `n`），可知至少存在一个重复的整数。假设**只有一个重复的整数**，找出这个重复的数。

注意：不能更改原数组。只能使用额外的 $O(1)$ 的空间，时间复杂度小于 $O(n^2)$ 。数组中只有一个重复的数字，但它可能不止重复出现一次。


---
> 证明在数组 `nums` 中必然存在至少一个重复元素……很简单，不过是鸽巢定理的逆向罢了——如果要把 `n + 1` 个物体放进 `n` 个盒子，那么至少有一个盒子包含两个或更多个物体；如果把 `n` 个物体放进 `n + 1` 个盒子，允许重复装入某个物体，那么至少有两个盒子包含同样的物体。

### 解法1 辅助空间（哈希表）
使用哈希表记录每个整数的出现次数，如果发现某个元素出现次数 `> 1` ，就直接返回：
```cpp
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        unordered_map<int, int> rec;
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            ++rec[nums[i]];
            if (rec[nums[i]] > 1) return nums[i];
        } 
        return 0;
    }
};
```
或者改成哈希集合：
```cpp
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        unordered_set<int> rec;
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            if (rec.find(nums[i]) != rec.end()) return nums[i];
            rec.insert(nums[i]);
        } 
        return 0;
    }
};
```
或者改成位图：
```cpp
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        bitset<30010> bst;
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            if (bst[nums[i]]) return nums[i];
            bst[nums[i]] = 1;
        } 
        return 0;
    }
};
```
使用位图的效率更高一些：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了60.79% 的用户
内存消耗：11.2 MB, 在所有 C++ 提交中击败了15.47% 的用户
```
---
### 解法2 快慢指针
这道题的 $O(1)$ 空间、$O(n)$ 时间、不修改数组的解法，我还是看了评论区里的大佬讲解才做出来的。快慢指针，一个时间复杂度为 $O(n)$ 的算法——对于链表问题，快慢指针可以判断环的存在与入环的位置。**难点是如何将数组 `nums[]` 配合其下标抽象为链表问题。**

一个例子是：`nums = [1,3,4,2,2]` ，构造为链表就是：`0(索引0) -> 1(nums[0]) -> 3(nums[1]) -> 2(nums[3]) -> 4(nums[2]) -> 2(nums[4])` 。显而易见，重复值 `2` 会导致环的出现，它也是入环的第一个结点。我们可以利用快慢指针找到环的起始位置：
- 快慢指针都初始化为 `0` ；
- 快指针 `hi` 每次走两步，慢指针每次走 `1` 步，直到两个指针相遇；
- 接着使用新的一个指针 `p` ，`p` 开始为 `0` ，接着 `p` 和 `slow` 同步前行，直到两个指针相遇，相遇点即为入环的第一个结点，也是我们想要找的重复数字。

```cpp
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int lo = 0, hi = 0;
        do { //一定有环
            lo = nums[lo]; //lo = lo->next;
            hi = nums[nums[hi]]; //hi = hi->next->next;
        } while (lo != hi);
        int p = 0;
        while (p != lo) { //寻找入环的第一个结点
            p = nums[p];
            lo = nums[lo];
        }
        return lo;
    }
};
```
执行效率如下：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了86.73% 的用户
内存消耗：11.1 MB, 在所有 C++ 提交中击败了34.57% 的用户
```

