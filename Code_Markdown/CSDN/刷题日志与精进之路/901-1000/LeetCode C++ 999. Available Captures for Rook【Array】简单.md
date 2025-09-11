<p>On an 8 x 8 chessboard, there is one white rook.&nbsp; There also may be empty squares, white bishops, and black pawns.&nbsp; These are given as characters 'R', '.', 'B', and 'p' respectively. Uppercase characters represent white pieces, and lowercase characters represent black pieces.</p>

<p>The rook moves as in the rules of Chess: it chooses one of four cardinal directions (north, east, west, and south), then moves in that direction until it chooses to stop, reaches the edge of the board, or captures an opposite colored pawn by moving to the same square it occupies.&nbsp; Also, rooks cannot move into the same square as other friendly bishops.</p>

<p>Return the number of pawns the rook can capture in one move.</p>

 
<p><strong>Example 1:</strong></p>

<p><img style="width: 300px; height: 305px;" src="https://assets.leetcode.com/uploads/2019/02/20/1253_example_1_improved.PNG" width="55%" alt=""></p>

```haskell
Input: [[".",".",".",".",".",".",".","."],[".",".",".","p",".",".",".","."],[".",".",".","R",".",".",".","p"],[".",".",".",".",".",".",".","."],[".",".",".",".",".",".",".","."],[".",".",".","p",".",".",".","."],[".",".",".",".",".",".",".","."],[".",".",".",".",".",".",".","."]]
Output: 3
Explanation: 
In this example the rook is able to capture all the pawns.
```

 
<p><strong>Example 2:</strong></p>

<p><img style="width: 300px; height: 306px;" src="https://assets.leetcode.com/uploads/2019/02/19/1253_example_2_improved.PNG" width="55%" alt=""></p>

```haskell
Input: [[".",".",".",".",".",".",".","."],[".","p","p","p","p","p",".","."],[".","p","p","B","p","p",".","."],[".","p","B","R","B","p",".","."],[".","p","p","B","p","p",".","."],[".","p","p","p","p","p",".","."],[".",".",".",".",".",".",".","."],[".",".",".",".",".",".",".","."]]
Output: 0
Explanation: 
Bishops are blocking the rook to capture any pawn.
```

 
<p><strong>Example 3:</strong></p>

<p><img style="width: 300px; height: 305px;" src="https://assets.leetcode.com/uploads/2019/02/20/1253_example_3_improved.PNG" width="55%" alt=""></p>

```haskell
Input: [[".",".",".",".",".",".",".","."],[".",".",".","p",".",".",".","."],[".",".",".","p",".",".",".","."],["p","p",".","R",".","p","B","."],[".",".",".",".",".",".",".","."],[".",".",".","B",".",".",".","."],[".",".",".","p",".",".",".","."],[".",".",".",".",".",".",".","."]]
Output: 3
Explanation: 
The rook can capture the pawns at positions b5, d6 and f5.
```

 
 

<p><strong>Note:</strong></p>

<ol>
	<li><code>board.length == board[i].length == 8</code></li>
	<li><code>board[i][j]</code> is either <code>'R'</code>, <code>'.'</code>, <code>'B'</code>, or&nbsp;<code>'p'</code></li>
	<li>There is exactly one cell with <code>board[i][j] == 'R'</code></li>
</ol>


题意：在一个 `8 x 8` 的棋盘上，有一个白色的车，用字符 `'R'` 表示。棋盘上还可能存在空方块，白色的象以及黑色的卒，分别用字符 `'.'`，`'B'` 和 `'p'` 表示。大写字符表示的是白棋，小写字符表示的是黑棋。

车按国际象棋中的规则移动。东，西，南，北四个基本方向任选其一，然后一直向选定的方向移动，直到满足下列四个条件之一：
- 棋手选择主动停下来
- 棋子因到达棋盘的边缘而停下
- 棋子移动到某一方格来捕获位于该方格上敌方（黑色）的卒，停在该方格内
- 车不能进入/越过已经放有其他友方棋子（白色的象）的方格，停在友方棋子前

现在可以控制车移动一次，统计有多少敌方的卒处于捕获范围内（即可以被一步捕获的棋子数）。 


---
### 解法 暴力
具体细节都写在代码里了：
```cpp
class Solution {
public:
    int numRookCaptures(vector<vector<char>>& board) {
        int ans = 0;
        bool rook = false;
        function<bool(char)> f = [&](char c) { //判断是否退出,计数
            if (c == '.') return false; 
            if (c == 'p') ++ans; 
            return true;
        };
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j] == 'R') { //先找到rook的位置
                    rook = true; //再扫描四个方向
                    for (int r = i - 1; r >= 0; --r) if (f(board[r][j])) break;
                    for (int r = i + 1; r < 8; ++r) if (f(board[r][j])) break;
                    for (int c = j - 1; c >= 0; --c) if (f(board[i][c])) break;
                    for (int c = j + 1; c < 8; ++c) if (f(board[i][c])) break;
                    break;
                }
            }
            if (rook) break;
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.6 MB, 在所有 C++ 提交中击败了12.80% 的用户
```
