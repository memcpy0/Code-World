Alice plays the following game, loosely based on the card game "21".

Alice starts with `0 ` points, and draws numbers while she has **less than `K` points**.  During each draw, she gains **an integer number** of points randomly from the range `[1, W]`, where `W` is **an integer**.  Each draw is **independent** and the outcomes have equal probabilities.

Alice stops drawing numbers when she **gets `K` or more points**.  What is the `probability` that she **has `N ` or less points**?

Example 1:
```clike
Input: N = 10, K = 1, W = 10
Output: 1.00000
Explanation:  Alice gets a single card, then stops.
```

Example 2:
```clike
Input: N = 6, K = 1, W = 10
Output: 0.60000
Explanation:  Alice gets a single card, then stops.
In 6 out of W = 10 possibilities, she is at or below N = 6 points.
```

Example 3:
```clike
Input: N = 21, K = 17, W = 10
Output: 0.73278
```

Note:

-   `0 <= K <= N <= 10000`
-  `1 <= W <= 10000`
-    Answers will **be accepted as correct** if they are within `10^-5` of the correct answer.
-    The judging time limit **has been reduced** for this question.


方案的概率不是均等的！
```cpp
class Solution {
public:
    // int n, k, w;
    // double s, t;
    // double new21Game(int N, int K, int W) {
    //     n = N, k = K, w = W;
    //     s = 0, t = 0;
    //     dfs(0);
    //     return s / t;
    // }
    // void dfs(int curPoints) {
    //     if (curPoints >= k) {
    //         if (curPoints <= n) ++s, ++t;
    //         else ++t;
    //         return;
    //     }
    //     for (int i = 1; i <= w; ++i) {
    //         dfs(curPoints + i);
    //     }
    // }
};
```
方法：

```clike
class Solution {
public: 
    double new21Game(int N, int K, int W) {
        //K为0时所有可能均<=N; (K - 1) + W <= N时, 所有可能也<=N
        //if (K == 0 || W <= N - K + 1) return 1.0; 

        //从K - 1的角度来说, 只能抛一次, 其方案的点数之和<=N可以如下描述：
        // s = {x | 1 <= x <= W && K - 1 + x <= N}
        // P(K - 1)概率为 P(K - 1) = len(s) / len(W)
        //对于<K-1的情况, 可以抛多次, P(k-i) = sum(len(s) / len(W))
        vector<double> dp(K + W, 0.0);
        double sum = 0;
        for (int i = K; i < K + W; ++i) { //K-1抽一张牌, 可以是1-W
            if (i > N) dp[i] = 0; //因此K <= i <= K+W-1
            else dp[i] = 1;       //i <= N即K-1 + x <= N的可能性数目
            sum += dp[i];
        }
        // 由于重复子问题是对一段区间里的值求和，而区间长度并不是固定的
        // 可以将区间看成是一个滑动窗口，从右边滑到左边
        for (int i = K - 1; i >= 0; --i) {
            dp[i] = sum / W;
            sum -= dp[i + W]; //滑动窗口
            sum += dp[i]; //每次都得减去窗口右侧的值，加上窗口左侧值
        }
        return dp[0];
    }
};
```
先上结果，0ms，4MB，最终答案直接跳到最后，想看优化思路按顺序看。

第一版

```csharp
// 思路一：当已有分值为K-1时，你只能抛一次
// 假设你抛出了i，此时你抛出符合条件的数字集合为s = {i | (K-1)+i <= N}
// 概率是使得f(K-1) = sum(len(s)/len(W))，f函数的自变量是当前分数
// 当已有分值为K-2时，假设你抛到1使得分数为K-1，那么此时你相当于走到了f(K-1)，抛到其他数字同理
func new21Game(N int, K int, W int) float64 {
    cache := make([]float64,N+1)
    for i:=K;i<=N;i++ {
        cache[i] = 1.0
    }
    for i:=K-1; i>=0; i-- {
        for j:=1 ;j<=W && i+j<=N; j++ {
            // 明显第二层for循环对cache[i+j]求和的操作是重复子问题
            // 所以可以通过一个变量记录起来，降为一层循环
            // 并且float64(1)/float64(W)其实是一个固定值，可以提取出来
            cache[i] += float64(1)/float64(W)*cache[i+j]
        }
        fmt.Println(cache[i])
    }
    return cache[0]
}
```

上面的思路其实已经可以解决问题，但效率不行，双层的for循环使得复杂度很高。

避免重复求子问题是一个优化突破口，于是有了第二版

```csharp
func new21Game(N int, K int, W int) float64 {
    cache := make([]float64,N+1)
    var ans float64 = 1.0 * float64(N-K+1)
    for i:=K;i<=N;i++ {
        cache[i] = 1.0
    }
    // 由于重复子问题是对一段区间里的值求和，而区间长度并不是固定的
    // 可以将区间看成是一个滑动窗口，从右边滑到左边
    // 由于i+W<=N的限制，因此滑动窗口的长度可能是逐渐变大，然后固定
    var right int = N
    for i:=K-1; i>=0; i-- {
        cache[i] = float64(1)/float64(W)*ans
        ans += cache[i]
        if i+W <= N {
            // 此时区间长度固定，每次都得减去窗口右侧的值，加上窗口左侧值
            ans -= cache[right]
            right--
        }
    }
    return cache[0]
}
```


