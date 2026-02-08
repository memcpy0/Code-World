> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个长度为 `n` 的字符串 `word` 和一个整数 `k` ，其中 `k` 是 `n` 的因数。

在一次操作中，你可以选择任意两个下标 `i` 和 `j`，其中 `0 <= i, j < n` ，且这两个下标都可以被 `k` 整除，然后用从 `j` 开始的长度为 `k` 的子串替换从 `i` 开始的长度为 `k` 的子串。也就是说，将子串 `word[i..i + k - 1]` 替换为子串 `word[j..j + k - 1]` 。

返回使 `word` 成为 **K 周期字符串** 所需的 **最少** 操作次数。

如果存在某个长度为 `k` 的字符串 `s`，使得 `word` 可以表示为任意次数连接 `s` ，则称字符串 `word` 是 **K 周期字符串** 。例如，如果 `word == "ababab"`，那么 `word` 就是 `s = "ab"` 时的 2 周期字符串 。

**示例 1：**
```c
输入：word = "leetcodeleet", k = 4
输出：1
解释：可以选择 i = 4 和 j = 0 获得一个 4 周期字符串。这次操作后，word 变为 "leetleetleet" 。
```
**示例 2：**
```c
输入：word = "leetcoleet", k = 2
输出：3
解释：可以执行以下操作获得一个 2 周期字符串。
```

|i|j|word|
|---|---|---|
|0|2|etetcoleet|
|4|0|etetetleet|
|6|0|etetetetet|
**提示：**
- `1 <= n == word.length <= 10^5`
- `1 <= k <= word.length`
- `k` 能整除 `word.length` 。
- `word` 仅由小写英文字母组成。

### 方法：统计子串个数
根据题意，我们只能选择首字母下标为 $0,k,2k,3k,⋯,n−k$ 的长为 $k$ 的子串来操作（替换）。

并且，$k$ 周期字符串意味着，所有首字母下标为 $0,k,2k,3k,⋯,n−k$ 的长为 $k$ 的子串**均相等**。

**为使操作次数尽量少，我们可以计算最多保留多少个子串不变**。也就是统计 $word$ 中的这些首字母下标为 $0,k,2k,3k,⋯,n−k$ 的长为 $k$ 的子串中，**出现次数最多的子串**的出现次数 $mx$ 。**用出现次数最多的子串，替换其余子串**。

所以用子串的个数 $\dfrac{n}{k}$  减去 $mx$ ，就是最少操作次数。
```python
class Solution:
    def minimumOperationsToMakeKPeriodic(self, word: str, k: int) -> int:
        n = len(word)
        cnt = Counter(word[i - k: i] for i in range(k, n + 1, k))
        mx = max(cnt.values())
        return n // k - mx
```
```java
class Solution {
    public int minimumOperationsToMakeKPeriodic(String word, int k) {
        int mx = 0;
        int n = word.length();
        HashMap<String, Integer> rec = new HashMap<>();
        for (int i = 0; i < n; i += k) {
            String s = word.substring(i, i + k);
            int c = rec.merge(s, 1, Integer::sum);
            mx = Math.max(c, mx);
        }
        return n / k - mx;
    }
}
```
```cpp
class Solution {
public:
    int minimumOperationsToMakeKPeriodic(string word, int k) {
        int n = word.length(), mx = 0;
        unordered_map<string, int> cnt;
        for (int i = k; i <= n; i += k) {
            mx = max(mx, ++cnt[word.substr(i - k, k)]);
        }
        return n / k - mx;
    }
};
```
```go
func minimumOperationsToMakeKPeriodic(word string, k int) int {
	n := len(word)
	cnt := map[string]int{}
	for i := k; i <= n; i += k {
		cnt[word[i-k:i]]++
	}
	mx := 0
	for _, c := range cnt {
		mx = max(mx, c)
	}
	return n/k - mx
}
```
```js
var minimumOperationsToMakeKPeriodic = function(word, k) {
    const n = word.length;
    const cnt = new Map();
    for (let i = k; i <= n; i += k) {
        const sub = word.slice(i - k, i);
        cnt.set(sub, (cnt.get(sub) ?? 0) + 1);
    }
    const mx = Math.max(...Array.from(cnt.values()));
    return Math.floor(n / k) - mx;
};
```
```rust
use std::collections::HashMap;

impl Solution {
    pub fn minimum_operations_to_make_k_periodic(word: String, k: i32) -> i32 {
        let n = word.len();
        let k = k as usize;
        let mut cnt = HashMap::new();
        for i in (k..=n).step_by(k) {
            *cnt.entry(&word[i - k..i]).or_insert(0) += 1;
        }
        let mx = *cnt.values().max().unwrap();
        (n / k) as i32 - mx
    }
}
```
**复杂度分析**
- 时间复杂度：$O(n)$ ，其中 $n$ 为 $word$ 的长度。
- 空间复杂度：$O(n)$ 。