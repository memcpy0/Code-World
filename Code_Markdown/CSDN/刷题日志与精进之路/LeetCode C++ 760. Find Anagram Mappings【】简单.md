Given two lists <code>A</code> and <code>B</code>, and <code>B</code> is an anagram of <code>A</code>. <code>B</code> is an anagram of <code>A</code> means <code>B</code> is made by randomizing the order of the elements in <code>A</code>.
</p><p>
We want to find an <i>index mapping</i> <code>P</code>, from <code>A</code> to <code>B</code>. A mapping <code>P[i] = j</code> means the <code>i</code>th element in <code>A</code> appears in <code>B</code> at index <code>j</code>.
</p><p>
These lists <code>A</code> and <code>B</code> may contain duplicates.  If there are multiple answers, output any of them.
</p>

<p>
For example, given

```cpp
A = [12, 28, 46, 32, 50]
B = [50, 12, 32, 46, 28] 
```

 
We should return

```haskell
[1, 4, 3, 2, 0]
```

as <code>P[0] = 1</code> because the <code>0</code>th element of <code>A</code> appears at <code>B[1]</code>,
and <code>P[1] = 4</code> because the <code>1</code>st element of <code>A</code> appears at <code>B[4]</code>,
and so on.
<p></p>

<p><b>Note:</b></p><ol>
<li><code>A, B</code> have equal lengths in range <code>[1, 100]</code>.</li>
<li><code>A[i], B[i]</code> are integers in range <code>[0, 10^5]</code>.</li>
</ol><p></p>


题意：给定两个列表  `A, B`，并且 `B` 是 `A` 的变位（即 `B` 是由 `A` 中的元素随机排列后组成的新列表）。

我们希望找出一个从 `A` 到 `B` 的索引映射 `P` 。一个映射 `P[i] = j` 指的是列表 `A` 中的第 `i` 个元素出现于列表 `B` 中的第 `j` 个元素上。列表 `A` 和 `B` 可能出现重复元素。如果有多于一种答案，输出任意一种。
 
