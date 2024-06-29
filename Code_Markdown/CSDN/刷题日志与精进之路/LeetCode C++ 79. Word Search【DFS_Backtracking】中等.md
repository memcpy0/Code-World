Given a `2D` board and a word, find if the word exists in the grid.

The word can be constructed from letters of sequentially adjacent cell, where "adjacent" cells are those horizontally or vertically neighboring. The same letter cell may not be used more than once.

**Example:**
```cpp
board =
[
  ['A','B','C','E'],
  ['S','F','C','S'],
  ['A','D','E','E']
]

Given word = "ABCCED", return true.
Given word = "SEE", return true.
Given word = "ABCB", return false.
```
**Constraints:**
- `board` and `word` consists only of lowercase and uppercase English letters.
- `1 <= board.length <= 200`
- `1 <= board[i].length <= 200`
- `1 <= word.length <= 10^3`

题意：给出一个二维网格和一个单词，判断该单词是否存在于网格中。注意：**同一个单元格内的字母不允许被重复使用。**

---
### 解法 DFS+剪枝回溯
对每个和单词的第一个字符匹配的网格位置，进行DFS。如果匹配，就返回 `true` 。最后仍然没有找到时返回 `false` 。代码如下：
```cpp
class Solution {
private:
    vector<vector<bool>> vis;
    int Move[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    bool dfs(const vector<vector<char>>& board, int i, int j, const string& word, int idx) {
        vis[i][j] = true; //访问这一点
        ++idx;  //指向下一个要匹配的字符
        if (idx >= word.size()) return true; 
        for (int t = 0; t < 4; ++t) {
            int ti = i + Move[t][0], tj = j + Move[t][1];
            if (ti >= 0 && ti < board.size() && tj >= 0 && tj < board[ti].size() //ti,tj在范围内 
                && vis[ti][tj] == false && board[ti][tj] == word[idx] //避免重复访问,board字符和新字符匹配
                && dfs(board, ti, tj, word, idx)) return true; //深度访问
        } 
        vis[i][j] = false; //以这一点出发无法匹配,不进行访问
        return false;
    }
public:
    bool exist(vector<vector<char>>& board, string word) {
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board[i].size(); ++j) {
                if (board[i][j] == word[0]) {
                    vis.clear();
                    vis.resize(board.size());
                    for (int i = 0; i < vis.size(); ++i) vis[i].resize(board[i].size());
                    if (dfs(board, i, j, word, 0))
                        return true;
                }
            }
        }
        return false;
    }
};
```
代码写得不怎么优雅，而且效率也比较低：
```cpp
执行用时：224 ms, 在所有 C++ 提交中击败了29.59% 的用户
内存消耗：17.5 MB, 在所有 C++ 提交中击败了38.96% 的用户
```
后来修改了一下，比如 `vis` 数组不用反复 `resize` ，因为如果匹配失败，`vis` 数组**在回溯时会自动恢复原状**；使用 `row, col` 而不是反复调用 `size()` ：
```cpp
class Solution {
private: 
    int Move[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    bool dfs(const vector<vector<char>>& board, int i, int j, const string& word, int idx, 
            vector<vector<bool>>& vis) {
        vis[i][j] = true; //访问这一点
        ++idx;  //指向下一个要匹配的字符
        if (idx >= word.size()) return true; 
        for (int t = 0; t < 4; ++t) {
            int ti = i + Move[t][0], tj = j + Move[t][1];
            if (ti >= 0 && ti < board.size() && tj >= 0 && tj < board[0].size() //ti,tj在范围内 
                && vis[ti][tj] == false && board[ti][tj] == word[idx] //避免重复访问,board字符和新字符匹配
                && dfs(board, ti, tj, word, idx, vis)) return true; //深度访问
        } 
        return (vis[i][j] = false); //以这一点出发无法匹配,不进行访问 
    }
public:
    bool exist(vector<vector<char>>& board, string word) {
        if (board.empty() || board[0].empty()) return false;
        const int row = board.size(), col = board[0].size();
        vector<vector<bool>> vis(row, vector<bool>(col, false));
        for (int i = 0; i < row; ++i) 
            for (int j = 0; j < col; ++j) 
                if (board[i][j] == word[0] && dfs(board, i, j, word, 0, vis))
                        return true; 
        return false;
    }
};
```
效率如下，大大提高：
```cpp
执行用时：40 ms, 在所有 C++ 提交中击败了60.65% 的用户
内存消耗：8.4 MB, 在所有 C++ 提交中击败了59.16% 的用户
```
---
由于只有大小写字母，可以进一步优化，**复用原始的二维矩阵**。不过下面的写法，隐晦地修改了原始的 `board[][]` 矩阵（在匹配完成的时候）：
```cpp
class Solution {
private: 
    int Move[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    bool dfs(vector<vector<char>>& board, int i, int j, const string& word, int idx) {
        board[i][j] += 128; //访问这一点
        ++idx;  //指向下一个要匹配的字符
        if (idx >= word.size()) {
        	//board[i][j] -= 128; //需要加上这一句
        	return true; 
        }
        for (int t = 0; t < 4; ++t) {
            int ti = i + Move[t][0], tj = j + Move[t][1];
            if (ti >= 0 && ti < board.size() && tj >= 0 && tj < board[0].size() //ti,tj在范围内 
                && board[ti][tj] == word[idx] //避免重复访问,board字符和新字符匹配
                && dfs(board, ti, tj, word, idx)) return true; //深度访问
        } 
        board[i][j] -= 128;
        return false; //以这一点出发无法匹配,不进行访问 
    }
public:
    bool exist(vector<vector<char>>& board, string word) { 
        const int row = board.size(), col = board[0].size(); 
        for (int i = 0; i < row; ++i) 
            for (int j = 0; j < col; ++j) 
                if (board[i][j] == word[0] && dfs(board, i, j, word, 0))
                        return true; 
        return false;
    }
};
```
执行效率如下，可喜的进步：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了80.83% 的用户
内存消耗：7.7 MB, 在所有 C++ 提交中击败了90.20% 的用户
```
进一步修改，使得矩阵能够保持原状，代码风格更加优雅。也是本题的**最优解**：
```cpp
class Solution {
public:
    bool exist(vector<vector<char>> &board, string &word) {
        if (board.empty() || board[0].empty()) return false;
        const int row = board.size(), col = board[0].size();
        for (int i = 0; i < row; ++i) 
            for (int j = 0; j < col; ++j) 
                if (board[i][j] == word[0] && dfs(board, word, i, j, 0))
                    return true;
        return false;
    }
    bool dfs(vector<vector<char>> &board, const string &word, int i, int j, int idx) {
        if (idx >= word.size()) return true;
        if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size() || board[i][j] != word[idx]) return false;
        board[i][j] += 128;
        bool result = dfs(board, word, i - 1, j, idx + 1) || dfs(board, word, i + 1, j, idx + 1) 
            || dfs(board, word, i, j - 1, idx + 1) || dfs(board, word, i, j + 1, idx + 1);
        board[i][j] -= 128;
        return result;
    }
};
```
效率倒是没有太多变化：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了80.83% 的用户
内存消耗：7.8 MB, 在所有 C++ 提交中击败了81.94% 的用户
```
---
顺带一提，如果用Java写一段一模一样的代码，效率如下，时间仅为上者的 $1/4$ ，空间则消耗巨大，为上者的 $5$ 倍多：
```cpp
执行用时：7 ms, 在所有 Java 提交中击败了77.22% 的用户
内存消耗：42 MB, 在所有 Java 提交中击败了31.98% 的用户
```
即使用C写：
```c
bool dfs(char** board, int row, int col, int i, int j, char* word, int k) {
    if (word[k] == '\0') return true;
    if (i < 0 || i >= row || j < 0 || j >= col || board[i][j] != word[k]) return false;
    board[i][j] = '\0';
    bool result = dfs(board, row, col, i - 1, j, word, k + 1) ||
                  dfs(board, row, col, i + 1, j, word, k + 1) ||
                  dfs(board, row, col, i, j - 1, word, k + 1) ||
                  dfs(board, row, col, i, j + 1, word, k + 1);
    board[i][j] = word[k];
    return result;
}

bool exist(char** board, int boardSize, int* boardColSize, char* word){
    for (int i = 0; i < boardSize; ++i) 
        for (int j = 0; j < boardColSize[0]; ++j) 
            if (board[i][j] == word[0] && dfs(board, boardSize, *boardColSize, i, j, word, 0)) 
                return true;
    return false;
}
```
效率也没有这样高，为什么呢？
```c
执行用时：16 ms, 在所有 C 提交中击败了100.00% 的用户
内存消耗：6.5 MB, 在所有 C 提交中击败了64.76% 的用户
```
