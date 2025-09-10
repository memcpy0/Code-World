> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>Given two version numbers,&nbsp;<code>version1</code> and <code>version2</code>, compare them.</p>
 
<p>Version numbers consist of <strong>one or more revisions</strong> joined by a dot&nbsp;<code>'.'</code>. Each revision&nbsp;consists of <strong>digits</strong>&nbsp;and may contain leading <strong>zeros</strong>. Every revision contains <strong>at least one character</strong>. Revisions are <strong>0-indexed from left to right</strong>, with the leftmost revision being revision 0, the next revision being revision 1, and so on. For example&nbsp;<code>2.5.33</code>&nbsp;and&nbsp;<code>0.1</code>&nbsp;are valid version numbers.</p>

<p>To compare version numbers, compare their revisions in <strong>left-to-right order</strong>. Revisions are compared using their&nbsp;<strong>integer value ignoring any leading zeros</strong>. This means that revisions&nbsp;<code>1</code>&nbsp;and&nbsp;<code>001</code>&nbsp;are considered&nbsp;<strong>equal</strong>. If a version number does not specify a revision at an index, then&nbsp;<strong>treat the revision as&nbsp;<code>0</code></strong>. For example, version&nbsp;<code>1.0</code> is less than version&nbsp;<code>1.1</code>&nbsp;because their revision 0s are the same, but their revision 1s are&nbsp;<code>0</code>&nbsp;and&nbsp;<code>1</code>&nbsp;respectively, and&nbsp;<code>0 &lt; 1</code>.</p>

<p><em>Return the following:</em></p>

<ul>
	<li>If <code>version1 &lt; version2</code>, return <code>-1</code>.</li>
	<li>If <code>version1 &gt; version2</code>, return <code>1</code>.</li>
	<li>Otherwise, return <code>0</code>.</li>
</ul>
 
<p><strong>Example 1:</strong></p>

```cpp
Input: version1 = "1.01", version2 = "1.001"
Output: 0
Explanation: Ignoring leading zeroes, both "01" and "001" represent the same integer "1".
```
 
<p><strong>Example 2:</strong></p>

```cpp
Input: version1 = "1.0", version2 = "1.0.0"
Output: 0
Explanation: version1 does not specify revision 2, which means it is treated as "0".
```
<p><strong>Example 3:</strong></p>

```cpp
Input: version1 = "0.1", version2 = "1.1"
Output: -1
Explanation: version1's revision 0 is "0", while version2's revision 0 is "1". 0 < 1, so version1 < version2. 
```
<p><strong>Example 4:</strong></p>

```cpp
Input: version1 = "1.0.1", version2 = "1"
Output: 1
```

<p><strong>Example 5:</strong></p>

```cpp
Input: version1 = "7.5.2.4", version2 = "7.5.3"
Output: -1
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= version1.length, version2.length &lt;= 500</code></li>
	<li><code>version1</code> and <code>version2</code>&nbsp;only contain digits and <code>'.'</code>.</li>
	<li><code>version1</code> and <code>version2</code>&nbsp;<strong>are valid version numbers</strong>.</li>
	<li>All the given revisions in&nbsp;<code>version1</code> and <code>version2</code>&nbsp;can be stored in&nbsp;a&nbsp;<strong>32-bit integer</strong>.</li>
</ul>


题意：版本号由一个或多个修订号组成，各修订号由一个 <code>'.'</code> 连接。每个修订号由 <strong>多位数字</strong> 组成，可能包含 <strong>前导零</strong> 。每个版本号至少包含一个字符。修订号从左到右编号，下标从 0 开始，最左边的修订号下标为 0 ，下一个修订号下标为 1 ，以此类推。例如，<code>2.5.33</code> 和 <code>0.1</code> 都是有效的版本号。</p>

<p>比较版本号时，请按从左到右的顺序依次比较它们的修订号。比较修订号时，只需比较 <strong>忽略任何前导零后的整数值</strong> 。也就是说，修订号 <code>1</code> 和修订号 <code>001</code> <strong>相等 </strong>。如果版本号没有指定某个下标处的修订号，则该修订号视为 <code>0</code> 。例如，版本 <code>1.0</code> 小于版本 <code>1.1</code> ，因为它们下标为 <code>0</code> 的修订号相同，而下标为 <code>1</code> 的修订号分别为 <code>0</code> 和 <code>1</code> ，<code>0 &lt; 1</code> 。</p>

<p>给你两个版本号 <code>version1</code> 和 <code>version2</code> ，请你比较它们。如果 <code><em>version1 </em> > <em>version2</em></code> 返回 <code>1</code>，如果 <code><em>version1 </em>< <em>version2</em></code> 返回 <code>-1</code>，除此之外返回 <code>0</code>。
 

---
### 解法 字符串+模拟
对字符串按照 `.` 进行分割，然后逐个比较修订号大小即可，缺省时用 `0` 代替。算法的时间复杂度为 $O(n + m)$ ，空间复杂度为 $O(n + m)$ ： 
```cpp
//C++ version
class Solution {
private:
    vector<string> split(const string& s, char delim = ' ') {
        vector<string> tokens;
        if (s.empty()) return tokens;
        size_t beginPos = s.find_first_not_of(delim);
        size_t endPos = s.find_first_of(delim, beginPos);
        while (beginPos != string::npos) {
            tokens.push_back(s.substr(beginPos, endPos));
            beginPos = s.find_first_not_of(delim, endPos);
            endPos = s.find_first_of(delim, beginPos);
        }
        return tokens;
    }
public:
    int compareVersion(string version1, string version2) {
        vector<string> &&v1 = split(version1, '.'), &&v2 = split(version2, '.');
        for (int i = 0, j = 0, n = v1.size(), m = v2.size(); i < n || j < m; ++i, ++j) {
            int revis1 = i < n ? stoi(v1[i]) : 0;
            int revis2 = j < m ? stoi(v2[j]) : 0;
            if (revis1 != revis2) return revis1 < revis2 ? -1 : 1;
        }
        return 0;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：6.4 MB, 在所有 C++ 提交中击败了5.05% 的用户
```
```java
class Solution {
    public int compareVersion(String version1, String version2) {
        String[] a = version1.split("\\."), b = version2.split("\\.");
        for (int i = 0, j = 0, n = a.length, m = b.length; i < n || j < m; ++i, ++j) {
            int an = (i < a.length) ? Integer.parseInt(a[i]) : 0;
            int bn = (i < b.length) ? Integer.parseInt(b[i]) : 0;
            if (an != bn) return an < bn ? -1 : 1;
        }
        return 0;  
    }
}
//执行用时：1 ms, 在所有 Java 提交中击败了79.74% 的用户
//内存消耗：36.5 MB, 在所有 Java 提交中击败了45.55% 的用户
```
