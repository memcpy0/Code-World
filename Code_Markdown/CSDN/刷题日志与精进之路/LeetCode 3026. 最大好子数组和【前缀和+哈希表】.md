> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个长度为 `n` 的数组 `nums` 和一个 **正** 整数 `k` 。

如果 `nums` 的一个子数组中，第一个元素和最后一个元素 **差的绝对值恰好** 为 `k` ，我们称这个子数组为 **好** 的。换句话说，如果子数组 `nums[i..j]` 满足 `|nums[i] - nums[j]| == k` ，那么它是一个好子数组。

请你返回 `nums` 中 **好** 子数组的 **最大** 和，如果没有好子数组，返回 `0` 。

**示例 1：**

**输入：**nums = [1,2,3,4,5,6], k = 1
**输出：**11
**解释：**好子数组中第一个元素和最后一个元素的差的绝对值必须为 1 。好子数组有 [1,2] ，[2,3] ，[3,4] ，[4,5] 和 [5,6] 。最大子数组和为 11 ，对应的子数组为 [5,6] 。

**示例 2：**

**输入：**nums = [-1,3,2,4,5], k = 3
**输出：**11
**解释：**好子数组中第一个元素和最后一个元素的差的绝对值必须为 3 。好子数组有 [-1,3,2] 和 [2,4,5] 。最大子数组和为 11 ，对应的子数组为 [2,4,5] 。

**示例 3：**

**输入：**nums = [-1,-2,-3,-4], k = 2
**输出：**-6
**解释：**好子数组中第一个元素和最后一个元素的差的绝对值必须为 2 。好子数组有 [-1,-2,-3] 和 [-2,-3,-4] 。最大子数组和为 -6 ，对应的子数组为 [-1,-2,-3] 。

**提示：**

- `2 <= nums.length <= 105`
- `-109 <= nums[i] <= 109`
- `1 <= k <= 109`

---
### 前缀和+哈希表（附题单）Python/Java/C++/Go

对于数组 aaa，定义它的前缀和 s[0]=0\textit{s}[0]=0s[0]=0，s[i+1]=∑j=0ia[j]\textit{s}[i+1] = \sum\limits_{j=0}^{i}a[j]s[i+1]= 
j=0
∑
i
​
 a[j]。

根据这个定义，有 s[i+1]=s[i]+a[i]s[i+1]=s[i]+a[i]s[i+1]=s[i]+a[i]。

例如 a=[1,2,1,2]a=[1,2,1,2]a=[1,2,1,2]，对应的前缀和数组为 s=[0,1,3,4,6]s=[0,1,3,4,6]s=[0,1,3,4,6]。

通过前缀和，我们可以把连续子数组的元素和转换成两个前缀和的差，a[left]a[\textit{left}]a[left] 到 a[right]a[\textit{right}]a[right] 的元素和等于

∑j=leftrighta[j]=∑j=0righta[j]−∑j=0left−1a[j]=s[right+1]−s[left]\sum_{j=\textit{left}}^{\textit{right}}a[j] = \sum\limits_{j=0}^{\textit{right}}a[j] - \sum\limits_{j=0}^{\textit{left}-1}a[j] = \textit{s}[\textit{right}+1] - \textit{s}[\textit{left}]
j=left
∑
right
​
 a[j]= 
j=0
∑
right
​
 a[j]− 
j=0
∑
left−1
​
 a[j]=s[right+1]−s[left]
例如 aaa 的子数组 [2,1,2][2,1,2][2,1,2] 的和就可以用 s[4]−s[1]=6−1=5s[4]-s[1]=6-1=5s[4]−s[1]=6−1=5 算出来。

注：s[0]=0s[0]=0s[0]=0 表示一个空数组的元素和。为什么要额外定义它？想一想，如果要计算的子数组恰好是一个前缀（从 a[0]a[0]a[0] 到 a[right]a[\textit{right}]a[right]），你要用 s[right+1]s[\textit{right}+1]s[right+1] 减去谁呢？通过定义 s[0]=0s[0]=0s[0]=0，任意子数组（包括前缀）都可以表示为两个前缀和的差。

思路
为方便描述，把 nums\textit{nums}nums 简称为 aaa。

子数组 a[i..j]a[i..j]a[i..j] 的元素和为

s[j+1]−s[i]s[j+1]-s[i]
s[j+1]−s[i]
枚举 jjj，我们需要找到最小的 s[i]s[i]s[i]，满足 ∣a[i]−a[j]∣=k|a[i]-a[j]|=k∣a[i]−a[j]∣=k，即 a[i]=a[j]−ka[i] = a[j]-ka[i]=a[j]−k 或 a[i]=a[j]+ka[i]=a[j]+ka[i]=a[j]+k。

