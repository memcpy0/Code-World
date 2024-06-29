> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091337298.png)

将一个给定字符串 `s` 根据给定的行数 `numRows` ，以从上往下、从左到右进行 Z 字形排列。

比如输入字符串为 `"PAYPALISHIRING"` 行数为 `3` 时，排列如下：
```
P   A   H   N
A P L S I I G
Y   I   R
```
之后，你的输出需要从左往右逐行读取，产生出一个新的字符串，比如：`"PAHNAPLSIIGYIR"`。

请你实现这个将字符串进行指定行数变换的函数：
```java
string convert(string s, int numRows);
```
**示例 1：**
```java
输入：s = "PAYPALISHIRING", numRows = 3
输出："PAHNAPLSIIGYIR"
```
**示例 2：**
```java
输入：s = "PAYPALISHIRING", numRows = 4
输出："PINALSIGYAHRPI"
解释：
P     I    N
A   L S  I G
Y A   H R
P     I
```
**示例 3：**
```java
输入：s = "A", numRows = 1
输出："A"
```
**提示：**
- `1 <= s.length <= 1000`
- `s` 由英文字母（小写和大写）、`','` 和 `'.'` 组成
- `1 <= numRows <= 1000`

---
方法一：利用二维矩阵模拟
设 nnn 为字符串 sss 的长度，r=numRowsr=\textit{numRows}r=numRows。对于 r=1r=1r=1（只有一行）或者 r≥nr\ge nr≥n（只有一列）的情况，答案与 sss 相同，我们可以直接返回 sss。对于其余情况，考虑创建一个二维矩阵，然后在矩阵上按 Z 字形填写字符串 sss，最后逐行扫描矩阵中的非空字符，组成答案。

根据题意，当我们在矩阵上填写字符时，会向下填写 rrr 个字符，然后向右上继续填写 r−2r-2r−2 个字符，最后回到第一行，因此 Z 字形变换的周期 t=r+r−2=2r−2t=r+r-2=2r-2t=r+r−2=2r−2，每个周期会占用矩阵上的 1+r−2=r−11+r-2=r-11+r−2=r−1 列。

因此我们有 ⌈nt⌉\Big\lceil\dfrac{n}{t}\Big\rceil⌈ 
t
n
​
 ⌉ 个周期（最后一个周期视作完整周期），乘上每个周期的列数，得到矩阵的列数 c=⌈nt⌉⋅(r−1)c=\Big\lceil\dfrac{n}{t}\Big\rceil\cdot(r-1)c=⌈ 
t
n
​
 ⌉⋅(r−1)。

创建一个 rrr 行 ccc 列的矩阵，然后遍历字符串 sss 并按 Z 字形填写。具体来说，设当前填写的位置为 (x,y)(x,y)(x,y)，即矩阵的 xxx 行 yyy 列。初始 (x,y)=(0,0)(x,y)=(0,0)(x,y)=(0,0)，即矩阵左上角。若当前字符下标 iii 满足 i mod t<r−1i\bmod t<r-1imodt<r−1，则向下移动，否则向右上移动。

填写完成后，逐行扫描矩阵中的非空字符，组成答案。

Python3
C++
Java
C#
Golang
C
JavaScript
class Solution {
public:
    string convert(string s, int numRows) {
        int n = s.length(), r = numRows;
        if (r == 1 || r >= n) {
            return s;
        }
        int t = r * 2 - 2;
        int c = (n + t - 1) / t * (r - 1);
        vector<string> mat(r, string(c, 0));
        for (int i = 0, x = 0, y = 0; i < n; ++i) {
            mat[x][y] = s[i];
            if (i % t < r - 1) {
                ++x; // 向下移动
            } else {
                --x;
                ++y; // 向右上移动
            }
        }
        string ans;
        for (auto &row : mat) {
            for (char ch : row) {
                if (ch) {
                    ans += ch;
                }
            }
        }
        return ans;
    }
};
复杂度分析

