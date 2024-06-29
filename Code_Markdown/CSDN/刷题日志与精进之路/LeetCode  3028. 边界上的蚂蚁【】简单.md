> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

边界上有一只蚂蚁，它有时向 **左** 走，有时向 **右** 走。

给你一个 **非零** 整数数组 `nums` 。蚂蚁会按顺序读取 `nums` 中的元素，从第一个元素开始直到结束。每一步，蚂蚁会根据当前元素的值移动：

- 如果 `nums[i] < 0` ，向 **左** 移动 `-nums[i]`单位。
- 如果 `nums[i] > 0` ，向 **右** 移动 `nums[i]`单位。

返回蚂蚁 **返回** 到边界上的次数。

**注意：**

- 边界两侧有无限的空间。
- 只有在蚂蚁移动了 `|nums[i]|` 单位后才检查它是否位于边界上。换句话说，如果蚂蚁只是在移动过程中穿过了边界，则不会计算在内。

**示例 1：**

**输入：**nums = [2,3,-5]
**输出：**1
**解释：**第 1 步后，蚂蚁距边界右侧 2 单位远。
第 2 步后，蚂蚁距边界右侧 5 单位远。
第 3 步后，蚂蚁位于边界上。
所以答案是 1 。

**示例 2：**

**输入：**nums = [3,2,-3,-4]
**输出：**0
**解释：**第 1 步后，蚂蚁距边界右侧 3 单位远。
第 2 步后，蚂蚁距边界右侧 5 单位远。
第 3 步后，蚂蚁距边界右侧 2 单位远。
第 4 步后，蚂蚁距边界左侧 2 单位远。
蚂蚁从未返回到边界上，所以答案是 0 。

**提示：**

- `1 <= nums.length <= 100`
- `-10 <= nums[i] <= 10`
- `nums[i] != 0`

---
一边遍历数组，一边累加元素，如果发现累加值等于 000，说明返回到边界，把答案加一。

```cpp
class Solution:
    def returnToBoundaryCount(self, nums: List[int]) -> int:
        return sum(s == 0 for s in accumulate(nums))

作者：灵茶山艾府
链接：https://leetcode.cn/problems/ant-on-the-boundary/solutions/2630964/tong-ji-qian-zhui-he-zhong-de-0-de-ge-sh-fh1u/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
    public int returnToBoundaryCount(int[] nums) {
        int ans = 0;
        int sum = 0;
        for (int x : nums) {
            sum += x;
            if (sum == 0) {
                ans++;
            }
        }
        return ans;
    }
}

作者：灵茶山艾府
链接：https://leetcode.cn/problems/ant-on-the-boundary/solutions/2630964/tong-ji-qian-zhui-he-zhong-de-0-de-ge-sh-fh1u/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
public:
    int returnToBoundaryCount(vector<int> &nums) {
        int ans = 0, sum = 0;
        for (int x : nums) {
            sum += x;
            ans += sum == 0;
        }
        return ans;
    }
};

作者：灵茶山艾府
链接：https://leetcode.cn/problems/ant-on-the-boundary/solutions/2630964/tong-ji-qian-zhui-he-zhong-de-0-de-ge-sh-fh1u/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

func returnToBoundaryCount(nums []int) (ans int) {
	sum := 0
	for _, x := range nums {
		sum += x
		if sum == 0 {
			ans++
		}
	}
	return
}

作者：灵茶山艾府
链接：https://leetcode.cn/problems/ant-on-the-boundary/solutions/2630964/tong-ji-qian-zhui-he-zhong-de-0-de-ge-sh-fh1u/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

复杂度分析
时间复杂度：O(n)\mathcal{O}(n)O(n)，其中 nnn 为 nums\textit{nums}nums 的长度。
空间复杂度：O(1)\mathcal{O}(1)O(1)。

作者：灵茶山艾府
链接：https://leetcode.cn/problems/ant-on-the-boundary/solutions/2630964/tong-ji-qian-zhui-he-zhong-de-0-de-ge-sh-fh1u/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

