> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091355454.png)
请你判断一个 `9 x 9` 的数独是否有效。只需要 **根据以下规则** ，验证已经填入的数字是否有效即可。
1. 数字 `1-9` 在每一行只能出现一次。
2. 数字 `1-9` 在每一列只能出现一次。
3. 数字 `1-9` 在每一个以粗实线分隔的 `3x3` 宫内只能出现一次。（请参考示例图）

**注意：**
- 一个有效的数独（部分已被填充）不一定是可解的。
- 只需要根据以上规则，验证已经填入的数字是否有效即可。
- 空白格用 `'.'` 表示。

**示例 1：**
![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2021/04/12/250px-sudoku-by-l2g-20050714svg.png)
```java
输入：board = 
[["5","3",".",".","7",".",".",".","."]
,["6",".",".","1","9","5",".",".","."]
,[".","9","8",".",".",".",".","6","."]
,["8",".",".",".","6",".",".",".","3"]
,["4",".",".","8",".","3",".",".","1"]
,["7",".",".",".","2",".",".",".","6"]
,[".","6",".",".",".",".","2","8","."]
,[".",".",".","4","1","9",".",".","5"]
,[".",".",".",".","8",".",".","7","9"]]
输出：true
```
**示例 2：**
```java
输入：board = 
[["8","3",".",".","7",".",".",".","."]
,["6",".",".","1","9","5",".",".","."]
,[".","9","8",".",".",".",".","6","."]
,["8",".",".",".","6",".",".",".","3"]
,["4",".",".","8",".","3",".",".","1"]
,["7",".",".",".","2",".",".",".","6"]
,[".","6",".",".",".",".","2","8","."]
,[".",".",".","4","1","9",".",".","5"]
,[".",".",".",".","8",".",".","7","9"]]
输出：false
解释：除了第一行的第一个数字从 5 改为 8 以外，空格内其他数字均与 示例1 相同。 但由于位于左上角的 3x3 宫内有两个 8 存在, 因此这个数独是无效的。
```
**提示：**
- `board.length == 9`
- `board[i].length == 9`
- `board[i][j]` 是一位数字（`1-9`）或者 `'.'`

---
有效的数独满足以下三个条件：

同一个数字在每一行只能出现一次；

同一个数字在每一列只能出现一次；

同一个数字在每一个小九宫格只能出现一次。

可以使用哈希表记录每一行、每一列和每一个小九宫格中，每个数字出现的次数。只需要遍历数独一次，在遍历的过程中更新哈希表中的计数，并判断是否满足有效的数独的条件即可。

对于数独的第 iii 行第 jjj 列的单元格，其中 0≤i,j<90 \le i, j < 90≤i,j<9，该单元格所在的行下标和列下标分别为 iii 和 jjj，该单元格所在的小九宫格的行数和列数分别为 ⌊i3⌋\Big\lfloor \dfrac{i}{3} \Big\rfloor⌊ 
3
i
​
 ⌋ 和 ⌊j3⌋\Big\lfloor \dfrac{j}{3} \Big\rfloor⌊ 
3
j
​
 ⌋，其中 0≤⌊i3⌋,⌊j3⌋<30 \le \Big\lfloor \dfrac{i}{3} \Big\rfloor, \Big\lfloor \dfrac{j}{3} \Big\rfloor < 30≤⌊ 
3
i
​
 ⌋,⌊ 
3
j
​
 ⌋<3。

由于数独中的数字范围是 111 到 999，因此可以使用数组代替哈希表进行计数。

具体做法是，创建二维数组 rows\textit{rows}rows 和 columns\textit{columns}columns 分别记录数独的每一行和每一列中的每个数字的出现次数，创建三维数组 subboxes\textit{subboxes}subboxes 记录数独的每一个小九宫格中的每个数字的出现次数，其中 rows[i][index]\textit{rows}[i][\textit{index}]rows[i][index]、columns[j][index]\textit{columns}[j][\textit{index}]columns[j][index] 和 subboxes[⌊i3⌋][⌊j3⌋][index]\textit{subboxes}\Big[\Big\lfloor \dfrac{i}{3} \Big\rfloor\Big]\Big[\Big\lfloor \dfrac{j}{3} \Big\rfloor\Big]\Big[\textit{index}\Big]subboxes[⌊ 
3
i
​
 ⌋][⌊ 
3
j
​
 ⌋][index] 分别表示数独的第 iii 行第 jjj 列的单元格所在的行、列和小九宫格中，数字 index+1\textit{index} + 1index+1 出现的次数，其中 0≤index<90 \le \textit{index} < 90≤index<9，对应的数字 index+1\textit{index} + 1index+1 满足 1≤index+1≤91 \le \textit{index} + 1 \le 91≤index+1≤9。

