> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Range模块是跟踪数字范围的模块。设计一个数据结构来跟踪表示为 **半开区间** 的范围并查询它们。

**半开区间** `[left, right)` 表示所有 `left <= x < right` 的实数 `x` 。

实现 `RangeModule` 类:
- `RangeModule()` 初始化数据结构的对象。
- `void addRange(int left, int right)` 添加 **半开区间** `[left, right)`，跟踪该区间中的每个实数。添加与当前跟踪的数字部分重叠的区间时，应当添加在区间 `[left, right)` 中尚未跟踪的任何数字到该区间中。
- `boolean queryRange(int left, int right)` 只有在当前正在跟踪区间 `[left, right)` 中的每一个实数时，才返回 `true` ，否则返回 `false` 。
- `void removeRange(int left, int right)` 停止跟踪 **半开区间** `[left, right)` 中当前正在跟踪的每个实数。

**示例 1：**
```java
输入
["RangeModule", "addRange", "removeRange", "queryRange", "queryRange", "queryRange"]
[[], [10, 20], [14, 16], [10, 14], [13, 15], [16, 17]]
输出
[null, null, null, true, false, true]
解释
RangeModule rangeModule = new RangeModule();
rangeModule.addRange(10, 20);
rangeModule.removeRange(14, 16);
rangeModule.queryRange(10, 14); 返回 true （区间 [10, 14) 中的每个数都正在被跟踪）
rangeModule.queryRange(13, 15); 返回 false（未跟踪区间 [13, 15) 中像 14, 14.03, 14.17 这样的数字）
rangeModule.queryRange(16, 17); 返回 true （尽管执行了删除操作，区间 [16, 17) 中的数字 16 仍然会被跟踪）
```
**提示：**
- `1 <= left < right <= 10^9`
- 在单个测试用例中，对 `addRange` 、  `queryRange` 和 `removeRange` 的调用总数不超过 `10^4` 次

---
### 解法1 有序集合 / 有序映射
可使用有序集合或者有序映射来**实时维护所有的区间**。**在任意一次 $addRange$ 或 $removeRange$ 操作后，我们需要保证有序集合中的区间两两不能合并成一个更大的连续区间**。也就是说：如果当前有序集合中有 $k$ 个区间 $[l_1, r_1), [l_2, r_2), \cdots, [l_k, r_k)$ ，那么需要保证：
$$l_1 < r_1 < l_2 < r_2 < \cdots < l_k < r_k$$
成立。这样一来 $queryRange$ 操作就会变得非常方便：对于 $\text{queryRange(left, right)}$ 而言，只需要判断是否存在一个区间 $[l_i, r_i)$ ，满足 $l_i \leq \textit{left} < \textit{right} \leq r_i$ 即可。

接下来详细讲解如何处理 $addRange$ 或 $removeRange$ 和操作。对于 $\text{addRange(left, right)}$ 操作，**我们首先在有序集合上进行二分**，找出**最后一个满足 $l_i \leq \textit{left}$ 的区间 $[l_i, r_i)$**（==處理前面==），那么会有如下的四种情况：
- 如果不存在这样的区间，那么我们可以忽略这一步；
- 如果 $l_i \leq \textit{left} < \textit{right} \leq r_i$ ，即 $[l_i, r_i)$ 完全包含待添加的区间，那么我们不需要进行任何操作，可以直接返回；
- 如果 $l_i \leq \textit{left} \leq r_i < \textit{right}$ ，**要删除区间 $[l_i, r_i)$并把 $\textit{left}$ 置为 $l_i$** 。此时 $[\textit{left}, \textit{right})$ 就表示待添加区间与 $[l_i, r_i)$​ 的并集；注意 $left \le r_i$ 。
- 如果 $l_i < r_i < \textit{left} < \textit{right}$ ，那么我们也可以忽略这一步。

随后，我们遍历 $[l_i, r_i)$ 之后的区间（如果前面不存在满足要求的 $[l_i, r_i)$ ，那么就从头开始遍历），**这些区间 $[l_j, r_j)$ ，都满足 $l_j > \textit{left}$** ，那么只要 $l_j \leq \textit{right}$ ，$[l_j, r_j)$ 就可以与 $[\textit{left}, \textit{right})$ 合并成一个更大的连续区间。当遍历到 $l_j > \textit{right}$ 时，**根据集合的有序性，之后的所有区间都不会和 $[\textit{left}, \textit{right})$ 有交集，就可以结束遍历。

