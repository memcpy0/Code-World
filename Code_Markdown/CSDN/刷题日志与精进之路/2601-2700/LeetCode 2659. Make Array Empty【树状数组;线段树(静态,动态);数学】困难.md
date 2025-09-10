> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个包含若干 **互不相同** 整数的数组 `nums` ，你需要执行以下操作 **直到****数组为空** ：

- 如果数组中第一个元素是当前数组中的 **最小值** ，则删除它。
- 否则，将第一个元素移动到数组的 **末尾** 。

请你返回需要多少个操作使 `nums` 为空。

**示例 1：**

**输入：**nums = [3,4,-1]
**输出：**5

|Operation|Array|
|---|---|
|1|[4, -1, 3]|
|2|[-1, 3, 4]|
|3|[3, 4]|
|4|[4]|
|5|[]|

**示例 2：**

**输入：**nums = [1,2,4,3]
**输出：**5

|Operation|Array|
|---|---|
|1|[2, 4, 3]|
|2|[4, 3]|
|3|[3, 4]|
|4|[4]|
|5|[]|

**示例 3：**

**输入：**nums = [1,2,3]
**输出：**3

|Operation|Array|
|---|---|
|1|[2, 3]|
|2|[3]|
|3|[]|

**提示：**

- `1 <= nums.length <= 105`
- `-109 <= nums[i] <= 109`
- `nums` 中的元素 **互不相同** 。

---
方法一：树状数组
直接模拟是 O(n2)\mathcal{O}(n^2)O(n 
2
 ) 的（最坏情况发生在降序数组），如何优化呢？

想象成用一个下标反复遍历数组，考虑下标的总共移动次数。

以 nums=[2,4,1,3]\textit{nums}=[2,4,1,3]nums=[2,4,1,3] 为例：

初始时，下标指向第一个数字 222。
移动 222 次到 111，删除 111，此时下标指向 333。
移动 111 次到 222，删除 222，此时下标指向 444。
移动 111 次到 333（注意 111 已经被删除了），删除 333，此时下标指向 444（注意 222 已经被删除了）。
无需移动，删除 444。
总共移动 2+1+1=42+1+1=42+1+1=4 次，加上删除操作 444 次，故答案为 888。
上述过程有两个要点：

按照元素值从小到大的顺序删除。
需要跳过已经删除的元素。换句话说，需要知道移动到下一个位置的途中，有多少元素被删除了。
由于要按照元素值从小到大的顺序删除，需要对数组排序。但是数组元素的顺序很重要，不能直接排序。可以创建一个下标数组，对下标数组按照 nums[i]\textit{nums}[i]nums[i] 的大小，从小到大排序。

由于要全部删除，先把数组长度 nnn 计入答案，这样后面只需要统计移动次数。

为方便计算，后面讨论的下标从 111 开始。

设上一个被删除的数的位置为 pre\textit{pre}pre（初始为 111），当前需要删除的位置为 iii。定义 query(i,j)\text{query}(i,j)query(i,j) 表示下标在 [i,j][i,j][i,j] 中的被删除的元素个数。考虑从 pre\textit{pre}pre 到 iii 的移动次数：

如果 pre≤i\textit{pre}\le ipre≤i，那么移动次数等于 i−pre−query(pre,i)i-\textit{pre}-\text{query}(\textit{pre},i)i−pre−query(pre,i)。
如果 pre>i\textit{pre}> ipre>i，那么移动次数等于 (n−pre−query(pre,n))+(i−query(1,i))(n-\textit{pre}-\text{query}(\textit{pre},n))+(i-\text{query}(1, i))(n−pre−query(pre,n))+(i−query(1,i))。假设删除了 kkk 个数，那么 query(pre,n))+query(1,i)=k−query(i+1,pre−1)=k−query(i,pre−1)\text{query}(\textit{pre},n))+\text{query}(1,i) = k - \text{query}(i+1,\textit{pre}-1)=k - \text{query}(i,\textit{pre}-1)query(pre,n))+query(1,i)=k−query(i+1,pre−1)=k−query(i,pre−1)，因为 iii 处的元素还没被删除。所以上式最终化简为 i−pre+n−k+query(i,pre−1)i-\textit{pre}+n-k+\text{query}(i,\textit{pre}-1)i−pre+n−k+query(i,pre−1)。
代码实现时，query(i,j)\text{query}(i,j)query(i,j) 可以用树状数组实现。（也可以用线段树、名次树等数据结构。）

