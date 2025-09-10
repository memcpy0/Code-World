> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

There is a school that has classes of students and each class will be having a final exam. You are given a 2D integer array `classes`, where `classes[i] = [passi, totali]`. You know beforehand that in the `ith` class, there are `totali` total students, but only `passi` number of students will pass the exam.

You are also given an integer `extraStudents`. There are another `extraStudents` brilliant students that are **guaranteed** to pass the exam of any class they are assigned to. You want to assign each of the `extraStudents` students to a class in a way that **maximizes** the **average** pass ratio across **all** the classes.

The **pass ratio** of a class is equal to the number of students of the class that will pass the exam divided by the total number of students of the class. The **average pass ratio** is the sum of pass ratios of all the classes divided by the number of the classes.

Return _the **maximum** possible average pass ratio after assigning the_ `extraStudents` _students._ Answers within `10-5` of the actual answer will be accepted.

**Example 1:**
```java
Input: classes = [[1,2],[3,5],[2,2]], `extraStudents` = 2
Output: 0.78333
Explanation: You can assign the two extra students to the first class. The average pass ratio will be equal to (3/4 + 3/5 + 2/2) / 3 = 0.78333.
```
**Example 2:**
```java
Input: classes = [[2,4],[3,9],[4,5],[2,10]], `extraStudents` = 4
Output: 0.53485
```
**Constraints:**
-   `1 <= classes.length <= 105`
-   `classes[i].length == 2`
-   `1 <= passi <= totali <= 105`
-   `1 <= extraStudents <= 105`

题意：学校的每个班都要期末考试了，给出一个二维数组 `classes = [[passi, totali], ...]` ，其中第 `i` 个班有 `totali` 个学生，但只有 `passi` 个学生能通过期末考试。现在有 `extraStudents` 个额外的学生，无论在哪个班，他们都一定能通过期末考试！我们的任务是，将每个额外的学生都指定到某一个班级中，**让期末考试的平均通过率最大化**。

一个班级的通过率是 `passi / totali` ，平均通过率是所有班级通过率的总和除以班级数。返回最大能得到的平均通过率。

---
### 解法1 贪心+堆
==由于班级总数不会变化，因此题目所求「最大化平均通过率」等价于「最大化总通过率」==。一开始我想的是：将所有班级的通过率放进堆中，然后不断取出最低通过率，并给对应班级塞进一个 `extraStudent` ，然后重新放回堆中，直到全部额外学生均被分配完成。不过示例1就让这个贪心策略胎死腹中，==增加最低通过率确实能增加总通过率，但不是最优的做法==。

设某个班级的人数为 $\textit{total}$ ，其中通过考试的人数为 $\textit{pass}$ ，那么给这个班级安排一个额外通过考试的学生，其通过率会增加：
$$\frac{\textit{pass} + 1}{\textit{total} + 1} - \frac{\textit{pass}}{\textit{total}}$$
​我们的贪心策略是：**优先选择通过率增加量最大的班级**，这样做的正确性在于，给同一个班级不断地增加安排的学生数量时，其增加的通过率是单调递减的，即：
$$\frac{\textit{pass} + 2}{\textit{total} + 2} - \frac{\textit{pass} + 1}{\textit{total} + 1} < \frac{\textit{pass} + 1}{\textit{total} + 1} - \frac{\textit{pass}}{\textit{total}}$$
- 时间复杂度：$O((n + m) \log n)$ ，$n$ 为 $\textit{classes}$ 的长度，$m$ 等于 $\textit{extraStudents}$ 。如果采用堆化写法 *heapify* ，时间复杂度为 $O(n + m \log n)$ 。
- 空间复杂度：$O(n)$ 。若直接在 $classes$ 上原地堆化，则可以做到 $O(1)$ 额外空间。
```cpp
class Solution {
public:
    double maxAverageRatio(vector<vector<int>>& classes, int extraStudents) {
        priority_queue<pair<double, int>> pq; //key通过率增量, value班级索引
        for (int i = 0, n = classes.size(); i < n; ++i) //预处理每个班级,添加一个通过率增量
            pq.push({1.0 * (classes[i][0] + 1) / (classes[i][1] + 1) 
                - 1.0 * classes[i][0] / classes[i][1], i});
        while (extraStudents) { // 贪心的添加extraStudents个学生
            pair<double, int> p = pq.top(); pq.pop(); // 得到最大通过率增量班级
            ++classes[p.second][0], ++classes[p.second][1]; // 该班级添加一个extraStudent
            --extraStudents;
            p.first = 1.0 * (classes[p.second][0] + 1) / (classes[p.second][1] + 1) - 1.0 * classes[p.second][0] / classes[p.second][1];
            pq.push(p); // 上面是新的最大通过率增量
        }
        double ans = 0;
        for (const vector<int>& cls : classes) ans += 1.0 * cls[0] / cls[1];
        return ans / classes.size(); // 所有班级平均通过率
    }
};
```

