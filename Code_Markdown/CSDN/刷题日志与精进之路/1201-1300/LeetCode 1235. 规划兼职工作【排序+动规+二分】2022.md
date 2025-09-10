> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

你打算利用空闲时间来做兼职工作赚些零花钱。

这里有 `n` 份兼职工作，每份工作预计从 `startTime[i]` 开始到 `endTime[i]` 结束，报酬为 `profit[i]`。

给你一份兼职工作表，包含开始时间 `startTime`，结束时间 `endTime` 和预计报酬 `profit` 三个数组，请你计算并返回可以获得的最大报酬。

注意，时间上出现重叠的 2 份工作不能同时进行。

如果你选择的工作在时间 `X` 结束，那么你可以立刻进行在时间 `X` 开始的下一份工作。

**示例 1：**
**![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2019/10/19/sample1_1584.png)**
```cpp
输入：startTime = [1,2,3,3], endTime = [3,4,5,6], profit = [50,10,40,70]
输出：120
解释：
我们选出第 1 份和第 4 份工作， 
时间范围是 [1-3]+[3-6]，共获得报酬 120 = 50 + 70。
```
**示例 2：**
**![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2019/10/19/sample22_1584.png)**
```java
输入：startTime = [1,2,3,4,6], endTime = [3,5,10,6,9], profit = [20,20,100,70,60]
输出：150
解释：
我们选择第 1，4，5 份工作。 
共获得报酬 150 = 20 + 70 + 60。
```
**示例 3：**

**![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2019/10/19/sample3_1584.png)**
```java
输入：startTime = [1,1,1], endTime = [2,3,4], profit = [5,6,4]
输出：6
```
**提示：**
- `1 <= startTime.length == endTime.length == profit.length <= 5 * 10^4`
- `1 <= startTime[i] < endTime[i] <= 10^9`
- `1 <= profit[i] <= 10^4`

