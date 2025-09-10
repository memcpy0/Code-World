> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

现在有一个特殊的排名系统，依据参赛团队在投票人心中的次序进行排名，每个投票者都需要按从高到低的顺序对参与排名的所有团队进行排位。

排名规则如下：

- 参赛团队的排名次序依照其所获「排位第一」的票的多少决定。如果存在多个团队并列的情况，将继续考虑其「排位第二」的票的数量。以此类推，直到不再存在并列的情况。
- 如果在考虑完所有投票情况后仍然出现并列现象，则根据团队字母的字母顺序进行排名。

给你一个字符串数组 `votes` 代表全体投票者给出的排位情况，请你根据上述排名规则对所有参赛团队进行排名。

请你返回能表示按排名系统 **排序后** 的所有团队排名的字符串。

**示例 1：**

**输入：**votes = ["ABC","ACB","ABC","ACB","ACB"]
**输出：**"ACB"
**解释：**A 队获得五票「排位第一」，没有其他队获得「排位第一」，所以 A 队排名第一。
B 队获得两票「排位第二」，三票「排位第三」。
C 队获得三票「排位第二」，两票「排位第三」。
由于 C 队「排位第二」的票数较多，所以 C 队排第二，B 队排第三。

**示例 2：**

**输入：**votes = ["WXYZ","XYZW"]
**输出：**"XWYZ"
**解释：**X 队在并列僵局打破后成为排名第一的团队。X 队和 W 队的「排位第一」票数一样，但是 X 队有一票「排位第二」，而 W 没有获得「排位第二」。 

**示例 3：**

**输入：**votes = ["ZMNAGUEDSJYLBOPHRQICWFXTVK"]
**输出：**"ZMNAGUEDSJYLBOPHRQICWFXTVK"
**解释：**只有一个投票者，所以排名完全按照他的意愿。

**示例 4：**

**输入：**votes = ["BCA","CAB","CBA","ABC","ACB","BAC"]
**输出：**"ABC"
**解释：** 
A 队获得两票「排位第一」，两票「排位第二」，两票「排位第三」。
B 队获得两票「排位第一」，两票「排位第二」，两票「排位第三」。
C 队获得两票「排位第一」，两票「排位第二」，两票「排位第三」。
完全并列，所以我们需要按照字母升序排名。

**示例 5：**

**输入：**votes = ["M","M","M","M"]
**输出：**"M"
**解释：**只有 M 队参赛，所以它排名第一。

**提示：**

- `1 <= votes.length <= 1000`
- `1 <= votes[i].length <= 26`
- `votes[i].length == votes[j].length` for `0 <= i, j < votes.length`
- `votes[i][j]` 是英文 **大写** 字母
- `votes[i]` 中的所有字母都是唯一的
- `votes[0]` 中出现的所有字母 **同样也** 出现在 `votes[j]` 中，其中 `1 <= j < votes.length`

```cpp
class Solution {
private:
    struct Rec {
        char c;
        bool vis;
        vector<int> cnt;
        Rec() : cnt(26), vis(false) {}
    };
public:
    string rankTeams(vector<string>& votes) {
        if (votes.size() == 1) return votes[0];
        Rec rec[26];
        for (int i = 0; i < 26; ++i) rec[i].c = 'A' + i;   
        int m = votes.size(), n = votes[0].size();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                int p = votes[j][i] - 'A';
                ++rec[p].cnt[i];
                rec[p].vis = true;
            }
        }
        sort(rec, rec + 26, [&](const Rec &a, const Rec &b) {
            for (int i = 0; i < 26; ++i) 
                if (a.cnt[i] != b.cnt[i]) return a.cnt[i] > b.cnt[i];
            return a.c < b.c;
        });
        string ans;
        for (int i = 0; i < 26; ++i) if (rec[i].vis) ans.push_back(rec[i].c);
        return ans;
    }
};
```