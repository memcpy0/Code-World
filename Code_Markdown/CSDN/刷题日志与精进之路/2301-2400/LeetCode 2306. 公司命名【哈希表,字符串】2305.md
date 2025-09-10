> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个字符串数组 `ideas` 表示在公司命名过程中使用的名字列表。公司命名流程如下：
1. 从 `ideas` 中选择 2 个 **不同** 名字，称为 `ideaA` 和 `ideaB` 。
2. 交换 `ideaA` 和 `ideaB` 的首字母。
3. 如果得到的两个新名字 **都** 不在 `ideas` 中，那么 `ideaA ideaB`（**串联** `ideaA` 和 `ideaB` ，中间用一个空格分隔）是一个有效的公司名字。
4. 否则，不是一个有效的名字。

返回 **不同** 且有效的公司名字的数目。

**示例 1：**
```c
输入：ideas = ["coffee","donuts","time","toffee"]
输出：6
解释：下面列出一些有效的选择方案：
- ("coffee", "donuts")：对应的公司名字是 "doffee conuts" 。
- ("donuts", "coffee")：对应的公司名字是 "conuts doffee" 。
- ("donuts", "time")：对应的公司名字是 "tonuts dime" 。
- ("donuts", "toffee")：对应的公司名字是 "tonuts doffee" 。
- ("time", "donuts")：对应的公司名字是 "dime tonuts" 。
- ("toffee", "donuts")：对应的公司名字是 "doffee tonuts" 。
因此，总共有 6 个不同的公司名字。

下面列出一些无效的选择方案：
- ("coffee", "time")：在原数组中存在交换后形成的名字 "toffee" 。
- ("time", "toffee")：在原数组中存在交换后形成的两个名字。
- ("coffee", "toffee")：在原数组中存在交换后形成的两个名字。
```
**示例 2：**
```c
输入：ideas = ["lack","back"]
输出：0
解释：不存在有效的选择方案。因此，返回 0 。
```
**提示：**
- `2 <= ideas.length <= 5 * 10^4`
- `1 <= ideas[i].length <= 10`
- `ideas[i]` 由小写英文字母组成
- `ideas` 中的所有字符串 **互不相同**

---
## 分析
什么样的一对字符串无法交换首字母？

示例 1 中的 $coffee$ 和 $time$ ，虽然这样两个字符串完全不一样，但如果交换了 $coffee$ 和 $time$ 的首字母，会得到字符串 $toffee$ ，它在数组 $ideas$ 中，不符合题目要求。

又例如 $ideas=[aa,ab,ac,bc,bd,be]$ ，将其分成两组：
- 第一组：$aa,ab,ac$ 。
- 第二组：$bc,bd,be$ 。

其中**每一组内的字符串是不能交换首字母的**，因为交换后字符串不变，必然在 $ideas$ 中。

考虑交换第一组的字符串和第二组的字符串，哪些是可以交换首字母的，哪些是不能交换首字母的？
- 第一组的 $ac$ 无法和第二组的任何字符串交换，因为交换后会得到 $bc$ ，它在 $ideas$ 中。
- 第二组的 $bc$ 无法和第一组的任何字符串交换，因为交换后会得到 $ac$ ，它在 $ideas$ 中。
- 其余字符串对可以交换首字母。

上面的分析立刻引出了如下方法。

### 方法一：按照首字母分组，存储后缀
按照首字母，把 $ideas$ 分成（至多）$26$ 组字符串。

例如 $ideas=[aa,ab,ac,bc,bd,be]$ 分成如下两组（**只记录去掉首字母后的字符串**）：
- $A$ 组（集合）：${a,b,c}$ 。
- $B$ 组（集合）：${c,d,e}$ 。

分组后：
- 从 $A$ 中选一个不等于 $c$ 的字符串，这有 $2$ 种选法。
- 从 $B$ 中选一个不等于 $c$ 的字符串，这有 $2$ 种选法。

**考虑两个字符串的先后顺序**（谁在左谁在右），有 $2$ 种方法。根据**乘法原理**，有 $2×2×2=8$ 对符合要求的字符串。

由于无法选交集中的字符串，一般地，从 $A$ 和 $B$ 中可以选出
$$2⋅(∣A∣−∣A∩B∣)⋅(∣B∣−∣A∩B∣)$$
对符合要求的字符串。其中 $∣S∣$ 表示集合 $S$ 的大小。

