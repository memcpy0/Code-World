> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091400541.png)
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307042336009.png)
给你一个 **无重复元素** 的整数数组 `candidates` 和一个目标整数 `target` ，找出 `candidates` 中可以使数字和为目标数 `target` 的 所有 **不同组合** ，并以列表形式返回。你可以按 **任意顺序** 返回这些组合。

`candidates` 中的 **同一个** 数字可以 **无限制重复被选取** 。如果至少一个数字的被选数量不同，则两种组合是不同的。 

对于给定的输入，保证和为 `target` 的不同组合数少于 `150` 个。

**示例 1：**
```js
输入：candidates = `[2,3,6,7],` target = `7`
输出：[[2,2,3],[7]]
解释：
2 和 3 可以形成一组候选，2 + 2 + 3 = 7 。注意 2 可以使用多次。
7 也是一个候选， 7 = 7 。
仅有这两种组合。
```
**示例 2：**
```js
输入: candidates = [2,3,5]`,` target = 8
输出: [[2,2,2,2],[2,3,3],[3,5]]
```
**示例 3：**
```js
输入: candidates = `[2],` target = 1
输出: []
```
**提示：**
- `1 <= candidates.length <= 30`
- `2 <= candidates[i] <= 40`
- `candidates` 的所有元素 **互不相同**
- `1 <= target <= 40`

---
### 解法 搜索回溯
对于这类寻找所有可行解的题，我们都可以尝试用「搜索回溯」的方法来解决。

回到本题，我们定义递归函数 $dfs(target, combine, idx)$ 表示当前在 $candidates$ 数组的第 $idx$ 位，还剩 $target$ 要组合，已经组合的列表为 $combine$ 。递归的终止条件为 $target\le 0$ 或者 $candidates$ 数组被全部用完。

那么在当前的函数中，每次我们可以选择跳过不用第 $idx$ 个数，即执行
$dfs(target,combine,idx+1)$ 。也可以选择使用第 $idx$ 个数，即执行  $dfs(target−candidates[idx],combine,idx)$ ，注意到**每个数字可以被无限制重复选取，因此搜索的下标仍为 $idx$ **。

更形象化地说，如果我们将整个搜索过程用一个树来表达，即如下图呈现，==每次的搜索都会延伸出两个分叉==，直到递归的终止条件，这样我们就能不重复且不遗漏地找到所有可行解：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307042337906.png)
当然，搜索回溯的过程一定存在一些优秀的剪枝方法来使得程序运行得更快，而这里只给出了**最朴素不含剪枝的写法**。
```cpp
class Solution {
public:
    vector<vector<int>> ans;
    void dfs(vector<int>& candidates, int target,  vector<int>& combine, int idx) {
        if (idx == candidates.size()) return;
        if (target == 0) {
            ans.emplace_back(combine);
            return;
        }
        // 直接跳过
        dfs(candidates, target, combine, idx + 1);
        // 选择当前数
        if (target - candidates[idx] >= 0) {
            combine.emplace_back(candidates[idx]);
            dfs(candidates, target - candidates[idx], combine, idx);
            combine.pop_back();
        }
    }
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<int> combine;
        dfs(candidates, target, combine, 0);
        return ans;
    }
};
```
还可以进行排序，然后剪枝：
```cpp
class Solution {
public:
    vector<vector<int>> ans;
    void dfs(vector<int>& candidates, int target,  vector<int>& combine, int idx) { 
        if (idx == candidates.size()) return;
        if (target == 0) {
            ans.emplace_back(combine);
            return;
        }
        if (target - candidates[idx] < 0) return;
        // 直接跳过
        dfs(candidates, target, combine, idx + 1);
        // 选择当前数
        combine.emplace_back(candidates[idx]);
        dfs(candidates, target - candidates[idx], combine, idx);
        combine.pop_back();
    }
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<int> combine;
        sort(candidates.begin(), candidates.end());
        dfs(candidates, target, combine, 0);
        return ans;
    }
};
```
> 这种**不在意组合内元素顺序的**，比如 `[2,2,3]` 和 `[3,2,2]` 都是答案的，反而要按照特定的某种顺序进行搜索，保证一个答案只会搜索出特定的一种顺序。
> 而**在意排列或组合内元素顺序的**，往往只需要用哈希表来记录一个元素之前是否使用过。

复杂度分析：
- 时间复杂度：$O(S)$ ，其中 $S$ 为所有可行解的长度之和。从分析给出的搜索树，我们可以看出时间复杂度取决于搜索树所有叶子节点的深度之和，即所有可行解的长度之和。在这题中，我们很难给出一个比较紧的上界，我们知道 $O(n \times 2^n)$ 是一个比较松的上界，即在这份代码中，$n$ 个位置每次考虑选或者不选，如果符合条件，就加入答案的时间代价。但实际运行的时候，因为不可能所有的解都满足条件，递归时我们还会用 $target−candidates[idx]≥0$ 进行剪枝，所以实际运行情况是远远小于这个上界的。
- 空间复杂度：$O(target)$ 。除答案数组外，空间复杂度取决于递归的栈深度，在最差情况下需要递归 $O(target)$ 层。



