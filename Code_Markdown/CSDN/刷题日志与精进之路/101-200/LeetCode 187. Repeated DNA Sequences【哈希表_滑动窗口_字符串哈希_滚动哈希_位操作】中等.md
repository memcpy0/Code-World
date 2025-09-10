> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>The <strong>DNA sequence</strong> is composed of a series of nucleotides abbreviated as <code>'A'</code>, <code>'C'</code>, <code>'G'</code>, and <code>'T'</code>.</p>

<ul>
	<li>For example, <code>"ACGAATTCCG"</code> is a <strong>DNA sequence</strong>.</li>
</ul>

<p>When studying <strong>DNA</strong>, it is useful to identify repeated sequences within the DNA.</p>

<p>Given a string <code>s</code> that represents a <strong>DNA sequence</strong>, return all the <strong><code>10</code>-letter-long</strong> sequences (substrings) that occur more than once in a DNA molecule. You may return the answer in <strong>any order</strong>.</p>

 
<p><strong>Example 1:</strong></p> 

```cpp
Input: s = "AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT"
Output: ["AAAAACCCCC","CCCCCAAAAA"]
```
 
<p><strong>Example 2:</strong></p> 

```cpp
Input: s = "AAAAAAAAAAAAA"
Output: ["AAAAAAAAAA"]
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= s.length &lt;= 10<sup>5</sup></code></li>
	<li><code>s[i]</code> is either <code>'A'</code>, <code>'C'</code>, <code>'G'</code>, or <code>'T'</code>.</li>
</ul>

题意：所有 DNA 都由一系列缩写为 <code>'A'</code>，<code>'C'</code>，<code>'G'</code> 和 <code>'T'</code> 的核苷酸组成，例如 <code>"ACGAATTCCG"</code>。在研究 DNA 时，识别 DNA 中的重复序列有时会对研究非常有帮助。编写一个函数来找出所有目标子串，目标子串的长度为 10，且在 DNA 字符串 <code>s</code> 中出现次数超过一次。
 
---
### 解法1 滑动窗口+哈希表
由于数据范围只有 $10^5$ 、目标子串长度为 $10$ ，一个朴素解法是：从左到右处理字符串 $s$ ，使用固定长度为 $10$ 的滑动窗口得到每个以 $s[i]$ 开头且长度为 $10$ 的子串，同时使用哈希表记录每个子串的出现次数，**如果该子串出现次数超过一次，则加入答案**——为了「**避免其后再次遍历哈希表**」（即只使用一次循环），同时「**防止在一次遍历中重复添加相同子串**」，我们规定，当且仅当该子串之前出现过一次、加上本次后出现次数为两次时，将该子串加入答案。

实际代码如下。由于每次要检查以 $s[i]$ 开头、长度为 $10$ 的字符串，时间复杂度为 $O(n  C)$ ，其中 $C = 10$ 。由于长度固定的子串数量不会超过 $n$ 个，所以空间复杂度为 $O(nC)$ ，其中 $C = 10$ 。
```cpp
//C++ version
class Solution {
public:
    vector<string> findRepeatedDnaSequences(string s) {
        unordered_map<string, int> rec;
        vector<string> ans;
        for (int i = 0, n = s.size(); i + 10 <= n; ++i) {
            string &&cur = s.substr(i, 10); // 使用右值引用避免不必要的拷贝赋值调用
            int &cnt = rec[cur]; //节省再次查找的时间
            if (cnt == 1) ans.push_back(cur);
            ++cnt;
        } 
        return ans;
    }
};
//执行用时：44 ms, 在所有 C++ 提交中击败了85.74% 的用户
//内存消耗：22.9 MB, 在所有 C++ 提交中击败了16.66% 的用户
```
---
### 解法2 字符串哈希+滚动哈希+滑动窗口+哈希表（优化解法1）
如果题目给定的子串长度大于 $100$ ，加上生成子串和哈希表本身的常数操作，解法1的计算量将超过 $10^7$ ，绝对会TLE。为了严格做到 $O(n)$ 时间，需要使用字符串哈希+**滚动哈希**对解法1进行**优化**。

具体来说，这里使用一个与字符串 $s$ 等长的哈希数组 $hash[]$ 和 次方数组 $p[]$ ，通过预处理字符串，我们先用 $O(n)$ 时间计算出哈希数组和次方数组。需要实际计算子串 $s[i...j]$ 的哈希值时，只需要计算 $hash[j] - hash[i - 1] * p[j - i + 1]$ ，即可在 $O(1)$ 时间内得出子串 $s[i...j]$ 的哈希值（**与子串长度无关**），接着用哈希表统计这一哈希值出现的次数……随后的做法和解法1完全一致。

可以看到，随着循环变量 $i$ 的不断增大，我们滚动向前计算以 $i$ 为起点、长度为 $10$ 的子串的哈希值，这一技巧称为「**滚动哈希**」，常与「**字符串哈希**」相关技巧一起使用。通过使用滚动哈希+字符串哈希，我们可以在 $O(1)$ 时间内计算出某个子串的哈希值，**让进行计数的哈希表不必以 `string` 作为 `key` ，而是使用 `int`（即哈希值结果本身）作为 `key`** ，从而避免了哈希表内部对「**重叠子串**」哈希值的重复计算，大大提高了算法的效率。

实际代码如下所示，时间复杂度为 $O(n)$ ，空间复杂度为 $O(n)$ 。
```cpp
//C++ version
class Solution {
private:
    static const int maxn = 1e5 + 10;
    unsigned P = 131313;
    unsigned hash[maxn] = {0}, p[maxn] = {1};
public:
    vector<string> findRepeatedDnaSequences(string s) {
        int n = s.size();
        vector<string> ans;
        for (int i = 1; i <= n; ++i) {
            hash[i] = hash[i - 1] * P + s[i - 1];
            p[i] = p[i - 1] * P;
        }
        unordered_map<unsigned, int> rec;
        for (int i = 1; i <= n - 10 + 1; ++i) {
            int j = i + 10 - 1; // [i,j]标志这一长度为10的子串区间
            unsigned seqHash = hash[j] - hash[i - 1] * p[j - i + 1]; // p[10]
            int &cnt = rec[seqHash];
            if (cnt == 1) ans.push_back(s.substr(i - 1, 10));
            ++cnt;
        }
        return ans;
    }
};
//执行用时：28 ms, 在所有 C++ 提交中击败了96.65% 的用户
//内存消耗：16 MB, 在所有 C++ 提交中击败了80.48% 的用户
```

这样的优化已经足够强大，不过由于本题中要计算哈希值的子串具有固定的长度，我们完全可以节省掉 $p[]$ 的计算，代之以变量 $p10$ ：
```cpp
//C++ version
class Solution {
private:
    static const int maxn = 1e5 + 10;
    unsigned P = 131313;
    unsigned hash[maxn] = {0}, p10 = 40769121;
public:
    vector<string> findRepeatedDnaSequences(string s) {
        int n = s.size();
        vector<string> ans;
        for (int i = 1; i <= n; ++i)
            hash[i] = hash[i - 1] * P + s[i - 1];
        unordered_map<unsigned, int> rec;
        for (int i = 1; i <= n - 10 + 1; ++i) {
            int j = i + 10 - 1; // [i,j]标志这一长度为10的子串区间
            unsigned seqHash = hash[j] - hash[i - 1] * p10;
            int &cnt = rec[seqHash];
            if (cnt == 1) ans.push_back(s.substr(i - 1, 10));
            ++cnt;
        }
        return ans;
    }
}; 
//执行用时：28 ms, 在所有 C++ 提交中击败了96.65% 的用户
//内存消耗：15.5 MB, 在所有 C++ 提交中击败了82.18% 的用户
```

### 拓展思考
 到这里，~~本题就做完了~~，只是有些疑问要解决。字符串哈希在「构造数组」和「计算哈希」的过程中，不会溢出吗？**肯定会溢出**！这就是为什么我使用的是 `unsigned int` 而非 `int` ，因为 `unsigned` 溢出后相当于对 $2^{32}$ 取模，C++不会报错。

使用哈希值代替该子串，在哈希表中查询出现次数时，不会出现哈希冲突吗？**可能会冲突**！两个 `unsigned` 类型的哈希值模 $2^{32}$ 同余时，就会产生错误结果（哈希冲突），此时要考虑**修改 $P$** 或者**使用表示范围更大的数据类型来替代 `unsigned`**（不治本） 。至于这个数字 $P = 131313$ 是如何产生的呢？首先 **$P$ 一定要是素数**，然后从 $131, 13131$ 一路尝试着WA过来得到的。关于质数表，可见[Wiki List of prime numbers](https://en.wikipedia.org/wiki/List_of_prime_numbers)。

---
### 解法3 位运算+滚动哈希+字符串哈希+滑动窗口+位图
其实以一般的眼光来看，解法2第二版已经高效到不能再高效了。不过算法的优化难有止境，这里有（比解法2）更高效的一种算法。当然，其本质也是对解法2的再度优化。

我们以挑刺的眼光看待解法2，发现有以下问题：
- 计算哈希值时要多次使用乘法，而乘法比较慢；
- 我们不需要统计子串（或哈希值）出现的次数，只需要知道它是否出现过两次，因此浪费了一定的空间；
- 哈希冲突的可能性仍然存在，只是由于LeetCode的测试样例不够多，才能AC
- ……

其实，对 $A, C, G, T$ 四个字符可用 $0, 1, 2, 3$ 即二进制的 $00, 01, 10, 11$ 来表示，于是长度为 $10$ 的字符串可用 $20$ 位的二进制数表示。相对解法2，此解法的优势在于：
- 所有可能的字符组合，都能一一对应到 $[0, 2^{20}) = [0,  1048 576)$ ，二者之间是**一一映射**，**不存在溢出或冲突的问题**
- 使用位图 `bitset` 来存储，只需要 $32768$ 个 `int` ；由于要确定至少出现过两次，就需要两个 `bitset` 即 $65 536$ 个 `int` 。比起解法2来说，**使用空间大幅减少**，因为对哈希数组、次方数组和哈希表，全代之以两个 `bitset` 
- 滚动计算二进制数，**使用位运算，比起乘法运算效率更高**

算法的流程也是一致的，从左到右遍历字符串 $s$ ，使用固定长度为 $10$ 的滑动窗口，滚动计算每个以 $s[i]$ 结尾且长度为 $10$ 的子串对应的二进制数，接着用位图表示这一二进制数出现的次数……随后的做法和解法1、2几乎一致。

```cpp
//C++ version
class Solution {
private: // 对应二进制数{00,01,10,11},那么长度为10的子串只要20位即可表示
    unordered_map<char, int> rec = {{'A', 0}, {'C', 1}, {'G', 2}, {'T', 3}};
public:
    vector<string> findRepeatedDnaSequences(string s) {
        if (s.size() < 10) return {};
        vector<string> ans;
        bitset<1 << 20> s1, s2; // 所有可能子串的二进制数值在0到(1 << 20 - 1)之间
        int n = s.size(), val = 0, mask = (1 << 20) - 1; // mask为二进制数的20个1
        // 使用滑动窗口+滚动计算,先把前10个字母的二进制数计算出来
        for (int i = 0; i < 10; ++i) val = (val << 2) | rec[s[i]];
        s1.set(val); // 置位,表示前10个字母组成的子串已经出现
        for (int i = 10; i < n; ++i) {
            val = ((val << 2) & mask) | rec[s[i]]; // 去掉左移的一个字符,再加上一个新字符
            if (s2.test(val)) continue; // 出现过两次则跳过
            if (s1.test(val)) { // 出现了一次
                ans.push_back(s.substr(i - 9, 10));
                s2.set(val); // 表示出现了第二次
            } else s1.set(val); // 表示出现了一次
        }
        return ans;
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了99.82% 的用户
//内存消耗：7.4 MB, 在所有 C++ 提交中击败了98.33% 的用户
```
可以看到，虽然和解法2一样都是 $O(n)$ 时空复杂度的算法，但是二者的常数差距较大，且解法3的效率比较稳定。
