> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的字符串 `word` 和一个整数 `k` 。

在每一秒，你必须执行以下操作：

- 移除 `word` 的前 `k` 个字符。
- 在 `word` 的末尾添加 `k` 个任意字符。

**注意** 添加的字符不必和移除的字符相同。但是，必须在每一秒钟都执行 **两种** 操作。

返回将 `word` 恢复到其 **初始** 状态所需的 **最短** 时间（该时间必须大于零）。

**示例 1：**

**输入：**word = "abacaba", k = 3
**输出：**2
**解释：**
第 1 秒，移除 word 的前缀 "aba"，并在末尾添加 "bac" 。因此，word 变为 "cababac"。
第 2 秒，移除 word 的前缀 "cab"，并在末尾添加 "aba" 。因此，word 变为 "abacaba" 并恢复到始状态。
可以证明，2 秒是 word 恢复到其初始状态所需的最短时间。

**示例 2：**

**输入：**word = "abacaba", k = 4
**输出：**1
**解释：**
第 1 秒，移除 word 的前缀 "abac"，并在末尾添加 "caba" 。因此，word 变为 "abacaba" 并恢复到初始状态。
可以证明，1 秒是 word 恢复到其初始状态所需的最短时间。

**示例 3：**

**输入：**word = "abcbabcd", k = 2
**输出：**4
**解释：**
每一秒，我们都移除 word 的前 2 个字符，并在 word 末尾添加相同的字符。
4 秒后，word 变为 "abcbabcd" 并恢复到初始状态。
可以证明，4 秒是 word 恢复到其初始状态所需的最短时间。

**提示：**

- `1 <= word.length <= 50`
- `1 <= k <= word.length`
- `word`仅由小写英文字母组成。




---
### 【视频讲解】Z 函数（扩展 KMP）
请看 视频讲解。包含 Z 函数（扩展 KMP）的理论讲解。

可视化

下文将 word\textit{word}word 简记为 sss。

如果只操作一次，就能让 sss 恢复成其初始值，意味着什么？

由于可以往 sss 的末尾添加任意字符，这意味着只要 s[k:]s[k:]s[k:] 是 sss 的前缀，就能让 sss 恢复成其初始值，其中 s[k:]s[k:]s[k:] 表示从 s[k]s[k]s[k] 开始的后缀。

例如示例 2 的 s=abacaba, k=4s=\text{abacaba},\ k=4s=abacaba, k=4，由于后缀 s[4:]=abas[4:]=\text{aba}s[4:]=aba 是 sss 的前缀，所以只需操作一次。

如果操作一次不行，我们就看 s[2k:]s[2k:]s[2k:] 是否为 sss 的前缀。依此类推。

如果任意非空 s[xk:]s[xk:]s[xk:]（x>0x>0x>0）都不是 sss 的前缀（例如示例 3），那么只能操作 ⌈nk⌉\left\lceil\dfrac{n}{k}\right\rceil⌈ 
k
n
​
 ⌉ 次，把 sss 的字符全部删除，由于可以添加任意字符，我们可以直接生成一个新的 sss。

