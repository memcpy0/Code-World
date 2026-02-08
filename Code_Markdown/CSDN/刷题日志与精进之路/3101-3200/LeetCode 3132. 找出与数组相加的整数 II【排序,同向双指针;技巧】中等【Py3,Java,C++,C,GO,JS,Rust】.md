> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个整数数组 `nums1` 和 `nums2`。

从 `nums1` 中移除两个元素，并且所有其他元素都与变量 `x` 所表示的整数相加。如果 `x` 为负数，则表现为元素值的减少。

执行上述操作后，`nums1` 和 `nums2` **相等** 。当两个数组中包含相同的整数，并且这些整数出现的频次相同时，两个数组 **相等** 。

返回能够实现数组相等的 **最小** 整数 `x` 。

**示例 1:**
```rust
输入：nums1 = [4,20,16,12,8], nums2 = [14,18,10]
输出：-2
解释：
移除 `nums1` 中下标为 `[0,4]` 的两个元素，并且每个元素与 `-2` 相加后，`nums1` 变为 `[18,14,10]` ，与 `nums2` 相等。
```
**示例 2:**
```cpp
输入：nums1 = [3,5,5,3], nums2 = [7,7]
输出：2
解释：
移除 `nums1` 中下标为 `[0,3]` 的两个元素，并且每个元素与 `2` 相加后，`nums1` 变为 `[7,7]` ，与 `nums2` 相等。
```
**提示：**
- `3 <= nums1.length <= 200`
- `nums2.length == nums1.length - 2`
- `0 <= nums1[i], nums2[i] <= 1000`
- 测试用例以这样的方式生成：存在一个整数 `x`，`nums1` 中的每个元素都与 `x` 相加后，再移除两个元素，`nums1` 可以与 `nums2` 相等。

---
### 解法 $O(n\log n)$ 排序+判断子序列
把两个数组都从小到大排序。**由于只能移除两个元素，所以 $nums_1$ 的前三小元素必定有一个是保留下来的**，我们可以**枚举**保留下来的最小元素是 $nums_1[0]$ 还是 $nums_1[1]$ 还是 $nums_1[2]$ 。
> ⚠注意：保留下来的**最小**元素绝对不可能是 $nums_1​[3]$ 或者更大的数，因为这意味着我们把 $nums_1[0],\ nums_1[1],\ nums_1[2]$ 都移除了，而题目要求只能移除两个元素。

例如排序后 $nums_1=[2,5,6,7,8,10],\ nums_2=[3,4,5,8]$ ，如果 $nums_1$ 中保留下来的最小元素是 $nums_1[1]=5$ ，那么 $x=nums_2[0]−nums_1[1]=3−5=−2$ ，这意味着如果我们把 $nums_1$
 的每个数都加上 $x=−2$ ，得到 $nums_1=[0,3,4,5,6,8]$ ，问题就变成判断 $nums_2$ 是否为 $nums_1$ 的子序列，如果是子序列，那么我们就可以放心地移除多余的两个数了。

