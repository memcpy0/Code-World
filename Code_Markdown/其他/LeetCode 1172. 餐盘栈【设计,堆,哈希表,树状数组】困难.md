> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

我们把无限数量 ∞ 的栈排成一行，按从左到右的次序从 0 开始编号。每个栈的的最大容量 `capacity` 都相同。

实现一个叫「餐盘」的类 `DinnerPlates`：
-   `DinnerPlates(int capacity)` - 给出栈的最大容量 `capacity`。
-   `void push(int val)` - 将给出的正整数 `val` 推入 **从左往右第一个** 没有满的栈。
-   `int pop()` - 返回 **从右往左第一个** 非空栈顶部的值，并将其从栈中删除；如果所有的栈都是空的，请返回 `-1`。
-   `int popAtStack(int index)` - 返回编号 `index` 的栈顶部的值，并将其从栈中删除；如果编号 `index` 的栈是空的，请返回 `-1`。

**示例：**
```java
输入：
["DinnerPlates","push","push","push","push","push","popAtStack","push","push","popAtStack","popAtStack","pop","pop","pop","pop","pop"]
[[2],[1],[2],[3],[4],[5],[0],[20],[21],[0],[2],[],[],[],[],[]]
输出：
[null,null,null,null,null,null,2,null,null,20,21,5,4,3,1,-1]
解释：
DinnerPlates D = DinnerPlates(2);  // 初始化，栈最大容量 capacity = 2
D.push(1);
D.push(2);
D.push(3);
D.push(4);
D.push(5);         // 栈的现状为：   2  4
                                    1  3  5
                                    ﹈ ﹈ ﹈
D.popAtStack(0);   // 返回 2。栈的现状为：     4
                                          1  3  5
                                          ﹈ ﹈ ﹈
D.push(20);        // 栈的现状为：  20 4
                                   1  3  5
                                   ﹈ ﹈ ﹈
D.push(21);        // 栈的现状为：  20 4  21
                                   1  3  5
                                   ﹈ ﹈ ﹈
D.popAtStack(0);   // 返回 20。栈的现状为：      4 21
                                            1  3  5
                                            ﹈ ﹈ ﹈
D.popAtStack(2);   // 返回 21。栈的现状为：      4
                                            1  3  5
                                            ﹈ ﹈ ﹈ 
D.pop()            // 返回 5。栈的现状为：       4
                                            1  3 
                                            ﹈ ﹈  
D.pop()            // 返回 4。栈的现状为：   1  3 
                                           ﹈ ﹈   
D.pop()            // 返回 3。栈的现状为：   1 
                                           ﹈   
D.pop()            // 返回 1。现在没有栈。
D.pop()            // 返回 -1。仍然没有栈。
```
**提示：**
-   `1 <= capacity <= 20000`
-   `1 <= val <= 20000`
-   `0 <= index <= 100000`
-   最多会对 `push`，`pop`，和 `popAtStack` 进行 `200000` 次调用。

---
### 解法 栈数组+堆/有序集合（维护所有未满栈的下标）
第一次写时，我是维护最后一个非空的栈的位置，这样可以优化 $pop$ 的复杂度——这个很简单，$pop$ 的就是最后还存在的那个栈，如果栈已空就将该栈也出栈，这样就能轻松找到最后一个非空栈。**事实上，本题优化的关键点在于 $push$ 方法，$pop, popAtStack$ 都可做到 $O(1)$ 复杂度**。

第二次发现，还可以维护第一个非满的栈的位置，这样就能优化一下 $push$ 的复杂度——但仅仅是一下。试想：假如一开始连续执行了很多次 $push$ 操作，就会得到一排整整齐齐的栈；然后再随机挑选 $index$执行一些 $popAtStack$ 操作，就会把这些栈弄得参差不齐。
> 注：如果中间的一个栈清空了，那么它会继续占据着这个位置，它右侧的栈不会挤过来。

这个时候再交替执行 $push$ 和 $popAtStack$ ，「从左往右第一个没满的栈」就没有什么规律可言了。==如果把第一个未满栈填满，就要暴力寻找下一个未满栈了==。如何优化？**格局打开：与其维护第一个未满栈，不如维护所有未满栈**。

假设可以用一个数据结构来维护这些未满栈（的下标），看看需要哪些操作：
1. 对于 $push$ 来说，需要**知道这些下标中的最小值**。如果入栈后，栈满了，那么这个栈就不能算作未满栈，此时**这个最小下标就需要从数据结构中移除**。
2. 对于 $popAtStack$ 来说，如果操作的是一个满栈，操作后就得到了一个未满栈，那么就**往数据结构中添加这个栈的下标**。
3. 对于 $pop$ 来说，它等价于（可直接调用）$popAtStack(lastIndex)$ ，其中 $lastIndex$ 是最后一个非空栈的下标。

