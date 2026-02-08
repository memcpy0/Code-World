Given a sequence of K integers $\{ N_1, N_2, ..., N_K\}$ . A continuous subsequence is defined to be $\{ N_i, N_{i+1}, ..., N_j \}$ where $1≤i≤j≤K$. The Maximum Subsequence is the continuous subsequence which has the largest sum of its elements. For example, given sequence $\{ -2, 11, -4, 13, -5, -2 \}$, its maximum subsequence is $\{ 11, -4, 13 \}$ with the largest sum being $20$.

Now you are supposed to find the largest sum, together with the first and the last numbers of the maximum subsequence.
### Input Specification:

Each input file contains one test case. Each case occupies two lines. The first line contains a positive integer $K (≤10000)$. The second line contains $K$ numbers, separated by a space.
### Output Specification:

For each test case, output in one line the largest sum, together with the first and the last numbers of the maximum subsequence. The numbers must be separated by one space, but there must be no extra space at the end of a line. In case that the maximum subsequence is not unique, output the one with the smallest indices $i$ and $j$ (as shown by the sample case). If all the $K$ numbers are negative, then its maximum sum is defined to be 0, and you are supposed to output the first and the last numbers of the whole sequence.
### Sample Input:

```groovy
10
-10 1 2 3 4 -5 -23 3 7 -21
```

### Sample Output:

```groovy
10 1 4
```

```cpp
#include <cstdio>
int a[10010];
int main() {
    int K;
    scanf("%d", &K);
    int ThisSum = 0, ThisLeft = 0;  // 指示当前子列和的左位序, 如果当前子列和不小于0不会变化  
    int MaxSum = -1, MaxLeft = 0, MaxRight = K-1; // 指示最大子列和的左右位序，需要更新的时候就分别更新
    for (int i = 0; i < K; i++) {
        scanf("%d", &a[i]);    
        ThisSum += a[i];
        if (ThisSum > MaxSum) {
            MaxSum = ThisSum;
            MaxLeft = ThisLeft; 
            MaxRight = i;     
        } else if (ThisSum < 0) { // 这里一定要用else if??
            ThisSum = 0;
            ThisLeft = i + 1;
        }
    }
    if (MaxSum < 0) MaxSum = 0;
    printf("%d %d %d", MaxSum, a[MaxLeft], a[MaxRight]);        
    return 0;
}
```
下面也是正确的程序？（可以通过的程序）：
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 10100;
int arr[maxn], dp[maxn] = {0}, k;
int main() {
    scanf("%d", &k);
    bool allNeg = true;
    int maxNum = 0;
    for (int i = 0; i < k; ++i) {
        scanf("%d", &arr[i]);
        if (arr[i] >= 0) allNeg = false;
        maxNum = max(arr[i], maxNum);
    }
    if (allNeg) { printf("0 %d %d", arr[0], arr[k - 1]); return 0; }
    if (maxNum == 0) { printf("0 0 0"); return 0; } //最大的值为0,于是全为0
    int maxsum = 0, e = 0;
    for (int i = 0; i < k; ++i) {
        dp[i + 1] = max(dp[i + 1], dp[i] + arr[i]);
        if (dp[i + 1] > maxsum) {
            maxsum = dp[i + 1];
            e = i;
        }
    }
    int tempsum = maxsum, s = e;
    while (tempsum && s >= 0) {
        tempsum -= arr[s--];
        if (tempsum == 0)
            while (s >= 0 && arr[s] == 0) --s;
    }
    printf("%d %d %d", maxsum, arr[s + 1], arr[e]);
    return 0;
}
```
发现用暴力也能过！！！
```cpp
#include <cstdio>
int a[10010];
int main() { 
    int K;
     scanf("%d", &K);
    int maxsum = -1, maxleft = 0, maxright = K - 1; //最大子序列和为0时会更新左右值的序号
    for (int i = 0; i < K; ++i)
        scanf("%d", &a[i]);
    for (int i = 0; i < K; ++i) {
        int sum = 0;
        for (int j = i; j < K; ++j) {
            sum += a[j];
            if (sum > maxsum) {
                maxsum = sum;
                maxleft = i;
                maxright = j;
            }
        }
    }
    if (maxsum < 0) maxsum = 0;
    printf("%d %d %d\n", maxsum, a[maxleft], a[maxright]);       
    return 0;
}
```

