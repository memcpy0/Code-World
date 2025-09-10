> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `salary` ，数组里每个数都是 **唯一** 的，其中 `salary[i]` 是第 `i` 个员工的工资。

请你返回去掉最低工资和最高工资以后，剩下员工工资的平均值。

**示例 1：**
```java
输入：salary = [4000,3000,1000,2000]
输出：2500.00000
解释：最低工资和最高工资分别是 1000 和 4000 。
去掉最低工资和最高工资以后的平均工资是 (2000+3000)/2= 2500
```
**示例 2：**
```java
输入：salary = [1000,2000,3000]
输出：2000.00000
解释：最低工资和最高工资分别是 1000 和 3000 。
去掉最低工资和最高工资以后的平均工资是 (2000)/1= 2000
```
**示例 3：**
```java
输入：salary = [6000,5000,4000,3000,2000,1000]
输出：3500.00000
```
**示例 4：**
```java
输入：salary = [8000,9000,2000,3000,6000,1000]
输出：4750.00000
```
**提示：**
- `3 <= salary.length <= 100`
- `10^3 <= salary[i] <= 10^6`
- `salary[i]` 是唯一的。
- 与真实值误差在 `10^-5` 以内的结果都将视为正确答案。

---
### 解法 数学
设 $\textit{salary}$ 的元素和为 $s$ ，最小值为 $m$ ，最大值为 $M$ ，那么剩下 $n-2$ 个人的工资之和为 $s-m-M$ ，平均值为
$$\dfrac{s-m-M}{n-2}$$

```python
class Solution:
    def average(self, salary: List[int]) -> float:
        return (sum(salary) - min(salary) - max(salary)) / (len(salary) - 2)
```

```java
class Solution {
    public double average(int[] salary) {
        int s = 0;
        int m = Integer.MAX_VALUE;
        int M = Integer.MIN_VALUE;
        for (int x : salary) {
            s += x;
            m = Math.min(m, x);
            M = Math.max(M, x);
        }
        int n = salary.length;
        return (double) (s - m - M) / (n - 2);
    }
}

class Solution {
    public double average(int[] salary) {
        int s = Arrays.stream(salary).sum();
        int m = Arrays.stream(salary).min().getAsInt();
        int M = Arrays.stream(salary).max().getAsInt();
        int n = salary.length;
        return (double) (s - m - M) / (n - 2);
    }
}
```

```cpp
class Solution {
public:
    double average(vector<int>& salary) {
        int s = reduce(salary.begin(), salary.end());
        auto [m, M] = ranges::minmax(salary);
        return (double) (s - m - M) / (salary.size() - 2);
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n)$ ，其中 $n$ 为 $\textit{salary}$ 的长度。
- 空间复杂度：$\mathcal{O}(1)$ 。
