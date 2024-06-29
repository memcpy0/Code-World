/*
** Problem: https://leetcode.cn/problems/snapshot-array
** Article: https://memcpy0.blog.csdn.net/article/details/138247498
** Author: memcpy0
*/
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