@[toc]
# 1332. 删除回文子序列 

    用户通过次数 295
    用户尝试次数 443
    通过次数 304
    提交次数 829
    题目难度 Easy

给你一个字符串`s`，它仅由字母` 'a' `和` 'b' `组成。每一次删除操作都可以从`s`中删除一个回文**子序列**。

返回删除给定字符串中所有字符（字符串为空）的最小删除次数。

「子序列」定义：如果一个字符串可以通过删除原字符串某些字符而**不改变原字符顺序**得到，那么这个字符串就是原字符串的一个子序列。

「回文」定义：如果一个字符串向后和向前读是一致的，那么这个字符串就是一个回文。

 

示例 1：
```
输入：s = "ababa"
输出：1
解释：字符串本身就是回文序列，只需要删除一次。
```
示例 2：
```
输入：s = "abb"
输出：2
解释："abb" -> "bb" -> "". 
先删除回文子序列 "a"，然后再删除 "bb"。
```
示例 3：
```
输入：s = "baabb"
输出：2
解释："baabb" -> "b" -> "". 
先删除回文子序列 "baab"，然后再删除 "b"。
```
示例 4：
```
输入：s = ""
输出：0
```

提示：
-  `0 <= s.length <= 1000`
-   `s` 仅包含字母 'a'  和 'b'

题意：返回删除字符串中所有字符需要的**最小删除数**，每次删除都从该字符串中删除一个回文子序列。乍一看起来有点难，因为是回文子序列而非子串。

一开始，我想到的是下面的写法，因为给出示例的解释，写成了【删除回文子串】：
```c
class Solution {
public:
    bool isPalindrome(const string& s, int lo , int hi) {
        for (; lo < hi; lo++, hi--) {
            if (s[lo] != s[hi]) return false;
        }
        return true;
    }
    int removePalindromeSub(string s) {
        if (s == " ") return 0;
        int ans = 0;
        for (int i = 0; i < s.size(); ) { 
            int j = s.size() - 1;
            for (; j >= i; --j) {
                if (isPalindrome(s, i, j)) {
                    ans++; 
                    break;
                }
            }
            i = j + 1;
        }
        return ans;   
    }
};
```
发现一个测试用例通不过，`bbaababa`按照上面的写法，先删`bbaa`，再删`b`，再删`aba`，需要3次，但是给出的是只用两次......

这是为什么？然后我发现，题目给出的字符串只有‘a’和‘b’两种字符，因此按照子序列的定义，最多删除两次，先删'a'，再删'b'。emm，答案就很简单了。其实示例的解释如果这样写的话，...就没有难度了。

答案：
```cpp
class Solution {
public: 
    int removePalindromeSub(string s) { 
        if (s.size() == 0) return 0;
        bool flag = true;
        for (int i = 0, j = s.size() - 1; flag && i < j; ++i, --j) 
            if (s[i] != s[j]) flag = false;
        if (flag) return 1;
        else return 2; 
    }
};
```

# 1333. 餐厅过滤器 
    用户通过次数 412
    用户尝试次数 501
    通过次数 418
    提交次数 1317
    题目难度 Medium

给你一个餐馆信息数组`restaurants`，其中`restaurants[i] = [idi, ratingi, veganFriendlyi, pricei, distancei]`。你必须使用以下三个过滤器来过滤这些餐馆信息。

其中素食者友好过滤器`veganFriendly`的值可以为`true`或者`false`，如果为 *true* 就意味着你应该只包括 `veganFriendlyi`为 *true* 的餐馆，为 *false* 则意味着可以包括任何餐馆。此外，我们还有最大价格`maxPrice`和最大距离`maxDistance`两个过滤器，它们分别考虑餐厅的价格因素和距离因素的最大值。

过滤后返回餐馆的 ***id***，按照 ***rating*** 从高到低排序。如果 ***rating*** 相同，那么按 ***id*** 从高到低排序。简单起见，`veganFriendlyi` 和`veganFriendly`为 *true* 时取值为 *1*，为 *false* 时，取值为 *0* 。


