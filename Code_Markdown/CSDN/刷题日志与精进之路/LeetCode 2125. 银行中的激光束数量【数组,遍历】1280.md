> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

银行内部的防盗安全装置已经激活。给你一个下标从 **0** 开始的二进制字符串数组 `bank` ，表示银行的平面图，这是一个大小为 `m x n` 的二维矩阵。 `bank[i]` 表示第 `i` 行的设备分布，由若干 `'0'` 和若干 `'1'` 组成。`'0'` 表示单元格是空的，而 `'1'` 表示单元格有一个安全设备。

对任意两个安全设备而言，**如果****同时** 满足下面两个条件，则二者之间存在 **一个** 激光束：
- 两个设备位于两个 **不同行** ：`r1` 和 `r2` ，其中 `r1 < r2` 。
- 满足 `r1 < i < r2` 的 **所有** 行 `i` ，都 **没有安全设备** 。

激光束是独立的，也就是说，一个激光束既不会干扰另一个激光束，也不会与另一个激光束合并成一束。

返回银行中激光束的总数量。

**示例 1：**
![](https://assets.leetcode.com/uploads/2021/12/24/laser1.jpg)
```java
输入：bank = ["011001","000000","010100","001000"]
输出：8
解释：在下面每组设备对之间，存在一条激光束。总共是 8 条激光束：
 * bank[0][1] -- bank[2][1]
 * bank[0][1] -- bank[2][3]
 * bank[0][2] -- bank[2][1]
 * bank[0][2] -- bank[2][3]
 * bank[0][5] -- bank[2][1]
 * bank[0][5] -- bank[2][3]
 * bank[2][1] -- bank[3][2]
 * bank[2][3] -- bank[3][2]
注意，第 0 行和第 3 行上的设备之间不存在激光束。
这是因为第 2 行存在安全设备，这不满足第 2 个条件。
```
**示例 2：**
![](https://assets.leetcode.com/uploads/2021/12/24/laser2.jpg)
```java
输入：bank = ["000","111","000"]
输出：0
解释：不存在两个位于不同行的设备
```
**提示：**
- `m == bank.length`
- `n == bank[i].length`
- `1 <= m, n <= 500`
- `bank[i][j]` 为 `'0'` 或 `'1'`

---
### 解法 数组+遍历
根据题目的要求，对于两个不同的行 $r_1$ 和 $r_2~(r_1 < r_2)$ ，如果它们恰好是相邻的两行（即 $r_1 + 1 = r_2$ ），或它们之间的所有行都全为 $0$ ，那么第 $r_1$ 行的任意一个安全设备与第 $r_2$ 行的任意一个安全设备之间都有激光束。

因此，我们只需要统计每一行的安全设备个数，记为 $\textit{next}$ ，以及**上一个**不全为 $0$ 的行的安全设备个数，记为 $pre$ 。那么 $\textit{pre} \times \textit{next}$ 即为激光束的个数。**遍历所有的行，维护 $pre$ 和 $next$ 并对 $pre \times next$ 进行累加**，即可得到激光束的总数量。
```cpp
// cpp
class Solution {
public:
    int numberOfBeams(vector<string>& bank) {
        int pre = 0, ans = 0;
        for (const string& line : bank) {
            int next = count_if(line.begin(), line.end(), [](char ch) { return ch == '1'; });
            if (next) {
                ans += next * pre;
                pre = next;
            }
        }
        return ans;
    }
};
// java
class Solution {
    public int numberOfBeams(String[] bank) {
        int pre = 0, ans = 0;
        for (String line : bank) {
            int next = 0;
            for (char c : line.toCharArray()) if (c == '1') ++next;
            if (next != 0) {
                ans += pre * next;
                pre = next;
            }
        }
        return ans;
    }
}
// python
class Solution:
    def numberOfBeams(self, bank: List[str]) -> int:
        pre = ans = 0
        for line in bank:
            next = line.count("1")
            if next != 0:
                ans += pre * next
                pre = next
        return ans
// go
func numberOfBeams(bank []string) (ans int) {
    pre := 0
    for _, row := range bank {
        next := strings.Count(row, "1")
        if next != 0 {
            ans += pre * next
            pre = next
        }
    }
    return
}
```
**复杂度分析**：
- 时间复杂度：$O(mn)$ 。
- 空间复杂度：$O(1)$ 。