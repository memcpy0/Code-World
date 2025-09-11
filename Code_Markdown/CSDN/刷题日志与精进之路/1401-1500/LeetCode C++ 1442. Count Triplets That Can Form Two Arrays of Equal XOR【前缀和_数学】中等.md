<p>Given an array of&nbsp;integers <code>arr</code>.</p>

<p>We want to select three indices <code>i</code>, <code>j</code> and <code>k</code> where <code>(0 &lt;= i &lt; j &lt;= k &lt; arr.length)</code>.</p>

<p>Let's define <code>a</code> and <code>b</code> as follows:</p>

<ul>
	<li><code>a = arr[i] ^ arr[i + 1] ^ ... ^ arr[j - 1]</code></li>
	<li><code>b = arr[j] ^ arr[j + 1] ^ ... ^ arr[k]</code></li>
</ul>

<p>Note that <strong>^</strong> denotes the <strong>bitwise-xor</strong> operation.</p>

<p>Return <em>the number of triplets</em> (<code>i</code>, <code>j</code> and <code>k</code>) Where <code>a == b</code>.</p>
 
<p><strong>Example 1:</strong></p>

```clike
Input: arr = [2,3,1,6,7]
Output: 4
Explanation: The triplets are (0,1,2), (0,2,2), (2,3,4) and (2,4,4)
```


<p><strong>Example 2:</strong></p>

```clike
Input: arr = [1,1,1,1,1]
Output: 10 
```

<p><strong>Example 3:</strong></p>

```clike
Input: arr = [2,3]
Output: 0
```

 
<p><strong>Example 4:</strong></p>

```clike
Input: arr = [1,3,5,7,9]
Output: 3
```

 
<p><strong>Example 5:</strong></p>

```clike
Input: arr = [7,11,12,9,5,2,7,17,22]
Output: 8 
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= arr.length &lt;= 300</code></li>
	<li><code>1 &lt;= arr[i] &lt;= 10^8</code></li>
</ul>


题意：给你一个整数数组 <code>arr</code> 。现需要从数组中取三个下标 <code>i</code>、<code>j</code> 和 <code>k</code> ，其中 <code>(0 &lt;= i &lt; j &lt;= k &lt; arr.length)</code> 。</p>

<code>a = arr[i] ^ arr[i + 1] ^ ... ^ arr[j - 1]</code>，<code>b = arr[j] ^ arr[j + 1] ^ ... ^ arr[k]</code> 。请返回能够令 <code>a == b</code> 成立的三元组 `(i, j, k)` 的数目。</p>


---
### 解法1 暴力
稍微优化一点的暴力。遍历所有的三元组 `(i, j, k)` ，只是在求子数组 `arr[i, j), arr[j, k]` 的异或和时用到了前缀和优化。整个算法的时间复杂度为 $O(n^3)$ ：
```cpp
class Solution {
public:
    int countTriplets(vector<int>& arr) {
        int n = arr.size(), cnt = 0;
        vector<int> xorsum(n + 1);
        partial_sum(arr.begin(), arr.end(), xorsum.begin() + 1, bit_xor<int>());
        for (int i = 0; i < n - 1; ++i) 
            for (int j = i + 1; j < n; ++j)
                for (int k = j; k < n; ++k) 
                    if ((xorsum[j] ^ xorsum[i]) == (xorsum[k + 1] ^ xorsum[j]))
                        ++cnt;
        return cnt;
    }
};
```
运算效率如下：
```cpp
执行用时：332 ms, 在所有 C++ 提交中击败了8.37% 的用户
内存消耗：7.2 MB, 在所有 C++ 提交中击败了75.81% 的用户
```
---
### 解法2 数学
我们可以转换一下题意， `i < j <= k` 且令 `xorsum(arr[i, j)) == xorsum(arr[j, k])` ，前后两个子数组的异或和相等……这说明了什么？说明两个子数组的总区域异或和为 `0` ，且整个区域长度大于等于 `2` 。

于是题意转换为，枚举并寻找长度 `>= 2` 且异或和为 `0` 的子数组。接着可以很简单的求出每个子数组区域 `[i, k]` 中三元组的个数（ `i < j <= k` ，易知三元组的个数即为 `j` 可以取的位置数 `k - i` ），对这些个数求和即可得到整个题目的答案。
```cpp
class Solution {
public:
    int countTriplets(vector<int>& arr) {
        int n = arr.size(), cnt = 0;
        for (int i = 0; i < n - 1; ++i) {
            int val = arr[i];
            for (int k = i + 1; k < n; ++k) {
                val ^= arr[k];
                if (val == 0) cnt += (k - i);
            }
        } 
        return cnt;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7.2 MB, 在所有 C++ 提交中击败了76.74% 的用户
```
