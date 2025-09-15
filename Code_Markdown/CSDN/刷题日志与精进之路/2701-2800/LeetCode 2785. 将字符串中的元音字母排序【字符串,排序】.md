> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的字符串 `s` ，将 `s` 中的元素重新 **排列** 得到新的字符串 `t` ，它满足：
- 所有辅音字母都在原来的位置上。更正式的，如果满足 `0 <= i < s.length` 的下标 `i` 处的 `s[i]` 是个辅音字母，那么 `t[i] = s[i]` 。
- 元音字母都必须以他们的 **ASCII** 值按 **非递减** 顺序排列。更正式的，对于满足 `0 <= i < j < s.length` 的下标 `i` 和 `j`  ，如果 `s[i]` 和 `s[j]` 都是元音字母，那么 `t[i]` 的 ASCII 值不能大于 `t[j]` 的 ASCII 值。

请你返回结果字母串。

元音字母为 `'a'` ，`'e'` ，`'i'` ，`'o'` 和 `'u'` ，它们可能是小写字母也可能是大写字母，辅音字母是除了这 5 个字母以外的所有字母。

**示例 1：**
```c
输入：s = "lEetcOde"
输出："lEOtcede"
解释：'E' ，'O' 和 'e' 是 s 中的元音字母，'l' ，'t' ，'c' 和 'd' 是所有的辅音。将元音字母按照 ASCII 值排序，辅音字母留在原地。
```
**示例 2：**
```c
输入：s = "lYmpH"
输出："lYmpH"
解释：s 中没有元音字母（s 中都为辅音字母），所以我们返回 "lYmpH" 。
```
**提示：**
- `1 <= s.length <= 10^5`
- `s` 只包含英语字母表中的 **大写** 和 **小写** 字母。

---
### 方法一 排序+位运算优化
示例 1 的 $s=\text{lEetcOde}$ ，其中元音字母为 $\text{EeOe}$ ，排序后为 $\text{EOee}$ 。（大写字母排前面是因为大写字母的 ASCII 值更小）

原来的 $s=\text{lEetcOde}$ 视作 $\text{l\_\_tc\_d\_}$ ，包含 $4$ 个空位。在空位依次填入 $\text{EOee}$ ，得到答案 $\text{lEOtcede}$ 。
```java
class Solution {
    public String sortVowels(String S) {
        StringBuilder vowels = new StringBuilder();
        char[] s = S.toCharArray();
        for (char ch : s) {
            char c = Character.toLowerCase(ch);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowels.append(ch);
            }
        }

        char[] sortedVowels = vowels.toString().toCharArray();
        Arrays.sort(sortedVowels);

        int j = 0;
        for (int i = 0; i < s.length; i++) {
            char c = Character.toLowerCase(s[i]);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                s[i] = sortedVowels[j++];
            }
        }
        return new String(s);
    }
}
```
```cpp
class Solution {
public:
    string sortVowels(string s) {
        string vowels;
        for (char ch : s) {
            char c = tolower(ch);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowels += ch;
            }
        }
        ranges::sort(vowels); // 排序
        int j = 0;
        for (char& ch : s) {
            char c = tolower(ch);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                ch = vowels[j++];
            }
        }
        return s;
    }
};
```
```rust
impl Solution {
    pub fn sort_vowels(s: String) -> String {
        let mut vowels = s.bytes()
            .filter(|&c| "AEIOUaeiou".contains(c as char))
            .collect::<Vec<_>>();
        vowels.sort_unstable();
        let mut i : usize = 0;
        let mut s = s.into_bytes(); // Vec<u8>
        for c in s.iter_mut() {
            if "AEIOUaeiou".contains(*c as char) {
                *c = vowels[i];
                i += 1;
            }
        }
        String::from_utf8_lossy(&s).to_string()
        // unsafe { String::from_utf8_unchecked(s) }
    }
}
```
```python
class Solution:
    def sortVowels(self, s: str) -> str:
        vowels = sorted(ch for ch in s if ch in "AEIOUaeiou")
        t = list(s)  # str 无法修改，转成 list
        j = 0
        for i, ch in enumerate(t):
            if ch in "AEIOUaeiou":
                t[i] = vowels[j]  # 填空
                j += 1
        return ''.join(t)
```
```c
#define VOWEL_MASK 0x208222

int cmp(const void* a, const void* b) {
    return *(char*)a - *(char*)b;
}

char* sortVowels(char* s) {
    int n = strlen(s);
    char* vowels = malloc(n * sizeof(char));
    int k = 0;
    for (int i = 0; i < n; i++) {
        char c = tolower(s[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            vowels[k++] = s[i];
        }
    }

    qsort(vowels, k, sizeof(char), cmp);

    k = 0;
    for (int i = 0; i < n; i++) {
        char c = tolower(s[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            s[i] = vowels[k++];
        }
    }

    free(vowels);
    return s;
}
```
```go
func sortVowels(s string) string {
	vowels := []byte{}
	for _, ch := range s {
		c := unicode.ToLower(ch)
		if strings.ContainsRune("aeiou", c) {
			vowels = append(vowels, byte(ch))
		}
	}

	slices.Sort(vowels)

	t := []byte(s)
	j := 0
	for i, ch := range t {
		c := unicode.ToLower(rune(ch))
		if strings.ContainsRune("aeiou", c) {
			t[i] = vowels[j]
			j++
		}
	}
	return string(t)
}
```
```javascript
var sortVowels = function(s) {
    const vowels = [];
    for (const ch of s) {
        if ("AEIOUaeiou".includes(ch)) {
            vowels.push(ch);
        }
    }

    vowels.sort();

    let j = 0;
    const t = s.split('');
    for (let i = 0; i < t.length; i++) {
        if ("AEIOUaeiou".includes(t[i])) {
            t[i] = vowels[j++];
        }
    }
    return t.join('');
};
```
我们可以用位运算进行优化。查看ASCII表可知，$\text{A}$ 到 $\text{Z}$ 的ASCII值的二进制低 $5$ 位是 $1$ 到 $26$ ，$\text{a}$ 到 $\text{z}$ 的ASCII值的二进制低 $5$ 位也是 $1$ 到 $26$ 位。所以，可以用 `ch & 31` 把字母 `ch` 转成 $1$ 到 $26$ ，**这对大小写都是统一规则**。

