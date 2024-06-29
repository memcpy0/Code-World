You are given an integer `n`, the number of teams in a tournament that has strange rules:
-  If the current number of teams is **even**, each team gets paired with another team. A total of `n / 2` matches are played, and `n / 2` teams advance to the next round.
-  If the current number of teams is **odd**, one team randomly advances in the tournament, and the rest gets paired. A total of `(n - 1) / 2` matches are played, and `(n - 1) / 2 + 1` teams advance to the next round.

*Return the number of matches played in the tournament until a winner is decided.*

  
**Example 1:**

```cpp
Input: n = 7
Output: 6
Explanation: Details of the tournament: 
- 1st Round: Teams = 7, Matches = 3, and 4 teams advance.
- 2nd Round: Teams = 4, Matches = 2, and 2 teams advance.
- 3rd Round: Teams = 2, Matches = 1, and 1 team is declared the winner.
Total number of matches = 3 + 2 + 1 = 6.
```

**Example 2:**

```cpp
Input: n = 14
Output: 13
Explanation: Details of the tournament:
- 1st Round: Teams = 14, Matches = 7, and 7 teams advance.
- 2nd Round: Teams = 7, Matches = 3, and 4 teams advance.
- 3rd Round: Teams = 4, Matches = 2, and 2 teams advance.
- 4th Round: Teams = 2, Matches = 1, and 1 team is declared the winner.
Total number of matches = 7 + 3 + 2 + 1 = 13.
```

**Constraints:** `1 <= n <= 200`

题意：一个整数 `n` 表示比赛中的队伍数。返回在比赛中进行的配对次数，直到决出获胜队伍为止。

---
### 解法1 遵循题意
```cpp
class Solution {
public:
    int numberOfMatches(int n) {
        int ans = 0;
        while (n > 1) { //n<=1时无需比赛
            if (n & 1) { ans += (n - 1) / 2; n = (n - 1) / 2 + 1; } //奇数,加上(n-1)/2次配对次数,剩下(n-1)/2+1队伍
            else { ans += n / 2; n /= 2; } //偶数,加上n/2次配对次数,剩下n/2支队伍
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
---
### 解法2 数学规律
每一轮中会进行多次配对，每次配对进行比赛，都会淘汰一支队伍。直到决出获胜队伍。因此最终只进行 `n - 1` 次配对：
```cpp
class Solution {
public:
    int numberOfMatches(int n) {
        return n - 1;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
