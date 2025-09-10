> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


Given a List of words, return *the words that can be typed using letters of **alphabet** on only one row's of American keyboard like the image below*.
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/57e1d3e17a40464690f72aa13ff16f9b.png)
In the **American keyboard**:
- the first row consists of the characters `"qwertyuiop"`,
- the second row consists of the characters `"asdfghjkl"`, and
-  the third row consists of the characters `"zxcvbnm"`.

**Example 1:**
```cpp
Input: words = ["Hello","Alaska","Dad","Peace"]
Output: ["Alaska","Dad"]
```

 
**Example 2:**

```cpp
Input: words = ["omk"]
Output: []
```

**Example 3:**

```cpp
Input: words = ["adsdf","sfd"]
Output: ["adsdf","sfd"]
```

 

**Note:**
-  You may use one character in the keyboard more than once.
-  `1 <= words.length <= 20`
- `1 <= words[i].length <= 100`
- `words[i]` consists of English letters (both lowercase and uppercase). 
 

 
题意：给出一个字符串数组，返回那些使用「在键盘同一行的字母」能打印出来的单词。

---
### 解法1 哈希表
记录每个大写字母的行号，然后将字符串中的字符转换为大写，查询其所在行号，如果全部都在同一行，就加入到返回列表中。
```cpp
//C++ version
class Solution {
public:
    vector<string> findWords(vector<string>& words) {
        unordered_map<char, int> board = {
            {'Q', 1}, {'W', 1}, {'E', 1}, {'R', 1}, {'T', 1}, {'Y', 1}, {'U', 1}, {'I', 1}, {'O', 1}, {'P', 1},
            {'A', 2}, {'S', 2}, {'D', 2}, {'F', 2}, {'G', 2}, {'H', 2}, {'J', 2}, {'K', 2}, {'L', 2},
            {'Z', 3}, {'X', 3}, {'C', 3}, {'V', 3}, {'B', 3}, {'N', 3}, {'M', 3}
        };
        vector<string> ans;
        for (const string &word : words) {
            int p1 = board[toupper(word[0])];
            bool flag = true;
            for (int i = 1, n = word.size(); i < n; ++i) {
                int p2 = board[toupper(word[i])];
                if (p1 != p2) {
                    flag = false;
                    break;
                }
                p1 = p2;
            }
            if (flag) ans.push_back(word);
        }
        return ans;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：6.7 MB, 在所有 C++ 提交中击败了62.22% 的用户
```
---
### 解法2 常量数组
使用常量数组记录每个小写字母的行号，然后比对即可：

```cpp
//C++ version
static const int pos[26] = {2, 3, 3, 2, 1, 2, 2, 2, 1, 2, 2, 2, 3, 3, 1, 1, 1, 1, 2, 1, 1, 3, 1, 3, 1, 3};
class Solution {
public:
    vector<string> findWords(vector<string>& words) {
        vector<string> ans;
        for (const string &word : words) {
            bool flag = true;
            int p1 = pos[tolower(word[0]) - 'a'];
            for (int i = 1, n = word.size(); i < n; ++i) {
                int p2 = pos[tolower(word[i]) - 'a'];
                if (p1 != p2) {
                    flag = false;
                    break;
                }
                p1 = p2;
            }
            if (flag) ans.push_back(word);
        }
        return ans;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：6.4 MB, 在所有 C++ 提交中击败了96.81% 的用户
```

