> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

有时候人们会用重复写一些字母来表示额外的感受，比如 `"hello" -> "heeellooo"`, `"hi" -> "hiii"`。我们将相邻字母都相同的一串字符定义为相同字母组，例如："h", "eee", "ll", "ooo"。

对于一个给定的字符串 S ，如果另一个单词能够通过将一些字母组扩张从而使其和 S 相同，我们将这个单词定义为可扩张的（stretchy）。扩张操作定义如下：选择一个字母组（包含字母 `c` ），然后往其中添加相同的字母 `c` 使其长度达到 3 或以上。

例如，以 "hello" 为例，我们可以对字母组 "o" 扩张得到 "hellooo"，但是无法以同样的方法得到 "helloo" 因为字母组 "oo" 长度小于 3。此外，我们可以进行另一种扩张 "ll" -> "lllll" 以获得 "helllllooo"。如果 `s = "helllllooo"`，那么查询词 "hello" 是可扩张的，因为可以对它执行这两种扩张操作使得 `query = "hello" -> "hellooo" -> "helllllooo" = s`。

输入一组查询单词，输出其中可扩张的单词数量。

**示例：**

**输入：** 
s = "heeellooo"
words = ["hello", "hi", "helo"]
**输出：**1
**解释**：
我们能通过扩张 "hello" 的 "e" 和 "o" 来得到 "heeellooo"。
我们不能通过扩张 "helo" 来得到 "heeellooo" 因为 "ll" 的长度小于 3 。

**提示：**

- `1 <= s.length, words.length <= 100`
- `1 <= words[i].length <= 100`
- s 和所有在 `words` 中的单词都只由小写字母组成。

```cpp
class Solution {
private:
    typedef pair<char, int> pci;
    vector<pci> proc(string &s) {
        vector<pci> p;
        int pre, i, n;
        for (pre = 0, i = 1, n = s.size(); i < n; ++i) {
            if (s[i] != s[i - 1]) { // s[pre:i-1]为一组
                p.push_back(pci(s[i - 1], i - pre));
                pre = i;
            }
        }
        p.push_back(pci(s.back(), i - pre));
        return p;
    }
public:
    int expressiveWords(string s, vector<string>& words) {
        vector<pci> p = proc(s);
        int ans = 0;
        for (string &word : words) {
            vector<pci> tp = proc(word);
            if (p.size() != tp.size()) continue;
            bool flag = true;
            for (int i = 0, m = p.size(); i < m; ++i) {
                if (p[i].first != tp[i].first ||
                    (p[i].second < 3 && p[i].second != tp[i].second) ||
                    (p[i].second < tp[i].second)
                ) {
                    flag = false; break;
                }
            }
            if (flag) ++ans;
        }
        return ans;
    }
};

/*
int expressiveWords(char* S, char** words, int wordsSize) {
    int result = 0;
    for(int i = 0; i < wordsSize; i++)//比较words[i] 和 s
    {
        int a = 0, b = 0, yes = 1;
        while(words[i][a] != '\0' && S[b]!= '\0')
        {
            if(words[i][a] != S[b]) {yes = 0; break;}
            else 
            {
                int count_a = 0, count_b = 0;
                char ca = words[i][a], cb = S[b];
                while(words[i][a] == ca) {count_a++; a++;}
                while(S[b] == cb) {count_b++; b++;}
                if(count_a == count_b) continue;
                else if(count_b < count_a || count_b < 3) {yes = 0; break;}
            }
        }
        if (words[i][a] != '\0' ||  S[b]!= '\0') yes = 0;
        if(yes) result++;
    }
    return result;
}

*/
```