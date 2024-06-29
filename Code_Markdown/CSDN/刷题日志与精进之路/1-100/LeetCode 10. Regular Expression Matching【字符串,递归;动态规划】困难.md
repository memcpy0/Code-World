> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091341905.png)

给你一个字符串 `s` 和一个字符规律 `p`，请你来实现一个支持 `'.'` 和 `'*'` 的正则表达式匹配。

- `'.'` 匹配任意单个字符
- `'*'` 匹配零个或多个前面的那一个元素

所谓匹配，是要涵盖 **整个** 字符串 `s`的，而不是部分字符串。

**示例 1：**
```java
输入：s = "aa", p = "a"
输出：false
解释："a" 无法匹配 "aa" 整个字符串。
```
**示例 2:**
```java
输入：s = "aa", p = "a*"
输出：true
解释：因为 '*' 代表可以匹配零个或多个前面的那一个元素, 在这里前面的元素就是 'a'。因此，字符串 "aa" 可被视为 'a' 重复了一次。
```
**示例 3：**
```java
输入：s = "ab", p = ".*"
输出：true
解释：".*" 表示可匹配零个或多个（'*'）任意字符（'.'）。
```
**提示：**
- `1 <= s.length <= 20`
- `1 <= p.length <= 20`
- `s` 只包含从 `a-z` 的小写字母。
- `p` 只包含从 `a-z` 的小写字母，以及字符 `.` 和 `*`。
- 保证每次出现字符 `*` 时，前面都匹配到有效的字符

---
题意：匹配正则表达式。

思路：递归，基准条件是 `p空` ，然后匹配第一个字符，接着匹配剩下的字符。

代码：
```cpp
class Solution {
public:
    bool isMatch(string s, string p) {
        if (p.empty()) return s.empty(); //p空和s空为真;p空s非空为假
        bool firstMatch = !s.empty() && (s[0] == p[0] || p[0] == '.'); //s非空的情况下比较第一个字符

        if (p.size() >= 2 && p[1] == '*') { //匹配s[0]0次或多次 
            return isMatch(s, p.substr(2)) || (firstMatch && isMatch(s.substr(1), p));
        } else 
            return firstMatch && isMatch(s.substr(1), p.substr(1)); //递归判断第二个字符是否匹配
    }
};
```
这种做法之前能过，现在已经超时了。 
 
方法一：动态规划
思路与算法

题目中的匹配是一个「逐步匹配」的过程：我们每次从字符串 ppp 中取出一个字符或者「字符 + 星号」的组合，并在 sss 中进行匹配。对于 ppp 中一个字符而言，它只能在 sss 中匹配一个字符，匹配的方法具有唯一性；而对于 ppp 中字符 + 星号的组合而言，它可以在 sss 中匹配任意自然数个字符，并不具有唯一性。因此我们可以考虑使用动态规划，对匹配的方案进行枚举。

我们用 f[i][j]f[i][j]f[i][j] 表示 sss 的前 iii 个字符与 ppp 中的前 jjj 个字符是否能够匹配。在进行状态转移时，我们考虑 ppp 的第 jjj 个字符的匹配情况：

如果 ppp 的第 jjj 个字符是一个小写字母，那么我们必须在 sss 中匹配一个相同的小写字母，即

