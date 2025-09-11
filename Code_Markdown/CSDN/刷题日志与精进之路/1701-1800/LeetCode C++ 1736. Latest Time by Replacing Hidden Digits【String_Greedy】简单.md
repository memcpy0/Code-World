<p>You are given a string <code>time</code> in the form of <code> hh:mm</code>, where some of the digits in the string are hidden (represented by <code>?</code>).</p>

<p>The valid times are those inclusively between <code>00:00</code> and <code>23:59</code>.</p>

<p>Return <em>the latest valid time you can get from</em> <code>time</code><em> by replacing the hidden</em> <em>digits</em>.</p>

 
<p><strong>Example 1:</strong></p>

```clike
Input: time = "2?:?0"
Output: "23:50"
Explanation: The latest hour beginning with the digit '2' is 23 and the latest minute ending with the digit '0' is 50.
```

<p><strong>Example 2:</strong></p>

```clike
Input: time = "0?:3?"
Output: "09:39" 
```

<p><strong>Example 3:</strong></p>

```clike
Input: time = "1?:22"
Output: "19:22"
```

 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>time</code> is in the format <code>hh:mm</code>.</li>
	<li>It is guaranteed that you can produce a valid time from the given string.</li>
</ul>


题意：给你一个字符串 `time` ，格式为 `hh:mm`（小时：分钟），其中某几位数字被隐藏（用 `?` 表示）。有效的时间为 `00:00` 到 `23:59` 之间的所有时间，包括 `00:00` 和 `23:59` 。替换 `time` 中隐藏的数字，返回你可以得到的最晚有效时间。

---
### 解法 贪心
很简单的题目：
```cpp
class Solution {
public:
    string maximumTime(string time) {
        if (time[0] == '?') time[0] = ((time[1] == '?' || time[1] <= '3') ? '2' : '1');
        if (time[1] == '?') time[1] = (time[0] == '2' ? '3' : '9');
        if (time[3] == '?') time[3] = '5';
        if (time[4] == '?') time[4] = '9';
        return time;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.8 MB, 在所有 C++ 提交中击败了30.13% 的用户
```
