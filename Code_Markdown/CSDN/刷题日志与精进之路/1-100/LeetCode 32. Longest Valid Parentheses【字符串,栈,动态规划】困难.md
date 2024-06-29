> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091352110.png)
给你一个只包含 `'('` 和 `')'` 的字符串，找出最长有效（格式正确且连续）括号子串的长度。

**示例 1：**

**输入：**s = "(()"
**输出：**2
**解释：**最长有效括号子串是 "()"

**示例 2：**

**输入：**s = ")()())"
**输出：**4
**解释：**最长有效括号子串是 "()()"

**示例 3：**

**输入：**s = ""
**输出：**0

**提示：**

- `0 <= s.length <= 3 * 104`
- `s[i]` 为 `'('` 或 `')'`

---
### 解法1 动态规划（看懂了）
我们定义 dp[i]\textit{dp}[i]dp[i] 表示以下标 iii 字符结尾的最长有效括号的长度。我们将 dp\textit{dp}dp 数组全部初始化为 000 。显然有效的子串一定以 ‘)’\text{‘)’}‘)’ 结尾，因此我们可以知道以 ‘(’\text{‘(’}‘(’ 结尾的子串对应的 dp\textit{dp}dp 值必定为 000 ，我们只需要求解 ‘)’\text{‘)’}‘)’ 在 dp\textit{dp}dp 数组中对应位置的值。

我们从前往后遍历字符串求解 dp\textit{dp}dp 值，我们每两个字符检查一次：

s[i]=‘)’s[i] = \text{‘)’}s[i]=‘)’ 且 s[i−1]=‘(’s[i - 1] = \text{‘(’}s[i−1]=‘(’，也就是字符串形如 “……()”“……()”“……()”，我们可以推出：
dp[i]=dp[i−2]+2 \textit{dp}[i]=\textit{dp}[i-2]+2
dp[i]=dp[i−2]+2
我们可以进行这样的转移，是因为结束部分的 "()" 是一个有效子字符串，并且将之前有效子字符串的长度增加了 $2$ 。
2. s[i]=‘)’s[i] = \text{‘)’}s[i]=‘)’ 且 s[i−1]=‘)’s[i - 1] = \text{‘)’}s[i−1]=‘)’，也就是字符串形如 “……))”“……))”“……))”，我们可以推出： 如果 s[i−dp[i−1]−1]=‘(’s[i - \textit{dp}[i - 1] - 1] = \text{‘(’}s[i−dp[i−1]−1]=‘(’，那么

dp[i]=dp[i−1]+dp[i−dp[i−1]−2]+2 \textit{dp}[i]=\textit{dp}[i-1]+\textit{dp}[i-\textit{dp}[i-1]-2]+2
dp[i]=dp[i−1]+dp[i−dp[i−1]−2]+2
我们考虑如果倒数第二个 ‘)’\text{‘)’}‘)’ 是一个有效子字符串的一部分（记作 subssub_ssub 
s
​
 ），对于最后一个 ‘)’\text{‘)’}‘)’ ，如果它是一个更长子字符串的一部分，那么它一定有一个对应的 ‘(’\text{‘(’}‘(’ ，且它的位置在倒数第二个 ‘)’\text{‘)’}‘)’ 所在的有效子字符串的前面（也就是 subssub_ssub 
s
​
  的前面）。因此，如果子字符串 subssub_ssub 
