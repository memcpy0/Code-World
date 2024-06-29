You have a large text file containing words. Given any two words, find the shortest distance (in terms of number of words) between them in the file. If the operation will be repeated many times for the same file (but different pairs of words), can you optimize your solution?

**Example:** 
```swift
Input: words = ["I","am","a","student","from","a","university","in","a","city"], word1 = "a", word2 = "student"
Output: 1
```
**Note:** `words.length <= 100000`

题意：有个内含单词的超大文本文件，给定任意两个单词，找出在这个文件中这两个单词的最短距离(相隔单词数)。

如果多次在这个文本文件中调用这个函数，而每次寻找的单词不同，你能对此优化吗？

---
### 解法 双指针
遍历一遍单词表，然后每次遇到 `word1` 和 `word2` 就更新对应的下标，接着求两个下标之间的距离，尝试更新全局最小值即可。
```cpp
class Solution {
public:
    int findClosest(vector<string>& words, string word1, string word2) {
        int n = words.size(), i = -1, j = -1, minDist = INT_MAX;
        for (int cur = 0; cur < n; ++cur) {
            if (words[cur] == word1) i = cur;
            else if (words[cur] == word2) j = cur;
            if (i != -1 && j != -1) minDist = min(minDist, abs(i - j));
        }
        return minDist;
    }
};
```
时间复杂度 $O(n)$ ，运行效率如下：
```cpp
执行用时：200 ms, 在所有 C++ 提交中击败了83.47% 的用户
内存消耗：58.5 MB, 在所有 C++ 提交中击败了32.20% 的用户
```
