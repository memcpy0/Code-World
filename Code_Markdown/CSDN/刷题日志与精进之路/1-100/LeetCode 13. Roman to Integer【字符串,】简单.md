> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091342909.png)

罗马数字包含以下七种字符: `I`， `V`， `X`， `L`，`C`，`D` 和 `M`。
```java
字符          数值
I             1
V             5
X             10
L             50
C             100
D             500
M             1000
```
例如， 罗马数字 `2` 写做 `II` ，即为两个并列的 1 。`12` 写做 `XII` ，即为 `X` + `II` 。 `27` 写做  `XXVII`, 即为 `XX` + `V` + `II` 。

通常情况下，罗马数字中小的数字在大的数字的右边。但也存在特例，例如 4 不写做 `IIII`，而是 `IV`。数字 1 在数字 5 的左边，所表示的数等于大数 5 减小数 1 得到的数值 4 。同样地，数字 9 表示为 `IX`。这个特殊的规则只适用于以下六种情况：
- `I` 可以放在 `V` (5) 和 `X` (10) 的左边，来表示 4 和 9。
- `X` 可以放在 `L` (50) 和 `C` (100) 的左边，来表示 40 和 90。 
- `C` 可以放在 `D` (500) 和 `M` (1000) 的左边，来表示 400 和 900。

给定一个罗马数字，将其转换成整数。

**示例 1:**
```java
输入: s = "III"
输出: 3
```
**示例 2:**
```java
输入: s = "IV"
输出: 4
```
**示例 3:**
```java
输入: s = "IX"
输出: 9
```
**示例 4:**
```java
输入: s = "LVIII"
输出: 58
解释: L = 50, V= 5, III = 3.
```
**示例 5:**
```java
输入: s = "MCMXCIV"
输出: 1994
解释: M = 1000, CM = 900, XC = 90, IV = 4.
```
**提示：**
- `1 <= s.length <= 15`
- `s` 仅含字符 `('I', 'V', 'X', 'L', 'C', 'D', 'M')`
- 题目数据保证 `s` 是一个有效的罗马数字，且表示整数在范围 `[1, 3999]` 内
- 题目所给测试用例皆符合罗马数字书写规则，不会出现跨位等情况。
- IL 和 IM 这样的例子并不符合题目要求，49 应该写作 XLIX，999 应该写作 CMXCIX 。
- 关于罗马数字的详尽书写规则，可以参考 [罗马数字 - Mathematics](https://b2b.partcommunity.com/community/knowledge/zh_CN/detail/10753/%E7%BD%97%E9%A9%AC%E6%95%B0%E5%AD%97#knowledge_article) 。

---
### 解法 遍历字符串
在[12. Integer to Roman的博客]( https://memcpy0.blog.csdn.net/article/details/116866679)中已经写清楚了罗马数字的规则。所以本题很简单。
```cpp
class Solution {
public:
    int romanToInt(string s) {
        unordered_map<string, int> rec = {
            {"I", 1}, {"IV", 4}, {"V", 5}, {"IX", 9}, {"X", 10}, 
            {"XL", 40}, {"L", 50}, {"XC", 90}, {"C", 100}, 
            {"CD", 400}, {"D", 500}, {"CM", 900}, {"M", 1000}
        };
        int ans = 0, n = s.size();
        for (int i = 0; i < n; ) {
            string &&str = s.substr(i, 2);
            if (rec.find(str) != rec.end()) { ans += rec[str]; i += 2; }
            else { ans += rec[s.substr(i, 1)]; ++i; }
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：32 ms, 在所有 C++ 提交中击败了15.14% 的用户
内存消耗：12.6 MB, 在所有 C++ 提交中击败了9.55% 的用户
```
以前我虽然不是很了解罗马数字的规则，但是也做出了这道题。下面是我以前的代码，其实和上面的代码是等价的，而且运行效率更高：
```cpp

class Solution {
public:
    int romanToInt(string s) {
        auto it = s.begin();
        int res = 0;
        while (it != s.end()) {
            switch (*it) {
                case 'I':   
                    if (*(it + 1) == 'V') res += 4, it += 2;
                    else if (*(it + 1) == 'X') res += 9, it += 2; 
                    else ++res, ++it;
                    break;
                case 'V': res += 5; ++it; break;
                case 'X': 
                    if (*(it + 1) == 'L') res += 40, it += 2;
                    else if (*(it + 1) == 'C') res += 90, it += 2;
                    else res += 10, ++it;
                    break;
                case 'L': res += 50; ++it; break;
                case 'C':
                    if (*(it + 1) == 'D') res += 400, it += 2;
                    else if (*(it + 1) == 'M') res += 900, it +=2;
                    else res += 100, ++it;
                    break;
                case 'D': res += 500; ++it; break;
                case 'M': res += 1000; ++it; break;
            }        
        }
        return res;
    }
};
```
运行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了81.32% 的用户
内存消耗：5.9 MB, 在所有 C++ 提交中击败了68.31% 的用户
```

class Solution {
private:
    unordered_map<char, int> symbolValues = {
        {'I', 1},
        {'V', 5},
        {'X', 10},
        {'L', 50},
        {'C', 100},
        {'D', 500},
        {'M', 1000},
    };

public:
    int romanToInt(string s) {
        int ans = 0;
        int n = s.length();
        for (int i = 0; i < n; ++i) {
            int value = symbolValues[s[i]];
            if (i < n - 1 && value < symbolValues[s[i + 1]]) {
                ans -= value;
            } else {
                ans += value;
            }
        }
        return ans;
    }
};

**复杂度分析**

- 时间复杂度：O(n)O(n)O(n)，其中 nnn 是字符串 sss 的长度。
    
- 空间复杂度：O(1)O(1)O(1)。