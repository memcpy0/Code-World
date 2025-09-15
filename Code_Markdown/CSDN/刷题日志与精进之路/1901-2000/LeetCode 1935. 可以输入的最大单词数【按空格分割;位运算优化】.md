> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

键盘出现了一些故障，有些字母键无法正常工作。而键盘上所有其他键都能够正常工作。

给你一个由若干单词组成的字符串 `text` ，单词间由单个空格组成（不含前导和尾随空格）；另有一个字符串 `brokenLetters` ，由所有已损坏的不同字母键组成，返回你可以使用此键盘完全输入的 `text` 中单词的数目。

**示例 1：**
```c
输入：text = "hello world", brokenLetters = "ad"
输出：1
解释：无法输入 "world" ，因为字母键 'd' 已损坏。
```
**示例 2：**
```c
输入：text = "leet code", brokenLetters = "lt"
输出：1
解释：无法输入 "leet" ，因为字母键 'l' 和 't' 已损坏。
```
**示例 3：**
```c
输入：text = "leet code", brokenLetters = "e"
输出：0
解释：无法输入任何单词，因为字母键 'e' 已损坏。
```
**提示：**
- `1 <= text.length <= 10^4`
- `0 <= brokenLetters.length <= 26`
- `text` 由若干用单个空格分隔的单词组成，且不含任何前导和尾随空格
- 每个单词仅由小写英文字母组成
- `brokenLetters` 由 **互不相同** 的小写英文字母组成

### 方法一 按空格分割
把 $text$ 按照空格分割成单词，对每个单词，如果单词中的所有字母都不在 $brokenLetters$ 中，那么把答案加一。
```java
class Solution {
    public int canBeTypedWords(String text, String brokenLetters) {
        int ans = 0;
        for (String word : text.split(" ")) {
            if (!containsAny(word, brokenLetters)) {
                ans++;
            }
        }
        return ans;
    }

    private boolean containsAny(String word, String brokenLetters) {
        for (char c : word.toCharArray()) {
            if (brokenLetters.indexOf(c) != -1) {
                return true;
            }
        }
        return false;
    }
}
```
```cpp
class Solution {
public:
    int canBeTypedWords(string text, string brokenLetters) {
        istringstream iss(text);
        string word;
        int ans = 0;
        while (iss >> word) {
            if (ranges::all_of(word, [&](char c){ return brokenLetters.find(c) == string::npos; })) {
                ans++;
            }
        }
        return ans;
    }
};
```
```rust
impl Solution {
    pub fn can_be_typed_words(text: String, broken_letters: String) -> i32 {
        let mut ans = 0;
        for word in text.split_whitespace() {
            let mut broken = true;
            for ch in word.bytes() {
                if broken_letters.contains(ch as char) {
                    broken = false;
                    break;
                }
            }
            if broken {
                ans += 1;
            }
        }
        ans
    }
}
```
```python
class Solution:
    def canBeTypedWords(self, text: str, brokenLetters: str) -> int:
        ans = 0
        for word in text.split():
            if all(c not in brokenLetters for c in word):
                ans += 1
        return ans
```
```go
func canBeTypedWords(text, brokenLetters string) (ans int) {
	for _, word := range strings.Split(text, " ") {
		if !strings.ContainsAny(word, brokenLetters) {
			ans++
		}
	}
	return
}
```
复杂度分析：
- 时间复杂度：$O(nm)$ ，其中 $n$ 是 $text$ 的长度，$m$ 是 $brokenLetters$ 的长度。
- 空间复杂度：$O(n)$ 或 $O(1)$ 。
### 方法二 位运算优化
将 $brokenLetters$ 视作一个字母集合，我们需要判断单词的字母是否在这个集合中。集合可以用二进制数表示，集合的运算可以用位运算快速计算。
```java
class Solution {
    public int canBeTypedWords(String text, String brokenLetters) {
        int brokenMask = 0;
        for (char c : brokenLetters.toCharArray()) {
            brokenMask |= 1 << (c - 'a'); // 把 c 加到集合中
        }

        int ans = 0;
        int ok = 1;
        for (char c : text.toCharArray()) {
            if (c == ' ') { // 上一个单词遍历完毕
                ans += ok;
                ok = 1;
            } else if ((brokenMask >> (c - 'a') & 1) > 0) { // c 在 brokenLetters 中
                ok = 0;
            }
        }
        ans += ok; // 最后一个单词
        return ans;
    }
}
```
```cpp
class Solution {
public:
    int canBeTypedWords(string text, string brokenLetters) {
        int broken_mask = 0;
        for (char c : brokenLetters) {
            broken_mask |= 1 << (c - 'a'); // 把 c 加到集合中
        }

        int ans = 0;
        bool ok = true;
        for (char c : text) {
            if (c == ' ') { // 上一个单词遍历完毕
                ans += ok;
                ok = true;
            } else if (broken_mask >> (c - 'a') & 1) { // c 在 brokenLetters 中
                ok = false;
            }
        }
        ans += ok; // 最后一个单词
        return ans;
    }
};
```
```rust
impl Solution {
    pub fn can_be_typed_words(text: String, broken_letters: String) -> i32 {
        let mut broken_mask = 0;
        for c in broken_letters.bytes() {
            broken_mask |= 1 << (c - b'a');
        }
        let mut ans = 0;
        let mut ok = 1;
        for c in text.bytes() {
            if c == b' ' {
                ans += ok;
                ok = 1;
            } else if (broken_mask >> (c - b'a') & 1) > 0 {
                ok = 0;
            }
        }
        ans + ok
    }
}
```
```python
class Solution:
    def canBeTypedWords(self, text: str, brokenLetters: str) -> int:
        broken_mask = 0
        for c in brokenLetters:
            broken_mask |= 1 << (ord(c) - ord('a'))  # 把 c 加到集合中

        ans = 0
        ok = True
        for c in text:
            if c == ' ':  # 上一个单词遍历完毕
                ans += ok
                ok = True
            elif broken_mask >> (ord(c) - ord('a')) & 1:  # c 在 brokenLetters 中
                ok = False
        ans += ok  # 最后一个单词
        return ans
```
```go
func canBeTypedWords(text, brokenLetters string) (ans int) {
	brokenMask := 0
	for _, c := range brokenLetters {
		brokenMask |= 1 << (c - 'a') // 把 c 加到集合中
	}

	ok := 1
	for _, c := range text {
		if c == ' ' { // 上一个单词遍历完毕
			ans += ok
			ok = 1
		} else if brokenMask>>(c-'a')&1 > 0 { // c 在 brokenLetters 中
			ok = 0
		}
	}
	ans += ok // 最后一个单词
	return
}
```
复杂度分析：
- 时间复杂度：$O(n + m)$ ，其中 $n$ 是 $text$ 的长度，$m$ 是 $brokenLetters$ 的长度。
- 空间复杂度：$O(1)$ 。