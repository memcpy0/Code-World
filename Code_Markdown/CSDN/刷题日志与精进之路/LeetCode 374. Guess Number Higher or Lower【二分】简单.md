<p>We are playing the Guess Game. The game is as follows:</p>

<p>I pick a number from <code>1</code> to <code>n</code>. You have to guess which number I picked.</p>

<p>Every time you guess wrong, I will tell you whether the number I picked is higher or lower than your guess.</p>

<p>You call a pre-defined API <code>int guess(int num)</code>, which returns 3 possible results:</p>

<ul>
	<li><code>-1</code>: The number I picked is lower than your guess (i.e. <code>pick &lt; num</code>).</li>
	<li><code>1</code>: The number I picked is higher than your guess (i.e. <code>pick &gt; num</code>).</li>
	<li><code>0</code>: The number I picked is equal to your guess (i.e. <code>pick == num</code>).</li>
</ul>

<p>Return <em>the number that I picked</em>.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: n = 10, pick = 6
Output: 6 
```
<p><strong>Example 2:</strong></p>

```cpp
Input: n = 1, pick = 1
Output: 1 
```

</pre><p><strong>Example 3:</strong></p>

```cpp
Input: n = 2, pick = 1
Output: 1 
```

<p><strong>Example 4:</strong></p>

```cpp
Input: n = 2, pick = 2
Output: 2 
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= n &lt;= 2<sup>31</sup> - 1</code></li>
	<li><code>1 &lt;= pick &lt;= n</code></li>
</ul>

题意：猜数字游戏。

---
### 解法 二分
```cpp
/** 
 * Forward declaration of guess API.
 * @param  num   your guess
 * @return 	     -1 if num is lower than the guess number
 *			      1 if num is higher than the guess number
 *               otherwise return 0
 * int guess(int num);
 */
class Solution {
public:
    int guessNumber(int n) {
        int l = 0, h = n;
        while (l <= h) {
            int m = l + (h - l) / 2, t = guess(m);
            if (t == 0) return m;
            else if (t == -1) h = m - 1;
            else l = m + 1; 
        }
        return 0;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.7 MB, 在所有 C++ 提交中击败了82.14% 的用户
```
