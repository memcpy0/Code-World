> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个长度为 `n` 、下标从 **0** 开始的整数数组 `nums1` 和 `nums2` ，另给你一个下标从 **1** 开始的二维数组 `queries` ，其中 `queries[i] = [xi, yi]` 。

对于第 `i` 个查询，在所有满足 `nums1[j] >= xi` 且 `nums2[j] >= yi` 的下标 `j` `(0 <= j < n)` 中，找出 `nums1[j] + nums2[j]` 的 **最大值** ，如果不存在满足条件的 `j` 则返回 **-1** 。

返回数组 `answer` _，_其中 `answer[i]` 是第 `i` 个查询的答案。

**示例 1：**

**输入：**nums1 = [4,3,1,2], nums2 = [2,4,9,5], queries = [[4,1],[1,3],[2,5]]
**输出：**[6,10,7]
**解释：**
对于第 1 个查询：`xi = 4` 且 `yi = 1` ，可以选择下标 `j = 0` ，此时 `nums1[j] >= 4` 且 `nums2[j] >= 1` 。`nums1[j] + nums2[j]` 等于 6 ，可以证明 6 是可以获得的最大值。
对于第 2 个查询：`xi = 1` 且 `yi = 3` ，可以选择下标 `j = 2` ，此时 `nums1[j] >= 1` 且 `nums2[j] >= 3` 。`nums1[j] + nums2[j]` 等于 10 ，可以证明 10 是可以获得的最大值。
对于第 3 个查询：`xi = 2` 且 `yi = 5` ，可以选择下标 `j = 3` ，此时 `nums1[j] >= 2` 且 `nums2[j] >= 5` 。`nums1[j] + nums2[j]` 等于 7 ，可以证明 7 是可以获得的最大值。
因此，我们返回 `[6,10,7]` 。

**示例 2：**

**输入：**nums1 = [3,2,5], nums2 = [2,3,4], queries = [[4,4],[3,2],[1,1]]
**输出：**[9,9,9]
**解释：**对于这个示例，我们可以选择下标 `j = 2` ，该下标可以满足每个查询的限制。

**示例 3：**

**输入：**nums1 = [2,1], nums2 = [2,3], queries = [[3,3]]
**输出：**[-1]
**解释：**示例中的查询 `xi` = 3 且 `yi` = 3 。对于每个下标 j ，都只满足 nums1[j] < `xi` 或者 nums2[j] < `yi` 。因此，不存在答案。 

**提示：**
- `nums1.length == nums2.length` 
- `n == nums1.length` 
- `1 <= n <= 10^5`
- `1 <= nums1[i], nums2[i] <= 10^9` 
- `1 <= queries.length <= 10^5`
- `queries[i].length == 2`
- `xi == queries[i][1]`
- `yi == queries[i][2]`
- `1 <= xi, yi <= 10^9`

---
### 解法1 树状数组
二维偏序模板题。把 (nums1[i], nums2[i]) 看成二维平面上的一个红点，(queries[i][0], queries[i][1]) 看成二维平面上的一个蓝点。我们要对每个蓝点求出它的右上方横纵坐标之和最大的红点。

我们将所有点先**按横坐标从大到小排序**，然后依此枚举每个点。这样遇到一个蓝点 (x, y)，我们**只要求之前枚举过的，且纵坐标大于等于 y 的红点中**，横纵坐标之和最大是多少。用树状数组维护即可。复杂度 O(nlog⁡n)\mathcal{O}(n\log n)O(nlogn)。

因为 nums1，nums2 和 queries 里面的数很大，但是我们只关心数的相对大小关系，因此先把所有数离散化再计算（当然，离散化之前要把 nums1[i] + nums2[i] 的值存起来）。

