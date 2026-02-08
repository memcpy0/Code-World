> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202309102029795.png)

找出所有相加之和为 `n` 的 `k` 个数的组合，且满足下列条件：
- 只使用数字1到9
- 每个数字 **最多使用一次** 

返回 _所有可能的有效组合的列表_ 。该列表不能包含相同的组合两次，组合可以以任何顺序返回。

**示例 1:**
```cpp
输入: k = 3, n = 7
输出: [[1,2,4]]
解释:
1 + 2 + 4 = 7
没有其他符合的组合了。
```
**示例 2:**
```cpp
输入: k = 3, n = 9
输出: [[1,2,6], [1,3,5], [2,3,4]]
解释:
1 + 2 + 6 = 9
1 + 3 + 5 = 9
2 + 3 + 4 = 9
没有其他符合的组合了。
```
**示例 3:**
```cpp
输入: k = 4, n = 1
输出: []
解释: 不存在有效的组合。
在[1,9]范围内使用4个不同的数字，我们可以得到的最小和是1+2+3+4 = 10，因为10 > 1，没有有效的组合。
```
**提示:**
- `2 <= k <= 9`
- `1 <= n <= 60`

---
### 解法 回溯+剪枝
这道题其实是**子集型回溯**的修改版——**组合型回溯**。回顾一下，从 $[3,2,1]$ 中选出所有子集的搜索树如下，第一层是选择一个数的所有情况，第二层是选择两个数的所有情况……这刚好是我们要求的组合问题，从 $n$ 个数中选 $k$ 个数的**组合**可看做是**长度固定的子集**。

所以**只需要在求子集的基础上，增加一个判断逻辑**，比如选两个数，当路径长度为 $2$ 时就可以直接返回、不用继续递归了。如果选 $3$ 个数，第一个数选的是 $2$ ，那后面还需要继续递归吗？不需要，因为后面只能选 $1$ ，无法选出三个数，可直接返回。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202404212023648.png)
从这里可以看出，组合问题我们是可以做一些优化的。
- **剪枝1 剩余数字数目不够**：设 $path$ 长度为 $m$ ，那么还需要选 $d = k - m$ 个数。**从大到小枚举**，当前需要从 $[1, i]$ 这 $i$ 个数中选择，如果 $i < d$ ，最后必然无法选出 $k$ 个数——倒序枚举是因为这样不等式 $i < d$ 会简单一些；正序枚举：从 $[i, n]$ 这 $n - i +1$ 个数中选择，如果 $n - i + 1 < d$ ……
- **剪枝2 目标数 $t < 0$**
- **剪枝3 剩余数字即使全部选最大的，和也不够 $t$** ：例如 $i = 5$ ，还需要选 $d = 3$ 个数，那么如果 $t > 5 + 4 + 3$ ，可以直接返回—— $t > i +\dots + (i - d + 1) = \dfrac{(i + i - d + 1) d }{2}$ 。
#### 枚举选哪个+剪枝
```python
class Solution:
    def combinationSum3(self, k: int, n: int) -> List[List[int]]:
        ans = []
        path = []
        def dfs(i: int, t: int) -> None:
            d = k - len(path) # 还要选d个数
            if t < 0 or t > (i * 2 - d + 1) * d // 2: # 剪枝
                return
            # 把路径加入答案时不用判断t是否等于0,因为剪枝这步保证剩余d个数一定能组合出t
            # 代入d=0,则剪枝这一行为if t<0 or t>0, 只有t=0时才会越过剪枝
            if d == 0: # 选到一个合法组合
                ans.append(path.copy())
                return
            for j in range(i, d - 1, -1): # d剪枝,i>=d
                path.append(j)
                dfs(j - 1, t - j) # 选了j就是目标就是t-j
                path.pop()
        dfs(9, n)
        return ans
```

