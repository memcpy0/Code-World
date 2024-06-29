![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304081038054.png)


### 解法概述 转换成01背包问题
重新描述一遍问题：从 `people` 中选择一些元素（技能集合），这些技能集合的并集等于 `reqSkills` ，要求选的元素个数尽量少。

把 `people[i]` 看成**物品**（这里是个集合），`reqSkills` 看成**背包容量**（这里是个目标集合），选出元素个数尽量少看做本题目标，这就是一个集合版本的0-1背包问题——我们从 $m$ 个物品集合中选择，每个物品集合只能选择一次，选择的物品集合必须装满指定“容量”的背包（或者说选择的物品集合的并集要包含「指定需要的所有物品」），且选择的集合数尽量少。

为方便计算，先把 `reqSkills` 中的每个字符串映射到某个下标上，记到一个哈希表 `mp` 中。然后**把 `reqSkills` 和每个 `people[i]` 都通过映射转换成数字集合，再压缩成一个二进制数**。

例如示例1，把 `"java","nodejs","reactjs"` 分别映射到 $0,1,2$ 上，那么 `people[0],people[1],people[2]` 按照这种映射关系就转换成集合 $\{0\},\{1\},\{1,2\}$ ，对应的二进制数分别为 $1_{(2)}, 10_{(2)}, 110_{(2)}$ 。那么选择集合 $\{0\}$ 和 $\{1,2\}$ ，它俩的并集为 $\{0,1,2\}$ ，满足题目要求。这等价于选择二进制数 $1_{(2)}$ ​和 $110_{(2)}$ ，它俩的或运算的结果是 $111_{(2)}$ ，就对应着集合 $\{0,1,2\}$ 。

---
### 解法1 DFS解01背包
类似0-1背包，定义 $dfs(i,j)$ 表示从前 $i$ 个集合中选择一些集合，并集包含 $j$ ，至少需要选择的集合个数。分类讨论：
- 不选第 $i$ 个集合：$dfs(i,j)=dfs(i−1,j)$ 。
- 选第 $i$ 个集合：$\textit{dfs}(i, j) = \textit{dfs}(i-1, j\setminus\textit{people}[i])+1$ 。这是因为选了第 $i$ 个集合 $\textit{people}[i]$ 后，就不需要再包含 $people[i]$ 中的任意元素了，所以直接去掉，去计算包含 $j\setminus\textit{people}[i]$ 至少要选多少个集合。
 - 取最小值，即 $\textit{dfs}(i, j) = \min(\textit{dfs}(i-1, j),\textit{dfs}(i-1, j\setminus\textit{people}[i])+1)$ 。

由于本题还需要输出具体方案，**为了方便存储，把人员编号集合也用二进制数表示**。因为本题 `people` 的长度不超过 $60$ ，可以压缩到一个 $64$ 位整数中。如果长度大于 $64$ 呢？更加通用的做法是使用 `bitset` 。

修改后，**$dfs(i,j)$ 定义成从前 $i$ 个集合中选择一些集合，并集包含 $j$ ，所选择的最小下标集合**：
- 不选第 $i$ 个集合：$dfs(i,j)=dfs(i−1,j)$ 。
- 选第 $i$ 个集合：$\textit{dfs}(i, j) = \textit{dfs}(i-1, j\setminus\textit{people}[i])\cup \{i\}$ 。
- **取这两个集合中大小最小的**。

