> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given a list of `words`, list of  single `letters` (might be repeating) and `score` of every character.

Return the maximum score of **any** valid set of words formed by using the given letters (`words[i]` cannot be used two or more times).

It is not necessary to use all characters in `letters` and each letter can only be used once. Score of letters `'a'`, `'b'`, `'c'`, ... ,`'z'` is given by `score[0]`, `score[1]`, ... , `score[25]` respectively.
<p><strong class="example">Example 1:</strong></p>
<pre><strong>Input:</strong> words = ["dog","cat","dad","good"], letters = ["a","a","c","d","d","d","g","o","o"], score = [1,0,9,5,0,0,3,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0]
<strong>Output:</strong> 23
<strong>Explanation:</strong>
Score  a=1, c=9, d=5, g=3, o=2
Given letters, we can form the words "dad" (5+1+5) and "good" (3+2+2+5) with a score of 23.
Words "dad" and "dog" only get a score of 21.</pre>
<p><strong class="example">Example 2:</strong></p>
<pre><strong>Input:</strong> words = ["xxxz","ax","bx","cx"], letters = ["z","a","b","c","x","x","x"], score = [4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,10]
<strong>Output:</strong> 27
<strong>Explanation:</strong>
Score  a=4, b=4, c=4, x=5, z=10
Given letters, we can form the words "ax" (4+5), "bx" (4+5) and "cx" (4+5) with a score of 27.
Word "xxxz" only get a score of 25.</pre>
<p><strong class="example">Example 3:</strong></p>
<pre><strong>Input:</strong> words = ["leetcode"], letters = ["l","e","t","c","o","d"], score = [0,0,1,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0]
<strong>Output:</strong> 0
<strong>Explanation:</strong>
Letter "e" can only be used once.</pre>

<strong>Constraints:</strong>
<ul>
	<li><code>1 &lt;= words.length &lt;= 14</code></li>
	<li><code>1 &lt;= words[i].length &lt;= 15</code></li>
	<li><code>1 &lt;= letters.length &lt;= 100</code></li>
	<li><code>letters[i].length == 1</code></li>
	<li><code>score.length ==&nbsp;26</code></li>
	<li><code>0 &lt;= score[i] &lt;= 10</code></li>
	<li><code>words[i]</code>, <code>letters[i]</code>&nbsp;contains only lower case English letters.</li>
</ul>
题意：给定一个单词数组 `words` ，一个字母数组 `letters`（字母可能重复），都是小写字母，然后是每个字母的分数组成的数组 `score` 。返回使用给定字母（来自 `letters` ，同个字母有多少个就能用多少次）组成的任何有效单词集（单词来自 `words` ，其中每个单词只能组成一次）的最大得分。具体做法看Example就明白了。

---
### 解法1 指数枚举+DFS+回溯
首先使用哈希表记录每个字母出现的次数，这也是我们能使用的字母次数。然后遍历 `words` ，将其中有效的单词（可以被 `letters` 中的字符组成）挑出来、得到一个有效单词数组，同时计算每个有效单词的分数、并记录下来。最后是DFS，指数枚举（要么选择一个有效单词，然后以此DFS，**回溯时记得恢复状态**；要么不选择，然后DFS），不断更新最大得分。
```cpp
class Solution {
private:
    int maxScore = INT_MIN;
    unordered_map<string, int> wordToScore;
    vector<string> valids;
    void dfs(int i, unordered_map<char, int> ltt, int tscore) {
        if (i >= valids.size()) {
            if (tscore > maxScore) maxScore = tscore;
            return;
        }
        unordered_map<char, int> temp;        
        for (const char &c : valids[i]) ++temp[c];  
        bool flag = true;      
        for (auto &it : temp) {
            if (it.second > ltt[it.first]) {
                flag = false;
                break;
            }
        }
        if (flag) {
            for (auto &it : temp) ltt[it.first] -= it.second;
            dfs(i + 1, ltt, tscore + wordToScore[valids[i]]);
            for (auto &it : temp) ltt[it.first] += it.second;
        }
        dfs(i + 1, ltt, tscore);
    }
public:
    int maxScoreWords(vector<string>& words, vector<char>& letters, vector<int>& score) {
        unordered_map<char, int> letterToTimes;
        for (const char &c : letters) ++letterToTimes[c];
        for (const string& word : words) {
            unordered_map<char, int> temp;
            int wscore = 0;
            bool flag = true;
            for (const char &c : word) {
                ++temp[c];    
                wscore += score[c - 'a'];
                if (temp[c] > letterToTimes[c]) {
                    flag = false;
                    break;
                }
            }  
            if (flag) {
                valids.push_back(word);
                wordToScore[word] = wscore;
            }
        }
        dfs(0, letterToTimes, 0);
        return maxScore;
    }
};
```

---
### 解法2 状态压缩+二进制枚举
因为单词数目不超过 $14$ ，因此可以使用状态压缩的方式来枚举所有的单词子集。**使用整数 $s$ 表示单词子集，$s$ 的第 $k$ 位为 $1$ 代表单词子集 $s$ 包含单词 $\textit{words}[k]$** ，$s$ 的第 $k$ 位为 $0$ 代表单词子集 $s$ 不包含单词 $\textit{words}[k]$ 。

使用 $\textit{count}$ 保存字母表 $\textit{letters}$ 中各种字母的数目，**对二进制枚举出的每个单词子集，分别使用 $\textit{wordCount}$ 保存单词子集 $s$ 中所有单词的各种字母的数目**。如果 $\textit{wordCount}$ 中的任一字母的数目都小于等于 $\textit{count}$ 中对应字母的数目，那么说明单词子集的单词可以由字母表 $\textit{letters}$ 拼写，计算单词子集的分数，最终结果就是这些分数中的最大值。
```cpp
class Solution {
public:
    int maxScoreWords(vector<string>& words, vector<char>& letters, vector<int>& score) {
        int n = words.size(), ans = 0, tot = 1 << n;
        vector<int> count(26);
        for (auto c : letters) ++count[c - 'a'];
        for (int s = 1; s < tot; ++s) {
            vector<int> wordCount(26); // 统计子集s所有单词的字母数目
            for (int k = 0; k < n; ++k) {
                if ((s & (1 << k)) == 0) continue; // words[k]不在子集s中
                for (auto c : words[k]) ++wordCount[c - 'a'];
            }
            bool ok = true; // 判断子集s是否valid
            int sum = 0; // 保存子集s的得分
            for (int i = 0; i < 26; ++i) {
                sum += score[i] * wordCount[i];
                ok = ok && (wordCount[i] <= count[i]);
            }
            if (ok) ans = max(ans, sum);
        }
        return ans;
    }
};
```