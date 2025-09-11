<div class="notranslate"><p>Given an array of integers <code>arr</code> of even length <code>n</code> and an integer <code>k</code>.</p>

<p>We want to divide the array into exactly <code>n /&nbsp;2</code> pairs such that the sum of each pair is divisible by <code>k</code>.</p>

<p>Return <em>True</em> If you can find a way to do that or <em>False</em> otherwise.</p>
 
<p><strong>Example 1:</strong></p>

```swift
Input: arr = [1,2,3,4,5,10,6,7,8,9], k = 5
Output: true
Explanation: Pairs are (1,9),(2,8),(3,7),(4,6) and (5,10).
```

 
<p><strong>Example 2:</strong></p>

```swift
Input: arr = [1,2,3,4,5,6], k = 7
Output: true
Explanation: Pairs are (1,6),(2,5) and(3,4).
```

 
<p><strong>Example 3:</strong></p>

```swift
Input: arr = [1,2,3,4,5,6], k = 10
Output: false
Explanation: You can try all possible pairs to see that there is no way to divide arr into 3 pairs each with sum divisible by 10.
```

 
<p><strong>Example 4:</strong></p>
 

```swift
Input: arr = [-10,10], k = 2
Output: true
```

<p><strong>Example 5:</strong></p>

```swift
Input: arr = [-1,1,-2,2,-3,3,-4,4], k = 3
Output: true
```

 
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>arr.length == n</code></li>
	<li><code>1 &lt;= n &lt;= 10^5</code></li>
	<li><code>n</code> is even.</li>
	<li><code>-10^9 &lt;= arr[i] &lt;= 10^9</code></li>
	<li><code>1 &lt;= k &lt;= 10^5</code></li>
</ul>
</div>

题意：给出整数数组 `arr` 和整数 `k` ，其中数组长度是偶数，值为 `n` 。现在需要把数组恰好分成 `n / 2` 对，以使每对数字的和都能够被 `k` 整除。如果存在这样的分法，请返回 `True` ；否则，返回 `False` 。

---
### 解法 模运算+哈希表
暴力算法 $O(n^2)$ 的时间复杂度是无论如何都过不了本题的，更何况本题不需要输出数对分配方案，只是一个判定问题。因此，通过利用模运算的性质 $$(x + y) \bmod k =  ((x \bmod k) + (y \bmod k)) \bmod k = 0$$

（即只要数 `x` 模 `k` 和数 `y` 模 `k` 的值相加等于 `k` ，就说明 `x + y` 能够被 `k` 整除），我们可以得到更优的 $O(n)$ 解法：
```cpp
class Solution {
public:
    bool canArrange(vector<int>& arr, int k) {
        unordered_map<int, int> rec;
        //记录数组中每个元素模k后的值的出现频数
        for (const int &v : arr) ++rec[((v % k) + k) % k]; //避免负数取模和取余的差异
        for (int i = 0; i < k; ++i) {
            if (i == 0) continue; //i等于0,k-i等于k
            if (rec[i] != rec[k - i]) return false; //存在数对相加之后,其和不能被k整除
        }
        return rec[0] % 2 == 0; //模k为0的数是否有偶数个
    }
};
```
提交后运行效率如下：
```cpp
执行用时：444 ms, 在所有 C++ 提交中击败了20.32% 的用户
内存消耗：74.9 MB, 在所有 C++ 提交中击败了7.62% 的用户
```
将 `unordered_map<int, int> rec` 换成整数数组 `vector<int> rec(k)` 后，执行效率大幅提高：
```cpp
执行用时：220 ms, 在所有 C++ 提交中击败了99.80% 的用户
内存消耗：60.7 MB, 在所有 C++ 提交中击败了43.50% 的用户
```
