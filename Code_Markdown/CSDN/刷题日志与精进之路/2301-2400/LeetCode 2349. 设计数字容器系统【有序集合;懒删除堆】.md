> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

设计一个数字容器系统，可以实现以下功能：
- 在系统中给定下标处 **插入** 或者 **替换** 一个数字。
- **返回** 系统中给定数字的最小下标。

请你实现一个 `NumberContainers` 类：
- `NumberContainers()` 初始化数字容器系统。
- `void change(int index, int number)` 在下标 `index` 处填入 `number` 。如果该下标 `index` 处已经有数字了，那么用 `number` 替换该数字。
- `int find(int number)` 返回给定数字 `number` 在系统中的最小下标。如果系统中没有 `number` ，那么返回 `-1` 。

**示例：**
```c
输入：
["NumberContainers", "find", "change", "change", "change", "change", "find", "change", "find"]
[[], [10], [2, 10], [1, 10], [3, 10], [5, 10], [10], [1, 20], [10]]
输出：
[null, -1, null, null, null, null, 1, null, 2]
解释：
NumberContainers nc = new NumberContainers();
nc.find(10); // 没有数字 10 ，所以返回 -1 。
nc.change(2, 10); // 容器中下标为 2 处填入数字 10 。
nc.change(1, 10); // 容器中下标为 1 处填入数字 10 。
nc.change(3, 10); // 容器中下标为 3 处填入数字 10 。
nc.change(5, 10); // 容器中下标为 5 处填入数字 10 。
nc.find(10); // 数字 10 所在的下标为 1 ，2 ，3 和 5 。因为最小下标为 1 ，所以返回 1 。
nc.change(1, 20); // 容器中下标为 1 处填入数字 20 。注意，下标 1 处之前为 10 ，现在被替换为 20 。
nc.find(10); // 数字 10 所在下标为 2 ，3 和 5 。最小下标为 2 ，所以返回 2 。
```
**提示：**
- `1 <= index, number <= 10^9`
- 调用 `change` 和 `find` 的 **总次数** 不超过 `10^5` 次。
### 方法一 哈希表+有序集合
为了实现 `find` ，我们需要对每个 $number$ 创建一个有序集合，维护这个 $number$ 对应的所有下标。**用有序集合可以快速获取最小下标**。

对于 `change` ，如果 $index$ 处有数字，我们需要先删除旧的数字，所以还需要知道每个 $index$ 对应的 $number$ 是多少，这可用一个哈希表记录。

具体来说，创建一个哈希表 $indexToNumber$ ，以及一个哈希表套有序集合 $numberToIndices$ 。

对于 `change` ：
- 如果 $index$ 处有数字 $x$ ，那么从 $numberToIndices[x]$ 中删除 $index$（删除旧的数据）。
- 然后，更新（或者插入）$indexToNumber[index] = number$ ，往 $numberToIndices[number]$ 中添加 $index$ 。

