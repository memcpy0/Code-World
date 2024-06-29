> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


Given a string `licensePlate` and an array of strings `words` , find the **shortest completing** word in `words` .

A **completing word** is a word that **contains all the letters** in `licensePlate` . **Ignore numbers and spaces** in `licensePlate` , and treat letters as **case insensitive**. If a letter appears more than once in `licensePlate` , then it must appear in the word the same number of times or more.

For example, if `licensePlate = "aBc 12c"` , then it contains letters `'a', 'b'` (ignoring case), and `'c'` twice. Possible **completing words** are `"abccdef"` , `"caaacab"` , and `"cbca"` .

Return ***the shortest completing word*** in `words` . It is guaranteed an answer exists. If there are multiple shortest **completing words**, return the first one that occurs in `words` .

**Example 1:**

```swift
Input: licensePlate = "1s3 PSt", words = ["step","steps","stripe","stepple"]
Output: "steps"
Explanation: licensePlate contains letters 's', 'p', 's' (ignoring case), and 't'.
"step" contains 't' and 'p', but only contains 1 's'.
"steps" contains 't', 'p', and both 's' characters.
"stripe" is missing an 's'.
"stepple" is missing an 's'.
Since "steps" is the only word containing all the letters, that is the answer.
```

**Example 2:**

```swift
Input: licensePlate = "1s3 456", words = ["looks","pest","stew","show"]
Output: "pest"
Explanation: licensePlate only contains the letter 's'. All the words contain 's', but among these "pest", "stew", and "show" are shortest. The answer is "pest" because it is the word that appears earliest of the 3.
```

**Example 3:**

```swift
Input: licensePlate = "Ah71752", words = ["suggest","letter","of","husband","easy","education","drug","prevent","writer","old"]
Output: "husband"
```

**Example 4:**

```swift
Input: licensePlate = "OgEu755", words = ["enough","these","play","wide","wonder","box","arrive","money","tax","thus"]
Output: "enough"
```

**Example 5:**

```swift
Input: licensePlate = "iMSlpe4", words = ["claim","consumer","student","camera","public","never","wonder","simple","thought","use"]
Output: "simple" 
```

**Constraints:**
- `1 <= licensePlate.length <= 7`
-  `licensePlate` contains digits, letters (uppercase or lowercase), or space `' '` .
- `1 <= words.length <= 1000`
- `1 <= words[i].length <= 15`
- `words[i]` consists of lower case English letters.

题意：给定一个字符串牌照 `licensePlate` 和一个字符串数组 `words` ，请你找出并返回 `words` 中的 最短补全词 。如果单词列表 `words` 中的一个单词包含牌照 `licensePlate` 中所有的字母，那么称之为**补全词**。在所有最短的补全词中，返回单词列表中最靠前的一个。
 
---
### 解法 哈希表+模拟
根据题意，先对 `licensePlate` 进行词频统计（忽略空格和数字），然后遍历 `words` 进行词频统计，从所有符合要求的 `words[i]` 中寻找最短的补全词。

设 $n$ 为字符串 `licensePlate` 的长度，$m$ 为 `words[]` 的长度，$L$ 为所有 `words[i]` 长度的总和，字符集大小为 $C = 26$ ，则整体时间复杂度为 $O(n + \displaystyle \sum^{m - 1}_{i = 0} (C + words[i].length))$ 。空间复杂度为 $O(C)$ ：
```cpp
//C++ version
class Solution {
private:
    int scnt[26], cnt[26];
    bool check(const string &s) {
        memset(scnt, 0, sizeof(scnt));
        for (const char &c : s) ++scnt[c - 'a'];
        for (int i = 0; i < 26; ++i) 
            if (cnt[i] > scnt[i]) return false;
        return true; 
    }
public:
    string shortestCompletingWord(string licensePlate, vector<string>& words) {
        for (const char &c : licensePlate) 
            if (isalpha(c)) ++cnt[tolower(c) - 'a'];
        int minLen = INT_MAX, idx = 0;
        for (int i = 0, wordLen = words.size(); i < wordLen; ++i) {
            if (check(words[i])) { //检查是否补全词
                if (words[i].size() < minLen) {
                    minLen = words[i].size();
                    idx = i;
                }
            }
        }
        return words[idx];
    }
};
//执行用时：12 ms, 在所有 C++ 提交中击败了99.11% 的用户
//内存消耗：11.4 MB, 在所有 C++ 提交中击败了87.56% 的用户
```
由于本人决定转向Java方向，所以Java的写法也不能落下：
```java
//Jave version
class Solution {
    public String shortestCompletingWord(String licensePlate, String[] words) {
        int[] cnt = getCnt(licensePlate);
        int idx = -1;
        for (int i = 0, n = words.length; i < n; ++i) {
            int[] scnt = getCnt(words[i]);
            boolean flag = true;
            for (int j = 0; j < 26 && flag; ++j)
                if (cnt[j] > scnt[j]) flag = false;
            if (flag && (idx == -1 || words[idx].length() > words[i].length())) idx = i;
        }
        return words[idx];
    }
    int[] getCnt(String s) {
        int[] cnt = new int[26];
        for (int i = 0, n = s.length(); i < n; ++i) 
            if (Character.isLetter(s.charAt(i))) 
                ++cnt[Character.toLowerCase(s.charAt(i)) - 'a'];
        return cnt;
    }
}
//执行用时：4 ms, 在所有 Java 提交中击败了59.71% 的用户
//内存消耗：38.8 MB, 在所有 Java 提交中击败了42.63% 的用户
```
