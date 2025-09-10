> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `nums` 和一个整数 `k`。如果某个连续子数组中恰好有 `k` 个奇数数字，我们就认为这个子数组是「**优美子数组**」。

请返回这个数组中 **「优美子数组」** 的数目。

**示例 1：**
```java
输入：nums = [1,1,2,1,1], k = 3
输出：2
解释：包含 3 个奇数的子数组是 [1,1,2,1] 和 [1,2,1,1] 。
```
**示例 2：**
```java
输入：nums = [2,4,6], k = 1
输出：0
解释：数列中不包含任何奇数，所以不存在优美子数组。
```
**示例 3：**
```java
输入：nums = [2,2,2,1,2,2,1,2,2,2], k = 2
输出：16
```
**提示：**
- `1 <= nums.length <= 50000`
- `1 <= nums[i] <= 10^5`
- `1 <= k <= nums.length`

---
### 解法1 数学 
这个题目中偶数其实是没有用的，可以单独建立一个 **$odd$ 数组来记录第 $i$ 个奇数的下**标。那么枚举奇数，假设当前枚举到第 $i$ 个，那么 $[odd[i],odd[i+k−1]]$ 这个子数组就恰好包含 $k$ 个奇数。由于奇数和奇数间存在偶数，所以一定存在其他子数组 $[l,r]$ 、满足 **$[l,r]$ 包含 $[odd[i],odd[i+k−1]]$ 且 $[l,r]$ 里的奇数个数为 $k$ 个**，那么这个需要怎么统计呢？

