> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的字符串数组 `garbage` ，其中 `garbage[i]` 表示第 `i` 个房子的垃圾集合。`garbage[i]` 只包含字符 `'M'` ，`'P'` 和 `'G'` ，但可能包含多个相同字符，每个字符分别表示一单位的金属、纸和玻璃。垃圾车收拾 **一** 单位的任何一种垃圾都需要花费 `1` 分钟。

同时给你一个下标从 **0** 开始的整数数组 `travel` ，其中 `travel[i]` 是垃圾车从房子 `i` 行驶到房子 `i + 1` 需要的分钟数。

城市里总共有三辆垃圾车，分别收拾三种垃圾。每辆垃圾车都从房子 `0` 出发，**按顺序** 到达每一栋房子。但它们 **不是必须** 到达所有的房子。

任何时刻只有 **一辆** 垃圾车处在使用状态。当一辆垃圾车在行驶或者收拾垃圾的时候，另外两辆车 **不能** 做任何事情。

请你返回收拾完所有垃圾需要花费的 **最少** 总分钟数。

**示例 1：**
```java
输入：garbage = ["G","P","GP","GG"], travel = [2,4,3]
输出：21
解释：
收拾纸的垃圾车：
1. 从房子 0 行驶到房子 1
2. 收拾房子 1 的纸垃圾
3. 从房子 1 行驶到房子 2
4. 收拾房子 2 的纸垃圾
收拾纸的垃圾车总共花费 8 分钟收拾完所有的纸垃圾。
收拾玻璃的垃圾车：
1. 收拾房子 0 的玻璃垃圾
2. 从房子 0 行驶到房子 1
3. 从房子 1 行驶到房子 2
4. 收拾房子 2 的玻璃垃圾
5. 从房子 2 行驶到房子 3
6. 收拾房子 3 的玻璃垃圾
收拾玻璃的垃圾车总共花费 13 分钟收拾完所有的玻璃垃圾。
由于没有金属垃圾，收拾金属的垃圾车不需要花费任何时间。
所以总共花费 8 + 13 = 21 分钟收拾完所有垃圾。
```
**示例 2：**
```java
输入：garbage = ["MMM","PGM","GP"], travel = [3,10]
输出：37
解释：
收拾金属的垃圾车花费 7 分钟收拾完所有的金属垃圾。
收拾纸的垃圾车花费 15 分钟收拾完所有的纸垃圾。
收拾玻璃的垃圾车花费 15 分钟收拾完所有的玻璃垃圾。
总共花费 7 + 15 + 15 = 37 分钟收拾完所有的垃圾。
```
**提示：**
- `2 <= garbage.length <= 10^5`
- `garbage[i]` 只包含字母 `'M'` ，`'P'` 和 `'G'` 。
- `1 <= garbage[i].length <= 10`
- `travel.length == garbage.length - 1`
- `1 <= travel[i] <= 100`

---
### 解法 多次遍历/正序一次遍历/倒序一次遍历
#### 总体思路
由于「任何时刻只有一辆垃圾车处在使用状态」，我们可以先让收集 $M$（金属）的垃圾车从左到右跑一趟，然后让收集 $P$（纸）的垃圾车从左到右跑一趟，最后让收集 $G$（玻璃）的垃圾车从左到右跑一趟。

总用时可以拆分成两个部分：
- 收集垃圾的总用时，这等于所有 $\textit{garbage}[i]$ 的长度之和。
- 行驶的总用时，这等于**每辆垃圾车的行驶用时之和**。对于收集 $M$ 的垃圾车，设 $\textit{garbage}[i]$ 是最后一个包含 $M$ 的字符串，那么收集 $M$ 的垃圾车的行驶用时为 $\textit{travel}[0]+\textit{travel}[1]+\cdots + \textit{travel}[i-1]$ 。同理可得另外两辆垃圾车的行驶用时。

下面介绍三种实现方法：
- 第一种方法针对本题数据，看上去会多次遍历数组，但实际跑的飞快。理由见复杂度分析。
- 另外两种方法适用性更广，即使有 $10^5$ 种垃圾也可以通过。

#### 方法一：多次遍历
先让所有垃圾车都跑满全程，再倒着遍历 $\textit{garbage}$ ，减去多跑的时间。

对于收集 $M$ 的垃圾车，设 $\textit{garbage}[i]$ 是最后一个包含 $M$ 的字符串，那么收集 $M$ 的垃圾车的行驶用时，等于跑满全程的用时，减去多跑的用时 $\textit{travel}[i]+\textit{travel}[i+1]+\cdots + \textit{travel}[n-2]$ ，其中 $n$ 是 $\textit{garbage}$ 的长度。注意 $\textit{travel}$ 的下标和 $\textit{garbage}$ 的下标相差 $1$ 。
```python
class Solution:
    def garbageCollection(self, garbage: List[str], travel: List[int]) -> int:
        ans = sum(map(len, garbage)) + sum(travel) * 3
        for c in "MPG":
            for g, t in zip(reversed(garbage), reversed(travel)):
                if c in g:
                    break
                ans -= t # 没有垃圾c, 多跑了
        return ans
```

