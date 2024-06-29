> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>There is a <strong>family tree</strong> rooted at <code>0</code> consisting of <code>n</code> nodes numbered <code>0</code> to <code>n - 1</code>. You are given a <strong>0-indexed</strong> integer array <code>parents</code>, where <code>parents[i]</code> is the parent for node <code>i</code>. Since node <code>0</code> is the <strong>root</strong>, <code>parents[0] == -1</code>.</p>

<p>There are <code>10<sup>5</sup></code> genetic values, each represented by an integer in the <strong>inclusive</strong> range <code>[1, 10<sup>5</sup>]</code>. You are given a <strong>0-indexed</strong> integer array <code>nums</code>, where <code>nums[i]</code> is a <strong>distinct </strong>genetic value for node <code>i</code>.</p>

<p>Return <em>an array </em><code>ans</code><em> of length </em><code>n</code><em> where </em><code>ans[i]</code><em> is</em> <em>the <strong>smallest</strong> genetic value that is <strong>missing</strong> from the subtree rooted at node</em> <code>i</code>.</p>

<p>The <strong>subtree</strong> rooted at a node <code>x</code> contains node <code>x</code> and all of its <strong>descendant</strong> nodes.</p>

<p><strong>Example 1:</strong></p>
<img style="width: 204px; height: 167px;" src="https://assets.leetcode.com/uploads/2021/08/23/case-1.png" alt="">

```cpp
Input: parents = [-1,0,0,2], nums = [1,2,3,4]
Output: [5,1,1,1]
Explanation: The answer for each subtree is calculated as follows:
- 0: The subtree contains nodes [0,1,2,3] with values [1,2,3,4]. 5 is the smallest missing value.
- 1: The subtree contains only node 1 with value 2. 1 is the smallest missing value.
- 2: The subtree contains nodes [2,3] with values [3,4]. 1 is the smallest missing value.
- 3: The subtree contains only node 3 with value 4. 1 is the smallest missing value.
```

<p><strong>Example 2:</strong></p>
<img style="width: 247px; height: 168px;" src="https://assets.leetcode.com/uploads/2021/08/23/case-2.png" alt="">

```cpp
Input: parents = [-1,0,1,0,3,3], nums = [5,4,6,2,1,3]
Output: [7,1,1,4,2,1]
Explanation: The answer for each subtree is calculated as follows:
- 0: The subtree contains nodes [0,1,2,3,4,5] with values [5,4,6,2,1,3]. 7 is the smallest missing value.
- 1: The subtree contains nodes [1,2] with values [4,6]. 1 is the smallest missing value.
- 2: The subtree contains only node 2 with value 6. 1 is the smallest missing value.
- 3: The subtree contains nodes [3,4,5] with values [2,1,3]. 4 is the smallest missing value.
- 4: The subtree contains only node 4 with value 1. 2 is the smallest missing value.
- 5: The subtree contains only node 5 with value 3. 1 is the smallest missing value.
```

 
<p><strong>Example 3:</strong></p>

