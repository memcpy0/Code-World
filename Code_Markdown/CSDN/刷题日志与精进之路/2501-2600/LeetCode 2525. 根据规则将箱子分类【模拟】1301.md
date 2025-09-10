> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你四个整数 `length` ，`width` ，`height` 和 `mass` ，分别表示一个箱子的三个维度和质量，请你返回一个表示箱子 **类别** 的字符串。
- 如果满足以下条件，那么箱子是 `"Bulky"` 的：
    - 箱子 **至少有一个** 维度大于等于 `104` 。
    - 或者箱子的 **体积** 大于等于 `109` 。
- 如果箱子的质量大于等于 `100` ，那么箱子是 `"Heavy"` 的。
- 如果箱子同时是 `"Bulky"` 和 `"Heavy"` ，那么返回类别为 `"Both"` 。
- 如果箱子既不是 `"Bulky"` ，也不是 `"Heavy"` ，那么返回类别为 `"Neither"` 。
- 如果箱子是 `"Bulky"` 但不是 `"Heavy"` ，那么返回类别为 `"Bulky"` 。
- 如果箱子是 `"Heavy"` 但不是 `"Bulky"` ，那么返回类别为 `"Heavy"` 。

**注意**，箱子的体积等于箱子的长度、宽度和高度的乘积。

**示例 1：**
```js
输入：length = 1000, width = 35, height = 700, mass = 300
输出："Heavy"
解释：
箱子没有任何维度大于等于 104 。
体积为 24500000 <= 109 。所以不能归类为 "Bulky" 。
但是质量 >= 100 ，所以箱子是 "Heavy" 的。
由于箱子不是 "Bulky" 但是是 "Heavy" ，所以我们返回 "Heavy" 。
```
**示例 2：**
```js
输入：length = 200, width = 50, height = 800, mass = 50
输出："Neither"
解释：
箱子没有任何维度大于等于 104 。
体积为 8 * 106 <= 109 。所以不能归类为 "Bulky" 。
质量小于 100 ，所以不能归类为 "Heavy" 。
由于不属于上述两者任何一类，所以我们返回 "Neither" 。
```
**提示：**
- `1 <= length, width, height <= 10^5`
- `1 <= mass <= 10^3`

---
### 解法 模拟
读题题，按照要求分类讨论即可。可以用变量名来简化逻辑。
```cpp
class Solution {
public:
    string categorizeBox(int length, int width, int height, int mass) {
        bool x = length >= 10000 || width >= 10000 || height >= 10000 ||
            1LL * length * width * height >= 1000000000;
        bool y = mass >= 100;
        if (x && y) return "Both";
        if (x) return "Bulky";
        if (y) return "Heavy";
        return "Neither";
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(1)$ 。
- 空间复杂度：$\mathcal{O}(1)$ ，仅用到若干变量。