Let's play the minesweeper game!

You are given a `2D` char matrix representing the game board. `'M'` represents an **unrevealed** mine, `'E'` represents an **unrevealed** empty square, `'B'` represents a **revealed** blank square that has no adjacent (above, below, left, right, and all `4` diagonals) mines, digit (`'1'` to `'8'`) represents how many mines are adjacent to this **revealed** square, and finally `'X'` represents a **revealed** mine.

Now given **the next click position** (row and column indices) among all the **unrevealed** squares (`'M'` or `'E'`), return the board after revealing this position according to the following rules:
-    If a mine (`'M'`) is revealed, then the game is over - change it to `'X'` .
-    If an empty square (`'E'`) with **no adjacent mines** is revealed, then change it to revealed blank (`'B'`) and all of its adjacent **unrevealed** squares should be revealed recursively.
-    If an empty square (`'E'`) with **at least one adjacent mine** is revealed, then change it to a digit (`'1'` to `'8'`) representing the number of adjacent mines.
-   Return the board when no more squares will be revealed.

 **Example 1:**

```cpp
Input: 
[['E', 'E', 'E', 'E', 'E'],
 ['E', 'E', 'M', 'E', 'E'],
 ['E', 'E', 'E', 'E', 'E'],
 ['E', 'E', 'E', 'E', 'E']]

Click : [3,0]

Output: 
[['B', '1', 'E', '1', 'B'],
 ['B', '1', 'M', '1', 'B'],
 ['B', '1', '1', '1', 'B'],
 ['B', 'B', 'B', 'B', 'B']]
```
**Explanation:**
<img src="https://img-blog.csdnimg.cn/20200820172520329.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
 
**Example 2:**
```cpp
Input: 
[['B', '1', 'E', '1', 'B'],
 ['B', '1', 'M', '1', 'B'],
 ['B', '1', '1', '1', 'B'],
 ['B', 'B', 'B', 'B', 'B']]

Click : [1,2]

Output: 
[['B', '1', 'E', '1', 'B'],
 ['B', '1', 'X', '1', 'B'],
 ['B', '1', '1', '1', 'B'],
 ['B', 'B', 'B', 'B', 'B']]
```
**Explanation:**
<img src="https://img-blog.csdnimg.cn/20200820172550604.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">

**Note:**
-    The range of the input matrix's height and width is `[1,50]` .
-    The click position will only be an unrevealed square (`'M'` or `'E'`), which also means the input board contains at least one clickable square.
-    The input board won't be a stage when game is over (some mines have been revealed).
 -   For simplicity, not mentioned rules should be ignored in this problem. For example, you don't need to reveal all the unrevealed mines when the game is over, consider any cases that you will win the game or flag any squares.

题意：扫雷游戏。其中给出下一次的点击位置，点击的只会是未被挖出的方块，我们需要返回点击后的字符矩阵——如果点击到的是地雷 `M` ，那么需要改成 `X` ，然后游戏就结束了；如果点击到的是周围存在地雷的空方块 `E` ，就需要修改它为地雷的数量；如果点击到的是周围不存在地雷的空方块 `E` ，就修改它为 `B` ，然后递归揭露相邻的未挖出的空方块。

---
思路1：深度优先搜索，挖的时候先将该位置的 `E` 修改为 `B` ，然后统计周围地雷的个数：数量为零时递归挖周围的 `E` ；存在地雷时修改为地雷的数量，直接返回。

代码如下：
```cpp
class Solution {
public:
    void dfs(vector<vector<char>>& b, int r, int c, const vector<vector<int>> &Move) { 
        b[r][c] = 'B';
        /* 统计周边地雷的个数 */
        int nMines = 0;
        for (int i = 0; i < 8; ++i) {
            int tr = r + Move[i][0], tc = c + Move[i][1];
            if (tr >= 0 && tr < b.size() && tc >= 0 && tc < b[0].size() && b[tr][tc] == 'M') ++nMines;
        } 
    	/* 地雷个数非0, 标记为'0' + nMines */
        if (nMines != 0) {
            b[r][c] = '0' + nMines; 
            return;
        }
        /* 地雷个数为0, 标记为B, 递归处理周边元素 */
        for (int i = 0; i < 8; ++i) {
            int tr = r + Move[i][0], tc = c + Move[i][1];
            if (tr >= 0 && tr < b.size() && tc >= 0 && tc < b[0].size() && b[tr][tc] == 'E') 
                dfs(b, tr, tc, Move);
        }    
    }

    vector<vector<char>> updateBoard(vector<vector<char>>& board, vector<int>& click) {
        int r = click[0], c = click[1];
        if (board[r][c] == 'M') {
            board[r][c] = 'X';
            return board;
        }
        vector<vector<int>> Move = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
        dfs(board, r, c, Move);
        return board;
    }
};
```
效率：
```cpp
执行用时：108 ms, 在所有 C++ 提交中击败了19.60% 的用户
内存消耗：16.4 MB, 在所有 C++ 提交中击败了16.15% 的用户
```
---
思路2：广度优先搜索。注意：访问当前结点时会将周边的可访问结点进队，此时需对它们进行一下标记，否则可能造成某些结点重复进队的问题，从而超出时间限制。

代码如下：
```cpp
class Solution {
public: 
    int dx[8] ={0, 1, 0, -1, -1, 1, -1, 1};
    int dy[8] ={1, 0, -1, 0, -1, -1, 1, 1};
    vector<vector<char>> updateBoard(vector<vector<char>>& board, vector<int>& click) {
        int r = click[0], c = click[1], m = board.size(), n = board[0].size();
        if (board[r][c] == 'M') {
            board[r][c] = 'X';
            return board;
        } 
        queue<pair<int, int>> q;
        q.push({r, c});
        while (!q.empty()) {
            pair<int, int> tmp = q.front();
            q.pop();  
            r = tmp.first, c = tmp.second;
            vector<pair<int, int>> v_pair;

            int numMines = 0;
            for (int i = 0; i < 8; ++i) {  //记录周围的地雷数量和可能访问的位置
                int tr = r + dx[i], tc = c + dy[i];
                if (tr >= 0 && tr < m && tc >= 0 && tc < n) {
                    if (board[tr][tc] == 'M') ++numMines;
                    else if (board[tr][tc] == 'E') v_pair.push_back({tr, tc});
                }
            }
            if (numMines != 0) board[r][c] = '0' + numMines;
            else {
                board[r][c] = 'B';
                for (auto p : v_pair) {
                    board[p.first][p.second] = '0';   
                    q.push(p);
                }
            }
        }
        return board;
    }
};
```
效率：
```cpp
执行用时：68 ms, 在所有 C++ 提交中击败了76.70% 的用户
内存消耗：13.8 MB, 在所有 C++ 提交中击败了57.81% 的用户
```
