<p>You are given two integer arrays <code>nums1</code> and <code>nums2</code>. You are tasked to implement a data structure that supports queries of two types:</p>

<ol>
	<li><strong>Add</strong> a positive integer to an element of a given index in the array <code>nums2</code>.</li>
	<li><strong>Count</strong> the number of pairs <code>(i, j)</code> such that <code>nums1[i] + nums2[j]</code> equals a given value (<code>0 &lt;= i &lt; nums1.length</code> and <code>0 &lt;= j &lt; nums2.length</code>).</li>
</ol>

<p>Implement the <code>FindSumPairs</code> class:</p>

<ul>
	<li><code>FindSumPairs(int[] nums1, int[] nums2)</code> Initializes the <code>FindSumPairs</code> object with two integer arrays <code>nums1</code> and <code>nums2</code>.</li>
	<li><code>void add(int index, int val)</code> Adds <code>val</code> to <code>nums2[index]</code>, i.e., apply <code>nums2[index] += val</code>.</li>
	<li><code>int count(int tot)</code> Returns the number of pairs <code>(i, j)</code> such that <code>nums1[i] + nums2[j] == tot</code>.</li>
</ul>

 
<p><strong>Example 1:</strong></p>

```clike
Input
["FindSumPairs", "count", "add", "count", "count", "add", "add", "count"]
[[[1, 1, 2, 2, 2, 3], [1, 4, 5, 2, 5, 4]], [7], [3, 2], [8], [4], [0, 1], [1, 1], [7]]
Output
[null, 8, null, 2, 1, null, null, 11]

Explanation
FindSumPairs findSumPairs = new FindSumPairs([1, 1, 2, 2, 2, 3], [1, 4, 5, 2, 5, 4]);
findSumPairs.count(7);  // return 8; pairs (2,2), (3,2), (4,2), (2,4), (3,4), (4,4) make 2 + 5 and pairs (5,1), (5,5) make 3 + 4
findSumPairs.add(3, 2); // now nums2 = [1,4,5,4,5,4]
findSumPairs.count(8);  // return 2; pairs (5,2), (5,4) make 3 + 5
findSumPairs.count(4);  // return 1; pair (5,0) makes 3 + 1
findSumPairs.add(0, 1); // now nums2 = [2,4,5,4,5,4]
findSumPairs.add(1, 1); // now nums2 = [2,5,5,4,5,4]
findSumPairs.count(7);  // return 11; pairs (2,1), (2,2), (2,4), (3,1), (3,2), (3,4), (4,1), (4,2), (4,4) make 2 + 5 and pairs (5,3), (5,5) make 3 + 4
 
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums1.length &lt;= 1000</code></li>
	<li><code>1 &lt;= nums2.length &lt;= 10<sup>5</sup></code></li>
	<li><code>1 &lt;= nums1[i] &lt;= 10<sup>9</sup></code></li>
	<li><code>1 &lt;= nums2[i] &lt;= 10<sup>5</sup></code></li>
	<li><code>0 &lt;= index &lt; nums2.length</code></li>
	<li><code>1 &lt;= val &lt;= 10<sup>5</sup></code></li>
	<li><code>1 &lt;= tot &lt;= 10<sup>9</sup></code></li>
	<li>At most <code>1000</code> calls are made to <code>add</code> and <code>count</code> <strong>each</strong>.</li>
</ul>


题意：给你两个整数数组 <code>nums1</code> 和 <code>nums2</code> ，请你实现一个支持下述两类查询的数据结构：</p>

<ol>
	<li><strong>累加</strong> ，将一个正整数加到 <code>nums2</code> 中指定下标对应元素上。</li>
	<li><strong>计数 </strong>，统计满足 <code>nums1[i] + nums2[j]</code> 等于指定值的下标对 <code>(i, j)</code> 数目（<code>0 &lt;= i &lt; nums1.length</code> 且 <code>0 &lt;= j &lt; nums2.length</code>）。</li>
</ol>

<p>实现 <code>FindSumPairs</code> 类：</p>

<ul>
	<li><code>FindSumPairs(int[] nums1, int[] nums2)</code> 使用整数数组&nbsp;<code>nums1</code> 和 <code>nums2</code> 初始化 <code>FindSumPairs</code> 对象。</li>
	<li><code>void add(int index, int val)</code> 将 <code>val</code> 加到 <code>nums2[index]</code> 上，即，执行 <code>nums2[index] += val</code> 。</li>
	<li><code>int count(int tot)</code> 返回满足&nbsp;<code>nums1[i] + nums2[j] == tot</code> 的下标对 <code>(i, j)</code> 数目。</li>
</ul>
 
---
### 解法 哈希表
本题其实就是两数之和的变体，加上了动态的单点修改，以及是求加上 `nums1[i]` 等于 `tot` 的 `nums2[j]` 的个数。由于 `nums1` 的长度远小于 `nums2` ，因此可以遍历 `nums1[]` 而将 `nums2[]` 存储到哈希表中。

单点修改 `nums2[index]` ，相当于哈希表中多了一个数 `nums2[index] + val` ，少了一个数 `nums2[index]` 。由于函数的形式参数为 `index, val` ，所以类中还要存储 `nums2[]` 。
```cpp
class FindSumPairs {
private:
    vector<int> arr1, arr2;
    unordered_map<int, int> rec;
public:
    FindSumPairs(vector<int>& nums1, vector<int>& nums2) {
        arr1 = nums1;
        arr2 = nums2;
        for (const int &v : nums2) ++rec[v];
    }
    
    void add(int index, int val) { //O(1)
        --rec[arr2[index]];
        arr2[index] += val;
        ++rec[arr2[index]];
    }
    
    int count(int tot) { 
        int ans = 0;
        for (int i = 0; i < arr1.size(); ++i) {
            int target = tot - arr1[i];
            if (rec.find(target) != rec.end()) ans += rec[target];
        }
        return ans;
    }
}; 
```
运行效率如下：
```cpp
执行用时：392 ms, 在所有 C++ 提交中击败了74.55% 的用户
内存消耗：72 MB, 在所有 C++ 提交中击败了74.31% 的用户
```