参考代码（c++）
```
class Solution {
public:
    vector<int> maximumSumQueries(vector<int>& nums1, vector<int>& nums2, vector<vector<int>>& queries) {
        // 将坐标离散化，顺便把大小关系翻转 !!! 好！！！
        map<int, int> mp;
        for (auto x : nums1) mp[x] = 1;
        for (auto x : nums2) mp[x] = 1;
        for (auto v : queries) mp[v[0]] = 1, mp[v[1]] = 1;
        int tot = mp.size();
        for (auto &p : mp) p.second = tot--;
        tot = mp.size();

        int n = nums1.size();
        // val[i] = nums1[i] + nums2[i]（离散化前）
        vector<int> val;
        for (int i = 0; i < n; i++) {
            val.push_back(nums1[i] + nums2[i]);
            nums1[i] = mp[nums1[i]];
            nums2[i] = mp[nums2[i]];
        }
        int q = queries.size();
        for (int i = 0; i < q; i++) {
            queries[i][0] = mp[queries[i][0]];
            queries[i][1] = mp[queries[i][1]];
        }

        // 树状数组模板
        int tree[tot + 10];
        memset(tree, -1, sizeof(tree));

        auto lb = [](int x) { return x & (-x); };

        auto modify = [&](int pos, int val) {
            for (; pos <= tot; pos += lb(pos)) tree[pos] = max(tree[pos], val);
        };

        auto query = [&](int pos) {
            int ret = -1;
            for (; pos; pos -= lb(pos)) ret = max(ret, tree[pos]);
            return ret;
        };

        // 将数值点和询问点先按横坐标排序，注意相同坐标的数值点要排在询问点前面
        typedef pair<int, int> pii;
        typedef pair<pii, int> piii;
        vector<piii> vec;
        for (int i = 0; i < n; i++) vec.push_back(piii(pii(nums1[i], nums2[i]), -val[i]));
        for (int i = 0; i < q; i++) vec.push_back(piii(pii(queries[i][0], queries[i][1]), i));
        sort(vec.begin(), vec.end());

        vector<int> ans(q);
        for (auto &p : vec) {
            int x = p.first.first, y = p.first.second, v = p.second;
            if (v < 0) modify(y, -v);
            else ans[v] = query(y);
        }
        return ans;
    }
};
```

把nums1,nums2看成修改，queries看成查询。 两种操作一起排序，把x大的放前面，x相同的话修改操作放前面。 把BIT看成一个普通数组B： 对于修改操作，就是B[y]=max(B[y],x+y)B[y]=max(B[y],x+y)B[y]=max(B[y],x+y); 对于查询操作，result=max(B[y:∞])result=max(B[y:\infty])result=max(B[y:∞])。

时间复杂度O((n+q)log(n+q)+(n+q)log(U))O((n+q)log(n+q)+(n+q)log(U))O((n+q)log(n+q)+(n+q)log(U))

空间复杂度没想明白,好像是O(nlog(U/n)+n+q)O(nlog(U/n)+n+q)O(nlog(U/n)+n+q)。


思路
由题意可知，要求两者都大且和最大，那么我们先按nums1升序，然后一个个检查nums2，如果后面出现nums2比前面大，那说明前面那个数没有用了（两者都比你大，那么和也比你更大） 剔除掉无用数后，我们可以发现第一维是升序，第二维是降序。 对于每一个询问queries，我们可以用二分法找到第一个比x大的下标left（left之后的都比它大） 同理，可以找到比y大的下标right（right之前的都比它大） 接下来我们需要返回的答案就是left和right之间所有下标里和最大的值（线段树维护区间最大值）

解题方法
二分+线段树

复杂度
时间复杂度: O(3mlogn)O(3mlogn)O(3mlogn)(应该是)
Code
C++
//借鉴的大佬代码
#define x first
#define y second
typedef long long LL;
typedef pair<int, int> PII;
const int N = 1e5 + 5;
struct Node
{
    int l, r;
    int sum;
}tr[4 * N];
class Solution {
public:
    vector<PII> w, p;
    void pushup(int u)
    {
        tr[u].sum = max(tr[u << 1].sum, tr[u << 1 | 1].sum);
    }
    void build(int u, int l, int r)
    {
        if (l == r) tr[u] = {l, r, w[r - 1].x + w[r - 1].y};
        else
        {
            tr[u] = {l, r};
            int mid = l + r >> 1;
            build(u << 1, l, mid), build(u << 1 | 1, mid + 1, r);
            pushup(u);
        }
    }
    LL query(int u, int l, int r)
    {
        if (tr[u].l >= l && tr[u].r <= r) return tr[u].sum;
        int mid = tr[u].l + tr[u].r >> 1;
        LL res = -1;
        if (mid >= l) res = query(u << 1, l, r);
        if (mid < r) res = max(res, query(u << 1 | 1, l, r));
        return res;
    }
    vector<int> maximumSumQueries(vector<int>& nums1, vector<int>& nums2, vector<vector<int>>& queries) {
        int n = nums1.size(), m = queries.size();
        for (int i = 0; i < n; i ++) p.push_back({nums1[i], nums2[i]});
        sort(p.begin(), p.end());
        for (int i = 0; i < n; i ++)
        {
            while (w.size() && w.back().y < p[i].y) w.pop_back();
            w.push_back(p[i]);
        }
        n = w.size();
        build(1, 1, n);
        vector<int> ans(m, -1);
        for (int i = 0; i < m; i ++)
        {
            int left, right;
            int l = 0, r = n - 1;
            while (l < r)
            {
                int mid = l + r >> 1;
                if (w[mid].x >= queries[i][0]) r = mid;
                else l = mid + 1;
            }
            left = l;
            l = 0, r = n - 1;
            while (l < r)
            {
                int mid = l + r + 1 >> 1;
                if (w[mid].y >= queries[i][1]) l = mid;
                else r = mid - 1;
            }
            right = l;
            if (w[left].x >= queries[i][0] && w[right].y >= queries[i][1]) ans[i] = query(1, left + 1, right + 1);
        }
        return ans;
    }
};

