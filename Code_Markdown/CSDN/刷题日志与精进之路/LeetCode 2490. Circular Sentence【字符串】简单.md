> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

**句子** 是由单个空格分隔的一组单词，且不含前导或尾随空格。
- 例如，`"Hello World"`、`"HELLO"`、`"hello world hello world"` 都是符合要求的句子。

单词 **仅** 由大写和小写英文字母组成。且大写和小写字母会视作不同字符。

如果句子满足下述全部条件，则认为它是一个 **回环句** ：
- 单词的最后一个字符和下一个单词的第一个字符相等。
- 最后一个单词的最后一个字符和第一个单词的第一个字符相等。

例如，`"leetcode exercises sound delightful"`、`"eetcode"`、`"leetcode eats soul"` 都是回环句。然而，`"Leetcode is cool"`、`"happy Leetcode"`、`"Leetcode"` 和 `"I like Leetcode"` 都 **不** 是回环句。

给你一个字符串 `sentence` ，请你判断它是不是一个回环句。如果是，返回 `true` ；否则，返回 `false` 。

<strong>示例 1：</strong>
<pre><strong>输入：</strong>sentence = "leetcode exercises sound delightful"
<strong>输出：</strong>true
<strong>解释：</strong>句子中的单词是 ["leetcode", "exercises", "sound", "delightful"] 。
- leetcod<strong><em>e</em></strong> 的最后一个字符和 <strong><em>e</em></strong>xercises 的第一个字符相等。
- exercise<em><strong>s</strong></em> 的最后一个字符和 <em><strong>s</strong></em>ound 的第一个字符相等。
- <em><strong>s</strong></em>ound 的最后一个字符和 delightfu<em><strong>l</strong></em> 的第一个字符相等。
- delightfu<em><strong>l</strong></em> 的最后一个字符和 <em><strong>l</strong></em>eetcode 的第一个字符相等。
这个句子是回环句。</pre>

<strong>示例 2：</strong>
<pre><strong>输入：</strong>sentence = "eetcode"
<strong>输出：</strong>true
<strong>解释：</strong>句子中的单词是 ["eetcode"] 。
- eetcod<em><strong>e</strong></em> 的最后一个字符和 <em><strong>e</strong></em>etcod<em>e</em> 的第一个字符相等。
这个句子是回环句。</pre>

<strong>示例 3：</strong> 
<pre><strong>输入：</strong>sentence = "Leetcode is cool"
<strong>输出：</strong>false
<strong>解释：</strong>句子中的单词是 ["Leetcode", "is", "cool"] 。
- Leetcod<em><strong>e</strong></em>&nbsp;的最后一个字符和 <em><strong>i</strong></em>s 的第一个字符 <strong>不</strong> 相等。 
这个句子 <strong>不</strong> 是回环句。</pre>

**提示：**
- `1 <= sentence.length <= 500`
- `sentence` 仅由大小写英文字母和空格组成
- `sentence` 中的单词由单个空格进行分隔
- 不含任何前导或尾随空格

---
### 解法1 分词
将「给出的不含前导或尾随空格的句子」按照空格分隔，然后检查是否是回环句。注意：**只有一个单词时，该单词的首尾字符要相同**，比如 `"eetcode"` 是回环句。
```java
class Solution {
    public boolean isCircularSentence(String sentence) {
        String[] words = sentence.split(" ");
        int n = words.length;
        int m = words[n - 1].length();
        if (words[0].charAt(0) != words[n - 1].charAt(m - 1)) return false;
        for (int i = 1; i < n; ++i) {
            m = words[i - 1].length();
            if (words[i].charAt(0) != words[i - 1].charAt(m - 1)) return false;
        } 
        return true;
    }
}
```
上述做法为了存储分隔后的单词列表，需要 $O(n)$ 空间。

---
### 解法2 O(1)空间
我们将回环句的判断条件，转换成判断：
- **句子首尾字符**是否相同。
- **每个空格左右的字符**是否相同。

```cpp
class Solution {
public:
    bool isCircularSentence(string s) {
        if (s[0] != s.back()) return false;
        for (int i = 1, n = s.size(); i < n - 1; ++i) 
            if (s[i] == ' ' && s[i - 1] != s[i + 1]) return false;
        return true;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为 $sentence$ 的长度。
- 空间复杂度：$O(1)$ ，仅用到若干额外变量。