> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

 给你一个由非负整数 `a1, a2, ..., an` 组成的数据流输入，请你将到目前为止看到的数字总结为不相交的区间列表。

实现 `SummaryRanges` 类：
- `SummaryRanges()` 使用一个空数据流初始化对象。
- `void addNum(int val)` 向数据流中加入整数 `val` 。
- `int[][] getIntervals()` 以不相交区间 `[starti, endi]` 的列表形式返回对数据流中整数的总结。

**示例：**
```js
输入：
["SummaryRanges", "addNum", "getIntervals", "addNum", "getIntervals", "addNum", "getIntervals", "addNum", "getIntervals", "addNum", "getIntervals"]
[[], [1], [], [3], [], [7], [], [2], [], [6], []]
输出：
[null, null, [[1, 1]], null, [[1, 1], [3, 3]], null, [[1, 1], [3, 3], [7, 7]], null, [[1, 3], [7, 7]], null, [[1, 3], [6, 7]]]

解释：
SummaryRanges summaryRanges = new SummaryRanges();
summaryRanges.addNum(1);      // arr = [1]
summaryRanges.getIntervals(); // 返回 [[1, 1]]
summaryRanges.addNum(3);      // arr = [1, 3]
summaryRanges.getIntervals(); // 返回 [[1, 1], [3, 3]]
summaryRanges.addNum(7);      // arr = [1, 3, 7]
summaryRanges.getIntervals(); // 返回 [[1, 1], [3, 3], [7, 7]]
summaryRanges.addNum(2);      // arr = [1, 2, 3, 7]
summaryRanges.getIntervals(); // 返回 [[1, 3], [7, 7]]
summaryRanges.addNum(6);      // arr = [1, 2, 3, 6, 7]
summaryRanges.getIntervals(); // 返回 [[1, 3], [6, 7]]
```
**提示：**
- `0 <= val <= 10^4`
- 最多调用 `addNum` 和 `getIntervals` 方法 `3 * 10^4` 次

**进阶：** 如果存在大量合并，并且与数据流的大小相比，不相交区间的数量很小，该怎么办?

---
```cpp
class SummaryRanges {
private:
    typedef pair<int, int> pii;
    set<pii> st;
public:
    SummaryRanges() {

    }
    
    void addNum(int value) {
        int l = value, r = value;
        auto it = st.lower_bound(pii(l - 1, -2e4));
        while (it != st.end()) {
            if (it->second > value + 1) break;
            l = min(l, it->second);
            r = max(r, it->first);
            st.erase(it++);
        }
        st.insert(pii(r, l));
    }
    
    vector<vector<int>> getIntervals() {
        vector<vector<int>> ans;
        for (auto it : st) ans.push_back(vector<int>{it.second, it.first});
        return ans;
    }
};
```

方法一：使用有序映射维护区间
思路与算法

假设我们使用某一数据结构维护这些不相交的区间，在设计具体的数据结构之前，我们需要先明确 void addNum(int val)\texttt{void addNum(int val)}void addNum(int val) 这一操作会使得当前的区间集合发生何种变化：

情况一：如果存在一个区间 [l,r][l, r][l,r]，它完全包含 val\textit{val}val，即 l≤val≤rl \leq \textit{val} \leq rl≤val≤r，那么在加入 val\textit{val}val 之后，区间集合不会有任何变化；

情况二：如果存在一个区间 [l,r][l, r][l,r]，它的右边界 rrr「紧贴着」val\textit{val}val，即 r+1=valr + 1 = \textit{val}r+1=val，那么在加入 val\textit{val}val 之后，该区间会从 [l,r][l, r][l,r] 变为 [l,r+1][l, r+1][l,r+1]；

情况三：如果存在一个区间 [l,r][l, r][l,r]，它的左边界 lll「紧贴着」val\textit{val}val，即 l−1=vall - 1 = \textit{val}l−1=val，那么在加入 val\textit{val}val 之后，该区间会从 [l,r][l, r][l,r] 变为 [l−1,r][l - 1, r][l−1,r]；

情况四：如果情况二和情况三同时满足，即存在一个区间 [l0,r0][l_0, r_0][l 
0
​
 ,r 
0
​
 ] 满足 r0+1=valr_0+1 = \textit{val}r 
0
​
 +1=val，并且存在一个区间 [l1,r1][l_1, r_1][l 
1
​
 ,r 
1
​
 ] 满足 l1−1=vall_1-1 = \textit{val}l 
