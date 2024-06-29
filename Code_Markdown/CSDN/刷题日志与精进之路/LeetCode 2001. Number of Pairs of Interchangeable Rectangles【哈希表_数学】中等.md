> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>You are given <code>n</code> rectangles represented by a <strong>0-indexed</strong> 2D integer array <code>rectangles</code>, where <code>rectangles[i] = [width<sub>i</sub>, height<sub>i</sub>]</code> denotes the width and height of the <code>i<sup>th</sup></code> rectangle.</p>

<p>Two rectangles <code>i</code> and <code>j</code> (<code>i &lt; j</code>) are considered <strong>interchangeable</strong> if they have the <strong>same</strong> width-to-height ratio. More formally, two rectangles are <strong>interchangeable</strong> if <code>width<sub>i</sub>/height<sub>i</sub> == width<sub>j</sub>/height<sub>j</sub></code> (using decimal division, not integer division).</p>

<p>Return <em>the <strong>number</strong> of pairs of <strong>interchangeable</strong> rectangles in </em><code>rectangles</code>.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: rectangles = [[4,8],[3,6],[10,20],[15,30]]
Output: 6
Explanation: The following are the interchangeable pairs of rectangles by index (0-indexed):
- Rectangle 0 with rectangle 1: 4/8 == 3/6.
- Rectangle 0 with rectangle 2: 4/8 == 10/20.
- Rectangle 0 with rectangle 3: 4/8 == 15/30.
- Rectangle 1 with rectangle 2: 3/6 == 10/20.
- Rectangle 1 with rectangle 3: 3/6 == 15/30.
- Rectangle 2 with rectangle 3: 10/20 == 15/30.
```

<p><strong>Example 2:</strong></p>

```cpp
Input: rectangles = [[4,5],[7,8]]
Output: 0
Explanation: There are no interchangeable pairs of rectangles.
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>n == rectangles.length</code></li>
	<li><code>1 &lt;= n &lt;= 10<sup>5</sup></code></li>
	<li><code>rectangles[i].length == 2</code></li>
	<li><code>1 &lt;= width<sub>i</sub>, height<sub>i</sub> &lt;= 10<sup>5</sup></code></li>
</ul>

题意：用一个下标从 <strong>0</strong> 开始的二维整数数组&nbsp;<code>rectangles</code> 来表示 <code>n</code> 个矩形，其中 <code>rectangles[i] = [width<sub>i</sub>, height<sub>i</sub>]</code> 表示第 <code>i</code> 个矩形的宽度和高度。

<p>如果两个矩形 <code>i</code> 和 <code>j</code>（<code>i &lt; j</code>）的宽高比相同，则认为这两个矩形 <strong>可互换</strong> 。更规范的说法是，两个矩形满足&nbsp;<code>width<sub>i</sub>/height<sub>i</sub> == width<sub>j</sub>/height<sub>j</sub></code>（使用实数除法而非整数除法），则认为这两个矩形 <strong>可互换</strong> 。</p>

<p>计算并返回&nbsp;<code>rectangles</code> 中有多少对 <strong>可互换 </strong>矩形。</p>
 
---
### 解法 哈希表+数学
在竞赛中本人的写法，重写了 `pair<long long, long long>` 的哈希函数和判等函数，保存有理数并用哈希表计数，这样就不会有精度问题。由于使用 $C^2_n$ 计算**可互换矩形**的个数，可能整型溢出，要用 `long long` 来计算。
```cpp
//C++ version
class Solution {
private:
    using Tuple = pair<long long, long long>;
    int gcd(int a, int b) { return !b ? a : gcd(b, a % b); }
    struct TupleHash {
        std::size_t operator()(const Tuple& k) const {
            return k.first * 1e5 + k.second;
        }
    };
    struct TupleEqual {
        bool operator()(const Tuple& lhs, const Tuple& rhs) const {
            return lhs.first == rhs.first && lhs.second == rhs.second;
        }
    };
public:
    long long interchangeableRectangles(vector<vector<int>>& rectangles) {
        unordered_map<Tuple, long long, TupleHash, TupleEqual> rec;
        for (const vector<int>& rect : rectangles) {
            int g = gcd(rect[0], rect[1]);
            ++rec[pair{rect[0] / g, rect[1] / g}];
        }
        long long ans = 0;
        for (const auto& it : rec) {
            long long num = it.second;
            ans += num * (num - 1) / 2;
        }
        return ans;
    }
};
//执行用时：416 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：141.7 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
由于 <code>1 &lt;= width<sub>i</sub>, height<sub>i</sub> &lt;= 10<sup>5</sup></code>，我们可以计算 <code>width<sub>i</sub> / height<sub>i</sub></code> ，再来计算有多少个矩形满足 <code>width<sub>i</sub> / height<sub>i</sub> = width<sub>j</sub> / height<sub>j</sub></code> 。由于精度问题，难以用哈希表完成计数，不过也可以脑洞大开——先将除法结果存储为一个数组，然后从小到大排序，再进行差分，如果差分值小于 `1e-8` ，就认为前后两个值相等：
```cpp
//C++ version
class Solution {
public:
    double d = 1e-8;
    long long interchangeableRectangles(vector<vector<int>>& rectangles) {
        int n = rectangles.size();
        long long ans = 0, cnt = 1;
        vector<double> diff(n);
        for (int i = 0; i < n; ++i) //相除并直接保存
            diff[i] = (double)rectangles[i][0] / (double)rectangles[i][1];
        sort(diff.begin(), diff.end());
        for (int i = n - 1; i > 0; --i) diff[i] -= diff[i - 1]; //差分
        for (int i = 1; i < n; ++i) {
            if (diff[i] < d) {
                ++cnt;
                continue;
            }
            ans += (cnt * (cnt - 1)) / 2;
            cnt = 1;
        }
        ans += (cnt * (cnt - 1)) / 2;
        return ans;
    }
};
//执行用时：340 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：117.6 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
如果不用 `unordered_map` ，可以使用 `map` 来简化代码：
```cpp
//C++ version
#define mp make_pair
typedef long long LL;
typedef pair<int, int> PII;
class Solution {
public:
    long long interchangeableRectangles(vector<vector<int>>& r) {
        LL ans = 0;
        map<PII, int> rec;
        for (const vector<int>& e : r) {
            int a = e[0], b = e[1], g = __gcd(a, b);
            a /= g, b /= g;
            PII t = mp(a, b);
            ans += rec[t]; //可以和矩形e互换的矩形
            ++rec[t];
        }
        return ans;
    }
};
//执行用时：600 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：134.6 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
