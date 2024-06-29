

For two strings `s` and `t` , we say "`t` divides `s`" if and only if `s = t + ... + t`  (`t` concatenated with itself 1 or more times)

Given two strings `str1` and `str2`, return the largest string `x` such that `x` divides both `str1` and `str2`.

 

**Example 1:**

```haskell
Input: str1 = "ABCABC", str2 = "ABC"
Output: "ABC"
```

**Example 2:**

```haskell
Input: str1 = "ABABAB", str2 = "ABAB"
Output: "AB"
```

**Example 3:**

```haskell
Input: str1 = "LEET", str2 = "CODE"
Output: ""
```

**Example 4:**

```haskell
Input: str1 = "ABCDEF", str2 = "ABC"
Output: "" 
```

**Constraints:**
- `1 <= str1.length <= 1000`
 - `1 <= str2.length <= 1000`
 - `str1` and `str2` consist of English uppercase letters.




题意：对于字符串 `S` 和 `T`，只有在 `S = T + ... + T`（`T` 自身连接 `1` 次或多次）时，我们才认定 `T` 能除尽 `S` 。返回最长字符串 `X` ，要求满足 `X` 能除尽 `str1` 且 `X` 能除尽 `str2` 。