由于已经记录了每个奇数的下标，所以知道对于第 $i$ 个奇数，它的前一个奇数的下标为 $odd[i−1]$ ，也就是说 $(odd[i−1],odd[i])$ 间的数都为偶数。同理可得 $(odd[i+k−1],odd[i+k])$ 间的数也都为偶数。那么可以得出——满足 $l∈(odd[i−1],odd[i]\ ]$ 且 $r∈[odd[i+k−1],odd[i+k])$ 条件的子数组 $[l,r]$ ，包含 $[odd[i],odd[i+k−1]]$ 且 $[l,r]$ 里的奇数个数为 $k$ 个。因此**对于第 $i$ 个奇数，它对答案的贡献为符合条件的 $[l,r]$ 的个数**，即：
$$(odd[i]−odd[i−1])×(odd[i+k]−odd[i+k−1])$$ 
只要遍历一遍 $odd$ 数组即可求得最后的答案，注意边界的处理。
```cpp
class Solution {
public:
    int numberOfSubarrays(vector<int>& nums, int k) {
        int n = nums.size(), ans = 0;
        int odd[n + 2], cnt = 0;
        for (int i = 0; i < n; ++i)
            if (nums[i] & 1) odd[++cnt] = i; // 记录第cnt个奇数的下标
        odd[0] = -1, odd[++cnt] = n; // 两个哨兵
        for (int i = 1; i + k <= cnt; ++i) // cnt已经加了1
            ans += (odd[i] - odd[i - 1]) * (odd[i + k] - odd[i + k - 1]);
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$，其中 $n$ 为数组的大小。遍历 $odd$ 数组最坏情况下需要 $O(n)$ 的时间。
- 空间复杂度：$O(n)$，其中 $n$ 为数组的大小。$odd$ 数组需要 $O(n)$ 的空间。

---
### 解法2 前缀和+哈希表 
考虑以 $i$ 结尾的「优美子数组」个数，我们需要统计符合条件的下标 $j$ 的个数，其中 $0≤j≤i$ 这个子数组里的奇数个数恰好为 $k$ 。如果枚举 $[0..i]$ 里所有的下标来判断是否符合条件，那么复杂度将会达到 $O(n^2)$ ，无法通过所有测试用例，因此需要**优化枚举的时间复杂度**。

我们定义 $sum[i+1]$ 为 $[0..i]$ 中奇数的个数，则 $sum[i+1]$ 可以由 $sum[i]$ 和 $nums[i]$ 递推而来，即： 
$$sum[i+1]=sum[i]+(nums[i]\& 1)$$ 
那么「**$[j...i]$ 这个子数组里的奇数个数恰好为 $k$** 」这个条件可转化为
$$sum[i+1]−sum[j]==k$$
简单移项，可得符合条件的下标 $j$ 需要满足
$$sum[j]==sum[i+1]−k$$
所以，考虑以 $i$ 结尾的「优美子数组」个数时，只要统计有多少个奇数个数为 $sum[i+1]−k$ 即可。我们只要建立**频次数组**/哈希表 $cnt$ 记录 $sum[i]$ 出现的次数，从左往右边更新 $cnt$ 边计算答案，那么以 $i$ 结尾的答案 $cnt[sum[i]−k]$ 即可 $O(1)$ 得到。最后的答案即为所有下标结尾的「优美子数组」个数之和。

需要注意的是，从左往右边更新边计算时，已经保证了 $cnt[sum[i+1]−k]$ 里记录的 $sum[j]$ 的下标范围是 $0≤j≤i$ 。同时，由于 $sum[i]$ 的计算只与前一项的答案有关，因此可以不用建立 $sum$ 数组，直接用 $sum$ 变量来记录即可。
```cpp
class Solution {
public:
    int numberOfSubarrays(vector<int>& nums, int k) {
        int n = nums.size(), sum = 0, ans = 0;
        // unordered_map<int, int> cnt;
        int cnt[n + 1];
        memset(cnt, 0, sizeof(cnt));
        cnt[0] = 1;
        for (int i = 0; i < n; ++i) {
            sum += (nums[i] & 1);
            ans += sum >= k ? cnt[sum - k] : 0;
            ++cnt[sum];
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$，其中 $n$ 为数组的大小。只需遍历一遍数组即可求得答案。
- 空间复杂度：$O(n)$，其中 $n$ 为数组的大小。频次数组 $cnt$ 记录的最大值不会超过 $n$ ，因此只需要额外的 $O(n)$ 的空间。

---
### 解法3 快慢指针+滑动窗口
这个解法是解法1的空间优化版。我们不断右移 $right$ 指针来扩大滑动窗口，使其包含 $k$ 个奇数；若当前滑动窗口包含了 $k$ 个奇数，则如下「计算当前窗口的优美子数组个数」：
1. 统计第 $1$ 个奇数左边的偶数个数 $leftEvenCnt$ 。 这 $leftEvenCnt$ 个偶数都可以作为「优美子数组」的起点，因此起点的选择有 $leftEvenCnt + 1$ 种（因为可以一个偶数都不取）。
2. 统计第 $k$ 个奇数右边的偶数个数 $rightEvenCnt$ 。 这 $rightEvenCnt$ 个偶数都可以作为「优美子数组」的终点，因此终点的选择有 $rightEvenCnt + 1$ 种（因为可以一个偶数都不取）。
3. 因此「优美子数组」左右起点的选择组合数为 $(leftEvenCnt + 1) \times (rightEvenCnt + 1)$ 。

```cpp
class Solution {
public:
    int numberOfSubarrays(vector<int>& nums, int k) {
        int n = nums.size(), ans = 0;
        int left = 0, right = 0, oddCnt = 0;
        while (right < n) {
            // 右指针先走,每遇到一个奇数则+1
            if (nums[right++] & 1) ++oddCnt;
            // 若当前滑动窗口[left,right)有k个奇数,进入此分支统计当前滑动窗口中优美子数组个数
            if (oddCnt == k) {
                // 先将滑动窗口右边界向右拓展,直到遇到下个奇数(或出界)
                // rightEvenCnt即为第k个奇数右边的偶数个数
                int tmp = right;
                while (right < n && (nums[right] & 1) == 0) ++right;
                int rightEvenCnt = right - tmp;
                // leftEventCnt即为第1个奇数左边的偶数个数
                int leftEventCnt = 0;
                while ((nums[left] & 1) == 0) {
                    ++leftEventCnt;
                    ++left;
                }
                // 第1个奇数左边的leftEvenCnt个偶数都可作为优美子数组的起点
                // 因为第1个奇数左边可1个偶数都不取,所以起点的选择有leftEvenCnt + 1 种）
                // 第k个奇数右边的rightEvenCnt个偶数都可作为优美子数组的终点
                // 因为第k个奇数右边可以1个偶数都不取,所以终点的选择有 rightEvenCnt + 1 种）
                // 所以该滑动窗口中,优美子数组左右起点的选择组合数为(leftEvenCnt + 1)*(rightEvenCnt + 1) 
                ans += (leftEventCnt + 1) * (rightEvenCnt + 1);
                // 此时left指向的是第1个奇数,因为该区间已经统计完了,因此left右移一位,oddCnt--
                ++left;
                --oddCnt;
            }
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ 
- 空间复杂度：$O(1)$
 