> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>You are given an <code>m x n</code> matrix <code>board</code>, representing the<strong> current </strong>state of a crossword puzzle. The crossword contains lowercase English letters (from solved words), <code>' '</code> to represent any <strong>empty </strong>cells, and <code>'#'</code> to represent any <strong>blocked</strong> cells.</p>

<p>A word can be placed<strong> horizontally</strong> (left to right <strong>or</strong> right to left) or <strong>vertically</strong> (top to bottom <strong>or</strong> bottom to top) in the board if:</p>

<ul>
	<li>It does not occupy a cell containing the character <code>'#'</code>.</li>
	<li>The cell each letter is placed in must either be <code>' '</code> (empty) or <strong>match</strong> the letter already on the <code>board</code>.</li>
	<li>There must not be any empty cells <code>' '</code> or other lowercase letters <strong>directly left or right</strong><strong> </strong>of the word if the word was placed <strong>horizontally</strong>.</li>
	<li>There must not be any empty cells <code>' '</code> or other lowercase letters <strong>directly above or below</strong> the word if the word was placed <strong>vertically</strong>.</li>
</ul>

<p>Given a string <code>word</code>, return <code>true</code><em> if </em><code>word</code><em> can be placed in </em><code>board</code><em>, or </em><code>false</code><em> <strong>otherwise</strong></em>.</p>


<p><strong>Example 1:</strong></p>
<img style="width: 170px; height: 150px;" src="https://assets.leetcode.com/uploads/2021/09/18/crossword-1.png" alt="">


```cpp
Input: board = [["#", " ", "#"], [" ", " ", "#"], ["#", "c", " "]], word = "abc"
Output: true
Explanation: The word "abc" can be placed as shown above (top to bottom).
```

<p><strong>Example 2:</strong></p>
<img style="width: 170px; height: 151px;" src="https://assets.leetcode.com/uploads/2021/09/18/c2.png" alt="">
 

```cpp
Input: board = [[" ", "#", "a"], [" ", "#", "c"], [" ", "#", "a"]], word = "ac"
Output: false
Explanation: It is impossible to place the word because there will always be a space/letter above or below it.
```

<p><strong>Example 3:</strong></p>
<img style="width: 171px; height: 146px;" src="https://assets.leetcode.com/uploads/2021/09/18/crossword-2.png" alt="">

