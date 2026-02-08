> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始长度为 `3` 的整数数组 `nums` ，需要用它们来构造三角形。

- 如果一个三角形的所有边长度相等，那么这个三角形称为 **equilateral** 。
- 如果一个三角形恰好有两条边长度相等，那么这个三角形称为 **isosceles** 。
- 如果一个三角形三条边的长度互不相同，那么这个三角形称为 **scalene** 。

如果这个数组无法构成一个三角形，请你返回字符串 `"none"` ，否则返回一个字符串表示这个三角形的类型。

**示例 1：**

**输入：**nums = [3,3,3]
**输出：**"equilateral"
**解释：**由于三条边长度相等，所以可以构成一个等边三角形，返回 "equilateral" 。

**示例 2：**

**输入：**nums = [3,4,5]
**输出：**"scalene"
**解释：**
nums[0] + nums[1] = 3 + 4 = 7 ，大于 nums[2] = 5 `。`
nums[0] + nums[2] = 3 + 5 = 8 ，大于 nums[1] = 4 。
nums[1] + nums[2] = 4 + 5 = 9 ，大于 nums[0] = 3 。
由于任意两边纸盒都大于第三边，所以可以构成一个三角形。
因为三条边的长度互不相等，所以返回 "scalene" 。

**提示：**

- `nums.length == 3`
- `1 <= nums[i] <= 100`

### 按题意模拟（Python/Java/C++/Go）

```cpp
class Solution:
    def triangleType(self, nums: List[int]) -> str:
        nums.sort()
        x, y, z = nums
        if x + y <= z:  # 排序后，只需比较 x+y 和 z
            return "none"
        if x == z:  # 排序了，说明 y 也和 x z 相等
            return "equilateral"
        if x == y or y == z:
            return "isosceles"
        return "scalene"

作者：灵茶山艾府
链接：https://leetcode.cn/problems/type-of-triangle-ii/solutions/2630670/an-ti-yi-mo-ni-pythonjavacgo-by-endlessc-zq6e/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
    public String triangleType(int[] nums) {
        Arrays.sort(nums);
        int x = nums[0];
        int y = nums[1];
        int z = nums[2];
        if (x + y <= z) { // 排序后，只需比较 x+y 和 z
            return "none";
        }
        if (x == z) { // 排序了，说明 y 也和 x z 相等
            return "equilateral";
        }
        if (x == y || y == z) {
            return "isosceles";
        }
        return "scalene";
    }
}

作者：灵茶山艾府
链接：https://leetcode.cn/problems/type-of-triangle-ii/solutions/2630670/an-ti-yi-mo-ni-pythonjavacgo-by-endlessc-zq6e/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

func triangleType(nums []int) string {
	slices.Sort(nums)
	x, y, z := nums[0], nums[1], nums[2]
	if x+y <= z { // 排序后，只需比较 x+y 和 z
		return "none"
	}
	if x == z { // 排序了，说明 y 也和 x z 相等
		return "equilateral"
	}
	if x == y || y == z {
		return "isosceles"
	}
	return "scalene"
}

作者：灵茶山艾府
链接：https://leetcode.cn/problems/type-of-triangle-ii/solutions/2630670/an-ti-yi-mo-ni-pythonjavacgo-by-endlessc-zq6e/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
public:
    string triangleType(vector<int> &nums) {
        sort(nums.begin(), nums.end());
        int x = nums[0], y = nums[1], z = nums[2];
        if (x + y <= z) { // 排序后，只需比较 x+y 和 z
            return "none";
        }
        if (x == z) { // 排序了，说明 y 也和 x z 相等
            return "equilateral";
        }
        if (x == y || y == z) {
            return "isosceles";
        }
        return "scalene";
    }
};

作者：灵茶山艾府
链接：https://leetcode.cn/problems/type-of-triangle-ii/solutions/2630670/an-ti-yi-mo-ni-pythonjavacgo-by-endlessc-zq6e/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。


```
#### 复杂度分析

- 时间复杂度：O(1)\mathcal{O}(1)O(1)。
- 空间复杂度：O(1)\mathcal{O}(1)O(1)。