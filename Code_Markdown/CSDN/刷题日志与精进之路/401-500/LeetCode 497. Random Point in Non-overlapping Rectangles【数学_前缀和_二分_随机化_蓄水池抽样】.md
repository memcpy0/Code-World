> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>You are given an array of non-overlapping axis-aligned rectangles <code>rects</code> where <code>rects[i] = [a<sub>i</sub>, b<sub>i</sub>, x<sub>i</sub>, y<sub>i</sub>]</code> indicates that <code>(a<sub>i</sub>, b<sub>i</sub>)</code> is the bottom-left corner point of the <code>i<sup>th</sup></code> rectangle and <code>(x<sub>i</sub>, y<sub>i</sub>)</code> is the top-right corner point of the <code>i<sup>th</sup></code> rectangle. Design an algorithm to pick a random integer point inside the space covered by one of the given rectangles. A point on the perimeter of a rectangle is included in the space covered by the rectangle.</p>

<p>Any integer point inside the space covered by one of the given rectangles should be equally likely to be returned.</p>

<p><strong>Note</strong> that an integer point is a point that has integer coordinates.</p>

<p>Implement the <code>Solution</code> class:</p>

<ul>
	<li><code>Solution(int[][] rects)</code> Initializes the object with the given rectangles <code>rects</code>.</li>
	<li><code>int[] pick()</code> Returns a random integer point <code>[u, v]</code> inside the space covered by one of the given rectangles.</li>
</ul>
 
<p><strong>Example 1:</strong></p>
<img style="width: 419px; height: 539px;" src="https://assets.leetcode.com/uploads/2021/07/24/lc-pickrandomrec.jpg" alt="">

