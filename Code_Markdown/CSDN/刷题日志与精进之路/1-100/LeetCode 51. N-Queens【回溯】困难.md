> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091737322.png)

The n-queens puzzle is the problem of placing `n` queens on an `n×n` chessboard such that no two queens attack each other.

Given an integer `n` , return all distinct solutions to the n-queens puzzle.

Each solution contains a distinct board configuration of the n-queens' placement, where `'Q'` and `'.'` both indicate a queen and an empty space respectively.

**Example:**
```cpp
Input: 4
Output: [
 [".Q..",  // Solution 1
  "...Q",
  "Q...",
  "..Q."],

 ["..Q.",  // Solution 2
  "Q...",
  "...Q",
  ".Q.."]
]
Explanation: There exist two distinct solutions to the 4-queens puzzle as shown above.
```

题意：返回N皇后的具体方案。

---
思路：做过很多次的题目了，就是形成方案的时候有点麻烦。代码如下：
```cpp
class Solution {
public:
    vector<int> colNote;    //记录之前的每行(以1开始)的列号
    vector<bool> colVis;    //记录列号是否被使用过
    vector<vector<string>> ans;    //记录答案
    string rowstr;          //记录每行的字符串答案
    int endGame;
    bool check(int row, int col) {
        for (int j = 0; j < row; ++j) 
            if (abs(col - colNote[j]) == abs(row - j))  //和前几行是否成对角线
                return false;
        return true;
    }
    void dfs(int row) {
        if (row >= endGame) { 
            vector<string> tmp;
            for (int i = 0; i < colNote.size(); ++i) { //得到每行的列号
                tmp.push_back(rowstr);    
                tmp[i][colNote[i]] = 'Q'; //原来记录的行列都从1开始
            }
            ans.push_back(tmp);
            return;
        }
        for (int i = 0; i < endGame; ++i) {
            if (!colVis[i] && check(row, i)) { //和前几行的列号是否相等或者成对角线
                colNote[row] = i; 
                colVis[i] = true;
                dfs(row + 1);
                colVis[i] = false;
            }
        }
    }
    vector<vector<string>> solveNQueens(int n) {
        if (n == 0 || n == 2 || n == 3) return vector<vector<string>>();
        colNote.resize(n, 0);
        colVis.resize(n, false);
        endGame = n; 
        rowstr.resize(n, '.');
        dfs(0); //从第0行开始
        return ans;
    }
};
```
感觉写的不咋地，但还是比Python快得多：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020090323555735.png)