s
​
  的前面恰好是 ‘(’\text{‘(’}‘(’ ，那么我们就用 222 加上 subssub_ssub 
s
​
  的长度（dp[i−1]\textit{dp}[i-1]dp[i−1]）去更新 dp[i]\textit{dp}[i]dp[i]。同时，我们也会把有效子串 “(subs)”“(sub_s)”“(sub 
s
​
 )” 之前的有效子串的长度也加上，也就是再加上 dp[i−dp[i−1]−2]\textit{dp}[i-\textit{dp}[i-1]-2]dp[i−dp[i−1]−2]。

最后的答案即为 dp\textit{dp}dp 数组中的最大值。

Golang
class Solution {
public:
    int longestValidParentheses(string s) {
        int maxans = 0, n = s.length();
        vector<int> dp(n, 0);
        for (int i = 1; i < n; i++) {
            if (s[i] == ')') {
                if (s[i - 1] == '(') {
                    dp[i] = (i >= 2 ? dp[i - 2] : 0) + 2;
                } else if (i - dp[i - 1] > 0 && s[i - dp[i - 1] - 1] == '(') {
                    dp[i] = dp[i - 1] + ((i - dp[i - 1]) >= 2 ? dp[i - dp[i - 1] - 2] : 0) + 2;
                }
                maxans = max(maxans, dp[i]);
            }
        }
        return maxans;
    }
};
复杂度分析

时间复杂度： O(n)O(n)O(n)，其中 nnn 为字符串的长度。我们只需遍历整个字符串一次，即可将 dp\textit{dp}dp 数组求出来。

空间复杂度： O(n)O(n)O(n)。我们需要一个大小为 nnn 的 dp\textit{dp}dp 数组。

方法二：栈
思路和算法

撇开方法一提及的动态规划方法，相信大多数人对于这题的第一直觉是找到每个可能的子串后判断它的有效性，但这样的时间复杂度会达到 O(n3)O(n^3)O(n 
3
 )，无法通过所有测试用例。但是通过栈，我们可以在遍历给定字符串的过程中去判断到目前为止扫描的子串的有效性，同时能得到最长有效括号的长度。

具体做法是我们始终保持栈底元素为当前已经遍历过的元素中「最后一个没有被匹配的右括号的下标」，这样的做法主要是考虑了边界条件的处理，栈里其他元素维护左括号的下标：

对于遇到的每个 ‘(’\text{‘(’}‘(’ ，我们将它的下标放入栈中
对于遇到的每个 ‘)’\text{‘)’}‘)’ ，我们先弹出栈顶元素表示匹配了当前右括号：
如果栈为空，说明当前的右括号为没有被匹配的右括号，我们将其下标放入栈中来更新我们之前提到的「最后一个没有被匹配的右括号的下标」
如果栈不为空，当前右括号的下标减去栈顶元素即为「以该右括号为结尾的最长有效括号的长度」
我们从前往后遍历字符串并更新答案即可。

需要注意的是，如果一开始栈为空，第一个字符为左括号的时候我们会将其放入栈中，这样就不满足提及的「最后一个没有被匹配的右括号的下标」，为了保持统一，我们在一开始的时候往栈中放入一个值为 −1-1−1 的元素。


1 / 11
Java
C++
C
Golang
class Solution {
public:
    int longestValidParentheses(string s) {
        int maxans = 0;
        stack<int> stk;
        stk.push(-1);
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == '(') {
                stk.push(i);
            } else {
                stk.pop();
                if (stk.empty()) {
                    stk.push(i);
                } else {
                    maxans = max(maxans, i - stk.top());
                }
            }
        }
        return maxans;
    }
};
复杂度分析

时间复杂度： O(n)O(n)O(n)，nnn 是给定字符串的长度。我们只需要遍历字符串一次即可。

空间复杂度： O(n)O(n)O(n)。栈的大小在最坏情况下会达到 nnn，因此空间复杂度为 O(n)O(n)O(n) 。

方法三：不需要额外的空间
思路和算法

在此方法中，我们利用两个计数器 left\textit{left}left 和 right\textit{right}right 。首先，我们从左到右遍历字符串，对于遇到的每个 ‘(’\text{‘(’}‘(’，我们增加 left\textit{left}left 计数器，对于遇到的每个 ‘)’\text{‘)’}‘)’ ，我们增加 right\textit{right}right 计数器。每当 left\textit{left}left 计数器与 right\textit{right}right 计数器相等时，我们计算当前有效字符串的长度，并且记录目前为止找到的最长子字符串。当 right\textit{right}right 计数器比 left\textit{left}left 计数器大时，我们将 left\textit{left}left 和 right\textit{right}right 计数器同时变回 000。

这样的做法**贪心地考虑了以当前字符下标结尾的有效括号长度**，每次当右括号数量多于左括号数量的时候之前的字符我们都扔掉不再考虑，重新从下一个字符开始计算，但这样会漏掉一种情况，就是遍历的时候左括号的数量始终大于右括号的数量，即 (() ，这种时候最长有效括号是求不出来的。

解决的方法也很简单，我们只需要从右往左遍历用类似的方法计算即可，只是这个时候判断条件反了过来：

当 left\textit{left}left 计数器比 right\textit{right}right 计数器大时，我们将 left\textit{left}left 和 right\textit{right}right 计数器同时变回 000
当 left\textit{left}left 计数器与 right\textit{right}right 计数器相等时，我们计算当前有效字符串的长度，并且记录目前为止找到的最长子字符串
这样我们就能涵盖所有情况从而求解出答案。


1 / 17
Java
C++
C
Golang
class Solution {
public:
    int longestValidParentheses(string s) {
        int left = 0, right = 0, maxlength = 0;
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == '(') {
                left++;
            } else {
                right++;
            }
            if (left == right) {
                maxlength = max(maxlength, 2 * right);
            } else if (right > left) {
                left = right = 0;
            }
        }
        left = right = 0;
        for (int i = (int)s.length() - 1; i >= 0; i--) {
            if (s[i] == '(') {
                left++;
            } else {
                right++;
            }
            if (left == right) {
                maxlength = max(maxlength, 2 * left);
            } else if (left > right) {
                left = right = 0;
            }
        }
        return maxlength;
    }
};
复杂度分析

时间复杂度： O(n)O(n)O(n)，其中 nnn 为字符串长度。我们只要正反遍历两边字符串即可。

空间复杂度： O(1)O(1)O(1)。我们只需要常数空间存放若干变量。

作者：力扣官方题解
链接：https://leetcode.cn/problems/longest-valid-parentheses/solutions/314683/zui-chang-you-xiao-gua-hao-by-leetcode-solution/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。