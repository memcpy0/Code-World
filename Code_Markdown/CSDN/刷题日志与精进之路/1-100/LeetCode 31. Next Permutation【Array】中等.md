> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091352457.png)
整数数组的一个 **排列**  就是将其所有成员以序列或线性顺序排列。
- 例如，`arr = [1,2,3]` ，以下这些都可以视作 `arr` 的排列：`[1,2,3]`、`[1,3,2]`、`[3,1,2]`、`[2,3,1]` 。

整数数组的 **下一个排列** 是指其整数的下一个字典序更大的排列。更正式地，如果数组的所有排列根据其字典顺序从小到大排列在一个容器中，那么数组的 **下一个排列** 就是在这个有序容器中排在它后面的那个排列。如果不存在下一个更大的排列，那么这个数组必须重排为字典序最小的排列（即，其元素按升序排列）。
- 例如，`arr = [1,2,3]` 的下一个排列是 `[1,3,2]` 。
- 类似地，`arr = [2,3,1]` 的下一个排列是 `[3,1,2]` 。
- 而 `arr = [3,2,1]` 的下一个排列是 `[1,2,3]` ，因为 `[3,2,1]` 不存在一个字典序更大的排列。

给你一个整数数组 `nums` ，找出 `nums` 的下一个排列。

必须 **[原地](https://baike.baidu.com/item/%E5%8E%9F%E5%9C%B0%E7%AE%97%E6%B3%95)** 修改，只允许使用额外常数空间。

**示例 1：**
```java
输入：nums = [1,2,3]
输出：[1,3,2]
```
**示例 2：**
```java
输入：nums = [3,2,1]
输出：[1,2,3]
```
**示例 3：**
```java
输入：nums = [1,1,5]
输出：[1,5,1]
```
**提示：**
- `1 <= nums.length <= 100`
- `0 <= nums[i] <= 100`

---
### 解法1 `next_permutation` 
```cpp
class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        next_permutation(nums.begin(), nums.end());
    }
};
```
提交后效果如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了93.99% 的用户
内存消耗：12.1 MB, 在所有 C++ 提交中击败了31.11% 的用户
```

---
### 解法2 规律 
前面知道了如何实现这个算法，所以自己手写一下就可以了。
```cpp
class Solution {
public:
    void nextPermutation(vector<int>& nums) { 
        if (nums.empty() || nums.size() == 1) return;
        int cur = nums.size() - 1;
        while (cur > 0 && nums[cur] <= nums[cur - 1]) --cur;
        if (cur == 0) reverse(nums.begin(), nums.end()); //全部逆转为最小序
        else { 
            int t = cur;
            reverse(nums.begin() + cur, nums.end());
            while (t < nums.size() && nums[t] <= nums[cur - 1]) ++t;
            swap(nums[cur - 1], nums[t]);
        }
    }
};
```
上面的代码写得有点怪，可以看之前的实现：[STL next_permutation和prev_permutation 算法原理和自行实现](https://blog.csdn.net/myRealization/article/details/104803834) 。
 
 **2020/11/10 Update** 今日更新代码：
```cpp
class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        if (nums.size() <= 1) return;
        int cur = nums.size() - 1, pre = cur - 1;
        while (cur > 0 && nums[cur] <= nums[pre]) --cur, --pre; //pre指向partitionNumber
        if (cur == 0) { reverse(nums.begin(), nums.end()); return; } //最大排列,逆序得到最小排列
        for (cur = nums.size() - 1; nums[cur] <= nums[pre]; --cur); //cur指向changeNumber
        swap(nums[pre], nums[cur]);
        reverse(nums.begin() + pre + 1, nums.end());
    }
};
```
效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了65.28% 的用户
内存消耗：12.2 MB, 在所有 C++ 提交中击败了9.25% 的用户
```

```cpp
class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        if (nums.size() <= 1) return;
        int n = nums.size(), cur = n - 1, pre = cur - 1;
        while (cur > 0 && nums[cur] <= nums[pre]) --cur, --pre;
        if (cur == 0) { reverse(nums.begin(), nums.end()); return; }
        for (cur = n - 1; nums[cur] <= nums[pre]; --cur);
        swap(nums[pre], nums[cur]);
        reverse(nums.begin() + pre + 1, nums.end());
    }
};
```