递归边界：如果 $j=\varnothing$ ，返回 $\varnothing$ 。如果 $i<0$ ，返回全集 $U={0,1,⋯ ,m−1}$（也可以再多加一个 $m$ ，不过由于题目保证答案存在，这样就够了）。上述是用集合的语言描述的，代码中用位运算实现。
```cpp
class Solution { 
public: 
    vector<int> smallestSufficientTeam(vector<string>& req_skills, vector<vector<string>>& people) {
        int n = req_skills.size(), m = people.size();
        unordered_map<string, int> mp; 
        for (int i = 0; i < n; ++i) mp[req_skills[i]] = i;
        int mask[m]; memset(mask, 0, sizeof(mask));
        for (int i = 0; i < m; ++i) {  
            for (auto &s : people[i]) // 把people[i]压缩为一个二进制数mask[i]
                mask[i] |= 1 << mp[s]; 
        }  
        int tot = 1 << n;
        // memo[i][j]从前i个集合选择一些集合、并集包含j、要选择的最小下标集合
        long long memo[m][tot], all = (1LL << m) - 1; // all为longlong避免溢出
        memset(memo, -1, sizeof(memo)); // -1表示没有计算过
        function<long long(int, int)> dfs = [&](int i, int j) -> long long {
            if (j == 0) return 0; // 并集包含0,背包已装满
            if (i < 0) return all; // 没法装满背包，返回全集，这样下面比较集合大小会取更小的
            auto &res = memo[i][j]; // 注意这里是引用，下面会直接修改 memo[i][j]
            if (res != -1) return res;
            auto r1 = dfs(i - 1, j); // 不选mask[i]
            auto r2 = dfs(i - 1, j & ~mask[i]) | (1LL << i); // 选mask[i]
            return res = __builtin_popcountll(r1) < __builtin_popcountll(r2) ? r1 : r2;
        };
        auto res = dfs(m - 1, tot - 1);
        vector<int> ans;
        for (int i = 0; i < m; ++i)
            if ((res >> i) & 1)
                ans.push_back(i); // 所有在 res 中的下标
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(T+m2^n)$ ，其中 $T$ 为 $\textit{people}$ 中的字符串的长度之和，$m$ 为 $\textit{people}$ 的长度，$n$ 为 $\textit{reqSkills}$ 的长度。忽略比较字符串的时间。初始化数组 $mask$ 需要 $O(T)$ 的时间。由于每个状态只会计算一次，记忆化搜索的时间复杂度 = 状态个数 $\times$ 单个状态的计算时间。本题状态个数为 $O(m2^n)$ ，单个状态的计算时间为 $O(1)$ ，因此记忆化搜索的时间复杂度为 $O(m2^n)$ 。
- 空间复杂度：$O(S+n2^m)$ ，其中 $S$ 为 $reqSkills$ 中的字符串的长度之和。注意 $T\ge S$ ，所以时间复杂度中忽略了 $S$ 。

### 解法2 1:1翻译成递推
把 `dfs` 改成 `dp` 数组，把递归改成循环就好了。相当于原来是用递归计算每个状态 $(i, j)$ ，现在改用循环去计算每个状态 $(i, j)$ 。由于需要处理 $i<0$ ，也就是需要 $dp[-1]$ 这个状态，那么在 $dp$ 的前面插入一个状态，$dp[0]$ 就对应到 $i<0$ 的情况了，原来的 $dp[i]$ 需要变成 $f[i+1]$ 。
> 问：为什么变慢了？
> 答：因为有很多状态是不需要计算的。比如集合 {0}\{0\}{0} 和 {1,2}\{1,2\}{1,2}，在记忆化搜索中，是不会递归到 j={0,1}j=\{0,1\}j={0,1} 这种集合的，而递推需要计算所有状态。没关系，后面会优化。
 ```java
