> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个字符串 `start` 和 `target` ，长度均为 `n` 。每个字符串 **仅** 由字符 `'L'`、`'R'` 和 `'_'` 组成，其中：
- 字符 `'L'` 和 `'R'` 表示片段，其中片段 `'L'` 只有在其左侧直接存在一个 **空位** 时才能向 **左** 移动，而片段 `'R'` 只有在其右侧直接存在一个 **空位** 时才能向 **右** 移动。
- 字符 `'_'` 表示可以被 **任意** `'L'` 或 `'R'` 片段占据的空位。

如果在移动字符串 `start` 中的片段任意次之后可以得到字符串 `target` ，返回 `true` ；否则，返回 `false` 。

**示例 1：**
<pre><strong>输入：</strong>start = "_L__R__R_", target = "L______RR"
<strong>输出：</strong>true
<strong>解释：</strong>可以从字符串 start 获得 target ，需要进行下面的移动：
- 将第一个片段向左移动一步，字符串现在变为 "<strong>L</strong>___R__R_" 。
- 将最后一个片段向右移动一步，字符串现在变为 "L___R___<strong>R</strong>" 。
- 将第二个片段向右移动三步，字符串现在变为 "L______<strong>R</strong>R" 。
可以从字符串 start 得到 target ，所以返回 true 。
</pre>

**示例 2：**
<pre><strong>输入：</strong>start = "R_L_", target = "__LR"
<strong>输出：</strong>false
<strong>解释：</strong>字符串 start 中的 'R' 片段可以向右移动一步得到 "_<strong>R</strong>L_" 。
但是，在这一步之后，不存在可以移动的片段，所以无法从字符串 start 得到 target 。
</pre>

**示例 3：**
<pre><strong>输入：</strong>start = "_R", target = "R_"
<strong>输出：</strong>false
<strong>解释：</strong>字符串 start 中的片段只能向右移动，所以无法从字符串 start 得到 target 。</pre>

**提示：**
- `n == start.length == target.length`
- `1 <= n <= 10^5`
- `start` 和 `target` 由字符 `'L'`、`'R'` 和 `'_'` 组成

---
### 解法 双指针
首先，无论怎么移动，由于 $L$ 和 $R$ 无法互相穿过对方，那么去掉 `_` 后的剩余字符应该是相同的，否则返回 $false$ 。然后用双指针从左向右遍历 $\textit{start}$ 和 $\textit{target}$ ，分类讨论：
- 如果当前字符为 $L$ 且 $i<j$ ，由于 $L$ 由于无法向右移动，返回 $false$ ；
- 如果当前字符为 $R$ 且 $i>j$ ，由于 $R$ 由于无法向左移动，返回 $false$ 。
- 遍历完，若中途没有返回 $false$ 就返回 $true$ 。

```cpp
class Solution {
public:
    bool canChange(string start, string target) {
        int n = start.size();
        int i = 0;
        for (int j = 0; j < n; ++j) {
            if (target[j] == '_') continue;
            while (i < n && start[i] == '_') ++i;
            if (i == n || 
                start[i] != target[j] || // 当前字符不同
                i != j && (start[i] == 'L') == (i < j)) 
                return false; // 顺序不符合
            ++i; 
        }
        while (i < n)
            if (start[i++] != '_') return false;
        return true;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n)$ ，其中 $n$ 为 $\textit{start}$ 的长度。
- 空间复杂度：$\mathcal{O}(1)$ 。