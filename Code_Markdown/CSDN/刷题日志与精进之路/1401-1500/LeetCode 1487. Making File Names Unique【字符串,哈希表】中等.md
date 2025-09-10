> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an array of strings `names` of size `n`. You will create `n` folders in your file system **such that**, at the `ith` minute, you will create a folder with the name `names[i]`.

Since two files **cannot** have the same name, if you enter a folder name that was previously used, the system will have a suffix addition to its name in the form of `(k)`, where, `k` is the **smallest positive integer** such that the obtained name remains unique.

Return _an array of strings of length_ `n` where `ans[i]` is the actual name the system will assign to the `ith` folder when you create it.

**Example 1:**
```java
Input: names = ["pes","fifa","gta","pes(2019)"]
Output: ["pes","fifa","gta","pes(2019)"]
Explanation: Let's see how the file system creates folder names:
"pes" --> not assigned before, remains "pes"
"fifa" --> not assigned before, remains "fifa"
"gta" --> not assigned before, remains "gta"
"pes(2019)" --> not assigned before, remains "pes(2019)"
```
**Example 2:**
```java
Input: names = ["gta","gta(1)","gta","avalon"]
Output: ["gta","gta(1)","gta(2)","avalon"]
Explanation: Let's see how the file system creates folder names:
"gta" --> not assigned before, remains "gta"
"gta(1)" --> not assigned before, remains "gta(1)"
"gta" --> the name is reserved, system adds (k), since "gta(1)" is also reserved, systems put k = 2. it becomes "gta(2)"
"avalon" --> not assigned before, remains "avalon"
```
**Example 3:**
```java
Input: names = ["onepiece","onepiece(1)","onepiece(2)","onepiece(3)","onepiece"]
Output: ["onepiece","onepiece(1)","onepiece(2)","onepiece(3)","onepiece(4)"]
Explanation: When the last folder is created, the smallest positive valid k is 4, and it becomes "onepiece(4)".
```
**Constraints:**
-   `1 <= names.length <= 5 * 104`
-   `1 <= names[i].length <= 20`
-   `names[i]` consists of lowercase English letters, digits, and/or round brackets.

题意：给定一个长度为 `n` 的字符串数组 `names` ，你在你的文件系统中依次创建文件夹 `names[i]` 。由于**两个文件不能同名**，如果你输入了一个已经存在的名字，系统会给该名字后面加上一个后缀 `(k)` ，`k` 是使文件名字唯一的**最小正整数**。

---
### 解法 模拟+哈希表+字符串
一开始想复杂了，现在连想得多么复杂都不愿想起来了。总之，这个题目就是模拟文件系统的命名机制，给出一个文件名 `names[i]` ，如果没有出现过，则直接命名为 `names[i]` ；如果出现过，则从 `(1)` 开始给 `names[i]` 添加后缀 `(k)` ，直到找到没有出现过的文件名 `"names[i](k)"` ，这的 `k` 就是使文件名字唯一的**最小正整数**。

多说无益，看看几个示例：
- 对于 `["pes","fifa","gta","pes(2019)"]` ，其中每个名字在之前都没出现过，所以输出就是 `["pes","fifa","gta","pes(2019)"]` 。
- 对于 `["gta","gta(1)","gta","avalon"]` ，`"gta", "gta(1)"` 在之前都没出现过，直接加入答案数组；后面的 `"gta"` 则在前面出现过，所以先加后缀命名为 `"gta(1)"` ，发现也出现过，于是命名为 `"gta(2)"` ，没有出现过，直接加入答案数组。输出就是 `["gta","gta(1)","gta(2)","avalon"]` 。
- 对于 `["gta","gta(1)","gta","gta(2)"]` ，前三个都是一样，到第四个 `"gta(2)"` 时，发现它出现过，和第三个 `"gta"` 重新命名后的名字相同，那就需要在 `"gta(2)"` 后面加后缀，先变为 `"gta(2)(1)"` ，发现没有出现过，就直接加入答案数组。

看了这几个示例，代码写起来也很简单了：**使用哈希表 `rec` 存储所有第一次出现（包括「添加后缀之后首次出现」的字符串）的字符串、和后面的同名字符串将要使用的数字（一开始是1）**。每次从 `names` 拿到一个新名字 `names[i]` 时：
1. 先判断 `rec` 中是否存在，若不存在，说明之前没出现过，可直接用作文件名，此时 `rec[names[i]] = 1` ；
2. 否则，设 `s = names[i]` ，然后不断从 `rec` 中取出 `names[i]` 已经使用的数字 `k` 作为后缀拼在 `s` 后面，`++rec[names[i]` 。如果这一名字也已在 `rec` 中，就继续循环这一步，直到得到新的文件名，并设置 `rec[s] = 1` 。
- 时间复杂度：$O(n)$ 。如果都是不同字符串，则全部名字都可直接用，遍历一遍 `names` 数组即可；如果都是相同字符串，则要依次取 `rec[names[i]]` 、添加新的后缀、`++rec[names[i]]` 。
- 空间复杂度：$O(n)$ 
```java
class Solution {
    public String[] getFolderNames(String[] names) {
        String[] ans = new String[names.length];
        var rec = new HashMap<String, Integer>();
        for (int i = 0; i < names.length; ++i) {
            String s = names[i];
            while (rec.containsKey(s) ) {
                int suffix = rec.get(names[i]);
                s = names[i] + "(" + suffix + ")";
                rec.put(names[i], suffix + 1);
            }
            rec.put(s, 1);
            ans[i] = s;
        }
        return ans;
    }
}
```