> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

实现支持下列接口的「快照数组」- SnapshotArray：
- `SnapshotArray(int length)` - 初始化一个与指定长度相等的 类数组 的数据结构。**初始时，每个元素都等于** **0**。
- `void set(index, val)` - 会将指定索引 `index` 处的元素设置为 `val`。
- `int snap()` - 获取该数组的快照，并返回快照的编号 `snap_id`（快照号是调用 `snap()` 的总次数减去 `1`）。
- `int get(index, snap_id)` - 根据指定的 `snap_id` 选择快照，并返回该快照指定索引 `index` 的值。

**示例：**
```java
输入：["SnapshotArray","set","snap","set","get"]
     [[3],[0,5],[],[0,6],[0,0]]
输出：[null,null,0,null,5]
解释：
SnapshotArray snapshotArr = new SnapshotArray(3); // 初始化一个长度为 3 的快照数组
snapshotArr.set(0,5);  // 令 array[0] = 5
snapshotArr.snap();  // 获取快照，返回 snap_id = 0
snapshotArr.set(0,6);
snapshotArr.get(0,0);  // 获取 snap_id = 0 的快照中 array[0] 的值，返回 5
```
**提示：**
- `1 <= length <= 50000`
- 题目最多进行`50000` 次 `set`，`snap`，和 `get`的调用 。
- `0 <= index < length`
- `0 <= snap_id <` 我们调用 `snap()` 的总次数
- `0 <= val <= 10^9`

---
### 解法 哈希表+二分查找
调用 $snap()$ 时，复制一份当前数组，作为「历史版本」。返回这是第几个历史版本（从 $0$ 开始）。

调用 $get(index,snapId)$ 时，返回第 $snapId$ 个历史版本的下标为 $\textit{index}$ 的元素值。

暴力？每次调用 $snap()$ ，就复制一份数组，可以吗？不行，最坏情况下，复制 $50000$ 次长为 $50000$ 的数组，会「超出内存限制」。

假设每调用一次 $set$ ，就生成一个快照（复制一份数组）。仅仅是一个元素发生变化，就去复制整个数组，这太浪费了。

能否不复制数组呢？换个视角，调用 $set(\textit{index}, \textit{val})$ 时，不去修改数组，而是往**下标为 $\textit{index}$ 的历史修改记录末尾**添加一条数据：此时的快照编号和 $val$ 。有点像**解决哈希冲突的拉链法**。

举例说明：
- 在快照编号等于 $2$ 时，调用 $set(0, 6)$ 。
- 在快照编号等于 $3$ 时，调用 $set(0,1)$ 。
- 在快照编号等于 $3$ 时，调用 $set(0,7)$ 。
- 在快照编号等于 $5$ 时，调用 $set(0,2)$ 。

这四次调用结束后，下标 $0$ 的历史修改记录 $\textit{history}[0] = [(2,6),(3,1),(3,7),(5,2)]$ ，每个数对中的第一个数为调用 $set$ 时的快照编号，第二个数为调用 $set$ 时传入的 $val$ 。注意**历史修改记录中的快照编号是有序的**。

那么：
- 调用 $get(0,4)$ 。由于历史修改记录中的快照编号是有序的，我们可以在 $history[0]$ 中二分查找快照编号 $\le 4$ 的最后一条修改记录，即 $(3,7)$ 。修改记录中的 $val=7$ 就是答案。
- 调用 $get(0,1)$ 。在 $history[0]$ 中，快照编号 $\le 1$ 的记录不存在，说明在快照编号 $≤1$ 时，我们并没有修改下标 $0$ 保存的元素，返回初始值 $0$ 。

对于 $snap$，只需把当前快照编号加一（快照编号初始值为 $0$ ），返回加一前的快照编号。
```python
class SnapshotArray:

    def __init__(self, length: int):
        self.cur_snap_id = 0
        self.history = defaultdict(list) # 每个index的历史修改记录都是list

    def set(self, index: int, val: int) -> None:
        self.history[index].append((self.cur_snap_id, val))

    def snap(self) -> int:
        self.cur_snap_id += 1
        return self.cur_snap_id - 1

    def get(self, index: int, snap_id: int) -> int:
        # 找快照编号 <= snap_id 的最后一次修改记录
        # 等价于找快照编号 >= snap_id+1 的第一个修改记录，它的上一个就是答案
        j = bisect_left(self.history[index], (snap_id + 1, )) - 1
        return self.history[index][j][1] if j >= 0 else 0
```

```java
class SnapshotArray {
    private final Map<Integer, List<int[]>> history = new HashMap<>();
    private int curSnapId; // 当前快照编号，初始值为0
    public SnapshotArray(int length) {
    }
    
    public void set(int index, int val) {
        history.computeIfAbsent(index, k -> new ArrayList<>())
            .add(new int[]{ curSnapId, val });
    }
    
    public int snap() {
        return curSnapId++;
    }
    
    public int get(int index, int snap_id) {
        if (!history.containsKey(index)) return 0;
        List<int[]> h = history.get(index);
        int j = search(h, snap_id);
        return j < 0 ? 0 : h.get(j)[1];
    }

    // 返回最大的下标i,满足 h[i][0]<=x
    // 如果不存在则返回-1
    private int search(List<int[]> h, int x) {
        // 开区间(left, right)
        int left = -1;
        int right = h.size();
        while (left + 1 < right) { // 区间不为空
            // 循环不变量
            // h[left][0] <= x
            // h[right][0] > x
            int mid = left + (right - left) / 2;
            if (h.get(mid)[0] <= x) {
                left = mid; // 区间缩小为(mid, right)
            } else {
                right = mid; // 区间缩小为(left, mid)
            }
        }
        // 根据循环不变量，此时 h[left][0]<=x 且 h[left+1][0] = h[right][0] > x
        // 所以left是最大的满足 h[left][0]<=x 的下标
        // 如果不存在，则left为其初始值-1
        return left;
    }
}
```

```cpp
class SnapshotArray {
private:
    int cur_snap_id = 0;
    unordered_map<int, vector<pair<int, int>>> history; // 每个index的历史修改记录
public:
    SnapshotArray(int length) {}
    
    void set(int index, int val) {
        history[index].emplace_back(cur_snap_id, val);
    }
    
    int snap() {
        return cur_snap_id++;
    }
    
    int get(int index, int snap_id) {
        auto& h = history[index];
        // 找快照编号 <= snap_id 的最后一次修改记录
        // 等价于找快照编号 >= snap_id+1 的第一个修改记录，它的上一个就是答案
        int j = ranges::lower_bound(h, make_pair(snap_id + 1, 0)) - h.begin() - 1;
        return j >= 0 ? h[j].second : 0;
    }
};
```

复杂度分析：
- 时间复杂度：初始化、$\texttt{set}set$、$\texttt{snap}$ 均为 $\mathcal{O}(1)$ ，$\texttt{get}$ 为 $\mathcal{O}(\log q)$ ，其中 $q$ 为 $\texttt{set}$ 的调用次数。
- 空间复杂度：$\mathcal{O}(q)$ 。

