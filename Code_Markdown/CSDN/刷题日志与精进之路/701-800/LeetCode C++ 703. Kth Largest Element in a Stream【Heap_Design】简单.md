<p>Design a class to find&nbsp;the <code>k<sup>th</sup></code> largest element in a stream. Note that it is the <code>k<sup>th</sup></code> largest element in the sorted order, not the <code>k<sup>th</sup></code> distinct element.</p>

<p>Implement&nbsp;<code>KthLargest</code>&nbsp;class:</p>

<ul>
	<li><code>KthLargest(int k, int[] nums)</code>&nbsp;Initializes the object with the integer <code>k</code> and the stream of integers <code>nums</code>.</li>
	<li><code>int add(int val)</code>&nbsp;Returns the element representing the <code>k<sup>th</sup></code> largest element in the stream.</li>
</ul>

 
<p><strong>Example 1:</strong></p>

```haskell
Input
["KthLargest", "add", "add", "add", "add", "add"]
[[3, [4, 5, 8, 2]], [3], [5], [10], [9], [4]]
Output
[null, 4, 5, 5, 8, 8]

Explanation
KthLargest kthLargest = new KthLargest(3, [4, 5, 8, 2]);
kthLargest.add(3);   // return 4
kthLargest.add(5);   // return 5
kthLargest.add(10);  // return 5
kthLargest.add(9);   // return 8
kthLargest.add(4);   // return 8
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= k &lt;= 10<sup>4</sup></code></li>
	<li><code>0 &lt;= nums.length &lt;= 10<sup>4</sup></code></li>
	<li><code>-10<sup>4</sup> &lt;= nums[i] &lt;= 10<sup>4</sup></code></li>
	<li><code>-10<sup>4</sup> &lt;= val &lt;= 10<sup>4</sup></code></li>
	<li>At most <code>10<sup>4</sup></code> calls will be made to <code>add</code>.</li>
	<li>It is guaranteed that there will be at least <code>k</code> elements in the array when you search for the <code>k<sup>th</sup></code> element.</li>
</ul> 

题意：设计一个找到数据流中第 `k` 大元素的 `KthLargest` 类。注意是排序后的第 `k` 大元素，不是第 `k` 个不同的元素：
- `KthLargest(int k, int[] nums)` 使用整数 `k` 和整数流 `nums` 初始化对象。
-  `int add(int val)` 返回当前数据流中第 `k` 大的元素。

---
### 解法 最小堆

题面写得太差了，`add(val)` 的含义是：添加 `val` 到数据流中，**然后**返回当前数据流中第 `k` 大的元素。题目示例的解释如下：
```css
8 5 4 2, 返回第k = 3大的元素
add(3):   8 5 4 3 2, return 4, 
add(5):   8 5 5 4 3 2, return 5,
add(10): 10 8 5 5 4 3 2, return 5,
add(9):  10 9 8 5 5 4 3 2, return 8,
add(4):  10 9 8 5 5 4 4 3 2, return 8
```
解法是**维护一个 `k` 个元素的最小堆**，如果最小堆不足 `k` 个元素，就直接将新元素压入堆中；否则，如果数据流的新元素大于堆顶值，就弹出堆顶并将新元素入堆。于是第 `k` 大的元素一定在堆顶，我们返回堆顶元素即可。C++代码如下：
```cpp
class KthLargest {
private:
    priority_queue<int, vector<int>, greater<int>> pq; //维护一个k个元素的最小堆
    int k;
public:
    KthLargest(int k, vector<int>& nums) {
        this->k = k;
        for (const int &v : nums) {
            if (pq.size() < k) pq.push(v); 
            else if (v > pq.top()) { //pq.size()>=k
                pq.pop();
                pq.push(v);
            } 
        }
    }
    
    int add(int val) {
        if (pq.size() < k) pq.push(val); //最小堆的大小<k时,直接添加新的元素,此时至少有k个元素（根据题面）
        else if (val >= pq.top()) { //pq.size() >= k
            pq.pop();
            pq.push(val);
        }
        return pq.top();
    }
};
```
运行效率如下：
```cpp
执行用时：76 ms, 在所有 C++ 提交中击败了74.80% 的用户
内存消耗：20 MB, 在所有 C++ 提交中击败了32.25% 的用户
```
如果使用Python，可以 `import heapq` ， `heapq` 就是使用**小顶堆**实现的优先队列：
```py
class KthLargest:
    def __init__(self, k: int, nums: List[int]):
        self.k = k
        self.heap = nums
        heapq.heapify(self.heap) # 小顶堆实现的优先队列
        while len(self.heap) > k:
            heapq.heappop(self.heap)

    def add(self, val: int) -> int:
        if len(self.heap) < self.k:
            heapq.heappush(self.heap, val)
        elif val > self.heap[0]:
            heapq.heapreplace(self.heap, val)
        return self.heap[0]
```
