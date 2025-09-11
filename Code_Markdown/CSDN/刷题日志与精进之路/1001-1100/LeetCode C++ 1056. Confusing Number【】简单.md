<p>Given a number <code>N</code>, return <code>true</code> if and only if it is a <em>confusing number</em>, which satisfies the following condition:</p>

<p>We can rotate digits by 180 degrees to form new digits. When 0, 1, 6, 8, 9 are rotated 180 degrees, they become 0, 1, 9, 8, 6 respectively. When 2, 3, 4, 5 and 7 are rotated 180 degrees, they become invalid. A <em>confusing number</em> is a number that when rotated 180 degrees becomes a <strong>different</strong> number with each digit valid.</p>


<p><strong>Example 1:</strong></p>

<p><img style="width: 180px; height: 90px;" src="https://assets.leetcode.com/uploads/2019/03/23/1268_1.png" alt=""></p>

```haskell
Input: 6
Output: true
Explanation: 
We get 9 after rotating 6, 9 is a valid number and 9!=6.
```

 
<p><strong>Example 2:</strong></p>

<p><img style="width: 180px; height: 90px;" src="https://assets.leetcode.com/uploads/2019/03/23/1268_2.png" alt=""></p>

```haskell
Input: 89
Output: true
Explanation: 
We get 68 after rotating 89, 86 is a valid number and 86!=89.
```

 
<p><strong>Example 3:</strong></p>

<p><img style="width: 301px; height: 121px;" src="https://assets.leetcode.com/uploads/2019/03/26/1268_3.png" alt=""></p>

```haskell
Input: 11
Output: false
Explanation: 
We get 11 after rotating 11, 11 is a valid number but the value remains the same, thus 11 is not a confusing number.
```
 
<p><strong>Example 4:</strong></p>

<p><img style="width: 180px; height: 90px;" src="https://assets.leetcode.com/uploads/2019/03/23/1268_4.png" alt=""></p>

```haskell
Input: 25
Output: false
Explanation: 
We get an invalid number after rotating 25.
```

 
<p><strong>Note:</strong></p>

<ol>
	<li><code>0 &lt;= N &lt;= 10^9</code></li>
	<li>After the rotation we can ignore leading zeros, for example if after rotation we have <code>0008</code>&nbsp;then this number is considered as just <code>8</code>.</li>
</ol>


题意：给定一个数字 `N`，当原数字旋转 `180°` 以后可以得到新的数字的时候返回 `true`。易混淆数 (`confusing number`) 在旋转180°以后，可以得到和原来不同的数，且新数字的每一位都是有效的。
 