我们通过计算 sss 后缀与 sss 的 LCP（最长公共前缀）长度，即 Z 函数（扩展 KMP）来判断，如果 LCP 长度大于等于后缀长度，就说明对应操作可以让 sss 恢复成其初始值。
```cpp
class Solution:
    def minimumTimeToInitialState(self, s: str, k: int) -> int:
        n = len(s)
        z = [0] * n
        l = r = 0
        for i in range(1, n):
            if i <= r:
                z[i] = min(z[i - l], r - i + 1)
            while i + z[i] < n and s[z[i]] == s[i + z[i]]:
                l, r = i, i + z[i]
                z[i] += 1
            if i % k == 0 and z[i] >= n - i:
                return i // k
        return (n - 1) // k + 1

作者：灵茶山艾府
链接：https://leetcode.cn/problems/minimum-time-to-revert-word-to-initial-state-ii/solutions/2630932/z-han-shu-kuo-zhan-kmp-by-endlesscheng-w44j/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
public:
    int minimumTimeToInitialState(string s, int k) {
        int n = s.size();
        vector<int> z(n);
        int l = 0, r = 0;
        for (int i = 1; i < n; i++) {
            if (i <= r) {
                z[i] = min(z[i - l], r - i + 1);
            }
            while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
                l = i;
                r = i + z[i];
                z[i]++;
            }
            if (i % k == 0 && z[i] >= n - i) {
                return i / k;
            }
        }
        return (n - 1) / k + 1;
    }
};

作者：灵茶山艾府
链接：https://leetcode.cn/problems/minimum-time-to-revert-word-to-initial-state-ii/solutions/2630932/z-han-shu-kuo-zhan-kmp-by-endlesscheng-w44j/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
    public int minimumTimeToInitialState(String S, int k) {
        char[] s = S.toCharArray();
        int n = s.length;
        int[] z = new int[n];
        int l = 0, r = 0;
        for (int i = 1; i < n; i++) {
            if (i <= r) {
                z[i] = Math.min(z[i - l], r - i + 1);
            }
            while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
                l = i;
                r = i + z[i];
                z[i]++;
            }
            if (i % k == 0 && z[i] >= n - i) {
                return i / k;
            }
        }
        return (n - 1) / k + 1;
    }
}

作者：灵茶山艾府
链接：https://leetcode.cn/problems/minimum-time-to-revert-word-to-initial-state-ii/solutions/2630932/z-han-shu-kuo-zhan-kmp-by-endlesscheng-w44j/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

func minimumTimeToInitialState(s string, k int) int {
	n := len(s)
	z := make([]int, n)
	for i, l, r := 1, 0, 0; i < n; i++ {
		if i <= r {
			z[i] = min(z[i-l], r-i+1)
		}
		for i+z[i] < n && s[z[i]] == s[i+z[i]] {
			l, r = i, i+z[i]
			z[i]++
		}
		if i%k == 0 && z[i] >= n-i {
			return i / k
		}
	}
	return (n-1)/k + 1
}

作者：灵茶山艾府
链接：https://leetcode.cn/problems/minimum-time-to-revert-word-to-initial-state-ii/solutions/2630932/z-han-shu-kuo-zhan-kmp-by-endlesscheng-w44j/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

```

直接KMP就行

```csharp
class Solution {
    int kmp(string &s, int k) {
        int l = s.size();
        vector<int> pi(l);
        for (int i = 1, j = 0; i < l; i += 1) {
            j = pi[i - 1];
            while (j > 0 && s[i] != s[j]) {
                j = pi[j - 1];
            }
            if (s[i] == s[j]) {
                pi[i] = j + 1;
            } else {
                pi[i] = j;
            }
        }
        int res = pi.back();
        while (res > 0 && (l - res) % k != 0) {
            res = pi[res - 1];
        }
        return (l - res - 1) / k + 1;
    }
public:
    int minimumTimeToInitialState(string word, int k) {
        return kmp(word, k);
    }
};
```

KMP变种，k个k个分段，把k个字母看成一个字母，可以写个比较函数

```csharp
class Solution {
    string s;
    int k;
    int l;
    bool cmp(int i, int j) {
        for (int c = 0; i < l && j < l && c < k; i++, j++, c++) {
            if (s[i] != s[j]) {
                return false;
            }
        }
        return true;
    }
    int kmp() {
        l = s.size();
        vector<int> pi((l - 1) / k + 1);
        for (int i = k, j = 0; i < l; i += k) {
            j = pi[i / k - 1];
            while (j > 0 && !cmp(i, j * k)) {
                j = pi[j - 1];
            }
            if (cmp(i, j * k)) {
                pi[i / k] = j + 1;
            } else {
                pi[i / k] = j;
            }
        }
        return (l - 1) / k + 1 - pi.back();
    }
public:
    int minimumTimeToInitialState(string word, int k) {
        this->k = k;
        s.swap(word);
        return kmp();
    }
};
```

4

回复