> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091400156.png)
给定一个候选人编号的集合 `candidates` 和一个目标数 `target` ，找出 `candidates` 中所有可以使数字和为 `target` 的组合。

`candidates` 中的每个数字在每个组合中只能使用 **一次** 。

**注意**：解集不能包含重复的组合。 

**示例 1:**
```js
输入: candidates = `[10,1,2,7,6,1,5]`, target = `8`,
输出:
[
[1,1,6],
[1,2,5],
[1,7],
[2,6]
]
```
**示例 2:**
```js
输入: candidates = [2,5,2,1,2], target = 5,
输出:
[
[1,2,2],
[5]
]
```
**提示:**
- `1 <= candidates.length <= 100`
- `1 <= candidates[i] <= 50`
- `1 <= target <= 30`

---
### 解法 回溯+剪枝
由于我们需要求出所有和为 $target$ 的组合，并且每个数只能使用一次，因此我们可以使用递归 + 回溯的方法来解决这个问题：

我们用 $dfs(pos,rest)$ 表示递归的函数，其中 $pos$ 表示我们当前递归到了数组 $candidates$s 中的第 $pos$ 个数，而 $rest$ 表示我们还需要选择和为 $rest$ 的数放入列表作为一个组合；

对于当前的第 $pos$ 个数，我们有两种方法：选或者不选。
- 如果我们选了这个数，那么我们调用 $dfs(pos+1,rest−candidates[pos])$ 进行递归，注意这里必须满足 $rest≥candidates[pos]$ 
- 如果我们不选这个数，那么我们调用 $dfs(pos+1,rest)$ 进行递归；

在某次递归开始前，如果 $rest$ 的值为 $0$ ，说明我们找到了一个和为 $target$ 的组合，将其放入答案中。每次调用递归函数前，如果我们选了那个数，就需要将其放入列表的末尾，该列表中存储了我们选的所有数。在回溯时，如果我们选了那个数，就要将其从列表的末尾删除。

上述算法就是一个标准的递归 + 回溯算法，但是它并不适用于本题。这是因为题目描述中规定了解集不能包含重复的组合，而上述的算法中并没有去除重复的组合。例如当 $candidates=[2,2]$ ，$target=2$ 时，上述算法会将列表 $[2]$ 放入答案两次。

因此，我们需要改进上述算法，**在求出组合的过程中就进行去重的操作**。我们可以==考虑将相同的数放在一起进行处理==，也就是说，如果数 $x$ 出现了 $y$ 次，那么在递归时一次性地处理它们，即分别调用选择 $0,1,⋯ ,y$  次 $x$ 的递归函数。这样我们就不会得到重复的组合。具体地：
1. 我们使用一个哈希映射（HashMap）统计数组 $candidates$ 中每个数出现的次数。在统计完成之后，我们将结果放入一个列表 $freq$ 中，方便后续的递归使用。
2. 列表 $freq$ 的长度即为数组 $candidates$s 中不同数的个数。其中的每一项对应着哈希映射中的一个键值对，即某个数以及它出现的次数。
3. 在递归时，对于当前的第 $pos$ 个数，它的值为 $freq[pos][0]$ ，出现的次数为 $freq[pos][1]$ ，那么我们可以调用
$$\textit{dfs}(\textit{pos} + 1, \textit{rest} - i \times \textit{freq}[\textit{pos}][0])$$
    即我们选择了这个数 $i$ 次。这里 **$i$ 不能大于这个数出现的次数，并且 $i \times \textit{freq}[\textit{pos}][0]$ 也不能大于 $\textit{rest}$** 。同时，我们需要将 $i$ 个 $\textit{freq}[\textit{pos}][0]$ 放入列表中。
    
这样一来，我们就可以**不重复地枚举所有的组合**了。

我们还可以进行什么优化（剪枝）呢？**一种比较常用的优化方法是，我们将 $freq$ 根据数从小到大排序，这样我们在递归时会先选择小的数，再选择大的数**（同[[LeetCode 39. Combination Sum【回溯,剪枝】中等]]的剪枝优化一样）。这样做的好处是，当我们递归到 $dfs(pos,rest)$ 时，**如果 $freq[pos][0]$ 已经大于 $rest$，那么后面还没有递归到的数也都大于 $rest$** ，这就说明不可能再选择若干个和为 $rest$ 的数放入列表了。此时，我们就可以直接回溯。
```cpp
class Solution {
public:
    vector<pair<int, int>> freq;
    vector<vector<int>> ans;
    vector<int> seq;
    void dfs(int pos, int rest) {
        if (rest == 0) {
            ans.push_back(seq);
            return;
        }
        if (pos == freq.size() || rest < freq[pos].first) return;
        // 直接跳过
        dfs(pos + 1, rest);
        int most = min(rest / freq[pos].first, freq[pos].second);
        for (int i = 1; i <= most; ++i) {
            seq.push_back(freq[pos].first);
            dfs(pos + 1, rest - i * freq[pos].first);
        }
        for (int i = 1; i <= most; ++i) seq.pop_back();
    }
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        for (int num : candidates) {
            if (freq.empty() || num != freq.back().first)
                freq.emplace_back(num, 1);
            else ++freq.back().second;
        }
        dfs(0, target);
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(2^n \times n)$ ，其中 $n$ 是数组 $candidates$ 的长度。在大部分递归 + 回溯的题目中，我们无法给出一个严格的渐进紧界，故这里只分析一个较为宽松的渐进上界。在最坏的情况下，数组中的每个数都不相同，那么列表 $freq$ 的长度同样为 $n$ 。在递归时，每个位置可以选或不选，如果数组中所有数的和不超过 $target$ ，那么 $2^n$ 种组合都会被枚举到；在 $target$ 小于数组中所有数的和时，我们并不能解析地算出满足题目要求的组合的数量，但我们知道每得到一个满足要求的组合，需要 $O(n)$ 的时间将其放入答案中，因此我们将 $O(2^n)$ 与 $O(n)$ 相乘，即可估算出一个宽松的时间复杂度上界。由于 $O(2^n \times n)$ 在渐进意义下大于排序的时间复杂度 $O(n \log n)$ ，因此后者可以忽略不计。
- 空间复杂度：$O(n)$ 。除了存储答案的数组外，我们需要 $O(n)$ 的空间存储列表 $freq$ 、递归中存储当前选择的数的列表、以及递归需要的栈。
