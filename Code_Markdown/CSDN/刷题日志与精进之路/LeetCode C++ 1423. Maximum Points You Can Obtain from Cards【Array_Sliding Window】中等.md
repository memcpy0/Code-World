<p>There are several cards&nbsp;<strong>arranged in a row</strong>, and each card has an associated number of points&nbsp;The points are given in the integer array&nbsp;<code>cardPoints</code>.</p>

<p>In one step, you can take one card from the beginning or from the end of the row. You have to take exactly <code>k</code> cards.</p>

<p>Your score is the sum of the points of the cards you have taken.</p>

<p>Given the integer array <code>cardPoints</code> and the integer <code>k</code>, return the <em>maximum score</em> you can obtain.</p>

 
<p><strong>Example 1:</strong></p>

```css
Input: cardPoints = [1,2,3,4,5,6,1], k = 3
Output: 12
Explanation: After the first step, your score will always be 1. However, choosing the rightmost card first will maximize your total score. The optimal strategy is to take the three cards on the right, giving a final score of 1 + 6 + 5 = 12.
```
 
<p><strong>Example 2:</strong></p>

```css
Input: cardPoints = [2,2,2], k = 2
Output: 4
Explanation: Regardless of which two cards you take, your score will always be 4.
```

 
<p><strong>Example 3:</strong></p>

```css
Input: cardPoints = [9,7,7,9,7,7,9], k = 7
Output: 55
Explanation: You have to take all the cards. Your score is the sum of points of all cards.
```

<p><strong>Example 4:</strong></p>

```css
Input: cardPoints = [1,1000,1], k = 1
Output: 1
Explanation: You cannot take the card in the middle. Your best score is 1. 
```
 
<p><strong>Example 5:</strong></p>

```css
Input: cardPoints = [1,79,80,1,1,1,200,1], k = 3
Output: 202
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= cardPoints.length &lt;= 10^5</code></li>
	<li><code>1 &lt;= cardPoints[i] &lt;= 10^4</code></li>
	<li><code>1 &lt;= k &lt;= cardPoints.length</code></li>
</ul>

题意：几张卡牌排成一行，每张卡牌都有一个对应的点数。点数由整数数组 `cardPoints` 给出。每次行动可以从行的开头或者末尾拿一张卡牌，最终必须正好拿 `k` 张卡牌，返回可以拿到手中的所有卡牌的最大点数之和。

---
### 解法 滑动窗口
这道题看起来有点难度，但是只要想明白了，就很简单。假设我们从开头拿到了 `x` 张卡牌，那么在末尾就只能拿 `k - x` 张卡牌，中间的卡牌序列有 `cardPoints.length - k` 张卡牌。要想使得我们手中的 `k` 张卡牌点数之和最大，只需要想办法令中间的卡牌序列点数之和最小。

从而，问题转换为：找到 `cardPoints` 数组中一个长度为 `cardPoints.length - k` 的子数组，使得子数组中所有卡牌点数之和最小。我们用固定长度的滑动窗口解决这一题：
```cpp
class Solution {
public:
    int maxScore(vector<int>& cardPoints, int k) {
        int n = cardPoints.size(), t = n - k, sum = 0, partsum = 0, minWinSum = INT_MAX; 
        for (const int &v : cardPoints) sum += v;
        //找到一个固定长度为t的子数组,其中卡牌点数之和最小
        for (int i = 0; i < n; ++i) { //i为滑动窗口右端
            partsum += cardPoints[i]; //滑动窗口右端扩展
            if (i >= t) partsum -= cardPoints[i - t]; //超出固定长度时滑动窗口左端收缩
            if (i + 1 >= t) minWinSum = min(minWinSum, partsum); //达到固定长度时记录
        }
        return sum - minWinSum;
    }
};
```
运行效率如下：
```cpp
执行用时：72 ms, 在所有 C++ 提交中击败了31.57% 的用户
内存消耗：41.4 MB, 在所有 C++ 提交中击败了60.60% 的用户
```