对于 `find` ，获取 $numberToIndices[number]$ 中的最小元素即可。
```cpp
class NumberContainers {
    unordered_map<int, int> index_to_number;
    unordered_map<int, set<int>> number_to_indices;

public:
    void change(int index, int number) {
        // 移除旧数据
        auto it = index_to_number.find(index);
        if (it != index_to_number.end()) {
            number_to_indices[it->second].erase(index);
        }

        // 添加新数据
        index_to_number[index] = number;
        number_to_indices[number].insert(index);
    }

    int find(int number) {
        auto it = number_to_indices.find(number);
        return it == number_to_indices.end() || it->second.empty() ? -1 : *it->second.begin();
    }
};
```
```java
class NumberContainers {
    private final Map<Integer, Integer> indexToNumber = new HashMap<>();
    private final Map<Integer, TreeSet<Integer>> numberToIndices = new HashMap<>();

    public void change(int index, int number) {
        // 移除旧数据
        Integer oldNumber = indexToNumber.get(index);
        if (oldNumber != null) {
            numberToIndices.get(oldNumber).remove(index);
        }

        // 添加新数据
        indexToNumber.put(index, number);
        numberToIndices.computeIfAbsent(number, _ -> new TreeSet<>()).add(index);
    }

    public int find(int number) {
        TreeSet<Integer> indices = numberToIndices.get(number);
        return indices == null || indices.isEmpty() ? -1 : indices.first();
    }
}
```
```rust
use std::collections::{BTreeSet, HashMap};

struct NumberContainers {
    index_to_number: HashMap<i32, i32>,
    number_to_indices: HashMap<i32, BTreeSet<i32>>,
}

/** 
 * `&self` means the method takes an immutable reference.
 * If you need a mutable reference, change it to `&mut self` instead.
 */
impl NumberContainers {
    fn new() -> Self {
        Self {
            index_to_number: HashMap::new(),
            number_to_indices: HashMap::new(),
        }
    }
    
    fn change(&mut self, index: i32, number: i32) {
        if let Some(&prev) = self.index_to_number.get(&index) {
            if prev != 0 {
                if let Some(set) = self.number_to_indices.get_mut(&prev) {
                    set.remove(&index);
                }
            }
        }
        self.index_to_number.insert(index, number);
        self.number_to_indices.entry(number).or_insert_with(BTreeSet::new).insert(index);
    }
    
    fn find(&mut self, number: i32) -> i32 {
        self.number_to_indices.get(&number).and_then(|s| s.iter().next().copied()).unwrap_or(-1)
    }
}
```
```python
class NumberContainers:
    def __init__(self):
        self.index_to_number = {}
        # from sortedcontainers import SortedSet
        self.number_to_indices = defaultdict(SortedSet)

    def change(self, index: int, number: int) -> None:
        # 移除旧数据
        old_number = self.index_to_number.get(index, None)
        if old_number is not None:
            self.number_to_indices[old_number].discard(index)

        # 添加新数据
        self.index_to_number[index] = number
        self.number_to_indices[number].add(index)

    def find(self, number: int) -> int:
        indices = self.number_to_indices[number]
        return indices[0] if indices else -1
```
```go
// import "github.com/emirpasic/gods/v2/trees/redblacktree"
type NumberContainers struct {
	indexToNumber   map[int]int
	numberToIndices map[int]*redblacktree.Tree[int, struct{}]
}

func Constructor() NumberContainers {
	return NumberContainers{map[int]int{}, map[int]*redblacktree.Tree[int, struct{}]{}}
}

func (n NumberContainers) Change(index, number int) {
	// 移除旧数据
	if oldNumber, ok := n.indexToNumber[index]; ok {
		n.numberToIndices[oldNumber].Remove(index)
	}

	// 添加新数据
	n.indexToNumber[index] = number
	if n.numberToIndices[number] == nil {
		n.numberToIndices[number] = redblacktree.New[int, struct{}]()
	}
	n.numberToIndices[number].Put(index, struct{}{})
}

func (n NumberContainers) Find(number int) int {
	indices, ok := n.numberToIndices[number]
	if !ok || indices.Empty() {
		return -1
	}
	return indices.Left().Key
}
```
复杂度分析：
- 时间复杂度：
	- 初始化 $O(1)$ 。
	- `change` ：$O(\log q)$ ，其中 $q$ 是 `change` 的调用次数。
	- `find` ：$O(\log q)$ 或 $O(1)$ ，取决于有序集合是否额外维护最小值。
- 空间复杂度：$O(q)$ 。
---
### 方法二 哈希表+懒删除堆
$numberToIndices$ 改成哈希表套最小堆。

对于 `change` ，不删除旧数据。

