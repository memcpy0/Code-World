<p>Given a nested list of integers, return the sum of all integers in the list weighted by their depth.</p>

<p>Each element is either an integer, or a list -- whose elements may also be integers or other lists.</p>

<div>
<p><strong>Example 1:</strong></p>

```haskell
Input: [[1,1],2,[1,1]]
Output: 10 
Explanation: Four 1's at depth 2, one 2 at depth 1. 
```

<div>
<p><strong>Example 2:</strong></p>

```haskell
Input: [1,[4,[6]]]
Output: 27 
Explanation: One 1 at depth 1, one 4 at depth 2, and one 6 at depth 3; 1 + 4*2 + 6*3 = 27. 
```


题意：给定一个嵌套的整数列表，请返回该列表按深度加权后所有整数的总和。

每个元素要么是整数，要么是列表。同时，列表中元素同样也可以是整数或者是另一个列表。
