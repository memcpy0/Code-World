> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个包含若干星号 `*` 的字符串 `s` 。

在一步操作中，你可以：
- 选中 `s` 中的一个星号。
- 移除星号 **左侧** 最近的那个 **非星号** 字符，并移除该星号自身。

返回移除 **所有** 星号之后的字符串。

**注意：**
- 生成的输入保证总是可以执行题面中描述的操作。
- 可以证明结果字符串是唯一的。

**示例 1：**
<pre><strong>输入：</strong>s = "leet**cod*e"
<strong>输出：</strong>"lecoe"
<strong>解释：</strong>从左到右执行移除操作：
- 距离第 1 个星号最近的字符是 "lee<em><strong>t</strong></em>**cod*e" 中的 't' ，s 变为 "lee*cod*e" 。
- 距离第 2 个星号最近的字符是 "le<em><strong>e</strong></em>*cod*e" 中的 'e' ，s 变为 "lecod*e" 。
- 距离第 3 个星号最近的字符是 "leco<em><strong>d</strong></em>*e" 中的 'd' ，s 变为 "lecoe" 。
不存在其他星号，返回 "lecoe" 。</pre>

**示例 2：**
<pre><strong>输入：</strong>s = "erase*****"
<strong>输出：</strong>""
<strong>解释：</strong>整个字符串都会被移除，所以返回空字符串。
</pre>

**提示：**
- `1 <= s.length <= 10^5`
- `s` 由小写英文字母和星号 `*` 组成
- `s` 可以执行上述操作

---
### 方法 $O(n)$ 用栈维护
用栈维护，遇到星号 `*` 则弹出栈顶，否则把字符入栈。最后从栈底到栈顶就是答案。
> 注：题目保证生成的输入总是可以执行题面中描述的操作。

```python
class Solution:
    def removeStars(self, s: str) -> str:
        st = []
        for c in s:
            if c == '*':
                st.pop()
            else:
                st.append(c)
        return ''.join(st)
```
```java
class Solution {
    public String removeStars(String s) {
        StringBuilder st = new StringBuilder();
        for (char c : s.toCharArray()) {
            if (c == '*') {
                st.deleteCharAt(st.length() - 1);
            } else {
                st.append(c);
            }
        }
        return st.toString();
    }
}
```
```cpp
class Solution {
public:
    string removeStars(string s) {
        string st;
        for (char c : s) {
            if (c == '*') st.pop_back();
            else st.push_back(c);
        }
        return st;
    }
};
```
```c
char* removeStars(char* s) {
    int top = 0; // 栈顶
    for (int i = 0; s[i]; i++) {
        if (s[i] == '*') top--; // 出栈
        else s[top++] = s[i]; // 入栈（把 s 当栈）
    }
    s[top] = '\0';
    return s;
}
```
```go
func removeStars(s string) string {
	st := []rune{}
	for _, c := range s {
		if c == '*' {
			st = st[:len(st)-1]
		} else {
			st = append(st, c)
		}
	}
	return string(st)
}
```
```js
var removeStars = function(s) {
    const st = [];
    for (const c of s) {
        if (c === '*') {
            st.pop();
        } else {
            st.push(c);
        }
    }
    return st.join('');
};
```
```rust
impl Solution {
    pub fn remove_stars(s: String) -> String {
        let mut st = vec![];
        for c in s.bytes() {
            if c == b'*' {
                st.pop();
            } else {
                st.push(c);
            }
        }
        unsafe { String::from_utf8_unchecked(st) }
    }
}
```
**复杂度分析**
- 时间复杂度：$O(n)$ ，其中 $n$ 为 $s$ 的长度。
- 空间复杂度：$O(n)$ 或 $O(1)$ 。如果把 $s$ 当作栈，则空间复杂度为 $O(1)$ ，见 C 语言。