```java
class Solution {
    public List<List<Integer>> combinationSum3(int k, int n) {
        List<List<Integer>> ans = new ArrayList<>();
        List<Integer> path = new ArrayList<>(k);
        dfs(9, n, k, ans, path);
        return ans;
    }
    private void dfs(int i, int t, int k, List<List<Integer>> ans, List<Integer> path) {
        int d = k - path.size(); // 还要选d个数
        if (t < 0 || t > (i * 2 - d + 1) * d / 2) return; // 剪枝
        if (d == 0) { // 找到一个合法组合
            ans.add(new ArrayList<>(path));
            return;
        }
        for (int j = i; j >= d; --j) {
            path.add(j);
            dfs(j - 1, t - j, k, ans, path);
            path.remove(path.size() - 1);
        }
    }
}
```

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        vector<vector<int>> ans;
        vector<int> path;
        function<void(int, int)> dfs = [&](int i, int t) {
            int d = k - path.size(); // 还要选d个数
            if (t < 0 || t > (i * 2 - d + 1) * d / 2) return; // 剪枝
            if (d == 0) { // 找到一个合法组合
                ans.emplace_back(path);
                return;
            }
            for (int j = i; j >= d; --j) {
                path.push_back(j);
                dfs(j - 1, t - j);
                path.pop_back();
            }
        };
        dfs(9, n);
        return ans;
    }
};
```

```go
func combinationSum3(k int, n int) (ans [][]int) {
    path := []int{}
    var dfs func(int, int) 
    dfs = func(i, t int) {
        d := k - len(path) // 还要选d个数
        if t < 0 || t > (i * 2 - d + 1) * d / 2 { // 剪枝
            return
        }
        if d == 0 { // 找到一个合法组合
            ans = append(ans, slices.Clone(path))
            return
        }
        for j := i; j >= d; j-- {
            path = append(path, j)
            dfs(j - 1, t - j)
            path = path[:len(path) - 1]
        }
    }
    dfs(9, n)
    return
}
```
复杂度分析：
- 时间复杂度：分析回溯问题的时间复杂度，有一个通用公式：路径长度 $\times$ 搜索树的叶子数。对于本题，它等于 $\mathcal{O}(k\cdot C(9,k))$（去掉剪枝就是 **77. 组合**）。
- 空间复杂度：$\mathcal{O}(k)$ 。返回值不计入。

#### 选或不选+剪枝
```python
class Solution:
    def combinationSum3(self, k: int, n: int) -> List[List[int]]:
        ans = []
        path = []
        def dfs(i: int, t: int) -> None:
            d = k - len(path) # 还要选d个数
            if t < 0 or t > (i * 2 - d + 1) * d // 2: # 剪枝
                return
            # 不用判断t是否等于0，因为剪枝这步保证剩余d个数一定能组合出t
            # 代入d=0,则剪枝这一行为if t<0 or t>0, 只有t=0时才会越过剪枝
            if d == 0: # 选到一个合法组合
                ans.append(path.copy())
                return
            # 不选i(剩下d个数必须选)
            if i > d:
                dfs(i - 1, t)
            # 选i
            path.append(i)
            dfs(i - 1, t - i)
            path.pop()
            
        dfs(9, n)
        return ans
```

```java
class Solution {
    public List<List<Integer>> combinationSum3(int k, int n) {
        List<List<Integer>> ans = new ArrayList<>();
        List<Integer> path = new ArrayList<>(k);
        dfs(9, n, k, ans, path);
        return ans;
    }
    private void dfs(int i, int t, int k, List<List<Integer>> ans, List<Integer> path) {
        int d = k - path.size(); // 还要选d个数
        if (t < 0 || t > (i * 2 - d + 1) * d / 2) return; // 剪枝
        if (d == 0) { // 找到一个合法组合
            ans.add(new ArrayList<>(path));
            return;
        }
        // 不选i
        if (i > d) dfs(i - 1, t, k, ans, path);
        // 选i
        path.add(i);
        dfs(i - 1, t - i, k, ans, path);
        path.remove(path.size() - 1); // 恢复现场
    }
}
```

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        vector<vector<int>> ans;
        vector<int> path;
        function<void(int, int)> dfs = [&](int i, int t) {
            int d = k - path.size(); // 还要选d个数
            if (t < 0 || t > (i * 2 - d + 1) * d / 2) return; // 剪枝
            if (d == 0) { // 找到一个合法组合
                ans.emplace_back(path);
                return;
            }
            // 不选i
            if (i > d) dfs(i - 1, t);
            // 选i
            path.push_back(i);
            dfs(i - 1, t - i);
            path.pop_back();
        };
        dfs(9, n);
        return ans;
    }
};
```

```go
func combinationSum3(k int, n int) (ans [][]int) {
    path := []int{}
    var dfs func(int, int) 
    dfs = func(i, t int) {
        d := k - len(path) // 还要选d个数
        if t < 0 || t > (i * 2 - d + 1) * d / 2 { // 剪枝
            return
        }
        if d == 0 { // 找到一个合法组合
            ans = append(ans, slices.Clone(path))
            return
        }
        // 不选i
        if i > d {
            dfs(i - 1, t)
        }
        // 选i
        path = append(path, i)
        dfs(i - 1, t - i)
        path = path[:len(path) - 1]
    }
    dfs(9, n)
    return
}
```
复杂度分析
- [见此](https://leetcode.cn/problems/combinations/solutions/1815859/dfs-suan-fa-de-fu-za-du-fen-xi-by-hqztru-14v8/)。

---
### 解法2 二进制子集枚举
