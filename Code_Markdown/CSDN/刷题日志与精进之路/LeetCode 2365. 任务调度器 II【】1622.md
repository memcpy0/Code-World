> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的正整数数组 `tasks` ，表示需要 **按顺序** 完成的任务，其中 `tasks[i]` 表示第 `i` 件任务的 **类型** 。

同时给你一个正整数 `space` ，表示一个任务完成 **后** ，另一个 **相同** 类型任务完成前需要间隔的 **最少** 天数。

在所有任务完成前的每一天，你都必须进行以下两种操作中的一种：

- 完成 `tasks` 中的下一个任务
- 休息一天

请你返回完成所有任务所需的 **最少** 天数。

**示例 1：**

**输入：**tasks = [1,2,1,2,3,1], space = 3
**输出：**9
**解释：**
9 天完成所有任务的一种方法是：
第 1 天：完成任务 0 。
第 2 天：完成任务 1 。
第 3 天：休息。
第 4 天：休息。
第 5 天：完成任务 2 。
第 6 天：完成任务 3 。
第 7 天：休息。
第 8 天：完成任务 4 。
第 9 天：完成任务 5 。
可以证明无法少于 9 天完成所有任务。

**示例 2：**

**输入：**tasks = [5,8,8,5], space = 2
**输出：**6
**解释：**
6 天完成所有任务的一种方法是：
第 1 天：完成任务 0 。
第 2 天：完成任务 1 。
第 3 天：休息。
第 4 天：休息。
第 5 天：完成任务 2 。
第 6 天：完成任务 3 。
可以证明无法少于 6 天完成所有任务。

**提示：**

- `1 <= tasks.length <= 10^5`
- `1 <= tasks[i] <= 10^9`
- `1 <= space <= tasks.length`

```cpp
class Solution {
public:
    long long taskSchedulerII(vector<int>& tasks, int space) {
        unordered_map<int, long long> lastTask;
        long long ans = 0;
        for (int i = 0, n = tasks.size(); i < n; ++i) {
            if (lastTask.find(tasks[i]) == lastTask.end()) {
                lastTask[tasks[i]] = ans;
                ++ans;
            } else {
                long long prev = lastTask[tasks[i]];
                if (ans > prev + space) {
                    lastTask[tasks[i]] = ans;
                    ++ans;
                } else {
                    ans = prev + space + 1;
                    
                    lastTask[tasks[i]] = ans;
                    ++ans;
                }
            }
            // cout << i << " " <<ans << endl;
        }
        return ans;
    }
};
```