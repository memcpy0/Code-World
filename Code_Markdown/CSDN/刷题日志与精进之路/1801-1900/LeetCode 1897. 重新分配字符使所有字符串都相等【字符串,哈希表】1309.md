> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个字符串数组 `words`（下标 **从 0 开始** 计数）。

在一步操作中，需先选出两个 **不同** 下标 `i` 和 `j`，其中 `words[i]` 是一个非空字符串，接着将 `words[i]` 中的 **任一** 字符移动到 `words[j]` 中的 **任一** 位置上。

如果执行任意步操作可以使 `words` 中的每个字符串都相等，返回 `true` ；否则，返回 `false` 。

**示例 1：**
```java
输入：words = ["abc","aabc","bc"]
输出：true
解释：将 `words[1] 中的第一个` 'a' 移动到 `words[2] 的最前面。 使` `words[1]` = "abc" 且 words[2] = "abc" 。
所有字符串都等于 "abc" ，所以返回 `true` 。
```
**示例 2：**
```java
输入：words = ["ab","a"]
输出：false
解释：执行操作无法使所有字符串都相等。
```
**提示：**
- `1 <= words.length <= 100`
- `1 <= words[i].length <= 100`
- `words[i]` 由小写英文字母组成

---
### 解法 哈希表
我们可以任意进行移动字符的操作。因此，假设 $words$ 的长度为 $n$，我们只需要**使每种字符的总出现次数能被 $n$ 整除**，即可以存在一种操作，使得操作后所有字符串均相等。

用 $\textit{cnt}$ 数组维护每种字符的频数。由于每个字符串 $words[i]$ 仅由小写英文字母组成，因此将 $\textit{cnt}$ 的长度设为对应字符集的大小 $\Sigma| = 26$ 。同时，$\textit{cnt}[k]$ 对应字典序第 $k$ 个字符的频数。

为了判断是否可行，我们遍历 $words$ 中的每个字符串统计每种字符的频数，并最终判断它们是否均可以被 $n$ 整除。
```cpp
// cpp
class Solution {
public:
    bool makeEqual(vector<string>& words) {
        int cnt[26] = {0}, n = words.size();
        for (const string& word : words)
            for (char c : word) ++cnt[c - 'a'];
        return all_of(cnt, cnt + 26, [n](int x) { return x % n == 0; });
    }
};
// python
class Solution:
    def makeEqual(self, words: List[str]) -> bool:
        cnt = [0] * 26 # 每种字符的频数
        n = len(words)
        for word in words:
            for c in word:
                cnt[ord(c) - ord('a')] += 1
        return all(k % n == 0 for k in cnt)
// go
func makeEqual(words []string) bool {
    n := len(words)
    cnt := [26]int{}
    for _, word := range words {
        for _, c := range word {
            cnt[c - 'a']++
        }
    }
    for _, v := range cnt {
        if v % n > 0 {
            return false
        }
    }
    return true
}
```
复杂度分析：
- 时间复杂度：$O(m + |\Sigma|)$ ，其中 $m$ 为 $\textit{words}$ 中所有字符串的长度总和，$\Sigma|$ 为字符集的大小，本题中即为所有小写英文字符的数量。初始化 $\textit{cnt}$ 数组的时间复杂度为 $O(|\Sigma|)$ ，遍历统计每个字符数量的时间复杂度为 $O(m)$ ，判断整除的时间复杂度为 $O(|\Sigma|)$ 。
- 空间复杂度：$O(|\Sigma|)$ ，即为 $cnt$ 数组的大小。