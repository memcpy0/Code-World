> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个二维 `3 x 3` 的矩阵 `grid` ，每个格子都是一个字符，要么是 `'B'` ，要么是 `'W'` 。字符 `'W'` 表示白色，字符 `'B'` 表示黑色。

你的任务是改变 **至多一个** 格子的颜色，使得矩阵中存在一个 `2 x 2` 颜色完全相同的正方形。

如果可以得到一个相同颜色的 `2 x 2` 正方形，那么返回 `true` ，否则返回 `false` 。

**示例 1：**
```c
输入：grid = [["B","W","B"],["B","W","W"],["B","W","B"]]
输出：true
解释：
修改 `grid[0][2]` 的颜色，可以满足要求。
```
**示例 2：**
```c
输入：grid = [["B","W","B"],["W","B","W"],["B","W","B"]]
输出：false
解释：
只改变一个格子颜色无法满足要求。
```
**示例 3：**
```c
输入：grid = [["B","W","B"],["B","W","W"],["B","W","W"]]
输出：true
解释：
`grid` 已经包含一个 `2 x 2` 颜色相同的正方形了。
```
**提示：**
- `grid.length == 3`
- `grid[i].length == 3`
- `grid[i][j]` 要么是 `'W'` ，要么是 `'B'` 。

---
### 解法 遍历统计
遍历矩阵中的每个 $2×2$ 子矩形。对于每个子矩形，统计 $B$ 和 $W$ 的个数，如果其中一个字母的出现次数 $≥3$ ，则返回 $true$ 。注：**也可以判断其中一个字母的出现次数 $\ne 2$** 。

如果四个子矩形都不满足要求，返回 $false$ 。

代码实现时，由于 $B$ 和 $W$ 的 ASCII 值的奇偶性（二进制最低位）不同，可以统计其二进制最低位，代替统计字母。
```python
class Solution:
    def canMakeSquare(self, grid: List[List[str]]) -> bool:
        def check(i: int, j: int) -> bool:
            cnt = defaultdict(int)
            cnt[grid[i][j]] += 1
            cnt[grid[i][j + 1]] += 1
            cnt[grid[i + 1][j]] += 1
            cnt[grid[i + 1][j + 1]] += 1
            return cnt['B'] != 2
        return check(0, 0) or check(0, 1) or check(1, 0) or check(1, 1)
```
```java
class Solution {
    private boolean check(char[][] grid, int i, int j) {
        int[] cnt = new int[2];
        cnt[grid[i][j] & 1]++;
        cnt[grid[i][j + 1] & 1]++;
        cnt[grid[i + 1][j] & 1]++;
        cnt[grid[i + 1][j + 1] & 1]++;
        return cnt[0] != 2;
    }
    public boolean canMakeSquare(char[][] grid) {
        return check(grid, 0, 0) || check(grid, 0, 1) ||
            check(grid, 1, 0) || check(grid, 1, 1);
    }
}
```
```cpp
class Solution {
public:
    bool canMakeSquare(vector<vector<char>>& grid) {
        auto check = [&](int i, int j) {
            int cnt[2]{};
            cnt[grid[i][j] & 1]++;
            cnt[grid[i][j + 1] & 1]++;
            cnt[grid[i + 1][j] & 1]++;
            cnt[grid[i + 1][j + 1] & 1]++;
            return cnt[0] != 2;
        };
        return check(0, 0) || check(0, 1) || check(1, 0) || check(1, 1);
    }
};
```
```go
func canMakeSquare(grid [][]byte) bool {
	check := func(i, j int) bool {
		cnt := [2]int{}
		cnt[grid[i][j]&1]++
		cnt[grid[i][j+1]&1]++
		cnt[grid[i+1][j]&1]++
		cnt[grid[i+1][j+1]&1]++
		return cnt[0] != 2
	}
	return check(0, 0) || check(0, 1) || check(1, 0) || check(1, 1)
}
```
```rust
impl Solution {
    pub fn check(grid: &Vec<Vec<char>>, x: usize, y: usize) -> bool {
        let mut cnt = 0;
        for i in 0..2 {
            for j in 0..2 {
                if grid[(x + i) as usize][(y + j) as usize] == 'B' {
                    cnt += 1
                }
            }
        }
        cnt != 2
    }
    pub fn can_make_square(grid: Vec<Vec<char>>) -> bool {
        Self::check(&grid, 0, 0) || Self::check(&grid, 0, 1) || 
            Self::check(&grid, 1, 0) || Self::check(&grid, 1, 1)
    }
}
```
```javascript
var check = function(grid, x, y) {
    let cnt = 0;
    for (let i = 0; i <= 1; ++i) {
        for (let j = 0; j <= 1; ++j) {
            if (grid[x + i][y + j] == 'B') {
                ++cnt;
            }
        }
    }
    return cnt != 2;
}

/**
 * @param {character[][]} grid
 * @return {boolean}
 */
var canMakeSquare = function(grid) {
    return check(grid, 0, 0) || check(grid, 0, 1) ||
        check(grid, 1, 0) || check(grid, 1, 1);
};
```
**复杂度分析**：
- 时间复杂度：$O(1)$ 。
- 空间复杂度：$O(1)$ 。