<p>You are playing the following Flip Game with your friend: Given a string that contains only these two characters: <code>+</code> and <code>-</code>, you and your friend take turns to flip two <b>consecutive</b> <code>"++"</code> into <code>"--"</code>. The game ends when a person can no longer make a move and therefore the other person will be the winner.</p>

<p>Write a function to compute all possible states of the string after one valid move.</p>

<p><strong>Example:</strong></p>
 

```haskell
Input: s = "++++"
Output: 
[
  "--++",
  "+--+",
  "++--"
]
```
<p><strong>Note: </strong>If there is no valid move, return an empty list <code>[]</code>.</p> 


题意：你和朋友玩一个叫做「翻转游戏」的游戏，游戏规则：给定一个只有 + 和 - 的字符串。你和朋友轮流将 **连续** 的两个 `"++"` 反转成 `"--"` 。 当一方无法进行有效的翻转时便意味着游戏结束，则另一方获胜。

请你写出一个函数，来计算出第一次翻转后，字符串所有的可能状态。 

