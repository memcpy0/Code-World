

Eva would like to buy a string of beads with no repeated colors so she went to a small shop of which the owner had a very long string of beads. However the owner would only like to cut one piece at a time for his customer. With as many as ten thousand beads in the string, Eva needs your help to tell her how to obtain the longest piece of string that contains beads with all different colors. And more, each kind of these beads has a different value. If there are more than one way to get the longest piece, Eva would like to take the most valuable one. It is guaranteed that such a solution is unique.
## Input Specification:

Each input file contains one test case. Each case first gives in a line a positive integer N ($≤ 10,000$) which is the length of the original string in the shop. Then N positive numbers ($≤ 100$) are given in the next line, which are the values of the beads -- here we assume that the types of the beads are numbered from 1 to N, and the i-th number is the value of the i-th type of beads. Finally the last line describes the original string by giving the types of the beads from left to right. All the numbers in a line are separated by spaces.
## Output Specification:

For each test case, print in a line the total value of the piece of string that Eva wants, together with the beginning and the ending indices of the piece (start from 0). All the numbers must be separated by a space and there must be no extra spaces at the beginning or the end of the line.
## Sample Input:

```clike
8
18 20 2 97 23 12 8 5
3 3 5 8 1 5 2 1
```

## Sample Output:

```go
66 3 6
```

---
很快有时间了就更新题意和解题思路：
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1e4 + 10;
int arr[maxn], val[maxn], presum[maxn], vis[maxn], n, k, t;
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%d", &val[i]);
    for (int i = 0; i < n; ++i) { scanf("%d", &arr[i]); presum[i + 1] = presum[i] + val[arr[i]]; }
    memset(vis, 0xff, sizeof(vis));
    int lo = 0, hi, sum = 0, maxi = 0, maxlen = 0;
    for (hi = 0; hi < n; ++hi) {
        if (vis[arr[hi]] >= lo) {
            if (hi - lo > maxlen) maxlen = hi - lo, maxi = lo, sum = presum[hi] - presum[lo];
            else if (hi - lo == maxlen && presum[hi] - presum[lo] > sum) maxi = lo, sum = presum[hi] - presum[lo];
            lo = vis[arr[hi]] + 1;
        }
        vis[arr[hi]] = hi; //更新原值 
    }
    if (hi - lo > maxlen) maxlen = hi - lo, maxi = lo, sum = presum[hi] - presum[lo];
    else if (hi - lo == maxlen && presum[hi] - presum[lo] > sum) maxi = lo, sum = presum[hi] - presum[lo];
    printf("%d %d %d", sum, maxi, maxi + maxlen - 1);
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210522191748171.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

