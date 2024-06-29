> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

> 相关公司：小红书、谷歌 Google、彭博 Bloomberg、微软 Microsoft、亚马逊、字节跳动、J.P Morgan 摩根大通、优步 Uber

You are given a string `text`. You should split it to k substrings `(subtext1, subtext2, ..., subtextk)` such that:
-   `subtexti` is a **non-empty** string.
-   The concatenation of all the substrings is equal to `text` (i.e., `subtext1 + subtext2 + ... + subtextk == text`).
-   `subtexti == subtextk - i + 1` for all valid values of `i` (i.e., `1 <= i <= k`).

Return the largest possible value of `k`.

**Example 1:**
```java
Input: text = "ghiabcdefhelloadamhelloabcdefghi"
Output: 7
Explanation: We can split the string on "(ghi)(abcdef)(hello)(adam)(hello)(abcdef)(ghi)".
```
**Example 2:**
```java
Input: text = "merchant"
Output: 1
Explanation: We can split the string on "(merchant)".
```
**Example 3:**
```java
Input: text = "antaprezatepzapreanta"
Output: 11
Explanation: We can split the string on "(a)(nt)(a)(pre)(za)(tep)(za)(pre)(a)(nt)(a)".
```
**Constraints:**
-   `1 <= text.length <= 1000`
-   `text` consists only of lowercase English characters.

题意：你得到一个字符串 `text` 。应该把它分成 `k` 个子字符串 `(subtext1, subtext2，…， subtextk)` ，要求满足:
-   `subtexti` 是 **非空** 字符串
-   所有子字符串的连接等于 `text` ( 即`subtext1 + subtext2 + ... + subtextk == text` )
-   对于所有 `i` 的有效值( 即 `1 <= i <= k` ) ，`subtexti == subtextk - i + 1` 均成立

返回 `k` 可能最大值。

---
### 解法1 贪心+双指针
解决段式回文问题，我们可以设置两个指针 `left, right` 分别指向 `text` 的首尾、作为前缀子串的开始指针、后缀子串的结束指针。然后令 `l = left, r = right` ，再不断将 `r` 指针从后往前移动，一旦碰到 `text[r] == text[l]` ，就说明可能碰到相等的前后缀子串。

我们在里面再用一重循环，令 `i = l, j = r` ，将从 `l` 开始的前缀和从 `r` 开始的后缀匹配：
- **如果有一处字符不同、或者 `j` 没有遍历到后缀 `right` 处，说明不是相等的前后缀子串**，需要跳出最里面的循环，并 `--r` ，继续寻找前面的相等字符。这种情况下，如果始终没有找到相等的前后缀子串，就会让 `l == r` ，说明中间只有这一个子串，作为段式回文的中心，我们令 `ans++` ，就返回 `ans` 作为结果。
- **如果最里层的循环结束后，发现前后缀子串相等（此时 `j > right` ）**，就令答案 `ans += 2` ，并后移 `left = i` 、前移 `right = r - 1` ，跳出上一层循环，并接着寻找相等的前后缀子串。这种情况下 `l` 不会等于 `r` 。

```java
class Solution {
    public int longestDecomposition(String text) {
        int ans = 0;
        int left = 0, right = text.length() - 1;
        while (left <= right) {
            int l = left, r = right;
            while (l < r) { // 扫描从后往前的相同字符
                if (text.charAt(l) != text.charAt(r)) --r; // 继续往前找,text[l]==text[r]
                else {
                    int i = l, j = r; // [i,...] 和 [j,...]比较
                    boolean flag = true;
                    for (; i < r && j <= right; ++i, ++j) {
                        if (text.charAt(i) != text.charAt(j)) {
                            flag = false;
                            break;
                        }
                    }
                    if (flag && j > right) {
                        ans += 2;
                        left = i;
                        right = r - 1;
                        break; // 这里出来的l一定不等于r
                    } else --r; // 接着找上一个text[r]==text[l]
                }
            }
            if (l == r) { // 说明没找到一个text[r]==text[l],这就是一个字符串
                ++ans;
                break;
            }
        }
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n^2)$
- 空间复杂度：$O(1)$

---
### 解法2 贪心+枚举前后缀子串长度（递归/迭代）
我们不直接使用双指针，而是枚举前后缀长度 $1, 2, 3,\dots$ ，和上种解法一样，**一旦找到相同的前后缀，就直接切分为子串**。 
```java
(abc)zabc ... abcz(abc)
```
为什么不继续寻找？这就是证明贪心选择性质正确的地方：==如果已经找到短的相同前后缀，对于更长的相同前后缀，它一定能分出更多的子串==。如下所示，短的满足1=4，长的满足1=3且2=4，则2=3。
```java
abczabc ... abczabc
(abc)(z)(abc) ... (abc)(z)(abc)
  1       2         3       4              