在遍历完成后，我们还需要将 $[\textit{left}, \textit{right})$ 加入有序集合中。

对于 $\text{removeRange(left, right)}$ 操作，处理方法是类似的，首先在有序集合上进行二分，找出**最后一个满足 $l_i \leq \textit{left}$ 的区间 $[l_i, r_i)$**（==處理前面==），那么会有如下的四种情况：
 )，那么会有如下的四种情况：
 - 如果不存在这样的区间，那么我们可以忽略这一步；
- 如果 $l_i \leq \textit{left} \le \textit{right} \leq r_i$ ，即 $[l_i, r_i)$ 完全包含待添加的区间，那麼 $l_i \le left \le right \le r_i$ 的删除会导致 $[l_i, r_i)$ 变成两个短区间：$[l_i, left), [right, r_i)$ 。如果 $left = l_i$ ，那么第一个区间为空区间；如果 $right = r_i$ ，那么第二个区间为空区间。==我们将 $[l_i, r_i)$ 删除后，向有序集合中添加所有的非空集合，即可直接返回==。
- 如果 $l_i \leq \textit{left} \lt r_i < \textit{right}$ ，**要把区间 $[l_i, r_i)$ 变成 $[l_i,left)$** 。特别地，如果 $left = l_i$ ，我们可以直接把这个区间删除；
- 如果 $l_i < r_i \le \textit{left} < \textit{right}$ ，那么我们也可以忽略这一步。

随后，我们遍历 $[l_i, r_i)$ 之后的区间，这些区间 $[l_j, r_j)$ 都满足 $l_j > \textit{left}$ ，那么只要 $l_j < \textit{right}$ ​，$[l_j, r_j)$ 中的一部分就会被删除。
- 如果 $r_j \leq \textit{right}$ ，那么 $[l_j, r_j)$ 会被完全删除；
- 如果 $r_j > \textit{right}$ ，那么 $[l_j, r_j)$ 会剩下 $[\textit{right}, r_j)$ ，此时之后的所有区间都不会和 $[\textit{left}, \textit{right})$ 有交集，就可以结束遍历。

最后，对于 $\text{queryRange(left, right)}$ 操作，我们同样在有序集合上进行二分，找出最后一个满足 $l_i \leq \textit{left}$ 的区间 $[l_i, r_i)$ 。如果 $l_i \leq \textit{left} < \textit{right} \leq r_i$ ，那么返回 $True$ ，否则返回 $False$ 。

代码

C++
Java
Python3
Golang
class RangeModule {
public:
    RangeModule() {}
    
    void addRange(int left, int right) {
        auto it = intervals.upper_bound(left);
        if (it != intervals.begin()) {
            auto start = prev(it);
            if (start->second >= right) {
                return;
            }
            if (start->second >= left) {
                left = start->first;
                intervals.erase(start);
            }
        }
        while (it != intervals.end() && it->first <= right) {
            right = max(right, it->second);
            it = intervals.erase(it);
        }
        intervals[left] = right;
    }
    
    bool queryRange(int left, int right) {
        auto it = intervals.upper_bound(left);
        if (it == intervals.begin()) {
            return false;
        }
        it = prev(it);
        return right <= it->second;
    }
    
    void removeRange(int left, int right) {
        auto it = intervals.upper_bound(left);
        if (it != intervals.begin()) {
            auto start = prev(it);
            if (start->second >= right) {
                int ri = start->second;
                if (start->first == left) {
                    intervals.erase(start);
                }
                else {
                    start->second = left;
                }
                if (right != ri) {
                    intervals[right] = ri;
                }
                return;
            }
            else if (start->second > left) {
                if (start->first == left) {
                    intervals.erase(start);
                }
                else {
                    start->second = left;
                }
            }
        }
        while (it != intervals.end() && it->first < right) {
            if (it->second <= right) {
                it = intervals.erase(it);
            }
            else {
                intervals[right] = it->second;
                intervals.erase(it);
                break;
            }
        }
    }

private:
    map<int, int> intervals;
};
复杂度分析

