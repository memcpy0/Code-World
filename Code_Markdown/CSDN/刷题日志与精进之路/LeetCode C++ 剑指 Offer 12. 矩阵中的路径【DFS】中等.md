

请设计一个函数，用来判断在一个矩阵中是否存在一条包含某字符串所有字符的路径。路径可以从矩阵中的任意一格开始，每一步可以在矩阵中向左、右、上、下移动一格。如果一条路径经过了矩阵的某一格，那么该路径不能再次进入该格子。例如，在下面的 `3 × 4` 的矩阵中包含一条字符串 `“bfce”` 的路径（路径中的字母用加粗标出）。

[["a",**"b"**,"c","e"],
["s",**"f"**,**"c"**,"s"],
["a","d",**"e"**,"e"]]

但矩阵中不包含字符串 `“abfb”` 的路径，因为字符串的第一个字符 `b` 占据了矩阵中的第一行第二个格子之后，路径不能再次进入这个格子。

 

**示例 1：**

```swift
输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "ABCCED"
输出：true
```

**示例 2：**

```swift
输入：board = [["a","b"],["c","d"]], word = "abcd"
输出：false
```

**提示：**
- `1 <= board.length <= 200`
 - `1 <= board[i].length <= 200`

---
### 解法 DFS+原地标记矩阵
```cpp
class Solution {
private:
    int n, m;
    bool dfs(vector<vector<char>>& grid, int x, int y, string& word, int i) {
        if (i >= word.size()) return true;
        if (x < 0 || x >= n || y < 0 || y >= m || grid[x][y] < 0 || grid[x][y] != word[i]) return false; 
        grid[x][y] -= 128; //标记已经访问
        bool flag = dfs(grid, x - 1, y, word, i + 1) || 
                    dfs(grid, x + 1, y, word, i + 1) || 
                    dfs(grid, x, y - 1, word, i + 1) || 
                    dfs(grid, x, y + 1, word, i + 1);
        grid[x][y] += 128; //回溯状态
        return flag;
    }
public:
    bool exist(vector<vector<char>>& board, string word) {
        if (word.empty()) return false;
        n = board.size(), m = board[0].size();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (board[i][j] == word[0]) {
                    if (dfs(board, i, j, word, 0)) return true;
                }
            }
        } 
        return false;
    }
};
```
执行结果如下所示：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了98.94% 的用户
内存消耗：8 MB, 在所有 C++ 提交中击败了69.73% 的用户
```
