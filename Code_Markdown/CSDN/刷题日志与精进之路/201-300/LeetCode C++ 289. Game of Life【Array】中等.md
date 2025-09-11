
According to the Wikipedia's article: "The **Game of Life**, also known simply as **Life**, is a cellular automaton devised by the British mathematician John Horton Conway in 1970."

Given a `board` with `m` by `n` cells, each cell has an initial state *live* (1) or *dead* (0). Each cell interacts with its **eight neighbors** (horizontal, vertical, diagonal) using the following four rules (taken from the above Wikipedia article):

1.  Any live cell with fewer than two live neighbors dies, as if caused by under-population.
 2. Any live cell with two or three live neighbors lives on to the next generation.
 3. Any live cell with more than three live neighbors dies, as if by over-population..
  4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

Write a function to compute the next state (after one update) of the board given its current state. The next state is created by applying the above rules simultaneously to every cell in the current state, where births and deaths occur simultaneously.

**Example:**

```swift
Input: 
[
  [0,1,0],
  [0,0,1],
  [1,1,1],
  [0,0,0]
]
Output: 
[
  [0,0,0],
  [1,0,1],
  [0,1,1],
  [0,1,0]
]
```

**Follow up:**
- Could you solve it in-place? Remember that the board needs to be updated at the same time: You cannot update some cells first and then use their updated values to update other cells.
-  In this question, we represent the board using a 2D array. In principle, the board is infinite, which would cause problems when the active area encroaches the border of the array. How would you address these problems?

题意：给定一个包含 `m × n` 个格子的面板，每一个格子都可以看成是一个细胞。每个细胞都具有一个初始状态：`1` 即为活细胞， `0` 即为死细胞。每个细胞与其八个相邻位置（水平，垂直，对角线）的细胞都遵循以下四条生存定律：

- 如果活细胞周围八个位置的活细胞数少于两个，则该位置活细胞死亡；
-  如果活细胞周围八个位置有两个或三个活细胞，则该位置活细胞仍然存活；
-  如果活细胞周围八个位置有超过三个活细胞，则该位置活细胞死亡；
- 如果死细胞周围正好有三个活细胞，则该位置死细胞复活；

根据当前状态，计算面板上所有细胞的下一个（一次更新后的）状态。下一个状态是通过将上述规则同时应用于当前状态下的每个细胞所形成的。

 
---
### 解法1 辅助空间
开一个 $O(nm)$ 的二维数组，然后更新原 `board` ：
```cpp
class Solution {
public:
    void gameOfLife(vector<vector<int>>& board) {
        int n = board.size(), m = board[0].size();
        vector<vector<int>> temp(n, vector<int>(m));
        int Moves[][2] = {0, 1, 0, -1, 1, 0, -1, 0, -1, -1, -1, 1, 1, -1, 1, 1};
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                int cells = 0;
                for (int k = 0; k < 8; ++k) {
                    int x = i + Moves[k][0], y = j + Moves[k][1];
                    if (x >= 0 && x < n && y >= 0 && y < m && board[x][y] == 1) ++cells;
                }
                if (board[i][j] == 1) { //活细胞
                    if (cells < 2) temp[i][j] = 0; //周围活细胞数<2
                    else if (cells <= 3) temp[i][j] = 1; //活细胞数==2,3
                    else temp[i][j] = 0;           //周围活细胞数>3
                } else if (cells == 3) temp[i][j] = 1; //死细胞但是周围正好有3个活细胞
            }
        }
        board = temp;
    }
};
```
第一次提交结果如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7.4 MB, 在所有 C++ 提交中击败了7.53% 的用户
```
---
### 解法2 原地工作
想要做到不使用额外空间，就要利用好输入。这题的输入是 `vector<vector<int>>` ，`int` 可以存储更多的比特位——让原有的最低位存储当前状态，第二低位存储下个状态，最后全部右移一位即可。相当于原有的状态只有 `00, 01` ，现在新增加了 `10, 11` 两个状态。具体代码如下:
```cpp
class Solution {
public:
    void gameOfLife(vector<vector<int>>& board) {
        if (board.empty()) return;
        int n = board.size(), m = board[0].size();
        /*
        00: dead (next state) <- dead (current state)
        01: dead (next state) <- live (current state) 
        10: live (next state) <- dead (current state)
        11: live (next state) <- live (current state) 
        */
        int Moves[][2] = {0, 1, 0, -1, 1, 0, -1, 0, -1, -1, -1, 1, 1, -1, 1, 1};
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                int lives = 0;
                for (int k = 0; k < 8; ++k) {
                    int x = i + Moves[k][0], y = j + Moves[k][1]; //查看周围细胞的状态(01,11活细胞)
                    if (x >= 0 && x < n && y >= 0 && y < m) lives += (board[x][y] & 1); 
                }
                if (board[i][j] & 1) { //当前活细胞,状态是0b01
                    if (2 <= lives && lives <= 3) board[i][j] = 3; //活细胞数为2/3,仍是活细胞0b11 
                    //else board[i][j] = 1;  //周围活细胞数<2或者>3,下个状态是死细胞0b01,不用赋值
                } else if (lives == 3) board[i][j] = 2; //当前死细胞,周围正好有3个活细胞,转为活细胞0b10
            }
        }
        for (int i = 0; i < n; ++i) 
            for (int j = 0; j < m; ++j)
                board[i][j] >>= 1; //全部右移一位
    }
};
```
空间效率提升不算多：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7.1 MB, 在所有 C++ 提交中击败了33.16% 的用户
```
