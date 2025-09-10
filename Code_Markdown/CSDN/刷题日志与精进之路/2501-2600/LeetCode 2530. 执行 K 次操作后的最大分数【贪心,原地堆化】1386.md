> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的整数数组 `nums` 和一个整数 `k` 。你的 **起始分数** 为 `0` 。

在一步 **操作** 中：
1. 选出一个满足 `0 <= i < nums.length` 的下标 `i` ，
2. 将你的 **分数** 增加 `nums[i]` ，并且
3. 将 `nums[i]` 替换为 `ceil(nums[i] / 3)` 。

返回在 **恰好** 执行 `k` 次操作后，你可能获得的最大分数。

向上取整函数 `ceil(val)` 的结果是大于或等于 `val` 的最小整数。

**示例 1：**
```js
输入：nums = [10,10,10,10,10], k = 5
输出：50
解释：对数组中每个元素执行一次操作。最后分数是 10 + 10 + 10 + 10 + 10 = 50 。
```
**示例 2：**
```js
输入：nums = [1,10,3,3,3], k = 3
输出：17
解释：可以执行下述操作：
第 1 步操作：选中 i = 1 ，nums 变为 [1,4,3,3,3] 。分数增加 10 。
第 2 步操作：选中 i = 1 ，nums 变为 [1,2,3,3,3] 。分数增加 4 。
第 3 步操作：选中 i = 2 ，nums 变为 [1,1,1,3,3] 。分数增加 3 。
最后分数是 10 + 4 + 3 = 17 。
```
**提示：**
- `1 <= nums.length, k <= 10^5`
- `1 <= nums[i] <= 10^9`

---
### 解法 贪心+原地堆化
用一个最大堆模拟。循环 $k$ 次，每次循环把堆顶加入答案，然后把堆顶 $top$ 更新为 `ceil(top/3)` 。

原地堆化（`heapify`）可以做到 $O(1)$ 的空间复杂度。部分语言用的标准库自带的堆化函数，具体实现可以看下面的 Java 代码。
```cpp
class Solution {
public:
    long long maxKelements(vector<int>& nums, int k) {
        make_heap(nums.begin(), nums.end()); // 原地堆化(最大堆)
        long long ans = 0;
        while (k--) {
            pop_heap(nums.begin(), nums.end()); // 把堆顶移到末尾
            ans += nums.back();
            nums.back() = (nums.back() + 2) / 3;
            push_heap(nums.begin(), nums.end()); // 把末尾元素入堆
        }
        return ans;
    }
};
```
```java
class Solution {
    public long maxKelements(int[] nums, int k) {
        heapify(nums); // 原地堆化(最大堆)
        long ans = 0;
        while (k-- > 0) {
            ans += nums[0]; // 堆顶
            nums[0] = (nums[0] + 2) / 3;
            sink(nums, 0); // 堆化(只要把nums[0]下沉即可)
        }
        return ans;
    }
    // 原地堆化（最大堆）
    // 堆化可以保证 h[0] 是堆顶元素，且 h[i] >= max(h[2*i+1], h[2*i+2])
    private void heapify(int[] h) {
        // 下标 >= h.length / 2 的元素是二叉树的叶子，无需下沉
        // 倒着遍历，从而保证 i 的左右子树一定是堆，那么 sink(h, i) 就可以把左右子树合并成一个堆
        for (int i = h.length / 2 - 1; i >= 0; i--)
            sink(h, i);
    }
    // 把 h[i] 不断下沉，直到 i 的左右儿子都 <= h[i]
    private void sink(int[] h, int i) {
        int n = h.length;
        while (2 * i + 1 < n) {
            int j = 2 * i + 1; // i的左儿子
            if (j + 1 < n && h[j + 1] > h[j]) // i的右儿子比左儿子大
                ++j;
            if (h[j] <= h[i]) // 说明i的左右儿子都<=h[i],停止下沉
                break;
            swap(h, i, j); // 下沉
            i = j;
        }
    }
    // 交换h[i]和h[j]
    private void swap(int[] h, int i, int j) {
        int tmp = h[i];
        h[i] = h[j];
        h[j] = tmp;
    }
}
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n + k\log n)$ ，其中 $n$ 为 $\textit{nums}$ 的长度。把 $\textit{nums}$ 堆化需要 $\mathcal{O}(n)$ 时间。
- 空间复杂度：$\mathcal{O}(1)$ 。