时间复杂度：O(r⋅n)O(r\cdot n)O(r⋅n)，其中 r=numRowsr=\textit{numRows}r=numRows，nnn 为字符串 sss 的长度。时间主要消耗在矩阵的创建和遍历上，矩阵的行数为 rrr，列数可以视为 O(n)O(n)O(n)。

空间复杂度：O(r⋅n)O(r\cdot n)O(r⋅n)。矩阵需要 O(r⋅n)O(r\cdot n)O(r⋅n) 的空间。

方法二：压缩矩阵空间
方法一中的矩阵有大量的空间没有被使用，能否优化呢？

注意到每次往矩阵的某一行添加字符时，都会添加到该行上一个字符的右侧，且最后组成答案时只会用到每行的非空字符。因此我们可以将矩阵的每行初始化为一个空列表，每次向某一行添加字符时，添加到该行的列表末尾即可。

Python3
C++
Java
C#
Golang
C
JavaScript
class Solution {
public:
    string convert(string s, int numRows) {
        int n = s.length(), r = numRows;
        if (r == 1 || r >= n) {
            return s;
        }
        vector<string> mat(r);
        for (int i = 0, x = 0, t = r * 2 - 2; i < n; ++i) {
            mat[x] += s[i];
            i % t < r - 1 ? ++x : --x;
        }
        string ans;
        for (auto &row : mat) {
            ans += row;
        }
        return ans;
    }
};
复杂度分析

时间复杂度：O(n)O(n)O(n)。

空间复杂度：O(n)O(n)O(n)。压缩后的矩阵需要 O(n)O(n)O(n) 的空间。

方法三：直接构造
我们来研究方法一中矩阵的每个非空字符会对应到 sss 的哪个下标（记作 idx\textit{idx}idx），从而直接构造出答案。

由于 Z 字形变换的周期为 t=2r−2t=2r-2t=2r−2，因此对于矩阵第一行的非空字符，其对应的 idx\textit{idx}idx 均为 ttt 的倍数，即 idx≡0(modt)\textit{idx}\equiv 0\pmod tidx≡0(modt)；同理，对于矩阵最后一行的非空字符，应满足 idx≡r−1(modt)\textit{idx}\equiv r-1\pmod tidx≡r−1(modt)。

对于矩阵的其余行（行号设为 iii），每个周期内有两个字符，第一个字符满足 idx≡i(modt)\textit{idx}\equiv i\pmod tidx≡i(modt)，第二个字符满足 idx≡t−i(modt)\textit{idx}\equiv t-i\pmod tidx≡t−i(modt)。

Python3
C++
Golang
C
JavaScript
class Solution {
public:
    string convert(string s, int numRows) {
        int n = s.length(), r = numRows;
        if (r == 1 || r >= n) {
            return s;
        }
        string ans;
        int t = r * 2 - 2;
        for (int i = 0; i < r; ++i) { // 枚举矩阵的行
            for (int j = 0; j + i < n; j += t) { // 枚举每个周期的起始下标
                ans += s[j + i]; // 当前周期的第一个字符
                if (0 < i && i < r - 1 && j + t - i < n) {
                    ans += s[j + t - i]; // 当前周期的第二个字符
                }
            }
        }
        return ans;
    }
};
复杂度分析

时间复杂度：O(n)O(n)O(n)，其中 nnn 为字符串 sss 的长度。sss 中的每个字符仅会被访问一次，因此时间复杂度为 O(n)O(n)O(n)。

空间复杂度：O(1)O(1)O(1)。返回值不计入空间复杂度。

 
```cpp
class Solution {
public:
    string convert(string s, int numRows) {
        int n = s.size();
        int m = (numRows - 1) * 2;
        if (numRows == 1 || s.size() == 1) return s;
        string ans;
        for (int i = 0; i < numRows; ++i) {
            int t = m;
            if (i != 0 && i != numRows - 1) t = m - (i * 2);
            for (int j = i; j < n; ) {
                ans += s[j];
                j += t;
                t = (t == m ? m : m - t);
            }
        }
        return ans;
    }
};
```