```java
class Solution {
    public int garbageCollection(String[] garbage, int[] travel) {
        int ans = 0;
        for (String g : garbage) ans += g.length();
        for (int t : travel) ans += t * 3;
        for (char c : new char[]{'M', 'P', 'G'}) {
            for (int i = garbage.length - 1; i > 0 && garbage[i].indexOf(c) < 0; --i) {
                ans -= travel[i - 1]; // 没有垃圾c, 多跑了
            }
        }
        return ans;
    }
}
```

```cpp
class Solution {
public:
    int garbageCollection(vector<string>& garbage, vector<int>& travel) {
        int ans = reduce(travel.begin(), travel.end()) * 3;
        for (auto& g : garbage) ans += g.length();
        for (char c : {'M', 'P', 'G'}) {
            for (int i = garbage.size() - 1; i && garbage[i].find(c) == string::npos; --i) {
                ans -= travel[i - 1]; // 没有垃圾c, 多跑了
            }
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n+L)$ ，其中 $n$ 是 $\textit{garbage}$ 的长度，$L$ 是所有 $\textit{garbage}[i]$ 的长度之和。然而，考虑到测试数据包含大量随机生成的数据，在倒序遍历中，循环期望 $\mathcal{O}(1)$ 次就遇到字母 MPG 了，早早地退出循环，所以在随机数据下的时间复杂度为 $\mathcal{O}(n)$ 。换句话说，只有极少的字符串是需要遍历的，绝大多数字符串只需获取其长度，无需遍历其具体内容。
- 空间复杂度：$\mathcal{O}(1)$ 。

#### 方法二：正序一次遍历
我们可以在遍历 $garbage$ 的同时，计算从房子 $0$ 到房子 $i$ 的用时 $sumT$ ，以及一个哈希表（或者数组）$\textit{tMap}$ 记录每辆车目前的行驶用时。例如 $\textit{garbage}[i]=\texttt{GP}$ ，那么收集 G 和 P 的垃圾车的行驶用时更新为 $sumT$ ，收集 M 的垃圾车的行驶用时不变。循环结束后，$\textit{tMap}$ 中保存的就是每辆垃圾车各自的行驶用时了。

最后答案为所有 $\textit{garbage}[i]$ 的长度之和，加上 $\textit{tMap}$ 中保存的行驶用时之和。
```python
# 写法一
class Solution:
    def garbageCollection(self, garbage: List[str], travel: List[int]) -> int:
        ans = sum_t = 0
        t_map = {}
        for g, t in zip(garbage, [0] + travel):
            ans += len(g)
            sum_t += t
            for c in g:
                t_map[c] = sum_t
        return ans + sum(t_map.values())
# 写法二
class Solution:
    def garbageCollection(self, garbage: List[str], travel: List[int]) -> int:
        ans = sum_t = 0
        t_map = {}
        # 避免 [0] + travel 产生额外空间
        for i, g in enumerate(garbage):
            ans += len(g)
            sum_t += travel[i - 1] if i else 0
            for c in g:
                t_map[c] = sum_t
        return ans + sum(t_map.values())
```
```java
class Solution {
    public int garbageCollection(String[] garbage, int[] travel) {
        int ans = garbage[0].length();
        int sumT = 0;
        int[] tMap = new int[4]; // 如果垃圾种类更多，可以改成哈希表
        for (int i = 1; i < garbage.length; i++) {
            char[] s = garbage[i].toCharArray();
            ans += s.length;
            sumT += travel[i - 1];
            for (char c : s) {
                tMap[c & 3] = sumT; // MPG 的 ASCII 值的低两位互不相同
            }
        }
        for (int t : tMap) {
            ans += t;
        }
        return ans;
    }
}
```
```cpp
class Solution {
public:
    int garbageCollection(vector<string>& garbage, vector<int>& travel) {
        int ans = garbage[0].length();
        int sum_t = 0;
        int t_map[4]{}; // 如果垃圾种类更多，可以改成哈希表
        for (int i = 1; i < garbage.size(); i++) {
            auto& s = garbage[i];
            ans += s.length();
            sum_t += travel[i - 1];
            for (char c : s) {
                t_map[c & 3] = sum_t; // MPG 的 ASCII 值的低两位互不相同
            }
        }
        return ans + reduce(t_map, t_map + 4);
    }
};
```
进一步地，**在遍历 $\textit{garbage}$ 的过程中把行驶时间加入答案**，从而做到一次遍历。
```python
class Solution:
    def garbageCollection(self, garbage: List[str], travel: List[int]) -> int:
        ans = sum_t = 0
        t_map = defaultdict(int)
        for g, t in zip(garbage, [0] + travel):
            ans += len(g)
            sum_t += t
            for c in g:
                ans += sum_t - t_map[c]
                t_map[c] = sum_t
        return ans
