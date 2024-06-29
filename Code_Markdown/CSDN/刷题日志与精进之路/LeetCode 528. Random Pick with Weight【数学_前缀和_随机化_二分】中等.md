> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，这个截止期限可能是永远。这一系列刷题文章中，不仅会讲解多种解题思路及其优化，还将用多种编程语言实现题解，涉及到通用解法时更会归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还将建立相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>You are given an array of positive integers <code>w</code> where <code>w[i]</code> describes the weight of <code>i</code><sup><code>th</code> </sup>index (0-indexed).</p>

<p>We need to call the function <code>pickIndex()</code> which <strong>randomly</strong> returns an integer in the range <code>[0, w.length - 1]</code>. <code>pickIndex()</code> should return the integer proportional to its weight in the <code>w</code> array. For example, for <code>w = [1, 3]</code>, the probability of picking the index <code>0</code> is <code>1 / (1 + 3) = 0.25</code> (i.e 25%) while the probability of picking the index <code>1</code> is <code>3 / (1 + 3) = 0.75</code> (i.e 75%).</p>

<p>More formally, the probability of picking index <code>i</code> is <code>w[i] / sum(w)</code>.</p>

 
<p><strong>Example 1:</strong></p> 

```haskell
Input
["Solution","pickIndex"]
[[[1]],[]]
Output
[null,0]

Explanation
Solution solution = new Solution([1]);
solution.pickIndex(); // return 0. Since there is only one single element on the array the only option is to return the first element.
```
 
<p><strong>Example 2:</strong></p>

```haskell
Input
["Solution","pickIndex","pickIndex","pickIndex","pickIndex","pickIndex"]
[[[1,3]],[],[],[],[],[]]
Output
[null,1,1,1,1,0]

Explanation
Solution solution = new Solution([1, 3]);
solution.pickIndex(); // return 1. It's returning the second element (index = 1) that has probability of 3/4.
solution.pickIndex(); // return 1
solution.pickIndex(); // return 1
solution.pickIndex(); // return 1
solution.pickIndex(); // return 0. It's returning the first element (index = 0) that has probability of 1/4.

Since this is a randomization problem, multiple answers are allowed so the following outputs can be considered correct :
[null,1,1,1,1,0]
[null,1,1,1,1,1]
[null,1,1,1,0,0]
[null,1,1,1,0,1]
[null,1,0,1,0,0]
......
and so on. 
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= w.length &lt;= 10000</code></li>
	<li><code>1 &lt;= w[i] &lt;= 10^5</code></li>
	<li><code>pickIndex</code> will be called at most <code>10000</code> times.</li>
</ul>



题意：给定一个正整数数组 <code>w</code> ，其中 <code>w[i]</code> 代表下标 <code>i</code> 的权重（下标从 <code>0</code> 开始），请写一个函数 <code>pickIndex</code> ，它可以随机地获取下标 <code>i</code>，选取下标 <code>i</code> 的概率与 <code>w[i]</code> 成正比。也就是说，选取下标 <code>i</code> 的概率为 <code>w[i] / sum(w)</code> 。


---
### 解法1 前缀和+随机化+二分
这一道题乍一看可能懵了，该怎么做啊？不会啊？不过仔细一想，题意中说，`w[i]` 代表下标 `i` 的权重，选择 `i` 下标的概率与 `w[i]` 成正比为 `w[i] / sum(w)` ……这说明什么呢？说明在 `pickIndex` 调用总次数为 $\sum_{i=0}^{w.length-1} w[i]$ 时，下标 `i` 的返回次数应该为权重值 `w[i]` 次。 

于是我们要做的，就是**随机生成一个分布符合权重的序列**，其中随机数的产生可以使用语言自带的API，比如Java的 `Math.random()` 、C的 `srand(unsigned(time(0))); rand();` 、C++ `<random>` 设施。

由于 $1 \le w[i] \le 10^5$ 且 $w$ 长度达到了 $10^4$ ，直接构造一个长度为 `sum(w)` 、每个 `i` 出现 `w[i]` 次的数组（以供随机抽取）会MLE。由此，可以使用**前缀和数组**作为权重分布序列，**权重序列的基本单位为 $1$ ，代表 `1 / sum(w)` 的概率**。整个算法的步骤是：
- 计算 `w` 数组的前缀和数组 `s`（一定是严格升序的），将其看做总长度为 `sum(w)` 、基本单位为 `1` 的数轴；
- 接着使用随机函数产生 $[1,\ sum(w)]$ 范围内的随机数；
- 通过二分查找前缀和数组，即可找到分布位置，从而找到 `w` 数组中的原始下标值

以数组 `w[] = {1, 3, 5}` 为例，前缀和数组 `s[] = {0, 1, 4, 9}` ，随机生成 `[1, 2, 3, 4, 5, 6, 7, 8, 9]` 中的某个整数，其中生成 `1` 时对应的是 `w[0]` ，生成 `2 ~ 4` 时对应的是 `w[1]` ，生成 `5 ~ 9` 时对应的是 `w[2]` ，从而满足**选择 `i` 下标的概率与 `w[i]` 成正比**这一题目要求。

最后的代码如下所示，`Solution` 类的构造方法复杂度为 $O(n)$ ，`pickIndex` 方法的复杂度为 $O(\log n)$ ，空间复杂度为 $O(n)$ ：
```cpp
//C++ version
unsigned seed = chrono::system_clock::now().time_since_epoch().count();       
default_random_engine generator(seed);
class Solution {
private:
    vector<int> s;
    uniform_int_distribution<int> ud;
public:
    Solution(vector<int>& w) {
        int n = w.size();
        s.resize(n + 1);
        for (int i = 0; i < n; ++i) s[i + 1] = w[i] + s[i];
        ud.param(uniform_int_distribution<>::param_type {1, s.back()});
    }
    
