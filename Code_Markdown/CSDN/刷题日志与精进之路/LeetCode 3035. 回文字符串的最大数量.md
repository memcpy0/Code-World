> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的字符串数组 `words` ，数组的长度为 `n` ，且包含下标从 **0** 开始的若干字符串。

你可以执行以下操作 **任意** 次数（**包括零次**）：

- 选择整数`i`、`j`、`x`和`y`，满足`0 <= i, j < n`，`0 <= x < words[i].length`，`0 <= y < words[j].length`，**交换** 字符 `words[i][x]` 和 `words[j][y]` 。

返回一个整数，表示在执行一些操作后，`words` 中可以包含的回文字符串的 **最大** 数量。

**注意：**在操作过程中，`i` 和 `j` 可以相等。

**示例 1：**

**输入：**words = ["abbb","ba","aa"]
**输出：**3
**解释：**在这个例子中，获得最多回文字符串的一种方式是：
选择 i = 0, j = 1, x = 0, y = 0，交换 words[0][0] 和 words[1][0] 。words 变成了 ["bbbb","aa","aa"] 。
words 中的所有字符串都是回文。
因此，可实现的回文字符串的最大数量是 3 。

**示例 2：**

**输入：**words = ["abc","ab"]
**输出：**2
**解释：**在这个例子中，获得最多回文字符串的一种方式是： 
选择 i = 0, j = 1, x = 1, y = 0，交换 words[0][1] 和 words[1][0] 。words 变成了 ["aac","bb"] 。
选择 i = 0, j = 0, x = 1, y = 2，交换 words[0][1] 和 words[0][2] 。words 变成了 ["aca","bb"] 。
两个字符串都是回文 。
因此，可实现的回文字符串的最大数量是 2。

**示例 3：**

**输入：**words = ["cd","ef","a"]
**输出：**1
**解释：**在这个例子中，没有必要执行任何操作。
words 中有一个回文 "a" 。
可以证明，在执行任何次数操作后，无法得到更多回文。
因此，答案是 1 。

**提示：**

- `1 <= words.length <= 1000`
- `1 <= words[i].length <= 100`
- `words[i]` 仅由小写英文字母组成。

---
### 两种方法：正向思维 / 逆向思维（Python/Java/C++/Go）