class Solution {
public:
    vector<int> smallestSufficientTeam(vector<string> &req_skills, vector<vector<string>> &people) {
        unordered_map<string, int> sid;
        int m = req_skills.size();
        for (int i = 0; i < m; ++i)
            sid[req_skills[i]] = i; // 字符串映射到下标

        int n = people.size(), u = 1 << m;
        // f[i+1][j] 表示从前 i 个集合中选择一些集合，并集等于 j，需要选择的最小集合
        long long f[n + 1][u];
        fill(f[0], f[0] + u, (1LL << n) - 1); // 对应记忆化搜索中的 if (i < 0) return all;
        f[0][0] = 0;
        for (int i = 0; i < n; ++i) {
            int mask = 0;
            for (auto &s: people[i]) // 把 people[i] 压缩成一个二进制数 mask[i]
                mask |= 1 << sid[s];
            f[i + 1][0] = 0;
            for (int j = 1; j < u; ++j) {
                auto r1 = f[i][j]; // 不选 mask
                auto r2 = f[i][j & ~mask] | (1L << i); // 选 mask
                f[i + 1][j] = __builtin_popcountll(r1) < __builtin_popcountll(r2) ? r1 : r2;
            }
        }
        auto res = f[n][u - 1];

        vector<int> ans;
        for (int i = 0; i < n; ++i)
            if ((res >> i) & 1)
                ans.push_back(i); // 所有在 res 中的下标
        return ans;
    }
};
```
复杂度分析
时间复杂度：O(T+n2m)O(T+n2^m)O(T+n2 
m
 )，其中 TTT 为 people\textit{people}people 中的字符串的长度之和，nnn 为 people\textit{people}people 的长度，mmm 为 reqSkills\textit{reqSkills}reqSkills 的长度。忽略比较字符串的时间。初始化数组 mask\textit{mask}mask 需要 O(T)O(T)O(T) 的时间。由于每个状态只会计算一次，递推的时间复杂度 === 状态个数 ×\times× 单个状态的计算时间。本题状态个数为 O(n2m)O(n2^m)O(n2 
m
 )，单个状态的计算时间为 O(1)O(1)O(1)，因此递推的时间复杂度为 O(n2m)O(n2^m)O(n2 
m
 )。
空间复杂度：O(S+n2m)O(S+n2^m)O(S+n2 
m
 )，其中 SSS 为 reqSkills\textit{reqSkills}reqSkills 中的字符串的长度之和。
三、空间优化
由于计算 f[i+1]f[i+1]f[i+1] 只需要 f[i]f[i]f[i]，不需要下标更小的，所以只需要一个长为 2m2^m2 
m
  的数组。

实现时需要倒序循环 jjj，原理见 0-1 背包与完全背包【基础算法精讲 18】。

```
class Solution {
public:
    vector<int> smallestSufficientTeam(vector<string> &req_skills, vector<vector<string>> &people) {
        unordered_map<string, int> sid;
        int m = req_skills.size();
        for (int i = 0; i < m; ++i)
            sid[req_skills[i]] = i; // 字符串映射到下标

        int n = people.size(), u = 1 << m;
        long long f[u];
        fill(f, f + u, (1LL << n) - 1);
        f[0] = 0;
        for (int i = 0; i < n; ++i) {
            int mask = 0;
            for (auto &s: people[i]) // 把 people[i] 压缩成一个二进制数 mask[i]
                mask |= 1 << sid[s];
            for (int j = u - 1; j; --j) {
                auto r1 = f[j]; // 不选 mask
                auto r2 = f[j & ~mask] | (1L << i); // 选 mask
                f[j] = __builtin_popcountll(r1) < __builtin_popcountll(r2) ? r1 : r2;
            }
        }
        auto res = f[u - 1];

        vector<int> ans;
        for (int i = 0; i < n; ++i)
            if ((res >> i) & 1)
                ans.push_back(i); // 所有在 res 中的下标
        return ans;
    }
};
```
复杂度分析
时间复杂度：O(T+n2m)O(T+n2^m)O(T+n2 
m
 )，其中 TTT 为 people\textit{people}people 中的字符串的长度之和，nnn 为 people\textit{people}people 的长度，mmm 为 reqSkills\textit{reqSkills}reqSkills 的长度。忽略比较字符串的时间。初始化数组 mask\textit{mask}mask 需要 O(T)O(T)O(T) 的时间。由于每个状态只会计算一次，递推的时间复杂度 === 状态个数 ×\times× 单个状态的计算时间。本题状态个数为 O(n2m)O(n2^m)O(n2 
m
 )，单个状态的计算时间为 O(1)O(1)O(1)，因此递推的时间复杂度为 O(n2m)O(n2^m)O(n2 
m
 )。
空间复杂度：O(S+2m)O(S+2^m)O(S+2 
m
 )，其中 SSS 为 reqSkills\textit{reqSkills}reqSkills 中的字符串的长度之和。
四、查表法 vs 刷表法
上面的做法算作查表法，意思是用其它状态更新当前状态。但是这种写法无法跳过无效的状态，在很多不必要的计算上浪费了大量时间。

我们还可以用当前状态去更新其它状态，从小到大遍历每个 f[j]f[j]f[j]，然后遍历 mask\textit{mask}mask，用 f[j]f[j]f[j] 去更新 f[j∣mask[i]]f[j|\textit{mask}[i]]f[j∣mask[i]]。

由于我们是从小到大遍历 jjj，并且更新也是更新到比 jjj 更大的数上，如果 f[j]f[j]f[j] 等于其初始值，说明没有它被更新过，也就说明 jjj 无法由若干集合的并集得到，是无效状态，可以直接跳过。
 
```
class Solution {
public:
    vector<int> smallestSufficientTeam(vector<string> &req_skills, vector<vector<string>> &people) {
        unordered_map<string, int> sid;
        int m = req_skills.size();
        for (int i = 0; i < m; ++i)
            sid[req_skills[i]] = i; // 字符串映射到下标

        int n = people.size(), mask[n];
        memset(mask, 0, sizeof(mask));
        for (int i = 0; i < n; ++i)
            for (auto &s: people[i]) // 把 people[i] 压缩成一个二进制数 mask[i]
                mask[i] |= 1 << sid[s];

        int u = 1 << m;
        long long all = (1LL << n) - 1, f[u];
        fill(f, f + u, all);
        f[0] = 0;
        for (int j = 0; j < u - 1; ++j) // f[u-1] 无需计算
            if (f[j] < all)
                for (int i = 0; i < n; ++i)
                    if (__builtin_popcountll(f[j]) + 1 < __builtin_popcountll(f[j | mask[i]]))
                        f[j | mask[i]] = f[j] | (1LL << i); // 刷表：用 f[j] 去更新其它状态
        auto res = f[u - 1];

        vector<int> ans;
        for (int i = 0; i < n; ++i)
            if ((res >> i) & 1)
                ans.push_back(i); // 所有在 res 中的下标
        return ans;
    }
};
```
优化
由于计算二进制中 111 的个数也比较耗时，新开一个数组 ids\textit{ids}ids 单独记录压缩后的下标集合。这样 f[j]f[j]f[j] 的含义就是并集为 jjj 最少要选的集合个数了。

```
 
