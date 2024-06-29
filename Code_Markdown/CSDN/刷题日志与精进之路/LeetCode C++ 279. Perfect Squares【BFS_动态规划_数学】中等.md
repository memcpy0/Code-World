Given a positive integer `n` , find the least number of perfect square numbers (for example, `1, 4, 9, 16, ...` ) which sum to `n` .

**Example 1:**
```cpp
Input: n = 12
Output: 3 
Explanation: 12 = 4 + 4 + 4.
```
**Example 2:**
```cpp
Input: n = 13
Output: 2
Explanation: 13 = 4 + 9.
```

题意：给出正整数 `n` ，找到若干个完全平方数，比如 `1, 4, 9, 16, ...` 等，使它们的和等于 `n` 。需要让组成和的完全平方数的个数最少。

---
可能想到的是贪心，但是举一个反例就够了：
```py
n = 12
12 = 9 + 1 + 1 + 1 (贪心)
12 = 4 + 4 + 4	   (答案)
```
### 解法1 BFS
正确的做法是将其转换为一个图论问题：
- 从 `n` 到 `0` ，每个数字代表一个结点；
- 如果两个数字 `x, y` 之间相差一个完全平方数，就在图中连接一条边；
- 最后得到一个无权连通图；

这样，问题就转换为：**求这个无权图中从 `n` 到 `0` 的最短路径**。比如下图中，`4` 和 `0` 之间有一条边，是因为差了一个完全平方数 `4` ；从 `2` 到 `1` 有条边，是因为差了一个完全平方数 `1` ；依次类推。有了这张有向图，我们就可以很容易求出 `4` 到 `0` 的最短路径：

<img src="https://img-blog.csdnimg.cn/20200825203930145.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" width="56%">

如果 `n = 5` ，就添加一个结点和一些边：
<img src="https://img-blog.csdnimg.cn/20200825204252947.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" width="56%">
类似的，如果 `n = 6` 的话，`n` 可以到达 `2, 5` ，如下图：
<img src="https://img-blog.csdnimg.cn/20200825204620926.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" width="56%">
如果有 `7, 8` 的话，如下图：
<img src="https://img-blog.csdnimg.cn/2020082520471370.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" width="56%">
接着，有 `9` 的话，它可以和 `1, 5, 8` 相连：
<img src="https://img-blog.csdnimg.cn/20200825204821667.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" width="56%">

有了这个思路，代码就很容易了，这里也可以**提前求出数据范围内的完全平方数**：
```cpp
class Solution {
public:
    int numSquares(int n) {
        //assert(n > 0);	
        int step = -1;
        queue<int> q;
        q.push(n);
        while (!q.empty()) {
            int size = q.size();
            ++step;
            for (int i = 0; i < size; ++i) {
                int num = q.front(); q.pop();
                if (num == 0) return step;
                //求出所有小于等于num的完全平方数j*j,然后将num的邻接点num-j*j入队列
                for (int j = 1; num - j * j >= 0; ++j) 
                    q.push(num - j * j);
            }
        }
        //throw invalid_argument("No Solution."); //走到这一步,函数参数有问题
        return step;
    }
};
```
似乎没问题了，一提交，TLE。原因是什么呢？上面的代码不是一个**标准的BFS实现**。以上面的几幅图为例，`4` 可以从 `5` 也可以从 `8` 到达，甚至从 `20` 到达……因此，**我们入队了太多重复的结点**。为了避免这个问题，就需要一个 `visited[]` 数组，改造代码再加上一些**小的优化**：
```cpp
class Solution {
public:
    int numSquares(int n) {
        queue<int> q;
        q.push(n);
        int steps = -1;
        vector<bool> vis(n + 1);
        while (!q.empty()) {
            int size = q.size();
            ++steps;
            for (int i = 0; i < size; ++i) {
                int u = q.front(); q.pop();
                if (vis[u]) continue; //有些数字可能重复进入队列
                if (u == 0) return steps; //n不断减去完全平方数,得到零
                vis[u] = true; 
                for (int j = 1; j * j <= u; ++j) { //注意是<=
                    int v = u - j * j;
                    if (!vis[v]) q.push(v);
                }
            } 
        }
        return steps;
    }
};
```
上述写法虽然可以过，但是这种写法仍然可能**导致某些节点重复入队**：
```cpp
执行用时：564 ms, 在所有 C++ 提交中击败了7.32% 的用户
内存消耗：47.7 MB, 在所有 C++ 提交中击败了7.19% 的用
```
下面的写法提前探测下一层，**彻底避免了节点重复入队**，而且可能提前一层返回，效率大大提高：
```cpp
class Solution {
public:
    int numSquares(int n) {
        queue<int> q;
        q.push(n);
        int steps = -1;
        vector<bool> vis(n + 1);
        vis[n] = true;
        while (!q.empty()) {
            int size = q.size();
            ++steps;
            for (int i = 0; i < size; ++i) {
                int u = q.front(); q.pop();   
                for (int j = 1; j * j <= u; ++j) { //注意是<=
                    int v = u - j * j; 
                    if (v == 0) return steps + 1; //n不断减去完全平方数,得到零,提前返回
                    if (!vis[v]) { //探测下一层,避免重复入队
                        q.push(v); 
                        vis[v] = true; 
                    }
                }
            } 
        }
        return steps;
    }
};
```
两次执行，效率分别如下：
```cpp
执行用时：56 ms, 在所有 C++ 提交中击败了91.37% 的用户
内存消耗：8.3 MB, 在所有 C++ 提交中击败了82.37% 的用户

执行用时：64 ms, 在所有 C++ 提交中击败了89.79% 的用户
内存消耗：8.4 MB, 在所有 C++ 提交中击败了82.70% 的用户
```

