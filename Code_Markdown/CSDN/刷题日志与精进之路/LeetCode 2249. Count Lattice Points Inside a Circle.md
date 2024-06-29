> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个二维整数数组 `circles` ，其中 `circles[i] = [xi, yi, ri]` 表示网格上圆心为 `(xi, yi)` 且半径为 `ri` 的第 `i` 个圆，返回出现在 **至少一个** 圆内的 **格点数目** 。

**注意：**

- **格点** 是指整数坐标对应的点。
- **圆周上的点** 也被视为出现在圆内的点。

**示例 1：**

![](https://assets.leetcode.com/uploads/2022/03/02/exa-11.png)

**输入：**circles = [[2,2,1]]
**输出：**5
**解释：**
给定的圆如上图所示。
出现在圆内的格点为 (1, 2)、(2, 1)、(2, 2)、(2, 3) 和 (3, 2)，在图中用绿色标识。
像 (1, 1) 和 (1, 3) 这样用红色标识的点，并未出现在圆内。
因此，出现在至少一个圆内的格点数目是 5 。

**示例 2：**

![](https://assets.leetcode.com/uploads/2022/03/02/exa-22.png)

**输入：**circles = [[2,2,2],[3,4,1]]
**输出：**16
**解释：**
给定的圆如上图所示。
共有 16 个格点出现在至少一个圆内。
其中部分点的坐标是 (0, 2)、(2, 0)、(2, 4)、(3, 2) 和 (4, 4) 。

**提示：**

- `1 <= circles.length <= 200`
- `circles[i].length == 3`
- `1 <= xi, yi <= 100`
- `1 <= ri <= min(xi, yi)`

https://leetcode.cn/problems/count-lattice-points-inside-a-circle/description/


```cpp
class Solution { 
    typedef pair<int, int> pii;
public:
    int countLatticePoints(vector<vector<int>>& circles) {   
        set<pii> rec;
        for (vector<int>& circle : circles) {
            int x = circle[0], y = circle[1], r = circle[2];
            for (int i = x - r; i <= x + r; ++i) {
                for (int j = y - r; j <= y + r; ++j) {
                    if (rec.find(pii(i, j)) != rec.end()) continue;
                    if ((i - x) * (i - x) + (j - y) * (j - y) <= r * r) {
                        rec.insert(pii(i, j));
                    }
                }
            }
        }
        return rec.size();
    }
};
```