时间复杂度：对于操作 $queryRange$，时间复杂度为 O(log⁡(a+r))O(\log(a+r))O(log(a+r))，其中 aaa 是操作 $addRange$ 的次数，rrr 是操作 $removeRange$ 的次数。对于操作 $addRange$ 和 $removeRange$，时间复杂度为均摊 O(log⁡(a+r))O(\log(a+r))O(log(a+r))，这是因为 $addRange$ 操作最多添加一个区间，$removeRange$ 最多添加两个区间，每一个添加的区间只会在未来的操作中被移除一次，因此均摊时间复杂度为对有序集合 / 有序映射常数次操作需要的时间，即为 O(log⁡(a+r))O(\log(a+r))O(log(a+r))。

空间复杂度：O(a+r)O(a+r)O(a+r)，即为有序集合 / 有序映射需要使用的空间。



作者：力扣官方题解
链接：https://leetcode.cn/problems/range-module/solutions/1608992/range-mo-kuai-by-leetcode-solution-4utf/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

```cpp
class RangeModule {
private:
    typedef pair<int, int> pii;
    set<pii> st;
public:
    RangeModule() { }
    
    void addRange(int left, int right) {
        int L = left, R = right - 1; // [L,R]
        auto it = st.lower_bound(pii(L - 1, -2e9));
        while (it != st.end()) { // pii[r] >= L-1,可合并
            if (it->second > right) break;
            L = min(L, it->second);
            R = max(R, it->first);
            st.erase(it++);
        }
        st.insert(pii(R, L));
    }
    
    bool queryRange(int left, int right) {
        int L = left, R = right - 1; // [L,R]
        auto it = st.lower_bound(pii(L, -2e9));
        while (it != st.end()) { // pii[r]>=L,可能包含区间[L,R]
            if (it->second > right) return false;
            if (it->first >= R && it->second <= L) return true;
            ++it;
        }
        return false;
    }
    
    void removeRange(int left, int right) {
        int L = left, R = right - 1; // [L,R]
        auto it = st.lower_bound(pii(L, -2e9));
        while (it != st.end()) {
            if (it->second > right) break;
            if (it->first >= R && it->second <= L) { // 只有这个区间包含[L,R]
                int l1 = it->second, l2 = R + 1;
                int r1 = L - 1, r2 = it->first;
                st.erase(it);
                if (r1 >= l1) st.insert(pii(r1, l1));
                if (r2 >= l2) st.insert(pii(r2, l2));
                return;
            } else if (it->first < R) { //   lj < R 
                if (it->second <= L) {
                    int l = it->second;
                    st.erase(it++);
                    if (l != L)
                        st.insert(pii(L - 1, l));
                } else st.erase(it++);
            } else if (it->second > L) {
                if (it->first >= R) {
                    int r = it->first;
                    st.erase(it++);
                    if (r != R) 
                        st.insert(pii(r, R + 1));
                }
            }
        }
    }
};

/**
 * Your RangeModule object will be instantiated and called as such:
 * RangeModule* obj = new RangeModule();
 * obj->addRange(left,right);
 * bool param_2 = obj->queryRange(left,right);
 * obj->removeRange(left,right);
 */
```