且由于 `aeiou` 分别是第 $1, 5, 9, 15, 21$ 个字母，根据[从集合论到位运算](https://leetcode.cn/circle/discuss/CaOJ45/)，我们可以把元音集合 $\{ \text{a, e, i, o, u} \}$ 视作数字 $2^1 + 2^5 + 2^9 + 2^{15} + 2^{21} = 2130466$ ，即十六进制的 $\text{0x208222}$ 。如果字母对应的数字都减一，则可视作 $\text{0x104111}$ 。

可以用位运算，快速判断字母是否在元音集合中。
```java
 class Solution {
    public String sortVowels(String S) {
        final int VOWEL_MASK = 0x208222;

        char[] s = S.toCharArray();
        byte[] vowels = new byte[s.length]; // 比 StringBuilder 快
        int k = 0;
        for (char ch : s) {
            if ((VOWEL_MASK >> (ch & 31) & 1) > 0) {
                vowels[k++] = (byte) ch;
            }
        }

        Arrays.sort(vowels, 0, k);

        k = 0;
        for (int i = 0; i < s.length; i++) {
            if ((VOWEL_MASK >> (s[i] & 31) & 1) > 0) {
                s[i] = (char) vowels[k++];
            }
        }
        return new String(s);
    }
}
```
```cpp
class Solution {
public:
    string sortVowels(string s) {
        const int VOWEL_MASK = 0x208222;
        string vowels;
        for (char ch : s) {
            if (VOWEL_MASK >> (ch & 31) & 1) { // ch 是元音
                vowels += ch;
            }
        }

        ranges::sort(vowels);

        int j = 0;
        for (char& ch : s) {
            if (VOWEL_MASK >> (ch & 31) & 1) { // ch 是元音
                ch = vowels[j++];
            }
        }
        return s;
    }
}; 
```
```rust
impl Solution {
    pub fn sort_vowels(s: String) -> String {
        const VOWEL_MASK: u32 = 0x208222;
        let mut vowels = s.bytes()
            .filter(|&ch| VOWEL_MASK >> (ch & 31) & 1 > 0)
            .collect::<Vec<_>>();

        vowels.sort_unstable();

        let mut s = s.into_bytes();
        let mut j = 0;
        for ch in s.iter_mut() {
            if VOWEL_MASK >> (*ch & 31) & 1 > 0 {
                *ch = vowels[j];
                j += 1;
            }
        }
        unsafe { String::from_utf8_unchecked(s) }
    }
}
```
```python
class Solution:
    def sortVowels(self, s: str) -> str:
        VOWEL_MASK = 0x208222
        is_vowel = lambda ch: VOWEL_MASK >> (ord(ch) & 31) & 1

        vowels = sorted(filter(is_vowel, s))
        t = list(s)  # str 无法修改，转成 list
        j = 0
        for i, ch in enumerate(t):
            if is_vowel(ch):
                t[i] = vowels[j]  # 填空
                j += 1
        return ''.join(t)
```
```c
#define VOWEL_MASK 0x208222

int cmp(const void* a, const void* b) {
    return *(char*)a - *(char*)b;
}

char* sortVowels(char* s) {
    int n = strlen(s);
    char* vowels = malloc(n * sizeof(char));
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (VOWEL_MASK >> (s[i] & 31) & 1) {
            vowels[k++] = s[i];
        }
    }

    qsort(vowels, k, sizeof(char), cmp);

    k = 0;
    for (int i = 0; i < n; i++) {
        if (VOWEL_MASK >> (s[i] & 31) & 1) {
            s[i] = vowels[k++];
        }
    }

    free(vowels);
    return s;
}
```
```go
func sortVowels(s string) string {
	const vowelMask = 0x208222
	vowels := []byte{}
	for _, ch := range s {
		if vowelMask>>(ch&31)&1 > 0 { // ch 是元音
			vowels = append(vowels, byte(ch))
		}
	}

	slices.Sort(vowels)

	t := []byte(s)
	j := 0
	for i, ch := range t {
		if vowelMask>>(ch&31)&1 > 0 { // ch 是元音
			t[i] = vowels[j]
			j++
		}
	}
	return string(t)
}
```
```javascript
var sortVowels = function(s) {
    const VOWEL_MASK = 0x208222;
    const vowels = [];
    for (const ch of s) {
        if (VOWEL_MASK >> (ch.charCodeAt(0) & 31) & 1) {
            vowels.push(ch);
        }
    }

    vowels.sort();

    const t = s.split('');
    let j = 0;
    for (let i = 0; i < t.length; i++) {
        if (VOWEL_MASK >> (t[i].charCodeAt(0) & 31) & 1) {
            t[i] = vowels[j++];
        }
    }
    return t.join('');
};
```
复杂度分析：
- 时间复杂度：$O(n \log n)$ ，其中 $n$ 是 $s$ 的长度。
- 空间复杂度：$O(n)$ 。
### 方法二 计数排序
```java
class Solution {
    public String sortVowels(String S) {
        final int VOWEL_MASK = 0x208222;

        char[] s = S.toCharArray();
        int[] cnt = new int['u' + 1];
        for (char ch : s) {
            if ((VOWEL_MASK >> (ch & 31) & 1) > 0) {
                cnt[ch]++;
            }
        }

        int j = 'A';
        for (int i = 0; i < s.length; i++) {
            if ((VOWEL_MASK >> (s[i] & 31) & 1) == 0) {
                continue;
            }
            // 找下一个出现次数大于 0 的元音字母
            while (cnt[j] == 0) {
                j = j == 'Z' ? 'a' : j + 1;
            }
            s[i] = (char) j;
            cnt[j]--;
        }
        return new String(s);
    }
} 
```
```cpp
class Solution {
public:
    string sortVowels(string s) {
        const int VOWEL_MASK = 0x208222;
        int cnt['u' + 1]{};
        for (char ch : s) {
            if (VOWEL_MASK >> (ch & 31) & 1) {
                cnt[ch]++;
            }
        }

        char j = 'A';
        for (char& ch : s) {
            if ((VOWEL_MASK >> (ch & 31) & 1) == 0) {
                continue;
            }
            // 找下一个出现次数大于 0 的元音字母
            while (cnt[j] == 0) {
                j = j == 'Z' ? 'a' : j + 1;
            }
            ch = j;
            cnt[j]--;
        }
        return s;
    }
};
```
```rust
impl Solution {
    pub fn sort_vowels(s: String) -> String {
        const VOWEL_MASK: u32 = 0x208222;
        let mut cnt = [0; 'z' as usize + 1];
        for ch in s.bytes() {
            if (VOWEL_MASK >> (ch & 31)) & 1 > 0 {
                cnt[ch as usize] += 1;
            }
        }

        let mut s = s.into_bytes();
        let mut j = 0;
        for ch in s.iter_mut() {
            if VOWEL_MASK >> (*ch & 31) & 1 == 0 {
                continue;
            }
            // 找下一个出现次数大于 0 的元音字母
            while cnt[j as usize] == 0 {
                if j == b'Z' {
                    j = b'a';
                } else {
                    j += 1;
                }
            }
            *ch = j;
            cnt[j as usize] -= 1;
        }
        unsafe { String::from_utf8_unchecked(s) }
    }
}
```
```python
class Solution:
    def sortVowels(self, s: str) -> str:
        VOWELS = "AEIOUaeiou"
        cnt = Counter(ch for ch in s if ch in VOWELS)

        it = iter(VOWELS)
        cur = next(it)

        t = list(s)  # str 无法修改，转成 list
        for i, ch in enumerate(t):
            if ch in VOWELS:
                if cnt[cur] == 0:
                    # 找下一个出现次数大于 0 的元音字母
                    cur = next(c for c in it if cnt[c])
                t[i] = cur
                cnt[cur] -= 1
        return ''.join(t)
```
```c
#define VOWEL_MASK 0x208222

char* sortVowels(char* s) {
    int cnt['z' + 1] = {};
    for (int i = 0; s[i]; i++) {
        if (VOWEL_MASK >> (s[i] & 31) & 1) {
            cnt[s[i]]++;
        }
    }

    char j = 'A';
    for (int i = 0; s[i]; i++) {
        if ((VOWEL_MASK >> (s[i] & 31) & 1) == 0) {
            continue;
        }
        // 找下一个出现次数大于 0 的元音字母
        while (cnt[j] == 0) {
            j = j == 'Z' ? 'a' : j + 1;
        }
        s[i] = j;
        cnt[j]--;
    }
    return s;
}
```
```go
func sortVowels(s string) string {
	const vowelMask = 0x208222
	cnt := ['u' + 1]int{}
	for _, ch := range s {
		if vowelMask>>(ch&31)&1 > 0 {
			cnt[ch]++
		}
	}

	t := []byte(s)
	j := byte('A')
	for i, ch := range t {
		if vowelMask>>(ch&31)&1 == 0 {
			continue
		}
		// 找下一个出现次数大于 0 的元音字母
		for cnt[j] == 0 {
			if j == 'Z' {
				j = 'a'
			} else {
				j++
			}
		}
		t[i] = j
		cnt[j]--
	}
	return string(t)
}
```
```javascript
var sortVowels = function(s) {
    const VOWEL_MASK = 0x208222;
    const cnt = Array('u'.charCodeAt(0) + 1).fill(0);
    for (const ch of s) {
        const c = ch.charCodeAt(0);
        if (VOWEL_MASK >> (c & 31) & 1) {
            cnt[c]++;
        }
    }

    const t = s.split('');
    const ordZ = 'Z'.charCodeAt(0);
    let j = 'A'.charCodeAt(0);
    for (let i = 0; i < t.length; i++) {
        if ((VOWEL_MASK >> (t[i].charCodeAt(0) & 31) & 1) === 0) {
            continue;
        }
        // 找下一个出现次数大于 0 的元音字母
        while (cnt[j] === 0) {
            j = j == ordZ ? 'a'.charCodeAt(0) : j + 1;
        }
        t[i] = String.fromCharCode(j);
        cnt[j]--;
    }
    return t.join('');
};
```
复杂度分析：
- 时间复杂度：$O(n + | \Sigma | )$ ，其中 $n$ 是 $s$ 的长度，$|\Sigma| = 10$ 或 $52$ 或 $128$ 是字符集合的大小。
- 空间复杂度：$O(| \Sigma |)$ 。