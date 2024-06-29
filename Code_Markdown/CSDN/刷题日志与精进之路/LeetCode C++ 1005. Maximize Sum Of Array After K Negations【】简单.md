
Given an array `A` of integers, we **must** modify the array in the following way: we choose an `i` and replace `A[i]` with `-A[i]`, and we repeat this process `K` times in total.  (We may choose the same index `i` multiple times.)

Return the **largest** possible sum of the array after modifying it in this way.

 

**Example 1:**

```haskell
Input: A = [4,2,3], K = 1
Output: 5
Explanation: Choose indices (1,) and A becomes [4,-2,3].
```

**Example 2:**

```haskell
Input: A = [3,-1,0,2], K = 3
Output: 6
Explanation: Choose indices (1, 2, 2) and A becomes [3,1,0,2].
```

**Example 3:**

```haskell
Input: A = [2,-3,-1,5,-4], K = 2
Output: 13
Explanation: Choose indices (1, 4) and A becomes [2,3,-1,5,4].
```
 
**Note:**
- `1 <= A.length <= 10000`
 - `1 <= K <= 10000`
 - `-100 <= A[i] <= 100`

题意：给定一个整数数组 `A` ，只能用以下方法修改该数组：选择某个索引 `i` 并将 `A[i]` 替换为 `-A[i]` ，然后总共重复这个过程 `K` 次（可以多次选择同一个索引 `i` ）。以这种方式修改数组后，返回数组可能的最大和。 
