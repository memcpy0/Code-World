<p>
Given a <b>non-empty</b> string <code>s</code> and an abbreviation <code>abbr</code>, return whether the string matches with the given abbreviation.
</p>

<p>A string such as <code>"word"</code> contains only the following valid abbreviations:</p>

```haskell
["word", "1ord", "w1rd", "wo1d", "wor1", "2rd", "w2d", "wo2", "1o1d", "1or1", "w1r1", "1o2", "2r1", "3d", "w3", "4"]
```

 
<p>Notice that only the above abbreviations are valid abbreviations of the string <code>"word"</code>. Any other string is not a valid abbreviation of <code>"word"</code>.</p>

<p><b>Note:</b><br>
Assume <code>s</code> contains only lowercase letters and <code>abbr</code> contains only lowercase letters and digits.
</p>

<p><b>Example 1:</b><br>

```haskell
Given s = "internationalization", abbr = "i12iz4n":

Return true.
```

<p><b>Example 2:</b><br>

```haskell
Given s = "apple", abbr = "a2e":

Return false.
```
题意：给一个**非空**字符串 `s` 和一个单词缩写 `abbr` ，判断这个缩写是否可以是给定单词的缩写。
