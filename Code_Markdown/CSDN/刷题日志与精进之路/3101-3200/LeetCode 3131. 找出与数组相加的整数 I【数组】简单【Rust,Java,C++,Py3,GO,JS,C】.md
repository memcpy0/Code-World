> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个长度相等的数组 `nums1` 和 `nums2`。

数组 `nums1` 中的每个元素都与变量 `x` 所表示的整数相加。如果 `x` 为负数，则表现为元素值的减少。

在与 `x` 相加后，`nums1` 和 `nums2` **相等** 。当两个数组中包含相同的整数，并且这些整数出现的频次相同时，两个数组 **相等** 。

返回整数 `x` 。

**示例 1:**
```rust
输入：nums1 = [2,6,4], nums2 = [9,7,5]
输出：3
解释：
与 3 相加后，`nums1` 和 `nums2` 相等。
```
**示例 2:**
```rust
输入：nums1 = [10], nums2 = [5]
输出：-5
解释：
与 `-5` 相加后，`nums1` 和 `nums2` 相等。
```
**示例 3:**
```rust
输入：nums1 = [1,1,1,1], nums2 = [1,1,1,1]
输出：0
解释：
与 0 相加后，`nums1` 和 `nums2` 相等。
```
**提示：**
- `1 <= nums1.length == nums2.length <= 100`
- `0 <= nums1[i], nums2[i] <= 1000`
- 测试用例以这样的方式生成：存在一个整数 `x`，使得 `nums1` 中的每个元素都与 `x` 相加后，`nums1` 与 `nums2` 相等。

---
### 解法 最小值之差
考虑数组中的最小元素，有
$$\min(nums_1)+x=\min(nums_2)$$
即
$$x=\min(nums_2)−\min(nums_1)$$
```rust
impl Solution {
    pub fn added_integer(nums1: Vec<i32>, nums2: Vec<i32>) -> i32 {
        nums2.iter().min().unwrap() - nums1.iter().min().unwrap()
    }
}
```
```java
class Solution {
    public int addedInteger(int[] nums1, int[] nums2) {
        return min(nums2) - min(nums1);
    }

    private int min(int[] nums) {
        int res = Integer.MAX_VALUE;
        for (int x : nums) {
            res = Math.min(res, x);
        }
        return res;
    }
}
```
```cpp
class Solution {
public:
    int addedInteger(vector<int>& nums1, vector<int>& nums2) {
        return ranges::min(nums2) - ranges::min(nums1);
    }
};
```
```python
class Solution:
    def addedInteger(self, nums1: List[int], nums2: List[int]) -> int:
        return min(nums2) - min(nums1)
```
```go
func addedInteger(nums1, nums2 []int) int {
	return slices.Min(nums2) - slices.Min(nums1)
}
```
```js
var addedInteger = function(nums1, nums2) {
    return Math.min(...nums2) - Math.min(...nums1);
};
```
```c
int min(int* nums, int size) {
    int res = nums[0];
    for (int i = 1; i < size; i++) {
        if (nums[i] < res) {
            res = nums[i];
        }
    }
    return res;
}

int addedInteger(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    return min(nums2, nums2Size) - min(nums1, nums1Size);
}
```
**复杂度分析**
- 时间复杂度：$O(n)$ ，其中 $n$ 为 $nums_1$​ 的长度。
- 空间复杂度：$O(1)$ 。