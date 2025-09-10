> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>A <strong>sentence</strong> is a list of words that are separated by a single space with no leading or trailing spaces. Each of the words consists of <strong>only</strong> uppercase and lowercase English letters (no punctuation).</p>

<ul>
	<li>For example, <code>"Hello World"</code>, <code>"HELLO"</code>, and <code>"hello world hello world"</code> are all sentences.</li>
</ul>

<p>You are given a sentence <code>s</code>​​​​​​ and an integer <code>k</code>​​​​​​. You want to <strong>truncate</strong> <code>s</code>​​​​​​ such that it contains only the <strong>first</strong> <code>k</code>​​​​​​ words. Return <code>s</code>​​​​<em>​​ after <strong>truncating</strong> it.</em></p>


<p><strong>Example 1:</strong></p>

```java
Input: s = "Hello how are you Contestant", k = 4
Output: "Hello how are you"
Explanation:
The words in s are ["Hello", "how" "are", "you", "Contestant"].
The first 4 words are ["Hello", "how", "are", "you"].
Hence, you should return "Hello how are you".
```

<p><strong>Example 2:</strong></p>

```java
Input: s = "What is the solution to this problem", k = 4
Output: "What is the solution"
Explanation:
The words in s are ["What", "is" "the", "solution", "to", "this", "problem"].
The first 4 words are ["What", "is", "the", "solution"].
Hence, you should return "What is the solution".
```

<p><strong>Example 3:</strong></p> 

```java
Input: s = "chopper is not a tanuki", k = 5
Output: "chopper is not a tanuki"
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= s.length &lt;= 500</code></li>
	<li><code>k</code> is in the range <code>[1, the number of words in s]</code>.</li>
	<li><code>s</code> consist of only lowercase and uppercase English letters and spaces.</li>
	<li>The words in <code>s</code> are separated by a single space.</li>
	<li>There are no leading or trailing spaces.</li>
</ul>


题意：**句子** 是一个单词列表，列表中的单词之间用单个空格隔开，且不存在前导或尾随空格。每个单词仅由大小写英文字母组成（不含标点符号）。

给你一个句子 `s​​​​​`​ 和一个整数 `k`​​​​​​ ，请你将 `s`​​ **截断** ​，​​​使截断后的句子仅含 **前** `k`​​​​​​ 个单词。返回 **截断** `s`​​​​​​ 后得到的句子。

---
### 解法 字符串+模拟
根据题意模拟，一边拼接答案的同时，一边对空格进行计数即可。
```java
//Java version
class Solution {
    public String truncateSentence(String s, int k) {
        StringBuilder sb = new StringBuilder();
        int cnt = 0;
        for (int i = 0, n = s.length(); i < n && cnt < k; ++i) {
            char c = s.charAt(i);
            if (c == ' ') ++cnt;
            if (cnt < k) sb.append(c);
        }
        return sb.toString();
    }
}
//执行用时：1 ms, 在所有 Java 提交中击败了65.28% 的用户
//内存消耗：36.5 MB, 在所有 Java 提交中击败了80.68% 的用户
```
也可以使用下标 `i` 记录答案的结束位置：
```cpp
//C++ version
class Solution {
public:
    string truncateSentence(string s, int k) {
        int i = 0, n = s.size();
        while (i < n) {
            if (s[i] == ' ' && --k == 0) break;
            ++i;
        }
        return s.substr(0, i);
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：6.4 MB, 在所有 C++ 提交中击败了34.21% 的用户
```


1816. 截断句子
