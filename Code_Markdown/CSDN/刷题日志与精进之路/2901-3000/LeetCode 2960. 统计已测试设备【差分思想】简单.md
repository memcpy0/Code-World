> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个长度为 `n` 、下标从 **0** 开始的整数数组 `batteryPercentages` ，表示 `n` 个设备的电池百分比。

你的任务是按照顺序测试每个设备 `i`，执行以下测试操作：
- 如果 `batteryPercentages[i]` **大于** `0`：
    - **增加** 已测试设备的计数。
    - 将下标在 `[i + 1, n - 1]` 的所有设备的电池百分比减少 `1`，确保它们的电池百分比 **不会低于** `0` ，即 `batteryPercentages[j] = max(0, batteryPercentages[j] - 1)`。
    - 移动到下一个设备。
- 否则，移动到下一个设备而不执行任何测试。

返回一个整数，表示按顺序执行测试操作后 **已测试设备** 的数量。

**示例 1：**
```java
输入：batteryPercentages = [1,1,2,1,3]
输出：3
解释：按顺序从设备 0 开始执行测试操作：
在设备 0 上，batteryPercentages[0] > 0 ，现在有 1 个已测试设备，batteryPercentages 变为 [1,0,1,0,2] 。
在设备 1 上，batteryPercentages[1] == 0 ，移动到下一个设备而不进行测试。
在设备 2 上，batteryPercentages[2] > 0 ，现在有 2 个已测试设备，batteryPercentages 变为 [1,0,1,0,1] 。
在设备 3 上，batteryPercentages[3] == 0 ，移动到下一个设备而不进行测试。
在设备 4 上，batteryPercentages[4] > 0 ，现在有 3 个已测试设备，batteryPercentages 保持不变。
因此，答案是 3 。
```
**示例 2：**
```java
输入：batteryPercentages = [0,1,2]
输出：2
解释：按顺序从设备 0 开始执行测试操作：
在设备 0 上，batteryPercentages[0] == 0 ，移动到下一个设备而不进行测试。
在设备 1 上，batteryPercentages[1] > 0 ，现在有 1 个已测试设备，batteryPercentages 变为 [0,1,1] 。
在设备 2 上，batteryPercentages[2] > 0 ，现在有 2 个已测试设备，batteryPercentages 保持不变。
因此，答案是 2 。
```
**提示：**
- `1 <= n == batteryPercentages.length <= 100`
- `0 <= batteryPercentages[i] <= 100`

---
### 题单：差分数组
[1893. 检查是否区域内所有整数都被覆盖](https://leetcode.cn/problems/check-if-all-the-integers-in-a-range-are-covered/) 1307
[1094. 拼车](https://leetcode.cn/problems/car-pooling/) 1441
[1109. 航班预订统计](https://leetcode.cn/problems/corporate-flight-bookings/) 1570
[2406. 将区间分为最少组数](https://leetcode.cn/problems/divide-intervals-into-minimum-number-of-groups/) 1713
[2381. 字母移位 II](https://leetcode.cn/problems/shifting-letters-ii/) 1793
[995. K 连续位的最小翻转次数](https://leetcode.cn/problems/minimum-number-of-k-consecutive-bit-flips/) 1835
[1943. 描述绘画结果](https://leetcode.cn/problems/describe-the-painting/) 1969
[2251. 花期内花的数目](https://leetcode.cn/problems/number-of-flowers-in-full-bloom/) 2022
[2772. 使数组中的所有元素都等于零](https://leetcode.cn/problems/apply-operations-to-make-all-array-elements-equal-to-zero/) 2029
[2528. 最大化城市的最小供电站数目](https://leetcode.cn/problems/maximize-the-minimum-powered-city/) 2236
[370. 区间加法](https://leetcode.cn/problems/range-addition/)（会员题）
[3009. 折线图上的最大交点数量](https://leetcode.cn/problems/maximum-number-of-intersections-on-the-chart/)（会员题）
### 解法 差分思想
初始化 $\textit{dec}=0$ ，表示需要减一的次数。设 $x=\textit{batteryPercentages}[i]$ ，那么该电池的实际百分比为 $x - \textit{dec}$ ，**如果 $x - \textit{dec} > 0$ ，即 $x > \textit{dec}$ ，那么后面的数都要减一，根据差分数组的思想，把 $\textit{dec}$ 加一即可**。

答案就是 $\textit{dec}$ 。因为每次遇到 $x > \textit{dec}$ 都把 $\textit{dec}$ 加一，这正是题目要求统计的。
```python
class Solution:
    def countTestedDevices(self, batteryPercentages: List[int]) -> int:
        dec = 0
        for x in batteryPercentages:
            if x > dec:
                dec += 1
        return dec
```

```java
class Solution {
    public int countTestedDevices(int[] batteryPercentages) {
        int dec = 0;
        for (int x : batteryPercentages) if (x > dec) ++dec;
        return dec;
    }
}
```

```cpp
class Solution {
public:
    int countTestedDevices(vector<int>& batteryPercentages) {
        int dec = 0;
        for (int x : batteryPercentages) dec += x > dec;
        return dec;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n)$ ，其中 $n$ 为 $\textit{batteryPercentages}$ 的长度。
- 空间复杂度：$\mathcal{O}(1)$ 。仅用到若干额外变量。