1
​
 −1=val，那么在加入 val\textit{val}val 之后，这两个区间会合并成一个大区间 [l0,r1][l_0, r_1][l 
0
​
 ,r 
1
​
 ]；

情况五：在上述四种情况均不满足的情况下，val\textit{val}val 会单独形成一个新的区间 [val,val][\textit{val}, \textit{val}][val,val]。

根据上面的五种情况，我们需要找到离 val\textit{val}val 最近的两个区间。用严谨的语言可以表述为：

对于给定的 val\textit{val}val，我们需要找到区间 [l0,r0][l_0, r_0][l 
0
​
 ,r 
0
​
 ]，使得 l0l_0l 
0
​
  是最大的满足 l0≤vall_0 \leq \textit{val}l 
0
​
 ≤val 的区间。同时，我们需要找到区间 [l1,r1][l_1, r_1][l 
1
​
 ,r 
1
​
 ]，使得 l1l_1l 
1
​
  是最小的满足 l1>vall_1 > \textit{val}l 
1
​
 >val 的区间。

如果我们的数据结构能够快速找到这两个区间，那么上面的五种情况分别对应为：

情况一：l0≤val≤l1l_0 \leq \textit{val} \leq l_1l 
0
​
 ≤val≤l 
1
​
 ；
情况二：r0+1=valr_0 + 1 = \textit{val}r 
0
​
 +1=val；
情况三：l1−1=vall_1 - 1 = \textit{val}l 
1
​
 −1=val；
情况四：r0+1=valr_0 + 1 = \textit{val}r 
0
​
 +1=val 并且 l1−1=vall_1 - 1 = \textit{val}l 
1
​
 −1=val；
情况五：上述情况均不满足。
一种可以找到「最近」区间的数据结构是有序映射。有序映射中的键和值分别表示区间的左边界 lll 和右边界 rrr。由于有序映射支持查询「最大的比某个元素小的键」以及「最小的比某个元素大的键」这两个操作，因此我们可以快速地定位区间 [l0,r0][l_0, r_0][l 
0
​
 ,r 
0
​
 ] 和 [l1,r1][l_1, r_1][l 
1
​
 ,r 
1
​
 ]。

除此之外，对于 int[][] getIntervals()\texttt{int[][] getIntervals()}int[][] getIntervals() 操作，我们只需要对有序映射进行遍历，将所有的键值对返回即可。

细节

在实际的代码编写中，需要注意 [l0,r0][l_0, r_0][l 
0
​
 ,r 
0
​
 ] 或 [l1,r1][l_1, r_1][l 
1
​
 ,r 
1
​
 ] 不存在的情况。

代码

C++
Java
Python3
Golang
class SummaryRanges {
private:
    map<int, int> intervals;

public:
    SummaryRanges() {}
    
    void addNum(int val) {
        // 找到 l1 最小的且满足 l1 > val 的区间 interval1 = [l1, r1]
        // 如果不存在这样的区间，interval1 为尾迭代器
        auto interval1 = intervals.upper_bound(val);
        // 找到 l0 最大的且满足 l0 <= val 的区间 interval0 = [l0, r0]
        // 在有序集合中，interval0 就是 interval1 的前一个区间
        // 如果不存在这样的区间，interval0 为尾迭代器
        auto interval0 = (interval1 == intervals.begin() ? intervals.end() : prev(interval1));

        if (interval0 != intervals.end() && interval0->first <= val && val <= interval0->second) {
            // 情况一
            return;
        }
        else {
            bool left_aside = (interval0 != intervals.end() && interval0->second + 1 == val);
            bool right_aside = (interval1 != intervals.end() && interval1->first - 1 == val);
            if (left_aside && right_aside) {
                // 情况四
                int left = interval0->first, right = interval1->second;
                intervals.erase(interval0);
                intervals.erase(interval1);
                intervals.emplace(left, right);
            }
            else if (left_aside) {
                // 情况二
                ++interval0->second;
            }
            else if (right_aside) {
                // 情况三
                int right = interval1->second;
                intervals.erase(interval1);
                intervals.emplace(val, right);
            }
            else {
                // 情况五
                intervals.emplace(val, val);
            }
        }
    }
    
    vector<vector<int>> getIntervals() {
        vector<vector<int>> ans;
        for (const auto& [left, right]: intervals) {
            ans.push_back({left, right});
        }
        return ans;
    }
};
复杂度分析

时间复杂度：

void addNum(int val)\texttt{void addNum(int val)}void addNum(int val) 的时间复杂度为 O(log⁡n)O(\log n)O(logn)，即为对有序映射进行常数次添加、删除、修改操作需要的时间。

