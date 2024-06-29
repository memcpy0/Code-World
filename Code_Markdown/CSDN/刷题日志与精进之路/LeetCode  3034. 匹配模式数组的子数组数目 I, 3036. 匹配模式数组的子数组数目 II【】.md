> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始长度为 `n` 的整数数组 `nums` ，和一个下标从 `0` 开始长度为 `m` 的整数数组 `pattern` ，`pattern` 数组只包含整数 `-1` ，`0` 和 `1` 。

大小为 `m + 1` 的

子数组

 `nums[i..j]` 如果对于每个元素 `pattern[k]` 都满足以下条件，那么我们说这个子数组匹配模式数组 `pattern` ：

- 如果 `pattern[k] == 1` ，那么 `nums[i + k + 1] > nums[i + k]`
- 如果 `pattern[k] == 0` ，那么 `nums[i + k + 1] == nums[i + k]`
- 如果 `pattern[k] == -1` ，那么 `nums[i + k + 1] < nums[i + k]`

请你返回匹配 `pattern` 的 `nums` 子数组的 **数目** 。

**示例 1：**

**输入：**nums = [1,2,3,4,5,6], pattern = [1,1]
**输出：**4
**解释：**模式 [1,1] 说明我们要找的子数组是长度为 3 且严格上升的。在数组 nums 中，子数组 [1,2,3] ，[2,3,4] ，[3,4,5] 和 [4,5,6] 都匹配这个模式。
所以 nums 中总共有 4 个子数组匹配这个模式。

**示例 2：**

**输入：**nums = [1,4,4,1,3,5,5,3], pattern = [1,0,-1]
**输出：**2
**解释：**这里，模式数组 [1,0,-1] 说明我们需要找的子数组中，第一个元素小于第二个元素，第二个元素等于第三个元素，第三个元素大于第四个元素。在 nums 中，子数组 [1,4,4,1] 和 [3,5,5,3] 都匹配这个模式。
所以 nums 中总共有 2 个子数组匹配这个模式。

**提示：**

- `2 <= n == nums.length <= 100`
- `1 <= nums[i] <= 109`
- `1 <= m == pattern.length < n`
- `-1 <= pattern[i] <= 1`