所以我们需要一个支持**添加元素、查询最小值和移除最小值**的数据结构。毫无疑问就是最小堆。

此外还需要一个列表 $stacks$ ，它的每个元素都是一个栈。上面说的 $lastIndex$ 就是 $stacks$ 的长度减一。如何维护 $stacks$ 呢？
- **如果 $push$ 时最小堆为空，说明所有栈都是满的，那么就需要向 $stacks$ 的末尾添加一个新的栈**。如果 $capacity>1$（即新栈未满），就**把这个新的未满栈的下标入堆**。
- 如果 $push$ 时最小堆非空，则说明还有未满栈，向栈顶下标所在的栈加入元素。**加入后如果该栈已满，则移除最小堆堆顶下标**。
- **如果 $popAtStack$ 操作前栈满，则把对应下标入堆**。操作后栈一定不满，甚至直接变为空栈，空栈看情况来移除。
- **如果 $popAtStack$ 操作的是最后一个栈，且操作后栈为空，那么就从 $stacks$ 中移除最后一个栈**。如果移除后 $stacks$ 的最后一个栈也是空的，就不断移除直到 $stacks$ 为空或者最后一个栈不为空，这样就能轻松找到最后一个非空栈。==细节：需要同步移除最小堆中的下标吗？其实不需要，而且也不一定能短时间做到这一点==，解决方法是——如果在 $push$ 时发现最小堆堆顶的下标大于等于 $\textit{stacks}$ 的长度，说明都是越界的非法下标，我们把整个堆清空即可。==这个技巧叫懒删除==。
- 此外，如果 $popAtStack$ 操作越界或者操作的栈为空，则返回 $-1$ 。

> 问：懒删除是否会导致堆中有重复的下标？
> 答：不会有重复下标。
> - 假设重复下标是在 $push$ 时插入的，说明此前所有栈都是满的，（懒删除时）堆中可能残留的下标必然都大于等于 $stacks$ 的长度，但这种情况下 $push$ 会清空堆，不会导致重复下标。
> - 假设重复下标是在 $popAtStack$ 时插入的，这只会发生在 $stacks[index]$ 先前是满栈的情况下，而 $push$ 保证在一个栈满时去掉它的下标，所以也不会导致重复下标。

```java
class DinnerPlates {
    private int capacity; // 栈的容量
    private List<Deque<Integer>> stacks = new ArrayList<>(); // 所有栈
    private PriorityQueue<Integer> idx = new PriorityQueue<>(); // 最小堆,保存未满栈的下标
    public DinnerPlates(int capacity) { 
        this.capacity = capacity;
    }
    public void push(int val) {
        // 堆中所有未满栈的下标都越界了,即这些未满栈都已被移除,因此直接清空堆中下标
        if (!idx.isEmpty() && idx.peek() >= stacks.size()) idx.clear(); 
        if (idx.isEmpty()) { // 所有目前加入了元素的栈都满了
            var s = new ArrayDeque<Integer>();
            s.push(val);
            stacks.add(s); // 添加一个新栈
            if (capacity > 1) idx.add(stacks.size() - 1); // 新栈未满
        } else { // 还有未满栈
            var s = stacks.get(idx.peek());
            s.push(val); // 入栈
            if (s.size() == capacity) idx.poll(); // 栈满则从堆中去掉
        }
    }
    public int pop() { return popAtStack(stacks.size() - 1); } // 等价为popAtStack最后一个非空栈
    public int popAtStack(int index) { // 非法下标,所有栈为空时从pop过来的下标可能<0
        if (index < 0 || index >= stacks.size() || stacks.get(index).isEmpty()) return -1;
        var s = stacks.get(index);
        if (s.size() == capacity) idx.add(index); // 满栈,元素出栈后就不满了,把下标入堆
        int val = s.pop(); 
        // 去掉末尾的空栈(懒删除,堆中对应下标在push时处理)
        while (!stacks.isEmpty() && stacks.get(stacks.size() - 1).isEmpty()) 
            stacks.remove(stacks.size() - 1);
        return val;
    }
}
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(q\log q)$ ，其中 $q$ 为操作次数。虽然在 $popAtStack$ 中有个循环，但是均摊考虑，被 $popAtStack$ 清空的栈，只贡献了一次循环，所以均摊来说时间复杂度为 $\mathcal{O}(\log q)$ 。
- 空间复杂度：$\mathcal{O}(q)$ 。

> 变形题目：添加方法 `bool pushAtStack(int index, int val)`（将 $val$ 推入编号为 $index$ 的栈，返回是否成功入栈），要怎么实现呢？
> 继续变形：如果 $index$ 处的栈满了，则推入编号大于 $index$ 的第一个未满栈。
> 
> 这有些复杂。
> - 将 $val$ 推入指定编号的栈，有可能指定编号比现有的栈编号大很多，这么看的话就**必须把栈队列离散化处理**——“所有栈”的数组不但要保存“栈”本身，还得保存下标，为了维护下标映射关系，得添加一个哈希表来维护 `栈下标 -> 所有栈中的数组下标` 的映射关系，其他的就和本题一样了。
> - 如果还有 $index$ 满了要求推入下一个未满这个要求，仅仅依靠堆应该无法实现，似乎得维护一个有序集合作为“未满栈”？==事实上，本题也可用栈数组+有序集合解决==。

---
### 解法2 树状数组+二分查找
本题的关键点在于两个目标栈：
- 从右往左寻找非空的第一个栈（对应 $pop$ 操作）
- 从左往右寻找非满的第一个栈（对应 $push$ 操作）

如何加速这两个查找操作呢？这又如何与树状数组扯上关系的呢？假设我们用 `nums[]` 对每个栈分别维护其栈中元素数目，元素出栈时减少对应数目，元素进栈时增加对应个数。此时从 `nums[]` 出发，还是只能通过线性扫描找到目标栈。如要在数组中加速查找，第一时间想到的就是二分，但 `nums[]` 不具有单调性，显然是不能用二分的。

这时可以想到**前缀和，它具有单调性，对其可以用二分来查找**。因此我们对数组 `nums` 维护一个前缀和的数组 `sums` 。
- 从右往左第一个非空栈，其前缀和一定等于总元素个数，即 `sums[x] >= size` 。因此我们**只需要找到最小的一个值 `x` 、满足 `sums[x] >= size`** ，这一问题可用二分下界：
	![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304282243858.png)
- 从左往右第一个非满栈，前面一定都是满栈，这个非满栈满足 `sums[x] > x * capacity` 。这个问题可以用二分上界。
	![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304282246260.png)
但是，每个栈的元素数目是会发生变化的（即 `nums[i]` 会发生变化），从而整个前缀和数组也要改变。应对这种单点修改、动态维护前缀和的方法之一就是**树状数组**，因此还要使用一个 `tree[]` 来管理 `nums[]` 的前缀和数组 `sums[]` 。

算法总体思路是：对每次 $push$ 操作，二分搜索找到
```java
const int N = 1e5 + 10; 
static int tree[N];
static stack<int> vs[N];

