
<p>Given an array <code>A</code>&nbsp;of 0s and 1s, we may change up to <code>K</code>&nbsp;values from 0 to 1.</p>

<p>Return the length of the longest (contiguous) subarray that contains only 1s.&nbsp;</p>

 
<div>
<p><strong>Example 1:</strong></p>

<pre><strong>Input: </strong>A = <span id="example-input-1-1">[1,1,1,0,0,0,1,1,1,1,0]</span>, K = <span id="example-input-1-2">2</span>
<strong>Output: </strong><span id="example-output-1">6</span>
<strong>Explanation: </strong>
[1,1,1,0,0,<u><strong>1</strong>,1,1,1,1,<strong>1</strong></u>]
Bolded numbers were flipped from 0 to 1.  The longest subarray is underlined.</pre>

<div>
<p><strong>Example 2:</strong></p>

<pre><strong>Input: </strong>A = <span id="example-input-2-1">[0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1]</span>, K = <span id="example-input-2-2">3</span>
<strong>Output: </strong><span id="example-output-2">10</span>
<strong>Explanation: </strong>
[0,0,<u>1,1,<b>1</b>,<strong>1</strong>,1,1,1,<strong>1</strong>,1,1</u>,0,0,0,1,1,1,1]
Bolded numbers were flipped from 0 to 1.  The longest subarray is underlined.
</pre>
 
<p><strong><span>Note:</span></strong></p>

<ol>
	<li><code>1 &lt;= A.length &lt;= 20000</code></li>
	<li><code>0 &lt;= K &lt;= A.length</code></li>
	<li><code>A[i]</code> is <code>0</code> or <code>1</code>&nbsp;</li>
</ol>
</div>
</div>

题意：给定一个由若干 `0` 和 `1` 组成的数组 `A`，我们最多可以将 `K` 个值从 `0` 变成 `1` 。返回仅包含 `1` 的最长（连续）子数组的长度。

---
### 解法 滑动窗口
非固定长度的滑动窗口。当前元素为 `1` 或者还能翻转 `0` 为 `1` 时窗口扩张；当前元素为 `0` 且翻转次数已经用完时，窗口左端收缩，直到滑动窗口中零的个数小于 `K` 为止，此时我们重新拥有翻转 `0` 值的能力。
```cpp
class Solution {
public:
    int longestOnes(vector<int>& A, int K) {
        int n = A.size(), remainZeros = K, maxOnesLen = 0, left = 0, tempLen = 0;
        for (int i = 0; i < n; ++i) {
            if (A[i] == 1 || remainZeros > 0) {
                ++tempLen;
                if (A[i] == 0) --remainZeros;
            } else { //A[i] == 0 && remainZeros == 0
                while (left < i) {
                    ++left, --tempLen;
                    if (A[left - 1] == 0) { remainZeros = 1; break; }
                }
                if (remainZeros > 0) ++tempLen, --remainZeros; //处理A[i]
                if (K == 0) ++left; //注意K等于0的特殊情况
            }
            maxOnesLen = max(maxOnesLen, tempLen);
        }
        return maxOnesLen;
    }
};
```
运行效率如下：
```cpp
执行用时：68 ms, 在所有 C++ 提交中击败了58.52% 的用户
内存消耗：50.2 MB, 在所有 C++ 提交中击败了61.75% 的用户
```
