> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091355681.png)
Write a program to solve a Sudoku puzzle by filling the empty cells.
A sudoku solution must satisfy <strong>all of the following rules</strong>:
<ol>
	<li>Each of the digits <code>1-9</code> must occur exactly once in each row.</li>
	<li>Each of the digits <code>1-9</code> must occur exactly once in each column.</li>
	<li>Each of the digits <code>1-9</code> must occur exactly once in each of the 9 <code>3x3</code> sub-boxes of the grid.</li>
</ol>
The <code>'.'</code> character indicates empty cells.

<strong>Example 1:</strong>
![在这里插入图片描述](https://img-blog.csdnimg.cn/99ee0c456d24407a893145b3c90ba4ef.png)
<pre><strong>Input:</strong> board = [["5","3",".",".","7",".",".",".","."],["6",".",".","1","9","5",".",".","."],[".","9","8",".",".",".",".","6","."],["8",".",".",".","6",".",".",".","3"],["4",".",".","8",".","3",".",".","1"],["7",".",".",".","2",".",".",".","6"],[".","6",".",".",".",".","2","8","."],[".",".",".","4","1","9",".",".","5"],[".",".",".",".","8",".",".","7","9"]]
<strong>Output:</strong> [["5","3","4","6","7","8","9","1","2"],["6","7","2","1","9","5","3","4","8"],["1","9","8","3","4","2","5","6","7"],["8","5","9","7","6","1","4","2","3"],["4","2","6","8","5","3","7","9","1"],["7","1","3","9","2","4","8","5","6"],["9","6","1","5","3","7","2","8","4"],["2","8","7","4","1","9","6","3","5"],["3","4","5","2","8","6","1","7","9"]]
<strong>Explanation:</strong> The input board is shown above and the only valid solution is shown below:
</pre>
![在这里插入图片描述](https://img-blog.csdnimg.cn/d98278e34f014d9cbffeb8398fdeb614.png)

<strong>Constraints:</strong>
<ul>
	<li><code>board.length == 9</code></li>
	<li><code>board[i].length == 9</code></li>
	<li><code>board[i][j]</code> is a digit or <code>'.'</code>.</li>
	<li>It is <strong>guaranteed</strong> that the input board has only one solution.</li>
</ul>
题意：编写一个程序，通过填充空格来解决数独问题。数独的解法需 **遵循如下规则**：
1.  数字 `1-9` 在每一行只能出现一次。
2.  数字 `1-9` 在每一列只能出现一次。
3.  数字 `1-9` 在每一个以粗实线分隔的 `3x3` 宫内只能出现一次。

题目数据 **保证** 输入数独仅有一个解。

---
### 解法 递归+回溯
类似于N皇后，本题属于**经典回溯算法裸题**，我们可以使用回溯算法来解决。注意，此类题目具有明显的特征——数据范围很小，比如数独固定大小为 `9x9` 、N皇后的 `N` 一般不超过 `13` 。

上一题[36. Valid Sudoku](https://leetcode-cn.com/problems/valid-sudoku/)相对简单，只需要判断给定的 `borad` 是否为有效数独。这一题就要难上一些，需要给出「仅有一个解」的有效数独的解法。

具体来说，我们对每个应该填入数字的位置，填入符合出现规则的未使用的某数后，递归填写下一个格子：
- 如果递归结果为成功，则 `return true` 继续返回；
- 否则尝试填入下一个数字；
- 尝试完 `1 ~ 9` 后仍无法填入，则说明之前有地方填错了，我们 `return false` 进行回溯。

由于极端情况下，每个格子都需要枚举，且每个格子都可以从 `1` 枚举到 `9` ，所以枚举次数最大为 `9 * 9 * 9 = 729` ，即复杂度不随着数据变化而变化，时间复杂度为 $O(1)$ ，空间复杂度为 $O(1)$ ：
```cpp
class Solution {
private:
    // rows[i][j],cols[i][j],blocks[i][j]分别表示在第i行/列/个小数独是否出现数j
    int rows[9], cols[9], blocks[9]; 
    void resetNum(int i, int j, int b, int n) { 
        rows[i] ^= (1 << n), cols[j] ^= (1 << n), blocks[b] ^= (1 << n);
    }
    bool dfs(vector<vector<char>>& board, int i, int j) {
        if (j == 9) return dfs(board, i + 1, 0); // 某行被填完,转向下一行
        if (i == 9) return true; // 整个数独被填完
        if (board[i][j] != '.') return dfs(board, i, j + 1); // 填当前行的下一列   
        for (int k = 0; k < 9; ++k) { // 对每个格子填1-9
            int b = i / 3 * 3 + j / 3;
            if (!(rows[i] >> k & 1) && !(cols[j] >> k & 1) && !(blocks[b] >> k & 1)) { // 该数在第i行,第j列,第b个小数独未出现
                board[i][j] = (char)(k + '1');
                resetNum(i, j, b, k);
                if (dfs(board, i, j + 1)) return true; // 填当前行的下一列,成功则返回true
                board[i][j] = '.';
                resetNum(i, j, b, k);
            }
        }
        return false;
    }
public:
    void solveSudoku(vector<vector<char>>& board) {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j) 
                if (board[i][j] != '.')  //事先记录每行/每列/每个3x3宫内哪些数字已被使用 
                    resetNum(i, j, i / 3 * 3 + j / 3, board[i][j] - '1'); // 从0开始
        dfs(board, 0, 0); // 先填每行,再填每列 
    }
}; 
```
---
### 相同题目：牛客HJ44 Sudoku
```cpp
#include <iostream>
using namespace std;
char board[9][9];
int rs[9], cs[9], bs[9];
void set(int i, int j, int b, int n) {
    rs[i] |= (1 << n), cs[j] |= (1 << n), bs[b] |= (1 << n);
}
void reset(int i, int j, int b, int n) {
    rs[i] ^= (1 << n), cs[j] ^= (1 << n), bs[b] ^= (1 << n);
}
bool dfs(int i, int j) {
    if (j == 9) return dfs(i + 1, 0);
    if (i == 9) return true;
    if (board[i][j] != '0') return dfs(i, j + 1);
    for (int k = 1; k <= 9; ++k) {
        int b = i / 3 * 3 + j / 3;
        if (!(rs[i] >> k & 1) && !(cs[j] >> k & 1) && !(bs[b] >> k & 1)) {
            set(i, j, b, k);
            board[i][j] = k + '0';
            if (dfs(i, j + 1)) return true;
            board[i][j] = '0';
            reset(i, j, b, k);
        }
    }
    return false;
}
int main() {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            cin >> board[i][j];
            if (board[i][j] != '0')
                set(i, j, i / 3 * 3 + j / 3, board[i][j] - '0');
        }
    }
    dfs(0, 0);
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (j != 0) cout << " ";
            cout << board[i][j];
        }
        cout << endl;
    }
    return 0;
}
// 64 位输出请用 printf("%lld")
```