> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数 `total` ，表示你拥有的总钱数。同时给你两个整数 `cost1` 和 `cost2` ，分别表示一支钢笔和一支铅笔的价格。你可以花费你部分或者全部的钱，去买任意数目的两种笔。

请你返回购买钢笔和铅笔的 **不同方案数目** 。

**示例 1：**
```java
输入：total = 20, cost1 = 10, cost2 = 5
输出：9
解释：一支钢笔的价格为 10 ，一支铅笔的价格为 5 。
- 如果你买 0 支钢笔，那么你可以买 0 ，1 ，2 ，3 或者 4 支铅笔。
- 如果你买 1 支钢笔，那么你可以买 0 ，1 或者 2 支铅笔。
- 如果你买 2 支钢笔，那么你没法买任何铅笔。
所以买钢笔和铅笔的总方案数为 5 + 3 + 1 = 9 种。
```
**示例 2：**
```java
输入：total = 5, cost1 = 10, cost2 = 10
输出：1
解释：钢笔和铅笔的价格都为 10 ，都比拥有的钱数多，所以你没法购买任何文具。所以只有 1 种方案：买 0 支钢笔和 0 支铅笔。
```
**提示：**
- `1 <= total, cost1, cost2 <= 10^6`

---
### 解法1 数学+枚举
枚举买了 $i$ 支钢笔。至多能买 $\left\lfloor\dfrac{\textit{total}}{\textit{cost}_1}\right\rfloor$ 支钢笔。剩余钱数为 $\textit{total} - i * \textit{cost}_1$ ，至少可以买 $0$ 支铅笔，至多可以买 $\left\lfloor\dfrac{\textit{total} - i * \textit{cost}_1}{\textit{cost}_2}\right\rfloor$ 支铅笔，这一共有 $$1 + \left\lfloor\dfrac{\textit{total} - i * \textit{cost}_1}{\textit{cost}_2}\right\rfloor$$
种不同的购买方案。所以答案为
$$\sum_{i=0}^{\lfloor \textit{total}/\textit{cost}_1\rfloor} 1 + \left\lfloor\dfrac{\textit{total} - i * \textit{cost}_1}{\textit{cost}_2}\right\rfloor$$ 
，即$$1+\left\lfloor\dfrac{\textit{total}}{\textit{cost}_1}\right\rfloor + \sum_{i=0}^{\lfloor \textit{total}/\textit{cost}_1\rfloor} \left\lfloor\dfrac{\textit{total} - i * \textit{cost}_1}{\textit{cost}_2}\right\rfloor$$

```cpp
class Solution {
public:
    long long waysToBuyPensPencils(int total, int cost1, int cost2) {
        long long n = total / cost1, ans = n + 1;
        for (int i = 0; i <= n; ++i)
            ans += (total - i * cost1) / cost2;
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O\Bigg(\Bigg\lfloor \dfrac{total}{cost_1}\Bigg \rfloor\Bigg)$
- 空间复杂度：$O(1)$

---
### 解法2 类欧几里得算法
上面的和式可以直接套用类欧几里德算法解决。参考：[类欧几里德算法](https://leetcode.cn/link/?target=https%3A%2F%2Foi-wiki.org%2Fmath%2Fnumber-theory%2Feuclidean%2F)。
```cpp
class Solution {
public:
    long long waysToBuyPensPencils(int total, int cost1, int cost2) {
        long long n = total / cost1;
        int a = -cost1, b = total, c= cost2;
        long long ans=n+1;
        if (a<0){
            ans -= n * (n+1) * (1 - a / c) / 2;
            a = a % c + c;
        }
        return ans + floorsum(a, b, c, n);
    }

    long long floorsum(int a, int b, int c, long long n){
        long long ans=0;
        if (a>=c){
            ans += n * (n+1) * (a / c) / 2;
            a = a % c;
        }
        if (b >= c){
            ans += (n+1) * (b / c);
            b = b % c;
        }
        int m = (a*n+b)/c;
        if (m==0)
            return ans;
        return ans + n*m - floorsum(c, c-b-1, a, m-1);
    }
};
```
复杂度分析：
- 时间复杂度：$O\Bigg(\log \Bigg\lfloor \dfrac{total}{cost_1}\Bigg \rfloor\Bigg)$
- 空间复杂度：$O(1)$
