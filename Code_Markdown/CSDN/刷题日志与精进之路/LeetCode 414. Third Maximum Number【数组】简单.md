> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个非空数组，返回此数组中 **第三大的数** 。如果不存在，则返回数组中最大的数。

**示例 1：**
```js
输入：[3, 2, 1]
输出：1
解释：第三大的数是 1 。
```
**示例 2：**
```js
输入：[1, 2]
输出：2
解释：第三大的数不存在, 所以返回最大的数 2 。
```
**示例 3：**
```js
输入：[2, 2, 3, 1]
输出：1
解释：注意，要求返回第三大的数，是指在所有不同数字中排第三大的数。
此例中存在两个值为 2 的数，它们都排第二。在所有不同数字中排第三大的数为 1 。
```
**提示：**
- `1 <= nums.length <= 10^4`
- `-2^31 <= nums[i] <= 2^31 - 1`

**进阶：** 你能设计一个时间复杂度 `O(n)` 的解决方案吗？

---
### 解法 遍历
思路：先去重复，再排序的做法/用堆的方法都是 $n\log n$ 级别的，因此不考虑。下面是我第一次做的方法。先循环找出第一大(最大)的数，再找出第二大的数，再循环找出第三大的数，$O(3n)$ 的复杂度。
```cpp
class Solution {
public:
    int thirdMax(vector<int>& nums) {
        long firMax = LONG_MIN, secMax = LONG_MIN, thiMax = LONG_MIN;
        for (int i = 0; i < nums.size(); ++i) 
            if (nums[i] > firMax) 
                firMax = nums[i]; 
        for (int i = 0; i < nums.size(); ++i) 
            if (nums[i] > secMax && nums[i] < firMax) 
                secMax = nums[i];
        for (int i = 0; i < nums.size(); ++i) 
            if (nums[i] > thiMax && nums[i] < secMax) 
                thiMax = nums[i]; 
        if (thiMax == LONG_MIN) return firMax;
        return thiMax;
    }
};
```
然后其实，可以合成一个循环。像是冒泡或者是单调队列，用 $a, b,c$ 分别表示最大值，次大值和第三大的数。
-    如果当前元素比 $a$ 大，则说明其一定比 $b$ 和 $c$ 都大。 我们同时更新 $b$ 和 $c$ 的值。 具体来说就是将 $b$ 更新到 $c$ ，$a$ 更新到 $b$ （可以形象地考虑成是把元素往后挤出去）。
 -   否则我们继续判断是否比 $b$ 大，如果比 $b$ 大，那么肯定也比 $c$ 大，我们同时需要更新 $c$ 的值。
 -  如果都不比 $a$ 和 $b$ 大，我们继续判断是否比 $c$ 大，如果是，我们更新c的值。

我们初始化 $a,b,c$ 为 负无穷（`LONG_MIN`）。 这样我们最后只要判断 $c$ 是不是负无穷即可，如果是负无穷我们返回 $a$ ，否则我们返回 $c$ 。
```cpp
class Solution {
public:
    int thirdMax(vector<int>& nums) { 
        long a = LONG_MIN, b = LONG_MIN, c = LONG_MIN; 
        for (int &num : nums) {
            if (num > a) {
                c = b; b = a; a = num;
            } else if (num > b && num < a) {
                c = b; b = num;
            } else if (num > c && num < b) {
                c = num;
            }
        }
        return (c == LONG_MIN) ? a : c;
    }
};
```
另一种不依赖元素范围的做法是，将 $a$ 、$b$ 和 $c$ 初始化为空指针或空对象，视作「无穷小」，并**在比较大小前先判断是否为空指针或空对象**。遍历结束后，若 $c$ 为空，则说明第三大的数不存在，返回 $a$ ，否则返回 $c$ 。
```cpp
class Solution {
public:
    int thirdMax(vector<int> &nums) {
        int *a = nullptr, *b = nullptr, *c = nullptr;
        for (int &num : nums) {
            if (a == nullptr || num > *a) {
                c = b;
                b = a;
                a = &num;
            } else if (*a > num && (b == nullptr || num > *b)) {
                c = b;
                b = &num;
            } else if (b != nullptr && *b > num && (c == nullptr || num > *c)) {
                c = &num;
            }
        }
        return c == nullptr ? *a : *c;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是数组 $\textit{nums}$ 的长度。
- 空间复杂度：$O(1)$ 。

 
