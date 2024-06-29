> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个按 **非递减顺序** 排列的整数数组 `nums1` 和 `nums2`，另有两个整数 `m` 和 `n` ，分别表示 `nums1` 和 `nums2` 中的元素数目。

请你 **合并** `nums2` 到 `nums1` 中，使合并后的数组同样按 **非递减顺序** 排列。

**注意：** 最终，合并后数组不应由函数返回，而是存储在数组 `nums1` 中。为了应对这种情况，`nums1` 的初始长度为 `m + n`，其中前 `m` 个元素表示应合并的元素，后 `n` 个元素为 `0` ，应忽略。`nums2` 的长度为 `n` 。

**示例 1：**
<pre><strong>输入：</strong>nums1 = [1,2,3,0,0,0], m = 3, nums2 = [2,5,6], n = 3
<strong>输出：</strong>[1,2,2,3,5,6]
<strong>解释：</strong>需要合并 [1,2,3] 和 [2,5,6] 。
合并结果是 [<em><strong>1</strong></em>,<em><strong>2</strong></em>,2,<em><strong>3</strong></em>,5,6] ，其中斜体加粗标注的为 nums1 中的元素。
</pre>

**示例 2：**
```java
输入：nums1 = [1], m = 1, nums2 = [], n = 0
输出：[1]
解释：需要合并 [1] 和 [] 。
合并结果是 [1] 。
```
**示例 3：**
```java
输入：nums1 = [0], m = 0, nums2 = [1], n = 1
输出：[1]
解释：需要合并的数组是 [] 和 [1] 。
合并结果是 [1] 。
注意，因为 m = 0 ，所以 nums1 中没有元素。nums1 中仅存的 0 仅仅是为了确保合并结果可以顺利存放到 nums1 中。
```
**提示：**
- `nums1.length == m + n`
- `nums2.length == n`
- `0 <= m, n <= 200`
- `1 <= m + n <= 200`
- `-10^9 <= nums1[i], nums2[j] <= 10^9`

**进阶：** 你可以设计实现一个时间复杂度为 `O(m + n)` 的算法解决此问题吗？

---
### 解法1 双指针
为了利用数组 $\textit{nums}_1$ 与 $\textit{nums}_2$ 已被排序的性质，可以使用双指针方法。这一方法将两个数组看作队列，每次从两个数组头部取出比较小的数字放到结果中。

为了避免覆盖，可以先将 $nums_1$ 的元素全部移动到后面去，将前面空出来。注意，==为了避免元素覆盖，需要从 $nums_1$ 的最后一个元素开始移动，将它移动到数组的最后==。

为两个数组分别设置一个指针 $p_1$​ 与 $p_2$ 来作为队列的头部指针。代码实现如下：
```cpp
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int all = n + m;
        for (int i = all - 1; i >= all - m; --i) nums1[i] = nums1[i - n];
        int p1 = n, p2 = 0, k = 0;
        while (p1 < all && p2 < n) {
            if (nums1[p1] < nums2[p2]) nums1[k++] = nums1[p1++];
            else nums1[k++] = nums2[p2++];
        }
        while (p1 < all) nums1[k++] = nums1[p1++];
        while (p2 < n) nums1[k++] = nums2[p2++];
    }
}
```
复杂度分析：
- 时间复杂度：$O(m+n)$ 。指针移动单调递增，最多移动 $m+n$ 次，因此时间复杂度为 $O(m+n)$ 。
- 空间复杂度：$O(1)$。

---
### 解法2 逆向双指针
方法1中，之所以要将 $nums_1$ 的元素移动到后面，是因为如果直接合并到数组 $\textit{nums}_1$ 中，$\textit{nums}_1$ 中的元素可能会在取出之前被覆盖。那么如何直接避免覆盖 $\textit{nums}_1$ 中的元素呢？观察可知，$\textit{nums}_1$ 的后半部分是空的，可以直接覆盖而不会影响结果。因此，**指针设置为从后向前遍历，每次取两者之中的较大者放进 $\textit{nums}_1$ 的最后面**。

严格来说，在此遍历过程中的任意一个时刻，$\textit{nums}_1$ 数组中有 $m-p_1-1$ 个元素被放入 $\textit{nums}_1$ 的后半部，$\textit{nums}_2$ 数组中有 $n-p_2-1$ 个元素被放入 $\textit{nums}_1$ 的后半部，而在指针 $p_1$ 的后面，$\textit{nums}_1$ 数组有 $m+n-p_1-1$​ 个位置。由于
$$m+n-p_1-1\geq m-p_1-1+n-p_2-1$$
等价于
$$p_2\geq -1$$
​
永远成立，因此 **$p_1$ 后面的位置永远足够容纳被插入的元素**，不会产生 $p_1$​ 的元素被覆盖的情况。

当然，不会覆盖的原因，一句话就能说明白：把 $\textit{nums}_1$ 的数字**移到另一个空位，又产生了一个新的空位，空位个数不变**，所以总是有空位可以让 $\textit{nums}_2$​ 的数字填入。

实现代码如下：
```cpp
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int p1 = m - 1, p2 = n - 1, tail = m + n - 1;
        int cur;
        while (p1 >= 0 || p2 >= 0) {
            if (p1 == -1) cur = nums2[p2--];
            else if (p2 == -1) cur = nums1[p1--];
            else if (nums1[p1] > nums2[p2]) cur = nums1[p1--];
            else cur = nums2[p2--];
            nums1[tail--] = cur;
        }
    }
};
```
复杂度分析：
- 时间复杂度：$O(m+n)$。 指针移动单调递减，最多移动 $m+n$ 次，因此时间复杂度为 $O(m+n)$ 。
- 空间复杂度：$O(1)$ 。 直接对数组 $\textit{nums}_1$ 原地修改，不需要额外空间。

 