> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的字符串 `text` 和另一个下标从 **0** 开始且长度为 `2` 的字符串 `pattern` ，两者都只包含小写英文字母。

你可以在 `text` 中任意位置插入 **一个** 字符，这个插入的字符必须是 `pattern[0]` **或者** `pattern[1]` 。注意，这个字符可以插入在 `text` 开头或者结尾的位置。

请你返回插入一个字符后，`text` 中最多包含多少个等于 `pattern` 的 **子序列** 。

**子序列** 指的是将一个字符串删除若干个字符后（也可以不删除），剩余字符保持原本顺序得到的字符串。

**示例 1：**
<pre><b>输入：</b>text = "abdcdbc", pattern = "ac"
<b>输出：</b>4
<strong>解释：</strong>
如果我们在 text[1] 和 text[2] 之间添加 pattern[0] = 'a' ，那么我们得到 "ab<em><strong>a</strong></em>dcdbc" 。那么 "ac" 作为子序列出现 4 次。
其他得到 4 个 "ac" 子序列的方案还有 "<em><strong>a</strong></em>abdcdbc" 和 "abd<em><strong>a</strong></em>cdbc" 。
但是，"abdc<em><strong>a</strong></em>dbc" ，"abd<em><strong>c</strong></em>cdbc" 和 "abdcdbc<em><strong>c</strong></em>" 这些字符串虽然是可行的插入方案，但是只出现了 3 次 "ac" 子序列，所以不是最优解。
可以证明插入一个字符后，无法得到超过 4 个 "ac" 子序列。
</pre>

**示例 2：**
<pre><b>输入：</b>text = "aabb", pattern = "ab"
<b>输出：</b>6
<strong>解释：</strong>
可以得到 6 个 "ab" 子序列的部分方案为 "<em><strong>a</strong></em>aabb" ，"aa<em><strong>a</strong></em>bb" 和 "aab<em><strong>b</strong></em>b" 。
</pre>

**提示：**
- `1 <= text.length <= 10^5`
- `pattern.length == 2`
- `text` 和 `pattern` 都只包含小写英文字母。

---
### 方法：贪心+一次遍历
首先计算插入之前的 $pattern$ 子序列的个数，然后计算因为插入字母额外增加的 $pattern$ 子序列的个数。

设 $x=pattern[0],\ y=pattern[1]$ 。

遍历 $text$ 统计答案：遇到 $y$ 时，如果左边出现了 $3$ 个 $x$ ，那么就意味着我们找到了 $3$ 个 $pattern$ 子序列，把 $3$ 加入答案。一般地，**在遍历 $text$ 的同时，维护 $x$ 的出现次数 $cntX$ 。遇到 $y$ 时，把 $cntX$ 加入答案**。

然后考虑插入字母。根据题意，$x$ 插入的位置越靠左，$pattern$ 子序列的个数越多；$y$ 插入的位置越靠右，$pattern$ 子序列的个数越多。那么 $x$ 应插在 $text$ 最左侧，$y$ 应插在 $text$ 最右侧。

分类讨论：
- 把 $x$ 插在 $text$ 最左侧：答案额外增加 $cntY$ ，其中 $cntY$ 是 $y$ 在 $text$ 中的出现次数。
- 把 $y$ 插在 $text$ 最右侧：答案额外增加 $cntX$ ，其中 $cntX$ 是 $x$ 在 $text$ 中的出现次数。

> ⚠注意：代码没有特判 $x=y$ 的情况，要先更新答案，再更新 $cntX$ ，这可以保证更新答案时 $cntX$ 表示的是当前字母左边的 $x$ 的出现次数，$cntX$ 尚未计入当前字母。

> ⚠注意：如果特判两个字母相等的情况，此时就是等差数列求和，$text$ 中该字母的个数设为 $c$ ，则答案为 $\dfrac{c (c - 1)}{ 2} +c$ 。
```java
class Solution {
    public long maximumSubsequenceCount(String text, String pattern) {
        char x = pattern.charAt(0);
        char y = pattern.charAt(1);
        long ans = 0;
        int cntX = 0;
        int cntY = 0;
        for (char c : text.toCharArray()) {
            if (c == y) {
                ans += cntX;
                cntY++;
            }
            if (c == x) {
                cntX++;
            }
        }
        return ans + Math.max(cntX, cntY);
    }
}
```
```rust
impl Solution {
    pub fn maximum_subsequence_count(text: String, pattern: String) -> i64 {
        let pattern = pattern.as_bytes();
        let x = pattern[0];
        let y = pattern[1];
        let mut ans = 0i64;
        let mut cnt_x = 0;
        let mut cnt_y = 0;
        for c in text.bytes() {
            if c == y {
                ans += cnt_x as i64;
                cnt_y += 1;
            }
            if c == x {
                cnt_x += 1;
            }
        }
        ans + cnt_x.max(cnt_y) as i64
    }
}
```
```js
var maximumSubsequenceCount = function(text, pattern) {
    const [x, y] = pattern;
    let ans = 0, cntX = 0, cntY = 0;
    for (const c of text) {
        if (c === y) {
            ans += cntX;
            cntY++;
        }
        if (c === x) {
            cntX++;
        }
    }
    return ans + Math.max(cntX, cntY);
};
```
```cpp
class Solution {
public:
    long long maximumSubsequenceCount(string text, string pattern) {
        char x = pattern[0], y = pattern[1];
        long long ans = 0;
        int cnt_x = 0, cnt_y = 0;
        for (char c : text) {
            if (c == y) {
                ans += cnt_x;
                cnt_y++;
            }
            if (c == x) {
                cnt_x++;
            }
        }
        return ans + max(cnt_x, cnt_y);
    }
};
```
```python
class Solution:
    def maximumSubsequenceCount(self, text: str, pattern: str) -> int:
        x, y = pattern
        ans = cnt_x = cnt_y = 0
        for c in text:
            if c == y:
                ans += cnt_x
                cnt_y += 1
            if c == x:
                cnt_x += 1
        return ans + max(cnt_x, cnt_y)
```
```c
#define MAX(a, b) ((b) > (a) ? (b) : (a))

long long maximumSubsequenceCount(char* text, char* pattern) {
    char x = pattern[0], y = pattern[1];
    long long ans = 0;
    int cnt_x = 0, cnt_y = 0;
    for (int i = 0; text[i]; i++) {
        if (text[i] == y) {
            ans += cnt_x;
            cnt_y++;
        }
        if (text[i] == x) {
            cnt_x++;
        }
    }
    return ans + MAX(cnt_x, cnt_y);
}
```
```go
func maximumSubsequenceCount(text, pattern string) (ans int64) {
    x, y := pattern[0], pattern[1]
    cntX, cntY := 0, 0
    for i := range text {
        c := text[i]
        if c == y {
            ans += int64(cntX)
            cntY++
        }
        if c == x {
            cntX++
        }
    }
    return ans + int64(max(cntX, cntY))
}
```
**复杂度分析**
- 时间复杂度：$O(n)$ ，其中 $n$ 是 $text$ 的长度。
- 空间复杂度：$O(1)$ 。

**思考题**
如果 $pattern$ 的长度是 $3$ 呢？是 $m$ 呢？