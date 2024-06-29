> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `nums` 。请你创建一个满足以下条件的二维数组：
- 二维数组应该 **只** 包含数组 `nums` 中的元素。
- 二维数组中的每一行都包含 **不同** 的整数。
- 二维数组的行数应尽可能 **少** 。

返回结果数组。如果存在多种答案，则返回其中任何一种。

请注意，二维数组的每一行上可以存在不同数量的元素。

**示例 1：**
```java
输入：nums = [1,3,4,1,2,3,1]
输出：[[1,3,4,2],[1,3],[1]]
解释：根据题目要求可以创建包含以下几行元素的二维数组：
- 1,3,4,2
- 1,3
- 1
nums 中的所有元素都有用到，并且每一行都由不同的整数组成，所以这是一个符合题目要求的答案。
可以证明无法创建少于三行且符合题目要求的二维数组。
```
**示例 2：**
```java
输入：nums = [1,2,3,4]
输出：[[4,3,2,1]]
解释：nums 中的所有元素都不同，所以我们可以将其全部保存在二维数组中的第一行。
```
**提示：**
- `1 <= nums.length <= 200`
- `1 <= nums[i] <= nums.length`

---
### 解法 哈希表
用一个哈希表 $\textit{cnt}$ 记录每个元素的剩余次数。构造答案时，遍历哈希表，如果元素 $x$ 的剩余次数不为 $0$ ，则将 $x$ 加入新行中，并令 $cnt[x]$ 减一；如果 $x$ 的次数为 $0$ ，则将 $x$ 从 $cnt$ 中删除。这需要两个循环。
```python
# python
class Solution:
    def findMatrix(self, nums: List[int]) -> List[List[int]]:
        ans = []
        cnt = Counter(nums)
        while cnt:
            ans.append(list(cnt)) # 每次将返回字典的key列表放入ans中
            for x in ans[-1]:
                cnt[x] -= 1
                if cnt[x] == 0:
                    del cnt[x] # 从字典中删除键值对
        return ans

# go
func findMatrix(nums []int) (ans [][]int) {
    cnt := map[int]int {}
    for _, x := range nums {
        cnt[x]++
    }
    for len(cnt) > 0 {
        row := []int {}
        for x := range cnt {
            row = append(row, x)
            if cnt[x]--; cnt[x] == 0 {
                delete(cnt, x)
            }
        }
        ans = append(ans, row)
    }
    return
}
```
实际上，==我们可以在构造哈希表的循环中，就完成答案二维数组的构造==。类似「二叉树的右视图」那一题，我们遍历数组，记录每个元素的出现次数，**当发现元素 $x$ 第 $idx$ 次出现时，如果二维数组的行数 $\le idx$ ，则不需要新行，将 $x$ 放入第 $idx$ 行；否则添加新一行，再将元素 $x$ 放入**。
```cpp
// cpp
class Solution {
public:
    vector<vector<int>> findMatrix(vector<int>& nums) {
        vector<vector<int>> ans;
        int cnt[201] = {0};
        for (int x : nums) {
            ++cnt[x];
            if (ans.size() < cnt[x]) ans.emplace_back();
            ans[cnt[x] - 1].push_back(x);
        }   
        return ans;
    }
};

// java
class Solution {
    public List<List<Integer>> findMatrix(int[] nums) {
        var ans = new ArrayList<List<Integer>>();
        int[] cnt = new int[220];
        for (int x : nums) {
            ++cnt[x];
            if (ans.size() < cnt[x]) ans.add(new ArrayList<Integer>());
            ans.get(cnt[x] - 1).add(x);
        }
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为 $\textit{nums}$ 的长度。
- 空间复杂度：$O(n)$ 。