<p>You are given an array <code>nums</code> consisting of non-negative integers. You are also given a <code>queries</code> array, where <code>queries[i] = [x<sub>i</sub>, m<sub>i</sub>]</code>.</p>

<p>The answer to the <code>i<sup>th</sup></code> query is the maximum bitwise <code>XOR</code> value of <code>x<sub>i</sub></code> and any element of <code>nums</code> that does not exceed <code>m<sub>i</sub></code>. In other words, the answer is <code>max(nums[j] XOR x<sub>i</sub>)</code> for all <code>j</code> such that <code>nums[j] &lt;= m<sub>i</sub></code>. If all elements in <code>nums</code> are larger than <code>m<sub>i</sub></code>, then the answer is <code>-1</code>.</p>

<p>Return <em>an integer array </em><code>answer</code><em> where </em><code>answer.length == queries.length</code><em> and </em><code>answer[i]</code><em> is the answer to the </em><code>i<sup>th</sup></code><em> query.</em></p>

 
<p><strong>Example 1:</strong></p>

```clike
Input: nums = [0,1,2,3,4], queries = [[3,1],[1,3],[5,6]]
Output: [3,3,7]
Explanation:
1) 0 and 1 are the only two integers not greater than 1. 0 XOR 3 = 3 and 1 XOR 3 = 2. The larger of the two is 3.
2) 1 XOR 2 = 3.
3) 5 XOR 2 = 7. 
```

<p><strong>Example 2:</strong></p>

