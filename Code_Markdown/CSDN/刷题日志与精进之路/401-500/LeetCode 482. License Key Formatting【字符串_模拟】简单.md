> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given a license key represented as a string `S` which consists only **alphanumeric** character and **dashes**. The string is separated into `N+1` groups by `N` dashes.

Given a number `K` , we would want to reformat the strings such that each group contains exactly `K` characters, except for **the first group** which could be shorter than `K` , but still **must** contain at least one character. Furthermore, there must be a dash inserted between two groups and all lowercase letters should be converted to uppercase.

Given a non-empty string `S` and a number `K` , format the string according to the rules described above.

**Example 1:**
```swift
Input: S = "5F3Z-2e-9-w", K = 4
Output: "5F3Z-2E9W"

Explanation: The string S has been split into two parts, each part has 4 characters.
Note that the two extra dashes are not needed and can be removed.
```


**Example 2:**
```swift
Input: S = "2-5g-3-J", K = 2
Output: "2-5G-3J"

Explanation: The string S has been split into three parts, each part has 2 characters except the first part as it could be shorter as mentioned above.
```

**Constraints:**
<ul>
	<li><code>1 &lt;= s.length &lt;= 10<sup>5</sup></code></li>
	<li><code>s</code> consists of English letters, digits, and dashes <code>'-'</code>.</li>
	<li><code>1 &lt;= k &lt;= 10<sup>4</sup></code></li>
</ul>


> LeetCode上本题的英文题面有一定变化，变化主要集中于提示部分，原来的提示：
> **Note:**
> - The length of string `S` will not exceed `12,000` , and `K` is a positive integer.
> - String `S` consists only of alphanumerical characters ( `a-z` and/or `A-Z` and/or `0-9` ) and dashes( `-` ).
> - String `S` is non-empty.

题意：给出一个密钥字符串 `S` ，只包含字母、数字以及 `-`（破折号）。然后给出一个数字 `K` ，重新格式化字符串，使每个分组恰好包含 `K` 个字符。只有第一个分组包含的字符个数可以小于等于 `K` ，但至少要包含  `1` 个字符。分组之间需要用 `-` 隔开，并且将所有的小写字母转换为大写字母。
 
---
### 解法1 正序插入
先计算可以分成多少组，每组大小为 $k$ ，如果有多余的就分到第一组，组数加一。然后对每组按照该组字符的数量，依次分配 `s` 中的字母和数字。这一做法的时间复杂度为 $O(n)$ ，空间为 $O(n)$ 。
```cpp
//C++ version
class Solution {
private:
    void allocate(const string &s, string &t, int count, int &idx) {
        while (count--) {
            while (!isalnum(s[idx])) ++idx;
            t.push_back(toupper(s[idx++]));
        }
    }
public:
    string licenseKeyFormatting(string s, int k) { 
        int alnum = (s.size() - count(s.begin(), s.end(), '-')), firstGroupNum = alnum % k;
        int groups = alnum / k + (firstGroupNum != 0), idx = 0;
        string ans;
        for (int i = 0; i < groups; ++i) {
            if (i == 0 && firstGroupNum != 0) allocate(s, ans, firstGroupNum, idx); //分配第一组的字母或数字
            else allocate(s, ans, k, idx); //分配其他组的字母或数字
            if (i != groups - 1) ans.push_back('-'); // 知道组数
        }
        return ans;
    }
};
//执行用时：12 ms, 在所有 C++ 提交中击败了95.84% 的用户
//内存消耗：8 MB, 在所有 C++ 提交中击败了82.87% 的用户
```
更好的写法是，用一个 `num` 作为周期变量，和[达夫设备的实现](https://en.wikipedia.org/wiki/Duff's_device)有点相似，**先用完余数，再进行模数周期的变化**：
```cpp
//C++ version
class Solution { 
public:
    string licenseKeyFormatting(string s, int k) { 
    	// num=k-字母数字个数%k,
    	// num != k则num为(k-第一组大小),否则num = k
        int num = k - (s.size() - count(s.begin(), s.end(), '-')) % k; 
        string ans;
        for (const char &c : s) {
            if (c == '-') continue; 
            if (num == 0 && !ans.empty()) ans += '-'; // 不知道组数
            ans.push_back(toupper(s[i]));
            num = (num + 1) % k; // 从0到num-1不断循环
        }
        return ans;
    }
};
//执行用时：8 ms, 在所有 C++ 提交中击败了58.03% 的用户
//内存消耗：8.2 MB, 在所有 C++ 提交中击败了44.90% 的用户
```
---
### 解法2 逆序插入
从后往前处理，可以避免「**对首个分区的分情况讨论**」和「**取模操作**」。注意**添加 `-` 符号的时机**、全部使用大写字母等细节即可。
```cpp
//C++ version
class Solution {
public:
    string licenseKeyFormatting(string s, int k) { 
        string ans;
        for (int i = s.size() - 1, cnt = 0; i >= 0; --i) {
            if (s[i] == '-') continue;
            if (cnt == k) { // 不知道组数
                ans.push_back('-');
                cnt = 0;
            }
            ans.push_back(toupper(s[i])); // 全大写字母或数字
            ++cnt;
        }
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了93.86% 的用户
//内存消耗：8.1 MB, 在所有 C++ 提交中击败了54.56% 的用户
```
