> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091733966.png)

<p>Given an array of strings <code>strs</code>, group <strong>the anagrams</strong> together. You can return the answer in <strong>any order</strong>.</p>

<p>An <strong>Anagram</strong> is a word or phrase formed by rearranging the letters of a different word or phrase, typically using all the original letters exactly once.</p>

 
<p><strong>Example 1:</strong></p>

```haskell
Input: strs = ["eat","tea","tan","ate","nat","bat"]
Output: [["bat"],["nat","tan"],["ate","eat","tea"]]
```

 
</pre><p><strong>Example 2:</strong></p>

```haskell
Input: strs = [""]
Output: [[""]] 
```

</pre><p><strong>Example 3:</strong></p>

```haskell
Input: strs = ["a"]
Output: [["a"]]
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= strs.length &lt;= 10<sup>4</sup></code></li>
	<li><code>0 &lt;= strs[i].length &lt;= 100</code></li>
	<li><code>strs[i]</code> consists of lower-case English letters.</li>
</ul>

题意：给定一个字符串数组，将字母异位词组合在一起。字母异位词指字母相同，但排列不同的字符串。

---
### 解法 哈希表+排序
使用一个字符串到整数的哈希映射，其中的字符串指的是**字典序最小形式**，整数是**答案数组的下标**。如果当前字符串的**最小形式**（字符串中的字符从小到大排序）出现过，则将该字符串存入那一行；否则分配**新的下标**给这个最小形式（及其字母异位词序列），同时存入相应位置。
```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        if (strs.empty()) return {};
        unordered_map<string, int> rec;
        vector<vector<string>> ans;
        int idx = 0;
        for (const string &s : strs) {
            string t = s;
            sort(t.begin(), t.end()); 		//最小形式
            if (rec.find(t) == rec.end()) { //这种字母异位词没出现过
                rec[t] = idx++; 			//记录最新的行号
                ans.push_back(vector<string>()); //分配最新的字符串数组
                ans[idx - 1].push_back(s); //用来存储这一类字母异位词
            } else ans[rec[t]].push_back(s);
        }
        return ans;
    }
};
```
提交后运行效率如下：
```cpp
执行用时：56 ms, 在所有 C++ 提交中击败了95.30% 的用户
内存消耗：18.2 MB, 在所有 C++ 提交中击败了91.55% 的用户
```
或者写成如下形式，感觉像是一个以**字符串最小形式**为索引的字母异位词邻接表：
```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        if (strs.empty()) return {};
        unordered_map<string, vector<string>> rec;
        vector<vector<string>> ans;
        for (const string &s : strs) {
            string key = s;
            sort(key.begin(), key.end());
            rec[key].push_back(s);
        }
        for (auto it = rec.begin(); it != rec.end(); ++it)
            ans.push_back(it->second);
        return ans;
    }
};
```
提交后运行效率如下：
```cpp
执行用时：68 ms, 在所有 C++ 提交中击败了79.59% 的用户
内存消耗：19.5 MB, 在所有 C++ 提交中击败了43.16% 的用户
```