Python3
Java
C++
Go
// 树状数组模板
class BIT {
    vector<int> tree;
public:
    BIT(int n) : tree(n) {}

    // 将下标 i 上的数加一
    void inc(int i) {
        while (i < tree.size()) {
            ++tree[i];
            i += i & -i;
        }
    }

    // 返回闭区间 [1, i] 的元素和
    int sum(int x) {
        int res = 0;
        while (x > 0) {
            res += tree[x];
            x &= x - 1;
        }
        return res;
    }

    // 返回闭区间 [left, right] 的元素和
    int query(int left, int right) {
        return sum(right) - sum(left - 1);
    }
};

class Solution {
public:
    long long countOperationsToEmptyArray(vector<int> &nums) {
        int n = nums.size(), id[n];
        iota(id, id + n, 0);
        sort(id, id + n, [&](int i, int j) {
            return nums[i] < nums[j];
        });

        long long ans = n; // 先把 n 计入答案
        BIT t(n + 1); // 下标从 1 开始
        int pre = 1; // 上一个最小值的位置，初始为 1
        for (int k = 0; k < n; ++k) {
            int i = id[k] + 1; // 下标从 1 开始
            if (i >= pre) // 从 pre 移动到 i，跳过已经删除的数
                ans += i - pre - t.query(pre, i);
            else // 从 pre 移动到 n，再从 1 移动到 i，跳过已经删除的数
                ans += n - pre + i - k + t.query(i, pre - 1);
            t.inc(i); // 删除 i
            pre = i;
        }
        return ans;
    }
};
复杂度分析
时间复杂度：O(nlog⁡n)\mathcal{O}(n\log n)O(nlogn)，其中 nnn 为 nums\textit{nums}nums 的长度。
空间复杂度：O(n)\mathcal{O}(n)O(n)。

```cpp
class SegmentTree {
private:
    vector<int> sum;
public:
    SegmentTree(int n) : sum(n << 2) {}
    // 单点插入
    void add(int root, int s, int t, int idx, int val = 1) {
        if (s == t) {
            sum[root] += val;
            return;
        }
        int mid = (s + t) >> 1;
        if (idx <= mid) add(root << 1, s, mid, idx, val);
        else add(root << 1 | 1, mid + 1, t, idx, val);
        sum[root] = sum[root << 1] + sum[root << 1 | 1];
    }
    int query(int root, int l, int r, int s, int t) {
        if (l <= s && t <= r) return sum[root];
        int mid = (s + t) >> 1;
        int ans = 0;
        if (l <= mid) ans += query(root << 1, l, r, s, mid);
        if (r > mid) ans += query(root << 1 | 1, l, r, mid + 1, t);
        return ans;
    }
};
class Solution {
public:
    long long countOperationsToEmptyArray(vector<int>& nums) {
        int n = nums.size(), idx[n];
        for (int i = 0; i < n; ++i) idx[i] = i + 1;
        sort(idx, idx + n, [&](int a, int b) {
            return nums[a - 1] < nums[b - 1];
        });
        long long ans = n, pre = 1;
        SegmentTree t(n);
        for (int k = 0; k < n; ++k) {
            int i = idx[k];
            if (pre <= i) ans += i - pre - t.query(1, pre, i, 1, n);
            else ans += n - pre + i + t.query(1, i, pre - 1, 1, n) - k; 
            t.add(1, 1, n, i);
            pre = i;
        }
        return ans;
    }
};
``` 
方法二：进一步挖掘性质
仍然先把数组长度 nnn 计入答案，后面只统计移动次数。在统计移动次数时，遇到要删除的元素，相当于可以免费向后移动一步（因为删除操作已经计入答案）。试想一下，如果数组是单调递增的，就没有任何额外的移动次数。

