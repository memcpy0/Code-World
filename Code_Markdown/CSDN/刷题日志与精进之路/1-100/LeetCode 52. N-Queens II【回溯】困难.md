> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091739111.png)

The n-queens puzzle is the problem of placing *n* queens on an *n×n* chessboard such that no two queens attack each other.
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201018000311289.png#pic_center)

Given an integer `n` , return the number of **distinct** solutions to the n-queens puzzle.

**Example:**
```clike
Input: 4
Output: 2
Explanation: There are two distinct solutions to the 4-queens puzzle as shown below.
[
 [".Q..",  // Solution 1
  "...Q",
  "Q...",
  "..Q."],

 ["..Q.",  // Solution 2
  "Q...",
  "...Q",
  ".Q.."]
]
```

题意：计算合法的N皇后方案个数。

---
### 解法1 回溯法
这种题目做过很多次了，代码如下：
```cpp
class Solution {
private:
    vector<bool> colExist;
    vector<int> colOfRows; //colOfRows记录每一行使用的列号
    int ans = 0, maxRow, maxCol;
    //通过curRow的变化防止行的重复,通过colExist防止列的重复,通过check和colOfRows防止对角线的重复
    bool check(int curRow, int col) { 
        for (int i = 0; i < curRow; ++i) 
            if (abs(colOfRows[i] - col) == abs(i - curRow)) return true;
        return false;
    }
    void dfs(int curRow) {
        if (curRow >= maxRow) { ++ans; return; }
        for (int i = 0; i < maxCol; ++i) {
            if (colExist[i] || check(curRow, i)) continue;
            colExist[i] = true;     //选择了第i列 
            colOfRows[curRow] = i;  //记录第i列的使用 
            dfs(curRow + 1);
            colExist[i] = false;
        }
    }
public:
    int totalNQueens(int n) { 
        maxRow = maxCol = n;
        colExist.resize(n);
        colOfRows.resize(n);
        dfs(0);
        return ans;
    }
};
```
提交后结果如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了82.83% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了30.69% 的用户
```
---
### 解法二 打表
最快的方法：
```cpp
class Solution { 
public:
    const int table[20] = {0, 1, 0, 0, 2, 10, 4, 40, 92, 352, 724, 2680, 14200, 73712};
    int totalNQueens(int n) { return table[n]; }
};
```
提交后结果如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.9 MB, 在所有 C++ 提交中击败了67.39% 的用户
```