枚举所有组对，计算上式，累加到答案中。
```python
class Solution:
    def distinctNames(self, ideas: List[str]) -> int:
        groups = defaultdict(set)
        for s in ideas:
            groups[s[0]].add(s[1:])  # 按照首字母分组

        ans = 0
        for a, b in combinations(groups.values(), 2):  # 枚举所有组对
            m = len(a & b)  # 交集的大小
            ans += (len(a) - m) * (len(b) - m)
        return ans * 2  # 乘 2 放到最后
```
```rust
use std::collections::HashSet;

impl Solution {
    pub fn distinct_names(ideas: Vec<String>) -> i64 {
        let mut groups = vec![HashSet::new(); 26];
        for s in ideas { // 按首字母分组，保存后缀
            groups[(s.as_bytes()[0] - b'a') as usize].insert(s[1..].to_string());
        }
        let mut ans = 0i64;
        for a in 1..26 { // 枚举所有组对
            for b in 0..a {
                let m = groups[a].iter().filter(|&s| groups[b].contains(s)).count();
                ans += (groups[a].len() - m) as i64 * (groups[b].len() - m) as i64;
            }
        }
        ans * 2 // 乘2放到最后
    }
}
```
```cpp
class Solution {
public:
    long long distinctNames(vector<string>& ideas) {
        unordered_set<string> groups[26];
        for (auto &s : ideas) {
            groups[s[0] - 'a'].insert(s.substr(1)); // 按首字母分组，保存后缀
        }
        long long ans = 0;
        for (int a = 1; a < 26; ++a) { // 枚举所有组对
            for (int b = 0; b < a; ++b) {
                int m = 0; // 交集大小
                for (auto &s : groups[a]) m += groups[b].count(s);
                ans += (long long) (groups[a].size() - m) * (groups[b].size() - m);
            }
        }
        return ans * 2; // 乘2放到最后
    }
};
```
```java
class Solution {
    public long distinctNames(String[] ideas) {
        Set<String>[] groups = new HashSet[26];
        Arrays.setAll(groups, i -> new HashSet<>());
        for (String s : ideas) {
            groups[s.charAt(0) - 'a'].add(s.substring(1)); // 按照首字母分组
        }

        long ans = 0;
        for (int a = 1; a < 26; a++) { // 枚举所有组对
            for (int b = 0; b < a; b++) {
                int m = 0; // 交集的大小
                for (String s : groups[a]) {
                    if (groups[b].contains(s)) {
                        m++;
                    }
                }
                ans += (long) (groups[a].size() - m) * (groups[b].size() - m);
            }
        }
        return ans * 2; // 乘 2 放到最后
    }
}
```
```go
func distinctNames(ideas []string) (ans int64) {
    group := [26]map[string]bool{}
    for i := range group {
        group[i] = map[string]bool{}
    }
    for _, s := range ideas {
        group[s[0]-'a'][s[1:]] = true // 按照首字母分组
    }

    for i, a := range group { // 枚举所有组对
        for _, b := range group[:i] {
            m := 0 // 交集的大小
            for s := range a {
                if b[s] {
                    m++
                }
            }
            ans += int64(len(a)-m) * int64(len(b)-m)
        }
    }
    return ans * 2 // 乘 2 放到最后
}
```
```js
var distinctNames = function(ideas) {
    const groups = Array.from({ length: 26 }, () => new Set());
    for (const s of ideas) {
        groups[s.charCodeAt(0) - 'a'.charCodeAt(0)].add(s.slice(1)); // 按照首字母分组
    }

    let ans = 0;
    for (let a = 1; a < 26; a++) { // 枚举所有组对
        for (let b = 0; b < a; b++) {
            let m = 0; // 交集的大小
            for (const s of groups[a]) {
                if (groups[b].has(s)) {
                    m++;
                }
            }
            ans += (groups[a].size - m) * (groups[b].size - m);
        }
    }
    return ans * 2; // 乘 2 放到最后
};
```
**复杂度分析**：
- 时间复杂度：$O(nm|Σ|)$ ，其中 $n$ 为 $ideas$ 的长度，$m≤10$ 为单个字符串的长度，$|Σ|=26$ 是字符集合的大小。注意枚举组对的逻辑看上去是 $O(nm |Σ|^2)$ 的，但去掉第二层 $O(|Σ|)$ 的循环后，剩余循环相当于把 $ideas$ 遍历了一遍，是 $O(nm)$ ，所以总的时间复杂度是 $O(nm |Σ|)$ 。
- 空间复杂度：$O(nm+ |Σ|)$ 。

