> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `nums` ，你可以对它进行一些操作。

每次操作中，选择任意一个 `nums[i]` ，删除它并获得 `nums[i]` 的点数。之后，你必须删除 **所有** 等于 `nums[i] - 1` 和 `nums[i] + 1` 的元素。

开始你拥有 `0` 个点数。返回你能通过这些操作获得的最大点数。

**示例 1：**
```c
输入：nums = [3,4,2]
输出：6
解释：
删除 4 获得 4 个点数，因此 3 也被删除。
之后，删除 2 获得 2 个点数。总共获得 6 个点数。
```
**示例 2：**
```c
输入：nums = [2,2,3,3,3,4]
输出：9
解释：
删除 3 获得 3 个点数，接着要删除两个 2 和 4 。
之后，再次删除 3 获得 3 个点数，再次删除 3 获得 3 个点数。
总共获得 9 个点数。
```
**提示：**
- `1 <= nums.length <= 2 * 10^4`
- `1 <= nums[i] <= 10^4`
---
### 方法 值域打家劫舍
看示例2，$nums = [2,2,3,3,3,4]$ ，如果我们选了一个等于 $3$ 的数，那么所有等于 $2$ 和 $4$ 的数都被删除，也就是都不能选。选了一个 $3$ 后，剩下的 $3$ 可以继续选。所以如果要选 $3$ ，所有的 $3$ 都要选。

这种【相邻数字不能都选】联想到[198. 打家劫舍](https://leetcode.cn/problems/house-robber/)。

把 $nums$ 转换为一个值域数组 $a$ ，其中 $a[i]$ 表示 $nums$ 中等于 $i$ 的元素之和（下标代表值）。上面的例子中，$a = [0,0,4,9,4]$ ，因为 $nums$ 中有 $3$ 个 $3$ ，所以 $a[3] = 3+3+3 = 9$ 。

计算数组 $a$ 的[198. 打家劫舍](https://leetcode.cn/problems/house-robber/)，即为答案。
```cpp
class Solution {
    // 198. 打家劫舍
    int rob(vector<int>& nums) {
        int f0 = 0, f1 = 0;
        for (int x : nums) {
            int new_f = max(f1, f0 + x);
            f0 = f1;
            f1 = new_f;
        }
        return f1;
    }
public:
    int deleteAndEarn(vector<int>& nums) {
        int mx = ranges::max(nums);
        vector<int> a(mx + 1);
        for (int x : nums) {
            a[x] += x; // 统计等于 x 的元素之和
        }
        return rob(a);
    }
};
```
```java
class Solution {
    public int deleteAndEarn(int[] nums) {
        int mx = 0;
        for (int x : nums) {
            mx = Math.max(mx, x);
        }
        int[] a = new int[mx + 1];
        for (int x : nums) {
            a[x] += x; // 统计等于 x 的元素之和
        }
        return rob(a);
    }
    // 198. 打家劫舍
    private int rob(int[] nums) {
        int f0 = 0;
        int f1 = 0;
        for (int x : nums) {
            int newF = Math.max(f1, f0 + x);
            f0 = f1;
            f1 = newF;
        }
        return f1;
    }
}
```
```rust
use std::collections::HashMap;

impl Solution {
    pub fn delete_and_earn(nums: Vec<i32>) -> i32 {
        let mut rec = HashMap::new();
        for num in nums {
            let cnt = rec.entry(num).or_insert(0);
            *cnt += 1;
        }
        let mut a = Vec::new();
        for (k, v) in rec.iter() {
            a.push([k, v]);
        }
        a.sort_unstable_by(|f, s| f.cmp(s));
        let n = a.len();
        // f[i]表示从a[0]到a[i],通过操作可获得的最大点数
        // 不选择a[i],则f[i]=f[i-1]
        // 选择a[i],则f[i]=a[i]*cnt[a[i]]+f[j]
        // j表示a数组中a[i]-1所在位置的前一个位置
        let mut f = vec![0; n + 1];
        let mut j = 0;
        for i in 0..n {
            while *a[j][0] < *a[i][0] - 1 {
                j += 1;
            }
            f[i + 1] = f[i].max(f[j] + (a[i][0] * a[i][1]));
        }
        f[n]
    }
}

impl Solution {
    // 198. 打家劫舍
    fn rob(nums: Vec<i32>) -> i32 {
        let mut f0 = 0;
        let mut f1 = 0;
        for x in nums {
            let new_f = f1.max(f0 + x);
            f0 = f1;
            f1 = new_f;
        }
        f1
    }

    pub fn delete_and_earn(nums: Vec<i32>) -> i32 {
        let mx = *nums.iter().max().unwrap();
        let mut a = vec![0; mx as usize + 1];
        for x in nums {
            a[x as usize] += x; // 统计等于 x 的元素之和
        }
        Self::rob(a)
    }
}
```
```python
class Solution:
    # 198. 打家劫舍
    def rob(self, nums: List[int]) -> int:
        f0 = f1 = 0
        for x in nums:
            f0, f1 = f1, max(f1, f0 + x)
        return f1

    def deleteAndEarn(self, nums: List[int]) -> int:
        a = [0] * (max(nums) + 1)
        for x in nums:
            a[x] += x  # 统计等于 x 的元素之和
        return self.rob(a)
```
```c
#define MAX(a, b) ((b) > (a) ? (b) : (a))

// 198. 打家劫舍
int rob(int* nums, int numsSize) {
    int f0 = 0, f1 = 0;
    for (int i = 0; i < numsSize; i++) {
        int new_f = MAX(f1, f0 + nums[i]);
        f0 = f1;
        f1 = new_f;
    }
    return f1;
}

int deleteAndEarn(int* nums, int numsSize) {
    int mx = 0;
    for (int i = 0; i < numsSize; i++) {
        mx = MAX(mx, nums[i]);
    }

    int* a = calloc(mx + 1, sizeof(int));
    for (int i = 0; i < numsSize; i++) {
        a[nums[i]] += nums[i]; // 统计等于 x 的元素之和
    }

    int res = rob(a, mx + 1);
    free(a);
    return res;
}
```
```go
// 198. 打家劫舍
func rob(nums []int) int {
    f0, f1 := 0, 0
    for _, x := range nums {
        f0, f1 = f1, max(f1, f0+x)
    }
    return f1
}

func deleteAndEarn(nums []int) int {
    a := make([]int, slices.Max(nums)+1)
    for _, x := range nums {
        a[x] += x // 统计等于 x 的元素之和
    }
    return rob(a)
}
```
```js
// 198. 打家劫舍
var rob = function(nums) {
    let f0 = 0, f1 = 0;
    for (const x of nums) {
        [f0, f1] = [f1, Math.max(f1, f0 + x)]
    }
    return f1;
};

var deleteAndEarn = function(nums) {
    const mx = Math.max(...nums);
    const a = Array(mx + 1).fill(0);
    for (const x of nums) {
        a[x] += x; // 统计等于 x 的元素之和
    }
    return rob(a);
};
```
复杂度分析：
- 时间复杂度：$O(n + U)$ ，其中 $n$ 是 $nums$ 的长度，$U = \max(nums)$ 。
- 空间复杂度：$O(U)$ 。
### 思考题
如果值域更大，比如 $nums[i] \le 10^9$ ，该怎么做？对应的题是[3186. 施咒的最大总伤害](https://leetcode.cn/problems/maximum-total-damage-with-spell-casting/)。

更多相似题目，见动态规划题单中的「**§1.2 打家劫舍**」。