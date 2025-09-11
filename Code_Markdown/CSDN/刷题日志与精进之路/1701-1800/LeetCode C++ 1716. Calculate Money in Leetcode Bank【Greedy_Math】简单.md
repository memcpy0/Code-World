<p>Hercy wants to save money for his first car. He puts money in the Leetcode&nbsp;bank <strong>every day</strong>.</p>

<p>He starts by putting in <code>$1</code> on Monday, the first day. Every day from Tuesday to Sunday, he will put in <code>$1</code> more than the day before. On every subsequent Monday, he will put in <code>$1</code> more than the <strong>previous Monday</strong>.<span style="display: none;"> </span></p>

<p>Given <code>n</code>, return <em>the total amount of money he will have in the Leetcode bank at the end of the </em><code>n<sup>th</sup></code><em> day.</em></p>
 
<p><strong>Example 1:</strong></p>
 

```cpp
Input: n = 4
Output: 10
Explanation: After the 4th day, the total is 1 + 2 + 3 + 4 = 10.
```

 
<p><strong>Example 2:</strong></p>

 
<p><strong>Example 3:</strong></p>

 
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= n &lt;= 1000</code></li>
</ul>

题意：最开始，某人在周一存入力扣银行 `1` 块钱。从周二到周日，他每天都比前一天多存入 `1` 块钱。在接下来每一个周一，他都会比 **前一个周一** 多存入 `1` 块钱。给定 `n` ，返回在第 `n` 天结束的时候此人在力扣银行总共存了多少块钱。

---
### 解法 递归模拟
```cpp
class Solution {
public: 
    int begin = 0;
    int totalMoney(int n) {
        if (n <= 7) return (1 + n) * n / 2; 
        return 28 + (++begin * min(n - 7, 7) + totalMoney(n - 7));
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.9 MB, 在所有 C++ 提交中击败了98.73% 的用户
```
