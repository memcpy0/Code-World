<p>Given a paragraph&nbsp;and a list of banned words, return the most frequent word that is not in the list of banned words.&nbsp; It is guaranteed there is at least one word that isn't banned, and that the answer is unique.</p>

<p>Words in the list of banned words are given in lowercase, and free of punctuation.&nbsp; Words in the paragraph are not case sensitive.&nbsp; The answer is in lowercase.</p>
 
<p><strong>Example:</strong></p>

```haskell
Input: 
paragraph = "Bob hit a ball, the hit BALL flew far after it was hit."
banned = ["hit"]
Output: "ball"
Explanation: 
"hit" occurs 3 times, but it is a banned word.
"ball" occurs twice (and no other word does), so it is the most frequent non-banned word in the paragraph. 
Note that words in the paragraph are not case sensitive,
that punctuation is ignored (even if adjacent to words, such as "ball,"), 
and that "hit" isn't the answer even though it occurs more because it is banned.
```

 
<p><strong>Note: </strong></p>

<ul>
	<li><code>1 &lt;= paragraph.length &lt;= 1000</code>.</li>
	<li><code>0 &lt;= banned.length &lt;= 100</code>.</li>
	<li><code>1 &lt;= banned[i].length &lt;= 10</code>.</li>
	<li>The answer is unique, and written in lowercase (even if its occurrences in <code>paragraph</code>&nbsp;may have&nbsp;uppercase symbols, and even if it is a proper noun.)</li>
	<li><code>paragraph</code> only consists of letters, spaces, or the punctuation symbols <code>!?',;.</code></li>
	<li>There are no hyphens or hyphenated words.</li>
	<li>Words only consist of letters, never apostrophes or other punctuation symbols.</li>
</ul>

题意：给定一个段落 (`paragraph`) 和一个禁用单词列表 (`banned`)。返回出现次数最多，同时不在禁用列表中的单词。题目保证至少有一个词不在禁用列表中，而且答案唯一。禁用列表中的单词用小写字母表示，不含标点符号。段落中的单词不区分大小写。**答案都是小写字母**。  

---
### 解法 哈希集合、哈希映射
使用**哈希集合**存储禁用词集，使用**哈希映射**存储非禁用词及其出现次数，注意将单词的所有字符都转为小写。最终求出出现次数最多的非禁用词：
```cpp
class Solution {
private:
    vector<string> lowerAndSplit(const string &s, string delim = "!?',;. ") {
        vector<string> ans;
        size_t beginPos = s.find_first_not_of(delim);
        size_t endPos = s.find_first_of(delim, beginPos);
        while (beginPos != string::npos) {
            string &&temp = s.substr(beginPos, endPos - beginPos);
            for (char &c : temp) c = tolower(c);
            ans.push_back(temp);
            beginPos = s.find_first_not_of(delim, endPos);
            endPos = s.find_first_of(delim, beginPos);
        }
        return ans;
    }
public:
    string mostCommonWord(string paragraph, vector<string>& banned) {
        unordered_set<string> forbidden(banned.begin(), banned.end()); //禁用词集合
        unordered_map<string, int> rec;
        vector<string> &&tokens = lowerAndSplit(paragraph);
        int n = tokens.size(), mx = 0;
        string ans;
        for (int i = 0; i < n; ++i) if (forbidden.find(tokens[i]) == forbidden.end()) ++rec[tokens[i]];
        for (const auto it : rec) {
            if (it.second > mx) {
                mx = it.second;
                ans = it.first;
            }
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了85.47% 的用户
内存消耗：9.4 MB, 在所有 C++ 提交中击败了7.67% 的用户
```
