> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

你有一套活字字模 `tiles`，其中每个字模上都刻有一个字母 `tiles[i]`。返回你可以印出的非空字母序列的数目。

**注意：**本题中，每个活字字模只能使用一次。

**示例 1：**
```java
输入："AAB"
输出：8
解释：可能的序列为 "A", "B", "AA", "AB", "BA", "AAB", "ABA", "BAA"。
```
**示例 2：**
```java
输入："AAABBC"
输出：188
```
**示例 3：**
```java
输入："V"
输出：1
```
**提示：**
-   `1 <= tiles.length <= 7`
-   `tiles` 由大写英文字母组成
 
---
### 解法1 回溯+排列+子集生成
随手写的代码：
```cpp
class Solution {
private:
    int ans = 0;
    unordered_set<string> rec;
    void dfs(string& s, string& t, int b, int e) {
        if (b >= e) {
            string f = t;
            if (f.empty()) return; 
            sort(f.begin(), f.end()); 
            do { 
                if (rec.find(f) == rec.end()) {
                    ++ans;
                    rec.insert(f);
                }
            } while (next_permutation(f.begin(), f.end()));
            return ;
        } 
        dfs(s, t, b + 1, e);
        t.push_back(s[b]); 
        dfs(s, t, b + 1, e); 
        t.pop_back(); 
    }
public:
    int numTilePossibilities(string tiles) {
        string s;
        dfs(tiles, s, 0, tiles.size());
        return ans;
    }
};  
```
---
### 解法2 动态规划
#### 1. 寻找子问题
以 $tiles=\texttt{AABCC}$ 为例。先来思考，如何计算长为 $5$ 的序列的数目？
- 由于**相同字母不作区分**，先考虑 $2$ 个 $\texttt{C}$ 如何放置。这等价于在 $5$ 个位置中选 $2$ 个位置放 $\texttt{C}$ ，其余位置放 $\texttt{AAB}$ 。这 $2$ 个 $\texttt{C}$ 有 $\dbinom 5 2=10$ 种放法——表示从 $n$ 个数中选 $k$ 个数的方案数，即 $\dfrac{n!}{k!(n-k)!}$ 
- 剩余要解决的问题为，用 $\texttt{AAB}$ 构造长为 $3$ 的序列的数目。这是一个与原问题相似，且规模更小的子问题。
 
#### 2. 状态定义与转移
根据上面的讨论，**定义 $f[i]$ 表示用前 $i$ 种字符构造长为 $j$ 的序列的方案数**。

设第 $i$ 种字符有 $\textit{cnt}$ 个：
- 如果一个也不选，那么 $f[i][j] = f[i - 1][j]$ 。
- 如果选 $k$ 个，那么需要从 $j$ 个位置中选 $k$ 个放第 $i$ 种字符，其余位置就是用前 $i−1$ 种字符构造长为 $j-k$ 的序列的方案数，所以有 $f[i][j] =f[i-1][j-k]\cdot \dbinom j k$ 。这里 $k\le\min(j,\textit{cnt})$ 。特别地，一个也不选相当于 $k=0$ 的情况。

所以，**枚举 $k=0,1,\cdots,\min(j,\textit{cnt})$ ，把所有方案数相加，就得到了 $f[i][j]$** ，对应的状态转移方程为：
$$f[i][j] = \sum_{k=0}^{\min(j,\textit{cnt})} f[i-1][j-k]\cdot \binom j k$$

初始值：$f[0][0] = 1$ ，构造空序列的方案数为 $1$ 。
答案：$\sum\limits_{j=1}^{n}f[m][j]$ ，其中 $m$ 为 $\textit{tiles}$ 中的字母种数。

