> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>Given an integer array <code>nums</code> with possible <strong>duplicates</strong>, randomly output the index of a given <code>target</code> number. You can assume that the given target number must exist in the array.</p>

<p>Implement the <code>Solution</code> class:</p>

<ul>
	<li><code>Solution(int[] nums)</code> Initializes the object with the array <code>nums</code>.</li>
	<li><code>int pick(int target)</code> Picks a random index <code>i</code> from <code>nums</code> where <code>nums[i] == target</code>. If there are multiple valid i's, then each index should have an equal probability of returning.</li>
</ul>

 
<p><strong>Example 1:</strong></p>

```haskell
Input
["Solution", "pick", "pick", "pick"]
[[[1, 2, 3, 3, 3]], [3], [1], [3]]
Output
[null, 4, 0, 2]

Explanation
Solution solution = new Solution([1, 2, 3, 3, 3]);
solution.pick(3); // It should return either index 2, 3, or 4 randomly. Each index should have equal probability of returning.
solution.pick(1); // It should return 0. Since in the array only nums[0] is equal to 1.
solution.pick(3); // It should return either index 2, 3, or 4 randomly. Each index should have equal probability of returning.
```

  
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length &lt;= 2 * 10<sup>4</sup></code></li>
	<li><code>-2<sup>31</sup> &lt;= nums[i] &lt;= 2<sup>31</sup> - 1</code></li>
	<li><code>target</code> is an integer from <code>nums</code>.</li>
	<li>At most <code>10<sup>4</sup></code> calls will be made to <code>pick</code>.</li>
</ul>


题意：给定一个可能含有重复元素的整数数组，要求随机输出给定的数字的索引。 您可以假设给定的数字一定存在于数组中。注意，数组大小可能非常大。 使用太多额外空间的解决方案将不会通过测试。 
 
---
### 解法1 哈希表+随机化
用哈希表存储每个值和值对应的下标集合，查找 `target` 时随机选择一个下标返回即可。此时，`Solution` 类的构造方法的时间复杂度为 $O(n)$ ，`pick` 方法的时间复杂度为 $O(1)$ ，空间复杂度为 $O(n)$ 。
```cpp
//C++ version
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);
class Solution {
private:
    unordered_map<int, vector<int>> rec;
    uniform_int_distribution<int> ud;
public:
    Solution(vector<int>& nums) {
        for (int i = 0, n = nums.size(); i < n; ++i)
            rec[nums[i]].push_back(i);
    }
    int pick(int target) {
        const vector<int>& v = rec[target];
        int n = v.size() - 1;
        ud.param(uniform_int_distribution<>::param_type{0, n});
        return v[ud(generator)];
    }
};
//执行用时：116 ms, 在所有 C++ 提交中击败了25.22% 的用户
//内存消耗：50.5 MB, 在所有 C++ 提交中击败了22.75% 的用户
```
```java
//Java version
class Solution {
    private HashMap<Integer, ArrayList<Integer>> rec;
    public Solution(int[] nums) {
        rec = new HashMap<>();
        for (int i = 0; i < nums.length; ++i) {
            if (!rec.containsKey(nums[i])) rec.put(nums[i], new ArrayList<>());
            rec.get(nums[i]).add(i);
        }
    }
    
    public int pick(int target) {
        int n = rec.get(target).size();
        int k = new Random().nextInt(n);
        return rec.get(target).get(k);
    }
}
//执行用时：87 ms, 在所有 Java 提交中击败了10.81% 的用户
//内存消耗：51.8 MB, 在所有 Java 提交中击败了5.00% 的用户
```
---
### 解法2 蓄水池抽样
上述做法虽然正确，但是浪费了很多额外空间，带来了不必要的时间消耗。我们可以使用[蓄水池抽样 `Random Reservoir Sampling`](https://memcpy0.blog.csdn.net/article/details/120088851) 。保存整个 `nums` 数组，每次等概率选择 `target` 的下标值：
```cpp
//C++ version
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);
class Solution {
private:
    vector<int> arr;
    uniform_int_distribution<int> ud;
public:
    Solution(vector<int>& nums) { arr = nums; }
    int pick(int target) { 
        //扫描整个数组,如果遇到第cnt个target,就以1/cnt的概率选择其下标并替换之前选取的下标
        int cnt = 0, ans;
        for (int i = 0, n = arr.size(); i < n; ++i) {
            if (arr[i] == target) {
                ++cnt;
                ud.param(uniform_int_distribution<>::param_type{1, cnt});
                if (ud(generator) == cnt) ans = i;
            }
        }
        return ans;
    }
};
//执行用时：48 ms, 在所有 C++ 提交中击败了96.43% 的用户
//内存消耗：34.7 MB, 在所有 C++ 提交中击败了69.78% 的用户
```
通过只保存 `nums` 的开头和结尾迭代器，可以将额外空间降至 $O(1)$ ，其余思路则完全一致：
```cpp
//C++ version
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);
class Solution {
private:
    vector<int>::iterator begin, end;
    uniform_int_distribution<int> ud;
public:
    Solution(vector<int>& nums) { 
        begin = nums.begin(); 
        end = nums.end(); 
    }
    int pick(int target) {
        int cnt = 0, ans, i = 0;
        for (auto cur = begin; cur != end; ++cur, ++i) {
            if (*cur == target) {
                ++cnt;
                ud.param(uniform_int_distribution<>::param_type{1, cnt});
                if (ud(generator) == cnt) ans = i;
            }
        }
        return ans;
    }
}; 
//执行用时：48 ms, 在所有 C++ 提交中击败了96.43% 的用户
//内存消耗：33.9 MB, 在所有 C++ 提交中击败了85.74% 的用户
```
