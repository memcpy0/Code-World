> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的字符串 `details` 。`details` 中每个元素都是一位乘客的信息，信息用长度为 `15` 的字符串表示，表示方式如下：
- 前十个字符是乘客的手机号码。
- 接下来的一个字符是乘客的性别。
- 接下来两个字符是乘客的年龄。
- 最后两个字符是乘客的座位号。

请你返回乘客中年龄 **严格大于 60 岁** 的人数。

**示例 1：**
```js
输入：details = ["7868190130M7522","5303914400F9211","9273338290F4010"]
输出：2
解释：下标为 0 ，1 和 2 的乘客年龄分别为 75 ，92 和 40 。所以有 2 人年龄大于 60 岁。
```
**示例 2：**
```js
输入：details = ["1313579440F2036","2921522980M5644"]
输出：0
解释：没有乘客的年龄大于 60 岁。
```
**提示：**
- `1 <= details.length <= 100`
- `details[i].length == 15`
- `details[i]` 中的数字只包含 `'0'` 到 `'9'` 。
- `details[i][10]` 是 `'M'` ，`'F'` 或者 `'O'` 之一。
- 所有乘客的手机号码和座位号互不相同。

---
### 解法 遍历
```cpp
class Solution {
public:
    int countSeniors(vector<string>& details) {
        return count_if(details.begin(), details.end(), [&](const string &a) {
            return a.substr(11, 2) > "60";
        });
    }
};
```