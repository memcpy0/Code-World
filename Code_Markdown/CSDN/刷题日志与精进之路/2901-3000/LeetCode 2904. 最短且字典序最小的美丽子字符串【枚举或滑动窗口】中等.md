> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个二进制字符串 `s` 和一个正整数 `k` 。

如果 `s` 的某个子字符串中 `1` 的个数恰好等于 `k` ，则称这个子字符串是一个 **美丽子字符串** 。

令 `len` 等于 **最短** 美丽子字符串的长度。

返回长度等于 `len` 且字典序 **最小** 的美丽子字符串。如果 `s` 中不含美丽子字符串，则返回一个 **空** 字符串。

对于相同长度的两个字符串 `a` 和 `b` ，如果在 `a` 和 `b` 出现不同的第一个位置上，`a` 中该位置上的字符严格大于 `b` 中的对应字符，则认为字符串 `a` 字典序 **大于** 字符串 `b` 。
- 例如，`"abcd"` 的字典序大于 `"abcc"` ，因为两个字符串出现不同的第一个位置对应第四个字符，而 `d` 大于 `c` 。

**示例 1：**
```js
输入：s = "100011001", k = 3
输出："11001"
解释：示例中共有 7 个美丽子字符串：
1. 子字符串 "_100011_001" 。
2. 子字符串 "_1000110_01" 。
3. 子字符串 "_100011001_" 。
4. 子字符串 "1_00011001_" 。
5. 子字符串 "10_0011001_" 。
6. 子字符串 "100_011001_" 。
7. 子字符串 "1000_11001_" 。
最短美丽子字符串的长度是 5 。
长度为 5 且字典序最小的美丽子字符串是子字符串 "11001" 。
```
**示例 2：**
```js
输入：s = "1011", k = 2
输出："11"
解释：示例中共有 3 个美丽子字符串：
1. 子字符串 "_101_1" 。
2. 子字符串 "1_011_" 。
3. 子字符串 "10_11_" 。
最短美丽子字符串的长度是 2 。
长度为 2 且字典序最小的美丽子字符串是子字符串 "11" 。 
```
**示例 3：**
```js
输入：s = "000", k = 1
输出：""
解释：示例中不存在美丽子字符串。
```
**提示：**
- `1 <= s.length <= 100`
- `1 <= k <= s.length`

---
### 解法1 枚举
首先，如果 $s$ 中 $1$ 的个数不足 $k$ ，直接返回空串。否则一定有解。

