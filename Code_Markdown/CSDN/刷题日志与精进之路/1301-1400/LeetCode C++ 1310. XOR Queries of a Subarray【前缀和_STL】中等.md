Given the array <code>arr</code> of positive integers and the array <code>queries</code> where <code>queries[i] = [L<sub>i,&nbsp;</sub>R<sub>i</sub>]</code>,&nbsp;for each query <code>i</code> compute the <strong>XOR</strong> of elements from <code>L<sub>i</sub></code> to <code>Ri</code> (that is, <code>arr[L<sub>i</sub>] <strong>xor</strong> arr[L<sub>i+1</sub>] <strong>xor</strong> ... <strong>xor</strong> arr[R<sub>i</sub>]</code> ). Return an array containing the result for the given <code>queries</code>.
 
<p><strong>Example 1:</strong></p>

```clike
Input: arr = [1,3,4,8], queries = [[0,1],[1,2],[0,3],[3,3]]
Output: [2,7,14,8] 
Explanation: 
The binary representation of the elements in the array are:
1 = 0001 
3 = 0011 
4 = 0100 
8 = 1000 
The XOR values for queries are:
[0,1] = 1 xor 3 = 2 
[1,2] = 3 xor 4 = 7 
[0,3] = 1 xor 3 xor 4 xor 8 = 14 
[3,3] = 8
```

 
<p><strong>Example 2:</strong></p>

```clike
Input: arr = [4,8,2,10], queries = [[2,3],[1,3],[0,0],[0,3]]
Output: [8,0,4,4] 
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= arr.length &lt;= 3 *&nbsp;10^4</code></li>
	<li><code>1 &lt;= arr[i] &lt;= 10^9</code></li>
	<li><code>1 &lt;= queries.length &lt;= 3 * 10^4</code></li>
	<li><code>queries[i].length == 2</code></li>
	<li><code>0 &lt;= queries[i][0] &lt;= queries[i][1] &lt; arr.length</code></li>
</ul>


题意：有一个正整数数组&nbsp;<code>arr</code>，现给你一个对应的查询数组&nbsp;<code>queries</code>，其中&nbsp;<code>queries[i] = [L<sub>i,&nbsp;</sub>R<sub>i</sub>]</code>。对于每个查询&nbsp;<code>i</code>，计算从&nbsp;<code>L<sub>i</sub></code>&nbsp;到&nbsp;<code>R<sub>i</sub></code>&nbsp;的&nbsp;<strong>XOR</strong>&nbsp;值（即&nbsp;<code>arr[L<sub>i</sub>] <strong>xor</strong> arr[L<sub>i+1</sub>] <strong>xor</strong> ... <strong>xor</strong> arr[R<sub>i</sub>]</code>）作为本次查询的结果。并返回一个包含给定查询&nbsp;<code>queries</code>&nbsp;所有结果的数组。</p>


---
### 解法 前缀和/STL
本题类似于前缀和的写法，先预处理出 `arr[]` 的前缀异或和数组，然后 $O(1)$ 查询区间异或和即可：
```cpp
class Solution {
public:
    vector<int> xorQueries(vector<int>& arr, vector<vector<int>>& queries) {
        int n = arr.size();
        vector<int> partial_xor_sum(n + 1, 0), ans;
        for (int i = 0; i < n; ++i) partial_xor_sum[i + 1] = partial_xor_sum[i] ^ arr[i];
        for (const vector<int>& q : queries) 
            ans.push_back(partial_xor_sum[q[1] + 1] ^ partial_xor_sum[q[0]]);
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：64 ms, 在所有 C++ 提交中击败了96.85% 的用户
内存消耗：31.9 MB, 在所有 C++ 提交中击败了39.76% 的用户
```
事实上，C++在头文件 `<numeric>` 提供了一个名为 `partial_sum` 的函数模板，使得我们可以计算范围内的部分和，并输出到某个容器对象内。其函数声明如下，需要注意的是，此处的**和**指的是二元函数 `op` 计算的结果。
```cpp
(1)
template< class InputIt, class OutputIt >
OutputIt partial_sum( InputIt first, InputIt last, OutputIt d_first );
	(C++20 前)
template< class InputIt, class OutputIt >
constexpr OutputIt partial_sum( InputIt first, InputIt last, OutputIt d_first );
	(C++20 起)
	
(2) 	
template< class InputIt, class OutputIt, class BinaryOperation > 
OutputIt partial_sum( InputIt first, InputIt last, OutputIt d_first,
                      BinaryOperation op );
	(C++20 前)
template< class InputIt, class OutputIt, class BinaryOperation >
constexpr OutputIt partial_sum( InputIt first, InputIt last, OutputIt d_first,
                                BinaryOperation op );
	(C++20 起)
```
而在头文件 `<functional>` 中，C++又为我们提供了一系列用于实例化生成二元函数对象的类模板，包括 `plus, minus, multiplies, divides, modulus` 等算术运算、`equal_to, not_equal_to, greater, less, greater_equal, less_equal` 等比较运算、`bit_and, bit_or, bit_xor` 等按位运算等等。

因此本题代码可以写成如下形式：
```cpp
class Solution {
public:
    vector<int> xorQueries(vector<int>& arr, vector<vector<int>>& queries) {
        vector<int> partial_xor_sum(arr.size() + 1, 0), ans(queries.size());
        partial_sum(arr.begin(), arr.end(), partial_xor_sum.begin() + 1, bit_xor<int>());
        transform(queries.begin(), queries.end(), ans.begin(), [&](const vector<int>& q) {
            return partial_xor_sum[q[1] + 1] ^ partial_xor_sum[q[0]];
        });
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：68 ms, 在所有 C++ 提交中击败了94.90% 的用户
内存消耗：31.4 MB, 在所有 C++ 提交中击败了96.86% 的用户
```