[](https://leetcode.cn/u/endlesscheng/)
方法一：正向思维
提示 1
由于可以随意交换字母，先把所有字母都取出来，然后考虑如何填入各个字符串。

如果一个奇数长度字符串最终是回文串，那么它正中间的那个字母填什么都可以。

既然如此，不妨先把左右的字母填了，最后在往正中间填入字母。

提示 2
字符串越短，需要的字母越少。

所以按照字符串的长度从小到大填。

提示 3
统计所有字符串的长度之和，减去出现次数为奇数的字母，即为可以往左右填入的字母个数 tot\textit{tot}tot。

把字符串按照长度从小到大排序，然后遍历。注意长为奇数的字符串，由于不考虑正中间的字母，其长度要减一。

下面的代码用到了一些位运算技巧，请看 从集合论到位运算，常见位运算技巧分类总结！

作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-palindromes-after-operations/solutions/2637686/gou-zao-tan-xin-pythonjavacgo-by-endless-ib49/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

```cpp
class Solution:
    def maxPalindromesAfterOperations(self, words: List[str]) -> int:
        ans = tot = 0
        cnt = Counter()
        for w in words:
            tot += len(w)
            cnt += Counter(w)
        tot -= sum(c % 2 for c in cnt.values())  # 减去出现次数为奇数的字母

        words.sort(key=len)  # 按照长度从小到大排序
        for w in words:
            tot -= len(w) // 2 * 2  # 长为奇数的字符串，长度要减一
            if tot < 0: break
            ans += 1
        return ans

作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-palindromes-after-operations/solutions/2637686/gou-zao-tan-xin-pythonjavacgo-by-endless-ib49/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

func maxPalindromesAfterOperations(words []string) (ans int) {
	tot, mask := 0, 0
	for _, w := range words {
		tot += len(w)
		for _, c := range w {
			mask ^= 1 << (c - 'a')
		}
	}
	tot -= bits.OnesCount(uint(mask)) // 减去出现次数为奇数的字母

	slices.SortFunc(words, func(a, b string) int { return len(a) - len(b) })
	for _, w := range words {
		tot -= len(w) / 2 * 2 // 长为奇数的字符串，长度要减一
		if tot < 0 { break }
		ans++
	}
	return
}

作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-palindromes-after-operations/solutions/2637686/gou-zao-tan-xin-pythonjavacgo-by-endless-ib49/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
public:
    int maxPalindromesAfterOperations(vector<string> &words) {
        int ans = 0, tot = 0, mask = 0;
        for (auto &w : words) {
            tot += w.length();
            for (char c : w) {
                mask ^= 1 << (c - 'a');
            }
        }
        tot -= __builtin_popcount(mask); // 减去出现次数为奇数的字母

        ranges::sort(words, [](const auto &a, const auto &b) {
            return a.length() < b.length();
        });
        for (auto &w : words) {
            tot -= w.length() / 2 * 2; // 长为奇数的字符串，长度要减一
            if (tot < 0) break;
            ans++;
        }
        return ans;
    }
};

作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-palindromes-after-operations/solutions/2637686/gou-zao-tan-xin-pythonjavacgo-by-endless-ib49/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```
复杂度分析
时间复杂度：O(L+nlog⁡n)\mathcal{O}(L+n\log n)O(L+nlogn)，其中 LLL 为字符串的长度之和，nnn 为 words\textit{words}words 的长度。
空间复杂度：O(1)\mathcal{O}(1)O(1)。利用位运算可以做到 O(1)\mathcal{O}(1)O(1) 额外空间（忽略排序的栈开销）。
方法二：逆向思维
最少有多少个字符串，不是回文串？

提示 1
统计每个字母在所有字符串的总出现次数。如果每个字母的总出现次数都是偶数，所有字符串都可以是回文串吗？

分类讨论：

如果所有字符串的长度都是偶数，我们可以把这些字母对称放置，这样所有字符串都是回文串，例如示例 1。
如果有奇数长度字符串，那么这样的字符串的个数一定是偶数个（因为所有字母的出现次数都是偶数），我们先把字母对称放置，再把剩余的字母放在奇数长度字符串的正中间（剩余的字母可以随便放，不影响回文），这样所有字符串都是回文串。
所有字符串都可以是回文串，答案是 nnn。

提示 2
如果有字母的出现次数是奇数呢？

从出现次数为奇数的字母中，每种各取出一个。比如 a 出现了 333 次，b 出现了 555 次，我们先取出一个 a 和一个 b。

把取出来的字母集合记作 SSS，优先把 SSS 中的字母放在奇数长度字符串的正中间。如果奇数长度字符串的个数，不低于 SSS 的大小，那么把 SSS 中的字母填入正中间后，剩下的每种字母都是偶数，像提示 1 那样放置即可，答案是 nnn。
如果奇数长度字符串的个数，小于 SSS 的大小，那么把 SSS 中的一部分字母填入正中间后，SSS 中还有剩余字母，我们可以找一个长度最长的字符串，放入 SSS 中剩下的字母。如果 SSS 中还有剩余字母，就继续找次长的字符串放入。所以需要按照字符串的长度从大到小排序。由于奇数长度字符串的正中间已经填入了字母，所以在填入 SSS 中的剩余字母时，奇数长度字符串的长度要减一。答案初始化为 nnn，每用一个字符串放 SSS 剩余字母，就把答案减一。
Python3
Java
C++
Go


作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-palindromes-after-operations/solutions/2637686/gou-zao-tan-xin-pythonjavacgo-by-endless-ib49/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```cpp
class Solution:
    def maxPalindromesAfterOperations(self, words: List[str]) -> int:
        odd_l = 0  # 奇数长度字符串个数
        cnt = Counter()
        for w in words:
            odd_l += len(w) % 2
            cnt += Counter(w)

        words.sort(key=lambda w: -len(w))  # 按照长度从大到小排序

        ans = len(words)
        left = sum(c % 2 for c in cnt.values()) - odd_l  # S 中的剩余字母个数
        for w in words:
            if left <= 0: break
            left -= len(w) // 2 * 2
            ans -= 1
        return ans

作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-palindromes-after-operations/solutions/2637686/gou-zao-tan-xin-pythonjavacgo-by-endless-ib49/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
    public int maxPalindromesAfterOperations(String[] words) {
        int oddL = 0; // 奇数长度字符串个数
        int mask = 0; // 奇数个数的字母集合
        for (String w : words) {
            oddL += w.length() % 2;
            for (char c : w.toCharArray()) {
                mask ^= 1 << (c - 'a');
            }
        }

        Arrays.sort(words, (a, b) -> b.length() - a.length());

        int ans = words.length;
        int left = Integer.bitCount(mask) - oddL; // S 中的剩余字母个数
        for (String w : words) {
            if (left <= 0) break;
            left -= w.length() / 2 * 2;
            ans--;
        }
        return ans;
    }
}

作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-palindromes-after-operations/solutions/2637686/gou-zao-tan-xin-pythonjavacgo-by-endless-ib49/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
public:
    int maxPalindromesAfterOperations(vector<string> &words) {
        int oddL = 0; // 奇数长度字符串个数
        int mask = 0; // 奇数个数的字母集合
        for (auto &w : words) {
            oddL += w.length() % 2;
            for (char c : w) {
                mask ^= 1 << (c - 'a');
            }
        }

        ranges::sort(words, [](const auto &a, const auto &b) {
            return a.length() > b.length();
        });

        int ans = words.size();
        int left = __builtin_popcount(mask) - oddL; // S 中的剩余字母个数
        for (auto &w : words) {
            if (left <= 0) break;
            left -= w.length() / 2 * 2;
            ans--;
        }
        return ans;
    }
};

作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-palindromes-after-operations/solutions/2637686/gou-zao-tan-xin-pythonjavacgo-by-endless-ib49/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

func maxPalindromesAfterOperations(words []string) int {
	oddL, mask := 0, 0
	for _, w := range words {
		oddL += len(w) % 2 // 统计奇数长度字符串个数
		for _, c := range w {
			mask ^= 1 << (c - 'a')
		}
	}

	slices.SortFunc(words, func(a, b string) int { return len(b) - len(a) })

	ans := len(words)
	left := bits.OnesCount(uint(mask)) - oddL // S 中的剩余字母个数
	for _, w := range words {
		if left <= 0 { break }
		left -= len(w) / 2 * 2
		ans--
	}
	return ans
}

作者：灵茶山艾府
链接：https://leetcode.cn/problems/maximum-palindromes-after-operations/solutions/2637686/gou-zao-tan-xin-pythonjavacgo-by-endless-ib49/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```
复杂度分析
时间复杂度：O(L+nlog⁡n)\mathcal{O}(L+n\log n)O(L+nlogn)，其中 LLL 为字符串的长度之和，nnn 为 words\textit{words}words 的长度。
空间复杂度：O(1)\mathcal{O}(1)O(1)。利用位运算可以做到 O(1)\mathcal{O}(1)O(1) 额外空间（忽略排序的栈开销）。