```cpp
Input: parents = [-1,2,3,0,2,4,1], nums = [2,3,4,5,6,7,8]
Output: [1,1,1,1,1,1,1]
Explanation: The value 1 is missing from all the subtrees.
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>n == parents.length == nums.length</code></li>
	<li><code>2 &lt;= n &lt;= 10<sup>5</sup></code></li>
	<li><code>0 &lt;= parents[i] &lt;= n - 1</code> for <code>i != 0</code></li>
	<li><code>parents[0] == -1</code></li>
	<li><code>parents</code> represents a valid tree.</li>
	<li><code>1 &lt;= nums[i] &lt;= 10<sup>5</sup></code></li>
	<li>Each <code>nums[i]</code> is distinct.</li>
</ul>

题意：有一棵根节点为 <code>0</code>&nbsp;的 <strong>家族树</strong>&nbsp;，总共包含 <code>n</code>&nbsp;个节点，节点编号为 <code>0</code>&nbsp;到 <code>n - 1</code>&nbsp;。给你一个下标从 <strong>0</strong>&nbsp;开始的整数数组 <code>parents</code>&nbsp;，其中&nbsp;<code>parents[i]</code>&nbsp;是节点 <code>i</code>&nbsp;的父节点。由于节点 <code>0</code>&nbsp;是 <strong>根</strong>&nbsp;，所以&nbsp;<code>parents[0] == -1</code>&nbsp;。</p>

<p>总共有&nbsp;<code>10<sup>5</sup></code>&nbsp;个基因值，每个基因值都用 <strong>闭区间</strong>&nbsp;<code>[1, 10<sup>5</sup>]</code>&nbsp;中的一个整数表示。给你一个下标从&nbsp;<strong>0</strong>&nbsp;开始的整数数组&nbsp;<code>nums</code>&nbsp;，其中&nbsp;<code>nums[i]</code>&nbsp;是节点 <code>i</code>&nbsp;的基因值，且基因值 <strong>互不相同</strong>&nbsp;。</p>

<p>返回一个数组<em>&nbsp;</em><code>ans</code>&nbsp;，长度为&nbsp;<code>n</code>&nbsp;，其中&nbsp;<code>ans[i]</code>&nbsp;是以节点&nbsp;<code>i</code>&nbsp;为根的子树内 <b>缺失</b>&nbsp;的&nbsp;<strong>最小</strong>&nbsp;基因值。</p>

---
暴力做法很简单，后序遍历整棵树；然后对当前子树根节点，合并所有不同子节点的状态，从 $1$ 开始遍历，查找当前子树缺少的最小基因值。总的复杂度为 $O(n^2)$ ，也是我一开始的想法。

### 解法1 DFS先找1
如果先找到 $1$ 的位置，则其他不含 $1$ 的子树缺少的最小基因值直接为 $1$ ，接着只需要计算含 $1$ 的子树即可，即从根节点到 $1$ 这条路径上的节点。这样自下而上遍历，就可以使得复杂度降到 $O(n)$ ：
```cpp
//C++
class Solution {
private:
    vector<int> tree[100010]; //树 
    bool genes[100010], vis[100010]; //某个基因值是否出现,是否访问某个顶点
public:
    vector<int> smallestMissingValueSubtree(vector<int>& parents, vector<int>& nums) {
        int n = parents.size();
        for (int i = 1; i < n; ++i) tree[parents[i]].push_back(i); //0是根节点
        int pos = -1; //基因值1的下标位置
        for (int i = 0; i < n; ++i) if (nums[i] == 1) { pos = i; break; } 
        vector<int> ans(n, 1); //结果数组
        if (pos == -1) return ans; //不存在为1的基因值,全部返回1
        int last = 2; //遍历子树时如果基因值为last的顶点被访问,则++last
        function<void(int)> dfs = [&](int u) {
            if (vis[u]) return;
            vis[u] = true; //访问过这一顶点
            for (const int &v : tree[u]) dfs(v); //后根遍历
            genes[nums[u]] = true;   //标记nums[u]这一基因值的出现
            while (genes[last]) ++last; //出现last这一基因值,说明不再缺少last,++last
        };
        while (pos != -1) { //从基因值1的下标位置,沿着父节点一路上溯到根
            dfs(pos); //DFS遍历含有基因值1的子树
            ans[pos] = last; //得到当前(含有基因值1的)子树缺少的最小基因值
            pos = parents[pos]; //上溯
        }
        return ans;
    }
};
//执行用时：340 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：130.7 MB, 在所有 C++ 提交中击败了100.00% 的用户
```

---
### 解法2 启发式合并/线段树
有的大佬使用了启发式合并，使我大受震撼。以后有时间再写一下相关思路。还有写线段树的，不知道怎么想到的……