class Solution {
public:
    vector<int> smallestSufficientTeam(vector<string> &req_skills, vector<vector<string>> &people) {
        unordered_map<string, int> sid;
        int m = req_skills.size();
        for (int i = 0; i < m; ++i)
            sid[req_skills[i]] = i; // 字符串映射到下标

        int n = people.size(), mask[n];
        memset(mask, 0, sizeof(mask));
        for (int i = 0; i < n; ++i)
            for (auto &s: people[i]) // 把 people[i] 压缩成一个二进制数 mask[i]
                mask[i] |= 1 << sid[s];

        int u = 1 << m;
        long long ids[u]; memset(ids, 0, sizeof(ids));
        char f[u]; memset(f, 0x7f, sizeof(f));
        f[0] = 0;
        for (int j = 0; j < u - 1; ++j) // f[u-1] 无需计算
            if (f[j] < 0x7f)
                for (int i = 0; i < n; ++i)
                    if (f[j] + 1 < f[j | mask[i]]) {
                        f[j | mask[i]] = f[j] + 1; // 刷表：用 f[j] 去更新其它状态
                        ids[j | mask[i]] = ids[j] | (1LL << i);
                    }
        auto res = ids[u - 1];

        vector<int> ans;
        for (int i = 0; i < n; ++i)
            if ((res >> i) & 1)
                ans.push_back(i); // 所有在 res 中的下标
        return ans;
    }
};
```
复杂度分析
时间复杂度：O(T+n2m)O(T+n2^m)O(T+n2 
m
 )，其中 TTT 为 people\textit{people}people 中的字符串的长度之和，nnn 为 people\textit{people}people 的长度，mmm 为 reqSkills\textit{reqSkills}reqSkills 的长度。忽略比较字符串的时间。初始化数组 mask\textit{mask}mask 需要 O(T)O(T)O(T) 的时间。由于每个状态只会计算一次，递推的时间复杂度 === 状态个数 ×\times× 单个状态的计算时间。本题状态个数为 O(n2m)O(n2^m)O(n2 
m
 )，单个状态的计算时间为 O(1)O(1)O(1)，因此递推的时间复杂度为 O(n2m)O(n2^m)O(n2 
m
 )。
空间复杂度：O(S+n+2m)O(S+n+2^m)O(S+n+2 
m
 )，其中 SSS 为 reqSkills\textit{reqSkills}reqSkills 中的字符串的长度之和。
相似题目（0-1 背包）
494. 目标和
416. 分割等和子集
2518. 好分区的数目
 