> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

> 相关公司：Amazon、LinkedIn、Goldman Sachs
 
On an infinite plane, a robot initially stands at `(0, 0)` and faces north. Note that:
-   The **north direction** is the positive direction of the y-axis.
-   The **south direction** is the negative direction of the y-axis.
-   The **east direction** is the positive direction of the x-axis.
-   The **west direction** is the negative direction of the x-axis.

The robot can receive one of three instructions:
-   `"G"`: go straight 1 unit.
-   `"L"`: turn 90 degrees to the left (i.e., anti-clockwise direction).
-   `"R"`: turn 90 degrees to the right (i.e., clockwise direction).

The robot performs the `instructions` given in order, and repeats them forever.

Return `true` if and only if there exists a circle in the plane such that the robot never leaves the circle.

**Example 1:**
```java
Input: instructions = "GGLLGG"
Output: true
Explanation: The robot is initially at (0, 0) facing the north direction.
"G": move one step. Position: (0, 1). Direction: North.
"G": move one step. Position: (0, 2). Direction: North.
"L": turn 90 degrees anti-clockwise. Position: (0, 2). Direction: West.
"L": turn 90 degrees anti-clockwise. Position: (0, 2). Direction: South.
"G": move one step. Position: (0, 1). Direction: South.
"G": move one step. Position: (0, 0). Direction: South.
Repeating the instructions, the robot goes into the cycle: (0, 0) --> (0, 1) --> (0, 2) --> (0, 1) --> (0, 0).
Based on that, we return true.
```
**Example 2:**
```java
Input: instructions = "GG"
Output: false
Explanation: The robot is initially at (0, 0) facing the north direction.
"G": move one step. Position: (0, 1). Direction: North.
"G": move one step. Position: (0, 2). Direction: North.
Repeating the instructions, keeps advancing in the north direction and does not go into cycles.
Based on that, we return false.
```
**Example 3:**
```java
Input: instructions = "GL"
Output: true
Explanation: The robot is initially at (0, 0) facing the north direction.
"G": move one step. Position: (0, 1). Direction: North.
"L": turn 90 degrees anti-clockwise. Position: (0, 1). Direction: West.
"G": move one step. Position: (-1, 1). Direction: West.
"L": turn 90 degrees anti-clockwise. Position: (-1, 1). Direction: South.
"G": move one step. Position: (-1, 0). Direction: South.
"L": turn 90 degrees anti-clockwise. Position: (-1, 0). Direction: East.
"G": move one step. Position: (0, 0). Direction: East.
"L": turn 90 degrees anti-clockwise. Position: (0, 0). Direction: North.
Repeating the instructions, the robot goes into the cycle: (0, 0) --> (0, 1) --> (-1, 1) --> (-1, 0) --> (0, 0).
Based on that, we return true.
```
**Constraints:**
-   `1 <= instructions.length <= 100`
-   `instructions[i]` is `'G'`, `'L'` or, `'R'`.

题意：机器人在一个无限的二维平面上，给它一串移动指令，机器人反复执行这条指令，如果它在平面上重复走一个死循环，就返回 `true` 。

---
### 解法 模拟
当机器人执行完指令 `instructions` 后，它的位置和方向均有可能发生变化。我们先考虑位置，然后考虑其方向。
- **如果它的位置仍位于原点**，那么不管它此时方向是什么，都将永远无法离开。
- **如果它的位置不在原点**，我们分情况讨论此时机器人的方向：
	- 如果仍然朝北，那么机器人不会陷入循环。假设执行完一串指令后，机器人的位置是 $(x,y)$ 且不为原点，方向仍然朝北，那么执行完第二串指令后，机器人的位置便成为 $(2\times x, 2\times y)$ ，会不停地往外部移动，不会陷入循环。
	- 如果朝南，那么执行第二串指令时，机器人的位移与第一次相反，即第二次的位移是 $(−x,−y)$ ，并且结束后会回到原来的方向。这样一来，**每两串指令之后，机器人都会回到原点，并且方向朝北**，机器人会陷入循环。
	- 如果朝东，即右转了 $90\degree$ 。这样一来，每执行一串指令，机器人都会右转 $90°$ 。那么第一次和第三次指令的方向是相反的，第二次和第四次指令的方向是相反的，位移之和也为 $0$ ，这样一来，**每四串指令之后，机器人都会回到原点，并且方向朝北**，机器人会陷入循环。如果机器人朝西，也是一样的结果。

因此，机器人想要摆脱循环，**在一串指令之后的状态，必须是不位于原点且方向朝北**。
```cpp
class Solution {
public:
    bool isRobotBounded(string instructions) {
        int move[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        int x = 0, y = 0, dir = 0; // 0北,1东,2南,3西
        for (char &c : instructions) {
            if (c == 'G') x += move[dir][0], y += move[dir][1];
            else if (c == 'L') dir = (dir + 3) % 4;
            else dir = (dir + 1) % 4;
        }
        return (x == 0 && y == 0) || dir != 0;
    }
};
```
复杂度分析
- 时间复杂度：$O(n)$ ，其中 $n$ 是 `instructions` 的长度，需要遍历 `instructions` 一次。
- 空间复杂度：$O(1)$ ，只用到常数空间。