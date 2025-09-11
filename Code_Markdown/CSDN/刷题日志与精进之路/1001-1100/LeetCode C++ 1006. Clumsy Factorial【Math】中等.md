<p>Normally, the factorial of a positive integer <code>n</code>&nbsp;is the product of all positive integers less than or equal to <code>n</code>.&nbsp; For example, <code>factorial(10) = 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1</code>.</p>

<p>We instead make a <em>clumsy factorial:</em>&nbsp;using the integers in decreasing order, we&nbsp;swap out the multiply operations for a fixed rotation of operations:&nbsp;multiply (*), divide (/), add (+) and subtract (-) in this order.</p>

<p>For example, <code>clumsy(10) = 10 * 9 / 8 + 7 - 6 * 5 / 4 + 3 - 2 * 1</code>.&nbsp; However, these operations are still applied using the usual order of operations of arithmetic: we do all multiplication and division steps before any addition or subtraction steps, and multiplication and division steps are processed left to right.</p>

<p>Additionally, the division that we use is <em>floor division</em>&nbsp;such that&nbsp;<code>10 * 9 / 8</code>&nbsp;equals&nbsp;<code>11</code>.&nbsp; This guarantees the result is&nbsp;an integer.</p>

<p><code><font face="sans-serif, Arial, Verdana, Trebuchet MS">Implement the&nbsp;</font>clumsy</code>&nbsp;function&nbsp;as defined above: given an integer <code>N</code>, it returns the clumsy factorial of <code>N</code>.</p>

 
<p><strong>Example 1:</strong></p>

```csharp
Input: 4
Output: 7
Explanation: 7 = 4 * 3 / 2 + 1
```

<p><strong>Example 2:</strong></p>

```csharp
Input: 10
Output: 12
Explanation: 12 = 10 * 9 / 8 + 7 - 6 * 5 / 4 + 3 - 2 * 1
```

<p><strong>Note:</strong></p> 
<ol>
	<li><code>1 &lt;= N &lt;= 10000</code></li>
	<li><code>-2^31 &lt;= answer &lt;= 2^31 - 1</code>&nbsp; (The answer is guaranteed to fit within a 32-bit integer.)</li>
</ol>


题意：给定一个整数 `N` ，它返回 `N` 的笨阶乘。笨阶乘指的是对于 `N` 的阶乘序列 `N * (N - 1) * (N - 2) * (N - 3) * ... * 2 * 1` ，依次使用 `*, /, +, -` 替换其中的乘法操作符—— `N * (N - 1) / (N - 2) + (N - 3) - ...` ，计算这种做法的结果。

---
### 解法 数学
注意到减法可以变换为加法，即可解决这一题：
```cpp
class Solution {
public:
    int clumsy(int N) {
        int ans = 0, temp = N, oper = 0; //0:*,1:/,2:+,3-
        for (int i = N - 1; i >= 1; --i) {
            if (oper == 0) temp *= i;
            else if (oper == 1) temp /= i;
            else if (oper == 2) temp += i;
            else if (oper == 3) { 
                ans += temp;
                temp = -i;
            }
            oper = (oper + 1) % 4;
        } 
        return ans + temp;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.7 MB, 在所有 C++ 提交中击败了97.58% 的用户
```
