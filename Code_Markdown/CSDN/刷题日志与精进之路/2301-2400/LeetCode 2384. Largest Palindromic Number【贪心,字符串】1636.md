> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


给你一个仅由数字（`0 - 9`）组成的字符串 `num` 。

请你找出能够使用 `num` 中数字形成的 **最大回文** 整数，并以字符串形式返回。该整数不含 **前导零** 。

**注意：**

- 你 **无需** 使用 `num` 中的所有数字，但你必须使用 **至少** 一个数字。
- 数字可以重新排序。

**示例 1：**

**输入：**num = "444947137"
**输出：**"7449447"
**解释：**
从 "_**44494**__**7**_13_**7**_" 中选用数字 "4449477"，可以形成回文整数 "7449447" 。
可以证明 "7449447" 是能够形成的最大回文整数。

**示例 2：**

**输入：**num = "00009"
**输出：**"9"
**解释：**
可以证明 "9" 能够形成的最大回文整数。
注意返回的整数不应含前导零。

**提示：**

- `1 <= num.length <= 10^5`
- `num` 由数字（`0 - 9`）组成

```cpp
class Solution {
public:
    string largestPalindromic(string num) { 
        int cnt[10];
        memset(cnt, 0, sizeof(cnt));
        for (char c : num) ++cnt[c - '0'];
        deque<string> s, t;
        char c;
        bool first = false, nonZero = false;
        int lead = 0;
        for (int i = 9; i > 0; --i) {
            if (cnt[i] == 0) continue; 
            nonZero = true;
            // 最大的次数 
            int tcnt = cnt[i] >> 1;
            lead += cnt[i] >> 1;
            s.push_back(string(tcnt, '0' + i));
            t.push_front(string(tcnt, '0' + i));
            if (first == false && (cnt[i] & 1)) {
                c = '0' + i;
                first = true;
            }
            // 是偶数就全部使用，放在两边
            // 是奇数也全部使用，放在两边+中间
        }
        if (!nonZero) return "0";
        string ans;
        while (!s.empty()) { ans += s.front(); s.pop_front(); }
        if (lead) ans += string(cnt[0] >> 1, '0');
        if (first) ans += c;
        else if (cnt[0] & 1) ans += '0'; // 注意0为奇数,且其他不为奇数次
        if (lead) ans += string(cnt[0] >> 1, '0');
        while (!t.empty()) { ans += t.front(); t.pop_front(); }
        return ans;
    }
};
```