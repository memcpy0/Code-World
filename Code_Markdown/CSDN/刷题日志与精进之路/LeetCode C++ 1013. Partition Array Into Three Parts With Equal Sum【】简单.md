Given an array `A `of integers, return `true` if and only if we can partition the array into three **non-empty** parts with equal sums.

Formally, we can partition the array if we can find indexes `i+1 < j` with `(A[0] + A[1] + ... + A[i] == A[i+1] + A[i+2] + ... + A[j-1] == A[j] + A[j-1] + ... + A[A.length - 1])` .

**Example 1:**

```haskell
Input: A = [0,2,1,-6,6,-7,9,1,2,0,1]
Output: true
Explanation: 0 + 2 + 1 = -6 + 6 - 7 + 9 + 1 = 2 + 0 + 1
```

**Example 2:**

```haskell
Input: A = [0,2,1,-6,6,7,9,-1,2,0,1]
Output: false
```

**Example 3:**

```haskell
Input: A = [3,3,6,5,-2,2,5,1,-9,4]
Output: true
Explanation: 3 + 3 = 6 = 5 - 2 + 2 + 5 + 1 - 9 + 4
```
**Constraints:**
- `3 <= A.length <= 50000`
-  `-10^4 <= A[i] <= 10^4`




题意：给定一个整数数组 `A`，只有可以将其划分为三个和相等的非空部分时才返回 `true` ，否则返回 `false` 。

形式上，如果可以找出索引 `i + 1 < j` 且满足 `A[0] + A[1] + ... + A[i] == A[i+1] + A[i+2] + ... + A[j-1] == A[j] + A[j-1] + ... + A[A.length - 1]` 就可以将数组三等分。