### 方法二：按照后缀分组，计算交集
公式还是同样的：
$$2⋅(∣A∣−∣A∩B∣)⋅(∣B∣−∣A∩B∣)$$
下文把去掉首字母后的剩余部分称作**后缀**。横看成岭侧成峰，换一个角度计算交集大小 $∣A∩B∣$ 。

在遍历 $ideas=[aa,ab,ac,bc,bd,be]$ 的过程中，当我们遍历到 $bc$ 时，发现之前遍历过一个后缀也为 $c$ 的字符串 $ac$ ，这就对交集大小 $∣A∩B∣$ 产生了 $1$ 的贡献，也就是交集大小 $∣A∩B∣$ 增加 $1$ 。

具体来说，在遍历 $ideas$ 的过程中，维护如下信息：
1. （所有首字母开头的单词的）集合大小 $size[a]$ 。遍历到 $s=ideas[i]$ 时，把 $size[s[0]]$ 加一。
2. 交集大小 $intersection[a][b]$ 。遍历到 $s=ideas[i]$ 时，设 $b=s[0]$ ，把 $intersection[a][b]$ 和 $intersection[b][a]$ 加一，其中 $a$ 是和 $s$ **同后缀**的**其他字符串的首字母**。
3. 为了快速**知道前面有哪些字符串和 $s$ 有着相同的后缀**，用一个哈希表 $groups$ 维护，$key$ 是后缀，$value$ 是后缀对应的首字母列表。注意题目保证所有字符串互不相同。