示例 1：
```
输入：restaurants = [[1,4,1,40,10],[2,8,0,50,5],[3,8,1,30,4],[4,10,0,10,3],[5,1,1,15,1]], veganFriendly = 1, maxPrice = 50, maxDistance = 10
输出：[3,1,5] 
解释： 
这些餐馆为：
餐馆 1 [id=1, rating=4, veganFriendly=1, price=40, distance=10]
餐馆 2 [id=2, rating=8, veganFriendly=0, price=50, distance=5]
餐馆 3 [id=3, rating=8, veganFriendly=1, price=30, distance=4]
餐馆 4 [id=4, rating=10, veganFriendly=0, price=10, distance=3]
餐馆 5 [id=5, rating=1, veganFriendly=1, price=15, distance=1] 
在按照 veganFriendly = 1, maxPrice = 50 和 maxDistance = 10 进行过滤后，我们得到了餐馆 3, 餐馆 1 和 餐馆 5（按评分从高到低排序）。 
```
示例 2：
```
输入：restaurants = [[1,4,1,40,10],[2,8,0,50,5],[3,8,1,30,4],[4,10,0,10,3],[5,1,1,15,1]], veganFriendly = 0, maxPrice = 50, maxDistance = 10
输出：[4,3,2,1,5]
解释：餐馆与示例 1 相同，但在 veganFriendly = 0 的过滤条件下，应该考虑所有餐馆。
```
示例 3：
```
输入：restaurants = [[1,4,1,40,10],[2,8,0,50,5],[3,8,1,30,4],[4,10,0,10,3],[5,1,1,15,1]], veganFriendly = 0, maxPrice = 30, maxDistance = 3
输出：[4,5]
```
 

提示：

-   `1 <= restaurants.length <= 10^4`
-   `restaurants[i].length == 5`
-   `1 <= idi, ratingi, pricei, distancei <= 10^5`
-   `1 <= maxPrice, maxDistance <= 10^5`
-   `veganFriendlyi `和` veganFriendly `的值为 0 或 1 。
-   所有 `idi` 各不相同。

题意：按照函数参数的要求，返回需要的餐厅id的向量。

思路：简单题目，需要注意的是两个地方：
- 素食者友好过滤器`veganFriendly`的值可以为`true`或者`false`，如果为 *true* 就意味着你应该只包括 `veganFriendlyi`为 *true* 的餐馆，为 *false* 则意味着可以包括任何餐馆。
- 过滤后返回餐馆的 ***id***，按照 ***rating*** 从高到低排序。如果 ***rating*** 相同，那么按 ***id*** 从高到低排序。

答案：
先是简单的排序，用sort就好了。不过这里有点需要注意的地方。

然后，我这里筛选素食者友好的就是一个真值表：
| `veganFriendly` | `veganFriendlyi` | value |
|----|--|----|
| 0  | 0  | 1
| 0 | 1  | 1
| 1 | 0  | 0
| 1  |1  | 1

因此，`!veganFriendly` | `veganFriendlyi`应该等于1，这样才能过筛选。

先排序，再筛选：

