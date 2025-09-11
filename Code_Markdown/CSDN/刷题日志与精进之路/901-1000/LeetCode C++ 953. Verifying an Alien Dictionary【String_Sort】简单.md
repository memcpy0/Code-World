
In an alien language, surprisingly they also use english lowercase letters, but possibly in a different `order`. The `order` of the alphabet is some permutation of lowercase letters.

Given a sequence of `words` written in the alien language, and the `order` of the alphabet, return `true` if and only if the given `words` are sorted lexicographicaly in this alien language.

 

**Example 1:**

```swift
Input: words = ["hello","leetcode"], order = "hlabcdefgijkmnopqrstuvwxyz"
Output: true
Explanation: As 'h' comes before 'l' in this language, then the sequence is sorted.
```

**Example 2:**

```swift
Input: words = ["word","world","row"], order = "worldabcefghijkmnpqstuvxyz"
Output: false
Explanation: As 'd' comes after 'l' in this language, then words[0] > words[1], hence the sequence is unsorted.
```

**Example 3:**

```swift
Input: words = ["apple","app"], order = "abcdefghijklmnopqrstuvwxyz"
Output: false
Explanation: The first three characters "app" match, and the second string is shorter (in size.) According to lexicographical rules "apple" > "app", because 'l' > '∅', where '∅' is defined as the blank character which is less than any other character (More info).
```

 

**Constraints:**

-  `1 <= words.length <= 100`
 - `1 <= words[i].length <= 20`
-  `order.length == 26`
-  All characters in `words[i]` and `order` are English lowercase letters.

题意：某种外星语的字母表也使用英文小写字母，但可能顺序 `order` 不同——顺序是一些小写字母的排列。给定一组用外星语书写的单词 `words`，以及其字母表的顺序 `order`，只有当给定的单词在这种外星语中**按字典序排列**时，返回 `true`；否则，返回 `false`。


---
### 解法 比较函数
本题要求判断给定单词列表是否**按外星字典序从小到大排列**，由于可能存在完全相同的字符串，因此我们实现比较函数时，使用 `<=` 语义：
```cpp
class Solution {
private:
    int charToInt[26];
    bool cmp(const string &a, const string &b) { //a <= b -> true; a > b -> return false
        int alen = a.size(), blen = b.size(), len = min(alen, blen);
        for (int i = 0; i < len; ++i) {
            int ca = a[i] - 'a', cb = b[i] - 'a';
            if (charToInt[ca] < charToInt[cb]) return true; //a < b
            else if (charToInt[ca] > charToInt[cb]) return false; //a > b
        }
        return alen <= blen; //alen <= blen --> a <= b --> true , alen > blen --> a > b --> false
    }
public:
    bool isAlienSorted(vector<string>& words, string order) {
        int n = order.size(), m = words.size();
        for (int i = 0; i < n; ++i) charToInt[order[i] - 'a'] = i; 
        for (int i = 0; i < m - 1; ++i) //判断是否按照外星字典序从小到大排序
            if (cmp(words[i], words[i + 1]) == false)
                return false;
        return true;
    }
};
```
提交后效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了86.25% 的用户
内存消耗：9.7 MB, 在所有 C++ 提交中击败了32.38% 的用户
```