对于 `find` ，查看**堆顶的下标对应的值**是否等于 $number$ ，若不相同，则意味着堆顶是之前没有删除的旧数据，弹出堆顶；否则堆顶就是答案。
```cpp
class NumberContainers {
    unordered_map<int, int> index_to_number;
    unordered_map<int, priority_queue<int, vector<int>, greater<int>>> number_to_indices;

public:
    void change(int index, int number) {
        // 添加新数据
        index_to_number[index] = number;
        number_to_indices[number].push(index);
    }

    int find(int number) {
        auto& indices = number_to_indices[number];
        while (!indices.empty() && index_to_number[indices.top()] != number) {
            indices.pop(); // 堆顶货不对板，说明是旧数据，删除
        }
        return indices.empty() ? -1 : indices.top();
    }
};
```
```java
class NumberContainers {
    private final Map<Integer, Integer> indexToNumber = new HashMap<>();
    private final Map<Integer, PriorityQueue<Integer>> numberToIndices = new HashMap<>();

    public void change(int index, int number) {
        // 添加新数据
        indexToNumber.put(index, number);
        numberToIndices.computeIfAbsent(number, _ -> new PriorityQueue<>()).offer(index);
    }

    public int find(int number) {
        PriorityQueue<Integer> indices = numberToIndices.get(number);
        if (indices == null) {
            return -1;
        }
        while (!indices.isEmpty() && indexToNumber.get(indices.peek()) != number) {
            indices.poll(); // 堆顶货不对板，说明是旧数据，删除
        }
        return indices.isEmpty() ? -1 : indices.peek();
    }
}
```
```rust
use std::collections::{BinaryHeap, HashMap};
use std::cmp::Reverse;

struct NumberContainers {
    index_to_number: HashMap<i32, i32>,
    number_to_indices: HashMap<i32, BinaryHeap<Reverse<i32>>>,
}

/** 
 * `&self` means the method takes an immutable reference.
 * If you need a mutable reference, change it to `&mut self` instead.
 */
impl NumberContainers {

    fn new() -> Self {
        Self {
            index_to_number: HashMap::new(),
            number_to_indices: HashMap::new(),
        }
    }
    
    fn change(&mut self, index: i32, number: i32) {
        self.index_to_number.insert(index, number);
        self.number_to_indices.entry(number).or_insert(BinaryHeap::new()).push(Reverse(index));
    }
    
    fn find(&mut self, number: i32) -> i32 {
        if let Some(heap) = self.number_to_indices.get_mut(&number) {
            while let Some(&Reverse(top)) = heap.peek() {
                if self.index_to_number.get(&top) != Some(&number) {
                    heap.pop();
                } else {
                    return top;
                }
            }
        }
        -1
    }
}
```
```python
class NumberContainers:
    def __init__(self):
        self.index_to_number = {}
        self.number_to_indices = defaultdict(list)

    def change(self, index: int, number: int) -> None:
        # 添加新数据
        self.index_to_number[index] = number
        heappush(self.number_to_indices[number], index)

    def find(self, number: int) -> int:
        indices = self.number_to_indices[number]
        while indices and self.index_to_number[indices[0]] != number:
            heappop(indices)  # 堆顶货不对板，说明是旧数据，删除
        return indices[0] if indices else -1
```
```go
type NumberContainers struct {
	indexToNumber   map[int]int
	numberToIndices map[int]*hp
}

func Constructor() NumberContainers {
	return NumberContainers{map[int]int{}, map[int]*hp{}}
}

func (n NumberContainers) Change(index, number int) {
	// 添加新数据
	n.indexToNumber[index] = number
	if _, ok := n.numberToIndices[number]; !ok {
		n.numberToIndices[number] = &hp{}
	}
	heap.Push(n.numberToIndices[number], index)
}

func (n NumberContainers) Find(number int) int {
	indices, ok := n.numberToIndices[number]
	if !ok {
		return -1
	}
	for indices.Len() > 0 && n.indexToNumber[indices.IntSlice[0]] != number {
		heap.Pop(indices) // 堆顶货不对板，说明是旧数据，删除
	}
	if indices.Len() == 0 {
		return -1
	}
	return indices.IntSlice[0]
}

type hp struct{ sort.IntSlice }
func (h *hp) Push(v any) { h.IntSlice = append(h.IntSlice, v.(int)) }
func (h *hp) Pop() any   { a := h.IntSlice; v := a[len(a)-1]; h.IntSlice = a[:len(a)-1]; return v }
```
复杂度分析：
- 时间复杂度：
	- 初始化 $O(1)$ 。
	- `change` ：$O(\log q)$ ，其中 $q$ 是 `change` 的调用次数。
	- `find` ：均摊 $O(\log q)$ 。
- 空间复杂度：$O(q)$ 。
---
### 专题训练
见数据结构体题单的「**§5.6 懒删除堆**」。
[常用数据结构（前缀和/差分/栈/队列/堆/字典树/并查集/树状数组/线段树）](https://leetcode.cn/circle/discuss/mOr1u6/)
