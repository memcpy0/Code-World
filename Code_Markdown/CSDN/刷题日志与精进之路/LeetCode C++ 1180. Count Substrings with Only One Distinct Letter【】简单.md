
Given a string `S`, return the number of substrings that have only **one distinct** letter.

 

**Example 1:**

```haskell
Input: S = "aaaba"
Output: 8
Explanation: The substrings with one distinct letter are "aaa", "aa", "a", "b".
"aaa" occurs 1 time.
"aa" occurs 2 times.
"a" occurs 4 times.
"b" occurs 1 time.
So the answer is 1 + 2 + 4 + 1 = 8.
```

**Example 2:**

```haskell
Input: S = "aaaaaaaaaa"
Output: 55
```
 
**Constraints:**
- `1 <= S.length <= 1000`
-  `S[i]` consists of only lowercase English letters.

题意：给出一个字符串 `S` ，返回只含**单一字母**的子串个数。
