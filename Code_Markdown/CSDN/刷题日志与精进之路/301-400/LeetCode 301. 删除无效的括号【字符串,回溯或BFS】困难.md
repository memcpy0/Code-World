> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个由若干括号和字母组成的字符串 `s` ，删除最小数量的无效括号，使得输入的字符串有效。

返回所有可能的结果。答案可以按 **任意顺序** 返回。

**示例 1：**
```js
输入：s = "()())()"
输出：["(())()","()()()"]
```
**示例 2：**
```js
输入：s = "(a)())()"
输出：["(a())()","(a)()()"]
```
**示例 3：**
```js
输入：s = ")("
输出：[""]
```
**提示：**
- `1 <= s.length <= 25`
- `s` 由小写英文字母以及括号 `'('` 和 `')'` 组成
- `s` 中至多含 `20` 个括号

---
有效的「括号」：题目输入的字符串由一系列「左括号」和「右括号」组成，但是有一些额外的括号，使得括号不能正确配对。对于括号配对规则如果还不太清楚的读者，可以先完成问题「[20. 有效的括号](https://leetcode-cn.com/problems/valid-parentheses/)」。

可以一次遍历计算出多余的「左括号」和「右括号」： 根据括号匹配规则和根据求解「[22. 括号生成]」的**经验**，我们知道：==如果当前遍历到的「左括号」的数目严格小于「右括号」的数目则表达式无效==。因此，我们可以遍历一次输入字符串，统计「左括号」和「右括号」出现的次数。
- 当遍历到「左括号」的时候：「左括号」数量加 $1$。
- 当遍历到「右括号」的时候：
     - 如果此时「左括号」的数量不为 $0$，因为「右括号」可以与之前遍历到的「左括号」匹配，此时「左括号」出现的次数 $-1$ ；
     - 如果此时「左括号」的数量为 $0$，「右括号」数量加 $1$。

==通过这样的计数规则，得到的「左括号」和「右括号」的数量就是**各自最少应该删除的数量**==。

### 解法1 回溯+剪枝
题目让我们删除括号使得剩下的括号匹配，要求我们**删除最少的括号数**，并且要求得到所有的结果。我们可以使用回溯算法，尝试遍历所有可能的去掉非法括号的方案。

首先利用括号匹配的规则求出该字符串 $s$ 中**最少需要去掉的**左括号的数目 $lremove$ 和右括号的数目 $rremove$，然后我们**尝试在原字符串 $s$ 中去掉 $lremove$ 个左括号和 $rremove$ 个右括号**，然后**检测剩余的字符串是否合法匹配**，如果合法匹配则则认为该字符串为可能的结果，我们利用回溯算法来尝试搜索所有可能的去除括号的方案。

在进行回溯时可利用以下的剪枝技巧来增加搜索的效率：
- 我们从字符串中每去掉一个括号，则更新 $lremove$ 或者 $rremove$，**当我们发现剩余未尝试的字符串的长度小于 $lremove+rremove$ 时，则停止本次搜索**。
- 当 $lremove$ 和 $rremove$ 同时为 $0$ 时，则我们**检测当前的字符串是否合法匹配**，如果合法匹配则我们将其记录下来。

由于记录的字符串可能存在重复，因此需要对重复的结果进行去重，去重的办法有如下两种：
- 利用哈希表对最终生成的字符串去重。
- 我们在每次进行搜索时，如果遇到连续相同的括号就只需要搜索一次即可，比如当前遇到的字符串为 `"(((())"` ，去掉前四个左括号中的任意一个，生成的字符串是一样的，均为 `"((())` ，因此在尝试搜索时，**只需去掉一个左括号就进行下一轮搜索，不需要将前四个左括号都尝试一遍**。
    - 具体来说，递归搜索的第 $0$ 层中，当前从 $0$ 开始，删除 $s[0]$ 的左括号，递归搜索 `"((())"` ；
    - 递归搜索的第 $1$ 层中，当前也可以从 $0$ 开始，删除 $s[0]$ 的左括号，递归搜索 `"(())"` 。
    - 返回到递归搜索的第 $1$ 层，这时不会从连续左括号中进行删除，而是删除 $s[3]$ 的右括号

根据上面的去重思路2，可以写出**遍历下一个删除位置的写法**：
```cpp
class Solution {
public:
    vector<string> ans;
    vector<string> removeInvalidParentheses(string s) {
        int lremove = 0, rremove = 0; // 左括号和右括号最少应被删除的数量
        for (char c : s) {
            if (c == '(') ++lremove;
            else if (c == ')') {
                if (lremove == 0) ++rremove;
                else --lremove; // 左右括号匹配
            }
        }
        dfs(s, 0, lremove, rremove);
        return ans;
    }
    void dfs(const string &s, int start, int lremove, int rremove) {
        if (lremove == 0 && rremove == 0) {
            if (valid(s)) ans.push_back(s);
            return;
        }
        for (int i = start; i < s.size(); ++i) {
            if (i != start && s[i] == s[i - 1]) continue;
            // 如果剩余的字符无法满足去掉的数量要求，直接返回
            if (lremove + rremove > s.size() - i) return; // 剩余未尝试的字符串长度较小，无法继续
            // 尝试去掉一个左括号
            if (lremove > 0 && s[i] == '(')
                dfs(s.substr(0, i) + s.substr(i + 1), i, lremove - 1, rremove); // 注意下个开始的下标还是i
            // 尝试去掉一个右括号
            if (rremove > 0 && s[i] == ')')
                dfs(s.substr(0, i) + s.substr(i + 1), i, lremove, rremove - 1);
        }
    }
    inline bool valid(const string &s) {
        int cnt = 0;
        for (char c : s) {
            if (c == '(') ++cnt;
            else if (c == ')') { // 有括号以外的字符
                --cnt;
                if (cnt < 0) return false;
            }
        }
        return cnt == 0;
    }
};
```
再补充**一个当前删/不删+哈希集合去重的写法**，不知道该怎么去重，就用 `set` 存储结果：
```cpp
class Solution {
public:
    unordered_set<string> st;
    vector<string> removeInvalidParentheses(string s) {
        int lremove = 0, rremove = 0; // 左括号和右括号最少应被删除的数量
        for (char c : s) {
            if (c == '(') ++lremove;
            else if (c == ')') {
                if (lremove == 0) ++rremove;
                else --lremove;
            }
        }
        dfs(s, 0, lremove, rremove);
        return vector<string>(st.begin(), st.end());
    }
    void dfs(const string &s, int i, int lremove, int rremove) {
        if (lremove == 0 && rremove == 0) {
            if (valid(s)) st.insert(s);
            return;
        }
        // 还需要删除的括号个数 > 能够删除的字符个数
        if (lremove + rremove > s.size() - i) return;
        // 不删除
        dfs(s, i + 1, lremove, rremove);
        // 删除,分左右括号考虑
        // 尝试去掉一个左括号
        if (lremove > 0 && s[i] == '(')
            dfs(s.substr(0, i) + s.substr(i + 1), i, lremove - 1, rremove);
        // 尝试去掉一个右括号
        if (rremove > 0 && s[i] == ')')
            dfs(s.substr(0, i) + s.substr(i + 1), i, lremove, rremove - 1);
    }
    inline bool valid(const string &s) {
        int cnt = 0;
        for (char c : s) {
            if (c == '(') ++cnt;
            else if (c == ')') { // 有括号以外的字符
                --cnt;
                if (cnt < 0) return false;
            }
        }
        return cnt == 0;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n2^n)$ ，其中 $n$ 为字符串的长度。考虑到一个字符串最多可能有 $2^n$ 个子序列，每个子序列可能需要进行一次合法性检测，因此时间复杂度为 $O(n2^n)$ 。
- 空间复杂度：$O(n^2)$ ，其中 $n$ 为字符串的长度。返回结果不计入空间复杂度，考虑到递归调用栈的深度，并且每次递归调用时需要复制字符串一次，因此空间复杂度为 $O(n^2)$ 。

---
### 解法2 广度优先搜索
注意到题目中要求**最少删除**，这样的描述正是广度优先搜索算法应用的场景，并且题目也要求我们输出所有的结果。==我们在进行广度优先搜索时每一轮删除字符串中的 $1$ 个括号，直到出现合法匹配的字符串为止==，此时**进行轮转的次数**即为**最少需要删除括号的个数**。

我们进行广度优先搜索时，每次保存上一轮搜索的结果，然后对「上一轮已经保存的结果中的每个字符串」**尝试所有可能的删除一个括号的方法**，然后将保存的结果进行下一轮搜索。在保存结果时，我们可以利用哈希表对上一轮生成的结果去重，从而提高效率。
```cpp
class Solution {
public:
    unordered_set<string> curSet;
    vector<string> removeInvalidParentheses(string s) {
        vector<string> ans;
        curSet.insert(s);
        while (true) {
            for (auto &str : curSet) if (valid(str)) ans.emplace_back(str);
            if (ans.size() > 0) return ans; // BFS,删除括号最少的合法字符串
            unordered_set<string> nextSet;
            for (auto &str : curSet) {
                for (int i = 0; i < str.size(); ++i) {
                    if (i > 0 && str[i] == str[i - 1]) continue; // 对当前连续重复的括号只需删除一个
                    if (str[i] == '('  || str[i] == ')') // 第k步迭代,删除第k个括号
                        nextSet.insert(str.substr(0, i) + str.substr(i + 1, str.size()));
                }
            }
            curSet = nextSet;
        }
    }
    inline bool valid(const string &s) {
        int cnt = 0;
        for (char c : s) {
            if (c == '(') ++cnt;
            else if (c == ')') { // 有括号以外的字符
                --cnt;
                if (cnt < 0) return false;
            }
        }
        return cnt == 0;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n2^n)$ ，其中 $n$ 为字符串的长度。考虑到一个字符串最多可能有 $2^n$  个子序列，因此时间复杂度为 $O(n2^n)$ 。
- 空间复杂度：$O(n \times C_n^\frac{n}{2})$ ，其中 $n$ 为字符串的长度。我们在进行第 $i$ 轮迭代时，会从原始字符串中删除 $i$ 个括号，因此第 $i$ 轮迭代产生的字符串最多有 $C_n^i$​ 个，当 $i = \frac{n}{2}$  时组合数最大，此时迭代生成的字符串个数最多，因此空间复杂度为 $O(n \times C_n^\frac{n}{2})$ 。

---
### 解法3 枚举状态子集
首先我们利用**括号匹配的规则**，求出该字符串 $s$ 中最少需要去掉的左括号的数目 $lremove$ 和右括号的数目 $rremove$，然后我们利用状态子集，求出字符串 $s$ 中**所有的左括号去掉 $lremove$ 的左括号的子集**，和**所有的右括号去掉 $rremove$ 个右括号的子集**，==依次枚举这两种子集的组合==，检测组合后的字符串是否合法匹配，如果字符串合法则记录，最后我们利用哈希表对结果进行去重。
```cpp
class Solution {
public:
    bool checkValid(const string & str, int lmask, vector<int> & left, int rmask, vector<int> & right) {
        int pos1 = 0;
        int pos2 = 0;
        int cnt = 0;

        for (int i = 0; i < str.size(); i++) {
            if (pos1 < left.size() && i == left[pos1]) {
                if (!(lmask & (1 << pos1))) {
                    cnt++;
                }
                pos1++;
            } else if (pos2 < right.size() && i == right[pos2]) {
                if (!(rmask & (1 << pos2))) {
                    cnt--;
                    if (cnt < 0) {
                        return false;
                    }
                }
                pos2++;
            }
        }

        return cnt == 0;
    }

    string recoverStr(const string & str, int lmask, vector<int> & left, int rmask, vector<int> & right){
        string ans;
        int pos1 = 0;
        int pos2 = 0;

        for (int i = 0; i < str.size(); i++) {
            if (pos1 < left.size() && i == left[pos1]) {
                if (!(lmask & (1 << pos1))){
                    ans.push_back(str[i]);
                }
                pos1++;
            } else if (pos2 < right.size() && i == right[pos2]) {
                if (!(rmask & (1 << pos2))) {
                    ans.push_back(str[i]);
                }
                pos2++;
            } else {
                ans.push_back(str[i]);
            }
        }

        return ans;
    }

    vector<string> removeInvalidParentheses(string s) {
        int lremove = 0;
        int rremove = 0;
        vector<int> left;
        vector<int> right;
        vector<string> ans;
        unordered_set<string> cnt;

        for (int i = 0; i < s.size(); i++) {
            if (s[i] == '(') {
                left.push_back(i);
                lremove++;
            } else if (s[i] == ')') {
                right.push_back(i);
                if (lremove == 0) {
                    rremove++;
                } else {
                    lremove--;
                }
            }
        }

        int m = left.size();
        int n = right.size();
        vector<int> maskArr1;
        vector<int> maskArr2;
        for (int i = 0; i < (1 << m); i++) {
            if (__builtin_popcount(i) != lremove) {
                continue;
            }
            maskArr1.push_back(i);
        }
        for (int j = 0; j < (1 << n); j++) {
            if (__builtin_popcount(j) != rremove) {
                continue;
            }
            maskArr2.push_back(j);
        }
        for (auto mask1 : maskArr1) {
            for (auto mask2 : maskArr2) {
                if (checkValid(s, mask1, left, mask2, right)) {
                    cnt.insert(recoverStr(s, mask1, left, mask2, right));
                }
            }
        }
        for (auto v : cnt) {
            ans.emplace_back(v);
        }

        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n2^n)$ ，其中 $n$ 为字符串的长度。考虑到一个字符串最多可能有 $2^n$ 个子序列，每个子序列可能需要进行一次合法性检测，因此时间复杂度为 $O(n2^n)$ 。
- 空间复杂度：$O(n \times C_n^m)$ ，其中 $n$ 为字符串的长度，$m$ 为字符串中非法括号的数目。空间复杂度主要为集合 $\texttt{cnt}$ 占用的空间。
