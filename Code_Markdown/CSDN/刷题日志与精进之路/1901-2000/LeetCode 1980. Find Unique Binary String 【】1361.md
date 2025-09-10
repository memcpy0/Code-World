> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个字符串数组 `nums` ，该数组由 `n` 个 **互不相同** 的二进制字符串组成，且每个字符串长度都是 `n` 。请你找出并返回一个长度为 `n` 且 **没有出现** 在 `nums` 中的二进制字符串_。_如果存在多种答案，只需返回 **任意一个** 即可。

**示例 1：**

**输入：**nums = ["01","10"]
**输出：**"11"
**解释：**"11" 没有出现在 nums 中。"00" 也是正确答案。

**示例 2：**

**输入：**nums = ["00","01"]
**输出：**"11"
**解释：**"11" 没有出现在 nums 中。"10" 也是正确答案。

**示例 3：**

**输入：**nums = ["111","011","001"]
**输出：**"101"
**解释：**"101" 没有出现在 nums 中。"000"、"010"、"100"、"110" 也是正确答案。

**提示：**

- `n == nums.length`
- `1 <= n <= 16`
- `nums[i].length == n`
- `nums[i]` 为 `'0'` 或 `'1'`
- `nums` 中的所有字符串 **互不相同**

```cpp
class Solution {
public:
    string findDifferentBinaryString(vector<string>& nums) {
        bool vis[65536];
        memset(vis, false, sizeof(vis));
        for (string &s : nums) {
            int v = 0;
            for (char &c : s) v = (v << 1) + (c - '0');
            // cout << v << endl;
            vis[v] = true;
        }
        int n = nums.size();
        for (int i = 0; i < 65536; ++i) {
            if (vis[i] == false) {
                string s;
                int tmp = i;
                do {
                    s.push_back('0' + (tmp & 1));
                    tmp >>= 1;
                } while (tmp || s.size() < n);
                reverse(s.begin(), s.end());
                return s;
            }
        }
        return "";
    }
};
```