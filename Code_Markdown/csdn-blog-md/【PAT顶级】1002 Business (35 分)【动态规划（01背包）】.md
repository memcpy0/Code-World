As the manager of your company, you have to carefully consider, for each project, the time taken to finish it, the deadline, and the profit you can gain, in order to decide if your group should take this project. For example, given 3 projects as the following:

-  Project[1] takes 3 days, it must be finished in 3 days in order to gain 6 units of profit.
-  Project[2] takes 2 days, it must be finished in 2 days in order to gain 3 units of profit.
-  Project[3] takes 1 day only, it must be finished in 3 days in order to gain 4 units of profit.

You may take Project[1] to gain 6 units of profit. But if you take Project[2] first, then you will have 1 day left to complete Project[3] just in time, and hence gain 7 units of profit in total. Notice that once you decide to work on a project, you have to do it from beginning to the end without any interruption.
## Input Specification:  
Each input file contains one test case. For each case, the first line gives a positive integer $N$ ($≤50$), and then followed by $N$ lines of projects, each contains three numbers $P, L$, and $D$ where $P$ is the profit, $L$ the lasting days of the project, and $D$ the deadline. It is guaranteed that $L$ is never more than $D$ , and all the numbers are non-negative integers.
## Output Specification:

For each test case, output in a line the maximum profit you can gain.

## Sample Input:

```clike
4
7 1 3
10 2 3
6 1 2
5 1 1
```

## Sample Output:

```clike
18
```



---
现在先把代码放上来，很快有时间了就更新题意、解法。
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 55;
int n, maxDays = 0;
struct node { int p, l, d; } pld[maxn];
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%d%d%d", &pld[i].p, &pld[i].l, &pld[i].d);
        if (pld[i].d > maxDays) maxDays = pld[i].d;
    }
    vector<vector<int>> dp(n + 1, vector<int>(maxDays + 1, 0));
    sort(pld + 1, pld + n + 1, [&](const node& a, const node& b) {
        return a.d < b.d;
    }); 
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= maxDays; ++j) { //[0,l[i]),[l[i],d[i]],(d[i],maxDays)
            if (j < pld[i].l) dp[i][j] = dp[i - 1][j];
            else if (j <= pld[i].d) dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - pld[i].l] + pld[i].p);
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
//             cout << dp[i][j] << " ";
        }
//         cout << endl;
    }
    printf("%d\n", dp[n][maxDays]);
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210522190217753.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

