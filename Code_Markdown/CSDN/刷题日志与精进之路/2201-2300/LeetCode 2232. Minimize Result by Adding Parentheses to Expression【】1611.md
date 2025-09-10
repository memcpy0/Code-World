> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的字符串 `expression` ，格式为 `"<num1>+<num2>"` ，其中 `<num1>` 和 `<num2>` 表示正整数。

请你向 `expression` 中添加一对括号，使得在添加之后， `expression` 仍然是一个有效的数学表达式，并且计算后可以得到 **最小** 可能值。左括号 **必须** 添加在 `'+'` 的左侧，而右括号必须添加在 `'+'` 的右侧。

返回添加一对括号后形成的表达式 `expression` ，且满足 `expression` 计算得到 **最小** 可能值_。_如果存在多个答案都能产生相同结果，返回任意一个答案。

生成的输入满足：`expression` 的原始值和添加满足要求的任一对括号之后 `expression` 的值，都符合 32-bit 带符号整数范围。

**示例 1：**

**输入：**expression = "247+38"
**输出：**"2(47+38)"
**解释：**表达式计算得到 2 * (47 + 38) = 2 * 85 = 170 。
注意 "2(4)7+38" 不是有效的结果，因为右括号必须添加在 `'+' 的右侧。`
可以证明 170 是最小可能值。

**示例 2：**

**输入：**expression = "12+34"
**输出：**"1(2+3)4"
**解释：**表达式计算得到 1 * (2 + 3) * 4 = 1 * 5 * 4 = 20 。

**示例 3：**

**输入：**expression = "999+999"
**输出：**"(999+999)"
**解释：**表达式计算得到 999 + 999 = 1998 。

**提示：**

- `3 <= expression.length <= 10`
- `expression` 仅由数字 `'1'` 到 `'9'` 和 `'+'` 组成
- `expression` 由数字开始和结束
- `expression` 恰好仅含有一个 `'+'`.
- `expression` 的原始值和添加满足要求的任一对括号之后 `expression` 的值，都符合 32-bit 带符号整数范围

```cpp
class Solution {
public:
    string minimizeResult(string expression) {
        int pos = 0, n = expression.size();
        while (pos < n && expression[pos] != '+') ++pos;
        int mn = INT_MAX;
        string ans;
        for (int i = pos; i >= 1; --i) {
            string sp = (i == pos) ? "" : expression.substr(0, pos - i);
            int p = (i == pos) ? 1 : stoi(sp);
            string sa = expression.substr(pos - i, i);
            int ta = stoi(sa);

            for (int j = n - pos - 1; j >= 1; --j) {
                string sb = expression.substr(pos + 1, j);
                int tb = stoi(sb);

                string sq = (j == n - pos - 1) ? "" : expression.substr(pos + 1 + j);
                int q = (j == n - pos - 1) ? 1 : stoi(sq);
                int t = p * (ta + tb) * q;

                if (t < mn) {
                    mn = t;
                    ans = sp + "(" + sa + "+" + sb + ")" + sq;
                }
            }
        }
        return ans;
    }
};
```