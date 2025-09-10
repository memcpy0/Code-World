> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>You are given a stream of points on the X-Y plane. Design an algorithm that:</p>

<ul>
	<li><strong>Adds</strong> new points from the stream into a data structure. <strong>Duplicate</strong> points are allowed and should be treated as different points.</li>
	<li>Given a query point, <strong>counts</strong> the number of ways to choose three points from the data structure such that the three points and the query point form an <strong>axis-aligned square</strong> with <strong>positive area</strong>.</li>
</ul>

<p>An <strong>axis-aligned square</strong> is a square whose edges are all the same length and are either parallel or perpendicular to the x-axis and y-axis.</p>

<p>Implement the <code>DetectSquares</code> class:</p>

<ul>
	<li><code>DetectSquares()</code> Initializes the object with an empty data structure.</li>
	<li><code>void add(int[] point)</code> Adds a new point <code>point = [x, y]</code> to the data structure.</li>
	<li><code>int count(int[] point)</code> Counts the number of ways to form <strong>axis-aligned squares</strong> with point <code>point = [x, y]</code> as described above.</li>
</ul>

 
<p><strong>Example 1:</strong></p>
<img style="width: 869px; height: 504px;" src="https://assets.leetcode.com/uploads/2021/09/01/image.png" alt="">

<p><strong>Constraints:</strong></p>
<ul>
	<li><code>point.length == 2</code></li>
	<li><code>0 &lt;= x, y &lt;= 1000</code></li>
	<li>At most <code>3000</code> calls <strong>in total</strong> will be made to <code>add</code> and <code>count</code>.</li>
</ul>
 
题意：给你一个在 X-Y 平面上的点构成的数据流。设计一个 <code>DetectSquares</code> 类：</p>：
<ul>
	<li><code>DetectSquares()</code> 使用空数据结构初始化对象。</li>
	<li><code>void add(int[] point)</code> 向数据结构添加一个在数据流中的新点 <code>point = [x, y]</code> 。</strong>可以添加 <strong>重复</strong> 的点，并会视作不同的点进行处理。</li>
	<li><code>int count(int[] point)</code>  给你一个查询点，从数据结构中选出三个点，使这三个点和查询点一同构成一个 <strong>面积为正</strong> 的 <strong>轴对齐正方形</strong> ，<strong>统计</strong> 满足该要求的方案数目<strong>。</strong></li>
</ul>

<p><strong>轴对齐正方形</strong> 是一个正方形，除四条边长度相同外，还满足每条边都与 x-轴 或 y-轴 平行或垂直。</p>

 
---
### 解法1 哈希表+枚举（四个方向的长度）
考虑到调用 <code>add</code> 和 <code>count</code> 的 <strong>总次数</strong> 最多为 <code>3000</code> ，即最多只添加 `3000` 个点，最多查询 `3000` 次。不难发现，**优化的关键在 `count` 方法**。如果每次调用 `count()` 我们都使用三重循环查询点集合，绝对会TLE，即使两重循环也太多了——`count` 方法的复杂度只能为 $O(n)$ 。

为此可以使用哈希表，在 `add` 方法中记录每个点出现的次数，时间复杂度为 $O(1)$ 。然后想方设法优化 `count` 方法——考虑到 `0 <= x, y <= 1000` ，（以查询点 `point` 为左上角、左下角、右上角、右下角的）正方形的长度是有限的。假设向左上、向左下、向右上、向右下的最大可能长度分别为 `lu, ld, ru, rd` ，我们用四个循环分别从长度 `1` **枚举**到 `lu, ld, ru, rd` ，循环内查询可能的其他三个点是否存在，存在则计数。从而，整个 `count` 方法的时间复杂度为 $O(C)\ (C = 4 \times 1000)$ 。

