> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个由小写英文字母（`'a'` 到 `'z'`）组成的字符串 `s`。你的任务是找出出现频率 **最高** 的元音（`'a'`、`'e'`、`'i'`、`'o'`、`'u'` 中的一个）和出现频率**最高**的辅音（除元音以外的所有字母），并返回这两个频率之和。

**注意**：如果有多个元音或辅音具有相同的最高频率，可以任选其中一个。如果字符串中没有元音或没有辅音，则其频率视为 0。

一个字母 `x` 的 **频率** 是它在字符串中出现的次数。

**示例 1：**
```c
输入: s = "successes"
输出: 6
解释:
- 元音有：'u' 出现 1 次，'e' 出现 2 次。最大元音频率 = 2。
- 辅音有：'s' 出现 4 次，'c' 出现 2 次。最大辅音频率 = 4。
- 输出为 2 + 4 = 6 。
```
**示例 2：**
```c
输入: s = "aeiaeia"
输出: 3
解释:
- 元音有：'a' 出现 3 次，'e' 出现 2 次，'i' 出现 2 次。最大元音频率 = 3。
- s 中没有辅音。因此，最大辅音频率 = 0。
- 输出为 3 + 0 = 3 。
```
**提示:**
- `1 <= s.length <= 100`
- `s` 只包含小写英文字母
---
### 方法一 统计字母出现次数
在遍历 $s$ 的过程中，用**一个**哈希表（或数组）统计每个字母的出现次数 $cnt$ 。
- 如果字母是元音，更新 $maxVowelCnt$ 的最大值。
- 如果字母是辅音，更新 $maxConsonantCnt$ 的最大值。