    int pickIndex() {
        int target = ud(generator), l = 1, r = s.size(); 
        while (l < r) { //找到前缀和数组s中第一个>=target的下标位置
            int mid = l + (r - l) / 2;
            if (s[mid] >= target) r = mid;
            else l = mid + 1;
        }    
        return l - 1; //从s的下标位置转换到w的下标位置
    }
};
//执行用时：76 ms, 在所有 C++ 提交中击败了74.71% 的用户
//内存消耗：39.3 MB, 在所有 C++ 提交中击败了76.93% 的用户
```
Java版本的代码如下所示，不过这里生成的随机数范围为 $[0, sum(w))$ 。以数组 `w[] = {1, 3, 5}` 为例，前缀和数组 `s[] = {0, 1, 4, 9}` ，随机生成 `[1, 2, 3, 4, 5, 6, 7, 8, 9]` 中的某个整数，其中生成 `0` 时对应的是 `w[0]` ，生成 `1 ~ 3` 时对应的是 `w[1]` ，生成 `4 ~ 8` 时对应的是 `w[2]` 。因此二分代码也不一样：

```java
//Java version
class Solution {
    private int[] s;
    public Solution(int[] w) {
        s = new int[w.length + 1];
        for (int i = 0; i < w.length; ++i) s[i + 1] = w[i] + s[i];
    }

    public int pickIndex() {
        int n = s.length, l = 1, r = n;
        int target = new Random().nextInt(s[n - 1]); //生成一个随机的int值,介于[0,n)区间
        while (l < r) { //找到第一个大于target的数
            int mid = l + (r - l) / 2;
            if (s[mid] > target) r = mid;
            else l = mid + 1;
        }
        return l - 1; 
    }
}
```
做完这一题后，建议使用同样的方法做一下[497. Random Point in Non-overlapping Rectangles](https://leetcode-cn.com/problems/random-point-in-non-overlapping-rectangles/)。

---
### 解法2 模拟+桶轮询
这一做法主要是针对OJ，不建议用于实际工程。利用OJ对权重分布只做近似检查的特点，可以构造一个最短轮询序列（权重精度保留到小数点一位），并存储二元组 `(i, cnt)` ，代表下标 `i` 在最短轮询序列中出现次数为 `cnt` 。具体来说，步骤如下：
- 取出最小权重 `minw` ，计算出权重序列之和 `sum(w)` ，于是最小权重代表的概率是 `minp = minw / sum(w)` ；
- 求出使 `minp * k >= 1` 的 `k` 值，从而可以放大所有下标 `i` 的概率 `w[i] / sum(w)` 到大于等于 `1` ；
- 使用放大后的概率值作为下标 `i` 在最短轮询序列中出现的次数 `cnt` ，存储这些二元组，加总所有下标 `i` 对应的 `cnt` 就能得到轮询序列总长度 `tot` ；
- 在 `pickIndex` 方法中，使用桶编号 `bid` 和桶内编号 `inid` 来对 `w.length` 个桶进行轮询：
	- 访问当前桶 `list.get(bid)` ，如果桶内编号 `inid` 没有超出当前桶的数量范围 `cnt` ，就自增桶内编号 `++inid` ，再返回当前桶的编号；
	- 否则 `++bid` 移动到下一个桶，重置桶内编号 `inid = 0` ，递归调用 `pickIndex()` 查看下一个桶；
	- 如果当前桶编号 `bid` 超过 `w.length` ，就再从头开始。

通过使用这一固定的轮询序列，好处是不需要使用随机函数，同时返回的连续序列在**长度不短于最小段长度**时，总是符合近似权重分布。实际代码如下：
```cpp
//C++ version
class Solution {
private:
    using bucket = pair<int, int>;
    vector<bucket> seq;
    int bid = 0, inid = 0, tot = 0; //桶编号,桶内编号,最短轮询序列长度
public:
    Solution(vector<int>& w) {
        int n = w.size();
        double sum = 0, minw = DBL_MAX;
        for (int i = 0; i < n; ++i) {
            sum += w[i];
            minw = fmin(minw, w[i]);
        }
        double minp = minw / sum;
        int k = 1.0 / minp + 5;
        for (int i = 0; i < n; ++i) {
            int cnt = (int)(w[i] / sum * k);
            seq.push_back(bucket{i, cnt});
            tot += cnt;
        }
    }
    int pickIndex() {
        if (bid >= seq.size()) bid = inid = 0;
        bucket b = seq[bid];
        int id = b.first, cnt = b.second;
        if (inid >= cnt) {
            ++bid;
            inid = 0;
            return pickIndex();
        }
        ++inid;
        return bid;
    }
};
//执行用时：64 ms, 在所有 C++ 提交中击败了97.34% 的用户
//内存消耗：39.3 MB, 在所有 C++ 提交中击败了60.87% 的用户
```
