> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091347757.png)

数字 `n` 代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的并且 **有效的** 括号组合。

**示例 1：**
```java
输入：n = 3
输出：["((()))","(()())","(())()","()(())","()()()"]
```
**示例 2：**
```java
输入：n = 1
输出：["()"]
```
**提示：**
- `1 <= n <= 8`

---
### 解法 暴力递归（生成 $2^{2n}$ 个序列）
每个位置可以使用 `(` 或者 `)` ，因此总共有 $2^{2n}$ 个序列。我们全部生成出来，然后检查每个是否有效即可。

为了生成所有序列，我们可以使用递归。长度为 $n$ 的序列就是在长度为 $n - 1$ 的序列前加一个 `(` 或  `)` 。

为了检查序列是否有效，我们遍历这个序列，并使用一个变量 $\textit{balance}$ 表示左括号的数量减去右括号的数量。**如果在遍历过程中 $\textit{balance}$ 的值小于零，或者结束时 $\textit{balance}$ 的值不为零，那么该序列就是无效的**，否则它是有效的。
```cpp
class Solution {
private:
    vector<string> ans;
    bool valid(const string& s) {
        int balance = 0;
        for (char c : s) {
            if (c == '(') ++balance;
            else --balance;
            if (balance < 0) return false;
        }
        return balance == 0;
    }
    void dfs(string& s, int n) {
        if (n == s.size()) {
            if (valid(s)) ans.push_back(s); // 检查是否有效序列
            return;
        }
        s += '(';
        dfs(s, n);
        s.pop_back();
        s += ')';
        dfs(s, n);
        s.pop_back();
    }
public:
    vector<string> generateParenthesis(int n) {
        string current;
        dfs(current, n * 2);
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(2^{2n}n)$ ，对于 $2^{2n}$ 个序列中的每一个，我们用于建立和验证该序列的复杂度为 $O(n)$ 。
- 空间复杂度：$O(n)$ ，除了答案数组之外，我们所需要的空间取决于递归栈的深度，每一层递归函数需要 $O(1)$ 的空间，最多递归 $2n$ 层，因此空间复杂度为 $O(n)$ 。

---
### 解法2 回溯剪枝
方法一还有改进的余地：我们可以**只在序列仍然保持有效时才添加 `(, )` ，而不是像方法一那样每次添加**。我们可以通过**跟踪到目前为止放置的左括号和右括号的数目**来做到这一点：
- 如果已用左括号数量不大于 $n$ ，我们可以放一个左括号。
- 如果已用左括号的数量大于已用右括号数量，我们可以放一个右括号。
```cpp
class Solution {
private:
    vector<string> ans;
    void dfs(const string& s, int l, int r) {
        if (l < 0 || l > r) return;
        if (l == 0 && r == 0) {
            ans.push_back(s);
            return;
        }
        dfs(s + '(', l - 1, r);
        dfs(s + ')', l, r - 1);
    }
public:
    vector<string> generateParenthesis(int n) {
        dfs("", n, n);
        return ans;
    }
};
```
复杂度分析：这依赖于理解 $generateParenthesis(n)$ 中有多少个元素。这个分析超出了本文的范畴，但事实证明这是第 nnn 个卡特兰数 $\dfrac{1}{n+1}\dbinom{2n}{n}$ ，这是由 $\dfrac{4^n}{n\sqrt{n}}$ 渐近界定的。
- 时间复杂度：$O(\dfrac{4^n}{\sqrt{n}})$ ，在回溯过程中，每个答案需要 $O(n)$ 的时间复制到答案数组中。
- 空间复杂度：$O(n)$ ，除了答案数组之外，我们所需要的空间取决于递归栈的深度，每一层递归函数需要 $O(1)$ 的空间，最多递归 $2n$ 层，因此空间复杂度为 $O(n)$ 。

---
### 解法3 按括号序列的长度递归（记忆化搜索）或动态规划
任何一个括号序列都一定是由 `(` 开头，并且第一个 `(` 一定有一个唯一与之对应的 `)` 。这样一来，每一个括号序列可以用 $(a)b$ 来表示，其中 $a$ 与 $b$ 分别是一个合法的括号序列（可以为空）。

这样就把**生成 $n$ 对括号的所有序列**这一问题，==分解为「生成 $a$ 对括号的所有序列」+「生成 $b$ 对括号的所有序列」两个规模更小但是本质相同的子问题==，$a+b=n-1$ ，接着在 $a$ 对括号的所有序列外面加上一个括号，这样就得到了 $n$ 对括号的所有序列，并且完成了问题的分解。
> 总结一下，就是找到了一个最优子结构，将原问题转换为较小子问题求解。这道题的动态规划解难就是因为这个最优子结构不好想到。

一个示例如下：
- $i = 0$ 结果是空；
- $i = 1$ 结果有一种：$()$
- $i = 2$ 结果有两种：$()(),\ (())$
- $i = 3$ 的结果，使用公式 `( + a + ) + b` ，有如下三种情况共5种结果（以花括号来表示新添加的括号）：
    - $a = 2, b = 0$ ：$\{()()\},\ \{(())\}$ 
    - $a = 1, b = 1$ ：$\{()\}()$ 
    - $a= 0, b = 2$ ：$\{\}()(), \{\}(())$

为了生成所有长度为 $2n$（具有 $n$ 个括号）的括号序列，我们定义一个函数 $generate(n)$ 来返回所有可能的括号序列。那么在函数 $\textit{generate}(n)$ 的过程中：
- 我们需要枚举 $a$ ，从 $0$ 开始一直到 $n - 1$ ，并递归调用 $generate(a)$ 计算 $a$ 的所有可能性；
- 相应的，$b$ 从 $n - 1$ 一直到 $0$ ，递归调用 $generate(b)$ 即可计算 $b$ 的所有可能性；
- 遍历 $a$ 与 $b$ 的所有可能性并拼接，即可得到所有长度为 $2n$ 的括号序列。

为了节省计算时间，我们在每次 $generate(i)$ 函数返回之前，把返回值存储起来，下次再调用 $generate(i)$ 时可以直接返回，不需要再递归计算（即记忆化搜索）。
```cpp
class Solution {
private:
    shared_ptr<vector<string>> cache[100] = {nullptr}; // string[][]
    shared_ptr<vector<string>> generate(int n) {
        if (cache[n] != nullptr) return cache[n];
        if (n == 0) 
            cache[0] = shared_ptr<vector<string>>(new vector<string>{""});
        else {
            auto result = shared_ptr<vector<string>>(new vector<string>);
            for (int i = 0; i != n; ++i) {
                auto lefts = generate(i);
                auto rights = generate(n - i - 1);
                for (const string& left : *lefts)
                    for (const string& right : *rights)
                        result -> push_back("(" + left + ")" + right);
            }
            cache[n] = result;
        }
        return cache[n];
    }
public:
    vector<string> generateParenthesis(int n) {
        return *generate(n);
    }
};
```
动态规划解法如下：
```cpp
class Solution {
public:
    vector<string> generateParenthesis(int n) {
        if (n == 0) return {};
        if (n == 1) return { "()" };
        vector<vector<string>> dp(n + 1);
        dp[0] = { "" };
        dp[1] = { "()" };
        for (int parenthesisNum = 2; parenthesisNum <= n; ++parenthesisNum) {
            for (int i = 0; i < parenthesisNum; ++i) {
    int j = parenthesisNum - i - 1;
                for (string &a : dp[i])
                    for (string &b : dp[j]) 
                            dp[parenthesisNum].push_back("(" + a + ")" + b);
            }
        }
        return dp[n];
    }
};
```
复杂度分析：
- 时间复杂度：$O(\dfrac{4^n}{\sqrt{n}})$ ，该分析与 方法二 类似
- 空间复杂度：$O(\dfrac{4^n}{\sqrt{n}})$ ，此方法除答案数组外，中间过程中会存储与答案数组同样数量级的临时数组，是我们所需要的空间复杂度。