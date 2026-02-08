> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091713584.png)

给定一个不含重复数字的数组 `nums` ，返回其 _所有可能的全排列_ 。你可以 **按任意顺序** 返回答案。

**示例 1：**

**输入：**nums = [1,2,3]
**输出：**[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]

**示例 2：**

**输入：**nums = [0,1]
**输出：**[[0,1],[1,0]]

**示例 3：**

**输入：**nums = [1]
**输出：**[[1]]

**提示：**

- `1 <= nums.length <= 6`
- `-10 <= nums[i] <= 10`
- `nums` 中的所有整数 **互不相同**
class Solution {
    public List<List<Integer>> permute(int[] nums) {
        var res = new ArrayList<List<Integer>>();

        var output = new ArrayList<Integer>();
        for (int num : nums) {
            output.add(num);
        }

        int n = nums.length;
        backtrack(n, output, res, 0);
        return res;
    }

    public void backtrack(int n, List<Integer> output, List<List<Integer>> res, int first) {
        // 所有数都填完了
        if (first == n) {
            res.add(new ArrayList<Integer>(output));
        }
        for (int i = first; i < n; i++) {
            // 动态维护数组
            Collections.swap(output, first, i);
            // 继续递归填下一个数
            backtrack(n, output, res, first + 1);
            // 撤销操作
            Collections.swap(output, first, i);
        }
    }
}
复杂度分析

时间复杂度：O(n×n!)O(n \times n!)O(n×n!)，其中 nnn 为序列的长度。

算法的复杂度首先受 backtrack\textit{backtrack}backtrack 的调用次数制约，backtrack\textit{backtrack}backtrack 的调用次数为 ∑k=1nP(n,k)\sum_{k = 1}^{n}{P(n, k)}∑ 
k=1
n
​
 P(n,k) 次，其中 P(n,k)=n!(n−k)!=n(n−1)…(n−k+1)P(n, k) = \frac{n!}{(n - k)!} = n (n - 1) \ldots (n - k + 1)P(n,k)= 
(n−k)!
n!
​
 =n(n−1)…(n−k+1)，该式被称作 n 的 k - 排列，或者部分排列。

而 ∑k=1nP(n,k)=n!+n!1!+n!2!+n!3!+…+n!(n−1)!<2n!+n!2+n!22+n!2n−2<3n!\sum_{k = 1}^{n}{P(n, k)} = n! + \frac{n!}{1!} + \frac{n!}{2!} + \frac{n!}{3!} + \ldots + \frac{n!}{(n-1)!} < 2n! + \frac{n!}{2} + \frac{n!}{2^2} + \frac{n!}{2^{n-2}} < 3n!∑ 
k=1
n
​
 P(n,k)=n!+ 
1!
n!
​
 + 
2!
n!
​
 + 
3!
n!
​
 +…+ 
(n−1)!
n!
​
 <2n!+ 
2
n!
​
 + 
2 
2
 
n!
​
 + 
2 
n−2
 
n!
​
 <3n!

这说明 backtrack\textit{backtrack}backtrack 的调用次数是 O(n!)O(n!)O(n!) 的。

而对于 backtrack\textit{backtrack}backtrack 调用的每个叶结点（共 n!n!n! 个），我们需要将当前答案使用 O(n)O(n)O(n) 的时间复制到答案数组中，相乘得时间复杂度为 O(n×n!)O(n \times n!)O(n×n!)。

因此时间复杂度为 O(n×n!)O(n \times n!)O(n×n!)。

空间复杂度：O(n)O(n)O(n)，其中 nnn 为序列的长度。除答案数组以外，递归函数在递归过程中需要为每一层递归函数分配栈空间，所以这里需要额外的空间且该空间取决于递归的深度，这里可知递归调用深度为 O(n)O(n)O(n)。

作者：力扣官方题解
链接：https://leetcode.cn/problems/permutations/solutions/218275/quan-pai-lie-by-leetcode-solution-2/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。