从 $k$ 开始枚举答案的长度 $\textit{size}$ ，然后**在 $s$ 中枚举所有长为 $\textit{size}$ 的子串，同时维护字典序最小的子串**。如果存在一个子串，其中 $1$ 的个数等于 $k$ ，则返回字典序最小的子串（**且其长度此时为最短**）。
```python
class Solution:
    def shortestBeautifulSubstring(self, s: str, k: int) -> str:
        # 1 的个数不足 k
        if s.count('1') < k:
            return ''
        # 否则一定有解
        for size in count(k):  # 从 k 开始枚举
            ans = ''
            for i in range(size, len(s) + 1):
                t = s[i - size: i]
                if (ans == '' or t < ans) and t.count('1') == k:
                    ans = t
            if ans: return ans
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n^3)$ ，其中 $n$ 为 $s$ 的长度。
- 空间复杂度：$\mathcal{O}(n)$ 或 $\mathcal{O}(1)$ 。字符串切片需要 $\mathcal{O}(n)$ 的空间，Go 除外。

---
### 解法2 滑动窗口
由于答案中恰好有 $k$ 个 $1$ ，**多于 $k$ 个 $1$ 的子串内有等于 $k$ 个 $1$ 的子串**，我们可以用滑动窗口找最短的答案。

如果窗口内的 $1$ 的个数超过 $k$ ，或者窗口左端点是 $0$ ，就可以缩小窗口。
> 注：利用字符串哈希（或者后缀数组等），可以把比较字典序的时间降至 $\mathcal{O}(n\log n)$ ，这样可以做到 $\mathcal{O}(n\log n)$ 的时间复杂度。

```cpp
class Solution {
public:
    string shortestBeautifulSubstring(string s, int k) {
        if (count(s.begin(), s.end(), '1') < k) {
            return "";
        }
        string ans = s;
        int cnt1 = 0, left = 0; // cnt1为窗口内1的个数
        for (int right = 0; right < s.length(); right++) {
            cnt1 += s[right] - '0';            
            // 当窗口中1的个数>k 或 窗口左端点为0，就缩小窗口
            while (cnt1 > k || s[left] == '0') {
                cnt1 -= s[left++] - '0';
            }
            if (cnt1 == k) {
                string t = s.substr(left, right - left + 1);
                if (t.length() < ans.length() || t.length() == ans.length() && t < ans) {
                    ans = move(t);
                }
            }
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n^2)$ ，其中 $n$ 为 $s$ 的长度。
- 空间复杂度：$\mathcal{O}(n)$ 或 $\mathcal{O}(1)$ 。字符串切片需要 $\mathcal{O}(n)$ 的空间，Go 除外。

如果比赛时把数据范围加大，还可这样写：
- 先遍历一次 $s$ ，把其中为 $1$ 的下标统计放在数组 $oneIndexArr$ 中，题目就变成「从这个下标数组中连续取 $k$ 个数字」，变成了定长滑窗。
- 好处是，判断字典序时，可以通过**比较 $[i, j + k - 1]$ 之间 $oneIndexArr[i + x]\ (i < x < i + k - 1)$ 的下标值**进行。因为最短字符串肯定以 $1$ 开头和结尾，原来的 $s$ 中又只有 $0, 1$ ，两个 $1$ 中间这部分谁的 $1$ 的下标更小，就说明谁的字典序更大。 

真 o(n)

后缀数组+滑动窗口

利用后缀数组的rank数组来实现o(1)比较两个子串的字典序大小关系

```ruby
from collections import defaultdict
#可以用于重复串问题，因为前缀相同的子串在sa里必定连续
class SuffixArray:
    def sa_naive(self, s):       #实现了最朴素的后缀数组构建算法，其时间复杂度为 O(n^2 log n)，适用于小规模字符串。
        n = len(s)
        sa = list(range(n))
        sa.sort(key=lambda x: s[x:])
        return sa

    def sa_doubling(self, s):    #实现了倍增算法，其时间复杂度为 O(n log n)，适用于中等规模字符串。
        n = len(s)
        sa = list(range(n))
        rnk = s
        t = [0] * n
        k = 1
        while k < n:
            sa.sort(key=lambda x: (rnk[x], rnk[x + k])
            if x + k < n else (rnk[x], -1))
            t[sa[0]] = 0
            for i in range(1, n):
                t[sa[i]] = t[sa[i - 1]]
                if sa[i - 1] + k < n:
                    x = (rnk[sa[i - 1]], rnk[sa[i - 1] + k])
                else:
                    x = (rnk[sa[i - 1]], -1)
                if sa[i] + k < n:
                    y = (rnk[sa[i]], rnk[sa[i] + k])
                else:
                    y = (rnk[sa[i]], -1)
                if x < y:
                    t[sa[i]] += 1
            k *= 2
            t, rnk = rnk, t
        return sa

    def sa_is(self, s, upper):
        #实现了 SA-IS 算法，其时间复杂度为 O(n)，适用于大规模字符串。
        #SA-IS 算法基于桶排思想，通过对字符串进行分类和排序，最终得到后缀数组。
        #代码中使用了类似于桶排的技巧，通过计算各种类型的后缀数量和前缀相同的子串，将问题转化为子问题，然后递归求解。
        n = len(s)
        if n == 0:
            return []
        if n == 1:
            return [0]
        if n == 2:
            if s[0] < s[1]:
                return [0, 1]
            else:
                return [1, 0]
        if n < 10:
            return self.sa_naive(s)
        if n < 50:
            return self.sa_doubling(s)
        ls = [0] * n
        for i in range(n - 2, -1, -1):
            ls[i] = ls[i + 1] if s[i] == s[i + 1] else s[i] < s[i + 1]
        sum_l = [0] * (upper + 1)
        sum_s = [0] * (upper + 1)
        for i in range(n):
            if ls[i]:
                sum_l[s[i] + 1] += 1
            else:
                sum_s[s[i]] += 1
        for i in range(upper):
            sum_s[i] += sum_l[i]
            if i < upper:
                sum_l[i + 1] += sum_s[i]
        lms_map = [-1] * (n + 1)
        m = 0
        for i in range(1, n):
            if not ls[i - 1] and ls[i]:
                lms_map[i] = m
                m += 1
        lms = []
        for i in range(1, n):
            if not ls[i - 1] and ls[i]:
                lms.append(i)
        sa = [-1] * n
        buf = sum_s.copy()
        for d in lms:
            if d == n:
                continue
            sa[buf[s[d]]] = d
            buf[s[d]] += 1
        buf = sum_l.copy()
        sa[buf[s[n - 1]]] = n - 1
        buf[s[n - 1]] += 1
        for i in range(n):
            v = sa[i]
            if v >= 1 and not ls[v - 1]:
                sa[buf[s[v - 1]]] = v - 1
                buf[s[v - 1]] += 1
        buf = sum_l.copy()
        for i in range(n - 1, -1, -1):
            v = sa[i]
            if v >= 1 and ls[v - 1]:
                buf[s[v - 1] + 1] -= 1
                sa[buf[s[v - 1] + 1]] = v - 1
        if m:
            sorted_lms = []
            for v in sa:
                if lms_map[v] != -1:
                    sorted_lms.append(v)
            rec_s = [0] * m
            rec_upper = 0
            rec_s[lms_map[sorted_lms[0]]] = 0
            for i in range(1, m):
                l = sorted_lms[i - 1]
                r = sorted_lms[i]
                end_l = lms[lms_map[l] + 1] if lms_map[l] + 1 < m else n
                end_r = lms[lms_map[r] + 1] if lms_map[r] + 1 < m else n
                same = True
                if end_l - l != end_r - r:
                    same = False
                else:
                    while l < end_l:
                        if s[l] != s[r]:
                            break
                        l += 1
                        r += 1
                    if l == n or s[l] != s[r]:
                        same = False
                if not same:
                    rec_upper += 1
                rec_s[lms_map[sorted_lms[i]]] = rec_upper
            rec_sa = self.sa_is(rec_s, rec_upper)
            for i in range(m):
                sorted_lms[i] = lms[rec_sa[i]]
            sa = [-1] * n
            buf = sum_s.copy()
            for d in sorted_lms:
                if d == n:
                    continue
                sa[buf[s[d]]] = d
                buf[s[d]] += 1
            buf = sum_l.copy()
            sa[buf[s[n - 1]]] = n - 1
            buf[s[n - 1]] += 1
            for i in range(n):
                v = sa[i]
                if v >= 1 and not ls[v - 1]:
                    sa[buf[s[v - 1]]] = v - 1
                    buf[s[v - 1]] += 1
            buf = sum_l.copy()
            for i in range(n - 1, -1, -1):
                v = sa[i]
                if v >= 1 and ls[v - 1]:
                    buf[s[v - 1] + 1] -= 1
                    sa[buf[s[v - 1] + 1]] = v - 1
        return sa
    def suffix_array(self, s, upper=255):
        if type(s) is str:
            s = [ord(c) for c in s]
        return self.sa_is(s, upper)
    def _count_sort(self, ls):
        c = defaultdict(list)
        for i, v in enumerate(ls):
            c[v].append(i)
        ans = []
        for v in sorted(list(c.keys())):
            for k in c[v]:
                ans.append((k, v))
        return ans
    def _rk(self, sa):
        rk = [0 for _ in sa]
        for i in range(len(sa)):
            rk[sa[i]] = i
        return rk

    def _height(self, s):
        sa, rk = self.sa, self.rk
        ht = [0] * len(sa)
        k = 0
        for sai in range(0, len(s)):
            if k:
                k -= 1
            while True:
                ai, bi = sai + k, sa[rk[sai] - 1] + k
                if not (0 <= ai < len(s) and 0 <= bi < len(s)):
                    break
                if max(ai, bi) >= len(s):
                    break
                elif s[ai] == s[bi]:
                    k += 1
                else:
                    break
            ht[rk[sai]] = k
        return ht
    def __init__(self, s):             #下方的i都从1开始，字典序
        self.sa = self.suffix_array(s) #表示排名第i小的后缀在原字符串中的起始位置为sa[i]
        self.rk = self._rk(self.sa)    #字符串中的每个位置i，其对应的后缀在后缀数组中的排名为rk[i]
        self.height = self._height(s)  #第i小的后缀与它前一名的后缀的最长公共前缀，其他地方也可能交lcp
        self.height[0]=0               #在字符串为单一字符构成时(长度为1也算),heigh[0]会出错成1，按照定义应该为0


class Solution:
    def shortestBeautifulSubstring(self, s: str, k: int) -> str:
        idx=[i for i,c in enumerate(s) if c=="1"]
        if len(idx)<k:
            return ""
        sa=SuffixArray(s)
        lens=start=101
        for i in range(len(idx)-k+1):
            if idx[i+k-1]-idx[i]+1<lens:
                lens=idx[i+k-1]-idx[i]+1
                start=idx[i]
            elif idx[i+k-1]-idx[i]+1==lens and sa.rk[idx[i]]<sa.rk[start]:
                start=idx[i]
        return s[start:start+lens]
```

---
### 解法3 前缀和+无序映射
```cpp
class Solution {
public:
    string shortestBeautifulSubstring(string s, int k) {
        int n = s.size();
        int sum = 0;
        unordered_map<int, int> rec;
        rec[0] = -1;
        int minLen = INT_MAX;
        int t = -1;
        for (int i = 0; i <= n; ++i) { // s[i-k+1,i]
            sum += (i < n ? s[i] == '1' : 0);
            if (rec.find(sum - k) != rec.end()) {
                int index = rec[sum - k];
                int len = i - index;
                if (len < minLen) {
                    minLen = len;
                    t = index + 1;
                } else if (len == minLen) {
                    if (s.compare(t, len, s, index + 1, len) == 1) {
                        t = index + 1;
                    }
                }
            }
            rec[sum] = i;
        }
        return t == -1 ? "" : s.substr(t, minLen);
    }
};
```