class DinnerPlates {
public:
    int cap, size;

    DinnerPlates(int capacity) {
        cap = capacity;
        size = 0;
        memset(tree, 0, sizeof(tree));
        for (int i = 0; i < N; ++i) while(!vs[i].empty()) vs[i].pop();
    }

    inline int lowbit(int x) { return x & -x; }

    inline void add(int x, int v) {
        for (int i = x; i <= N; i += lowbit(i)) tree[i] += v;
    }

    inline int getSum(int x) {
        int ret = 0;
        for (int i = x; i; i -= lowbit(i))  ret += tree[i];
        return ret;
    }

    // pop
    int getPop() {
        int left = 1, right = N;
        while (left < right) {
            int mid = (left + right) >> 1;
            if (getSum(mid) >= size) right = mid;
            else left = mid + 1;
        }
        return left;
    }

    // push
    int getPush() {
        int l = 1, r = N;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (cap * mid > getSum(mid)) r = mid;
            else l = mid + 1;
        }
        return l;
    }

    void push(int val) {
        int p = getPush();
        vs[p].push(val);
        ++size;
        add(p, 1);
    }

    int pop() {
        if (!size) return -1;
        int p = getPop();
        int top = vs[p].top();
        vs[p].pop();
        --size;
        add(p, -1);
        return top;
    }

    int popAtStack(int index) {
        int p = index + 1;
        if (vs[p].empty()) return -1;
        int top = vs[p].top();
        vs[p].pop();
        --size;
        add(p, -1);
        return top;
    }
};
 
```
二分的时间复杂度为 O(logn)O(logn)O(logn)，树状数组求前缀和是 O(logn)O(logn)O(logn)，动态维护的时间复杂度是 O(logn)O(logn)O(logn) 所以一次操作的时间复杂度为 O(logn)∗O(logn)+O(logn)=O(log2n)O(logn) * O(logn) + O(logn) = O(log^2n)O(logn)∗O(logn)+O(logn)=O(log 
2
 n)
---
### 解法3 线段树+二分查找


https://leetcode.cn/problems/dinner-plate-stacks/solutions/1340603/cxian-duan-shu-by-nreyog-fdon/
https://leetcode.cn/problems/dinner-plate-stacks/solutions/25250/1172-can-pan-zhan-shu-zhuang-shu-zu-shang-bei-zeng/