代码实现时，组合数可以用如下恒等式预处理：
$$\binom {n}{k} = \binom{n-1}{k-1}+\binom{n-1}{k}$$
这个式子本质是考虑第 $n$ 个数「选或不选」。如果选，那么问题变成从 $n−1$ 个数中选 $k-1$ 个数的方案数；如果不选，那么问题变成从 $n−1$ 个数中选 $k$ 个数的方案数。二者相加即为从 $n$ 个数中选 $k$  个数的方案数。
```java
class Solution {
    private static final int MX = 8;
    private static final int[][] c = new int[MX][MX];
    static {
        for (int i = 0; i < MX; ++i) {
            c[i][0] = c[i][i] = 1;
            for (int j = 1; j < i; ++j) {
                c[i][j] = c[i - 1][j - 1] + c[i - 1][j]; // 预处理组合数
            }
        }
    } 
    public int numTilePossibilities(String tiles) {
        var counts = new HashMap<Character, Integer>(); // 统计每个字母的出现次数
        for (var c : tiles.toCharArray())
            counts.merge(c, 1, Integer::sum); // counts[c]++
        int m = counts.size(), n = tiles.length();
        var f = new int[m + 1][n + 1]; // 以前i种字母构造j长序列的方案数
        f[0][0] = 1; // 构造空序列的方案数
        int i = 1; 
        for (var cnt : counts.values()) { // 枚举第i种字母
            for (int j = 0; j <= n; ++j) // 枚举序列长度
                for (int k = 0; k <= j && k <= cnt; ++k) // 枚举第i种字母选了k个
                    f[i][j] += f[i - 1][j - k] * c[j][k];
                
            ++i;
        }
        int ans = 0;
        for (int j = 1; j <= n; ++j) ans += f[m][j];
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n^2)$ ，其中 $n$ 为 $tiles$ 的长度。虽然写了个三重循环，但换个角度，对于一个固定的 $j$ ，最内层的循环次数之和，约为所有字母的出现次数之和，即 $O(n)$ 。相当于最外层和最内层合起来是一个 $O(n)$ 的循环。所以三重循环的时间复杂度为 $O(n^2)$ 。
- 空间复杂度：$O(n^2)$ 。忽略预处理组合数的时间和空间。
- 注：也可以只预处理阶乘，用公式计算组合数。

#### 3. 空间优化
由于 $f[i]$ 只从 $f[i−1]$ 转移过来，我们可以去掉第一个维度，只用一个一维数组。

和0-1背包问题一样，如果 $j$ 从小到大遍历，那么 $f[i−1][j]$ 保存的数据会被 $f[i][j]$ 覆盖，但是计算右边的 $f[i][j']$ 时，又需要 $f[i−1][j]$ 。倒序遍历 $j$ 即可解决此问题。

此外，可以累加 $cnt$ ，**记作 $n$ ，作为第二层循环的初始值**，因为就算全部都选，前 $i$ 种字母的长度之和也不会超过 $n$ ，计算比 $n$ 更大的状态是没有意义的。
```java
class Solution {
    private static final int MX = 8;
    private static final int[][] c = new int[MX][MX];
    static {
        for (int i = 0; i < MX; ++i) {
            c[i][0] = c[i][i] = 1;
            for (int j = 1; j < i; ++j)
                c[i][j] = c[i - 1][j - 1] + c[i - 1][j]; // 预处理组合数
        }
    } 
    public int numTilePossibilities(String tiles) {
        // 改成 int[26] 统计可能会快一点点，感兴趣可以试试（下面 DP 跳过 cnt=0 的情况）
        var counts = new HashMap<Character, Integer>(); // 统计每个字母的出现次数
        for (var c : tiles.toCharArray())
            counts.merge(c, 1, Integer::sum); // counts[c]++
        var f = new int[tiles.length() + 1];
        f[0] = 1; // 构造空序列的方案数
        int n = 0;
        for (var cnt : counts.values()) { // 枚举第i种字母
            n += cnt; // 常数优化：相比从 tiles.length() 开始要更快
            for (int j = n; j > 0; --j)  // 枚举序列长度j
                // 枚举第i种字母选了k个，注意k=0时的方案数已经在f[j]中了
                for (int k = 1; k <= j && k <= cnt; ++k)
                    f[j] += f[j - k] * c[j][k];
        }
        int ans = 0;
        for (int j = 1; j <= n; ++j) ans += f[j];
        return ans;
    }
}
``` 
复杂度分析：
- 时间复杂度：$O(n^2)$ ，其中 $n$ 为 $tiles$ 的长度。虽然写了个三重循环，但换个角度，对于一个固定的 $j$ ，最内层的循环次数之和，约为所有字母的出现次数之和，即 $O(n)$ 。相当于最外层和最内层合起来是一个 $O(n)$ 的循环。所以三重循环的时间复杂度为 $O(n^2)$ 。
- 空间复杂度：$O(n)$ 。忽略预处理组合数的时间和空间。