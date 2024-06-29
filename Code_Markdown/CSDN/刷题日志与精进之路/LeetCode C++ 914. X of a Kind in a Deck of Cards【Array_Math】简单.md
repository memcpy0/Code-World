<p>In a deck of cards, each card has an integer written on it.</p>

<p>Return <code>true</code> if and only if you can choose&nbsp;<code>X &gt;= 2</code> such that&nbsp;it is possible to split the entire deck&nbsp;into 1 or more groups of cards, where:</p>

<ul>
	<li>Each group has exactly <code>X</code> cards.</li>
	<li>All the cards in each group have the same integer.</li>
</ul>
 
<p><strong>Example 1:</strong></p>

```css
Input: deck = [1,2,3,4,4,3,2,1]
Output: true
Explanation: Possible partition [1,1],[2,2],[3,3],[4,4].
```

<p><strong>Example 2:</strong></p>

```css
Input: deck = [1,1,1,2,2,2,3,3]
Output: false´
Explanation: No possible partition.
```

<p><strong>Example 3:</strong></p>

```css
Input: deck = [1]
Output: false
Explanation: No possible partition. 
```

<p><strong>Example 4:</strong></p>

```cpp
Input: deck = [1,1]
Output: true
Explanation: Possible partition [1,1]. 
```

<p><strong>Example 5:</strong></p>

```css
Input: deck = [1,1,2,2,2,2]
Output: true
Explanation: Possible partition [1,1],[2,2],[2,2].
```
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= deck.length &lt;= 10^4</code></li>
	<li><code>0 &lt;= deck[i] &lt;&nbsp;10^4</code></li>
</ul>

题意：给定一副牌，每张牌上都写着一个整数。此时，你需要选定一个数字 `X` ，使我们可以将整副牌按下述规则分成 `1` 组或更多组：
- 每组都有 `X` 张牌。
- 组内所有的牌上都写着相同的整数。

仅当你可选的 `X >= 2` 时返回 `true` 。 

---
# 解法 最大公约数
统计各个数出现的次数，然后求次数之间的最大公约数。具体代码如下：
```cpp
class Solution {
public:
    bool hasGroupsSizeX(vector<int>& deck) {
        vector<int> cnt(10000); 
        for (const int &v : deck) ++cnt[v]; //哈希计数
        int x = 0; //求gcd
        for (const int &v : cnt) {
            if (v) {
                x = gcd(x, v);
                if (x == 1) return false;
            }
        }
        return true;
    }
};
```
运行效率如下：
```cpp
执行用时：36 ms, 在所有 C++ 提交中击败了85.04% 的用户
内存消耗：21.2 MB, 在所有 C++ 提交中击败了5.19% 的用户
```

