

A robot on an infinite grid starts at point `(0, 0)` and faces north.  The robot can receive one of three possible types of commands:
- `-2`: turn left 90 degrees
 - `-1`: turn right 90 degrees
 - `1 <= x <= 9`: move forward `x` units

Some of the grid squares are obstacles. 

The `i`-th obstacle is at grid point `(obstacles[i][0], obstacles[i][1])`. 

If the robot would try to move onto them, the robot stays on **the previous grid square** instead (but still continues following the rest of the route.)

Return the **square** of the maximum Euclidean distance that the robot will be from the origin.

 

**Example 1:**

```swift
Input: commands = [4,-1,3], obstacles = []
Output: 25
Explanation: robot will go to (3, 4)
```

**Example 2:**

```swift
Input: commands = [4,-1,4,-2,4], obstacles = [[2,4]]
Output: 65
Explanation: robot will be stuck at (1, 4) before turning left and going to (1, 8)
```

 

**Note:**
- `0 <= commands.length <= 10000`
- `0 <= obstacles.length <= 10000`
- `-30000 <= obstacle[i][0] <= 30000`
-  `-30000 <= obstacle[i][1] <= 30000`
 - The answer is **guaranteed** to be less than `2^31` .


题意：机器人在一个无限大小的网格上行走，从点 `(0, 0)` 处开始出发，**面向北方**。该机器人可以接收以下三种类型的命令：
- `-2`：向左转 `90` 度
-  `-1`：向右转 `90` 度
- `1 <= x <= 9`：向前移动 `x` 个单位长度

在网格上有一些格子被视为障碍物。第 `i` 个障碍物位于网格点  `(obstacles[i][0], obstacles[i][1])` 。机器人无法走到障碍物上，它将会停留在障碍物的前一个网格方块上，但仍然可以继续该路线的其余部分。返回从原点到机器人所有经过的路径点（坐标为整数）的**最大欧式距离的平方**。


---
### 解法 遍历移动序列
这道题是一道蛮不错的模拟题。在代码中，为了方便检查障碍物的位置，将其坐标存储于集合中。此外需要注意的还有两点：使用路径点坐标 `(x, y)` 、移动数组 `moves` 和方向标识 `direction`）进行方向的转变和具体的移动；返回的结果是机器人经过的所有路径点中、离原点的最大欧式距离的平方。
```cpp
class Solution {
public:
    int robotSim(vector<int>& commands, vector<vector<int>>& obstacles) {
        int x = 0, y = 0, direction = 0; //向北走
        int maxEuclideanDist = 0;
        int moves[][2] = {0, 1, 1, 0, 0, -1, -1, 0}; //向北走, 向东走, 向南走, 向西走 
        set<pair<int, int>> st;
        for (const vector<int> &v : obstacles) st.insert({v[0], v[1]});
        for (const int &v : commands) {
            if (v == -2) { 
                --direction;
                if (direction < 0) direction += 4; //向左转90度
            } else if (v == -1) {
                ++direction;
                if (direction >= 4) direction -= 4; //向右转90度
            } else {
                for (int i = 1; i <= v; ++i) {
                    int tx = x + moves[direction][0];
                    int ty = y + moves[direction][1];
                    if (st.find({tx, ty}) != st.end()) break; //遇到障碍,停下
                    x = tx, y = ty;
                    maxEuclideanDist = max(maxEuclideanDist, x * x + y * y);
                } 
            }
        }
        return maxEuclideanDist;
    }
};
```
提交后运行效率如下：
```cpp
执行用时：220 ms, 在所有 C++ 提交中击败了74.57% 的用户
内存消耗：27.6 MB, 在所有 C++ 提交中击败了55.44% 的用户
```