int[][] getIntervals()\texttt{int[][] getIntervals()}int[][] getIntervals() 的时间复杂度为 O(n)O(n)O(n)，即为对有序映射进行一次遍历需要的时间。

空间复杂度：O(n)O(n)O(n)。在最坏情况下，数据流中的 nnn 个元素分别独自形成一个区间，有序映射中包含 nnn 个键值对。

作者：力扣官方题解
链接：https://leetcode.cn/problems/data-stream-as-disjoint-intervals/solutions/1037200/jiang-shu-ju-liu-bian-wei-duo-ge-bu-xian-hm1r/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

二分查找 + 朴素维护区间
一个朴素的做法是直接使用 ArrayList（数组）来维护所有的不相交区间，然后调用 addNum 时先通过二分找到当前值相邻的区间，然后根据题意进行模拟即可。

同时为了简化复杂的分情况讨论，起始时我们可以先往 ArrayList 添加两个哨兵分别代表正无穷和负无穷。

代码（不使用哨兵的代码见 P2P2P2）：

Java
Java
class SummaryRanges {
    List<int[]> list = new ArrayList<>();
    int[] head = new int[]{-10, -10}, tail = new int[]{10010, 10010};
    public SummaryRanges() {
        list.add(head);
        list.add(tail);
    }
    public void addNum(int val) {
        int n = list.size();
        int l = 0, r = n - 1;
        while (l < r) {
            int mid = l + r + 1 >> 1;
            if (list.get(mid)[0] <= val) l = mid;
            else r = mid - 1;
        }
        int[] cur = new int[]{val, val};
        int[] prev = list.get(r);
        int[] next = list.get(r + 1);
        if ((prev[0] <= val && val <= prev[1]) || (next[0] <= val && val <= next[1])) {
            // pass
        } else if (prev[1] + 1 == val && val == next[0] - 1) {
            prev[1] = next[1];
            list.remove(next);
        } else if (prev[1] + 1 == val) {
            prev[1] = val;
        } else if (next[0] - 1 == val) {
            next[0] = val;
        } else {
            list.add(r + 1, cur);
        }
    }
    public int[][] getIntervals() {
        int n = list.size();
        int[][] ans = new int[n - 2][2];
        int idx = 0;
        for (int i = 1; i < n - 1; i++) ans[idx++] = list.get(i);
        return ans;
    }
}
时间复杂度：令 mmm 为不交区间个数，addNum 操作中查询相邻区间的复杂度为 O(log⁡m)O(\log{m})O(logm)，维护不相交区间时，由于需要针对 listlistlist 的索引位置进行元素插入和删除，复杂度为 O(m)O(m)O(m)，整体复杂度为 O(m)O(m)O(m)；getIntervals 操作需要遍历所有的 listlistlist 元素，复杂度为 O(m)O(m)O(m)
空间复杂度：O(m)O(m)O(m)
二分查找 + 优化维护区间
解法一中虽然在 addNum 使用到了复杂度诶 O(log⁡m)O(\log{m})O(logm) 的二分查找，但在维护区间时，由于要对 listlistlist 进行针对索引的插入和删除，导致整体复杂度为 O(m)O(m)O(m)。

我们期望找到一种插入/删除操作复杂度同样为 O(log⁡m)O(\log{m})O(logm) 的方式来维护区间，这引导我们使用 TreeSet（红黑树）等数据结构。

具体做法不变，仍然是先使用「二分」（TreeSet 自带的 floor/ceiling）来找到当前值的相邻区间，

同时为了简化复杂的分情况讨论，起始时我们可以先往 TreeSet 添加两个哨兵分别代表正无穷和负无穷，以确保调用 floor/ceiling 时不会返回空。

代码：

