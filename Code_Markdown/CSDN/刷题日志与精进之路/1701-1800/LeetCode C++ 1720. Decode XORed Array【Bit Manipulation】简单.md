<p>There is a <strong>hidden</strong> integer array <code>arr</code> that consists of <code>n</code> non-negative integers.</p>

<p>It was encoded into another integer array <code>encoded</code> of length <code>n - 1</code>, such that <code>encoded[i] = arr[i] XOR arr[i + 1]</code>. For example, if <code>arr = [1,0,2,1]</code>, then <code>encoded = [1,2,3]</code>.</p>

<p>You are given the <code>encoded</code> array. You are also given an integer <code>first</code>, that is the first element of <code>arr</code>, i.e. <code>arr[0]</code>.</p>

<p>Return <em>the original array</em> <code>arr</code>. It can be proved that the answer exists and is unique.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: encoded = [1,2,3], first = 1
Output: [1,0,2,1]
Explanation: If arr = [1,0,2,1], then first = 1 and encoded = [1 XOR 0, 0 XOR 2, 2 XOR 1] = [1,2,3] 
```
<p><strong>Example 2:</strong></p>

```clike
Input: encoded = [6,2,7,3], first = 4
Output: [4,2,0,7,4] 
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>2 &lt;= n &lt;= 10<sup>4</sup></code></li>
	<li><code>encoded.length == n - 1</code></li>
	<li><code>0 &lt;= encoded[i] &lt;= 10<sup>5</sup></code></li>
	<li><code>0 &lt;= first &lt;= 10<sup>5</sup></code></li>
</ul>

 
题意：<strong>未知</strong> 整数数组 <code>arr</code> 由 <code>n</code> 个非负整数组成。经编码后变为长度为 <code>n - 1</code> 的另一个整数数组 <code>encoded</code> ，其中 <code>encoded[i] = arr[i] XOR arr[i + 1]</code> 。例如，<code>arr = [1,0,2,1]</code> 经编码后得到 <code>encoded = [1,2,3]</code> 。</p>

<p>给你编码后的数组 <code>encoded</code> 和原数组 <code>arr</code> 的第一个元素 <code>first</code>（<code>arr[0]</code>）。请解码返回原数组 <code>arr</code> 。可以证明答案存在并且是唯一的。</p>
 
---
### 解法 位运算
很简单的题目，有了 `first` 就可以 `first ^ encoded[0] = first ^ first ^ arr[1]` 得到 `arr[1]` ，得到 `arr[1]` 后继续这一异或操作，同理可得到 `arr[2], ..., arr[n - 1]` 。
```cpp
class Solution {
public:
    vector<int> decode(vector<int>& encoded, int first) {
        vector<int> ans;
        ans.push_back(first); 
        for (const int &v : encoded) {
            first = first ^ v;
            ans.push_back(first);
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了88.54% 的用户
内存消耗：25.3 MB, 在所有 C++ 提交中击败了38.19% 的用户
```
