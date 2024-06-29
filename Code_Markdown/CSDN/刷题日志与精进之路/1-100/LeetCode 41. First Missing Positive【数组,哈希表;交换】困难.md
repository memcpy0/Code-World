> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091401433.png)
Given an **unsorted** integer array, find the **smallest missing positive integer**.

Example 1:

```cpp
Input: [1,2,0]
Output: 3
```

Example 2:
```cpp
Input: [3,4,-1,1]
Output: 2
```

Example 3:
```cpp
Input: [7,8,9,11,12]
Output: 1
```
**Note:** Your algorithm should run in `O(n)` time and uses **constant extra space**.

题意：找到一个无序的数组中不存在的最小的正整数。

---
相似题目：
[丢失的数字](https://leetcode.cn/problems/missing-number/)

简单

[寻找重复数](https://leetcode.cn/problems/find-the-duplicate-number/)

中等

[找到所有数组中消失的数字](https://leetcode.cn/problems/find-all-numbers-disappeared-in-an-array/)

简单

[情侣牵手](https://leetcode.cn/problems/couples-holding-hands/)

困难

思路1：$\text{O(nlogn)}$ 和 $\text{O(n)}$ 的差别比较小，可以用下面的方法蒙混过关。先排序，后去重，然后从小往大搜索，忽视非正数。

代码1：$\text{O(nlogn)}$ 时间和常量空间。
```cpp
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int len = unique(nums.begin(), nums.end()) - nums.begin(), p = 1;
        for (int i = 0; i < len; ++i) {
            if (nums[i] <= 0) continue;
            else if (nums[i] == p) ++p;
            else break;
        }
        return p;
    }
};
```
---
思路2：如果用额外的空间做的话，可以用整数哈希表，从 `1` 开始，能过的话，是 $\text{O(n)}$ 时间。但是下面的——发现不可能开这样大的空间，即使用 `bitset` ，比如 `[2147483647]` 。
```cpp
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        bitset<20000> bst;
        for (const auto &i : nums) 
            if (i > 0) 
                bst.set(i);
        int i = 1;
        while (bst[i]) ++i;
        return i;
    }
};
```
不过这里其实是一时糊涂，除了 `<=0` 的数外，完全可以拒绝大于数组长度的数，然后将其他的数所指向的位置的 `bitset` 置 `1` 。最后检查即可。

代码2如下，在上面的代码中做了一点小修改：
```cpp
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        bitset<10000> bst;
        for (const auto &i : nums) 
            if (i > 0 && i <= nums.size()) 
                bst.set(i);
        int i = 1;
        while (bst[i]) ++i;
        return i;
    }
};
```

还有能过的是使用 `unordered_set` ，**此时不用在乎数据范围**。代码如下：
```cpp
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        unordered_set<int> st;
        for (const auto &i : nums) st.insert(i);
        int i = 1;
        while (st.find(i) != st.end()) ++i;
        return i;
    }
};
```
效率：
```
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.9 MB, 在所有 C++ 提交中击败了100.00% 的用户
```

---
思路3：既然题目说可以在 $\text{O(n)}$ 和不使用额外空间的情况下得到答案，那么只有一种可能，即**使用原数组**。我们忽视负数、`0` 、超出数组长度范围的数(说明它们不是答案)，则把另外的数，它们指向的数组位置做标记——不可能取负(以前有的题目可以，但是这里不可以)或置零——排除这些途径，**唯一的方法就是把这些数交换回原来的位置上**。

交换的方法也有一定的技巧：
- 从头检查起，如果发现一个数 `> 0 && <= nums.size() && nums[i] != nums[nums[i] - 1]` ，就将其与 `nums[nums[i] - 1]` (**为了容纳等于数组长度的那个正数**) 交换；此时仍指向 `i` 的位置，继续对这个值实施上面的过程；直到现在的 `nums[i]` 已经处于正确位置或属于逃票——票号 `<= 0`，或 `>=` 最大座位号，则终止交换；
- 然后对 `i+1,i+2` 位置的元素做同样的操作。

然后，我们从 `1` 位置开始检查，**发现值和位置+1不匹配，就返回这个位置的下标+1**；如果都匹配，则返回数组长度+1。

其实，这种做法看提示应该能够想得出来。
> Think about how you would solve the problem in non-constant space. Can you apply that logic to the existing space?
> We don't care about duplicates or non-positive integers.
> Remember that O(2n) = O(n).


 代码3：
```cpp
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        int n = nums.size();
        for (int i = 0; i < n; ++i) 
            while (nums[i] > 0 && nums[i] <= n && nums[i] != nums[nums[i] - 1]) 
                swap(nums[i], nums[nums[i] - 1]);
        for (int i = 0; i < n; ++i) 
            if (nums[i] != i + 1) 
                return i + 1;
        return n + 1;
    }
};
```
效率：
```
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