```cpp
Input: board = [["#", " ", "#"], [" ", " ", "#"], ["#", " ", "c"]], word = "ca"
Output: true
Explanation: The word "ca" can be placed as shown above (right to left).
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>m == board.length</code></li>
	<li><code>n == board[i].length</code></li>
	<li><code>1 &lt;= m * n &lt;= 2 * 10<sup>5</sup></code></li>
	<li><code>board[i][j]</code> will be <code>' '</code>, <code>'#'</code>, or a lowercase English letter.</li>
	<li><code>1 &lt;= word.length &lt;= max(m, n)</code></li>
	<li><code>word</code> will contain only lowercase English letters.</li>
</ul>


题意：给你一个&nbsp;<code>m x n</code>&nbsp;的矩阵&nbsp;<code>board</code>&nbsp;，它代表一个填字游戏&nbsp;<strong>当前</strong>&nbsp;的状态。填字游戏格子中包含小写英文字母（已填入的单词），表示&nbsp;<strong>空</strong>&nbsp;格的&nbsp;<code>' '</code>&nbsp;和表示&nbsp;<strong>障碍</strong>&nbsp;格子的&nbsp;<code>'#'</code>&nbsp;。</p>

<p>如果满足以下条件，那么我们可以 <strong>水平</strong>&nbsp;（从左到右 <strong>或者</strong>&nbsp;从右到左）或 <strong>竖直</strong>&nbsp;（从上到下 <strong>或者</strong>&nbsp;从下到上）填入一个单词：</p>

<ul>
	<li>该单词不占据任何&nbsp;<code>'#'</code>&nbsp;对应的格子。</li>
	<li>每个字母对应的格子要么是&nbsp;<code>' '</code>&nbsp;（空格）要么与 <code>board</code>&nbsp;中已有字母 <strong>匹配</strong>&nbsp;。</li>
	<li>如果单词是 <strong>水平</strong>&nbsp;放置的，那么该单词左边和右边 <strong>相邻</strong>&nbsp;格子不能为&nbsp;<code>' '</code>&nbsp;或小写英文字母。</li>
	<li>如果单词是&nbsp;<strong>竖直</strong>&nbsp;放置的，那么该单词上边和下边&nbsp;<strong>相邻</strong><strong>&nbsp;</strong>格子不能为&nbsp;<code>' '</code>&nbsp;或小写英文字母。</li>
</ul>

<p>给你一个字符串&nbsp;<code>word</code>&nbsp;，如果&nbsp;<code>word</code>&nbsp;可以被放入&nbsp;<code>board</code>&nbsp;中，请你返回&nbsp;<code>true</code>&nbsp;，否则请返回&nbsp;<code>false</code>&nbsp;。</p>

---
### 解法1 枚举+剪枝+DFS
枚举所有可放置字符串首位的格子，注意判断各个边界条件（**剪枝策略**），比如相邻格子、是否越界等，然后DFS看看能否向上、下、左、右放置这个字符串：
```cpp
//C++ version
class Solution {
private:
    int d[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    bool dfs(const vector<vector<char>>& b, const string& w, int i, int j, int k, int cur) {
        if (cur >= w.size()) return true;
        if (b[i][j] == '#' || (b[i][j] != ' ' && b[i][j] != w[cur])) return false;
        return dfs(b, w, i + d[k][0], j + d[k][1], k, cur + 1);
    }
public:
    bool placeWordInCrossword(vector<vector<char>>& board, string word) {
        int m = board.size(), n = board[0].size(), len = word.size();
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (board[i][j] != '#' && (board[i][j] == ' ' || board[i][j] == word[0])) {
                    int ti0 = i + len - 1, ti1 = i - len + 1, tj0 = j + len - 1, tj1 = j - len + 1; 
                    if ((ti0 < m && board[ti0][j] != '#') && //up to down
                        (i == 0 || board[i - 1][j] == '#') &&
                        (ti0 == m - 1 || board[ti0 + 1][j] == '#') &&
                        dfs(board, word, i, j, 0, 0)) return true;
                    if ((ti1 >= 0 && board[ti1][j] != '#') && //down to up
                        (i == m - 1 || board[i + 1][j] == '#') &&
                        (ti1 == 0 || board[ti1 - 1][j] == '#') &&
                        dfs(board, word, i, j, 1, 0)) return true;
                    if ((tj0 < n && board[i][tj0] != '#') && //left to right
                        (j == 0 || board[i][j - 1] == '#') &&
                        (tj0 == n - 1 || board[i][tj0 + 1] == '#') &&
                        dfs(board, word, i, j, 2, 0)) return true;
                    if ((tj1 >= 0 && board[i][tj1] != '#') && //right to left
                        (j == n - 1 || board[i][j + 1] == '#') &&
                        (tj1 == 0 || board[i][tj1 - 1] == '#') &&
                        dfs(board, word, i, j, 3, 0)) return true;
                }
            }
        }
        return false;
    }
};
//执行用时：148 ms, 在所有 C++ 提交中击败了99.73% 的用户
//内存消耗：58 MB, 在所有 C++ 提交中击败了51.91% 的用户
```


---
### 解法2 字符串匹配
题意很显然，字符串 `word` 只能放置在两个 `#` 之间的槽位（矩阵边界也视作 `#` ），于是可以遍历（正序+倒序）每个槽位，判断 `word` 能否恰好填入该槽位。虽然思路有些不同，但是核心想法与解法一也是一致的，相应代码就留待日后更新。