先对所有 xxx 和 yyy 取相反数, 这样计算左边对右边的贡献会更自然.

把 {(nums1[i],nums2[i],−1)}\{(\text{nums1}[i],\text{nums2}[i],-1)\}{(nums1[i],nums2[i],−1)} 称为修改, {(queries[i][0],queries[i][1],i)}\{(\text{queries}[i][0], \text{queries}[i][1],i)\}{(queries[i][0],queries[i][1],i)} 称为查询, 将其拼成一个序列进行排序. 注意排序的时候, 如果值相等, 修改必须在查询前, 这里可以通过比较 −1-1−1 和 iii 这一维实现.

首先使用任何一种 O(nlog⁡n)O(n\log n)O(nlogn)基于比较的排序对第一维进行排序, 然后在得到的排序序列中对第二维进行归并排序. 归并时左侧的序列第一维必然小于右侧的序列，那么左侧的修改只要考虑第二维就可以判断是否能对右侧的询问产生影响, 而右侧的修改不可能对左侧的询问产生影响. 在对第二维归并的过程中维护 ∣nums1[i]+nums2[i]∣|\text{nums1}[i]+\text{nums2}[i]|∣nums1[i]+nums2[i]∣ 的最大值即可更新答案, 这样对于每个合法 (((修改,,, 询问)))对都会在第一次将这两个元素归并在一个序列时影响被统计到.

复杂度 O(log⁡n)O(\log n)O(logn).

参考代码

class Solution:
    def maximumSumQueries(
        self, nums1: List[int], nums2: List[int], queries: List[List[int]]
    ) -> List[int]:
        n = len(nums1)
        q = len(queries)
        a = sorted(
            [(-nums1[i], -1, -nums2[i]) for i in range(n)]
            + [(-queries[i][0], i, -queries[i][1]) for i in range(q)]
        )
        ans = [-1] * q

        def rec(l, r):
            m = (l + r) // 2
            if l + 1 == r:
                return [(a[m][2], a[m][1], a[m][0])]
            L = rec(l, m)
            R = rec(m, r)
            res = []
            i = 0
            j = 0
            pre = -1
            while i < len(L) or j < len(R):
                if j == len(R) or (i != len(L) and L[i] < R[j]):
                    if L[i][1] < 0:
                        pre = max(pre, -L[i][0] - L[i][2])
                    res.append(L[i])
                    i += 1
                else:
                    if R[j][1] >= 0:
                        ans[R[j][1]] = max(ans[R[j][1]], pre)
                    res.append(R[j])
                    j += 1
            return res

        rec(0, len(a))
        return ans

思路
为方便处理，可以先把 nums1\textit{nums}_1nums 
1
​
  和询问中的 xix_ix 
i
​
  排序。

这样就可以把重心放在 nums2[j]\textit{nums}_2[j]nums 
2
​
 [j] 与 yiy_iy 
i
​
  的大小关系上。

我们可以按照 xix_ix 
i
​
  从大到小、nums1[j]\textit{nums}_1[j]nums 
1
​
 [j] 从大到小的顺序处理，同时增量地维护满足 nums1[j]≥xi\textit{nums}_1[j]\ge x_inums 
1
​
 [j]≥x 
i
​
  的 nums2[j]\textit{nums}_2[j]nums 
2
​
 [j]。

如何维护？分类讨论：

