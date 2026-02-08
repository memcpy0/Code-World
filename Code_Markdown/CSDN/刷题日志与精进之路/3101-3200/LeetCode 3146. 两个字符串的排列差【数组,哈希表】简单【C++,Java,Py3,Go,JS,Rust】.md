> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个字符串 `s` 和 `t`，每个字符串中的字符都不重复，且 `t` 是 `s` 的一个排列。

**排列差** 定义为 `s` 和 `t` 中每个字符在两个字符串中位置的绝对差值之和。

返回 `s` 和 `t` 之间的 **排列差** 。

**示例 1：**
```c
输入：s = "abc", t = "bac"
输出：2
解释：
对于 `s = "abc"` 和 `t = "bac"`，排列差是：
- `"a"` 在 `s` 中的位置与在 `t` 中的位置之差的绝对值。
- `"b"` 在 `s` 中的位置与在 `t` 中的位置之差的绝对值。
- `"c"` 在 `s` 中的位置与在 `t` 中的位置之差的绝对值。

即，`s` 和 `t` 的排列差等于 `|0 - 1| + |2 - 2| + |1 - 0| = 2`。
```
**示例 2：**
```c
输入：s = "abcde", t = "edbac"
输出：12
解释： `s` 和 `t` 的排列差等于 `|0 - 3| + |1 - 2| + |2 - 4| + |3 - 1| + |4 - 0| = 12`。
```
**提示：**
- `1 <= s.length <= 26`
- 每个字符在 `s` 中最多出现一次。
- `t` 是 `s` 的一个排列。
- `s` 仅由小写英文字母组成。

---
### 哈希表或数组记录字符下标
遍历 $s$ ，用哈希表（或者数组）记录字母 $c=s[i]$ 的下标 $pos[c]=i$ 。注意 $s$ 和 $t$ 中没有重复字母。然后遍历 $t$ ，累加 $∣i−pos[t[i]]∣$ ，即为答案。
```python
class Solution:
    def findPermutationDifference(self, s: str, t: str) -> int:
        pos = {c : i for i, c in enumerate(s)}
        return sum(abs(i - pos[c]) for i, c in enumerate(t))
```
```java
class Solution {
    public int findPermutationDifference(String s, String t) {
        int[] pos = new int[26];
        for (int i = 0; i < s.length(); ++i) pos[s.charAt(i) - 'a'] = i;
        int ans = 0;
        for (int i = 0; i < t.length(); ++i) ans += Math.abs(i - pos[t.charAt(i) - 'a']);
        return ans;
    }
}
```
```cpp
class Solution {
public:
    int findPermutationDifference(string s, string t) {
        int sp[26] = {0}, ans = 0;
        for (int i = 0, n = s.size(); i < n; ++i) sp[s[i] - 'a'] = i;
        for (int i = 0, n = t.size(); i < n; ++i) ans += abs(sp[t[i] - 'a'] - i);
        return ans;
    }
};
```
```go
func findPermutationDifference(s, t string) (ans int) {
	pos := [26]int{}
	for i, c := range s {
		pos[c-'a'] = i
	}
	for i, c := range t {
		ans += abs(i - pos[c-'a'])
	}
	return
}

func abs(x int) int { if x < 0 { return -x }; return x }
```
```javascript
/**
 * @param {string} s
 * @param {string} t
 * @return {number}
 */
var findPermutationDifference = function(s, t) {
    const pos = {};
    for (let i = 0; i < s.length; ++i) pos[s[i]] = i;
    let ans = 0;
    for (let i = 0; i < t.length; ++i) ans += Math.abs(pos[t[i]] - i);
    return ans;
};
```
```rust
use std::collections::HashMap;
impl Solution {
    pub fn find_permutation_difference(s: String, t: String) -> i32 {
        let mut pos: HashMap<char, i32> = HashMap::new();
        let mut i: i32 = 0;
        for c in s.chars() {
            pos.insert(c, i);
            i += 1; 
        }
        let mut ans: i32 = 0;
        i = 0;
        for c in t.chars() {
            ans += i32::abs(pos.get(&c).copied().unwrap() - i);
            i += 1;
        }
        ans
    }
}
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为 $s$ 的长度。
- 空间复杂度：$O(n)$ 。