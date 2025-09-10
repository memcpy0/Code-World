> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>You are given a string <code>s</code> that contains digits <code>0-9</code>, addition symbols <code>'+'</code>, and multiplication symbols <code>'*'</code> <strong>only</strong>, representing a <strong>valid</strong> math expression of <strong>single digit numbers</strong> (e.g., <code>3+5*2</code>). This expression was given to <code>n</code> elementary school students. The students were instructed to get the answer of the expression by following this <strong>order of operations</strong>:</p>

<ol>
	<li>Compute <strong>multiplication</strong>, reading from <strong>left to right</strong>; Then,</li>
	<li>Compute <strong>addition</strong>, reading from <strong>left to right</strong>.</li>
</ol>

<p>You are given an integer array <code>answers</code> of length <code>n</code>, which are the submitted answers of the students in no particular order. You are asked to grade the <code>answers</code>, by following these <strong>rules</strong>:</p>

<ul>
	<li>If an answer <strong>equals</strong> the correct answer of the expression, this student will be rewarded <code>5</code> points;</li>
	<li>Otherwise, if the answer <strong>could be interpreted</strong> as if the student applied the operators <strong>in the wrong order</strong> but had <strong>correct arithmetic</strong>, this student will be rewarded <code>2</code> points;</li>
	<li>Otherwise, this student will be rewarded <code>0</code> points.</li>
</ul>

<p>Return <em>the sum of the points of the students</em>.</p>


<p><strong>Example 1:</strong></p>
<img style="width: 678px; height: 109px;" src="https://assets.leetcode.com/uploads/2021/09/17/student_solving_math.png" alt="">
<pre><strong>Input:</strong> s = "7+3*1*2", answers = [20,13,42]
<strong>Output:</strong> 7
<strong>Explanation:</strong> As illustrated above, the correct answer of the expression is 13, therefore one student is rewarded 5 points: [20,<u><strong>13</strong></u>,42]
A student might have applied the operators in this wrong order: ((7+3)*1)*2 = 20. Therefore one student is rewarded 2 points: [<u><strong>20</strong></u>,13,42]
The points for the students are: [2,5,0]. The sum of the points is 2+5+0=7.
</pre>

<p><strong>Example 2:</strong></p>

<pre><strong>Input:</strong> s = "3+5*2", answers = [13,0,10,13,13,16,16]
<strong>Output:</strong> 19
<strong>Explanation:</strong> The correct answer of the expression is 13, therefore three students are rewarded 5 points each: [<strong><u>13</u></strong>,0,10,<strong><u>13</u></strong>,<strong><u>13</u></strong>,16,16]
A student might have applied the operators in this wrong order: ((3+5)*2 = 16. Therefore two students are rewarded 2 points: [13,0,10,13,13,<strong><u>16</u></strong>,<strong><u>16</u></strong>]
The points for the students are: [5,0,0,5,5,2,2]. The sum of the points is 5+0+0+5+5+2+2=19.
</pre>

<p><strong>Example 3:</strong></p>

<pre><strong>Input:</strong> s = "6+0*1", answers = [12,9,6,4,8,6]
<strong>Output:</strong> 10
<strong>Explanation:</strong> The correct answer of the expression is 6.
If a student had incorrectly done (6+0)*1, the answer would also be 6.
By the rules of grading, the students will still be rewarded 5 points (as they got the correct answer), not 2 points.
The points for the students are: [0,0,5,0,0,5]. The sum of the points is 10.
</pre>

<p><strong>Example 4:</strong></p>

<pre><strong>Input:</strong> s = "1+2*3+4", answers = [13,21,11,15]
<strong>Output:</strong> 11
<strong>Explanation:</strong> The correct answer of the expression is 11.
Every other student was rewarded 2 points because they could have applied the operators as follows:
- ((1+2)*3)+4 = 13
- (1+2)*(3+4) = 21
- 1+(2*(3+4)) = 15
The points for the students are: [2,2,5,2]. The sum of the points is 11.
</pre>

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>3 &lt;= s.length &lt;= 31</code></li>
	<li><code>s</code> represents a valid expression that contains only digits <code>0-9</code>, <code>'+'</code>, and <code>'*'</code> only.</li>
	<li>All the integer operands in the expression are in the <strong>inclusive</strong> range <code>[0, 9]</code>.</li>
	<li><code>1 &lt;=</code> The count of all operators (<code>'+'</code> and <code>'*'</code>) in the math expression <code>&lt;= 15</code></li>
	<li>Test data are generated such that the correct answer of the expression is in the range of <code>[0, 1000]</code>.</li>
	<li><code>n == answers.length</code></li>
	<li><code>1 &lt;= n &lt;= 10<sup>4</sup></code></li>
	<li><code>0 &lt;= answers[i] &lt;= 1000</code></li>
</ul>


题意：给你一个字符串&nbsp;<code>s</code>&nbsp;，它 <strong>只</strong> 包含数字&nbsp;<code>0-9</code>&nbsp;，加法运算符&nbsp;<code>'+'</code>&nbsp;和乘法运算符&nbsp;<code>'*'</code>&nbsp;，这个字符串表示一个&nbsp;<strong>合法</strong>&nbsp;的只含有&nbsp;<strong>个位数</strong><strong>数字</strong>&nbsp;的数学表达式（比方说&nbsp;<code>3+5*2</code>）。有 <code>n</code>&nbsp;位小学生将计算这个数学表达式，并遵循如下 <strong>运算顺序</strong>&nbsp;：
<ol>
	<li>按照 <strong>从左到右</strong>&nbsp;的顺序计算 <strong>乘法</strong>&nbsp;，然后</li>
	<li>按照 <strong>从左到右</strong>&nbsp;的顺序计算 <strong>加法</strong>&nbsp;。</li>
</ol>

<p>给你一个长度为 <code>n</code>&nbsp;的整数数组&nbsp;<code>answers</code>&nbsp;，表示每位学生提交的答案。你的任务是给 <code>answer</code>&nbsp;数组按照如下 <strong>规则</strong>&nbsp;打分：</p>

<ul>
	<li>如果一位学生的答案 <strong>等于</strong>&nbsp;表达式的正确结果，这位学生将得到 <code>5</code>&nbsp;分。</li>
	<li>否则，如果答案由&nbsp;<strong>一处或多处错误的运算顺序</strong>&nbsp;计算得到，那么这位学生能得到 <code>2</code>&nbsp;分。</li>
	<li>否则，这位学生将得到 <code>0</code>&nbsp;分。</li>
</ul>

<p>请你返回所有学生的分数和。</p>
 
