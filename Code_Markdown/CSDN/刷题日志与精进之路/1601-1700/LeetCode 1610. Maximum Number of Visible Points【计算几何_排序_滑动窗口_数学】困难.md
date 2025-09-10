> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>You are given an array <code>points</code>, an integer <code>angle</code>, and your <code>location</code>, where <code>location = [pos<sub>x</sub>, pos<sub>y</sub>]</code> and <code>points[i] = [x<sub>i</sub>, y<sub>i</sub>]</code> both denote <strong>integral coordinates</strong> on the X-Y plane.</p>

<p>Initially, you are facing directly east from your position. You <strong>cannot move</strong> from your position, but you can <strong>rotate</strong>. In other words, <code>pos<sub>x</sub></code> and <code>pos<sub>y</sub></code> cannot be changed. Your field of view in <strong>degrees</strong> is represented by <code>angle</code>, determining how wide you can see from any given view direction. Let <code>d</code> be the amount in degrees that you rotate counterclockwise. Then, your field of view is the <strong>inclusive</strong> range of angles <code>[d - angle/2, d + angle/2]</code>.</p>

<p>You can <strong>see</strong> some set of points if, for each point, the <strong>angle</strong> formed by the point, your position, and the immediate east direction from your position is <strong>in your field of view</strong>.</p>

<p>There can be multiple points at one coordinate. There may be points at your location, and you can always see these points regardless of your rotation. Points do not obstruct your vision to other points.</p>

<p>Return <em>the maximum number of points you can see</em>.</p>
 
<p><strong>Example 1:</strong></p>
<img style="width: 400px; height: 300px;" src="https://assets.leetcode.com/uploads/2020/09/30/89a07e9b-00ab-4967-976a-c723b2aa8656.png" alt="">
 
```cpp
Input: points = [[2,1],[2,2],[3,3]], angle = 90, location = [1,1]
Output: 3
Explanation: The shaded region represents your field of view. All points can be made visible in your field of view, including [3,3] even though [2,2] is in front and in the same line of sight.
```
<p><strong>Example 2:</strong></p>

```cpp
Input: points = [[2,1],[2,2],[3,4],[1,1]], angle = 90, location = [1,1]
Output: 4
Explanation: All points can be made visible in your field of view, including the one at your location.
```
<p><strong>Example 3:</strong></p>
<img style="width: 690px; height: 348px;" src="https://assets.leetcode.com/uploads/2020/09/30/5010bfd3-86e6-465f-ac64-e9df941d2e49.png" alt="">

```cpp
Input: points = [[1,0],[2,1]], angle = 13, location = [1,1]
Output: 1
Explanation: You can only see one of the two points, as shown above.
```
<p><strong>Constraints:</strong></p>
<ul>
	<li><code>1 &lt;= points.length &lt;= 10<sup>5</sup></code></li>
	<li><code>points[i].length == 2</code></li>
	<li><code>location.length == 2</code></li>
	<li><code>0 &lt;= angle &lt; 360</code></li>
	<li><code>0 &lt;= pos<sub>x</sub>, pos<sub>y</sub>, x<sub>i</sub>, y<sub>i</sub> &lt;= 100</code></li>
</ul>

题意：给你一个点数组 `points` 和一个表示角度的整数 `angle` ，你的位置是 `location` ，其中 `location = [posx, posy]` 且 `points[i] = [xi, yi]` 都表示 X-Y 平面上的整数坐标。

最开始，你面向东方进行观测。你 **不能** 进行移动改变位置，但可以通过 **自转** 调整观测角度。换句话说，`posx` 和 `posy` 不能改变。你的视野范围的角度用 `angle` 表示， 这决定了你观测任意方向时可以多宽。设 `d` 为你逆时针自转旋转的度数，那么你的视野就是角度范围 `[d - angle/2, d + angle/2]` 所指示的那片区域。 
 
对于每个点，如果由该点、你的位置以及从你的位置直接向东的方向形成的角度 **位于你的视野中** ，那么你就可以看到它。

同一个坐标上可以有多个点。你所在的位置也可能存在一些点，但不管你怎么旋转，总是可以看到这些点。同时，点不会阻碍你看到其他点。

返回你能看到的点的最大数目。

---
### 解法 数学+极角排序+（双指针）滑动窗口
这道题思维量比较大，且细节相当繁琐。题意中要求逆时针旋转一个「无限延伸、角度为 `angle` 的覆盖面」，要让该覆盖面所能覆盖的 `points[]` 中的点最多。设 `d` 为逆时针旋转的度数，角度范围就是 `[d - angle/2, d + angle/2]` 指示的那片区域。

==由于我们的位置在 `location = [posx, posy]` ，且逆时针旋转时以 `location` 为原点，我们可以将 `[posx, posy]` 作为「极点」，求出所有 `points[i]` 的「极角」==——即遵照[算法学习笔记 极角排序](https://memcpy0.blog.csdn.net/article/details/122006212)中的方法，计算出 `points` 数组中所有点相对于 `location` 的极角（[**弧度**](https://en.wikipedia.org/wiki/Radian)形式），得到极角数组 `polars` ，再对极角数组由小到大排序（极角按照第三、第四、第一、第二象限的顺序排列）。

此时，问题初步转换为——如何在极角数组 `polars` 中找到最长的连续一段 `[i, j]` ，使得 `polars[i]` 和 `polars[j]` 的角度差不超过 `angle` 。需要注意的是，**直接在原数组 `polars` 上操作，会漏掉夹角横跨第二和第三象限的情况**。

因此，本题的关键细节是，==极角排序后要复制一份放到极角数组后，复制的这一部分的极角要加上 $2\pi$（确保数组仍具有单调性），以保证不漏掉解==。具体的，假设极角数组长度为 `n` ，此时 `polars[n + i] = polars[i] + 2π` ，从而彻底将问题转换为求「最长合法子数组」问题——可用双指针实现滑动窗口来解决。

其他细节是，==题目规定了与 `location` 重合的点可在任意角度看到，因此我们需要对这些点特殊处理==。
```cpp
//C++ version
class Solution {
private:
    using Point = vector<int>;
    const double pi = acos(-1.0), eps = 1e-8;
public:
    int visiblePoints(vector<vector<int>>& points, int angle, vector<int>& location) {
        int x = location[0], y = location[1];
        int onLocal = 0; // 和location重合的点
        vector<double> polars;
        for (const Point &p : points) {
            if (p[0] == x && p[1] == y) { ++onLocal; continue; } // 这些点一定能看到
            polars.push_back(atan2(p[1] - y, p[0] - x));
        }
        sort(polars.begin(), polars.end(), [&](const double &a, const double &b) {
            return a - b < -eps; // 极角从小到大排序
        });
        for (int i = 0, n = polars.size(); i < n; ++i)
            polars.push_back(polars[i] + 2 * pi); // 避免漏掉解
        double radian = angle * pi / 180;
        int maxlen = 0;
        int l = 0, r = 0, n = polars.size();
        while (true) { // 滑动窗口
            while (r < n && polars[r] - polars[l] <= radian) ++r; // 窗口扩张
            maxlen = max(maxlen, r - l);
            if (r >= n) break;
            while (l < r && polars[r] - polars[l] > radian) ++l; // 窗口收缩
        }
        return maxlen + onLocal;
    }
};
//执行用时：440 ms, 在所有 C++ 提交中击败了67.02% 的用户
//内存消耗：159.7 MB, 在所有 C++ 提交中击败了38.24% 的用户
```
