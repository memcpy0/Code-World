> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定一个二进制数组 `nums` ( **索引从0开始** )。

我们将`xi` 定义为其二进制表示形式为子数组 `nums[0..i]` (从最高有效位到最低有效位)。
- 例如，如果 `nums =[1,0,1]` ，那么 `x0 = 1`, `x1 = 2`, 和 `x2 = 5`。

返回布尔值列表 `answer`，只有当 `xi` 可以被 `5` 整除时，答案 `answer[i]` 为 `true`，否则为 `false`。

**示例 1：**
```c
输入：nums = [0,1,1]
输出：[true,false,false]
解释：
输入数字为 0, 01, 011；也就是十进制中的 0, 1, 3 。只有第一个数可以被 5 整除，因此 answer[0] 为 true 。
```
**示例 2：**
```c
输入：nums = [1,1,1]
输出：[false,false,false]
```
**提示：**
- `1 <= nums.length <= 10^5` 
- `nums[i]` 仅为 `0` 或 `1`
---
### 解法 模运算
题意：计算 $nums$ 的每个前缀的二进制数值 $x$ ，判断 $x$ 是否为 $5$ 的倍数。

比如 $nums=[1,1,0,1]$ ，每个前缀对应的二进制数为 $1, 11, 110, 1101$ 。

如何计算这些二进制数呢？在十进制中，我们往 $12$ 的右边加 $3$ 得到 $123$ ，做法是 $12\times 10+3=123$ 。

对于二进制，做法类似，往 $110$ 的右侧添加 $1$ 得到 $1101$ ，做法是 $110 \times 2 + 1= 1101$ 或 $110 \lt \lt 1 | 1 =1101$ 。

注意本题 $nums$ 很长，算出的二进制数 $x$ 很大，但我们只需判断 $x\bmod 5 = 0$ 是否成立。为此可在**中途取模**，也就是每次循环计算出新的 $x$ 后，把 $x$ 替换为 $x\bmod 5$ 。中途取模的原理见[模运算的世界：当加减乘除遇上取模](https://leetcode.cn/circle/discuss/mDfnkW/)。
```java
class Solution {
    public List<Boolean> prefixesDivBy5(int[] nums) {
        List<Boolean> ans = new ArrayList<>(nums.length); // 预分配空间
        int x = 0;
        for (int bit : nums) {
            x = (x << 1 | bit) % 5;
            ans.add(x == 0);
        }
        return ans;
    }
}
```
```cpp
class Solution {
public:
    vector<bool> prefixesDivBy5(vector<int>& nums) {
        vector<bool> ans(nums.size());
        int x = 0;
        for (int i = 0; i < nums.size(); i++) {
            x = (x << 1 | nums[i]) % 5;
            ans[i] = x == 0;
        }
        return ans;
    }
};
```
```python
class Solution:
    def prefixesDivBy5(self, nums: List[int]) -> List[bool]:
        ans = [False] * len(nums)
        x = 0
        for i, bit in enumerate(nums):
            x = (x << 1 | bit) % 5
            ans[i] = x == 0
        return ans
```
```rust
impl Solution {
    pub fn prefixes_div_by5(nums: Vec<i32>) -> Vec<bool> {
        let mut ans = vec![false; nums.len()];
        let mut x = 0;
        for (i, bit) in nums.into_iter().enumerate() {
            x = (x << 1 | bit) % 5;
            ans[i] = x == 0;
        }
        ans
    }
}
```
```go
func prefixesDivBy5(nums []int) []bool {
	ans := make([]bool, len(nums))
	x := 0
	for i, bit := range nums {
		x = (x<<1 | bit) % 5
		ans[i] = x == 0
	}
	return ans
}
```
```c
bool* prefixesDivBy5(int* nums, int numsSize, int* returnSize) {
    *returnSize = numsSize;
    bool* ans = malloc(numsSize * sizeof(bool));
    int x = 0;
    for (int i = 0; i < numsSize; i++) {
        x = (x << 1 | nums[i]) % 5;
        ans[i] = x == 0;
    }
    return ans;
}
```
```js
var prefixesDivBy5 = function(nums) {
    const ans = new Array(nums.length);
    let x = 0;
    for (let i = 0; i < nums.length; i++) {
        x = ((x << 1) | nums[i]) % 5;
        ans[i] = x === 0;
    }
    return ans;
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是 $nums$ 长度。
- 空间复杂度：$O(1)$ ，返回值不计入。