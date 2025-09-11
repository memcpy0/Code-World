Alice and Bob take turns playing a **game**, with Alice starting first.

Initially, there is a number `N` on the chalkboard.  On each player's turn, that player makes a move consisting of:
-    Choosing any `x` with `0 < x < N and N % x == 0` .
-    Replacing the number `N` on the chalkboard with `N - x` .

Also, if a player cannot make a move, they lose the game.

Return `True` if and only if Alice wins the game, assuming both players play optimally.

 
Example 1:
```bash
Input: 2
Output: true
Explanation: Alice chooses 1, and Bob has no more moves.
```

Example 2:
```bash
Input: 3
Output: false
Explanation: Alice chooses 1, Bob chooses 1, and Alice has no more moves.
```
**Note**:  `1 <= N <= 1000` 

题意：这个题目比较简单。就是说，绝顶聪明的Alex和Bob一起玩♂游♂戏，轮流操作。Alice先手！每回合他们要进行如下的操作：
- 选择一个 `x` ，`0 < x < N && N % x == 0` ；
- 然后 `N -= x` ；

相当于有 `N` 颗石子，每回合玩家可以拿起 `x` 颗石头，`x` 是 `N` 的一个**真因子**，如果某回合玩家无法操作就输掉游戏。 

给出一个 `N` ，我们判断Alice是否能在游戏中取得胜利。

---
思路1：动态规划。我们这样思考：
- 如果没有石头，`N = 0` 时，此时Alice先手，直接胜利；
- 如果只有一颗石头，`N = 1` ，Alice无法找到 `1` 的真因子，她必输无疑；
- 如果有 `N = 2` 颗石头，Alice找到 `2` 的真因子 `1` ，她拿走了一颗石头，于是 `N = 1` 的局面留给了Bob。我们从前面可以知道，`N = 1` 时进行此局的玩家必输，于是Alice获胜；
- 如果有 `N` 颗石头，假设我们能够找到一个真因子 `x` ，使得面对 `N - x` 局面的Bob必然输掉游戏，Alice就能获胜。

为此，我们用 `dp[i]` 表示Alice在 `N = i` 时是否能够取得游戏的胜利，`dp[i] = true` 表示Alice必定胜利：
- 假如我们可以找到 `i` 的一个真因子 `j` ，令 `dp[i - j] = false` ，则 `dp[i] = true` ，因为我们能够将一个必输无疑的状态 `i - j` 留给了Bob；
- 假如不存在真因子，或者全部的真因子都使得 `dp[i - j] = true` ，即Alice无论如何都会将一个胜利的状态 `i - j` 留给Bob，于是Alice必输。
- 最后返回 `dp[N]` 即可。

代码如下：
```cpp
class Solution {
public:
    bool divisorGame(int N) {
        vector<bool> dp(N + 1);
        dp[0] = true; dp[1] = false;
        for (int i = 2; i <= N; ++i) 
            for (int j = 1; j < i; ++j) 
                if (i % j == 0 && !dp[i - j]) { dp[i] = true; break; } 
        return dp[N];
    }
};
```

---
思路2：我们可以找到一个绝对简单的算法，解决这道题目。关键点还是要落在 `x` 作为真因子的要求上。我们知道，奇数 `*` 奇数必然是奇数，奇数 `*` 偶数和偶数 `*` 偶数仍是偶数。于是：
- 如果 `N` 是 `>= 2` 的偶数，那么它的真因子必然有奇数（如 `1`），也有偶数。
- 如果 `N` 是 `>= 2` 的奇数，那么它的真因子必然只有奇数。

因此，有如下情况：
- 奇数 `N` ，我们求出它的真因子 `x` ，取走 `x` 后，结果必然变成偶数；
- 偶数 `N` ，我们可以取走奇数也可以取走偶数真因子，结果可能是奇数，也可能是偶数。

这和我们的题目有什么关系呢？仔细思考一下，就会发现：
- 如果 `N` 是奇数，那么进行一次 `N - x` 的操作结果一定是偶数。则Alice先手，遇到的是一个奇数，那么轮到Bob的时候**他得到的一定是偶数**。这个时候Bob会拿走奇数还是偶数？Bob很聪明，**他一定会选择一个必胜的方法**：
	- Bob只要拿走一个奇数，比如 `1` ，还给Alice的一定是奇数。**那么Bob就会一直得到偶数，Alice一直得到奇数**——最后Bob一定会拿到最小的偶数 `2` ，Alice一定会拿到最小的奇数 `1` ——然后Alice就输了。
	- 如果Bob拿走偶数，那么给Alice的就是偶数。于是状态颠倒过来了，Alice**也会选择一个必胜的方法**：她就会拿走一个奇数，给Bob一个奇数。然后Bob只能拿走一个奇数，给Alice一个偶数——于是Alice就能一直拿到偶数直到 `2` ，而Bob则拿到奇数 `1` ——这样Bob就会输掉游戏。
	- **因此，Bob面对偶数时一定会选择拿走奇数**。
- 如果 `N` 是偶数，这是上面讨论过的状态。

总之，任何一方，如果面对的是偶数，他就一定能够赢得游戏——Alice先手，如果 `N` 是偶数，则Alice必胜；反之，`N` 是奇数，那么Bob就能够得到偶数，于是Alice必输。

代码：
```cpp
class Solution {
public:
    bool divisorGame(int N) {
        return !(N & 1);
    }
};
```
