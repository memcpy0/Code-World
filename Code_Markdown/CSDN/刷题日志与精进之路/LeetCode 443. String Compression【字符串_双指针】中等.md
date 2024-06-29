> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>Given an array of characters <code>chars</code>, compress it using the following algorithm:</p>

<p>Begin with an empty string <code>s</code>. For each group of <strong>consecutive repeating characters</strong> in <code>chars</code>:</p>

<ul>
	<li>If the group's length is 1, append the character to&nbsp;<code>s</code>.</li>
	<li>Otherwise, append the character followed by the group's length.</li>
</ul>

<p>The compressed string&nbsp;<code>s</code> <strong>should not be returned separately</strong>, but instead be stored&nbsp;<strong>in the input character array&nbsp;<code>chars</code></strong>. Note that group lengths that are 10 or longer will be split into multiple characters in&nbsp;<code>chars</code>.</p>

<p>After you are done <b>modifying the input array</b>, return <em>the new length of the array</em>.</p>

You must write an algorithm that uses only constant extra space.


<p><strong>Example 1:</strong></p>

```bash
Input: chars = ["a","a","b","b","c","c","c"]
Output: Return 6, and the first 6 characters of the input array should be: ["a","2","b","2","c","3"]
Explanation: The groups are "aa", "bb", and "ccc". This compresses to "a2b2c3".
```

<p><strong>Example 2:</strong></p>

```bash
Input: chars = ["a"]
Output: Return 1, and the first character of the input array should be: ["a"]
Explanation: The only group is "a", which remains uncompressed since it's a single character.
```
<p><strong>Example 3:</strong></p>

```bash
Input: chars = ["a","b","b","b","b","b","b","b","b","b","b","b","b"]
Output: Return 4, and the first 4 characters of the input array should be: ["a","b","1","2"].
Explanation: The groups are "a" and "bbbbbbbbbbbb". This compresses to "ab12". 
```
  

<p><strong>Example 4:</strong></p>

```bash
Input: chars = ["a","a","a","b","b","a","a"]
Output: Return 6, and the first 6 characters of the input array should be: ["a","3","b","2","a","2"].
Explanation: The groups are "aaa", "bb", and "aa". This compresses to "a3b2a2". Note that each group is independent even if two groups have the same character.
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= chars.length &lt;= 2000</code></li>
	<li><code>chars[i]</code> is a lower-case English letter, upper-case English letter, digit, or symbol.</li>
</ul>


题意：给你一个字符数组 <code>chars</code> ，用算法压缩，从一个空字符串 <code>s</code> 开始。对于 <code>chars</code> 中的每组 <strong>连续重复字符</strong> ：</p>
<ul>
	<li>如果这一组长度为 <code>1</code> ，则将字符追加到 <code>s</code> 中。</li>
	<li>否则，需要向 <code>s</code> 追加字符，后跟这一组的长度。</li>
</ul>

<p>压缩后得到的字符串 <code>s</code> <strong>不应该直接返回</strong> ，需要转储到字符数组 <code>chars</code> 中。需要注意的是，如果组长度为 <code>10</code> 或 <code>10</code> 以上，则在 <code>chars</code> 数组中会被拆分为多个字符。</p>

<p>请在 <strong>修改完输入数组后</strong> ，返回该数组的新长度。必须设计并实现一个只使用常量额外空间的算法来解决此问题。</p>


---
### 解法 双指针
这道题其实很简单，就是实现长程压缩算法：
- 用两个指针 `i, j` ，`i, j` 一开始指向连续相同字符 `chars[i]` 的起点，同时用 `ans = 0` 记录压缩后数组的长度；
- `j` 不断往后遍历，直到遇到不同字符 `char[j]` 或到了 `chars[]` 的末尾，此时这一组字符的长度 `len = j - i` ；
- 将字符 `chars[i]` 写入 `chars[ans++]` 中；
- 长度大于 `1` 时，先用 `l` 记录此时的位置 `ans` ，再顺序从个位到最高位将 `len` 写入 `chars[ans++]` ，然后将 `[l, ans - 1]` 区间的数字逆序；
- 最后 `i = j` ，继续下一轮循环。

这无疑是最自然的想法，时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ 。代码实现如下：
```cpp
class Solution {
public:
    int compress(vector<char>& chars) { 
        int ans = 0;
        for (int i = 0, n = chars.size(); i < n; ) {
            int j = i;
            while (j < n && chars[j] == chars[i]) ++j;
            int len = j - i;
            chars[ans++] = chars[i];
            if (len > 1) {
                int l = ans;
                while (len) {
                    chars[ans++] = '0' + len % 10;;
                    len /= 10;
                }
                int r = ans - 1;
                while (l < r) swap(chars[l++], chars[r--]);
            }
            i = j;
        }
        return ans;
    }
}; 
//执行用时：4 ms, 在所有 C++ 提交中击败了87.32% 的用户
//内存消耗：8.6 MB, 在所有 C++ 提交中击败了71.52% 的用户
```
上述算法在具体细节上还能稍微简化一下，**我们实际上不需要指向连续相同字符起点的指针**，下面的过程相当于把上述的 `j` 换成了 `i` ：
- 先用指针 `i` 指向连续相同字符 `chars[i]` 的起点，同时用 `ans = 0` 记录压缩后数组的长度；
- `i` 不断往后遍历，直到遇到不同字符 `char[i + 1]` 或到了 `chars[]` 的末尾，同时用 `len` 计数这一组字符的长度；
- 将字符 `chars[i]` 写入 `chars[ans++]` 中；
- 长度大于 `1` 时，先用 `l` 记录此时的位置 `ans` ，再顺序从个位到最高位将 `len` 写入 `chars[ans++]` ，然后将 `[l, ans - 1]` 区间的数字逆序；
- 令 `len = 0` ，继续 `i` 的遍历。

代码实现如下：
```cpp
class Solution {
public:
    int compress(vector<char>& chars) { 
        int ans = 0;
        for (int i = 0, len = 1, n = chars.size(); i < n; ++i, ++len) {
            if (i + 1 != n && chars[i] == chars[i + 1]) continue;
            chars[ans++] = chars[i];
            if (len > 1) {
                int l = ans;
                while (len) {
                    chars[ans++] = '0' + len % 10;
                    len /= 10;
                }
                int r = ans - 1;
                while (l < r) swap(chars[l++], chars[r--]);
            }
            len = 0;
        } 
        return ans;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：8.5 MB, 在所有 C++ 提交中击败了91.39% 的用户
```
