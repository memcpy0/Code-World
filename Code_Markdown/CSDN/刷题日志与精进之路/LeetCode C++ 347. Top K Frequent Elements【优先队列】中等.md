Given a non-empty array of integers, return the `k` most frequent elements.

**Example 1:**
```cpp
Input: nums = [1,1,1,2,2,3], k = 2
Output: [1,2]
```
**Example 2:**
```cpp
Input: nums = [1], k = 1
Output: [1]
```
**Note:**
- You may assume `k` is always valid, `1 ≤ k ≤` number of unique elements.
- Your algorithm's time complexity must be better than `O(n log n)` , where `n` is the array's size.
- It's guaranteed that the answer is unique, in other words the set of the top `k` frequent elements is unique.
- You can return the answer in any order.

题意：给定一个非空的整数数组，返回其中出现频率前 `k` 高的元素。这里给定的 `k` 总是合理的，且 `1 ≤ k ≤` 数组中不相同的元素的个数。题目还要求，算法的时间复杂度必须优于 $\text{O(n log n)}$ ，$n$ 是数组的大小。另外，数组中前 `k` 个高频元素的集合是唯一的，可以按任意顺序返回答案。

----
思路1：扫描一遍统计频率，按照频率排序找到前 `k` 个出现频率最高的元素。算法时间复杂度是 $\text{O(nlogn)}$ 。

代码：
```cpp
class Solution { 
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        vector<int> ans;
        vector<pair<int, double>> vi;
        unordered_map<int, double> dict;
        for (const int v : nums) dict[v] += (1.0 / nums.size());
        for (auto it = dict.begin(); it != dict.end(); ++it)
            vi.push_back(*it);
        sort(vi.begin(), vi.end(), [](const pair<int, double> &a, const pair<int, double> &b) {
            return a.second == b.second ? a.first < b.first : a.second > b.second;
        });
        for (int i = 0; i < k; ++i) 
            ans.push_back(vi[i].first);
        return ans;
    }
};
```
不难发现，算法的瓶颈在排序：
```handlebars
执行用时：40 ms, 在所有 C++ 提交中击败了63.03% 的用户
内存消耗：14.2 MB, 在所有 C++ 提交中击败了24.19% 的用户
```
其实上面的代码还可以小优化一下，**不用统计频率，统计次数即可**：
```cpp
class Solution { 
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        vector<int> ans;
        vector<pair<int, int>> vi;
        unordered_map<int, int> dict;
        for (const int v : nums) ++dict[v];
        for (auto it = dict.begin(); it != dict.end(); ++it)
            vi.push_back(*it);
        sort(vi.begin(), vi.end(), [](const pair<int, double> &a, const pair<int, double> &b) {
            return a.second == b.second ? a.first < b.first : a.second > b.second;
        });
        for (int i = 0; i < k; ++i) 
            ans.push_back(vi[i].first);
        return ans;
    }
};
```
效率提升了一些：
```cpp
执行用时：36 ms, 在所有 C++ 提交中击败了83.80% 的用户
内存消耗：13.9 MB, 在所有 C++ 提交中击败了80.45% 的用户
```

---
思路2：求 `TopK` (前 `k` 大)用小根堆，维护堆大小不超过 `k` 即可：
- 检查堆大小是否等于 `k` ：
	- 是的话，压入堆前和堆顶元素的频率比较：
		- 如果遍历到的元素比队列中最小频率元素的频率高，则弹出堆顶，将队列中最小频率的元素出队，再将新元素入队；
		- 没有超过，就 `do nothing` 。	
	- 否则的话，直接进堆。
- 最后，队列中剩下的，就是前 `k` 个出现频率最高的元素。时间复杂度是 $\text{O(nlogk)}$ 级别的，在 `k << n` 时大大优于思路1的复杂度；如果 `k` 和 `n` 差不多的话，这个算法和前一个没有太大的性能差距。

注意：本题避免使用大根堆，因为要把所有元素压入堆，复杂度是  $\text{O(nlogn)}$ ，而且还浪费内存。如果是海量元素，就爆炸了。总结起来就是一句话：<font color="red">**求前 k 大，用小根堆，求前 k 小，用大根堆。**</font>


代码如下：
```cpp
class Solution { 
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        assert(k > 0);                   //k>=1
        unordered_map<int, int> freq;   //元素,出现的频数
        for (const int v : nums) ++freq[v];
        assert(k <= freq.size());       //k<=数组中不相同的元素的个数
        
        using E = pair<int, int>;
        //扫描freq,维护当前出现频率最高的k个元素: top k, minheap
        //优先队列中,按照频率排序,所以数据对pair<count, val>
        priority_queue<E, vector<E>, greater<E>> minHeap;
        for (const auto &it : freq) {
            if (minHeap.size() == k) {
                if (it.second < minHeap.top().first) continue;
                minHeap.pop(); 
            } 
            minHeap.emplace(it.second, it.first); 
        }
        
        vector<int> ans;
        while (!minHeap.empty()) {
            ans.push_back(minHeap.top().second);
            minHeap.pop();
        }
        return ans;
    }
};
```
运行速度如下：
```cpp
执行用时：36 ms, 在所有 C++ 提交中击败了83.80% 的用户
内存消耗：14 MB, 在所有 C++ 提交中击败了50.96% 的用户
```
---
思路3：对于这个问题，我们还可以设计一个时间复杂度为 $\text{O(nlog(n-k))}$ 的算法，只需要维护一个大小为 $n - k$ 的优先队列即可。此时如果 `n, k` 差不多的时候，这种算法优势就很大了。

甚至我们可以将思路2和思路3结合起来……这里暂不实现。