本题要求我们实现一个算法，将给定数字序列重新排列成字典序中下一个更大的排列。

以数字序列 [1,2,3][1,2,3][1,2,3] 为例，其排列按照字典序依次为：

[1,2,3]\\ [1,3,2]\\ [2,1,3]\\ [2,3,1]\\ [3,1,2]\\ [3,2,1]
这样，排列 [2,3,1][2,3,1][2,3,1] 的下一个排列即为 [3,1,2][3,1,2][3,1,2]。特别的，最大的排列 [3,2,1][3,2,1][3,2,1] 的下一个排列为最小的排列 [1,2,3][1,2,3][1,2,3]。

方法一：两遍扫描
思路及解法

注意到下一个排列总是比当前排列要大，除非该排列已经是最大的排列。我们希望找到一种方法，能够找到一个大于当前序列的新序列，且变大的幅度尽可能小。具体地：

我们需要将一个左边的「较小数」与一个右边的「较大数」交换，以能够让当前排列变大，从而得到下一个排列。

同时我们要让这个「较小数」尽量靠右，而「较大数」尽可能小。当交换完成后，「较大数」右边的数需要按照升序重新排列。这样可以在保证新排列大于原来排列的情况下，使变大的幅度尽可能小。

以排列 [4,5,2,6,3,1][4,5,2,6,3,1][4,5,2,6,3,1] 为例：

我们能找到的符合条件的一对「较小数」与「较大数」的组合为 222 与 333，满足「较小数」尽量靠右，而「较大数」尽可能小。

当我们完成交换后排列变为 [4,5,3,6,2,1][4,5,3,6,2,1][4,5,3,6,2,1]，此时我们可以重排「较小数」右边的序列，序列变为 [4,5,3,1,2,6][4,5,3,1,2,6][4,5,3,1,2,6]。

具体地，我们这样描述该算法，对于长度为 nnn 的排列 aaa：

首先从后向前查找第一个顺序对 (i,i+1)(i,i+1)(i,i+1)，满足 a[i]<a[i+1]a[i] < a[i+1]a[i]<a[i+1]。这样「较小数」即为 a[i]a[i]a[i]。此时 [i+1,n)[i+1,n)[i+1,n) 必然是下降序列。

如果找到了顺序对，那么在区间 [i+1,n)[i+1,n)[i+1,n) 中从后向前查找第一个元素 jjj 满足 a[i]<a[j]a[i] < a[j]a[i]<a[j]。这样「较大数」即为 a[j]a[j]a[j]。

交换 a[i]a[i]a[i] 与 a[j]a[j]a[j]，此时可以证明区间 [i+1,n)[i+1,n)[i+1,n) 必为降序。我们可以直接使用双指针反转区间 [i+1,n)[i+1,n)[i+1,n) 使其变为升序，而无需对该区间进行排序。



注意

如果在步骤 1 找不到顺序对，说明当前序列已经是一个降序序列，即最大的序列，我们直接跳过步骤 2 执行步骤 3，即可得到最小的升序序列。

该方法支持序列中存在重复元素，且在 C++ 的标准库函数 next_permutation 中被采用。

 
class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        int i = nums.size() - 2;
        while (i >= 0 && nums[i] >= nums[i + 1]) {
            i--;
        }
        if (i >= 0) {
            int j = nums.size() - 1;
            while (j >= 0 && nums[i] >= nums[j]) {
                j--;
            }
            swap(nums[i], nums[j]);
        }
        reverse(nums.begin() + i + 1, nums.end());
    }
};
复杂度分析

时间复杂度：O(N)O(N)O(N)，其中 NNN 为给定序列的长度。我们至多只需要扫描两次序列，以及进行一次反转操作。

空间复杂度：O(1)O(1)O(1)，只需要常数的空间存放若干变量。

作者：力扣官方题解
链接：https://leetcode.cn/problems/next-permutation/solutions/479151/xia-yi-ge-pai-lie-by-leetcode-solution/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。