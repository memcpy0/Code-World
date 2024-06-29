
Write an algorithm to print all ways of arranging n queens on an `n x n` chess board so that none of them share the same row, column, or diagonal. In this case, "diagonal" means all diagonals, not just the two that bisect the board.

**Notes**: This problem is a generalization of the original one in the book.

**Example:**
```swift
Input: 4
 Output: [[".Q..","...Q","Q...","..Q."],["..Q.","Q...","...Q",".Q.."]]
 Explanation: 4 queens has following two solutions
[
 [".Q..",  // solution 1
  "...Q",
  "Q...",
  "..Q."],

 ["..Q.",  // solution 2
  "Q...",
  "...Q",
  ".Q.."]
]
```

题意：八皇后，给出所有的解决方案。

---
### 解法 回溯
```cpp
class Solution {
private:
    vector<vector<string>> ans;
    vector<bool> colVis;
    vector<int> colNote;
    bool check(int curRow, int col) {
        for (int i = 0; i < curRow; ++i) //abs(前面的列号-当前选择的列号) == (当前的行号 - 之前的行号)
            if (abs(colNote[i] - col) == curRow - i) 
                return false;
        return true;
    }
    void dfs(int curRow, int n) {
        if (curRow >= n) {
            vector<string> temp;
            for (int i = 0; i < n; ++i) {
                string oneRow(n, '.');
                oneRow[colNote[i]] = 'Q';
                temp.push_back(oneRow);
            }
            ans.push_back(temp);
            return;
        }
        for (int i = 0; i < n; ++i) { //选择curRow行的列号
            if (colVis[i] || check(curRow, i) == false) continue;  //这一列已被选择或者和前面的选择成对角线
            colVis[i] = true;         //标记这一列已被选择
            colNote[curRow] = i;      //curRow行选择的是i列
            dfs(curRow + 1, n);       //递归
            colVis[i] = false;        //回溯
        }
    }
public:
    vector<vector<string>> solveNQueens(int n) {
        colVis.resize(n);
        colNote.resize(n);
        dfs(0, n);
        return ans;
    }
};
```
提交后效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了90.43% 的用户
内存消耗：8.5 MB, 在所有 C++ 提交中击败了29.88% 的用户
```
