 >本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given a personal information string `s`, representing either an **email address** or a **phone number**. Return _the **masked** personal information using the below rules_.

**Email address:** An email address is:
-   A **name** consisting of uppercase and lowercase English letters, followed by
-   The `'@'` symbol, followed by
-   The **domain** consisting of uppercase and lowercase English letters with a dot `'.'` somewhere in the middle (not the first or last character).

To mask an email:
-   The uppercase letters in the **name** and **domain** must be converted to lowercase letters.
-   The middle letters of the **name** (i.e., all but the first and last letters) must be replaced by 5 asterisks `"*****"`.

**Phone number:** A phone number is formatted as follows:
-   The phone number contains 10-13 digits.
-   The last 10 digits make up the **local number**.
-   The remaining 0-3 digits, in the beginning, make up the **country code**.
-   **Separation characters** from the set `{'+', '-', '(', ')', ' '}` separate the above digits in some way.

To mask a phone number:
-   Remove all **separation characters**.
-   The masked phone number should have the form:
    -   `"***-***-XXXX"` if the country code has 0 digits.
    -   `"+*-***-***-XXXX"` if the country code has 1 digit.
    -   `"+**-***-***-XXXX"` if the country code has 2 digits.
    -   `"+***-***-***-XXXX"` if the country code has 3 digits.
-   `"XXXX"` is the last 4 digits of the **local number**.

**Example 1:**
```java
Input: s = "LeetCode@LeetCode.com"
Output: "l*****e@leetcode.com"
Explanation: s is an email address.
The name and domain are converted to lowercase, and the middle of the name is replaced by 5 asterisks.
```
**Example 2:**
```java
Input: s = "AB@qq.com"
Output: "a*****b@qq.com"
Explanation: s is an email address.
The name and domain are converted to lowercase, and the middle of the name is replaced by 5 asterisks.
Note that even though "ab" is 2 characters, it still must have 5 asterisks in the middle.
```
**Example 3:**
```java
Input: s = "1(234)567-890"
Output: "***-***-7890"
Explanation: s is a phone number.
There are 10 digits, so the local number is 10 digits and the country code is 0 digits.
Thus, the resulting masked number is "***-***-7890".
```
**Constraints:**
-   `s` is either a **valid** email or a phone number.
-   If `s` is an email:
    -   `8 <= s.length <= 40`
    -   `s` consists of uppercase and lowercase English letters and exactly one `'@'` symbol and `'.'` symbol.
-   If `s` is a phone number:
    -   `10 <= s.length <= 20`
    -   `s` consists of digits, spaces, and the symbols `'('`, `')'`, `'-'`, and `'+'`.

题意：返回按照题目规则、隐藏个人信息后的电子邮件地址或电话号码。

---
### 解法 模拟
首先判断 $s$ 是邮箱还是电话号码。显然，如果 $s$ 中有字符 `@` ，那么它是邮箱，否则它是电话号码。

如果 $s$ 是邮箱，我们将 $s$ 的 `@` 之前的部分保留第一个和最后一个字符，中间用 `*****` 代替，并将整个字符串转换为小写。

如果 $s$ 是电话号码，**我们只保留 $s$ 中的所有数字**，**用 `regex_replace` 将符合匹配规则的子串替换为其他字符串**。再将最后 $10$ 位本地号码变成 `***-***-XXXX` 的形式，并判断 $s$ 中是否有额外的国际号码。如果有，则将国际号码之前添加 `+` 号并加到本地号码的最前端，具体讨论如下：
- 如果有 $10$ 位数字，则加上前缀位空字符串。
- 如果有 $11$ 位数字，则加上前缀 `+*-` 。
- 如果有 $12$ 位数字，则不加上前缀 `+**-` 。
- 如果有 $13$ 位数字，则不加上前缀 `+***-` 。
 
```cpp
class Solution {
private:
    vector<string> country = {"", "+*-", "+**-", "+***-"};
public:
    string maskPII(string s) {
        size_t idx = s.find('@'); // 有就是邮箱
        if (idx != string::npos) {
            transform(s.begin(), s.end(), s.begin(), ::tolower); // 全部小写
            return s.substr(0, 1) + "*****" + s.substr(idx - 1);
            // 名字第一个字母,中间字母用*****替换,名字最后一个字母和后面的域名不变
        }
        s = regex_replace(s, regex("[^0-9]"), ""); // 非数字字符替换为空
        return country[s.size() - 10] + "***-***-" + s.substr(s.size() - 4);
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是字符串的长度。
- 空间复杂度：$O(n)$ ，其中 $n$ 是字符串的长度。