再次考察 nums=[2,4,1,3]\textit{nums}=[2,4,1,3]nums=[2,4,1,3]：

从元素 111 移动到元素 222，由于 222 在 111 左侧，说明必须走一整圈才能到 222，减去删除 111 产生的免费移动，移动次数为 n−1=4−1=3n-1=4-1=3n−1=4−1=3。
从元素 222 移动到元素 333，这里合并到下面计算。
从元素 333 移动到元素 444，由于 444 在 333 左侧，说明必须再走一整圈才能到 444，减去删除 2,32,32,3 产生的免费移动，减去跳过的 111，移动次数为 n−3=4−3=1n-3=4-3=1n−3=4−3=1。
总共移动 3+1=43+1=43+1=4 次，加上删除操作 444 次，故答案为 888。
这里说的「走一整圈」指从数组左端走到右端，再回到数组左端。

从上面的例子中可以发现，如果第 kkk 次要删除的元素在第 k−1k-1k−1 次要删除的元素的左侧，那么必须多走一整圈，移动次数为 n−kn-kn−k。累加，即为总的移动次数。

最后如果剩下若干递增元素，直接一股脑删除，无需任何移动次数。

class Solution {
public:
    long long countOperationsToEmptyArray(vector<int> &nums) {
        int n = nums.size(), id[n];
        iota(id, id + n, 0);
        sort(id, id + n, [&](int i, int j) { return nums[i] < nums[j]; });
        long long ans = n;
        for (int k = 1; k < n; ++k)
            if (id[k] < id[k - 1]) // 必须多走一整圈
                ans += n - k; // 减去前面删除的元素个数
        return ans;
    }
};
复杂度分析
时间复杂度：O(nlog⁡n)\mathcal{O}(n\log n)O(nlogn)，其中 nnn 为 nums\textit{nums}nums 的长度。瓶颈在排序上。
空间复杂度：O(n)\mathcal{O}(n)O(n)。

作者：灵茶山艾府
链接：https://leetcode.cn/problems/make-array-empty/solutions/2250783/shu-zhuang-shu-zu-mo-ni-pythonjavacgo-by-

                            第二种举个例子：[3,2,1,4,5,6,8,7,9]  
第一次idx_2=1<2=idx_1:[3,2,4,5,6,8,7,9].共经过n-1个数,回到 id[0].  
第二次idx_3=0<1=idx_2:[3,4,5,6,8,7,9].共经过n-2个数,回到 id[0].  
第三次idx_8=6<7=idx_7:[8,7,9].共经过2个数,回到 id[0].  
第四次:[8,9].共经过0个数,结束.

对于数组[3,2,5,1,4], 把它的id存入ids数组, 按nums[id]排序后得到的ids是: [3,1,0,4,2], 预期第0个是最小值, 但是这个例子里最小值的位置是3, i=3, pre=0, 那么先要把数字3,2,5分别移动3次到尾部, 这样得到了1这个值在数组第一个位置, 这个时候才可以删除, 那么次数=移动3次+删除1次=4, 继续遍历, 这个时候pre=3, i=1, 那么1左边的需要移动,pre=3后面的也需要移动, 次数等于1-0+4-3=2, 再加删除1位置的数字(次大), 循环标记直到所有的数字都标记完!

可以用树状数组/线段树维护nums每个下标是否已删除(0已删除，1未删除)，则移动次数就是相应区间中1的个数，即区间和。套个线段树模版...

