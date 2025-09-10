> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个字符串数组 `words1` 和 `words2`。

现在，如果 `b` 中的每个字母都出现在 `a` 中，**包括重复出现的字母**，那么称字符串 `b` 是字符串 `a` 的 **子集** 。

- 例如，`"wrr"` 是 `"warrior"` 的子集，但不是 `"world"` 的子集。

如果对 `words2` 中的每一个单词 `b`，`b` 都是 `a` 的子集，那么我们称 `words1` 中的单词 `a` 是 **通用单词** 。

以数组形式返回 `words1` 中所有的通用单词。你可以按 **任意顺序** 返回答案。

**示例 1：**

**输入：**words1 = ["amazon","apple","facebook","google","leetcode"], words2 = ["e","o"]
**输出：**["facebook","google","leetcode"]

**示例 2：**

**输入：**words1 = ["amazon","apple","facebook","google","leetcode"], words2 = ["l","e"]
**输出：**["apple","google","leetcode"]

**示例 3：**

**输入：**words1 = ["amazon","apple","facebook","google","leetcode"], words2 = ["e","oo"]
**输出：**["facebook","google"]

**示例 4：**

**输入：**words1 = ["amazon","apple","facebook","google","leetcode"], words2 = ["lo","eo"]
**输出：**["google","leetcode"]

**示例 5：**

**输入：**words1 = ["amazon","apple","facebook","google","leetcode"], words2 = ["ec","oc","ceo"]
**输出：**["facebook","leetcode"]

**提示：**

- `1 <= words1.length, words2.length <= 10^4`
- `1 <= words1[i].length, words2[i].length <= 10`
- `words1[i]` 和 `words2[i]` 仅由小写英文字母组成
- `words1` 中的所有字符串 **互不相同**

```cpp
class Solution {
public:
    vector<string> wordSubsets(vector<string>& words1, vector<string>& words2) {
        int cnt[26]; memset(cnt, 0, sizeof(cnt));
        for (string &w : words2) {
            int tcnt[26]; memset(tcnt, 0, sizeof(tcnt));
            for (char &c : w) ++tcnt[c - 'a'];
            for (int i = 0; i < 26; ++i) cnt[i] = max(cnt[i], tcnt[i]);
        }
        vector<string> ans;
        for (string &w : words1) { 
            int tcnt[26]; memset(tcnt, 0, sizeof(tcnt));
            for (char &c : w) ++tcnt[c - 'a'];
            bool f = true;
            for (int i = 0; i < 26; ++i) {
                if (tcnt[i] < cnt[i]) {
                    f = false;
                    break;
                }
            }
            if (f) ans.push_back(w);
        }
        return ans;
    }
};
```