最终答案是 $maxVowelCnt + maxConsonantCnt$ 。
```java
class Solution {
    public int maxFreqSum(String s) {
        int[] cnt = new int[26];
        int maxVowelCnt = 0;
        int maxConsonantCnt = 0;
        for (char ch : s.toCharArray()) {
            cnt[ch - 'a']++;
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                maxVowelCnt = Math.max(maxVowelCnt, cnt[ch - 'a']);
            } else {
                maxConsonantCnt = Math.max(maxConsonantCnt, cnt[ch - 'a']);
            }
        }
        return maxVowelCnt + maxConsonantCnt;
    }
}
```
```cpp
class Solution {
public:
    int maxFreqSum(string s) {
        int cnt[26]{};
        int max_vowel_cnt = 0;
        int max_consonant_cnt = 0;
        for (char ch : s) {
            cnt[ch - 'a']++;
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                max_vowel_cnt = max(max_vowel_cnt, cnt[ch - 'a']);
            } else {
                max_consonant_cnt = max(max_consonant_cnt, cnt[ch - 'a']);
            }
        }
        return max_vowel_cnt + max_consonant_cnt;
    }
};
```
```rust
impl Solution {
    pub fn max_freq_sum(s: String) -> i32 {
        let mut cnt = [0; 26];
        let mut max_vowel_cnt = 0;
        let mut max_consonant_cnt = 0;
        for ch in s.bytes() {
            let idx = (ch as u8 - b'a') as usize;
            cnt[idx] += 1;
            if "aeiou".contains(ch as char) {
                max_vowel_cnt = max_vowel_cnt.max(cnt[idx]);
            } else {
                max_consonant_cnt = max_consonant_cnt.max(cnt[idx]);
            }
        }
        max_vowel_cnt + max_consonant_cnt
    }
}
```
```python
class Solution:
    def maxFreqSum(self, s: str) -> int:
        cnt = [0] * 26
        max_vowel_cnt = max_consonant_cnt = 0
        for ch in s:
            idx = ord(ch) - ord('a')
            cnt[idx] += 1
            if ch in "aeiou":
                max_vowel_cnt = max(max_vowel_cnt, cnt[idx])
            else:
                max_consonant_cnt = max(max_consonant_cnt, cnt[idx])
        return max_vowel_cnt + max_consonant_cnt
        
class Solution:
    def maxFreqSum(self, s: str) -> int:
        cnt = Counter(s)

        max_vowel_cnt = 0
        for ch in "aeiou":
            max_vowel_cnt = max(max_vowel_cnt, cnt[ch])
            del cnt[ch]  # 这样下面计算的一定是辅音出现次数的最大值

        max_consonant_cnt = max(cnt.values(), default=0)
        return max_vowel_cnt + max_consonant_cnt
```
```c
#define MAX(a, b) ((b) > (a) ? (b) : (a))

int maxFreqSum(char* s) {
    int cnt[26] = {};
    int max_vowel_cnt = 0;
    int max_consonant_cnt = 0;
    for (int i = 0; s[i]; i++) {
        char ch = s[i];
        cnt[ch - 'a']++;
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
            max_vowel_cnt = MAX(max_vowel_cnt, cnt[ch - 'a']);
        } else {
            max_consonant_cnt = MAX(max_consonant_cnt, cnt[ch - 'a']);
        }
    }
    return max_vowel_cnt + max_consonant_cnt;
}
```
```go
func maxFreqSum(s string) int {
	cnt := [26]int{}
	maxVowelCnt := 0
	maxConsonantCnt := 0
	for _, ch := range s {
		cnt[ch-'a']++
		if strings.ContainsRune("aeiou", ch) {
			maxVowelCnt = max(maxVowelCnt, cnt[ch-'a'])
		} else {
			maxConsonantCnt = max(maxConsonantCnt, cnt[ch-'a'])
		}
	}
	return maxVowelCnt + maxConsonantCnt
}
```
```js
var maxFreqSum = function(s) {
    const cnt = Array(26).fill(0);
    let maxVowelCnt = 0;
    let maxConsonantCnt = 0;
    for (const ch of s) {
        const idx = ch.charCodeAt(0) - 'a'.charCodeAt(0);
        cnt[idx]++;
        if ("aeiou".includes(ch)) {
            maxVowelCnt = Math.max(maxVowelCnt, cnt[idx]);
        } else {
            maxConsonantCnt = Math.max(maxConsonantCnt, cnt[idx]);
        }
    }
    return maxVowelCnt + maxConsonantCnt;
};
```
复杂度分析：
- 时间复杂度：$O(n)$ 或 $O(n + |\Sigma|)$ ，其中 $n$ 是 $s$ 的长度，$|\Sigma| = 26$ 是字符集合的大小。
- 空间复杂度：$O(|\Sigma|)$ 。
---
### 方法二 位运算优化
根据[从集合论到位运算](https://leetcode.cn/circle/discuss/CaOJ45/)，我们可以把元音集合 $\{a, e, i, o, u\}$ 视作数字 $2^0 + 2^4 + 2^{8} + 2^{14} + 2^{20} = 1065233$ ，即十六进制的 $\text{0x104111}$ ，就能用位运算快速判断字母是否在元音集合中。
```java
class Solution {
    public int maxFreqSum(String s) {
        final int VOWEL_MASK = 0x104111;
        int[] cnt = new int[26];
        int[] maxCnt = new int[2];
        for (char ch : s.toCharArray()) {
            ch -= 'a';
            int bit = VOWEL_MASK >> ch & 1;
            cnt[ch]++;
            maxCnt[bit] = Math.max(maxCnt[bit], cnt[ch]);
        }
        return maxCnt[0] + maxCnt[1];
    }
}
```
```cpp
class Solution {
public:
    int maxFreqSum(string s) {
        const int VOWEL_MASK = 0x104111;
        int cnt[26]{};
        int max_cnt[2]{};
        for (char ch : s) {
            ch -= 'a';
            int bit = VOWEL_MASK >> ch & 1;
            cnt[ch]++;
            max_cnt[bit] = max(max_cnt[bit], cnt[ch]);
        }
        return max_cnt[0] + max_cnt[1];
    }
};
```
```rust
impl Solution {
    pub fn max_freq_sum(s: String) -> i32 {
        const VOWEL_MASK: usize = 0x104111;
        let mut cnt = [0; 26];
        let mut max_cnt = [0; 2];
        for ch in s.bytes() {
            let idx = (ch as u8 - b'a') as usize;
            let bit = VOWEL_MASK >> idx & 1;
            cnt[idx] += 1;
            max_cnt[bit] = max_cnt[bit].max(cnt[idx]);
        }
        max_cnt[0] + max_cnt[1]
    }
}
```
```python
class Solution:
    def maxFreqSum(self, s: str) -> int:
        VOWEL_MASK = 0x104111
        cnt = [0] * 26
        max_cnt = [0] * 2
        for ch in s:
            ch = ord(ch) - ord('a')
            bit = VOWEL_MASK >> ch & 1
            cnt[ch] += 1
            max_cnt[bit] = max(max_cnt[bit], cnt[ch])
        return sum(max_cnt)
```
```c
#define VOWEL_MASK 0x104111
#define MAX(a, b) ((b) > (a) ? (b) : (a))

int maxFreqSum(char* s) {
    int cnt[26] = {};
    int max_cnt[2] = {};
    for (int i = 0; s[i]; i++) {
        int ch = s[i] - 'a';
        int bit = VOWEL_MASK >> ch & 1;
        cnt[ch]++;
        max_cnt[bit] = MAX(max_cnt[bit], cnt[ch]);
    }
    return max_cnt[0] + max_cnt[1];
}
```
```go
func maxFreqSum(s string) int {
	const vowelMask = 0x104111
	cnt := [26]int{}
	maxCnt := [2]int{}
	for _, ch := range s {
		ch -= 'a'
		bit := vowelMask >> ch & 1
		cnt[ch]++
		maxCnt[bit] = max(maxCnt[bit], cnt[ch])
	}
	return maxCnt[0] + maxCnt[1]
}
```
```js
var maxFreqSum = function(s) {
    const VOWEL_MASK = 0x104111;
    const cnt = Array(26).fill(0);
    const maxCnt = [0, 0];
    for (const ch of s) {
        const idx = ch.charCodeAt(0) - 'a'.charCodeAt(0);
        const bit = VOWEL_MASK >> idx & 1;
        cnt[idx]++;
        maxCnt[bit] = Math.max(maxCnt[bit], cnt[idx]);
    }
    return maxCnt[0] + maxCnt[1];
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是 $s$ 的长度。
- 空间复杂度：$O(|\Sigma|)$ 。