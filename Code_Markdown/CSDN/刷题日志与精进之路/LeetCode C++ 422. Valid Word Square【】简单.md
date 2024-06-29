<p>Given a sequence of words, check whether it forms a valid word square.</p>

<p>A sequence of words forms a valid word square if the <i>k</i><sup>th</sup> row and column read the exact same string, where 0 ≤ <i>k</i> &lt; max(numRows, numColumns).</p>

<p><b>Note:</b><br>
</p><ol>
<li>The number of words given is at least 1 and does not exceed 500.</li>
<li>Word length will be at least 1 and does not exceed 500.</li>
<li>Each word contains only lowercase English alphabet <code>a-z</code>.</li>
</ol>
<p></p>

<p><b>Example 1:</b>
 

```haskell
Input:
[
  "abcd",
  "bnrt",
  "crmy",
  "dtye"
]

Output:
true

Explanation:
The first row and first column both read "abcd".
The second row and second column both read "bnrt".
The third row and third column both read "crmy".
The fourth row and fourth column both read "dtye".

Therefore, it is a valid word square.
```
 
<p><b>Example 2:</b>

```haskell
Input:
[
  "abcd",
  "bnrt",
  "crm",
  "dt"
]

Output:
true

Explanation:
The first row and first column both read "abcd".
The second row and second column both read "bnrt".
The third row and third column both read "crm".
The fourth row and fourth column both read "dt".

Therefore, it is a valid word square. 
```

<p><b>Example 3:</b>

```haskell
Input:
[
 "ball",
 "area",
 "read",
 "lady"
]

Output:
false

Explanation:
The third row reads "read" while the third column reads "lead".

Therefore, it is NOT a valid word square. 
```
题意：给出一个单词序列，判断其是否形成了一个有效的单词方块。有效的单词方块是指此由单词序列组成的文字方块的第 `k` 行 和第 `k` 列 (`0 ≤ k < max(行数, 列数)`) 所显示的字符串完全相同。 