本题和周赛第四题是一样的，请看 [我的题解](https://leetcode.cn/problems/number-of-subarrays-that-match-a-pattern-ii/solution/liang-chong-fang-fa-kmp-zhan-shu-pythonj-zil4/)。


---

给你一个下标从 **0** 开始长度为 `n` 的整数数组 `nums` ，和一个下标从 `0` 开始长度为 `m` 的整数数组 `pattern` ，`pattern` 数组只包含整数 `-1` ，`0` 和 `1` 。

大小为 `m + 1` 的

子数组

 `nums[i..j]` 如果对于每个元素 `pattern[k]` 都满足以下条件，那么我们说这个子数组匹配模式数组 `pattern` ：

- 如果 `pattern[k] == 1` ，那么 `nums[i + k + 1] > nums[i + k]`
- 如果 `pattern[k] == 0` ，那么 `nums[i + k + 1] == nums[i + k]`
- 如果 `pattern[k] == -1` ，那么 `nums[i + k + 1] < nums[i + k]`

请你返回匹配 `pattern` 的 `nums` 子数组的 **数目** 。

**示例 1：**

**输入：**nums = [1,2,3,4,5,6], pattern = [1,1]
**输出：**4
**解释：**模式 [1,1] 说明我们要找的子数组是长度为 3 且严格上升的。在数组 nums 中，子数组 [1,2,3] ，[2,3,4] ，[3,4,5] 和 [4,5,6] 都匹配这个模式。
所以 nums 中总共有 4 个子数组匹配这个模式。

**示例 2：**

**输入：**nums = [1,4,4,1,3,5,5,3], pattern = [1,0,-1]
**输出：**2
**解释：**这里，模式数组 [1,0,-1] 说明我们需要找的子数组中，第一个元素小于第二个元素，第二个元素等于第三个元素，第三个元素大于第四个元素。在 nums 中，子数组 [1,4,4,1] 和 [3,5,5,3] 都匹配这个模式。
所以 nums 中总共有 2 个子数组匹配这个模式。

**提示：**

- `2 <= n == nums.length <= 106`
- `1 <= nums[i] <= 109`
- `1 <= m == pattern.length < n`
- `-1 <= pattern[i] <= 1`

---
### 两种方法：KMP / Z函数（Python/Java/C++/Go）

把 nums\textit{nums}nums 的相邻元素，根据题目规定的大小关系，转换成 1,0,−11,0,-11,0,−1，得到一个长为 n−1n-1n−1 的数组 bbb。

问题相当于问 bbb 中有多少个连续子数组等于 pattern\textit{pattern}pattern。

这是一个标准的字符串匹配问题（本题匹配的是数字不是字符），可以用 KMP 或者 Z 函数解决。

方法一：KMP
关于 KMP 的原理，请看我在知乎上的 [这篇讲解](https://leetcode.cn/link/?target=https%3A%2F%2Fwww.zhihu.com%2Fquestion%2F21923021%2Fanswer%2F37475572)

class Solution:
    def countMatchingSubarrays(self, nums: List[int], pattern: List[int]) -> int:
        m = len(pattern)
        pi = [0] * m
        cnt = 0
        for i in range(1, m):
            v = pattern[i]
            while cnt and pattern[cnt] != v:
                cnt = pi[cnt - 1]
            if pattern[cnt] == v:
                cnt += 1
            pi[i] = cnt

        ans = cnt = 0
        for x, y in pairwise(nums):
            v = (y > x) - (y < x)
            while cnt and pattern[cnt] != v:
                cnt = pi[cnt - 1]
            if pattern[cnt] == v:
                cnt += 1
            if cnt == m:
                ans += 1
                cnt = pi[cnt - 1]
        return ans

```cpp
class Solution {
public:
    int countMatchingSubarrays(vector<int> &nums, vector<int> &pattern) {
        int m = pattern.size();
        vector<int> pi(m);
        int cnt = 0;
        for (int i = 1; i < m; i++) {
            int v = pattern[i];
            while (cnt && pattern[cnt] != v) {
                cnt = pi[cnt - 1];
            }
            if (pattern[cnt] == v) {
                cnt++;
            }
            pi[i] = cnt;
        }

        int ans = 0;
        cnt = 0;
        for (int i = 1; i < nums.size(); i++) {
            int x = nums[i - 1], y = nums[i];
            int v = (y > x) - (y < x);
            while (cnt && pattern[cnt] != v) {
                cnt = pi[cnt - 1];
            }
            if (pattern[cnt] == v) {
                cnt++;
            }
            if (cnt == m) {
                ans++;
                cnt = pi[cnt - 1];
            }
        }
        return ans;
    }
};
```
复杂度分析
时间复杂度：O(n)\mathcal{O}(n)O(n)，其中 nnn 为 nums\textit{nums}nums 的长度。
空间复杂度：O(m)\mathcal{O}(m)O(m)，其中 mmm 为 pattern\textit{pattern}pattern 的长度。
方法二：Z 函数（扩展 KMP）
上场周赛讲了 Z 函数的原理。

我们可以把 pattern\textit{pattern}pattern 拼在 bbb 的前面（为了防止匹配越界，中间插入一个不在数组中的数字，比如 222），根据 Z 函数的定义，只要 z[i]=mz[i] = mz[i]=m，我们就找到了一个匹配。
```cpp
class Solution:
    def countMatchingSubarrays(self, nums: List[int], pattern: List[int]) -> int:
        m = len(pattern)
        pattern.append(2)
        pattern.extend((y > x) - (y < x) for x, y in pairwise(nums))

        n = len(pattern)
        z = [0] * n
        l = r = 0
        for i in range(1, n):
            if i <= r:
                z[i] = min(z[i - l], r - i + 1)
            while i + z[i] < n and pattern[z[i]] == pattern[i + z[i]]:
                l, r = i, i + z[i]
                z[i] += 1

        return sum(lcp == m for lcp in z[m + 1:])
复杂度分析
时间复杂度：O(n)\mathcal{O}(n)O(n)，其中 nnn 为 nums\textit{nums}nums 的长度。
空间复杂度：O(n)\mathcal{O}(n)O(n)。

作者：灵茶山艾府
链接：https://leetcode.cn/problems/number-of-subarrays-that-match-a-pattern-ii/solutions/2637713/liang-chong-fang-fa-kmp-zhan-shu-pythonj-zil4/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```
我是按照算法导论上的定义写的，pi 和 next 的关系是 next[i] = pi[i] - 1