```clike
Input: nums = [0,1,2,3,4], queries = [[3,1],[1,3],[5,6]]
Output: [3,3,7]
Explanation:
1) 0 and 1 are the only two integers not greater than 1. 0 XOR 3 = 3 and 1 XOR 3 = 2. The larger of the two is 3.
2) 1 XOR 2 = 3.
3) 5 XOR 2 = 7.
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length, queries.length &lt;= 10<sup>5</sup></code></li>
	<li><code>queries[i].length == 2</code></li>
	<li><code>0 &lt;= nums[j], x<sub>i</sub>, m<sub>i</sub> &lt;= 10<sup>9</sup></code></li>
</ul>


题意：给你一个由非负整数组成的数组 <code>nums</code> 。另有一个查询数组 <code>queries</code> ，其中 <code>queries[i] = [x<sub>i</sub>, m<sub>i</sub>]</code> 。

<p>第 <code>i</code> 个查询的答案是 <code>x<sub>i</sub></code> 和任何 <code>nums</code> 数组中不超过 <code>m<sub>i</sub></code> 的元素按位异或（<code>XOR</code>）得到的最大值。换句话说，答案是 <code>max(nums[j] XOR x<sub>i</sub>)</code> ，其中所有 <code>j</code> 均满足 <code>nums[j] &lt;= m<sub>i</sub></code> 。如果 <code>nums</code> 中的所有元素都大于 <code>m<sub>i</sub></code>，最终答案就是 <code>-1</code> 。</p>

<p>返回一个整数数组<em> </em><code>answer</code><em> </em>作为查询的答案，其中<em> </em><code>answer.length == queries.length</code><em> </em>且<em> </em><code>answer[i]</code><em> </em>是第<em> </em><code>i</code><em> </em>个查询的答案。</p>

---
### 解法 离线+排序+双指针+贪心+01字典树

本题不算是那种让人毫无思路的题，但是也不是那么简单。我们可以注意到，查询数组中每个查询 `queries[i] = [xi, mi]` 都必须考察 `nums[]` 中所有 `<= mi` 的元素，从中选出一个元素和 `xi` 异或并得到最大的异或和。如果每次都要遍历整个 `nums[]` ，复杂度肯定螺旋升天，令 `nums.length = n, queries.length = m` ，光这个循环遍历都需要 $O(nm)$ 的复杂度。

稍微改进一下，对 `nums[]` 从小到大排序，此时对于每个 `queries[i] = [xi, mi]` 中的 `mi` ，我们可以二分查询第一个 `> mi` 的元素在 `nums[]` 中的位置，这个位置之前的所有元素都 `<= mi` 。此时的复杂度已经达到 $O(n\log n)$ 。接下来，由于 `queries[]` 并非按照 `mi` 有序，我们必须对每个查询的 `mi` 都二分查找 `nums[]` 数组，然后将前面的元素插入到01字典树中，并找到和 `xi` 异或后最大的元素……一想就很麻烦，大概是 $O(n\log n + m\log n * n\log n)$ 的时间复杂度，也肯定会超时。

所以继续改进，将 `queries[]` 按照 `mi` 从小到大排序，此时可以用两个指针 `i, j` 分别指向 `nums[]` 和 `queries[]` 。不断往后移动 `i` ，当 `nums[i] <= queries[j].mj` 时，就将这些 `nums[i]` 插入到01字典树中。然后贪心地查询此时的01字典树中、和 `queries[j].xj` 异或后值最大的元素——如果 `i == 0` ，说明 `nums[]` 中所有的元素都大于 `queries[j].mj` ，此时这一查询的结果是 `-1` 。这样做的时间复杂度大概是 $O(n\log n + m\log m + m\log n)$ 。

效率高是高了，不过如何按照顺序返回查询数组的结果呢？我们事先离线记录每个查询及其在 `queries[]` 中的位置，然后进行排序……最后对每个查询的结果，按照记录的位置写到 `ans[]` 中。最终代码如下，需要注意的是，本题中为了节省空间，01字典树中没有存储节点值，虽然牺牲了一点效率：
```cpp
class Trie01 {
private:
    Trie01 *next[2]; //不存储节点值
public:
    Trie01() { memset(next, 0, sizeof(next)); }
    void insert(int x) {
        Trie01 *cur = this;
        for (int i = 31; i >= 0; --i) {
            int d = (x >> i) & 1;
            if (cur->next[d] == nullptr) cur->next[d] = new Trie01;
            cur = cur->next[d];
        }
    }
    int getMaxXorVal(int x) {
        Trie01 *cur = this;
        int ans = 0;
        for (int i = 31; i >= 0; --i) { //01字典树中的值都>=0
            int d = (x >> i) & 1;
            if (cur->next[d ^ 1]) {
                cur = cur->next[d ^ 1];
                ans = (ans << 1) + (d ^ 1);
            } else {
                cur = cur->next[d];
                ans = (ans << 1) + d;
            }
        }
        return ans;
    }
};
class Solution {
public:
    vector<int> maximizeXor(vector<int>& nums, vector<vector<int>>& queries) {
        using pv = pair<vector<int>, int>;
        vector<pv> tmp;
        int m = nums.size(), n = queries.size();
        for (int i = 0; i < n; ++i) tmp.push_back({queries[i], i}); //按照mi的大小排序
        sort(tmp.begin(), tmp.end(), [&](const pv &a, const pv &b) { return a.first[1] < b.first[1]; });
        sort(nums.begin(), nums.end());
        vector<int> ans(n);
        Trie01 trie; //01字典树
        int i = 0, j = 0; //i指向数组nums,j指向数组tmp
        while (j < n) { //当nums[i]<=mj时插入到01字典树中,i指向下一个nums[]元素
            while (i < m && nums[i] <= tmp[j].first[1]) trie.insert(nums[i++]);
            ans[tmp[j].second] = i ? trie.getMaxXorVal(tmp[j].first[0]) ^ tmp[j].first[0] : -1;
            ++j; //如果nums中所有元素都>mi,查询的答案就是-1
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：884 ms, 在所有 C++ 提交中击败了70.37% 的用户
内存消耗：228 MB, 在所有 C++ 提交中击败了43.10% 的用户
```
当然，本题也存在着在线的算法，不过稍微麻烦一点，而且效率较低。日后有时间了再更新相应做法。