Java
class SummaryRanges {
    TreeSet<int[]> ts = new TreeSet<>((a, b)->a[0]-b[0]);
    int[] head = new int[]{-10, -10}, tail = new int[]{10010, 10010};
    public SummaryRanges() {
        ts.add(head);
        ts.add(tail);
    }
    public void addNum(int val) {
        int[] cur = new int[]{val, val};
        int[] prev = ts.floor(cur);
        int[] next = ts.ceiling(cur);
        if ((prev[0] <= val && val <= prev[1]) || (next[0] <= val && val <= next[1])) {
            // pass
        } else if (prev[1] + 1 == val && next[0] - 1 == val) {
            prev[1] = next[1];
            ts.remove(next);
        } else if (prev[1] + 1 == val) {
            prev[1] = val;
        } else if (next[0] - 1 == val) {
            next[0] = val;
        } else {
            ts.add(cur);
        }
    }
    public int[][] getIntervals() {
        // using ceiling
        // int n = ts.size();
        // int[][] ans = new int[n - 2][2];
        // int[] cur = head;
        // for (int i = 0; i < n - 2; i++) {
        //     ans[i] = ts.ceiling(new int[]{cur[0] + 1, cur[1] + 1});
        //     cur = ans[i];
        // }
        // return ans;

        // using iterator
        int n = ts.size();
        int[][] ans = new int[n - 2][2];
        Iterator<int[]> iterator = ts.iterator();
        iterator.next();
        for (int i = 0; i < n - 2; i++) ans[i] = iterator.next();
        return ans;
    }
}
时间复杂度：令 mmm 为不交区间个数，addNum 操作中「查询相邻区间」和「维护不相交区间」的复杂度为 O(log⁡m)O(\log{m})O(logm)，整体复杂度为 O(log⁡m)O(\log{m})O(logm)；getIntervals 操作需要获取有序集合中的所有元素，使用 ceiling 复杂度为 O(mlog⁡m)O(m\log{m})O(mlogm)。使用 iterator 的话，由于 TreeSet 底层同步维护了一个 TreeMap，迭代器由 TreeMap 所提供，获取 iterator 来遍历所有元素的复杂度为 O(m)O(m)O(m)
空间复杂度：O(m)O(m)O(m)
进阶
如果存在大量合并，并且与数据流的大小相比，不相交区间的数量很小，该怎么办?

即需要确保 addNum 的复杂度，而对 getIntervals 复杂度要求不高，上述解法就已经是这么做的了。

作者：宫水三叶
链接：https://leetcode.cn/problems/data-stream-as-disjoint-intervals/solutions/1037331/gong-shui-san-xie-yi-ti-shuang-jie-er-fe-afrk/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。


理解题意
这道题描述太抽象了，我们先来理解下题意。

比如，我们输入的数字依次为 1、2、3、6、7，为什么输出的是 [1,3],[6,7]呢？



通过上图，可以看到，123是连在一起的，67是连在一起的，所以，最后输出的区间为 [1,3],[6,7]。

方法一、模拟
通过上面的图解相信你已经理解了题意，同时，题目约定了数据范围在 10410^410 
4
  以内，所以，我们可以开辟一个 10001 的数组记录每个数是否出现过，调用 getIntervals() 的时候再把区间合并返回即可。

请看代码：

class SummaryRanges {

    // 记录是否出现过，用boolean数组就可以了
    private boolean[] nums = new boolean[10001];

    public SummaryRanges() {
        
    }
    
    public void addNum(int val) {
        // 出现过标记为true
        nums[val] = true;
    }
    
    public int[][] getIntervals() {
        // 合并区间
        List<int[]> list = new ArrayList<>();
        int start = -1;
        int end = -1;
        for (int i = 0; i < 10001; i++) {
            if (nums[i]) {
                if (start == -1) {
                    start = i;
                    end = i;
                } else {
                    end = i;
                }
            } else {
                if (start != -1) {
                    list.add(new int[] {start, end});
                    start = -1;
                    end = -1;
                }
            }
        }
        // 最后一个元素可能有值
        if (start != -1) {
            list.add(new int[] {start, end});
        }

        // 转换成int[][]返回
        return list.toArray(new int[list.size()][2]);
    }
}
时间复杂度：addNum() 为 O(1)O(1)O(1)，getIntervals() 为 O(C)O(C)O(C)，CCC 固定为10001。
空间复杂度：O(C)O(C)O(C)，CCC 固定为10001。
运行结果如下：



方法二、并查集
通过方法一，可以发现，每次调用 getIntervals() 方法的时候都要全部计算一遍，导致效率并不高，所以，我们能不能把这个时间复杂度转移到 addNum() 方法呢？

答案是可以的，我们可以使用并查集来进行优化。

比如，还是输入 1、2、3、6、7，使用并查集后的图示如下：



我们同样开辟一个 10001 长度的数组，这个数组中的每个元素记录的是自己的父元素是谁，这个父元素呢就是我们区间的右边界，这样，我们在调用 getIntervals() 的时候，遍历到下标为 1 的元素时，可以通过 finx(x) 快速找到区间的右边界，下一次的循环从右边界的下一位开始遍历即可。