照此想法实现的代码，相当暴力，但是可能常数过大，提交后超时了：
```cpp
//C++ version
class DetectSquares {
private:
    using Key = pair<int, int>;
    struct KeyHash {
        size_t operator()(const Key& k) const {
            return k.first * 10000 + k.second;
        }
    };
    struct KeyEqual {
        bool operator()(const Key& lhs, const Key& rhs) const {
            return lhs.first == rhs.first && lhs.second == rhs.second;
        }
    };
    unordered_map<Key, int, KeyHash, KeyEqual> rec;
public:
    DetectSquares() {}
    
    void add(vector<int> point) { //O(1)
        ++rec[Key{point[0], point[1]}];
    }
    
    int count(vector<int> point) {
        int x = point[0], y = point[1];
        int lu = min(x, 1000 - y), ru = min(1000 - x, 1000 - y);
        int ld = min(x, y), rd = min(1000 - x, y);
        int ans = 0;
        for (int i = 1; i <= lu; ++i) {
            Key p1 = Key{x - i, y};
            Key p2 = Key{x - i, y + i};
            Key p3 = Key{x, y + i};
            ans += rec[p1] * rec[p2] * rec[p3];
        }
        for (int i = 1; i <= ru; ++i) {
            Key p1 = Key{x + i, y};
            Key p2 = Key{x + i, y + i};
            Key p3 = Key{x, y + i};
            ans += rec[p1] * rec[p2] * rec[p3];
        }
        for (int i = 1; i <= ld; ++i) {
            Key p1 = Key{x - i, y};
            Key p2 = Key{x - i, y - i};
            Key p3 = Key{x, y - i};
            ans += rec[p1] * rec[p2] * rec[p3];
        }
        for (int i = 1; i <= rd; ++i) {
            Key p1 = Key{x + i, y};
            Key p2 = Key{x + i, y - i};
            Key p3 = Key{x, y - i};
            ans += rec[p1] * rec[p2] * rec[p3];
        }
        return ans;
    }
}; 
```
痛定思痛，我优化了代码，使用 `bitset` 来查询对应的 `x, y` 值是否存在，不存在则跳过三个可能点的查询：
```cpp
//C++ version
class DetectSquares {
private:
    using Key = pair<int, int>;
    struct KeyHash {
        size_t operator()(const Key& k) const {
            return k.first * 10000 + k.second;
        }
    };
    struct KeyEqual {
        bool operator()(const Key& lhs, const Key& rhs) const {
            return lhs.first == rhs.first && lhs.second == rhs.second;
        }
    };
    unordered_map<Key, int, KeyHash, KeyEqual> rec;
    bitset<1010> xs, ys; //查询对应的x,y值是否存在
public:
    DetectSquares() {}
    
    void add(vector<int> point) { //O(1)
        ++rec[Key{point[0], point[1]}];
        xs[point[0]] = 1;
        ys[point[1]] = 1;
    }
    
    int count(vector<int> point) {
        int x = point[0], y = point[1];
        int lu = min(x, 1000 - y), ru = min(1000 - x, 1000 - y);
        int ld = min(x, y), rd = min(1000 - x, y);
        int ans = 0;
        for (int i = 1; i <= lu; ++i) {
            if (xs[x - i] == 0 || ys[y + i] == 0) continue;
            Key p1 = Key{x - i, y};
            Key p2 = Key{x - i, y + i};
            Key p3 = Key{x, y + i};
            ans += rec[p1] * rec[p2] * rec[p3];
        }
        for (int i = 1; i <= ru; ++i) {
            if (xs[x + i] == 0 || ys[y + i] == 0) continue;
            Key p1 = Key{x + i, y};
            Key p2 = Key{x + i, y + i};
            Key p3 = Key{x, y + i};
            ans += rec[p1] * rec[p2] * rec[p3];
        }
        for (int i = 1; i <= ld; ++i) {
            if (xs[x - i] == 0 || ys[y - i] == 0) continue;
            Key p1 = Key{x - i, y};
            Key p2 = Key{x - i, y - i};
            Key p3 = Key{x, y - i};
            ans += rec[p1] * rec[p2] * rec[p3];
        }
        for (int i = 1; i <= rd; ++i) {
            if (xs[x + i] == 0 || ys[y - i] == 0) continue;
            Key p1 = Key{x + i, y};
            Key p2 = Key{x + i, y - i};
            Key p3 = Key{x, y - i};
            ans += rec[p1] * rec[p2] * rec[p3];
        }
        return ans;
    }
}; 
//执行用时：360 ms, 在所有 C++ 提交中击败了66.85% 的用户
//内存消耗：105.6 MB, 在所有 C++ 提交中击败了39.71% 的用户
```
简化一下代码长度：
```cpp
//C++ version
class DetectSquares {
private:
    using Key = pair<int, int>;
    struct KeyHash {
        size_t operator()(const Key& k) const {
            return k.first * 10000 + k.second;
        }
    };
    struct KeyEqual {
        bool operator()(const Key& lhs, const Key& rhs) const {
            return lhs.first == rhs.first && lhs.second == rhs.second;
        }
    };
    unordered_map<Key, int, KeyHash, KeyEqual> rec;
    bitset<1010> xs, ys; //查询对应的x,y值是否存在
    int dir[4][2] = {{-1, 1}, {1, 1}, {-1, -1}, {1, -1}};
public:
    DetectSquares() {}
    void add(vector<int> point) { //O(1)
        ++rec[Key{point[0], point[1]}];
        xs[point[0]] = 1;
        ys[point[1]] = 1;
    }
    int count(vector<int> point) {
        int x = point[0], y = point[1];
        int lu = min(x, 1000 - y), ru = min(1000 - x, 1000 - y);
        int ld = min(x, y), rd = min(1000 - x, y);
        int lens[4] = {lu, ru, ld, rd};
        int ans = 0;
        for (int i = 0; i < 4; ++i) {
            for (int j = 1; j <= lens[i]; ++j) {
                int dx = x + dir[i][0] * j, dy = y + dir[i][1] * j;
                if (xs[dx] == 0 || ys[dy] == 0) continue;
                Key p1 = Key{dx, y};
                Key p2 = Key{dx, dy};
                Key p3 = Key{x, dy};
                ans += rec[p1] * rec[p2] * rec[p3];  
            }
        }
        return ans;
    }
}; 
//执行用时：388 ms, 在所有 C++ 提交中击败了64.67% 的用户
//内存消耗：105.5 MB, 在所有 C++ 提交中击败了39.71% 的用户
```
---
### 解法2 哈希表
上述做法虽然能通过，但是太繁琐、代码太长了，还要提供 `pair<int, int>` 的哈希函数和判等函数。我们可以思考其他方法，比如用哈希表 `rec` 计数，同时用哈希表 `row` 存储所有在同一垂直线上的点（的纵坐标）。`count` 查询时，遍历所有与 `point` 处于同一垂直线的点，这两点作为左边和右边分别确定一个正方形，然后查询另外两个点的数目即可。

具体代码如下所示。整个 `count` 方法的时间复杂度为 $O(n)$ ，$n$ 为点的个数。
```cpp
//C++ version
class DetectSquares {
private:
    unordered_map<int, unordered_map<int, int>> rec;
    unordered_map<int, unordered_set<int>> row; //存储所有在同一垂直线的点的纵坐标
public:
    DetectSquares() {}
    void add(vector<int> point) { //O(1)
        ++rec[point[0]][point[1]];
        row[point[0]].insert(point[1]);
    }
    int count(vector<int> point) {
        int x = point[0], y = point[1], ans = 0;
        for (const auto &v : row[x]) { //遍历与x处于同一垂直线的所有点v
            if (v == y) continue; //同一点,跳过
            int len = abs(y - v); //以y到v的距离作为正方形的边长len
            ans += rec[x][v] * rec[x + len][v] * rec[x + len][y]; //作为左边时
            ans += rec[x][v] * rec[x - len][v] * rec[x - len][y]; //作为右边时
        }
        return ans;
    }
}; 
//执行用时：268 ms, 在所有 C++ 提交中击败了84.40% 的用户
//内存消耗：92.4 MB, 在所有 C++ 提交中击败了56.87% 的用户
```
