> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `ranks` ，表示一些机械工的 **能力值** 。`ranksi` 是第 `i` 位机械工的能力值。能力值为 `r` 的机械工可以在 `r * n^2` 分钟内修好 `n` 辆车。

同时给你一个整数 `cars` ，表示总共需要修理的汽车数目。

请你返回修理所有汽车 **最少** 需要多少时间。

**注意：** 所有机械工可以同时修理汽车。

**示例 1：**
```js
输入：ranks = [4,2,3,1], cars = 10
输出：16
解释：
- 第一位机械工修 2 辆车，需要 4 * 2 * 2 = 16 分钟。
- 第二位机械工修 2 辆车，需要 2 * 2 * 2 = 8 分钟。
- 第三位机械工修 2 辆车，需要 3 * 2 * 2 = 12 分钟。
- 第四位机械工修 4 辆车，需要 1 * 4 * 4 = 16 分钟。
16 分钟是修理完所有车需要的最少时间。
```
**示例 2：**
```js
输入：ranks = [5,1,8], cars = 6
输出：16
解释：
- 第一位机械工修 1 辆车，需要 5 * 1 * 1 = 5 分钟。
- 第二位机械工修 4 辆车，需要 1 * 4 * 4 = 16 分钟。
- 第三位机械工修 1 辆车，需要 8 * 1 * 1 = 8 分钟。
16 分钟时修理完所有车需要的最少时间。
```
**提示：**
- `1 <= ranks.length <= 10^5`
- `1 <= ranks[i] <= 100`
- `1 <= cars <= 10^6`

---
### 解法 二分+优化
注意，能力值越低，修车越快。（按照英文描述，$ranks_i$ 应该翻译成「排名」，排名越靠前，修车越快）。

如果已知修车的时间 $t$，我们可以算出每个人在 $t$ 分钟内能修好多少辆车。例如一个能力值 $r=3$ 的人可以在 $t=16$ 分钟内修好 $2$ 辆车，但无法修好 $3$ 辆车。

根据题意，需要满足
$$rn^2 \le t$$解得
$$n\le \sqrt {\dfrac{t}{r}}$$

所以能力值为 $r$ 的机械工最多可以修
$$\left\lfloor\sqrt {\dfrac{t}{r}}\right\rfloor$$
​辆车。

累加每个人在 $t$ 分钟内能修的车的个数，如果
$$\sum_{i=0}^{i=n-1} \left\lfloor\sqrt {\dfrac{t}  {\textit{ranks}[i]} }\right\rfloor \ge \textit{cars}$$

则说明可以在 $t$ 分钟修完所有车。

上式表明，**$t$ 越大，能修的车越多，越能满足要求**。有了这样的单调性，我们就可以二分答案了。

二分上界为 $\min(\textit{ranks}) \cdot \textit{cars}^2$ ，即**让能力值最低（修车最快）的人修好所有车所需要的时间**。
> 问：开方直接取整的做法是否会有精度误差？
> 答：代码中对整数开方，只要整数转浮点没有丢失精度（在 $2^{53}-1$ 内），开方出来的整数部分就是正确的。具体可以参考 IEEE 754。

```cpp
class Solution {
public:
    long long repairCars(vector<int> &ranks, int cars) {
        int min_r = *min_element(ranks.begin(), ranks.end());
        long long left = 0, right = 1LL * min_r * cars * cars;
        while (left + 1 < right) { // 开区间
            long long mid = (left + right) / 2, s = 0;
            for (int r : ranks)
                s += sqrt(mid / r);
            (s >= cars ? right : left) = mid;
        }
        return right;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n\log(mc^2))$ ，其中 $n$ 为 $\textit{ranks}$ 的长度，$m=\min(\textit{ranks})$ ， $c=\textit{cars}$ 。
- 空间复杂度：$\mathcal{O}(1)$ 。仅用到若干额外变量。

---
### 解法2 优化
能力值相同的人，在 $t$ 分钟内修好的车的个数是一样的。

根据数据范围，$\textit{ranks}$ 中至多有 100100100 个不同的数字，我们可以统计 $ranks$ 中每个数字的出现次数，这样每次二分中至多循环 $100$ 次。

此外，**如果循环中发现 $s\ge \textit{cars}$ ，可以提前退出循环**。
```cpp
class Solution {
public:
    long long repairCars(vector<int> &ranks, int cars) {
        int min_r = ranks[0], cnt[101]{}; // 数组比哈希表更快
        for (int r : ranks) {
            min_r = min(min_r, r);
            cnt[r]++;
        }
        long long left = 0, right = 1LL * min_r * cars * cars;
        while (left + 1 < right) {
            long long mid = (left + right) / 2, s = 0;
            for (int r = min_r; r <= 100 && s < cars; r++) // 至多循环 100 次
                s += (long long) sqrt(mid / r) * cnt[r];
            (s >= cars ? right : left) = mid;
        }
        return right;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n + M\log(mc^2))$ ，其中 $n$ 为 $\textit{ranks}$ 的长度 $M=\max(\textit{ranks})$  ，$m=\min(\textit{ranks})$ ，$c=cars$ 。
- 空间复杂度：$\mathcal{O}(M)$ 。