定义哈希表 minS\textit{minS}minS，键为 a[i]a[i]a[i]，值为相同 a[i]a[i]a[i] 下的 s[i]s[i]s[i] 的最小值。

子数组最后一个数为 a[j]a[j]a[j] 时，子数组的最大元素和为

s[j+1]−minS[a[i]]= s[j+1]−min⁡(minS[a[j]−k],minS[a[j]+k])\begin{aligned} & s[j+1]-\textit{minS}[a[i]]\\ =\ &s[j+1]-\min(\textit{minS}[a[j]-k],\textit{minS}[a[j]+k]) \end{aligned}
= 
​
  
s[j+1]−minS[a[i]]
s[j+1]−min(minS[a[j]−k],minS[a[j]+k])
​
 
Python3
Java
C++
Go


作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-good-subarray-sum/solutions/2630644/qian-zhui-he-ha-xi-biao-fu-ti-dan-python-z61d/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```cpp
class Solution:
    def maximumSubarraySum(self, nums: List[int], k: int) -> int:
        ans = -inf
        min_s = defaultdict(lambda: inf)
        s = 0
        for x in nums:
            ans = max(ans, s + x - min(min_s[x - k], min_s[x + k]))
            min_s[x] = min(min_s[x], s)
            s += x
        return ans if ans > -inf else 0

作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-good-subarray-sum/solutions/2630644/qian-zhui-he-ha-xi-biao-fu-ti-dan-python-z61d/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
    public long maximumSubarraySum(int[] nums, int k) {
        long ans = Long.MIN_VALUE;
        long sum = 0;
        Map<Integer, Long> minS = new HashMap<>();
        for (int x : nums) {
            long s1 = minS.getOrDefault(x - k, Long.MAX_VALUE / 2);
            long s2 = minS.getOrDefault(x + k, Long.MAX_VALUE / 2);
            ans = Math.max(ans, sum + x - Math.min(s1, s2));
            minS.merge(x, sum, Math::min);
            sum += x;
        }
        return ans > Long.MIN_VALUE / 4 ? ans : 0;
    }
}


作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-good-subarray-sum/solutions/2630644/qian-zhui-he-ha-xi-biao-fu-ti-dan-python-z61d/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
public:
    long long maximumSubarraySum(vector<int> &nums, int k) {
        long long ans = LLONG_MIN, sum = 0;
        unordered_map<int, long long> min_s;
        for (int x: nums) {
            auto it = min_s.find(x + k);
            if (it != min_s.end()) {
                ans = max(ans, sum + x - it->second);
            }

            it = min_s.find(x - k);
            if (it != min_s.end()) {
                ans = max(ans, sum + x - it->second);
            }

            it = min_s.find(x);
            if (it == min_s.end() || sum < it->second) {
                min_s[x] = sum;
            }

            sum += x;
        }
        return ans == LLONG_MIN ? 0 : ans;
    }
};


作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-good-subarray-sum/solutions/2630644/qian-zhui-he-ha-xi-biao-fu-ti-dan-python-z61d/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

func maximumSubarraySum(nums []int, k int) int64 {
	ans := math.MinInt
	minS := map[int]int{}
	sum := 0
	for _, x := range nums {
		s, ok := minS[x+k]
		if ok {
			ans = max(ans, sum+x-s)
		}

		s, ok = minS[x-k]
		if ok {
			ans = max(ans, sum+x-s)
		}

		s, ok = minS[x]
		if !ok || sum < s {
			minS[x] = sum
		}

		sum += x
	}
	if ans == math.MinInt {
		return 0
	}
	return int64(ans)
}


作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-good-subarray-sum/solutions/2630644/qian-zhui-he-ha-xi-biao-fu-ti-dan-python-z61d/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

练习：前缀和与哈希表
930. 和相同的二元子数组 1592
560. 和为 K 的子数组
1524. 和为奇数的子数组数目 1611
974. 和可被 K 整除的子数组 1676
523. 连续的子数组和
525. 连续数组
1124. 表现良好的最长时间段 1908
2488. 统计中位数为 K 的子数组 1999
1590. 使数组和能被 P 整除 2039
2949. 统计美丽子字符串 II 2445
面试题 17.05. 字母与数字
1983. 范围和相等的最宽索引对（会员题）
2489. 固定比率的子字符串数（会员题）

作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-good-subarray-sum/solutions/2630644/qian-zhui-he-ha-xi-biao-fu-ti-dan-python-z61d/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。