f[i][j]={f[i−1][j−1],s[i]=p[j]false,s[i]≠p[j] f[i][j] = \begin{cases} f[i - 1][j - 1], & s[i] = p[j]\\ \text{false}, & s[i] \neq p[j] \end{cases}
f[i][j]={ 
f[i−1][j−1],
false,
​
  
s[i]=p[j]
s[i]

=p[j]
​
 
也就是说，如果 $s$ 的第 $i$ 个字符与 $p$ 的第 $j$ 个字符不相同，那么无法进行匹配；否则我们可以匹配两个字符串的最后一个字符，完整的匹配结果取决于两个字符串前面的部分。
如果 ppp 的第 jjj 个字符是 *，那么就表示我们可以对 ppp 的第 j−1j-1j−1 个字符匹配任意自然数次。在匹配 000 次的情况下，我们有

f[i][j]=f[i][j−2] f[i][j] = f[i][j - 2]
f[i][j]=f[i][j−2]
也就是我们「浪费」了一个字符 + 星号的组合，没有匹配任何 $s$ 中的字符。

在匹配 $1,2,3, \cdots$ 次的情况下，类似地我们有
f[i][j]=f[i−1][j−2],if s[i]=p[j−1]f[i][j]=f[i−2][j−2],if s[i−1]=s[i]=p[j−1]f[i][j]=f[i−3][j−2],if s[i−2]=s[i−1]=s[i]=p[j−1]⋯⋯ \begin{aligned} & f[i][j] = f[i - 1][j - 2], \quad && \text{if~} s[i] = p[j - 1] \\ & f[i][j] = f[i - 2][j - 2], \quad && \text{if~} s[i - 1] = s[i] = p[j - 1] \\ & f[i][j] = f[i - 3][j - 2], \quad && \text{if~} s[i - 2] = s[i - 1] = s[i] = p[j - 1] \\ & \cdots\cdots & \end{aligned}
​
  
f[i][j]=f[i−1][j−2],
f[i][j]=f[i−2][j−2],
f[i][j]=f[i−3][j−2],
⋯⋯
​
  
​
  
if s[i]=p[j−1]
if s[i−1]=s[i]=p[j−1]
if s[i−2]=s[i−1]=s[i]=p[j−1]
​
 
如果我们通过这种方法进行转移，那么我们就需要枚举这个组合到底匹配了 $s$ 中的几个字符，会增导致时间复杂度增加，并且代码编写起来十分麻烦。我们不妨换个角度考虑这个问题：字母 + 星号的组合在匹配的过程中，本质上只会有两种情况：

- 匹配 $s$ 末尾的一个字符，将该字符扔掉，而该组合还可以继续进行匹配；

- 不匹配字符，将该组合扔掉，不再进行匹配。

如果按照这个角度进行思考，我们可以写出很精巧的状态转移方程：
f[i][j]={f[i−1][j] or f[i][j−2],s[i]=p[j−1]f[i][j−2],s[i]≠p[j−1] f[i][j] = \begin{cases} f[i - 1][j] \text{~or~} f[i][j - 2], & s[i] = p[j - 1] \\ f[i][j - 2], & s[i] \neq p[j - 1] \end{cases}
f[i][j]={ 
f[i−1][j] or f[i][j−2],
f[i][j−2],
​
  
s[i]=p[j−1]
s[i]

=p[j−1]
​
 
在任意情况下，只要 p[j]p[j]p[j] 是 .，那么 p[j]p[j]p[j] 一定成功匹配 sss 中的任意一个小写字母。
最终的状态转移方程如下：

f[i][j]={if (p[j]≠ ‘*’)={f[i−1][j−1],matches(s[i],p[j])false,otherwiseotherwise={f[i−1][j] or f[i][j−2],matches(s[i],p[j−1])f[i][j−2],otherwise f[i][j] = \begin{cases} \text{if~} (p[j] \neq \text{~`*'}) = \begin{cases} f[i - 1][j - 1], & \textit{matches}(s[i], p[j])\\ \text{false}, & \text{otherwise} \end{cases} \\ \text{otherwise} = \begin{cases} f[i - 1][j] \text{~or~} f[i][j - 2], & \textit{matches}(s[i], p[j-1]) \\ f[i][j - 2], & \text{otherwise} \end{cases} \end{cases}
f[i][j]= 
⎩
⎨
⎧
​
  
if (p[j]

= ‘*’)={ 
f[i−1][j−1],
false,
​
  
matches(s[i],p[j])
otherwise
​
 
otherwise={ 
f[i−1][j] or f[i][j−2],
f[i][j−2],
​
  
matches(s[i],p[j−1])
otherwise
​
 
​
 
其中 matches(x,y)\textit{matches}(x, y)matches(x,y) 判断两个字符是否匹配的辅助函数。只有当 yyy 是 . 或者 xxx 和 yyy 本身相同时，这两个字符才会匹配。

细节

动态规划的边界条件为 f[0][0]=truef[0][0] = \text{true}f[0][0]=true，即两个空字符串是可以匹配的。最终的答案即为 f[m][n]f[m][n]f[m][n]，其中 mmm 和 nnn 分别是字符串 sss 和 ppp 的长度。由于大部分语言中，字符串的字符下标是从 000 开始的，因此在实现上面的状态转移方程时，需要注意状态中每一维下标与实际字符下标的对应关系。

在上面的状态转移方程中，如果字符串 ppp 中包含一个「字符 + 星号」的组合（例如 a*），那么在进行状态转移时，会先将 a 进行匹配（当 p[j]p[j]p[j] 为 a 时），再将 a* 作为整体进行匹配（当 p[j]p[j]p[j] 为 * 时）。然而，在题目描述中，我们必须将 a* 看成一个整体，因此将 a 进行匹配是不符合题目要求的。看来我们进行了额外的状态转移，这样会对最终的答案产生影响吗？这个问题留给读者进行思考。

C++
Java
Python3
Golang
class Solution {
public:
    bool isMatch(string s, string p) {
        int m = s.size();
        int n = p.size();

        auto matches = [&](int i, int j) {
            if (i == 0) {
                return false;
            }
            if (p[j - 1] == '.') {
                return true;
            }
            return s[i - 1] == p[j - 1];
        };

        vector<vector<int>> f(m + 1, vector<int>(n + 1));
        f[0][0] = true;
        for (int i = 0; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (p[j - 1] == '*') {
                    f[i][j] |= f[i][j - 2];
                    if (matches(i, j - 1)) {
                        f[i][j] |= f[i - 1][j];
                    }
                }
                else {
                    if (matches(i, j)) {
                        f[i][j] |= f[i - 1][j - 1];
                    }
                }
            }
        }
        return f[m][n];
    }
};
复杂度分析

时间复杂度：O(mn)O(mn)O(mn)，其中 mmm 和 nnn 分别是字符串 sss 和 ppp 的长度。我们需要计算出所有的状态，并且每个状态在进行转移时的时间复杂度为 O(1)O(1)O(1)。

空间复杂度：O(mn)O(mn)O(mn)，即为存储所有状态使用的空间。


整理一下题意，对于字符串 p 而言，有三种字符：

普通字符：需要和 s 中同一位置的字符完全匹配
'.'：能够匹配 s 中同一位置的任意字符
'*'：不能够单独使用 '*'，必须和前一个字符同时搭配使用，数据保证了 '*' 能够找到前面一个字符。能够匹配 s 中同一位置字符任意次。

 
所以本题关键是分析当出现 a* 这种字符时，是匹配 0 个 a、还是 1 个 a、还是 2 个 a ...

本题可以使用动态规划进行求解：

状态定义：f(i,j) 代表考虑 s 中以 i 为结尾的子串和 p 中的 j 为结尾的子串是否匹配。即最终我们要求的结果为 f[n][m] 。

状态转移：也就是我们要考虑 f(i,j) 如何求得，前面说到了 p 有三种字符，所以这里的状态转移也要分三种情况讨论：

p[j] 为普通字符：匹配的条件是前面的字符匹配，同时 s 中的第 i 个字符和 p 中的第 j 位相同。 即 f(i,j) = f(i - 1, j - 1) && s[i] == p[j] 。
p[j] 为 '.'：匹配的条件是前面的字符匹配， s 中的第 i 个字符可以是任意字符。即 f(i,j) = f(i - 1, j - 1) && p[j] == '.'。
p[j] 为 '*'：读得 p[j - 1] 的字符，例如为字符 a。 然后根据 a* 实际匹配 s 中 a 的个数是 0 个、1 个、2 个 ... 3.1. 当匹配为 0 个：f(i,j) = f(i, j - 2) 3.2. 当匹配为 1 个：f(i,j) = f(i - 1, j - 2) && (s[i] == p[j - 1] || p[j - 1] == '.') 3.3. 当匹配为 2 个：f(i,j) = f(i - 2, j - 2) && ((s[i] == p[j - 1] && s[i - 1] == p[j - 1]) || p[j - 1] == '.') ...
我们知道，通过「枚举」来确定 * 到底匹配多少个 a 这样的做法，算法复杂度是很高的。

我们需要挖掘一些「性质」来简化这个过程。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307021846022.png)
三叶姐图有个地方错了，红色那里最后应该是 s[i-2:i-1] ，你写成了s[i-2:i]

代码：

Java
class Solution {
    public boolean isMatch(String ss, String pp) {
        // 技巧：往原字符头部插入空格，这样得到 char 数组是从 1 开始，而且可以使得 f[0][0] = true，可以将 true 这个结果滚动下去
        int n = ss.length(), m = pp.length();
        ss = " " + ss;
        pp = " " + pp;
        char[] s = ss.toCharArray(), p = pp.toCharArray();
        // f(i,j) 代表考虑 s 中的 1~i 字符和 p 中的 1~j 字符 是否匹配
        boolean[][] f = new boolean[n + 1][m + 1];
        f[0][0] = true;
        for (int i = 0; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                // 如果下一个字符是 '*'，则代表当前字符不能被单独使用，跳过
                if (j + 1 <= m && p[j + 1] == '*') continue;
                
                if (i - 1 >= 0 && p[j] != '*') {
                    // 对应了 p[j] 为普通字符和 '.' 的两种情况
                    f[i][j] = f[i - 1][j - 1] && (s[i] == p[j] || p[j] == '.');
                } else if (p[j] == '*') {
                    // 对应了 p[j] 为 '*' 的情况
                    f[i][j] = (j - 2 >= 0 && f[i][j - 2]) || (i - 1 >= 0 && f[i - 1][j] && (s[i] == p[j - 1] || p[j - 1] == '.'));
                }
            }
        }
        return f[n][m];
    }
}

作者：宫水三叶
链接：https://leetcode.cn/problems/regular-expression-matching/solutions/572931/shua-chuan-lc-dong-tai-gui-hua-jie-fa-by-zn9w/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。