You are given a series of video clips from a sporting event that lasted `T` seconds.  These video clips can be overlapping with each other and have varied lengths.

Each video clip `clips[i]` is an **interval**: it starts at time `clips[i][0]` and ends at time `clips[i][1]`.  We can cut these clips into segments freely: for example, a clip `[0, 7]` can be cut into segments `[0, 1] + [1, 3] + [3, 7]`.

Return the **minimum** number of clips needed so that we can cut the clips into segments that cover the entire sporting event (`[0, T]`) .  If the task is impossible, return `-1` .

 

**Example 1:**

```swift
Input: clips = [[0,2],[4,6],[8,10],[1,9],[1,5],[5,9]], T = 10
Output: 3
Explanation: 
We take the clips [0,2], [8,10], [1,9]; a total of 3 clips.
Then, we can reconstruct the sporting event as follows:
We cut [1,9] into segments [1,2] + [2,8] + [8,9].
Now we have segments [0,2] + [2,8] + [8,10] which cover the sporting event [0, 10].
```

**Example 2:**

```swift
Input: clips = [[0,1],[1,2]], T = 5
Output: -1
Explanation: 
We can't cover [0,5] with only [0,1] and [1,2].
```

**Example 3:**

```swift
Input: clips = [[0,1],[6,8],[0,2],[5,6],[0,4],[0,3],[6,7],[1,3],[4,7],[1,4],[2,5],[2,6],[3,4],[4,5],[5,7],[6,9]], T = 9
Output: 3
Explanation: 
We can take clips [0,4], [4,7], and [6,9].
```

**Example 4:**
```swift
Input: clips = [[0,4],[2,8]], T = 5
Output: 2
Explanation: 
Notice you can have extra video after the event ends.
```
**Constraints:**
- `1 <= clips.length <= 100`
- `0 <= clips[i][0] <= clips[i][1] <= 100`
- `0 <= T <= 100`

题意：给出一系列视频片段，这些片段来自于一项持续时长为 `T` 秒的体育赛事。这些片段可能有所重叠，也可能长度不一。视频片段 `clips[i]` 是一个区间：开始于 `clips[i][0]` 并于 `clips[i][1]` 结束。我们可以对这些片段自由地再剪辑，例如片段 `[0, 7]` 可以剪切成 `[0, 1] + [1, 3] + [3, 7]` 三部分。

题目要求我们将这些片段进行再剪辑，并将剪辑后的内容拼接成覆盖整个运动过程的片段 `[0, T]` 。返回所需片段的最小数目，如果无法完成该任务，则返回 `-1` 。


---
### 解法1 贪心
先根据**区间右值**从大到小排序，然后遍历区间，找到右值大于等于当前 `T` 的区间（一开始 `T` 是原值），不断比较更新得到这些区间中**最小的左值**；当没有 `>= T` 的区间时，相当于我们选择了一个区间右值大于等于 `T` 、区间左值最小的区间（覆盖范围最广），此时 `++cnt` ，并更新 `T = cur` 。当 `T == 0` 时结束，或者找不到右值 `>= T` 的区间而直接返回 `-1` 。具体代码如下：
```cpp
class Solution {
public:
    int videoStitching(vector<vector<int>>& clips, int T) {
        sort(clips.begin(), clips.end(), [&](const vector<int> &a, const vector<int> &b) {
            return a[1] > b[1];
        });
        int cur = T, cnt = 0, i = 0, clipsSize = clips.size();
        while (T != 0) {
            if (i < clipsSize && clips[i][1] >= T) {
                cur = min(cur, clips[i][0]);
                ++i;
            } else {
                if (cur < T) {
                    T = cur;
                    ++cnt;
                } else return -1;
            }
        }
        return cnt;
    }
};
```
提交后效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了49.89% 的用户
内存消耗：7.7 MB, 在所有 C++ 提交中击败了38.84% 的用户
```
---
### 解法2 动态规划
设置一个 `dp[]` 数组，`dp[i]` 为**时间长度为 `i` 的运动过程**需要的最少的区间数目。显然 `dp[0] = 0` ，然后对每一秒，循环整个片段数组，如果片段 `clips[j]` 范围覆盖这一秒，则进行一次选择，是维持当前的区间数目，还是 `dp[clips[j][0]]` 的区间数目 `+ 1` 。具体代码如下：
```cpp
class Solution {
public:
    int videoStitching(vector<vector<int>>& clips, int T) {
        vector<int> dp(T + 1, T + 1);
        dp[0] = 0;
        for (int i = 1; i <= T; ++i) 
            for (const vector<int>& clip : clips) 
                if (clip[0] <= i && clip[1] >= i) 
                    dp[i] = min(dp[i], dp[clip[0]] + 1);
        return dp[T] == T + 1 ? -1 : dp[T];
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了93.16% 的用户
内存消耗：7.8 MB, 在所有 C++ 提交中击败了34.99% 的用户
```

> **2020/11/14 Update** 本次写的代码如下：
> ```cpp 
> class Solution { public:
>     int videoStitching(vector<vector<int>>& clips, int T) {
>         vector<int> dp(T + 1, 110);
>         //dp[i]表示覆盖一项持续时间为i的体育赛事,需要的最少视频片段
>         dp[0] = 0;
>         for (int i = 1; i <= T; ++i) {
>             bool flag = false;
>             for (vector<int>& clip : clips) {
>                 if (clip[0] <= i && i <= clip[1]) {
>                     dp[i] = min(dp[i], dp[clip[0]] + 1);
>                     flag = true;
>                 }
>             }
>             if (flag == false) return -1; //不覆盖这一时刻
>         }
>         return dp[T] == 110 ? -1 : dp[T]; //注意最后的判断!
>     } 
> }; 
> ```