```kotlin
var countOperationsToEmptyArray = function(nums) {
   const n = nums.length;
   let ans = 0;
   // 包含下标信息升序排序
   const arr = nums.map((item, idx) => [item, idx]).sort((a, b) => a[0] - b[0]);
   // 考虑用线段树维护nums每个下标是否已删除(0已删除，1未删除)，则移动次数就是相应区间中1的个数，即区间和
   const st = new SegTree(new Array(n).fill(1));
   // pre记录上次删除节点下标，便于统计每次删除需要统计移动次数的区间
   let pre = -1;
   for (let [_, i] of arr) {
       // 当前待删除的下标i>上次删除下标pre，移动pre->i
       if (i > pre) {
           // 区间查询:移动[pre,i]区间和
           ans += (st.querySum(pre, i));
       }
       // 当前待删除的下标i<上次删除下标pre，移动pre->n-1,0->i
       else if (i < pre) {
           // 区间查询:移动[pre,n-1]和[0,i]两段的区间和
           ans += (st.querySum(pre, n - 1) + st.querySum(0, i));
       }
       // 单点更新:i标记为0
       st.updateAdd(i, i, -1);
       pre = i;
   }
   return ans;
};
class SegTree {
   // 传入序列nums
   constructor(nums) {
       // 线段树的区间是[1,n]，需要将源序列索引右移一位对应
       this.a = nums.slice();
       this.a.unshift(0);
       this.n = nums.length;
       const N = 4 * this.n;
       this.t = new Array(N).fill(0).map(() => ({}));
       this._build();
   }
   ///对外区间查询/更新api，主要是将区间左移，这样外部调用api就可以直接用数组索引
   // 区间修改1:[L,R]区间加v
   updateAdd(L, R, v) {
       this.update(L + 1, R + 1, v);
   }
   // 区间查询1:[L,R]区间和
   querySum(L, R) {
       return this.query(L + 1, R + 1);
   }
   // 建树
   _build(u = 1, l = 1, r = this.n) {
       const t = this.t, a = this.a;
       if (l == r) {
           t[u].sum = a[l];
           return;
       }
       const mid = (l + r) >> 1;
       this._build(u << 1, l, mid), this._build(u << 1 | 1, mid + 1, r);
       this._pushup(u);
   }
   //
   _pushup(u) {
       const t = this.t;
       const lu = u << 1, ru = u << 1 | 1;
       t[u].sum = t[lu].sum + t[ru].sum;
   }
   // 向下传递lazytag(每次递归只向下传递一层)
   // 将当前节点的lazytag传给左右子节点(左右子节点更新节点值和lazytag(停止继续向下更新))
   _pushdown(u, l, r) {
       const t = this.t;
       const lu = u << 1, ru = u << 1 | 1, mid = (l + r) >> 1;
       // tad为lazytag
       if (t[u].tad) {
           this._push_add(lu, l, mid, t[u].tad);
           this._push_add(ru, mid + 1, r, t[u].tad);
       }
       t[u].tad = 0;
   }
   // 处理当前线段树节点的更新(单层更新，非递归更新)
   _push_add(u, l, r, v) {
       const t = this.t;
       // 更新节点值
       t[u].sum += (r - l + 1) * v;
       // 更新当前节点tag(停住，不再继续递归更新)
       // 这里将需要将更新累加到lazytag里(可以多次累加到lazytag然后一次更新其子节点值)
       t[u].tad += v;
   }
   // 区间修改:[L,R]区间加v
   update(L, R, v, u = 1, l = 1, r = this.n) {
       if (R < l || r < L) return;
       if (L <= l && r <= R) {
           return this._push_add(u, l, r, v);
       }
       const mid = (l + r) >> 1;
       this._pushdown(u, l, r);
       this.update(L, R, v, u << 1, l, mid);
       this.update(L, R, v, u << 1 | 1, mid + 1, r);
       this._pushup(u);
   }
   // 区间查询:[L,R]区间和
   query(L, R, u = 1, l = 1, r = this.n) {
       const t = this.t;
       if (R < l || r < L) return 0;
       if (L <= l && r <= R) {
           return t[u].sum;
       }
       const mid = (l + r) >> 1;
       this._pushdown(u, l, r);
       return this.query(L, R, u << 1, l, mid) + this.query(L, R, u << 1 | 1, mid + 1, r);
   }
}
```