请看代码：

class SummaryRanges {

    private int[] father = new int[10001];

    public SummaryRanges() {
        // 元素值的范围为 0~10000，所以，初始化为-1
        Arrays.fill(father, -1);
    }
    
    public void addNum(int val) {
        if (father[val] == -1) {
            father[val] = val;
            // 先合并右边，再合并左边
            union(val, val + 1);
            union(val - 1, val);
        }
    }

    private int find(int x) {
        if (father[x] != x) return father[x] = find(father[x]);
        return father[x];
    }

    private void union(int x, int y) {
        if (x >= 0 && x <= 10000 && father[x] != -1 && father[y] != -1) {
            int fx = find(x);
            int fy = find(y);
            if (fx != fy) {
                father[fx] = fy;
            }
        }
    }
    
    public int[][] getIntervals() {
        List<int[]> list = new ArrayList<>();

        for (int i = 0; i < 10001;) {
            if (father[i] != -1) {
                int start = i;
                // 快速找到右边界
                int end = find(i);
                list.add(new int[] {start, end});
                i = end + 1;
            } else {
                i++;
            }
        }

        return list.toArray(new int[list.size()][2]);
    }
}
时间复杂度：addNum() 为 O(1)O(1)O(1)，find(x) 的过程有压缩，所以是很快的，均摊后为 O(1)O(1)O(1)；getIntervals() 为 O(C)O(C)O(C)，CCC 固定为10001，但是实际上比 O(C)O(C)O(C) 要小很多。
空间复杂度：O(C)O(C)O(C)，CCC 固定为10001。
运行结果如下，从结果来看并没有明显的加速很多：



方法三、并查集 + 优化
通过方法二，我们可以看到效率依然很低，我们再想想有没有其他方法可以优化。

方法二中，虽然计算右边界加速了，但是，左边界依然要遍历整个数组才能拿到，所以，我们可以思考一下能不能使用什么容器把左边界也存储下来呢？

答案是可以的，我们这里可以使用有序的Set存储左边界，在插入元素和合并元素的时候做一些相关的处理即可。

请看代码：

class SummaryRanges {

    // 存储右边界
    private int[] father = new int[10001];
    // 使用有序的set存储左边界
    private Set<Integer> set = new TreeSet<>();

    public SummaryRanges() {
        // 元素值的范围为 0~10000，所以，初始化为-1
        Arrays.fill(father, -1);
    }
    
    public void addNum(int val) {
        if (father[val] == -1) {
            father[val] = val;
            // 将当前元素加入set中
            set.add(val);
            // 先合并右边，再合并左边
            union(val, val + 1);
            union(val - 1, val);
        }
    }

    private int find(int x) {
        if (father[x] != x) return father[x] = find(father[x]);
        return father[x];
    }

    private void union(int x, int y) {
        if (x >= 0 && x <= 10000 && father[x] != -1 && father[y] != -1) {
            int fx = find(x);
            int fy = find(y);
            if (fx != fy) {
                father[fx] = fy;
                // 可以合并，set中删除右边的那个数
                set.remove(y);
            }
        }
    }
    
    public int[][] getIntervals() {
        List<int[]> list = new ArrayList<>();

        // 遍历set拿到左边界
        for (int start : set) {
            int end = find(start);
            list.add(new int[] {start, end});
        }

        return list.toArray(new int[list.size()][2]);
    }
}
时间复杂度：addNum() 为 O(logk)O(logk)O(logk)，find(x) 的过程有压缩，所以是很快的，均摊后为 O(1)O(1)O(1)，TreeSet 的插入和删除是 O(logk)O(logk)O(logk) 的时间复杂度，k为TreeSet中元素的数量，即分段的数量；getIntervals() 为 O(k)O(k)O(k)，k为TreeSet中元素的数量。
空间复杂度：O(C)O(C)O(C)，CCC 固定为10001，TreeSet中存储的元素数量是远小于 C 的。
运行结果如下，可以看到加速了很多：



进阶
进阶： 如果存在大量合并，并且与数据流的大小相比，不相交区间的数量很小，该怎么办?

并查集合并的过程主要是调用 find(x) 找父元素， find(x) 调用的次数越多，越扁平，均摊的时间复杂度越小，所以，存在大量合并时，我们的算法更优。

作者：彤哥来刷题啦
链接：https://leetcode.cn/problems/data-stream-as-disjoint-intervals/solutions/1037841/tong-ge-lai-shua-ti-la-yi-ti-san-jie-mo-tpqtc/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。