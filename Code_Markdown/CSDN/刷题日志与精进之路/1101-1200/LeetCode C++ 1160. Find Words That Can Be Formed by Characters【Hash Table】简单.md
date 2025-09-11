### 
You are given an array of strings words and a string chars.
A string is good if it can **be formed by characters from chars** (each character can **only be used once**).

Return **the sum of lengths** of all good strings in words.

**Example 1:**
```
Input: words = ["cat","bt","hat","tree"], chars = "atach"
Output: 6
Explanation: 
The strings that can be formed are "cat" and "hat" so the answer is 3 + 3 = 6.
```
**Example 2:**
```
Input: words = ["hello","world","leetcode"], chars = "welldonehoneyr"
Output: 10
Explanation: 
The strings that can be formed are "hello" and "world" so the answer is 5 + 5 = 10.
```
**Note:**
-  `1 <= words.length <= 1000`
-  `1 <= words[i].length, chars.length <= 100`
-  All strings contain lowercase English letters only.
	
题意：给出一个字符串数组和一个字母表 `chars` ，判断其中哪些字符串可以用 `chars` 中的字符（无重复使用）组成，然后求出这些字符串的长度和。

---
思路1：用一个 `unordered_map` ，建立 `chars` 中的每个字符和出现次数的映射，然后判断字符串数组中的字符串中的字符是否在 `unordered_map` 中出现，其出现次数是否超过 `chars` 中该字符的数量。 代码如下：
```c
class Solution {
public:
    int countCharacters(vector<string>& words, string chars) {
        unordered_map<char, int> dict, t;
        for (const char c : chars) ++dict[c];
        int lenSum = 0;
        for (const string &s : words) {
            t.clear();
            for (const char c : s) ++t[c];
            bool flag = true;
            for (auto &it : t) {
                if (dict[it.first] == 0 || dict[it.first] < t[it.first]) {
                    flag = false;
                    break;
                }
            }
            if (flag) lenSum += s.size();
        }
        return lenSum;
    }
};
```
时间效率比较低：
```cpp
执行用时：424 ms, 在所有 C++ 提交中击败了37.95% 的用户
内存消耗：35.9 MB, 在所有 C++ 提交中击败了50.55% 的用户
```

同样用哈希，但是用整数数组来映射。然后每次建立其复制数组 `temp` ，当字符串的字符在 `chars` 中出现的时候 `temp[]` 中该字符相对应的数量-1。 
```c
class Solution {
public:
    int countCharacters(vector<string>& words, string chars) {
        int dict[30] = {0}, lenSum = 0; 
        for (const char c : chars) ++dict[c - 'a'];
        for (const string &s : words) {
            int temp[30] = {0};
            memcpy(temp, dict, sizeof(dict));
            bool flag = true;
            for (const char c : s) {
                if (temp[c - 'a'] <= 0) {
                    flag = false;
                    break;
                }
                --temp[c - 'a'];
            }
            if (flag) lenSum += s.size();
        }
        return lenSum;
    }
};
```
效率如下：
```cpp
执行用时：92 ms, 在所有 C++ 提交中击败了90.89% 的用户
内存消耗：14.2 MB, 在所有 C++ 提交中击败了97.34% 的用户
```
同样的用Java来运行：
```java
class Solution {
    public int countCharacters(String[] words, String chars) {
        int[] dict = new int[30];
        int lenSum = 0;
        for (char c : chars.toCharArray()) ++dict[c - 'a'];  
        for (String s : words) {
            int[] temp = Arrays.copyOf(dict, dict.length); 
            boolean flag = true;
            for (char c : s.toCharArray()) {
                if (temp[c - 'a'] <= 0) {
                    flag = false;
                    break;
                }
                --temp[c - 'a'];
            }
            if (flag) lenSum += s.length(); 
        }
        return lenSum;
    }
}
```
效率如下：
```java
执行用时：6 ms, 在所有 Java 提交中击败了94.94% 的用户
内存消耗：40.2 MB, 在所有 Java 提交中击败了68.82% 的用户
```
---
思路2：进一步优化。如果字符串数组中的字符串长度 `> chars.length()` ，**不用比较直接跳过**。另外，一边遍历目标字符串一边比较，**不用复制数组**——如果当前字符出现的次数大于 `chars` 中出现的该字符次数，就 `break` 。用C++这样写，似乎没有太多变化。所以下面的代码是Java：
```cpp
class Solution {
    public int countCharacters(String[] words, String chars) {
        int[] dict = new int[26];
        int lenSum = 0;
        for (char c : chars.toCharArray()) ++dict[c - 'a'];  
        for (String s : words) { 
            if (s.length() > chars.length()) continue;
            boolean flag = true;
            int[] temp = new int[26];
            for (char c : s.toCharArray()) {
                ++temp[c - 'a'];
                if (temp[c - 'a'] > dict[c - 'a']) {
                    flag = false;
                    break;
                }
            }
            if (flag) lenSum += s.length(); 
        }
        return lenSum;
    }
}
```
效率如下：
```java
执行用时：4 ms, 在所有 Java 提交中击败了98.33% 的用户
内存消耗：40.5 MB, 在所有 Java 提交中击败了22.25% 的用户
```
