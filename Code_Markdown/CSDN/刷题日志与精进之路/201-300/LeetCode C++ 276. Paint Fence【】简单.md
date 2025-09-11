<p>There is a fence with n posts, each post can be painted with one of the k colors.</p>

<p>You have to paint all the posts such that no more than two adjacent fence posts have the same color.</p>

<p>Return the total number of ways you can paint the fence.</p>

<p><b>Note:</b><br>
n and k are non-negative integers.</p>

<p><b>Example:</b></p>
 

```haskell
Input: n = 3, k = 2
Output: 6
Explanation: Take c1 as color 1, c2 as color 2. All possible ways are:

            post1  post2  post3      
 -----      -----  -----  -----       
   1         c1     c1     c2 
   2         c1     c2     c1 
   3         c1     c2     c2 
   4         c2     c1     c1  
   5         c2     c1     c2
   6         c2     c2     c1 
```

题意：有 `k` 种颜色的涂料和一个包含 `n` 个栅栏柱的栅栏，每个栅栏柱可以用其中一种颜色进行上色。

你需要给所有栅栏柱上色，并且保证其中相邻的栅栏柱 **最多连续两个** 颜色相同。然后，返回所有有效涂色的方案数。
 
