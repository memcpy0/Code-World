

Given a non-negative integer num represented as a string, remove k digits from the number so that the new number is the **smallest** possible.

**Note:**
- The length of num is less than `10002` and will be `≥ k` .
 - The given num does not contain any leading zero.

**Example 1:**

```swift
Input: num = "1432219", k = 3
Output: "1219"
Explanation: Remove the three digits 4, 3, and 2 to form the new number 1219 which is the smallest.
```

**Example 2:**

```swift
Input: num = "10200", k = 1
Output: "200"
Explanation: Remove the leading 1 and the number is 200. Note that the output must not contain leading zeroes.
```

**Example 3:**

```swift
Input: num = "10", k = 2
Output: "0"
Explanation: Remove all the digits from the number and it is left with nothing which is 0.
```
题意：给出一个长度小于 `10002` 的整数字符串 `num` ，移除 `k` 位使得新整数最小化。

---
### 解法 单调栈
尽管标准解法是单调栈，但是我们不一定能够直接想到这种方法，更可能想到的是**贪心**。

怎样贪心呢？因为我们要移除 $k$ 个字符

我们从前往后遍历，如果发现字符 `s[i] > s[i + 1]` ，不添加 `s[i]` （相当于删除 `s[i]`）；否则添加字符 `s[i]` 。直到删除了  `k` 个字符为止。