这道题来自：[Weekly Contest 55](https://leetcode.com/contest/leetcode-weekly-contest-55/)，，写了一个傻瓜式的线段树解法，从不会到通过debug了好久😰

线段树😪😪按照我自己的理解写得，哎，，：`1、`首先初始化线段树，总树最大区间`[0,1e9]`；`2、`添加区间操作：需要递归找到存在的最小的可以覆盖要添加的区间的那个子树，假如这个子树的covered参数是true，那么无需进一步操作，，否则需要向下继续找到那些最大的，存在的，并且可以被添加区间包裹住的子树（或者把这些最大的区间new出来），并且把这些区间的covered参数设为true，同时删除所有它们的子区间；`3、`查询操作：递归查找所有最大的，可以被所查询区间包裹住的所有最靠上的那些子树，这些子树必须全是covered==true，且不能是空；`4、`区间删除，这个操作很麻烦：首先假如一处区间对应的那个树是空，那么说明这些数字根本不存在，无需操作，，（1）假如移除到了某个子树的时候covered为true：首先如果移除的区间内可以精准覆盖这个子树，那么直接把covered设为false后删除所有它的子树即可，，其次如果无法精准覆盖，那么需要恢复这个子树的左右子树并设covered为true，再分别进行处理，，，（2）假如说碰到了某个子树的covered参数为false，这时只能说明这里的数字`不全存在，而不是全不存在`，那么需要递归继续操作左右子树（千万不要就此终止呀），最后不管如何要把covered设为false

```java
/*
@可爱抱抱呀
执行用时：49 ms, 在所有 Java 提交中击败了44.72%的用户
内存消耗：50.7 MB, 在所有 Java 提交中击败了21.43%的用户
2022年6月19日 15:56
*/
class SegTree{
    int l,r;//表示的是这个闭区间代表的左右边界
    SegTree left,right;
    boolean covered;//区间内的数字是否都被覆盖了
    public SegTree(int l,int r){
        this.l=l;
        this.r=r;
        this.covered=false;
    }
    public SegTree(int l,int r,boolean covered){
        this.l=l;
        this.r=r;
        this.covered=covered;
    }
}
class RangeModule {
    SegTree st;
    public RangeModule() {
        st=new SegTree(0,(int)1e9);
    }
    
    public void addRange(int left, int right) {
        addNum(st,left,right-1);
    }
    void addNum(SegTree t,int a,int b){
        //此方法在线段树中添加闭区间[a,b]
        int l=t.l,r=t.r,mid=(l+r)>>1;
        if(t.covered){return;}//区间内数字已经全部存在，无需进一步操作
        if(l>=a&&r<=b){
            //完全精准覆盖（所有添加操作最后一步都会到这里），此时只需要把这个区间设为true，删除所有子区间即可
            t.covered=true;
            t.left=t.right=null;
        }
        else if(b<=mid){
            //只更新左半边即可
            if(t.left==null){t.left=new SegTree(l,mid);}
            addNum(t.left,a,b);
        }
        else if(a>mid){
            //只更新右半边即可
            if(t.right==null){t.right=new SegTree(mid+1,r);}
            addNum(t.right,a,b);
        }
        else{
            //同时更新左右半边
            if(t.left==null){t.left=new SegTree(l,mid);}            
            if(t.right==null){t.right=new SegTree(mid+1,r);}
            addNum(t.left,a,mid);
            addNum(t.right,mid+1,b);
        }
    }
    
    public boolean queryRange(int left, int right) {
        return allCovered(st,left,right-1);
    }
    boolean allCovered(SegTree t,int a,int b){
        //此方法判断在闭区间[a,b]的所有数字是否都存在
        if(t==null){return false;}
        int l=t.l,r=t.r,mid=(l+r)>>1;
        if(t.covered){return true;}//所有的判断操作最后叶处都会分为这样的小操作
        if(b<=mid){return allCovered(t.left,a,b);}//在左边判断
        else if(a>mid){return allCovered(t.right,a,b);}//在右边判断
        return allCovered(t.left,a,mid)&&allCovered(t.right,mid+1,b);//在两边判断
    }
    
    public void removeRange(int left, int right) {
        removeNum(st,left,right-1);
    }
    void removeNum(SegTree t,int a,int b){
        //此方法移除树内闭区间[a,b]的所有数字
        if(t==null){return;}//区间不存在，说明本来就不存在这些数字
        int l=t.l,r=t.r,mid=(l+r)>>1;
        if(l>b||r<a){return;}
        if(l>=a&&r<=b){
            //如果删除的区间完全包括树的区间，那么设为false后，删除所有子区间
            t.left=t.right=null;
        }
        else if(t.covered){
            //此时线段树区间大于删除区间且区间内数字都存在，那么需要恢复左右子树后进行操作
            t.left=new SegTree(l,mid,true);
            t.right=new SegTree(mid+1,r,true);
            if(b<=mid){
                //需要处理左半部分，但是需要保持右半部分的情况与t相同，剩余的部分要纹丝不动
                removeNum(t.left,a,b);
            }
            else if(a>mid){
                //需要处理右半部分，但是需要保持左半部分的情况与t相同，剩余的部分要纹丝不动
                removeNum(t.right,a,b);
            }
            else{
                //需要处理左右两部分   
                removeNum(t.left,a,mid);
                removeNum(t.right,mid+1,b);
            }
        }
        else{
            //此时树内的数字不全存在，需要进一步看左右子树的情况后再进行操作
            if(b<=mid){removeNum(t.left,a,b);}
            else if(a>mid){removeNum(t.right,a,b);}
            else{
                removeNum(t.left,a,mid);
                removeNum(t.right,mid+1,b);
            }
        }    
        t.covered=false;//这个区间内的数字已经不全存在，因此要设成false
    }
}

/**
 * Your RangeModule object will be instantiated and called as such:
 * RangeModule obj = new RangeModule();
 * obj.addRange(left,right);
 * boolean param_2 = obj.queryRange(left,right);
 * obj.removeRange(left,right);
 */
```
官方题解看的人崩溃，从外网搬运下简洁解法，太绝

```java
class RangeModule {
   TreeMap<Integer, Integer> m = new TreeMap<>();
   public RangeModule() {}
   
   public void addRange(int s, int e) { // s: start, e: end
       // find overlap ranges, calc merged range, clear overlapped ranges, insert merged range
       var L = m.floorEntry(s); // left possible overlap entry
       var R = m.floorEntry(e); // right possible overlap entry

       if (L != null && L.getValue() >= s) s = L.getKey(); // update overlap start
       if (R != null && R.getValue() > e) e = R.getValue(); // update overlap end

       m.subMap(s, e).clear(); // clear all overlapped entries
       m.put(s, e); // save final merged entry
   }
   
   public boolean queryRange(int s, int e) {
       var L = m.floorEntry(s);
       return L != null && L.getValue() >= e; // if there exist a range: start <+ s, end >= e
   }
   
   public void removeRange(int s, int e) {
       var L = m.floorEntry(s); // left possible overlap entry
       var R = m.floorEntry(e); // right possible overlap entry

       if (L != null && L.getValue() > s) m.put(L.getKey(), s); // after removal, if anything left
       if (R != null && R.getValue() > e) m.put(e, R.getValue()); // after removal, if anything left

       m.subMap(s, e).clear(); // removal
   }
}
```


有问题的代码：
class RangeModule {

private:

    typedef pair<int, int> pii;

    set<pii> st;

public:

    RangeModule() { }

    void addRange(int left, int right) {

        int L = left, R = right - 1; // [L,R]

        auto it = st.lower_bound(pii(L - 1, -2e9));

        while (it != st.end()) { // R >= L-1

            if (it->second > right) break;

            L = min(L, it->second);

            R = max(R, it->first);

            st.erase(it++);

        }

        st.insert(pii(R, L));

    }

    bool queryRange(int left, int right) {

        int L = left, R = right - 1; // [L,R]

        auto it = st.lower_bound(pii(L - 1, -2e9 - 1));

        while (it != st.end()) {

            if (it->second > right) return false;

            if (it->first >= R && it->second <= L) return true;

            ++it;

        }

        return false;

    }

    void removeRange(int left, int right) {

        int L = left, R = right - 1; // [L,R]

        auto it = st.lower_bound(pii(L - 1, -2e9 - 1));

        while (it != st.end()) {

            if (it->second > right) break;

            if (it->first == R && it->second == L) {

                st.erase(it);

                return;

            }

            if (it->first >= R && it->second <= L) {

                int l1 = it->second, l2 = R + 1;

                int r1 = L - 1, r2 = it->first;

                if (r1 >= l1) st.insert(pii(l1, r1));

                if (r2 >= l2) st.insert(pii(l2, r2));

            }

        }

    }

};


基本分析
线段树（动态开点）写过超多次了。

令 mmm 为 addRange、queryRange 和 removeRange 的调用总数，n=1e9n = 1e9n=1e9 为值域大小。

由于值域过大，我们无法直接使用空间大小固定为 4×n4 \times n4×n 的常规线段树，而要采用「动态开点」的方式，其中动态开点的方式有两种 :「需要进行估点的数组实现」和「无须估点的动态指针」。

设计 Node 节点维护什么信息：

ls 和 rs 分别指向左右区间子节点（当采用「估点数组」方式时，记录的是左右区间子节点在线段树数组中的下标；在「动态指针」方式时，记录的是左右区间子节点对象）；
sum 为记录当前区间有多少个整数被追踪；
add 为懒标记，当 add = -1 代表 removeRange 懒标记，当 add = 1 则代表 addRange 懒标记。
线段树（动态开点）- 数组估点
对于常规的线段树实现来说，都是一开始就调用 build 操作创建空树，而线段树一般以「满二叉树」的形式用数组存储，因此需要 4×n4 \times n4×n 的空间，并且这些空间在起始 build 空树的时候已经锁死。

如果一道题仅仅是「值域很大」的离线题（提前知晓所有的询问），我们还能通过「离散化」来进行处理，将值域映射到一个小空间去，从而解决 MLE 问题。

但对于本题而言，由于「强制在线」的原因，我们无法进行「离散化」，同时值域大小达到 1e91e91e9 级别，因此如果我们想要使用「线段树」进行求解，只能采取「动态开点」的方式进行。

动态开点的优势在于，不需要事前构造空树，而是在插入操作 add 和查询操作 query 时根据访问需要进行「开点」操作。由于我们不保证查询和插入都是连续的，因此对于父节点 uuu 而言，我们不能通过 u << 1 和 u << 1 | 1 的固定方式进行访问，而要将节点 tr[u]tr[u]tr[u] 的左右节点所在 tr 数组的下标进行存储，分别记为 ls 和 rs 属性。对于 tr[u].ls=0tr[u].ls = 0tr[u].ls=0 和 tr[u].rs=0tr[u].rs = 0tr[u].rs=0 则是代表子节点尚未被创建，当需要访问到它们，而又尚未创建的时候，则将其进行创建。

由于存在「懒标记」，线段树的插入和查询都是 log⁡n\log{n}logn 的，因此我们在单次操作的时候，最多会创建数量级为 log⁡n\log{n}logn 的点，因此空间复杂度为 O(mlog⁡n)O(m\log{n})O(mlogn)，而不是 O(4×n)O(4 \times n)O(4×n)，而开点数的预估需不能仅仅根据 log⁡n\log{n}logn 来进行，还要对常熟进行分析，才能得到准确的点数上界。

动态开点相比于原始的线段树实现，本质仍是使用「满二叉树」的形式进行存储，只不过是按需创建区间，如果我们是按照连续段进行查询或插入，最坏情况下仍然会占到 4×n4 \times n4×n 的空间，因此盲猜 log⁡n\log{n}logn 的常数在 444 左右，保守一点可以直接估算到 666，因此我们可以估算点数为 6×m×log⁡n6 \times m \times \log{n}6×m×logn，其中 n=1e9n = 1e9n=1e9 和 m=1e4m = 1e4m=1e4 分别代表值域大小和查询次数。

当然一个比较实用的估点方式可以「尽可能的多开点数」，利用题目给定的空间上界和我们创建的自定义类（结构体）的大小，尽可能的多开（ Java 的 128M 可以开到 5×1065 \times 10^65×10 
6
  以上）。

代码：

Java
class RangeModule {
    class Node {
        int ls, rs, sum, add;
    }
    int N = (int)1e9 + 10, M = 500010, cnt = 1;
    Node[] tr = new Node[M];
    void update(int u, int lc, int rc, int l, int r, int v) {
        int len = rc - lc + 1;
        if (l <= lc && rc <= r) {
            tr[u].sum = v == 1 ? len : 0;
            tr[u].add = v;
            return ;
        }
        pushdown(u, len);
        int mid = lc + rc >> 1;
        if (l <= mid) update(tr[u].ls, lc, mid, l, r, v);
        if (r > mid) update(tr[u].rs, mid + 1, rc, l, r, v);
        pushup(u);
    }
    int query(int u, int lc, int rc, int l, int r) {
        if (l <= lc && rc <= r) return tr[u].sum;
        pushdown(u, rc - lc + 1);
        int mid = lc + rc >> 1, ans = 0;
        if (l <= mid) ans = query(tr[u].ls, lc, mid, l, r);
        if (r > mid) ans += query(tr[u].rs, mid + 1, rc, l, r);
        return ans;
    }
    void pushdown(int u, int len) {
        if (tr[u] == null) tr[u] = new Node();
        if (tr[u].ls == 0) {
            tr[u].ls = ++cnt;
            tr[tr[u].ls] = new Node();
        }
        if (tr[u].rs == 0) {
            tr[u].rs = ++cnt;
            tr[tr[u].rs] = new Node();
        }
        if (tr[u].add == 0) return;
        if (tr[u].add == -1) {
            tr[tr[u].ls].sum = tr[tr[u].rs].sum = 0;
        } else {
            tr[tr[u].ls].sum = len - len / 2;
            tr[tr[u].rs].sum = len / 2;
        }
        tr[tr[u].ls].add = tr[tr[u].rs].add = tr[u].add;
        tr[u].add = 0;
    }
    void pushup(int u) {
        tr[u].sum = tr[tr[u].ls].sum + tr[tr[u].rs].sum;
    }
    public void addRange(int left, int right) {
        update(1, 1, N - 1, left, right - 1, 1);
    }
    public boolean queryRange(int left, int right) {
        return query(1, 1, N - 1, left, right - 1) == right - left;
    }
    public void removeRange(int left, int right) {
        update(1, 1, N - 1, left, right - 1, -1);
    }
}
时间复杂度：addRange、queryRange 和 removeRange 操作复杂度均为 O(log⁡n)O(\log{n})O(logn)
空间复杂度：O(mlog⁡n)O(m\log{n})O(mlogn)
线段树（动态开点）- 动态指针
利用「动态指针」实现的「动态开点」可以有效避免数组估点问题，更重要的是可以有效避免 new 大数组的初始化开销，对于 LC 这种还跟你算所有样例总时长的 OJ 来说，在不考虑 static 优化/全局数组优化 的情况下，动态指针的方式要比估点的方式来得好。

代码：

Java
class RangeModule {
    class Node {
        Node ls, rs;
        int sum, add;
    }
    int N = (int)1e9 + 10;
    Node root = new Node();
    void update(Node node, int lc, int rc, int l, int r, int v) {
        int len = rc - lc + 1;
        if (l <= lc && rc <= r) {
            node.sum = v == 1 ? len : 0;
            node.add = v;
            return ;
        }
        pushdown(node, len);
        int mid = lc + rc >> 1;
        if (l <= mid) update(node.ls, lc, mid, l, r, v);
        if (r > mid) update(node.rs, mid + 1, rc, l, r, v);
        pushup(node);
    }
    int query(Node node, int lc, int rc, int l, int r) {
        if (l <= lc && rc <= r) return node.sum;
        pushdown(node, rc - lc + 1);
        int mid = lc + rc >> 1, ans = 0;
        if (l <= mid) ans = query(node.ls, lc, mid, l, r);
        if (r > mid) ans += query(node.rs, mid + 1, rc, l, r);
        return ans;
    }
    void pushdown(Node node, int len) {
        if (node.ls == null) node.ls = new Node();
        if (node.rs == null) node.rs = new Node();
        if (node.add == 0) return ;
        int add = node.add;
        if (add == -1) {
            node.ls.sum = node.rs.sum = 0;
        } else {
            node.ls.sum = len - len / 2;
            node.rs.sum = len / 2;
        }
        node.ls.add = node.rs.add = add;
        node.add = 0;
    }
    void pushup(Node node) {
        node.sum = node.ls.sum + node.rs.sum;
    }
    public void addRange(int left, int right) {
        update(root, 1, N - 1, left, right - 1, 1);
    }
    public boolean queryRange(int left, int right) {
        return query(root, 1, N - 1, left, right - 1) == right - left;
    }
    public void removeRange(int left, int right) {
        update(root, 1, N - 1, left, right - 1, -1);
    }
}
时间复杂度：addRange、queryRange 和 removeRange 操作复杂度均为 O(log⁡n)O(\log{n})O(logn)
空间复杂度：O(mlog⁡n)O(m\log{n})O(mlogn)

作者：宫水三叶
链接：https://leetcode.cn/problems/range-module/solutions/1612952/by-ac_oier-i4sw/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。