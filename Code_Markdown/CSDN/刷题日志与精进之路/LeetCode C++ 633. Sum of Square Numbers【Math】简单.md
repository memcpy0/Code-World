Given a **non-negative** integer c, your task is to decide whether there're **two integers** `a` and `b` such that $a^2 + b^2 = c$.

Example 1:
```cpp
Input: 5
Output: True
Explanation: 1 * 1 + 2 * 2 = 5
```
Example 2:
```cpp
Input: 3
Output: False
```
题意：给出一个非负整数 `c` ，判断是否存在整数 `a,b` 满足 `a*a+b*b=c` 。

这道题有多种做法，这里先写一种思路的代码。

思路1：暴力可能过不了；极端点 $a=0$，则可能存在 $b = \sqrt c$ 。因此，可以用双指针，`left = 0, right = sqrt(c)` ，然后向中间搜索。

思路2：可以进一步收缩数据范围，`a,b` 中必然有一个数 $\le \sqrt{c/2}$ ，另一个数 $\ge \sqrt{c/2}$ 。这样假设 `a` 为前者，`a` 从 `0` 开始搜索，每一次算出 `b` 来，如果 `b` 是整数，则返回 `true` 。最终没有结果就返回 `false` 。

代码：
```cpp
class Solution {
public:
    bool judgeSquareSum(int c) { 
        int sqr = sqrt(c / 2.0);
        for (int a = 0; a <= sqr; ++a) {
            double b = sqrt(c - a * a);
            if (b - trunc(b) < 0.00001)  
                return true; 
        } 
        return false;
    }
};
```
效率：
```
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.1 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