如果 board[i][j]\textit{board}[i][j]board[i][j] 填入了数字 nnn，则将 rows[i][n−1]\textit{rows}[i][n - 1]rows[i][n−1]、columns[j][n−1]\textit{columns}[j][n - 1]columns[j][n−1] 和 subboxes[⌊i3⌋][⌊j3⌋][n−1]\textit{subboxes}\Big[\Big\lfloor \dfrac{i}{3} \Big\rfloor\Big]\Big[\Big\lfloor \dfrac{j}{3} \Big\rfloor\Big]\Big[n - 1\Big]subboxes[⌊ 
3
i
​
 ⌋][⌊ 
3
j
​
 ⌋][n−1] 各加 111。如果更新后的计数大于 111，则不符合有效的数独的条件，返回 false\text{false}false。

如果遍历结束之后没有出现计数大于 111 的情况，则符合有效的数独的条件，返回 true\text{true}true。

作者：力扣官方题解
链接：https://leetcode.cn/problems/valid-sudoku/solutions/1001859/you-xiao-de-shu-du-by-leetcode-solution-50m6/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
### 解法1 哈希表
此题只需要我们判断是否为有效的数独，所以只需对 `board` 中出现的数进行判断，如果有数违反了数独的规则，返回 `false` ，否则最后返回 `true` 。

直觉上，我们可以用哈希表来记录某行/某列/某个 `3x3` 宫内哪些数字已经出现，帮助我们判断是否符合「有效数独」的定义。唯一的难点在于，如何确定下标 `(i, j)` 处的数落在哪个 `3x3` 宫内——做法是，先将行下标 `i` 除以 `3` 映射到 `0, 1, 2` ，再乘以 `3` 映射到 `0, 3, 6` ，作为九个 `3x3` 宫内左侧自上而下三个 `3x3` 宫的标号；然后，将 `j` 除以 `3` 同样映射为 `0, 1, 2` ，加上 `i / 3 * 3` ，即得到各个 `3x3` 宫与行列的关系：`blockIdx = i / 3 * 3 + j / 3` 。

实际代码如下所示。算法的时间复杂度为 $O(1)$ ，空间复杂度为 $O(1)$ ，因为是在固定 `9x9` 的问题中：
```cpp
//C++ version
class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        //row[i][num],col[i][num]判断i行或i列中数字(num+1)是否出现
        //blocks[i][num]判断第i个3x3矩阵内数字(num+1)是否出现
        bool row[9][9] = {false}, col[9][9] = {false}, blocks[9][9] = {false}; 
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] == '.') continue;
                int num = board[i][j] - '1', blockIdk = i / 3 * 3 + j / 3;
                if (row[i][num] || col[j][num] || blocks[blockIdk][num]) return false;
                row[i][num] = col[j][num] = blocks[blockIdk][num] = true;
            }
        }
        return true;
    }
};
//执行用时：8 ms, 在所有 C++ 提交中击败了99.29% 的用户
//内存消耗：17.5 MB, 在所有 C++ 提交中击败了82.45% 的用户
```
---
### 解法2 位运算
更进一步，我们可以使用一个 `int` 的低九位（或者直接用 `bitset<9>` ），来记录某行/某列/某个 `3x3` 宫内哪些数字已经出现。如 <code>(...1 0000 1011)<sub>2</sub></code> 表示数字 `1, 2, 4, 9` 已被填入。

```cpp
//C++ version
class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        //row[i][num],col[i][num]判断i行或i列中数字(num+1)是否出现
        //blocks[i][num]判断第i个3x3矩阵内数字(num+1)是否出现
        bitset<9> row[9], col[9], blocks[9];
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] == '.') continue;
                int num = board[i][j] - '1', blockIdk = i / 3 * 3 + j / 3;
                if (row[i][num] || col[j][num] || blocks[blockIdk][num]) return false;
                row[i][num] = col[j][num] = blocks[blockIdk][num] = true;
            }
        }
        return true;
    }
};
//执行用时：16 ms, 在所有 C++ 提交中击败了84.82% 的用户
//内存消耗：17.5 MB, 在所有 C++ 提交中击败了76.27% 的用户
```
尽管较 `bitset` 有浪费空间的嫌疑，这里还是演示一下用 `int` 的做法：
```cpp
//C++ version
class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        int row[9] = {0}, col[9] = {0}, blocks[9] = {0};
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] == '.') continue;
                int u = board[i][j] - '1', b = i / 3 * 3 + j / 3;
                if (((row[i] >> u) & 1) || ((col[j] >> u) & 1) || ((blocks[b] >> u) & 1)) return false;
                row[i] |= (1 << u); col[j] |= (1 << u); blocks[b] |= (1 << u);
            }
        }
        return true;
    }
};
//执行用时：12 ms, 在所有 C++ 提交中击败了95.97% 的用户
//内存消耗：17.3 MB, 在所有 C++ 提交中击败了99.55% 的用户
```
