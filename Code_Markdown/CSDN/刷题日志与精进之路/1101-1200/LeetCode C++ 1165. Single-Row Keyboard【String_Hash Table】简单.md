<p>There is a special keyboard with <strong>all keys in a single row</strong>.</p>

<p>Given a string <code>keyboard</code> of length 26 indicating the layout of the keyboard (indexed from 0 to&nbsp;25), initially your finger is at index 0. To type a character, you have to move your finger to the index of the desired character. The time taken to move your finger from index <code>i</code> to index <code>j</code> is <code>|i - j|</code>.</p>

<p>You want to type a string <code>word</code>. Write a function to calculate how much time it takes to type it with one finger.</p>

 
<p><strong>Example 1:</strong></p>

```haskell
Input: keyboard = "abcdefghijklmnopqrstuvwxyz", word = "cba"
Output: 4
Explanation: The index moves from 0 to 2 to write 'c' then to 1 to write 'b' then to 0 again to write 'a'.
Total time = 2 + 1 + 1 = 4. 
```

<p><strong>Example 2:</strong></p>

```haskell
Input: keyboard = "pqrstuvwxyzabcdefghijklmno", word = "leetcode"
Output: 73 
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>keyboard.length == 26</code></li>
	<li><code><font face="monospace">keyboard</font></code> contains each&nbsp;English lowercase letter&nbsp;exactly once in some order.</li>
	<li><code>1 &lt;= word.length &lt;= 10^4</code></li>
	<li><code>word[i]</code>&nbsp;is an English lowercase letter.</li>
</ul>

题意：定制了一款特殊的力扣键盘，所有的键都排列在一行上。可以按从左到右的顺序，用一个长度为 `26` 的字符串 `keyboard` 来表示该键盘的键位布局。现在需要测试这个键盘是否能够有效工作，测试需要你使用机械手输出指定的单词 `word` 。最初的时候，机械手位于索引为 `0` 的键的上方，从索引 `i` 移动到索引 `j` 所需要的时间是 `|i - j|` 。计算机械手输出该单词所需的时间。

---
### 解法 哈希
```cpp
class Solution {
public:
    int calculateTime(string keyboard, string word) {
        int pos[26] = {0}, m = keyboard.size(), ans = 0, beginPos = 0;
        for (int i = 0; i < m; ++i) pos[keyboard[i] - 'a'] = i; //保存每个键的布局位置
        for (const char &c : word) {
            ans += abs(pos[c - 'a'] - beginPos);
            beginPos = pos[c - 'a'];
        }
        return ans;
    }
};
```
运行结果如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了97.99% 的用户
内存消耗：7.2 MB, 在所有 C++ 提交中击败了29.05% 的用户
```
