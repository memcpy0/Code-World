<p>You are given two integers <code>memory1</code> and <code>memory2</code> representing the available memory in bits on two memory sticks. There is currently a faulty program running that consumes an increasing amount of memory every second.</p>

<p>At the <code>i<sup>th</sup></code> second (starting from 1), <code>i</code> bits of memory are allocated to the stick with <strong>more available memory</strong> (or from the first memory stick if both have the same available memory). If neither stick has at least <code>i</code> bits of available memory, the program <strong>crashes</strong>.</p>

<p>Return <em>an array containing </em><code>[crashTime, memory1<sub>crash</sub>, memory2<sub>crash</sub>]</code><em>, where </em><code>crashTime</code><em> is the time (in seconds) when the program crashed and </em><code>memory1<sub>crash</sub></code><em> and </em><code>memory2<sub>crash</sub></code><em> are the available bits of memory in the first and second sticks respectively</em>.</p>

 
<p><strong>Example 1:</strong></p>

```clike
Input: memory1 = 2, memory2 = 2
Output: [3,1,0]
Explanation: The memory is allocated as follows:
- At the 1st second, 1 bit of memory is allocated to stick 1. The first stick now has 1 bit of available memory.
- At the 2nd second, 2 bits of memory are allocated to stick 2. The second stick now has 0 bits of available memory.
- At the 3rd second, the program crashes. The sticks have 1 and 0 bits available respectively.
```

<p><strong>Example 2:</strong></p>

```clike
Input: memory1 = 8, memory2 = 11
Output: [6,0,4]
Explanation: The memory is allocated as follows:
- At the 1st second, 1 bit of memory is allocated to stick 2. The second stick now has 10 bit of available memory.
- At the 2nd second, 2 bits of memory are allocated to stick 2. The second stick now has 8 bits of available memory.
- At the 3rd second, 3 bits of memory are allocated to stick 1. The first stick now has 5 bits of available memory.
- At the 4th second, 4 bits of memory are allocated to stick 2. The second stick now has 4 bits of available memory.
- At the 5th second, 5 bits of memory are allocated to stick 1. The first stick now has 0 bits of available memory.
- At the 6th second, the program crashes. The sticks have 0 and 4 bits available respectively. 
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>0 &lt;= memory1, memory2 &lt;= 2<sup>31</sup> - 1</code></li>
</ul>

题意：给你两个整数&nbsp;<code>memory1</code> 和&nbsp;<code>memory2</code>&nbsp;分别表示两个内存条剩余可用内存的位数。现在有一个程序每秒递增的速度消耗着内存。</p>

<p>在第&nbsp;<code>i</code>&nbsp;秒（秒数从 1 开始），有 <code>i</code>&nbsp;位内存被分配到&nbsp;<strong>剩余内存较多</strong>&nbsp;的内存条（如果两者一样多，则分配到第一个内存条）。如果两者剩余内存都不足 <code>i</code>&nbsp;位，那么程序将 <b>意外退出</b>&nbsp;。</p>

<p>请你返回一个数组，包含<em> </em><code>[crashTime, memory1<sub>crash</sub>, memory2<sub>crash</sub>]</code>&nbsp;，其中&nbsp;<code>crashTime</code>是程序意外退出的时间（单位为秒），<em>&nbsp;</em><code>memory1<sub>crash</sub></code><em> </em>和<em>&nbsp;</em><code>memory2<sub>crash</sub></code><em>&nbsp;</em>分别是两个内存条最后剩余内存的位数。</p>

---
### 解法 模拟
由于分配的内存数成等差数列增长，所以简单的模拟就可以过本题：
```cpp
class Solution {
public:
    vector<int> memLeak(int memory1, int memory2) {
        int i;
        for (i = 1; memory1 >= i || memory2 >= i; ++i) {
            if (memory1 >= memory2) memory1 -= i;
            else memory2 -= i;
        }
        return {i, memory1, memory2}; 
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6 MB, 在所有 C++ 提交中击败了72.03% 的用户
```
