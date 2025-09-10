<p>There are <code>n</code> students in a class numbered from <code>0</code> to <code>n - 1</code>. The teacher will give each student a problem starting with the student number <code>0</code>, then the student number <code>1</code>, and so on until the teacher reaches the student number <code>n - 1</code>. After that, the teacher will restart the process, starting with the student number <code>0</code> again.</p>

<p>You are given a <strong>0-indexed</strong> integer array <code>chalk</code> and an integer <code>k</code>. There are initially <code>k</code> pieces of chalk. When the student number <code>i</code> is given a problem to solve, they will use <code>chalk[i]</code> pieces of chalk to solve that problem. However, if the current number of chalk pieces is <strong>strictly less</strong> than <code>chalk[i]</code>, then the student number <code>i</code> will be asked to <strong>replace</strong> the chalk.</p>

<p>Return <em>the <strong>index</strong> of the student that will <strong>replace</strong> the chalk</em>.</p>
 
<p><strong>Example 1:</strong></p>

```cpp
Input: chalk = [5,1,5], k = 22
Output: 0
Explanation: The students go in turns as follows:
- Student number 0 uses 5 chalk, so k = 17.
- Student number 1 uses 1 chalk, so k = 16.
- Student number 2 uses 5 chalk, so k = 11.
- Student number 0 uses 5 chalk, so k = 6.
- Student number 1 uses 1 chalk, so k = 5.
- Student number 2 uses 5 chalk, so k = 0.
Student number 0 does not have enough chalk, so they will have to replace it.
```

 
<p><strong>Example 2:</strong></p>

```cpp
Input: chalk = [3,4,1,2], k = 25
Output: 1
Explanation: The students go in turns as follows:
- Student number 0 uses 3 chalk so k = 22.
- Student number 1 uses 4 chalk so k = 18.
- Student number 2 uses 1 chalk so k = 17.
- Student number 3 uses 2 chalk so k = 15.
- Student number 0 uses 3 chalk so k = 12.
- Student number 1 uses 4 chalk so k = 8.
- Student number 2 uses 1 chalk so k = 7.
- Student number 3 uses 2 chalk so k = 5.
- Student number 0 uses 3 chalk so k = 2.
Student number 1 does not have enough chalk, so they will have to replace it.
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>chalk.length == n</code></li>
	<li><code>1 &lt;= n &lt;= 10<sup>5</sup></code></li>
	<li><code>1 &lt;= chalk[i] &lt;= 10<sup>5</sup></code></li>
	<li><code>1 &lt;= k &lt;= 10<sup>9</sup></code></li>
</ul>

题意：一个班级里有&nbsp;<code>n</code>&nbsp;个学生，编号为 <code>0</code>&nbsp;到 <code>n - 1</code>&nbsp;。每个学生会依次回答问题，编号为 <code>0</code>&nbsp;的学生先回答，然后是编号为 <code>1</code>&nbsp;的学生，以此类推，直到编号为 <code>n - 1</code>&nbsp;的学生，然后老师会重复这个过程，重新从编号为 <code>0</code>&nbsp;的学生开始回答问题。

<p>给你一个长度为 <code>n</code>&nbsp;且下标从 <code>0</code>&nbsp;开始的整数数组&nbsp;<code>chalk</code>&nbsp;和一个整数&nbsp;<code>k</code>&nbsp;。一开始粉笔盒里总共有&nbsp;<code>k</code>&nbsp;支粉笔。当编号为&nbsp;<code>i</code>&nbsp;的学生回答问题时，他会消耗 <code>chalk[i]</code>&nbsp;支粉笔。如果剩余粉笔数量 <strong>严格小于</strong>&nbsp;<code>chalk[i]</code>&nbsp;，那么学生 <code>i</code>&nbsp;需要 <strong>补充</strong>&nbsp;粉笔。请你返回需要 <strong>补充</strong>&nbsp;粉笔的学生 <strong>编号</strong>&nbsp;。</p> 

---
### 解法1 数组遍历
对 `chalks[]` 累计求和，由于数据范围比较大，需要使用 `long long` 避免溢出，再将 `k` 对和值取余，接着顺序遍历数组，不断从余值中减去 `chalks[i]` ，直到余值为负数：
```cpp
class Solution {
public:
    int chalkReplacer(vector<int>& chalk, int k) {
        int n = chalk.size();
        long long sum = 0;
        for (int i = 0; i < n; ++i) sum += chalk[i];
        k %= sum;
        for (int i = 0; i < n; ++i) {
            k -= chalk[i];
            if (k < 0) return i;
        }
        return n;
    }
};
```
运行效率如下：
```cpp
执行用时：148 ms, 在所有 C++ 提交中击败了43.85% 的用户
内存消耗：72.6 MB, 在所有 C++ 提交中击败了63.31% 的用户
```
时间复杂度为：$O(chalk.length \times 2)$ ，空间复杂度为：$O(1)$ 。

---
### 解法2 前缀和+二分
```cpp
class Solution {
public:
    int chalkReplacer(vector<int>& chalk, int k) {
        int n = chalk.size();
        vector<long long> sum(n);
        sum[0] = chalk[0];
        for (int i = 1; i < n; ++i) sum[i] = sum[i - 1] + chalk[i];
        k %= sum[n - 1];
        //找到第一个>k的前缀和值位置
        return upper_bound(sum.begin(), sum.end(), k) - sum.begin();
    }
};
```
运行效率如下：
```cpp
执行用时：148 ms, 在所有 C++ 提交中击败了43.85% 的用户
内存消耗：79.2 MB, 在所有 C++ 提交中击败了10.79% 的用户
```
时间复杂度为：$O(chalk.length + \log (chalk.length))$ ，空间复杂度为：$O(chalk.length)$ 。此处效率优势不大。

