> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定一个由 **整数** 组成的 **非空** 数组所表示的非负整数，在该数的基础上加一。

最高位数字存放在数组的首位， 数组中每个元素只存储**单个**数字。

你可以假设除了整数 0 之外，这个整数不会以零开头。

**示例 1：**

输入：digits = [1,2,3]
输出：[1,2,4]
解释：输入数组表示数字 123。

**示例 2：**

输入：digits = [4,3,2,1]
输出：[4,3,2,2]
解释：输入数组表示数字 4321。

**示例 3：**

输入：digits = [0]
输出：[1]

**提示：**

- `1 <= digits.length <= 100`
- `0 <= digits[i] <= 9`

方法一：找出最长的后缀 999
思路

当我们对数组 digits\textit{digits}digits 加一时，我们只需要关注 digits\textit{digits}digits 的末尾出现了多少个 999 即可。我们可以考虑如下的三种情况：

如果 digits\textit{digits}digits 的末尾没有 999，例如 [1,2,3][1, 2, 3][1,2,3]，那么我们直接将末尾的数加一，得到 [1,2,4][1, 2, 4][1,2,4] 并返回；

如果 digits\textit{digits}digits 的末尾有若干个 999，例如 [1,2,3,9,9][1, 2, 3, 9, 9][1,2,3,9,9]，那么我们只需要找出从末尾开始的第一个不为 999 的元素，即 333，将该元素加一，得到 [1,2,4,9,9][1, 2, 4, 9, 9][1,2,4,9,9]。随后将末尾的 999 全部置零，得到 [1,2,4,0,0][1, 2, 4, 0, 0][1,2,4,0,0] 并返回。

如果 digits\textit{digits}digits 的所有元素都是 999，例如 [9,9,9,9,9][9, 9, 9, 9, 9][9,9,9,9,9]，那么答案为 [1,0,0,0,0,0][1, 0, 0, 0, 0, 0][1,0,0,0,0,0]。我们只需要构造一个长度比 digits\textit{digits}digits 多 111 的新数组，将首元素置为 111，其余元素置为 000 即可。

算法

们只需要对数组 digits\textit{digits}digits 进行一次逆序遍历，找出第一个不为 999 的元素，将其加一并将后续所有元素置零即可。如果 digits\textit{digits}digits 中所有的元素均为 999，那么对应着「思路」部分的第三种情况，我们需要返回一个新的数组。

代码

C++
Java
C#
Python3
Golang
JavaScript
class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        int n = digits.size();
        for (int i = n - 1; i >= 0; --i) {
            if (digits[i] != 9) {
                ++digits[i];
                for (int j = i + 1; j < n; ++j) {
                    digits[j] = 0;
                }
                return digits;
            }
        }

        // digits 中所有的元素均为 9
        vector<int> ans(n + 1);
        ans[0] = 1;
        return ans;
    }
};
复杂度分析

时间复杂度：O(n)O(n)O(n)，其中 nnn 是数组 digits\textit{digits}digits 的长度。

空间复杂度：O(1)O(1)O(1)。返回值不计入空间复杂度。

作者：力扣官方题解
链接：https://leetcode.cn/problems/plus-one/solutions/1057162/jia-yi-by-leetcode-solution-2hor/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

```python
class Solution {
    public int[] plusOne(int[] digits) {
        int len = digits.length;
        for (int i = len - 1; i >= 0; i--) {
            digits[i] = (digits[i] + 1) % 10;
            if (digits[i] != 0) {
                return digits;
            }
        }
        digits = new int[len + 1];
        digits[0] = 1;
        return digits;
    }
}
```