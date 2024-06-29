> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定两个字符串 `s` 和 `t` ，编写一个函数来判断 `t` 是否是 `s` 的字母异位词。

**注意**：若 `s` 和 `t` 中每个字符出现的次数都相同，则称 `s` 和 `t` 互为字母异位词。

**示例 1:**
```java
输入: s = "anagram", t = "nagaram"
输出: true
```
**示例 2:**
```java
输入: s = "rat", t = "car"
输出: false
```
**提示:**
- `1 <= s.length, t.length <= 5 * 10^4`
- `s` 和 `t` 仅包含小写字母

**进阶:** 如果输入字符串包含 `unicode` 字符怎么办？你能否调整你的解法来应对这种情况？

---
### 解法1 排序
$t$ 是 $s$ 的异位词等价于「两个字符串排序后相等」。因此我们可以对字符串 $s$ 和 $t$ 分别排序，看排序后的字符串是否相等即可判断。此外，如果 $s$ 和 $t$ 的长度不同，$t$ 必然不是 $s$ 的异位词。
```cpp
// cpp
class Solution {
public:
    bool isAnagram(string s, string t) {
        sort(s.begin(), s.end());
        sort(t.begin(), t.end());
        return s == t;
    }
};
// java
class Solution {
    public boolean isAnagram(String s, String t) {
        if (s.length() != t.length()) return false;
        char[] s1 = s.toCharArray();
        char[] s2 = t.toCharArray();
        Arrays.sort(s1);
        Arrays.sort(s2);
        return Arrays.equals(s1, s2);
    }
}
// go
func isAnagram(s, t string) bool {
    s1, s2 := []byte(s), []byte(t)
    sort.Slice(s1, func(i, j int) bool { return s1[i] < s1[j] })
    sort.Slice(s2, func(i, j int) bool { return s2[i] < s2[j] })
    return string(s1) == string(s2)
}
```
复杂度分析：
- 时间复杂度：$O(n \log n)$ ，其中 $n$ 为 $s$ 的长度。排序的时间复杂度为 $O(n\log n)$ ，比较两个字符串是否相等时间复杂度为 $O(n)$ ，因此总体时间复杂度为 $O(n \log n+n)=O(n\log n)$ 。
- 空间复杂度：$O(\log n)$ 。排序需要 $O(\log n)$ 的空间复杂度。注意，在某些语言（比如 Java & JavaScript）中字符串是不可变的，因此我们需要额外的 $O(n)$ 的空间来拷贝字符串。但是我们忽略这一复杂度分析，因为：
    - 这依赖于语言的细节；
    - 这取决于函数的设计方式，例如，可以将函数参数类型更改为 `char[]` 。

---
### 解法2 哈希计数
从另一个角度考虑，$t$ 是 $s$ 的异位词等价于「两个字符串中字符出现的种类和次数均相等」。用两个整数数组，进行字符计数，然后比较。代码如下：
```cpp
// cpp
class Solution {
public:
    bool isAnagram(string s, string t) {
        // 考虑字符集和空串
        int sset[26] = {0}, tset[26] = {0};
        for (const char &c : s) ++sset[c - 'a'];
        for (const char &c : t) ++tset[c - 'a'];
        for (int i = 0; i < 26; ++i)
            if (sset[i] != tset[i]) return false;
        return true;
    }
};
```
进一步优化，我们可以**只维护**一个长度为 $26$ 的频次数组 $cnt$ ，先遍历记录字符串 $s$ 中字符出现的频次，**同时遍历记录**字符串 $t$ 并减去对应的频次。最后检查 $cnt$ 数组，如果出现 $cnt[i]<0$ ，则返回 $false$ ：
```cpp
// cpp
class Solution {
public:
    bool isAnagram(string s, string t) {
        //优化:长度不等时直接返回
        if (s.size() != t.size()) return false;
        int cnt[26] = {0};
        for (int i = 0, n = s.size(); i < n; ++i) {
            ++cnt[s[i] - 'a'];
            --cnt[t[i] - 'a'];
        }
        return all_of(cnt, cnt + 26, [](int x) { return x >= 0; });
    }
};
// python
class Solution:
    def isAnagram(self, s: str, t: str) -> bool:
        if len(s) != len(t):
            return False
        cnt = [0] * 26
        for c in s:
            cnt[ord(c) - ord('a')] += 1
        for c in t:
            cnt[ord(c) - ord('a')] -= 1
            if cnt[ord(c) - ord('a')] < 0:
                return False
        return True
// go
func isAnagram(s string, t string) bool {
    if len(s) != len(t) {
        return false
    }
    cnt := [26]int{}
    for i := range s { // 一次遍历
        cnt[s[i] - 'a']++
        cnt[t[i] - 'a']--
    }
    for _, v := range cnt {
        if v != 0 {
            return false
        }
    }
    return true
}
```
对于进阶问题，$\text{Unicode}$ 是为了解决传统字符编码的局限性而产生的方案，**它为每个语言中的字符规定了一个唯一的二进制编码**。而 $Unicode$ 中可能存在**一个字符对应多个字节**的问题，==为了让计算机知道多少字节表示一个字符，面向传输的编码方式的 $\text{UTF}-8$ 和 $\text{UTF}-16$ 也随之诞生逐渐广泛使用== 。

回到本题，进阶问题的核心点在于「字符是离散未知的」，因此我们用哈希表维护对应字符的频次即可。同时需要注意 $\text{Unicode}$ 一个字符可能对应多个字节的问题，不同语言对于字符串读取处理的方式是不同的：
```java
// java
class Solution {
    public boolean isAnagram(String s, String t) {
        if (s.length() != t.length()) return false;
        Map<Character, Integer> cnt = new HashMap<>();
        for (int i = 0; i < s.length(); ++i) {
            char c = s.charAt(i);
            cnt.put(c, cnt.getOrDefault(c, 0) + 1);
        }
        for (int i = 0; i < t.length(); ++i) {
            char c = t.charAt(i);
            cnt.put(c, cnt.getOrDefault(c, 0) - 1);
            if (cnt.get(c) < 0) return false; 
            // cnt为s中字符的频次,现在为负,说明不为异位词
        }
        return true;
    }
}
// python: 使用defaultdict
class Solution:
    def isAnagram(self, s: str, t: str) -> bool:
        if len(s) != len(t):
            return False
        from collections import defaultdict
        cnt = defaultdict(int)
        for c in s:
            cnt[c] += 1
        for c in t:
            cnt[c] -= 1
            if cnt[c] < 0:
                return False
        return True
// go
func isAnagram(s string, t string) bool {
    if len(s) != len(t) {
        return false
    }
    cnt := map[rune]int{} // rune
    for _, c := range s { // 一次遍历
        cnt[c]++
    }
    for _, c := range t {
        cnt[c]--
        if cnt[c] < 0 {
            return false
        }
    }
    return true
}
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为 $s$ 的长度。
- 空间复杂度：$O(S)$ ，其中 $S$ 为字符集大小，此处 $S=26$ 。