```cpp
class Solution {
public:
    static bool cmp(const vector<int> &a, const vector<int> &b) {
        if (a[1] != b[1]) return a[1] > b[1];
        if (a[0] != b[0]) return a[0] > b[0]; // 所有 idi 各不相同
        return true;
    }
    
    vector<int> filterRestaurants(vector<vector<int>>& restaurants, int veganFriendly, int maxPrice, int maxDistance) {
        vector<int> ans;
        sort(restaurants.begin(), restaurants.end(), cmp); 
        for (int i = 0; i < restaurants.size(); ++i)
            if (((!veganFriendly) | restaurants[i][2]) && restaurants[i][3] <= maxPrice && restaurants[i][4] <= maxDistance) 
                ans.push_back(restaurants[i][0]);
        return ans;
    }
};
```
或者先筛选，再排序：
```cpp
vector<vector<int>> ans;
inline bool cmp(const vector<int> &a, const vector<int> &b) {
    if (a[1] != b[1]) return a[1] > b[1];
    return a[0] > b[0]; // 所有 idi 各不相同 
}
class Solution {
public:    
    vector<int> filterRestaurants(vector<vector<int>>& restaurants, int veganFriendly, int maxPrice, int maxDistance) {
        ans.clear();
        for (auto res : restaurants) {
            int vegan = res[2], price = res[3], dist = res[4];
            if (veganFriendly == 1 && vegan == 0) continue; // 1 0 时直接跳过
            if (price > maxPrice) continue;
            if (dist > maxDistance) continue;
            ans.push_back(res); 
        }
        sort(ans.begin(), ans.end(), cmp);
        
        vector<int> ret;
        for (auto a : ans) ret.push_back(a[0]);
        
        return ret;
    }
};
```
# 阈值距离内邻居最少的城市

思路：

答案：
```cpp

```
 # 1335. 工作计划的最低难度  
    用户通过次数 103
    用户尝试次数 144
    通过次数 103
    提交次数 228
    题目难度 Hard

你需要制定一份`d`天的工作计划表。工作之间存在依赖，要想执行第 `i` 项工作，你必须完成全部 `j ` 项工作（` 0 <= j < i`）。

你每天 **至少** 需要完成一项任务。工作计划的总难度是这 ` d ` 天每一天的难度之和，而一天的工作难度是当天应该完成工作的**最大难度**。

给你一个整数数组 `jobDifficulty` 和一个整数 `d`，分别代表工作难度和需要计划的天数。第 `i` 项工作的难度是 `jobDifficulty[i]`。

返回整个工作计划的 **最小难度** 。如果无法制定工作计划，则返回 **-1** 。

 

示例 1：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200128010206396.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
```
输入：jobDifficulty = [6,5,4,3,2,1], d = 2
输出：7
解释：第一天，您可以完成前 5 项工作，总难度 = 6.
第二天，您可以完成最后一项工作，总难度 = 1.
计划表的难度 = 6 + 1 = 7 
```
示例 2：
```
输入：jobDifficulty = [9,9,9], d = 4
输出：-1
解释：就算你每天完成一项工作，仍然有一天是空闲的，你无法制定一份能够满足既定工作时间的计划表。
```
示例 3：
```
输入：jobDifficulty = [1,1,1], d = 3
输出：3
解释：工作计划为每天一项工作，总难度为 3 。
```
示例 4：
```
输入：jobDifficulty = [7,1,7,1,7,1], d = 3
输出：15
```
示例 5：
```
输入：jobDifficulty = [11,111,22,222,33,333,44,444], d = 6
输出：843
```
 
提示：

-  `1 <= jobDifficulty.length <= 300`
 - `0 <= jobDifficulty[i] <= 1000`
-  `1 <= d <= 10`

```cpp
const int MAXN = 300 + 20;
int dp[15][MAXN];

class Solution {
public:
    int minDifficulty(vector<int>& jobDifficulty, int d) {
        int n = jobDifficulty.size();
        if (n < d) return -1; 
        
        memset(dp, -1, sizeof(dp));
        dp[0][0] = 0;
        
        for (int i = 1; i <= d; i++) {
            for (int cur = 1; cur <= n; cur++) {
                int mx = jobDifficulty[cur - 1];
                for (int bac = cur - 1; bac >= 0; bac--) {
                    if (dp[i - 1][bac] != -1) 
                        if (dp[i][cur] == -1 || dp[i][cur] > mx + dp[i - 1][bac])
                        dp[i][cur] = mx + dp[i - 1][bac];
                    
                    if (bac > 0) mx = max(mx, jobDifficulty[bac - 1]);
                }
            }
        }
        return dp[d][n];                                
    }
};
```