如果 nums2[j]\textit{nums}_2[j]nums 
2
​
 [j] 比之前遍历过的 nums2[j′]\textit{nums}_2[j']nums 
2
​
 [j 
′
 ] 还要小，那么由于 nums1[j]\textit{nums}_1[j]nums 
1
​
 [j] 是从大到小处理的，所以 nums1[j]+nums2[j]\textit{nums}_1[j]+\textit{nums}_2[j]nums 
1
​
 [j]+nums 
2
​
 [j] 也比之前遍历过的 nums1[j′]+nums2[j′]\textit{nums}_1[j']+\textit{nums}_2[j']nums 
1
​
 [j 
′
 ]+nums 
2
​
 [j 
′
 ] 要小。那么在回答 ≤nums2[j]\le \textit{nums}_2[j]≤nums 
2
​
 [j] 的 yiy_iy 
i
​
  时，最大值不可能是 nums1[j]+nums2[j]\textit{nums}_1[j]+\textit{nums}_2[j]nums 
1
​
 [j]+nums 
2
​
 [j]，所以无需考虑这样的 nums2[j]\textit{nums}_2[j]nums 
2
​
 [j]。（这种单调性启发我们用单调栈来维护。）
如果是相等，那么同理，无需考虑。
如果是大于，那么就可以入栈。在入栈前还要去掉一些无效数据：
如果 nums1[j]+nums2[j]\textit{nums}_1[j]+\textit{nums}_2[j]nums 
1
​
 [j]+nums 
2
​
 [j] 不低于栈顶的 nums1[j′]+nums2[j′]\textit{nums}_1[j']+\textit{nums}_2[j']nums 
1
​
 [j 
′
 ]+nums 
2
​
 [j 
′
 ]，那么可以弹出栈顶。因为更大的 nums2[j]\textit{nums}_2[j]nums 
2
​
 [j] 更能满足 ≥yi\ge y_i≥y 
i
​
  的要求，栈顶的 nums1[j′]+nums2[j′]\textit{nums}_1[j']+\textit{nums}_2[j']nums 
1
​
 [j 
′
 ]+nums 
2
​
 [j 
′
 ] 在后续的询问中，永远不会是最大值。
代码实现时，可以直接比较 nums1[j]+nums2[j]\textit{nums}_1[j]+\textit{nums}_2[j]nums 
1
​
 [j]+nums 
2
​
 [j] 与栈顶的值，这是因为如果这一条件成立，由于 nums1[j]\textit{nums}_1[j]nums 
1
​
 [j] 是从大到小处理的，nums1[j]+nums2[j]\textit{nums}_1[j]+\textit{nums}_2[j]nums 
1
​
 [j]+nums 
2
​
 [j] 能比栈顶的大，说明 nums2[j]\textit{nums}_2[j]nums 
2
​
 [j] 必然不低于栈顶的 nums2[j′]\textit{nums}_2[j']nums 
2
​
 [j 
′
 ]。
这样我们会得到一个从栈底到栈顶，nums2[j]\textit{nums}_2[j]nums 
2
​
 [j] 递增，nums1[j]+nums2[j]\textit{nums}_1[j]+\textit{nums}_2[j]nums 
1
​
 [j]+nums 
2
​
 [j] 递减的单调栈。

最后在单调栈中二分 ≥yi\ge y_i≥y 
i
​
  的最小的 nums2[j]\textit{nums}_2[j]nums 
2
​
 [j]，对应的 nums1[j]+nums2[j]\textit{nums}_1[j]+\textit{nums}_2[j]nums 
1
​
 [j]+nums 
2
​
 [j] 就是最大的。

Python3
Go
class Solution:
    def maximumSumQueries(self, nums1: List[int], nums2: List[int], queries: List[List[int]]) -> List[int]:
        ans = [-1] * len(queries)
        st = []
        a = sorted((a, b) for a, b in zip(nums1, nums2))
        i = len(a) - 1
        for qid, (x, y) in sorted(enumerate(queries), key=lambda p: -p[1][0]):
            while i >= 0 and a[i][0] >= x:
                ax, ay = a[i]
                while st and st[-1][1] <= ax + ay:  # ay >= st[-1][0]
                    st.pop()
                if not st or st[-1][0] < ay:
                    st.append((ay, ax + ay))
                i -= 1
            j = bisect_left(st, (y,))
            if j < len(st):
                ans[qid] = st[j][1]
        return ans
复杂度分析
时间复杂度：O(n+qlog⁡n)\mathcal{O}(n + q\log n)O(n+qlogn)，其中 nnn 为 nums1\textit{nums}_1nums 
1
​
  的长度，qqq 为 queries\textit{queries}queries 的长度。
空间复杂度：O(n)\mathcal{O}(n)O(n)。返回值不计。

作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-sum-queries/solutions/2305395/pai-xu-dan-diao-zhan-shang-er-fen-by-end-of9h/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。