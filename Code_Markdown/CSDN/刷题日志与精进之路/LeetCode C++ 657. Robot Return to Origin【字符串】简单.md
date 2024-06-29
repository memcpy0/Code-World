There is a robot starting at position `(0, 0)` , the origin, on a `2D` plane. Given a sequence of its moves, judge if this robot ends up at `(0, 0)` after it completes its moves.

The move sequence is represented by a string, and the character `moves[i]` represents its ith move. Valid moves are `R (right)` , `L (left)` , `U (up)` , and `D (down)` . If the robot returns to the origin after it finishes all of its moves, return `true` . Otherwise, return `false` .

**Note:** The way that the robot is "facing" is irrelevant. `"R"` will always make the robot move to the right once, `"L"` will always make it move left, etc. Also, assume that the magnitude of the robot's movement is the same for each move.

**Example 1:**
```cpp
Input: "UD"
Output: true 
Explanation: The robot moves up once, and then down once. All moves have the same magnitude, so it ended up at the origin where it started. Therefore, we return true.
```
**Example 2:**
```cpp
Input: "LL"
Output: false
Explanation: The robot moves left twice. It ends up two "moves" to the left of the origin. We return false because it is not at the origin at the end of its moves.
```
题意：二维平面上的一个机器人从原点 `(0, 0)` 开始。给出它的移动顺序，判断这个机器人在完成移动后是否在 `(0, 0)` 处结束。注意：机器人“面朝”的方向无关紧要。  

---
思路：简单题目，用坐标进行模拟。代码如下：
```cpp
class Solution {
public:
    bool judgeCircle(string moves) {
        int x = 0, y = 0;
        for (char c : moves) {
            switch (c) {
                case 'R' : ++x; break;
                case 'L' : --x; break;
                case 'U' : ++y; break;
                case 'D' : --y; break;
            }
        }
        return !x && !y;
    }
};
```
---
思路2：计算 `U, D, L, R` 的出现次数，如果 `U = D, L = R` 就会回到原点。代码如下：
```cpp
class Solution {
public:
    bool judgeCircle(string moves) {
        int u, d, l, r;
        u = d = l = r = 0;
        for (char c : moves) {
            switch (c) {
                case 'R' : ++r; break;
                case 'L' : ++l; break;
                case 'U' : ++u; break;
                case 'D' : ++d; break;
            }
        }
        return u == d && l == r;
    }
};
```
