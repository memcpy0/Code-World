Given a list of **non-negative** numbers and a target **integer** `k` , write a function to check if the array has a continuous subarray of size at least `2` that sums up to a multiple of `k` , that is, sums up to `n*k`  where `n` is also an integer.

 

**Example 1:**
```cpp
Input: [23, 2, 4, 6, 7],  k=6
Output: True
Explanation: Because [2, 4] is a continuous subarray of size 2 and sums up to 6.
```

**Example 2:**
```cpp
Input: [23, 2, 6, 4, 7],  k=6
Output: True
Explanation: Because [23, 2, 6, 4, 7] is an continuous subarray of size 5 and sums up to 42.
```

 

**Constraints:**
- The length of the array won't exceed `10,000` .
 - You may assume the sum of all the numbers is in the range of a signed 32-bit integer.

题意：给定一个**非负数数组**和一个**目标整数** $k$ ，编写一个函数来判断该数组是否含有连续的子数组，其大小至少为 $2$ ，且总和为 $k$ 的倍数，即总和为 $n*k$ ，其中 $n$ 也是一个整数。

---
### 解法 哈希表+前缀和+模运算
模运算+前缀和。本题题意是寻找有没有长度 $\ge 2$ 的子数组，其元素和是 $k$ 的整数倍。等价于判断是否存在子数组之和模 $k$ 余零。设 $sum_i,\ sum_j$ 是数组某处的前缀和，根据同余的定义可知，前缀和 $sum_i, sum_j\ (i \lt j)$ 应该模 $k$ 同余，即：$${sum_i} \equiv {sum_j}\pmod{{k}}$$

或者，由模运算对减法的封闭性：$${(sum_j} \bmod {k - sum_i} \bmod {k)} \bmod {k} =({sum_j - sum_i)} \bmod {k} = 0$$
也可知，$sum_i$ 和 $sum_j$ 模 $k$ 的余数需要相等。这样它们之间的子数组的元素和将是 $k$ 的整数倍。

假设现在手中有一段前缀和 $sum_j$ ，它模 $k$ 的余数为 $q$ ，要寻找有没有模 $k$ 的余数等于 $0$ 的子数组 $arr_{i,j}$ ，就只需要判断是否存在一个前缀和 $sum_{i-1}$ ，它模 $k$ 的值等于 $q$ 。

于是只需要用哈希映射，并在其中寻找，有没有某个前缀和模 $k$ 的余数等于当前的前缀和 $sum_j$ 模 $k$ 的余数 $q$ ：
- 如果没有找到，就记录当前位置前缀和 $\%\ k$ 的值及其最右位置；
- 如果找到，且长度满足题意，就返回 `true` 。
- 最后实在找不到，返回 `false` 。

几点优化是：
- 数组长度 $< 2$ ，直接返回 `false` ；
- 如果数组中存在一个两个或更长的连续零，直接返回 `true` ；
- 如果 $k$ 为 $0$ ，且数组中没有两个或更长的连续零，直接返回 `false` ，因为其他非负整数的和不会是零的倍数；
- 如果 $k$ 小于 $0$ ，就转换为 $-k$ ，因为子数组的和是负数 $k$ 的整数倍，自然也是正数 $-k$ 的整数倍。这个题目有点艹，给出的是非负数数组，但是目标整数可以是负数，嗯……

 代码如下：
```cpp
class Solution {
public: 
    bool checkSubarraySum(vector<int>& nums, int k) {
        if (nums.size() < 2) return false;
        for (int i = 0; i < nums.size() - 1; ++i) 
            if (!nums[i] && !nums[i + 1]) return true;
        if (k == 0) return false;
        if (k < 0) k = -k;

        unordered_map<int, int> dict = {{0, -1}};
        int curSum = 0, mod;

        for (int i = 0; i < nums.size(); ++i) {
            curSum += nums[i];
            int mod = curSum % k;
            if (dict.find(mod) == dict.end()) dict[mod] = i;
            else if (i - dict[mod] > 1) return true;
        }
        return false;
    }
};
```
提交后结果如下：
```cpp
执行用时：72 ms, 在所有 C++ 提交中击败了87.35% 的用户
内存消耗：26 MB, 在所有 C++ 提交中击败了36.04% 的用户
```
---
**20210602 打卡更新**：发现题目变化了，新增的用例和数据范围如下：

> <p><strong>Example 3:</strong></p>
> 
> ```cpp 
> Input: nums = [23,2,6,4,7], k = 13 
> Output: false 
> ```
> <p><strong>Constraints:</strong></p>  <ul> 	<li><code>1 &lt;=
> nums.length &lt;= 10<sup>5</sup></code></li> 	<li><code>0 &lt;=
> nums[i] &lt;= 10<sup>9</sup></code></li> 	<li><code>0 &lt;=
> sum(nums[i]) &lt;= 2<sup>31</sup> - 1</code></li> 	<li><code>1 &lt;= k
> &lt;= 2<sup>31</sup> - 1</code></li> </ul>

于是，代码可以写得很简单：
```cpp
class Solution {
public:
    bool checkSubarraySum(vector<int>& nums, int k) {
        int n = nums.size(), sum = 0;
        unordered_map<int, int> rec; //rec[key]=i表示前缀和presum[i]%k==key
        rec[0] = 0;
        for (int i = 0; i < n; ++i) {
            sum += nums[i];
            int v = sum % k; 
            if (rec.find(v) == rec.end()) rec[v] = i + 1;
            else if (i > rec[v]) return true;
        }
        return false;
    }
};
```
测试用例应该也改变了，运行效率如下：
```cpp
执行用时：180 ms, 在所有 C++ 提交中击败了63.36% 的用户
内存消耗：94 MB, 在所有 C++ 提交中击败了63.05% 的用户
```