---
### 解法2 中位数分治
下面这种做法能达到 $O(n + m)$ 时间复杂度。用中位数分治思想可以解决的类似题目：[1648. 销售价值减少的颜色球](https://leetcode-cn.com/problems/sell-diminishing-valued-colored-balls/)，[1833. 雪糕的最大数量](https://leetcode-cn.com/problems/maximum-ice-cream-bars/)，[2141. 同时运行 N 台电脑的最长时间](https://leetcode-cn.com/problems/maximum-running-time-of-n-computers/)，[2187. 完成旅途的最少时间](https://leetcode-cn.com/problems/minimum-time-to-complete-trips/)，以及[2208. 将数组和减半的最少操作次数](https://leetcode.cn/problems/minimum-operations-to-halve-array-sum/)。


考虑一个一个加人，考虑每次加人时的通过率变化序列，问题就变成了在每个序列开头取若干个元素，求元素和最大值，由于每个通过率序列都是单调递减的，问题本质上变成了 [23. 合并K个升序链表](https://leetcode.cn/problems/merge-k-sorted-lists/)。另外补充一个 O(n+m) 复杂度的中位数分治解法，思路见 [这里](https://leetcode.cn/problems/minimum-operations-to-halve-array-sum/solution/onsuan-fa-by-hqztrue-jalf/)。
```cpp
class Solution {
public:
    struct Ratio {
        int num, den;
        operator double() const {
            return 1.0 * num / den;
        }
        bool operator<(const Ratio& other) const {
            return 1ll * (den - num) * other.den * (other.den + 1) < 1ll * den * (den + 1) * (other.den - other.num);
        }
    };

    double maxAverageRatio(const vector<vector<int>>& classes, int extraStudents) {
        const int n = classes.size();
        vector<Ratio> ratios;
        double l = 0.0;
        for (const auto& e : classes) {
            const int a = e[0], b = e[1];
            if (a == b) continue;
            ratios.push_back({a, b});
            l = max(l, (b - a) / (1.0 * (b + extraStudents) * (b + extraStudents + 1)));
        }
        const auto ptr = ratios.data();
        const int m = ratios.size();
        if (m == 0) return 1.0;
        int k = extraStudents;
        int p = 0;
        while (k > 0) {
            const int q = (p + m) / 2;
            nth_element(ptr + p, ptr + q, ptr + m);
            const double val = 1.0 * (ptr[q].den + 1) * ptr[q].den / (ptr[q].den - ptr[q].num);
            int cnt = 0;
            for (int i = q;i < m;++i)
                cnt += floor(sqrt((ptr[i].den - ptr[i].num) * val + 0.25) - ptr[i].den + 0.5 + 1e-10);
            if (cnt > k) {
                p = q;
                l = 1 / val;
            } else if (10 * cnt > k) {
                k -= cnt;
                for (int i = q;i < m;++i) {
                    const int d = floor(sqrt((ptr[i].den - ptr[i].num) * val + 0.25) - ptr[i].den + 0.5 + 1e-10);
                    ptr[i].num += d;
                    ptr[i].den += d;
                }
            } else {
                double r = 1 / val;
                while (r - l > 1e-10) {
                    const double mid = (l + r) / 2;
                    cnt = 0;
                    for (int i = p;i < m;++i)
                        cnt += max(int(floor(sqrt((ptr[i].den - ptr[i].num) / mid + 0.25) - ptr[i].den + 0.5 + 1e-10)), 0);
                    (cnt > k ? l : r) = mid;
                }
                for (int i = p;i < m && k > 0;++i) {
                    const int d = max(min(int(floor(sqrt((ptr[i].den - ptr[i].num) / l + 0.25) - ptr[i].den + 0.5 + 1e-10)), k), 0);
                    ptr[i].num += d;
                    ptr[i].den += d;
                    k -= d;
                }
            }
        }
        return (reduce(ptr, ptr + m, 0.0) + n - m) / n;
    }
};
```
这个代码是中位数分治的实现，复杂度 O(n+m)。