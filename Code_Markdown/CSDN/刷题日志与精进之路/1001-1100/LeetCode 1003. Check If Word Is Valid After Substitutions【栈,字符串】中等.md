> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个字符串 `s` ，请你判断它是否 **有效** 。字符串 `s` **有效** 需要满足：假设开始有一个空字符串 `t = ""` ，你可以执行 **任意次** 下述操作将 `t` **转换为** `s` ：
-   将字符串 `"abc"` 插入到 `t` 中的任意位置。形式上，`t` 变为 `tleft + "abc" + tright`，其中 `t == tleft + tright` 。注意，`tleft` 和 `tright` 可能为 **空** 。

如果字符串 `s` 有效，则返回 `true`；否则，返回 `false`。

<strong class="example">示例 1：</strong>
<pre><strong>Input:</strong> s = "aabcbc"
<strong>Output:</strong> true
<strong>Explanation:</strong>
"" -&gt; "<u>abc</u>" -&gt; "a<u>abc</u>bc"
Thus, "aabcbc" is valid.</pre>

<strong class="example">示例 2：</strong>
<pre><strong>Input:</strong> s = "abcabcababcc"
<strong>Output:</strong> true
<strong>Explanation:</strong>
"" -&gt; "<u>abc</u>" -&gt; "abc<u>abc</u>" -&gt; "abcabc<u>abc</u>" -&gt; "abcabcab<u>abc</u>c"
Thus, "abcabcababcc" is valid.
</pre>

<strong class="example">示例 3：</strong>
<pre><strong>Input:</strong> s = "abccba"
<strong>Output:</strong> false
<strong>Explanation:</strong> It is impossible to get "abccba" using the operation.
</pre>

**提示：**
-   `1 <= s.length <= 2 * 10^4`
-   `s` 由字母 `'a'`、`'b'` 和 `'c'` 组成
 
---
### 解法1 暴力
本题与[20. 有效的括号](https://leetcode.cn/problems/valid-parentheses/description/)类似，如果把问题改成「将字符串 `(), [], {}` 插入到 $t$ 中的任意位置」，比如 `() -> ([] -> ([]{})` ，根据「是否可用这种方式获得字符串 $s$ 」来判断字符串有效，就完全一致了。

这里最简单的做法是**逆向转换**，不断从字符串中**删除加入的 $abc$ 、最后删成空**，就能返回 `true` ：
```cpp
class Solution {
public:
    bool isValid(string s) {
        size_t pos;
        while ((pos = s.find("abc")) != string::npos)
            s.erase(pos, 3);
        return s.empty();
    }
};
```

---
### 解法2 栈
解法1复杂度高，于是想到了栈。[20. 有效的括号](https://leetcode.cn/problems/valid-parentheses/description/)也是用栈解决的：左括号入栈，对于右括号去判断栈顶是否为其匹配的左括号，匹配就让左括号出栈，否则返回 `false` 。本题也可用类似的方法完成：
- 字符 ${a}$ ：类似左括号，直接入栈。
- 字符 ${b}$ ：**如果栈为空，或者栈顶不为 ${a}$ ，则返回 `false`** ，否则将栈顶修改为 ${b}$（或者出栈再入栈）。
- 字符 ${c}$ ：**如果栈为空，或者栈顶不为 ${b}$ ，则返回 `false`** ，否则弹出栈顶，相当于找到了一个 ${abc}$ 。
- 代码实现时，${b}$ 和 ${c}$ 的逻辑可以合并在一起，${a}$ 和 ${b}$ 的入栈逻辑可以合并在一起。
- 循环结束后，如果栈为空，则返回 `true` ，否则返回 `false`。

> 一个合适的比喻是**消消乐**。比如 `ab abc c` ，中间的 `abc` 消除后，`c` 从后面滑到前面、撞击前面的 `ab` 、再次消除。

```cpp
class Solution {
public: 
	bool isValid(string s) { // s同时作为栈
		int i = 0; // i-1表示栈顶下标,i=0表示栈为空
		for (char c : s) {
			if (c > 'a' && (i == 0 || c - s[--i] != 1))
				return false;
			if (c < 'c') s[i++] = c; // 入栈
		}
		return i == 0;
	}
};
```
复杂度分析
- 时间复杂度：$O(n)$ ，其中 $n$ 为 $s$ 的长度。
- 空间复杂度：$O(n)$ 或 $O(1)$ 。如果可以直接修改字符串（例如C++），那么只需 $O(1)$ 的额外空间。
 