<p>Given a list of the scores of different students, <code>items</code>, where <code>items[i] = [ID<sub>i</sub>, score<sub>i</sub>]</code> represents one score from a student with <code>ID<sub>i</sub></code>, calculate each student's <strong>top five average</strong>.</p>

<p>Return <em>the answer as an array of pairs </em><code>result</code><em>, where </em><code>result[j] = [ID<sub>j</sub>, topFiveAverage<sub>j</sub>]</code><em> represents the student with </em><code>ID<sub>j</sub></code><em> and their <strong>top five average</strong>. Sort </em><code>result</code><em> by </em><code>ID<sub>j</sub></code><em> in <strong>increasing order</strong>.</em></p>

<p>A student's <strong>top five average</strong> is calculated by taking the sum of their top five scores and dividing it by <code>5</code> using <strong>integer division</strong>.</p>
 
<p><strong>Example 1:</strong></p>

```haskell
Input: items = [[1,91],[1,92],[2,93],[2,97],[1,60],[2,77],[1,65],[1,87],[1,100],[2,100],[2,76]]
Output: [[1,87],[2,88]]
Explanation: 
The student with ID = 1 got scores 91, 92, 60, 65, 87, and 100. Their top five average is (100 + 92 + 91 + 87 + 65) / 5 = 87.
The student with ID = 2 got scores 93, 97, 77, 100, and 76. Their top five average is (100 + 97 + 93 + 77 + 76) / 5 = 88.6, but with integer division their average converts to 88. 
```

<p><strong>Example 2:</strong></p>

```haskell
Input: items = [[1,100],[7,100],[1,100],[7,100],[1,100],[7,100],[1,100],[7,100],[1,100],[7,100]]
Output: [[1,100],[7,100]]
```

 

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= items.length &lt;= 1000</code></li>
	<li><code>items[i].length == 2</code></li>
	<li><code>1 &lt;= ID<sub>i</sub> &lt;= 1000</code></li>
	<li><code>0 &lt;= score<sub>i</sub> &lt;= 100</code></li>
	<li>For each <code>ID<sub>i</sub></code>, there will be <strong>at least</strong> five scores.</li>
</ul>

题意：给出一个不同学生的分数列表 <code>items</code>，其中 <code>items[i] = [ID<sub>i</sub>, score<sub>i</sub>]</code> 表示 <code>ID<sub>i</sub></code> 的学生的一科分数，计算每个学生&nbsp;<strong>最高的五科&nbsp;</strong>成绩的&nbsp;<strong>平均分</strong>。

<p>返回答案&nbsp;<code>result</code> 以数对数组形式给出<em>，</em>其中<em> </em><code>result[j] = [ID<sub>j</sub>, topFiveAverage<sub>j</sub>]</code><em> </em>表示<em> </em><code>ID<sub>j</sub></code><em> </em>的学生和他 <strong>最高的五科&nbsp;</strong>成绩的&nbsp;<strong>平均分</strong><em>。</em><code>result</code><em> </em>需要按<em> </em><code>ID<sub>j</sub></code><em>&nbsp; </em>递增的 <strong>顺序排列</strong> 。</p>

<p>学生 <strong>最高的五科&nbsp;</strong>成绩的&nbsp;<strong>平均分 </strong>的计算方法是将最高的五科分数相加，然后用 <strong>整数除法</strong> 除以 5 。</p>
 