```
不过还有疑问：**「短的前/后缀子串」的长度会超过「长的前/后缀子串」长度的一半**吗？如果会这样的话，我们不可能把字符串分为两段！
> 这是不可能的。反证法证明：假设短的长度超过了长的长度的一半，则上面的证明还可得到：1=2，结合前面的假设，说明1和2有重叠，重叠部分既是短前缀的前缀，也是其后缀。
> <b></b>
> 这么说很抽象，举个例子：`ababa ... ababa` 中1=2= `aba` ，重叠部分为 `a` ，既是 `aba` 的前缀，也是 `aba` 的后缀，这说明短的 `aba` 还可继续分割出更短的子串 `a` ，而不是作为一个无法分割的整体，矛盾。所以不会出现短的长度超过长的长度的一半的情况。

代码如下，可以递归求解：
```cpp
class Solution {
public:
	int longestDecomposition(string s) {
		if (s.empty()) return 0;
		for (int i = 1, n = s.length(); i <= n / 2; ++i) // 枚举前后缀长度
			if (s.substr(0, i) == s.substr(n - i)) // 立刻分割
				return 2 + longestDecomposition(s.substr(i, n - i * 2));
		return 1; // 无法分割
	}
};
```
迭代形式如下：
```cpp
class Solution {
public:
	int longestDecomposition(string s) {
		int ans = 0;
		while (!s.empty()) {
			int i = 1, n = s.length();
			while (i <= n / 2 && s.substr(0, i) != s.substr(n - i)) // 枚举前后缀
				++i;
			if (i > n / 2) { // 无法分割
				++ans;
				break;
			}
			ans += 2; // 分割出s[:i]和s[n-i:]
			s = s.substr(i, n - i * 2);
		}
		return ans;
	}
};
```
复杂度分析：
- 时间复杂度：$O(n^2)$ 。最坏情况下无法分割，需要执行 $O(n)$ 次长为 $O(n)$ 的字符串比较，所以时间复杂度为 $O(n^2)$ 。
- 空间复杂度：$O(n)$ 。

---
### 解法3 贪心+字符串哈希+滚动哈希
只要选定合适的基数、令不同字符串的哈希值不同，就可对字符串进行哈希。相等的前/后缀子串计算得到的哈希值也相等，一旦发现相等的哈希值，就立刻分割子串（贪心），并重置已得的哈希值。

如下所示，设基数为 `x` ，分别从1的第一个字符 `a` 和4的最后一个字符 `c` 开始滚动哈希，得到1和4的哈希值后发现二者相等，则答案+2：
```java
(abc)(z)(abc) ... (abc)(z)(abc)
  1       2         3       4
1: abc=a*x^2+b*x+c=((0*x+a)*x+b)*x+c
```
代码如下：
```java
class Solution { 
	int base = 131;
	public int longestDecomposition(String s) {
		int n = s.length(), halfLen = n / 2;
		int h1 = 0, h2 = 0, temp = 1;
		int ans = 0;
		int maxi = 0;
		for (int i = 1; i <= halfLen ; ++i) {
			h1 = h1 * base + s.charAt(i - 1);
			h2 = h2 + s.charA(n - i) * temp;
			temp = temp * base;
			if (h1 == h2) {
				ans += 2;
				temp = 1;
				h1 = 0;
				h2 = 0;
				maxi = i;
			}
		}
		if (maxi == halfLen) ans = halfLen * 2 < halfLen ? ans + 1 : ans; // 到达的最大长度
		else ++ans; // 无法分割到中间,ans++
		return ans;
	}
}
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(1)$ 