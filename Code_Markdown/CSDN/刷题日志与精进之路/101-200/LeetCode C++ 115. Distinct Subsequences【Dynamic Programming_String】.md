Given two strings `s` and `t`, *return the number of distinct subsequences of `s` which equals `t`.*

A string's **subsequence** is a new string formed from the original string by deleting some (can be none) of the characters without disturbing the relative positions of the remaining characters. (i.e., `"ACE"` is a subsequence of `"ABCDE"` while `"AEC"` is not).

It's guaranteed the answer fits on a 32-bit signed integer.
 

**Example 1:**
<pre><strong>Input:</strong> s = "rabbbit", t = "rabbit"
<strong>Output:</strong> 3
<strong>Explanation:</strong>
As shown below, there are 3 ways you can generate "rabbit" from S.
<strong><u>rabb</u></strong>b<strong><u>it</u></strong>
<strong><u>ra</u></strong>b<strong><u>bbit</u></strong>
<strong><u>rab</u></strong>b<strong><u>bit</u></strong>
</pre> 

**Example 2:**

<pre><strong>Input:</strong> s = "babgbag", t = "bag"
<strong>Output:</strong> 5
<strong>Explanation:</strong>
As shown below, there are 5 ways you can generate "bag" from S.
<strong><u>ba</u></strong>b<u><strong>g</strong></u>bag
<strong><u>ba</u></strong>bgba<strong><u>g</u></strong>
<u><strong>b</strong></u>abgb<strong><u>ag</u></strong>
ba<u><strong>b</strong></u>gb<u><strong>ag</strong></u>
babg<strong><u>bag</u></strong></pre> 
 
**Constraints:**
-  `0 <= s.length, t.length <= 1000`
-  `s` and `t` consist of English letters.


题意：给定一个字符串 `s` 和一个字符串 `t` ，计算在 `s` 的子序列中 `t` 出现的个数。题目数据保证答案符合 32 位带符号整数范围。
 
---
### 解法 动态规划