```
```python
class Solution:
    def garbageCollection(self, garbage: List[str], travel: List[int]) -> int:
        ans = sum_t = 0
        t_map = defaultdict(int)
        for i, g in enumerate(garbage):
            ans += len(g)
            sum_t += travel[i - 1] if i else 0
            for c in g:
                ans += sum_t - t_map[c]
                t_map[c] = sum_t
        return ans
```
```java
class Solution {
    public int garbageCollection(String[] garbage, int[] travel) {
        int ans = garbage[0].length();
        int sumT = 0;
        int[] tMap = new int[4];
        for (int i = 1; i < garbage.length; i++) {
            char[] s = garbage[i].toCharArray();
            ans += s.length;
            sumT += travel[i - 1];
            for (char c : s) {
                ans += sumT - tMap[c & 3];
                tMap[c & 3] = sumT;
            }
        }
        return ans;
    }
}
```
```cpp
class Solution {
public:
    int garbageCollection(vector<string>& garbage, vector<int>& travel) {
        int ans = garbage[0].length();
        int sum_t = 0;
        int t_map[4]{};
        for (int i = 1; i < garbage.size(); i++) {
            auto& s = garbage[i];
            ans += s.length();
            sum_t += travel[i - 1];
            for (char c : s) {
                ans += sum_t - t_map[c & 3];
                t_map[c & 3] = sum_t;
            }
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n+L)$ ，其中 $n$ 是 $\textit{garbage}$ 的长度，$L$ 是所有 $\textit{garbage}[i]$ 的长度之和。
- 空间复杂度：$\mathcal{O}(k)$ ，其中 $k=3$ 表示垃圾种类数。

#### 方法三：倒序一次遍历（贡献法）
方法二需要用到哈希表（hash map），能否只用哈希集合（hash set）呢？
- 如果 $\textit{garbage}[n-1]$ 只包含 M，这意味着只有一辆车需要行驶 $\textit{travel}[n-2]$ 分钟，所以 $\textit{travel}[n-2]$ 对答案的贡献是 $\textit{travel}[n-2]\cdot 1$ 。
- 如果 $\textit{garbage}[n-2] + \textit{garbage}[n-1]$ 包含 $M$ 和 $P$ ，这意味着有两辆车需要行驶 $travel[n−3]$ 分钟，所以 $\textit{travel}[n-3]$ 对答案的贡献是 $travel[n−3]⋅2$ 。
- 依此类推，如果从 $\textit{garbage}[i]$ 到 $\textit{garbage}[n-1]$ 有 $k$ 种字母，那么 $\textit{travel}[i-1]$ 对答案的贡献是 $\textit{travel}[i-1]\cdot k$ 。

累加贡献即为行驶总用时。代码实现时，可以用哈希集合（或者布尔数组）记录遇到的字母。
```python
class Solution:
    def garbageCollection(self, garbage: List[str], travel: List[int]) -> int:
        ans = len(garbage[0])
        seen = set()
        for g, t in zip(reversed(garbage), reversed(travel)):
            seen.update(g)
            ans += len(g) + t * len(seen)
        return ans
```

```java
class Solution {
    public int garbageCollection(String[] garbage, int[] travel) {
        int ans = garbage[0].length();
        HashSet<Character> seen = new HashSet<>();
        for (int i = garbage.length - 1; i > 0; --i) {
            char[] g = garbage[i].toCharArray();
            for (char c : g) seen.add(c);
            ans += g.length + travel[i - 1] * seen.size();
        }
        return ans;
    }
}
```

```cpp
class Solution {
public:
    int garbageCollection(vector<string>& garbage, vector<int>& travel) {
        int ans = garbage[0].length();
        unordered_set<char> seen;
        for (int i = garbage.size() - 1; i; --i) {
            auto &g = garbage[i];
            seen.insert(g.begin(), g.end());
            ans += g.length() + travel[i - 1] * seen.size();
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n+L)$ ，其中 $n$ 是 $\textit{garbage}$ 的长度，$L$ 是所有 $\textit{garbage}[i]$ 的长度之和。
- 空间复杂度：$\mathcal{O}(k)$ ，其中 $k=3$ 表示垃圾种类数。

