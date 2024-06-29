> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你区间的 **空** 集，请你设计并实现满足要求的数据结构：
- **新增：** 添加一个区间到这个区间集合中。
- **统计：** 计算出现在 **至少一个** 区间中的整数个数。

实现 `CountIntervals` 类：
- `CountIntervals()` 使用区间的空集初始化对象
- `void add(int left, int right)` 添加区间 `[left, right]` 到区间集合之中。
- `int count()` 返回出现在 **至少一个** 区间中的整数个数。

**注意：** 区间 `[left, right]` 表示满足 `left <= x <= right` 的所有整数 `x` 。

**示例 1：**
```js
输入
["CountIntervals", "add", "add", "count", "add", "count"]
[[], [2, 3], [7, 10], [], [5, 8], []]
输出
[null, null, null, 6, null, 8]

解释
CountIntervals countIntervals = new CountIntervals(); // 用一个区间空集初始化对象
countIntervals.add(2, 3);  // 将 [2, 3] 添加到区间集合中
countIntervals.add(7, 10); // 将 [7, 10] 添加到区间集合中
countIntervals.count();    // 返回 6
                           // 整数 2 和 3 出现在区间 [2, 3] 中
                           // 整数 7、8、9、10 出现在区间 [7, 10] 中
countIntervals.add(5, 8);  // 将 [5, 8] 添加到区间集合中
countIntervals.count();    // 返回 8
                           // 整数 2 和 3 出现在区间 [2, 3] 中
                           // 整数 5 和 6 出现在区间 [5, 8] 中
                           // 整数 7 和 8 出现在区间 [5, 8] 和区间 [7, 10] 中
                           // 整数 9 和 10 出现在区间 [7, 10] 中
```
**提示：**
- `1 <= left <= right <= 10^9`
- 最多调用  `add` 和 `count` 方法 **总计** `10^5` 次
- 调用 `count` 方法至少一次

