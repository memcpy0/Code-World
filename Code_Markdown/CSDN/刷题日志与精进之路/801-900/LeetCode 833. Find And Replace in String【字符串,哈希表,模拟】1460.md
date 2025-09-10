> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

你会得到一个字符串 `s` (索引从 0 开始)，你必须对它执行 `k` 个替换操作。替换操作以三个长度均为 `k` 的并行数组给出：`indices`, `sources`,  `targets`。

要完成第 `i` 个替换操作:
1. 检查 **子字符串**  `sources[i]` 是否出现在 **原字符串** `s` 的索引 `indices[i]` 处。
2. 如果没有出现， **什么也不做** 。
3. 如果出现，则用 `targets[i]` **替换** 该子字符串。

例如，如果 `s = "abcd"` ， `indices[i] = 0` , `sources[i] = "ab"`， `targets[i] = "eee"` ，那么替换的结果将是 `"eeecd"` 。

所有替换操作必须 **同时** 发生，这意味着替换操作不应该影响彼此的索引。测试用例保证元素间**不会重叠** 。
- 例如，一个 `s = "abc"` ，  `indices = [0,1]` ， `sources = ["ab"，"bc"]` 的测试用例将不会生成，因为 `"ab"` 和 `"bc"` 替换重叠。

_在对 `s` 执行所有替换操作后返回 **结果字符串** 。_

**子字符串** 是字符串中连续的字符序列。

**示例 1：**
![](https://assets.leetcode.com/uploads/2021/06/12/833-ex1.png)
```cpp
输入：s = "abcd", indices = [0,2], sources = ["a","cd"], targets = ["eee","ffff"]
输出："eeebffff"
解释：
"a" 从 s 中的索引 0 开始，所以它被替换为 "eee"。
"cd" 从 s 中的索引 2 开始，所以它被替换为 "ffff"。
```
**示例 2：**
![](https://assets.leetcode.com/uploads/2021/06/12/833-ex2-1.png)

```java
输入：s = "abcd", indices = [0,2], sources = ["ab","ec"], targets = ["eee","ffff"]
输出："eeecd"
解释：
"ab" 从 s 中的索引 0 开始，所以它被替换为 "eee"。
"ec" 没有从原始的 S 中的索引 2 开始，所以它没有被替换。
```
**提示：**
- `1 <= s.length <= 1000`
- `k == indices.length == sources.length == targets.length`
- `1 <= k <= 100`
- `0 <= indices[i] < s.length`
- `1 <= sources[i].length, targets[i].length <= 50`
- `s` 仅由小写英文字母组成
- `sources[i]` 和 `targets[i]` 仅由小写英文字母组成

---
### 解法 字符串+哈希表+模拟
设 $s$ 长度为 $n$ ，创建一个长为 $n$ 的 $matchIndex$ 列表，初始化每个元素为 $-1$ 。

遍历每个替换操作。对于第 $i$ 个替换操作，如果从 $\textit{indices}[i]$ 开始的字符串有前缀 $\textit{sources}[i]$ ，则可以替换成 $\textit{target}[i]$ 。例如 `s="abcd"` ，`s[1:]="bcd"` 有前缀 `"bc"` 。此时记录 $matchIndex[indices[i]]=i$ ，后面的 $i$ 指的是 $target[i]$ ，表示「从原串的 $indices[i]$ 位置开始要进行替换，替换后从 $sources[i]$ 变为 $targets[i]$ 」。

然后遍历 $matchIndex$ 列表，如果 $matchIndex[i] \ne -1$ ，说明要进行替换，把 $targets[matchIndex[i]]$ 加入答案，然后 $i$ 增加 $sources[matchIndex[i]]$ 的长度；否则说明无需替换，把 $s[i]$ 加入答案，然后 $i$ 加一。
```cpp
class Solution {
public:
    string findReplaceString(string s, vector<int>& indices, vector<string>& sources, vector<string>& targets) {
        string ans;
        int k = indices.size(), n = s.size();
        int matchIndex[n];
        memset(matchIndex, -1, sizeof(matchIndex));

        for (int i = 0; i < k; ++i) {
            int sn = sources[i].size();
            bool isMatch = true;
            for (int j = indices[i]; j < indices[i] + sn; ++j) { // j为原串中的下标
                if (sources[i][j - indices[i]] != s[j]) { // 某个字符不同
                    isMatch = false;
                    break;
                }
            } // 如果子串出现在原串的indices[i]处，则记录要用来替换的新串的下标
            if (isMatch) matchIndex[indices[i]] = i;
        }
        for (int i = 0; i < n; ++i) {
            if (matchIndex[i] != -1) { // 要进行替换
                int index = matchIndex[i];
                ans += targets[index];
                i = indices[index] + sources[index].size() - 1; // i要跳转到原串后面
            } else ans.push_back(s[i]);
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(M+N)$ 
- 空间复杂度：$O(M+N)$