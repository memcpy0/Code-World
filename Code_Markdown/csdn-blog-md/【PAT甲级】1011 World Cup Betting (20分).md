With the 2010 FIFA World Cup running, football fans the world over were becoming increasingly excited as the best players from the best teams doing battles for the World Cup trophy in South Africa. Similarly, football betting fans were putting their money where their mouths were, by laying all manner of World Cup bets.

Chinese Football Lottery provided a "Triple Winning" game. The rule of winning was simple: first select any three of the games. Then for each selected game, bet on one of the three possible results -- namely `W` for win, `T` for tie, and `L` for lose. There was an odd assigned to each result. The winner's odd would be the product of the three odds times 65%.

For example, 3 games' odds are given as the following:

```swift
 W    T    L
1.1  2.5  1.7
1.2  3.1  1.6
4.1  1.2  1.1
```

To obtain the maximum profit, one must buy `W` for the 3rd game, `T` for the 2nd game, and `T` for the 1st game. If each bet takes 2 yuans, then the maximum profit would be $(4.1×3.1×2.5×65\%−1)×2=39.31$ yuans (accurate up to 2 decimal places).

### Input Specification:

Each input file contains one test case. Each case contains the betting information of 3 games. Each game occupies a line with three distinct odds corresponding to `W`, `T` and `L`.
### Output Specification:

For each test case, print in one line the best bet of each game, and the maximum profit accurate up to 2 decimal places. The characters and the number must be separated by one space.
### Sample Input:

```swift
1.1 2.5 1.7
1.2 3.1 1.6
4.1 1.2 1.1
```

### Sample Output:

```swift
T T W 39.31
```

---
### 解法 贪心
边读入边计算每个game中选择哪个选项获利最大，然后求出最大利润：
```cpp
#include <bits/stdc++.h>
using namespace std;
double t, ans = 2 * 0.65;
int main() {
    for (int i = 0; i < 3; ++i) {
        double mx = DBL_MIN, k = -1;
        for (int j = 0; j < 3; ++j) {
            scanf("%lf", &t);
            if (t > mx) { mx = t; k = j; }
        }
        ans *= mx;
        printf("%c ", !k ? 'W' : (k == 1 ? 'T' : 'L'));
    }
    printf("%.2lf", ans - 2);
    return 0;
}
```
