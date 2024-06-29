<p>There is an integer array <code>perm</code> that is a permutation of the first <code>n</code> positive integers, where <code>n</code> is always <strong>odd</strong>.</p>

<p>It was encoded into another integer array <code>encoded</code> of length <code>n - 1</code>, such that <code>encoded[i] = perm[i] XOR perm[i + 1]</code>. For example, if <code>perm = [1,3,2]</code>, then <code>encoded = [2,1]</code>.</p>

<p>Given the <code>encoded</code> array, return <em>the original array</em> <code>perm</code>. It is guaranteed that the answer exists and is unique.</p>

 
<p><strong>Example 1:</strong></p>

```clike
Input: encoded = [3,1]
Output: [1,2,3]
Explanation: If perm = [1,2,3], then encoded = [1 XOR 2,2 XOR 3] = [3,1]
```

<p><strong>Example 2:</strong></p>

```clike
Input: encoded = [6,5,4,6]
Output: [2,4,1,5,3]
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>3 &lt;= n &lt;&nbsp;10<sup>5</sup></code></li>
	<li><code>n</code>&nbsp;is odd.</li>
	<li><code>encoded.length == n - 1</code></li>
</ul>

题意：给你一个整数数组&nbsp;<code>perm</code>&nbsp;，它是前&nbsp;<code>n</code>&nbsp;个正整数的排列，且&nbsp;<code>n</code>&nbsp;是个 <strong>奇数</strong>&nbsp;。它被加密成另一个长度为 <code>n - 1</code>&nbsp;的整数数组&nbsp;<code>encoded</code>&nbsp;，满足&nbsp;<code>encoded[i] = perm[i] XOR perm[i + 1]</code>&nbsp;。给你&nbsp;<code>encoded</code>&nbsp;数组，请你返回原始数组&nbsp;<code>perm</code>&nbsp;。题目保证答案存在且唯一。</p>

---
### 解法 位运算+递推
由题意知，`perm[]` 是 `1, ..., n` 的排列，于是 `perm[0] ^ perm[1] ^ ... ^ perm[n - 1] = 1 ^ 2 ^ ... ^ n` 。同时 `encoded[0] = perm[0] ^ perm[1], encode[1] = perm[1] ^ perm[2], ..., encode[i] = perm[i] ^ perm[i + 1], ...` ，可知 `perm[i + 1] = encode[i] ^ perm[i]` 。因此要求出整个原始数组，必须先求出 `perm[0]` 。

很容易想到，我们可以拼凑 `encode[]` 元素，有 `encode[1] ^ encode[3] ^ encode[5] ^ ... = perm[1] ^ perm[2] ^ perm[3] ^ perm[4] ^ perm[5] ^ perm[6] ^ ...` ，再异或 `1 ^ 2 ^ 3 ^ ... ^ n` ，从而得到 `perm[0]` 。接着递推即可求出原始的 `perm[]` 。
```cpp
class Solution {
public:
    vector<int> decode(vector<int>& encoded) {
        int n = encoded.size() + 1;
        vector<int> ans;
        ans.push_back(0);
        for (int i = 1; i <= n; ++i) { //n是奇数
            ans[0] ^= i;
            if (i % 2 == 0) ans[0] ^= encoded[i - 1];
        } 
        for (const int &v : encoded) ans.push_back(ans.back() ^ v);
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：176 ms, 在所有 C++ 提交中击败了32.67% 的用户
内存消耗：99.9 MB, 在所有 C++ 提交中击败了12.75% 的用户
```
由于位运算具有性质 $2i \oplus 2i + 1 = 1$，我们可以在一定程度上优化从 $1$ 到 $n$ 的异或计算。
$$1 \oplus ... \oplus n = \begin{cases}
1, &n = 1\\
0, &n = 3\\
1, &n = 5\\
0, & n = 7\\
...\\
1, &n = 4k + 1\\
0, &n = 4k + 3
\end{cases}
$$
此时代码如下：
```cpp
class Solution {
public:
    vector<int> decode(vector<int>& encoded) {
        int n = encoded.size() + 1; //n是奇数
        vector<int> ans;
        ans.push_back(n % 4 == 1);
        for (int i = 1; i < n - 1; i += 2) ans[0] ^= encoded[i];
        for (int i = 0; i < n - 1; ++i) ans.push_back(ans[i] ^ encoded[i]);
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：144 ms, 在所有 C++ 提交中击败了91.63% 的用户
内存消耗：99.8 MB, 在所有 C++ 提交中击败了27.09% 的用户
```
继续优化，不开辟新的空间，直接使用 `encoded[]` ：
```cpp
class Solution {
public:
    vector<int> decode(vector<int>& encoded) {
        int n = encoded.size() + 1, t = n % 4 == 1, p = 0; //n是奇数
        for (int i = 1; i < n - 1; i += 2) t ^= encoded[i]; //计算p[0]
        for (int i = 0; i < n - 1; ++i) {
            p = t ^ encoded[i]; //计算p[i+1]
            encoded[i] = t; //存储p[i]
            t = p; //保存p[i+1]
        }
        encoded.push_back(t); //保存p[n-1]
        return encoded;
    }
};
```
运行效率如下：
```cpp
执行用时：140 ms, 在所有 C++ 提交中击败了95.22% 的用户
内存消耗：95.8 MB, 在所有 C++ 提交中击败了67.33% 的用户
```
