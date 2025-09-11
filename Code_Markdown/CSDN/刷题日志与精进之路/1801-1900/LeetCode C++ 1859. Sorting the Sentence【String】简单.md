<p>A <strong>sentence</strong> is a list of words that are separated by a single space with no leading or trailing spaces. Each word consists of lowercase and uppercase English letters.</p>

<p>A sentence can be <strong>shuffled</strong> by appending the <strong>1-indexed word position</strong> to each word then rearranging the words in the sentence.</p>

<ul>
	<li>For example, the sentence <code>"This is a sentence"</code> can be shuffled as <code>"sentence4 a3 is2 This1"</code> or <code>"is2 sentence4 This1 a3"</code>.</li>
</ul>

<p>Given a <strong>shuffled sentence</strong> <code>s</code> containing no more than <code>9</code> words, reconstruct and return <em>the original sentence</em>.</p>
 
<p><strong>Example 1:</strong></p>

```clike
Input: s = "is2 sentence4 This1 a3"
Output: "This is a sentence"
Explanation: Sort the words in s to their original positions "This1 is2 a3 sentence4", then remove the numbers.
```
 
<p><strong>Example 2:</strong></p>

```clike
Input: s = "is2 sentence4 This1 a3"
Output: "This is a sentence"
Explanation: Sort the words in s to their original positions "This1 is2 a3 sentence4", then remove the numbers.
```

 
 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>2 &lt;= s.length &lt;= 200</code></li>
	<li><code>s</code> consists of lowercase and uppercase English letters, spaces, and digits from <code>1</code> to <code>9</code>.</li>
	<li>The number of words in <code>s</code> is between <code>1</code> and <code>9</code>.</li>
	<li>The words in <code>s</code> are separated by a single space.</li>
	<li><code>s</code> contains no leading or trailing spaces.</li>
</ul>


题意：一个 <strong>句子</strong>&nbsp;指的是一个序列的单词用单个空格连接起来，且开头和结尾没有任何空格。每个单词都只包含小写或大写英文字母。

<p>我们可以给一个句子添加 <strong>从 1 开始的单词位置索引 </strong>，并且将句子中所有单词&nbsp;<strong>打乱顺序</strong>&nbsp;。比方说，句子&nbsp;<code>"This is a sentence"</code>&nbsp;可以被打乱顺序得到&nbsp;<code>"sentence4 a3 is2 This1"</code>&nbsp;或者&nbsp;<code>"is2 sentence4 This1 a3"</code>&nbsp;。 

<p>给你一个 <strong>打乱顺序</strong>&nbsp;的句子&nbsp;<code>s</code>&nbsp;，它包含的单词不超过&nbsp;<code>9</code>&nbsp;个，请你重新构造并得到原本顺序的句子。</p>


---
### 解法 STL的 `stringstream`
可以用 `stringstream`  拆分**空白符分隔**的字符串，也可以自己写一个 `split` 函数。然后把单词尾部的数字当做下标用，放到桶中。最后拼接成原来的字符串：
```clike
class Solution {
public:
    string sortSentence(string s) {
        stringstream ss(s);
        string temp;
        vector<string> words(9);
        int n = 0;
        while (ss >> temp) {
            int k = temp.back() - '1';
            if (k > n) n = k;
            words[k] = temp.substr(0, temp.size() - 1);
        }
        temp.clear();
        for (int i = 0; i <= n; ++i) temp += (i ? " " : "") + words[i];
        return temp;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了30.77% 的用户
内存消耗：6.1 MB, 在所有 C++ 提交中击败了64.89% 的用户
```
