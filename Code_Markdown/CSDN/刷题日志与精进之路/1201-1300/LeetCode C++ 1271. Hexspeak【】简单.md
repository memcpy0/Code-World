

A decimal number can be converted to its Hexspeak representation by first converting it to an uppercase hexadecimal string, then replacing all occurrences of the digit `0` with the letter `O`, and the digit `1` with the letter `I`.  Such a representation is valid if and only if it consists only of the letters in the set `{"A", "B", "C", "D", "E", "F", "I", "O"}`.

Given a string `num` representing a decimal integer `N`, return the Hexspeak representation of `N` if it is valid, otherwise return `"ERROR"`.
 
**Example 1:**

```haskell
Input: num = "257"
Output: "IOI"
Explanation:  257 is 101 in hexadecimal.
```

**Example 2:** 
```haskell
Input: num = "3"
Output: "ERROR"
```

**Constraints:**
- `1 <= N <= 10^12`
-  There are no leading zeros in the given string.
-  All answers must be in uppercase letters.

题意：给出一个字符串 `num` ，它表示一个十进制数 `N`，如果它的十六进制魔术数字转换是有效的，请返回转换后的结果，否则返回 `"ERROR"` 。
