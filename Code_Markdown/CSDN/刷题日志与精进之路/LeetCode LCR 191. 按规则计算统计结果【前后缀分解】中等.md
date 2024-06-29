> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

为了深入了解这些生物群体的生态特征，你们进行了大量的实地观察和数据采集。数组 `arrayA` 记录了各个生物群体数量数据，其中 `arrayA[i]` 表示第 `i` 个生物群体的数量。请返回一个数组 `arrayB`，该数组为基于数组 `arrayA` 中的数据计算得出的结果，其中 `arrayB[i]` 表示将第 `i` 个生物群体的数量从总体中排除后的其他数量的乘积。

**示例 1：**
```js
输入：arrayA = [2, 4, 6, 8, 10]
输出：[1920, 960, 640, 480, 384]
```
**提示：**
- 所有元素乘积之和不会溢出 32 位整数
- `arrayA.length <= 100000`

---
### 解法 前后缀分解
和LeetCode 238. 除自身以外数组的乘积一样。
```cpp
class Solution {
public:
    vector<int> statisticalResult(vector<int>& arrayA) {
        int n = arrayA.size();
        vector<int> ans(n);
        for (int i = 0, pre = 1; i < n; pre *= arrayA[i++]) ans[i] = pre;
        for (int i = n - 1, suf = 1; i >= 0; suf *= arrayA[i--]) ans[i] *= suf;
        return ans;
    }
};
```