---
### 相似题目
- [1751. 最多可以参加的会议数目 II](https://leetcode.cn/problems/maximum-number-of-events-that-can-be-attended-ii/)
- [2008. 出租车的最大盈利](https://leetcode.cn/problems/maximum-earnings-from-taxi/)
### 解法 排序+动态规划+二分优化
将工作按照**结束时间**排序，以示例 2 为例，得到下图：

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202405052332562.png)

手动计算一下，按照结束时间排序后：
1. 前 $1$ 个工作的最大报酬为 $20$ ；
2. 前 $2$ 个工作的最大报酬为 $20$ ；
3. 前 $3$ 个工作的最大报酬为前 $1$ 个工作的最大报酬 $+70=20+70=90$ ；
4. 前 $4$ 个工作的最大报酬为前 $3$ 个工作的最大报酬 $+60=90+60=150$ ；
5. 前 $5$ 个工作的最大报酬，如果选了第 $5$ 个工作，那么报酬为前 $1$ 个工作的最大报酬 $+100=20+100=120$ ；但也可以不选第 $5$ 个工作，报酬为前 $4$ 个工作的最大报酬，即 $150$ 。由于 $150>120$ ，不选第 $5$ 个工作更好，因此前 $5$ 个工作的最大报酬为 $150$ 。

示例 2 等价于计算前 $5$ 个工作的最大报酬，即 $150$ 。

总结一下，我们可以分类讨论，求出**按照结束时间排序后的前 $i$ 个工作的最大报酬**：
- 不选第 $i$ 个工作，那么最大报酬等于前 $i−1$ 个工作的最大报酬（转换成了一个规模更小的子问题）；
- 选第 $i$ 个工作，由于**工作时间不能重叠**，设 $j$ 是最大的满足 $endTime[j]≤startTime[i]$ 的 $j$ ，那么最大报酬等于前 $j$ 个工作的最大报酬加上 $profit[i]$（同样转换成了一个规模更小的子问题）；

这两种决策取最大值。
> 注意，**由于按照结束时间排序，前 $j$ 个工作中的任意一个都不会与第 $i$ 个工作的时间重叠**。这是排序的作用其一；其二是为了用二分优化查找。

怎么实现？上述思路是一个标准的关于递推的描述，定义 $f[i]$ 表示按照结束时间排序后的前 $i$ 个工作的最大报酬，用「选或不选」分类讨论：
- 不选第 $i$ 个工作：$f[i]=f[i−1]$ ；
- 选第 $i$ 个工作：$f[i]=f[j]+profit[i]$ ，其中 $j$ 是最大的满足 $endTime[j]≤startTime[i]$ 的 $j$ ，不存在时为 $-1$ 。

两者取最大值，即
$$f[i] = \max(f[i-1], f[j]+\textit{profit}[i])$$
由于 $i=0$ 时会产生 $-1$ ，可以在 $f$ 数组前面插入一个 $0$ ，与 $f$ 有关的下标都 $+1$ ，即
$$f[i+1] = \max(f[i], f[j+1]+\textit{profit}[i])$$
初始项 $f[0]=0$ ，答案为 $f[n]$ 。

代码实现时，由于结束时间是有序的，$j$ 可以用二分查找计算出来。
```python
class Solution:
    def jobScheduling(self, startTime: List[int], endTime: List[int], profit: List[int]) -> int:
        jobs = sorted(zip(endTime, startTime, profit)) # 按照结束时间排序
        f = [0] * (len(jobs) + 1)
        for i, (_, st, p) in enumerate(jobs):
            j = bisect_left(jobs, (st + 1, ), hi = i) # hi=i表示二分上界为i(默认为n)
            # 状态转移中，为什么是 j 不是 j+1：上面算的是 > st，-1 后得到 <= st，但由于还要 +1，抵消了
            f[i + 1] = max(f[i], f[j] + p)
        return f[-1]
```

```java
class Solution {
    public int jobScheduling(int[] startTime, int[] endTime, int[] profit) {
        int n = startTime.length;
        int[][] jobs = new int[n][];
        for (int i = 0; i < n; ++i) 
            jobs[i] = new int[]{ startTime[i], endTime[i], profit[i] };
        Arrays.sort(jobs, (a, b) -> a[1] - b[1]); // 按照结束时间排序

        int[] f = new int[n + 1];
        for (int i = 0; i < n; ++i) {
            int j = search(jobs, i, jobs[i][0]);
            f[i + 1] = Math.max(f[i], f[j + 1] + jobs[i][2]);
        }
        return f[n];
    }
    // 返回 endTime <= upper 的最大下标
    private int search(int[][] jobs, int right, int upper) {
        int left = -1;
        while (left + 1 < right) { // (left,right)
            int mid = (left + right) >>> 1;
            if (jobs[mid][1] <= upper) left = mid;
            else right = mid;
        }
        return left;
    }
}
```

```cpp
class Solution {
public:
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        int n = startTime.size();
        vector<array<int, 3>> jobs(n);
        for (int i = 0; i < n; ++i) jobs[i] = { endTime[i], startTime[i], profit[i] };
        ranges::sort(jobs, [](auto &a, auto &b) { return a[0] < b[0]; }); // 按照结束时间排序

        vector<int> f(n + 1);
        for (int i = 0; i < n; ++i) {
            int j = upper_bound(jobs.begin(), jobs.begin() + i, 
                array<int, 3>{ jobs[i][1], INT_MAX }) - jobs.begin();            
            // 状态转移中，为什么是 j 不是 j+1：上面算的是 > 开始时间，-1 后得到 <= 开始时间，但由于还要 +1，抵消了
            f[i + 1] = max(f[i], f[j] + jobs[i][2]);
        }
        return f[n];
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n\log n)$ ，其中 $n$ 为 $\textit{startTime}$  的长度。排序的时间复杂度为 $\mathcal{O}(n\log n)$ ，动态规划部分一共计算了 $n$ 次二分，时间复杂度为 $\mathcal{O}(n\log n)$ ，因此总的时间复杂度为 $\mathcal{O}(n\log n)$ 。
- 空间复杂度：$\mathcal{O}(n)$ 。