---
### 解法1 set
**求区间并集模板题**，是[715. Range 模块](https://leetcode.cn/problems/range-module/) 的子问题。用一个 `set` 有序地维护所有不相交的区间，当加入区间 `[left, right]` 时，通过 `lower_bound` 快速找到「**第一个右端点大于等于 `left - 1` 的区间**」，然后不断用接下来的区间和 `[left, right]` 合并，**直到当前区间的左端点大于 `right + 1`** 。

为什么用 `left - 1` 、`right + 1` 比较，直接 `left` 和 `right` 不行吗？ **这是为了把虽不相交但连续的区间也合并**，比如 $[1,3], [4, 6]$ 可以合并成 $[1, 6]$ 。当然，对于本题而言不需要这样的合并，只把相交的区间合并、直接用 `left` 和 `right` 判断也可以，但作为模板的角度来说，合并可能更合适一点。
```cpp
class CountIntervals {
private:
    typedef pair<int, int> pii;
    int ans = 0;
    set<pii> st;
public:
    CountIntervals() {}
    void add(int left, int right) {
        int L = left, R = right;
        // 这里存储区间为(R,L)
        // (R1,L1) >= (R2,L2), 若 R1 > R2 或 (R1 = R2 且 L1 >= L2)
        // R1>R2 且 L1<=L2 -> (R1,L1) > (R2,L2)，但两个集合不能并存！
        auto it = st.lower_bound(pii(L - 1, -2e9)); // right>=L-1
        while (it != st.end()) {
            if (it->second > right + 1) break; // 区间的l>right+1,两个区间无法合并
            L = min(L, it->second);
            R = max(R, it->first); // 按照set的设计,后面的it->first越来越大
            ans -= it->first - it->second + 1;
            st.erase(it++); // it++返回旧值再++,删除的是旧it指向的区间,it到了下个区间
            // 或改成 it = st.erase(it);
        }
        ans += R - L + 1;
        st.insert(pii(R, L));
    }
    int count() { return ans; }
};
```
复杂度分析：
- 时间复杂度：由于每个区间只会加入以及离开 `set` 一次，复杂度 $O(n \log n)$ 。
- 空间复杂度：$O(n)$

---
### 解法2 珂朵莉树
用一颗平衡树维护若干个不相交的区间，每次 $add(left,right)$ 时，删除被该区间覆盖到的区间（部分覆盖也算），然后将这些区间与 $[\textit{left},\textit{right}]$ 合并成一个新的大区间，插入平衡树中。

代码实现时，**为方便找到第一个被 $[\textit{left},\textit{right}]$ 覆盖到的区间，我们可以用平衡树的 $\textit{key}$ 存区间右端点，$\textit{value}$ 存区间左端点。我们要找的就是第一个 $\textit{key}\ge\textit{left}$ 的区间**。与解法1很相似。
```java
class CountIntervals {
    map<int, int> m;
    int cnt = 0; // 所有区间长度和
public:
    CountIntervals() {}

    void add(int left, int right) {
        // 遍历所有被 [left,right] 覆盖到的区间（部分覆盖也算）
        for (auto it = m.lower_bound(left); it != m.end() && it->second <= right; m.erase(it++)) {
            int l = it->second, r = it->first;
            left = min(left, l);   // 合并后的新区间，其左端点为所有被覆盖的区间的左端点的最小值
            right = max(right, r); // 合并后的新区间，其右端点为所有被覆盖的区间的右端点的最大值
            cnt -= r - l + 1;
        }
        cnt += right - left + 1;
        m[right] = left; // 所有被覆盖到的区间与 [left,right] 合并成一个新区间
    }
    int count() { return cnt; }
};
```
复杂度分析：
- 时间复杂度：每个区间至多被添加删除各一次，因此 $add$ 操作是均摊 $O(\log n)$ 的，这里 $n$ 是 $add$ 的次数。
- 空间复杂度：$O(n)$ 。

---
### 解法3 动态开点线段树
原理见 [这篇文章](https://leetcode.cn/link/?target=https%3A%2F%2Fzhuanlan.zhihu.com%2Fp%2F246255556)。完整的动态开点线段树模板见 [大佬的算法竞赛模板库](https://leetcode.cn/link/?target=https%3A%2F%2Fgithub.com%2FEndlessCheng%2Fcodeforces-go%2Fblob%2Fmaster%2Fcopypasta%2Fsegment_tree.go)。对本题来说，线段树的每个节点可以保存对应范围的左右端点 $l$ 和 $r$ ，以及范围内 $add$ 过的整数个数 $cnt$ 。

代码实现时，无需记录lazy tag，这是**因为被覆盖的范围无需再次覆盖**，因此**若 $cnt$ 等于范围的长度 $r - l + 1$ ，则可直接返回**。

1e9是节点范围，但普通线段树维护4倍的节点数量，所以内存不够，可以离散化处理。不过这道题强制在线，所以就要用动态开点线段树了。
```java
class CountIntervals {
    CountIntervals *left = nullptr, *right = nullptr;
    int l, r, cnt = 0;

public:
    CountIntervals() : l(1), r(1e9) {}

    CountIntervals(int l, int r) : l(l), r(r) {}

    void add(int L, int R) { // 为方便区分变量名，将递归中始终不变的入参改为大写（视作常量）
        if (cnt == r - l + 1) return; // 当前节点已被完整覆盖，无需执行任何操作
        if (L <= l && r <= R) { // 当前节点已被区间 [L,R] 完整覆盖，不再继续递归
            cnt = r - l + 1;
            return;
        }
        int mid = (l + r) / 2;
        if (left == nullptr) left = new CountIntervals(l, mid); // 动态开点
        if (right == nullptr) right = new CountIntervals(mid + 1, r); // 动态开点
        if (L <= mid) left->add(L, R);
        if (mid < R) right->add(L, R);
        cnt = left->cnt + right->cnt;
    }

    int count() { return cnt; }
};
```



直接hash建树。

class CountIntervals {
public:
    CountIntervals() {

    }
    
    void update(int start, int end, int l, int r, int idx) {
        if (r < start || end < l) {
            return;
        }
        if (tree[idx] == r - l + 1) {
            return;
        } 
        if (start <= l && r <= end) {
            tree[idx] = r - l + 1;
        } else {
            int mid = (l + r) >> 1;
            if (end <= mid) {
                update(start, end, l, mid, 2 * idx);
            } else if (start > mid) {
                update(start, end, mid + 1, r, 2 * idx + 1);
            } else {
                update(start, mid, l, mid, 2 * idx);
                update(mid + 1, end, mid + 1, r, 2 * idx + 1);
            }            
            tree[idx] = tree[2 * idx] + tree[2 * idx + 1];
        }
    }
    
    void add(int left, int right) {
        update(left, right, 1, 1e9, 1);
    }
    
    int count() {
        return tree[1];
    }
private:
    unordered_map<int,int> tree;
};

贴一个懒更新的动态开点线段树版本：

public class CountIntervals {

  private static class SegmentTreeNode {
    SegmentTreeNode leftChild, rightChild;
    long left, right;
    long sum, lazy;

    public SegmentTreeNode(long left, long right) {
      this.left = left;
      this.right = right;
    }
  }

  private SegmentTreeNode root = new SegmentTreeNode(1, 1000000000);

  private void spread(SegmentTreeNode root) {
    if (root.lazy != 0) {
      long mid = (root.left + root.right) / 2;
      if (null == root.leftChild) {
        root.leftChild = new SegmentTreeNode(root.left, mid);
      }
      root.leftChild.sum = root.leftChild.right - root.leftChild.left + 1;
      if (null == root.rightChild) {
        root.rightChild = new SegmentTreeNode(mid + 1, root.right);
      }
      root.rightChild.sum = root.rightChild.right - root.rightChild.left + 1;
      root.leftChild.lazy = root.lazy;
      root.rightChild.lazy = root.lazy;
      root.lazy = 0;
    }
  }

  private void update(SegmentTreeNode root, long left, long right) {
    if (left <= root.left && root.right <= right) {
      root.sum = root.right - root.left + 1;
      root.lazy = 1;
      return;
    }
    spread(root);
    long mid = (root.left + root.right) / 2;
    if (root.leftChild == null) {
      root.leftChild = new SegmentTreeNode(root.left, mid);
    }
    if (root.rightChild == null) {
      root.rightChild = new SegmentTreeNode(mid + 1, root.right);
    }
    if (left <= mid) {
      update(root.leftChild, left, right);
    }
    if (mid < right) {
      update(root.rightChild, left, right);
    }
    root.sum = root.leftChild.sum + root.rightChild.sum;
  }

  public CountIntervals() {

  }

  public void add(int left, int right) {
    update(root, left, right);
  }

  public int count() {
    return (int) root.sum;
  }

}
 
原来线段树动态开点就是归并啊，学会了
 