代码实现时，可以用把**哈希表中维护的首字母列表**压缩成一个二进制数。
```python
class Solution:
    def distinctNames(self, ideas: List[str]) -> int:
        size = [0] * 26 # 各个集合的大小
        intersections = [[0] * 26 for _ in range(26)] # 交集大小
        groups = defaultdict(list) # 后缀 -> 首字母列表
        for s in ideas:
            b = ord(s[0]) - ord('a')
            size[b] += 1 # 增加集合大小
            g = groups[s[1:]]
            for a in g: # a是和s有着相同后缀的字符串的首字母
                intersections[a][b] += 1 # 增加交集大小
                intersections[b][a] += 1
            g.append(b)
        
        ans = 0
        for a in range(1, 26): # 枚举所有组对
            for b in range(a):
                m = intersections[a][b]
                ans += (size[a] - m) * (size[b] - m)
        return ans * 2 # 乘2放到最后
```
```rust
use std::collections::HashMap;

impl Solution {
    pub fn distinct_names(ideas: Vec<String>) -> i64 {
        let mut size = vec![0; 26]; // 集合大小
        let mut intersection = vec![vec![0; 26]; 26]; // 交集大小
        let mut groups = HashMap::new(); // 后缀 -> 首字母列表
        for s in ideas {
            let b = (s.as_bytes()[0] - b'a') as usize;
            size[b] += 1; // 增加集合大小
            let suffix = &s[1..];
            let mask = *groups.get(suffix).unwrap_or(&0);
            groups.insert(suffix.to_string(), mask | 1 << b); // 把b加到mask中
            for a in 0..26 { // a 是和 s 有着相同后缀的首字母
                if (mask >> a & 1) > 0 { // a 在 mask 中
                    intersection[b][a] += 1; // 增加交集大小
                    intersection[a][b] += 1; // 增加交集大小
                }
            }
        }

        let mut ans = 0i64;
        for a in 1..26 { // 枚举所有组对
            for b in 0..a {
                let m = intersection[a][b];
                ans += (size[a] - m) as i64 * (size[b] - m) as i64;
            }
        }
        ans * 2 // 乘 2 放到最后
    }
}
```
```javascript
/**
 * @param {string[]} ideas
 * @return {number}
 */
var distinctNames = function(ideas) {
    const size = Array(26).fill(0); // 集合大小
    const intersection = Array.from({ length: 26 }, () => Array(26).fill(0)); // 交集大小
    const groups = {}; // 后缀 -> 首字母
    for (let s of ideas) {
        const b = s.charCodeAt(0) - 'a'.charCodeAt(0);
        size[b]++; // 增加集合大小
        const suffix = s.slice(1); // 后缀
        const mask = groups[suffix] ?? 0;
        groups[suffix] = mask | 1 << b; // 把 b 加到 mask 中
        for (let a = 0; a < 26; a++) { // a 是和 s 有着相同后缀的字符串的首字母
            if (mask >> a & 1) { // a 在 mask 中
                intersection[b][a]++; // 增加交集大小
                intersection[a][b]++;
            }
        }
    }

    let ans = 0;
    for (let a = 1; a < 26; a++) { // 枚举所有组对
        for (let b = 0; b < a; b++) {
            const m = intersection[a][b];
            ans += (size[a] - m) * (size[b] - m);
        }
    }
    return ans * 2; // 乘 2 放到最后
};
```
```go
func distinctNames(ideas []string) (ans int64) {
    size := [26]int{} // 集合大小
    intersection := [26][26]int{} // 交集大小
    groups := map[string]int{} // 后缀 -> 首字母
    for _, s := range ideas {
        b := s[0] - 'a'
        size[b]++ // 增加集合大小
        suffix := s[1:]
        mask := groups[suffix]
        groups[suffix] = mask | 1<<b // 把 b 加到 mask 中
        for a := 0; a < 26; a++ { // a 是和 s 有着相同后缀的字符串的首字母
            if mask>>a&1 > 0 { // a 在 mask 中
                intersection[b][a]++ // 增加交集大小
                intersection[a][b]++
            }
        }
    }

    for a := 1; a < 26; a++ { // 枚举所有组对
        for b := 0; b < a; b++ {
            m := intersection[a][b]
            ans += int64(size[a]-m) * int64(size[b]-m)
        }
    }
    return ans * 2 // 乘 2 放到最后
}
```
```cpp
class Solution {
public:
    long long distinctNames(vector<string>& ideas) {
        int size[26]{}; // 集合大小
        int intersection[26][26]{}; // 交集大小
        unordered_map<string, int> groups; // 后缀 -> 首字母
        for (auto &s : ideas) {
            int b = s[0] - 'a';
            size[b]++; // 增加集合大小
            auto suffix = s.substr(1);
            int mask = groups[suffix];
            groups[suffix] = mask | 1 << b; // 把 b 加到 mask 中
            for (int a = 0; a < 26; ++a) { // a 是和 s 有着相同后缀的字符串的首字母
                if (mask >> a & 1) { // a 在 mask 中
                    intersection[b][a]++; // 增加交集大小
                    intersection[a][b]++;
                }
            }
        }

        long long ans = 0;
        for (int a = 1; a < 26; a++) { // 枚举所有组对
            for (int b = 0; b < a; b++) {
                int m = intersection[a][b];
                ans += (long long) (size[a] - m) * (size[b] - m);
            }
        }
        return ans * 2; // 乘 2 放到最后
    }
};
```
```java
class Solution {
    public long distinctNames(String[] ideas) {
        int[] size = new int[26]; // 集合大小
        int[][] intersection = new int[26][26]; // 交集大小
        Map<String, Integer> groups = new HashMap<>(); // 后缀 -> 首字母
        for (String s : ideas) {
            int b = s.charAt(0) - 'a';
            size[b]++; // 增加集合大小
            String suffix = s.substring(1);
            int mask = groups.getOrDefault(suffix, 0);
            groups.put(suffix, mask | 1 << b); // 把 b 加到 mask 中
            for (int a = 0; a < 26; a++) { // a 是和 s 有着相同后缀的字符串的首字母
                if ((mask >> a & 1) > 0) { // a 在 mask 中
                    intersection[b][a]++; // 增加交集大小
                    intersection[a][b]++;
                }
            }
        }

        long ans = 0;
        for (int a = 1; a < 26; a++) { // 枚举所有组对
            for (int b = 0; b < a; b++) {
                int m = intersection[a][b];
                ans += (long) (size[a] - m) * (size[b] - m);
            }
        }
        return ans * 2; // 乘 2 放到最后
    }
}
```
复杂度分析：
- 时间复杂度：$O(n(m+ |Σ|)+ |Σ|^2)$ ，其中 $n$ 为 $ideas$ 的长度，$m≤10$ 为单个字符串的长度，$|Σ|=26$ 是字符集合的大小。
- 空间复杂度：$O(nm+|Σ|^2)$ 。