这样就可以说，我们比较好地解决了整个问题。

---
### 解法2 完全背包+恰好问题
可以将这一问题转化为完全背包求解。先求出数据范围内的所有完全平方数**作为物品**（物品数量无限），**背包容量**为数的大小（最大为 `n` ），**代价**为数的大小，**价值**为数的个数。整个算法的时间复杂度为 $O(n\sqrt{n})$ ，因为小于等于 $n$ 的完全平方数个数为 $\sqrt{n}$ 。不难发现，本题的完全背包解法和[LeetCode C++ 322. Coin Change](https://memcpy0.blog.csdn.net/article/details/109458595)完全一样。

```swift
class Solution {
public:
    int numSquares(int n) { 
        int lst[101], dp[n + 1];
        for (int i = 0; i < 101; ++i) lst[i] = i * i; //物品的重量 
        fill(dp, dp + n + 1, 0x3fffffff);
        dp[0] = 0, dp[1] = 1;
        for (int i = 1; i <= 100; ++i)
            for (int j = lst[i]; j <= n; ++j)
                dp[j] = min(dp[j], dp[j - lst[i]] + 1);
        return dp[n];
    }
};
```
执行效率如下：
```cpp
执行用时：184 ms, 在所有 C++ 提交中击败了73.05% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了87.70% 的用户
```
**20210612 打卡更新**：今天用C写的解法如下：
```cpp
#define min(x, y) (((x) < (y)) ? (x) : (y))
int numSquares(int n){
    int nums[110]; //<=10000的完全平方数
    for (int i = 1; i <= 100; ++i) nums[i - 1] = i * i;
    //从这些数中选出 和恰好为n的 最少的完全平方数的数量
    int *dp = (int *)malloc(sizeof(int) * (n + 1));
    memset(dp, 0x3f, sizeof(int) * (n + 1));
    dp[0] = 0; //完全背包,容量为数的大小(最大为n),代价为数的大小,价值为数的个数
    for (int i = 0; i < 100; ++i)
        for (int j = nums[i]; j <= n; ++j) 
            dp[j] = min(dp[j], dp[j - nums[i]] + 1);
    return dp[n];
}
```
运行效率如下：
```cpp
执行用时：120 ms, 在所有 C 提交中击败了38.96% 的用户
内存消耗：8.4 MB, 在所有 C 提交中击败了32.28% 的用户
```


---
### 解法3 数学（四平方定理）
四平方定理：**任何正整数都可以表示为不超过 $4$ 个整数的平方之和**。于是可以推论：
- 如果一个数，最少可以拆成四数平方和，这这个数 $n$ 必定满足 $n=4^a(8b+7)$ ，因此先看这个数是否满足这一公式，不满足则答案只能是 $1, 2, 3$ ；
- 如果这个数本身是某个数的平方，那么答案就是 $1$ ；否则答案只能是 $2, 3$ ；
- 如果答案是 $2$ ，则 $n = a^2 + b^2$ ，不断枚举 $a$ 来验证，如果通过则答案是 $2$ ；
- 最后都不满足的情况下，答案只能是 $3$ 。

```cpp
class Solution {
public:
    int numSquares(int n) { 
        while (n % 4 == 0) n /= 4;
        if (n % 8 == 7) return 4;
        for (int a = 0; a * a <= n; ++a) {
            int b = (int)(sqrt(n - a * a));
            if (a * a + b * b == n) return (!a || !b) ? 1 : 2;
        }
        return 3;
    }
};
```
运行结果如下，可以发现效率相当高：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.8 MB, 在所有 C++ 提交中击败了97.05% 的用户
```