做法见 [392. 判断子序列](https://leetcode.cn/problems/is-subsequence/)，可以用[**同向双指针**](https://leetcode.cn/problems/is-subsequence/solution/jian-ji-xie-fa-pythonjavaccgojsrust-by-e-mz22/)解决。

代码实现时，可以先判断保留下来的最小元素是 $nums_1[2]$ ，再判断是 $nums_1[1]$ ，最后判断是 $nums_1[0]$ 。这是因为 **$nums_1[i]$ 越大，答案 $x$ 越小，第一个满足的就是答案**。此外，由于题目保证答案一定存在，所以当 $nums_1[2]$ 和 $nums_1[1]$ 都不满足时，直接返回 $nums_2[0]−nums_1[0]$ ，无需判断。
```rust
impl Solution {
    pub fn minimum_added_integer(mut nums1: Vec<i32>, mut nums2: Vec<i32>) -> i32 {
        nums1.sort_unstable();
        nums2.sort_unstable();
        // 枚举保留 nums1[2] 或者 nums1[1] 或者 nums1[0]
        // 倒着枚举是因为 nums1[i] 越大答案越小，第一个满足的就是答案
        for i in (1..3).rev() {
            let x = nums2[0] - nums1[i];
            // 在 {nums1[i] + x} 中找子序列 nums2
            let mut j = 0;
            for &v in &nums1[i..] {
                if nums2[j] == v + x && { j += 1; j == nums2.len() } {
                    // nums2 是 {nums1[i] + x} 的子序列
                    return x;
                }
            }
        }
        // 题目保证答案一定存在
        nums2[0] - nums1[0]
    }
}
```
```cpp
class Solution {
public:
    int minimumAddedInteger(vector<int>& nums1, vector<int>& nums2) {
        ranges::sort(nums1);
        ranges::sort(nums2);
        // 枚举保留 nums1[2] 或者 nums1[1] 或者 nums1[0]
        // 倒着枚举是因为 nums1[i] 越大答案越小，第一个满足的就是答案
        for (int i = 2; i > 0; i--) {
            int x = nums2[0] - nums1[i];
            // 在 {nums1[i] + x} 中找子序列 nums2
            int j = 0;
            for (int k = i; k < nums1.size(); k++) {
                if (nums2[j] == nums1[k] + x && ++j == nums2.size()) {
                    // nums2 是 {nums1[i] + x} 的子序列
                    return x;
                }
            }
        }
        // 题目保证答案一定存在
        return nums2[0] - nums1[0];
    }
};
```
```python
class Solution:
    def minimumAddedInteger(self, nums1: List[int], nums2: List[int]) -> int:
        nums1.sort()
        nums2.sort()
        # 枚举保留 nums1[2] 或者 nums1[1] 或者 nums1[0]
        # 倒着枚举是因为 nums1[i] 越大答案越小，第一个满足的就是答案
        for i in range(2, 0, -1):
            x = nums2[0] - nums1[i]
            # 在 {nums1[i] + x} 中找子序列 nums2
            j = 0
            for v in nums1[i:]:
                if nums2[j] == v + x:
                    j += 1
                    # nums2 是 {nums1[i] + x} 的子序列
                    if j == len(nums2):
                        return x
        # 题目保证答案一定存在
        return nums2[0] - nums1[0]

# python iter版
class Solution:
    def minimumAddedInteger(self, nums1: List[int], nums2: List[int]) -> int:
        nums1.sort()
        nums2.sort()
        for i in range(2, 0, -1):
            x = nums2[0] - nums1[i]
            it = iter(nums1[i:])
            # 判断 {nums2[j] - x} 是否为 nums1[i:] 的子序列
            # in 会消耗迭代器
            if all(v - x in it for v in nums2):
                return x
        return nums2[0] - nums1[0]
```
```java
class Solution {
    public int minimumAddedInteger(int[] nums1, int[] nums2) {
        Arrays.sort(nums1);
        Arrays.sort(nums2);
        // 枚举保留 nums1[2] 或者 nums1[1] 或者 nums1[0]
        // 倒着枚举是因为 nums1[i] 越大答案越小，第一个满足的就是答案
        for (int i = 2; i > 0; i--) {
            int x = nums2[0] - nums1[i];
            // 在 {nums1[i] + x} 中找子序列 nums2
            int j = 0;
            for (int k = i; k < nums1.length; k++) {
                if (nums2[j] == nums1[k] + x && ++j == nums2.length) {
                    // nums2 是 {nums1[i] + x} 的子序列
                    return x;
                }
            }
        }
        // 题目保证答案一定存在
        return nums2[0] - nums1[0];
    }
}
```
```go
func minimumAddedInteger(nums1, nums2 []int) int {
	slices.Sort(nums1)
	slices.Sort(nums2)
	// 枚举保留 nums1[2] 或者 nums1[1] 或者 nums1[0]
	// 倒着枚举是因为 nums1[i] 越大答案越小，第一个满足的就是答案
	for i := 2; i > 0; i-- {
		x := nums2[0] - nums1[i]
		// 在 {nums1[i] + x} 中找子序列 nums2
		j := 0
		for _, v := range nums1[i:] {
			if nums2[j] == v+x {
				j++
				// nums2 是 {nums1[i] + x} 的子序列
				if j == len(nums2) {
					return x
				}
			}
		}
	}
	// 题目保证答案一定存在
	return nums2[0] - nums1[0]
}
```
```c
int cmp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

int minimumAddedInteger(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    qsort(nums1, nums1Size, sizeof(int), cmp);
    qsort(nums2, nums2Size, sizeof(int), cmp);
    // 枚举保留 nums1[2] 或者 nums1[1] 或者 nums1[0]
    // 倒着枚举是因为 nums1[i] 越大答案越小，第一个满足的就是答案
    for (int i = 2; i > 0; i--) {
        int x = nums2[0] - nums1[i];
        // 在 {nums1[i] + x} 中找子序列 nums2
        int j = 0;
        for (int k = i; k < nums1Size; k++) {
            if (nums2[j] == nums1[k] + x && ++j == nums2Size) {
                // nums2 是 {nums1[i] + x} 的子序列
                return x;
            }
        }
    }
    // 题目保证答案一定存在
    return nums2[0] - nums1[0];
}
```
```js
var minimumAddedInteger = function(nums1, nums2) {
    nums1.sort((a, b) => a - b);
    nums2.sort((a, b) => a - b);
    // 枚举保留 nums1[2] 或者 nums1[1] 或者 nums1[0]
    // 倒着枚举是因为 nums1[i] 越大答案越小，第一个满足的就是答案
    for (let i = 2; i > 0; i--) {
        const x = nums2[0] - nums1[i];
        // 在 {nums1[i] + x} 中找子序列 nums2
        let j = 0;
        for (let k = i; k < nums1.length; k++) {
            if (nums2[j] === nums1[k] + x && ++j === nums2.length) {
                // nums2 是 {nums1[i] + x} 的子序列
                return x;
            }
        }
    }
    // 题目保证答案一定存在
    return nums2[0] - nums1[0];
};
```
**复杂度分析**：
- 时间复杂度：$O(n\log n)$ ，其中 $n$ 为 $nums_1$ 的长度。瓶颈在排序上。
- 空间复杂度：$O(1)$ 。忽略排序的栈开销，忽略切片的空间。
