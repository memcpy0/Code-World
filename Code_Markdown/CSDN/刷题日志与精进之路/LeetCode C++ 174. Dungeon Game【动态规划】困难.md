The demons had captured the princess (`P`) and imprisoned her in the **bottom-right** corner of a dungeon. The dungeon consists of `M x N` rooms laid out in a `2D` grid. Our valiant knight (`K`) was initially positioned in the **top-left** room and must fight his way through the dungeon to rescue the princess.

The knight has an initial health point represented by a **positive** integer. If at any point his health point drops to `0` or below, he dies immediately.

Some of the rooms are guarded by demons, so the knight loses health (negative integers) upon entering these rooms; other rooms are either empty (`0's`) or contain magic orbs that increase the knight's health (positive integers).

In order to reach the princess as quickly as possible, the knight decides to move only **rightward or downward** in each step.


Write a function to determine the knight's **minimum** initial health so that he is able to rescue the princess.

For example, given the dungeon below, the initial health of the knight must be at least `7` if he follows the optimal path `RIGHT-> RIGHT -> DOWN -> DOWN` .

|  列0 |   列1| 列2  |
|:-:|:-:|:-:|
| -2 (K) |	-3 |	3
| -5 |	-10 | 	1
|10 |	30 |	-5 | (P)


Note:
-    The knight's health has **no upper bound**.
-    Any room can contain threats or power-ups, even the first room the knight enters and the bottom-right room where the princess is imprisoned.

题意：恶魔抓走了公主，将她囚禁在地下城的最右下角，地下城是一个有着 `M*N` 个格子的二维平面，里面任何一个格子都有危险（减少骑士的健康值）或者是力量剂（增加骑士的健康值），即使是地下城的入口（左上角）和公主所在的右下角房间。

骑士从左上角，每次只能够向右或者向下移动一步，可能遇到空房间，或者恶魔守卫的房间（丢失健康值），或者是魔药（增加健康值），最终，骑士到达右下角，完成穿过地下城拯救公主的伟业。

要注意的是，任何时候，只要骑士的健康值跌为零或更低，他就会立刻死亡。因此，询问：骑士能够拯救到公主所需的**最低初始健康值**是多少？



---

思路：这是个动态规划的问题，尽管标签是困难，但是我认为应该是中等难度。用一个二维数组 `dp` ，`dp[i][j]` 是从 `(i,j)` 出发到达右下角所需要的最低初始健康值。然后，从终点向起点递推即可。代码中有几种情况：
- 终点，如果终点房间的 `dungeon[i][j]` 小于零，说明有危险，则 `dp[i][j] = abs(dungeon[i][j]) + 1`  ；否则只需要初始的 `1` 点健康值就可以了；
- 最后一行或者最后一列，只能够往右走或者往下走。因此，如果当前 `dungeon[i][j] >=` 下一步需要的初始最低健康值，就只要 `1` 点即可；否则需要满足下一步所需的 `dp` 值；
- 其他位置，可以向右走，或者向下走。为满足题意，选择健康值要求更低的那一步。然后同样，看当前的 `dungeon[i][j]` 是否满足所需，能够满足就设 `dp[i][j] = 1` ；否则应该弥补起来。

代码：
```cpp
class Solution {
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) return 0;
        int m = dungeon.size(), n = dungeon[0].size();
        vector<vector<int>> dp(m, vector<int>(n, 0)); //dp[i][j]是从坐标(i,j)到P需要的最小初始健康值
        vector<vector<int>> d = dungeon;              //dp[i][j]一定>=1,一旦<=0就会死去
        for (int i = m - 1; i >= 0; --i) {
            for (int j = n - 1; j >= 0; --j) {
                if (i == m - 1 && j == n - 1) //P所在位置
                    dp[i][j] = d[i][j] < 0 ? 1 - d[i][j] : 1;  //最低为1
                else if (i == m - 1)          //P所在行
                    dp[i][j] = d[i][j] >= dp[i][j + 1] ? 1 : dp[i][j + 1] - d[i][j];
                else if (j == n - 1)          //P所在列
                    dp[i][j] = d[i][j] >= dp[i + 1][j] ? 1 : dp[i + 1][j] - d[i][j];
                else {                        //中间位置
                    dp[i][j] = min(dp[i + 1][j], dp[i][j + 1]);
                    dp[i][j] = d[i][j] >= dp[i][j] ? 1 : dp[i][j] - d[i][j];
                }
            }
        } 
        return dp[0][0];
    }
};
```
效率：
```css
执行用时：12 ms, 在所有 C++ 提交中击败了80.82% 的用户
内存消耗：8.8 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
上面的代码用到太多 `?:` 运算符，虽然更清晰，但是写起来麻烦。可以写成下面这样，效率更高，只是理解起来难一点：
```cpp
class Solution {
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        if (dungeon.empty() || dungeon[0].empty()) return 0;
        int m = dungeon.size(), n = dungeon[0].size();
        vector<vector<int>> dp(m, vector<int>(n, 0)); //dp[i][j]是从坐标(i,j)到P需要的最小初始健康值
        vector<vector<int>> d = dungeon;              //dp[i][j]一定>=1,一旦<=0就会死去
        for (int i = m - 1; i >= 0; --i) {
            for (int j = n - 1; j >= 0; --j) {
                if (i == m - 1 && j == n - 1) //P所在位置
                    dp[i][j] = max(1, 1 - d[i][j]);  //最低为1
                else if (i == m - 1)          //P所在行
                    dp[i][j] = max(1, dp[i][j + 1] - d[i][j]);
                else if (j == n - 1)          //P所在列
                    dp[i][j] = max(1, dp[i + 1][j] - d[i][j]);
                else                         //中间位置
                    dp[i][j] = max(1, min(dp[i + 1][j], dp[i][j + 1]) - d[i][j]);
            }
        } 
        return dp[0][0];
    }
};
```
效率：
```
执行用时：8 ms, 在所有 C++ 提交中击败了98.60% 的用户
内存消耗：8.8 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
