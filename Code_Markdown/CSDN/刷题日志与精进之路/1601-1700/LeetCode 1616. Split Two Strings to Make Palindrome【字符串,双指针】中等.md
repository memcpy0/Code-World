> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。
 
You are given two strings `a` and `b` of the same length. Choose an index and split both strings **at the same index**, splitting `a` into two strings: `aprefix` and `asuffix` where `a = aprefix + asuffix`, and splitting `b` into two strings: `bprefix` and `bsuffix` where `b = bprefix + bsuffix`. Check if `aprefix + bsuffix` or `bprefix + asuffix` forms a palindrome.

When you split a string `s` into `sprefix` and `ssuffix`, either `ssuffix` or `sprefix` is allowed to be empty. For example, if `s = "abc"`, then `"" + "abc"`, `"a" + "bc"`, `"ab" + "c"` , and `"abc" + ""` are valid splits.

Return `true` _if it is possible to form_ _a palindrome string, otherwise return_ `false`.

**Notice** that `x + y` denotes the concatenation of strings `x` and `y`.

**Example 1:**
```java
Input: a = "x", b = "y"
Output: true
Explaination: If either a or b are palindromes the answer is true since you can split in the following way:
aprefix = "", asuffix = "x"
bprefix = "", bsuffix = "y"
Then, aprefix + bsuffix = "" + "y" = "y", which is a palindrome.
```
**Example 2:**
```java
Input: a = "xbdef", b = "xecab"
Output: false
```
**Example 3:**
```java
Input: a = "ulacfd", b = "jizalu"
Output: true
Explaination: Split them at index 3:
aprefix = "ula", asuffix = "cfd"
bprefix = "jiz", bsuffix = "alu"
Then, aprefix + bsuffix = "ula" + "alu" = "ulaalu", which is a palindrome.
```
**Constraints:**
-   `1 <= a.length, b.length <= 105`
-   `a.length == b.length`
-   `a` and `b` consist of lowercase English letters

题意：两个长度相同的字符串 `a` 和 `b` ，选择一个下标，将两个字符串都在 **相同的下标** 分割开。由 `a` 可以得到两个字符串： `aprefix` 和 `asuffix` ，满足 `a = aprefix + asuffix` ，同理，由 `b` 可以得到两个字符串 `bprefix` 和 `bsuffix` ，满足 `b = bprefix + bsuffix` 。**判断 `aprefix + bsuffix` 或者 `bprefix + asuffix` 能否构成回文串**，能则返回 `true` 。当你将一个字符串 `s` 分割成 `sprefix` 和 `ssuffix` 时， `ssuffix` 或者 `sprefix` 可以为空。

---
### 解法 贪心+双指针
![600](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181137551.png)
注意，双指针前后缀贪心匹配结束后，两个指针指向的位置恰好就是「接下来要判断的回文串」的左右边界，所以这两块代码逻辑可以无缝对接。
```cpp
class Solution {
public:
    bool checkPalindromeFormation(string a, string b) {
        function<bool(string, int, int)> isPalindrome = [&](const string &x, int l, int r) {
            while (l < r && x[l] == x[r]) ++l, --r;
            return l >= r;
        };
        // 双指针判断x的前缀和y的后缀是否组成回文
        function<bool(string, string)> check = [&](const string &x, const string &y) {
            for (int i = 0, j = y.size() - 1; i < j; ++i, --j) { // 相向双指针
                // x从前往后,与y从后往前对比,前后缀尽量匹配
                // ulac cfd , ula ccfd
                // zjiz alu , zji zalu
                if (x[i] != y[j]) { // 字符x[i]和y[j]不等,要组成回文
                    // 可能从i前分割开,于是要看y[i:j]是否回文
                    // 或者从j后分割开,于是要看x[i:j]是否回文
                    return isPalindrome(y, i, j) || isPalindrome(x, i, j);
                }
            }
            return true;
        };
        return check(a, b) || check(b, a); 
    }
}; 
```
---
### 解法2 中心扩展+双指针
还可使用**中心扩展法**（[LeetCode 647. Palindromic Substrings](https://leetcode.cn/problems/palindromic-substrings/)）。设 $a_{prefix}$ 取 $a$ 前 $m$ 个字符，因此在匹配的回文中也要取 $b_{suffix}$ 的后 $m$ 个字符。然后就要看剩下的 $a$ 或 $b$ **正中心**的 $n -2m$ 个字符是否是回文串。

回文串分两种：
- 奇数长度，中间的字母不用管，两侧的各自相等
- 偶数长度，没有中间的字母，两侧的各自相等
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303181813904.png)

`x, y` 字符串长度相同，令中心扩展检测回文串函数为 `check(x, y, left)` ，它从 `x[left]` 往左扩张、从 `y[right]` 往右扩展，然后彼此匹配。**这里的 `left` 我们传进去 `x.size() / 2 - 1` ，偶数时为左侧的中心、奇数时为中心左边那个位置**，`right = x.size() - left - 1` 为与中心对称的位置。

我们调用 `check(a, a, left), check(b, b, left)` 从中心向两侧分别检测字符串 $a$ 和 $b$ ，不断扩展，直到字符不相同。然后看 $a, b$ 哪个的中心回文串更长，具体是哪条更长不重要。

再以具有更长中心回文串的那个字符串为标准，将两个字符串前后缀分别拼接，并继续扩展检测，这次检测拼接后的字符串。即调用 `check(a, b, left), check(b, a, left)` ，这里的 `left` 是之前遍历结束的位置。

当再次结束检测时，如字符再次不相同，则匹配失败；如全部匹配成功，则 `left` 应该为-1。

结合下图观看，第一次检测时，字符串 $a$ 的中心并没有回文串，而字符串 $b$ 有一段合法回文串；第二次检测时，$a_{prefix} + b_{suffix}$ 通过测试。最终，$a_{prefix}$ 、$b_{suffix}$ 和 $b$ 的中心子串组合起来，就是拼接后的回文串（所有有底色的字符）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303182041920.png)

```cpp
// 中心扩展
class Solution {
public:
    bool checkPalindromeFormation(string a, string b) {
        int n = a.size();
        function<int(string, string, int)> check = [&](const string &x, const string& y, int left) {
            int right = n - left - 1;
            while (left >= 0 && right < n && x[left] == y[right]) --left, ++right;
            return left;
        };
        int left = n / 2 - 1; // 中心位置
        left = min(check(a, a, left), check(b, b, left));
        left = min(check(a, b, left), check(b, a, left));
        return left == -1;
    }
}
```
这种写法与第一种写法相反，是**相反双指针**。