```haskell
Input
["Solution", "pick", "pick", "pick", "pick", "pick"]
[[[[-2, -2, 1, 1], [2, 2, 4, 6]]], [], [], [], [], []]
Output
[null, [1, -2], [1, -1], [-1, -2], [-2, -2], [0, 0]]

Explanation
Solution solution = new Solution([[-2, -2, 1, 1], [2, 2, 4, 6]]);
solution.pick(); // return [1, -2]
solution.pick(); // return [1, -1]
solution.pick(); // return [-1, -2]
solution.pick(); // return [-2, -2]
solution.pick(); // return [0, 0] 
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= rects.length &lt;= 100</code></li>
	<li><code>rects[i].length == 4</code></li>
	<li><code>-10<sup>9</sup> &lt;= a<sub>i</sub> &lt; x<sub>i</sub> &lt;= 10<sup>9</sup></code></li>
	<li><code>-10<sup>9</sup> &lt;= b<sub>i</sub> &lt; y<sub>i</sub> &lt;= 10<sup>9</sup></code></li>
	<li><code>x<sub>i</sub> - a<sub>i</sub> &lt;= 2000</code></li>
	<li><code>y<sub>i</sub> - b<sub>i</sub> &lt;= 2000</code></li>
	<li>All the rectangles do not overlap.</li>
	<li>At most <code>10<sup>4</sup></code> calls will be made to <code>pick</code>.</li>
</ul>



题意：给定一个非重叠轴对齐矩形的列表 <code>rects</code>，写一个函数 <code>pick</code> 随机均匀地选取矩形覆盖的空间中的整数点。要注意的是：
<ol>
	<li><strong>整数点</strong>是具有整数坐标的点。</li>
	<li>矩形周边上的点包含在矩形覆盖的空间中。</li>
	<li>第 <code>i</code> 个矩形 <code>rects [i] = [x1，y1，x2，y2]</code>，其中&nbsp;<code>[x1，y1]</code> 是左下角的整数坐标，<code>[x2，y2]</code> 是右上角的整数坐标。</li>
	<li>每个矩形的长度和宽度不超过 2000。</li>
	<li><code>1 &lt;= rects.length&nbsp;&lt;= 100</code></li>
	<li><code>pick</code> 以整数坐标数组&nbsp;<code>[p_x, p_y]</code>&nbsp;的形式返回一个点。</li>
	<li><code>pick</code> 最多被调用10000次。</li>
</ol>
 
---
### 解法 前缀和/二分/随机化 ~~/蓄水池抽样~~
一个想法是，遍历所有矩形，内部遍历矩形中所有整数点，使用蓄水池抽样算法，随机均匀选取整数点。但是由于数据范围太大，整个算法可能TLE。

我从[528. Random Pick with Weight](https://leetcode-cn.com/problems/random-pick-with-weight/)受到启发，由于非重叠矩形的面积不等，选择某个矩形的实际权重也是不等的。因此，本题可以用前缀和逐个加总各个矩形内的整数点数目，得到整数点总数为 `Points(rects)` 。第 `i` 个矩形被选中的概率则是 `Points(rects[i]) / Points(rects)` ，或者说做出 `Points(rects)` 次选择后、第 `i` 个矩形被选中的次数大概是 `Points(rects[i])` 。这不就和528题差不多了吗？

接着产生一个 `[1, Points(rects)]` 范围的随机整数 `t` ，使用二分在前缀和数组中查找第一个大于等于 `t` 的元素的下标（假设为 `i`），我们要选取的就是第 `i` 个矩形。

随后在这个矩形内部，使用蓄水池抽样算法随机选取整数点。计算可知，整数点被选取的概率为 `(Points(rects[i]) / Points(rects)) * (1 / Points(rects[i]))` ，恰好就是 `1 / Points(rects)` ，满足随机均匀的题目要求。假设 `n = rects.length` ，第 `i` 个矩形内的整数点为 <code>p<sub>i</sub></code>，整个算法中 `Solution` 构造方法的时间复杂度为 $O(n)$ ，`pick` 方法的时间复杂度为 $O(p_i \times \log n)$ ，总体空间复杂度为 $O(n)$（不存储 `rects` 列表）。具体代码如下所示：
```cpp
//C++ version
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);
class Solution {
private:
    vector<int> s;
    vector<vector<int>>::iterator begin;
    uniform_int_distribution<int> ud, ud2; //ud用于选取矩形,ud2用于选取整数点
public:
    Solution(vector<vector<int>>& rects) {
        begin = rects.begin();
        s.push_back(0);
        for (int i = 0, n = rects.size(); i < n; ++i) {
            int ph = rects[i][2] - rects[i][0] + 1;
            int pw = rects[i][3] - rects[i][1] + 1;
            s.push_back(s.back() + (ph * pw)); //计算前缀和
        }
        ud.param(uniform_int_distribution<>::param_type{1, s.back()});
    }
    vector<int> pick() {
        int l = 1, r = s.size(), t = ud(generator);
        while (l < r) {
            int mid = l + r >> 1;
            if (s[mid] >= t) r = mid;
            else l = mid + 1;
        }
        const vector<int>& rect = *(begin + l - 1); //选取矩形
        int cnt = 0;
        vector<int> ans;
        for (int i = rect[0]; i <= rect[2]; ++i) { //蓄水池抽样
            for (int j = rect[1]; j <= rect[3]; ++j) {
                ++cnt;
                ud2.param(uniform_int_distribution<>::param_type{1, cnt});
                if (ud2(generator) == cnt) ans = vector<int>{i, j}; //概率为1/cnt
            }
        }
        return ans;
    }
};
```
emmm……看起来不错，只是不要忘记，`pick` 方法最多会被调用10000次，所以以上的做法也会TLE。这就是**有一个锤子、看什么都是钉子**，学了蓄水池抽样，就什么都想着用这个算法。

既然我们根据权重随机选取到了某个矩形，不就可以**再随机选择其中一个点**吗？现在 `Solution` 构造方法的时间复杂度为 $O(n)$ ，`pick` 方法的时间复杂度为 $O(\log n)$ ，总体空间复杂度为 $O(n)$（不存储 `rects` 列表）：
```cpp
//C++ version
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);
class Solution {
private:
    vector<int> s;
    vector<vector<int>>::iterator begin;
    uniform_int_distribution<int> ud, ud2; //ud用于选取矩形,ud2用于选取整数点
public:
    Solution(vector<vector<int>>& rects) {
        begin = rects.begin();
        s.push_back(0);
        for (int i = 0, n = rects.size(); i < n; ++i) {
            int ph = rects[i][2] - rects[i][0] + 1;
            int pw = rects[i][3] - rects[i][1] + 1;
            s.push_back(s.back() + (ph * pw)); //计算前缀和
        }
        ud.param(uniform_int_distribution<>::param_type{1, s.back()});
    }
    vector<int> pick() {
        int l = 1, r = s.size(), t = ud(generator);
        while (l < r) {
            int mid = l + r >> 1;
            if (s[mid] >= t) r = mid;
            else l = mid + 1;
        }
        const vector<int>& rect = *(begin + l - 1); //选取矩形
        vector<int> ans; 
        ud2.param(uniform_int_distribution<>::param_type{rect[0], rect[2]}); 
        ans.push_back(ud2(generator));
        ud2.param(uniform_int_distribution<>::param_type{rect[1], rect[3]});
        ans.push_back(ud2(generator));
        return ans;
    }
};
//执行用时：84 ms, 在所有 C++ 提交中击败了64.47% 的用户
//内存消耗：66.8 MB, 在所有 C++ 提交中击败了15.79% 的用户
```
不过这还不算是最优解，我们随机选取到某个矩形使用的那个 `[1, Points(rects)]` 范围内的随机整数 `t` ，**其实就能够代表该矩形中的某个随机整数点**！再根据该矩形的列数来生成随机坐标，即可解决本题：
```cpp
//C++ version
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);
class Solution {
private:
    vector<int> s;
    vector<vector<int>>::iterator begin;
    uniform_int_distribution<int> ud; //ud用于选取矩形
public:
    Solution(vector<vector<int>>& rects) {
        begin = rects.begin();
        s.push_back(0);
        for (const vector<int>& r : rects) //计算前缀和
            s.push_back(s.back() + (r[2] - r[0] + 1) * (r[3] - r[1] + 1));
        ud.param(uniform_int_distribution<>::param_type{1, s.back()});
    }
    vector<int> pick() {
        int l = 1, r = s.size(), t = ud(generator);
        while (l < r) { //找到第一个大于等于t的s[l]
            int mid = l + r >> 1;
            if (s[mid] >= t) r = mid;
            else l = mid + 1;
        }
        const vector<int>& rect = *(begin + l - 1); //选取矩形
        int k = s[l] - t; //随机数t代表该矩形中的第k个整数点
        int col = rect[3] - rect[1] + 1;
        return vector<int>{rect[0] + k / col, rect[1] + k % col}; //从左下开始
    }
};
//执行用时：72 ms, 在所有 C++ 提交中击败了88.82% 的用户
//内存消耗：65.3 MB, 在所有 C++ 提交中击败了72.37% 的用户
```
因此，本题实际上只是[528. Random Pick with Weight](https://leetcode-cn.com/problems/random-pick-with-weight/)的一个变形，没有用到蓄水池抽样算法。
