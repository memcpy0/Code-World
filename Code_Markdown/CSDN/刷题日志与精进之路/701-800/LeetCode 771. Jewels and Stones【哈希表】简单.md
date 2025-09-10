> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You're given strings `J` representing the types of stones that are jewels, and `S` representing the stones you have.  Each character in `S` is a type of stone you have.  You want to know how many of the stones you have are also jewels.

The letters in `J` are guaranteed distinct, and all characters in `J` and `S` are letters. Letters are case sensitive, so `"a"` is considered a different type of stone from `"A"` .

**Example 1:**
```swift
Input: J = "aA", S = "aAAbbbb"
Output: 3
```
**Example 2:**
```swift
Input: J = "z", S = "ZZ"
Output: 0
```

**Note:**
-  `S` and `J` will consist of letters and have length at most `50` .
- The characters in `J` are distinct.

题意：给定字符串 `J` 代表石头中宝石的类型，和字符串 `S` 代表拥有的石头。`S` 中每个字符代表了一种石头的类型，询问拥有的石头中有多少是宝石。`J` 中的字母不重复，`J` 和 `S` 中的所有字符都是字母。字母区分大小写，因此 `"a"` 和 `"A"` 是不同类型的石头。

---
### 解法1 暴力
暴力法的思路很直观，遍历字符串 $stones$ ，对于 $stones$ 中的每个字符，遍历一次字符串 $jewels$ ，如果其和 $\textit{jewels}$ 中的某一个字符相同，则是宝石。
```java
class Solution {
    public int numJewelsInStones(String jewels, String stones) {
        int jewelsCount = 0;
        int jewelsLength = jewels.length(), stonesLength = stones.length();
        for (int i = 0; i < stonesLength; i++) {
            char stone = stones.charAt(i);
            for (int j = 0; j < jewelsLength; j++) {
                char jewel = jewels.charAt(j);
                if (stone == jewel) {
                    ++jewelsCount;
                    break;
                }
            }
        }
        return jewelsCount;
    }
}
```
复杂度分析：
- 时间复杂度：$O(mn)$ ，其中 $m$ 是字符串 $jewels$ 的长度，$n$ 是字符串 $stones$ 的长度。遍历字符串 $stones$ 的时间复杂度是 $O(n)$ ，对于 $stones$ 中的每个字符，需要遍历字符串 $jewels$ 判断是否是宝石，时间复杂度是 $O(m)$，因此总时间复杂度是 $O(mn)$ 。
- 空间复杂度：$O(1)$ 。只需要维护常量的额外空间。

---
### 解法2 哈希集合
方法一中，对于字符串 $stones$ 中的每个字符，都需要遍历一次字符串 $jewels$，导致时间复杂度较高。如果使用哈希集合存储字符串 $jewels$ 中的宝石，则可以降低判断的时间复杂度。

遍历字符串 $jewels$，使用哈希集合存储其中的字符，然后遍历字符串 $stones$，对于其中的每个字符，如果其在哈希集合中，则是宝石。
```java
class Solution {
    public int numJewelsInStones(String jewels, String stones) {
        int jewelsCount = 0;
        Set<Character> jewelsSet = new HashSet<Character>();
        int jewelsLength = jewels.length(), stonesLength = stones.length();
        for (int i = 0; i < jewelsLength; i++) {
            char jewel = jewels.charAt(i);
            jewelsSet.add(jewel);
        }
        for (int i = 0; i < stonesLength; i++) {
            char stone = stones.charAt(i);
            if (jewelsSet.contains(stone)) {
                ++jewelsCount;
            }
        }
        return jewelsCount;
    }
}
```
复杂度分析：
- 时间复杂度：$O(m+n)$ ，其中 $m$ 是字符串 $jewels$ 的长度，$n$ 是字符串 $stones$ 的长度。遍历字符串 $jewels$ 将其中的字符存储到哈希集合中，时间复杂度是 $O(m)$，然后遍历字符串 $stones$，对于 $stones$ 中的每个字符在 $O(1)$ 的时间内判断当前字符是否是宝石，时间复杂度是 $O(n)$，因此总时间复杂度是 $O(m+n)$ 。
- 空间复杂度：$O(m)$，其中 $m$ 是字符串 $jewels$ 的长度。使用哈希集合存储字符串 $jewels$ 中的字符。

---
### 解法3 大小字母状态压缩
观察 ASCII 表，可以发现：
- 大写字母二进制的低 $6$ 位是从 $000 001$ 开始的（对应大写字母 $A$ ），一直到 $011010$（对应大写字母 Z）。
- 小写字母二进制的低 $6$ 位是从 $100001$ 开始的（对应小写字母 $a$ ），一直到 $111010$（对应小写字母 $z$ ），即十进制的 $58$ 。

所以取字符的的低 $6$ 位，就可以把不同的大小写字母映射到不同的数字上。所以，可以用一个 $64$ 位整数 $\textit{mask}$ 来代替哈希表/布尔数组，压缩存储 $jewels$ 中的字母。然后遍历 $stones$ ，判断每个字母是否在 $\textit{mask}$ 中。
```java
class Solution {
    public int numJewelsInStones(String jewels, String stones) {
        long mask = 0; // 把 jewels 转换成字符集合 mask
        for (char c : jewels.toCharArray()) mask |= 1L << (c & 63);
        int ans = 0; // 统计有多少 stones[i] 在集合 mask 中
        for (char c : stones.toCharArray())
            ans += mask >> (c & 63) & 1;
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n+m)$ ，其中 $n$ 为 $jewels$ 的长度，mmm 为 $stones$ 的长度。
- 空间复杂度：$\mathcal{O}(1)$ 。仅用到若干额外变量。
