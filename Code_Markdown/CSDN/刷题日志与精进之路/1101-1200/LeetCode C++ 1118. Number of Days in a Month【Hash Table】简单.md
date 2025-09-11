Given a year `Y` and a month `M`, return how many days there are in that month.

**Example 1:**

```haskell
Input: Y = 1992, M = 7
Output: 31
```

**Example 2:**

```haskell
Input: Y = 2000, M = 2
Output: 29
```

**Example 3:**

```haskell
Input: Y = 1900, M = 2
Output: 28
```
**Note:**
- `1583 <= Y <= 2100`
 - `1 <= M <= 12`

题意：指定年份 `Y` 和月份 `M` ，计算出该月一共有多少天。

---
### 解法 查表
```cpp
class Solution {
public:
    int numberOfDays(int Y, int M) {
        int months[2][12] = {
            {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
            {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
        };
        function<int(int)> isLeap = [](int y) { return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0); };
        return months[isLeap(Y)][M - 1];
    }
};
```
提交后运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了24.32% 的用户
```
