
Given two words (***beginWord*** and ***endWord***), and a dictionary's word list, find the length of shortest transformation sequence from ***beginWord*** to ***endWord***, such that:
1. Only one letter can be changed at a time.
2. Each transformed word must exist in the word list.

**Note:**
-  **Return 0** if there is no such transformation sequence.
-  All words have **the same length.**
 - All words contain only **lowercase alphabetic characters**.
- You may assume **no duplicates** in the word list.
- You may assume ***beginWord*** and ***endWord*** are **non-empt**y and are **not the same**.

**Example 1:**

```swift
Input:
beginWord = "hit",
endWord = "cog",
wordList = ["hot","dot","dog","lot","log","cog"]

Output: 5

Explanation: As one shortest transformation is "hit" -> "hot" -> "dot" -> "dog" -> "cog",
return its length 5.
```

**Example 2:**

```swift
Input:
beginWord = "hit"
endWord = "cog"
wordList = ["hot","dot","dog","lot","log"]

Output: 0

Explanation: The endWord "cog" is not in wordList, therefore no possible transformation.
```
题意：给定两个单词（ `beginWord` 和 `endWord` ）和一个字典，找到从 `beginWord` 到 `endWord` 的最短转换序列的长度。转换需遵循如下规则：
1. 每次转换只能改变一个字母。
2. 转换过程中的中间单词必须是字典中的单词。

说明:
- 如果不存在这样的转换序列，返回 `0` 。
- 所有单词具有相同的长度。
- 所有单词只由小写字母组成。
 - 字典中不存在重复的单词。
-  `beginWord` 和 `endWord` 是非空的，且二者不相同。


---
### 解法1 BFS
这道题实际上就是求**无向无权图中两点间的最短路径长度**，每个单词视作一个节点，只相差一个字符的单词节点间存在一条无向无权路径（或者是权重为 $1$）。

使用DFS会超时，说明测试用例的数量级比较大。如果使用BFS：
1. 将第一个单词节点加入队列；
2. 从队列中取出本层的单词节点，然后对其进行变换，如果存在于词典中，就加入到队列中；
3. 到达 `endWord` 或者队列为空时结束。

具体代码如下：
- 注意将 `wordList` 转换为哈希集合 `wordDict` 以**去重**和**提高查找效率**；
- 更重要的是，**判断两个单词是否只差一位字符**这种方式会耗时很久，每一位都要比较，每一轮都要比较很多次。**由于只存在小写字母，每次只变动一位**，所以直接对每个当前节点，**生成所有可能的下一个节点**，如果存在于 `wordDict` 中就入队；
- 还有注意，一个已达的单词节点可能会有其他节点也能到达，为此需要在入队单词节点后删除 `wordDict` 中的对应单词，**避免单词节点重复入队，提高算法效率**，这样也能防止环形遍历的出现。
- 另外，`beginWord` 可以不存在于词典中。
```cpp
class Solution { 
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> wordDict(wordList.begin(), wordList.end()); //去重; 便于查找
        if (wordDict.find(endWord) == wordDict.end()) return 0; //Not Found
        queue<string> q;
        q.push(beginWord);
        int step = 0;
        while (!q.empty()) {
            ++step;
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                string s = q.front(); q.pop();
                if (s == endWord) return step; //到达终止节点
                for (int j = 0; j < s.size(); ++j) { //对单词的每一位字符
                    string str = s;
                    for (char c = 'a'; c <= 'z'; ++c) { //都经过a-z的变换
                        if (str[j] == c) continue; //不同字符时进行以下步骤
                        str[j] = c;
                        if (wordDict.find(str) != wordDict.end()) {
                            q.push(str); //存在时入队  
                            wordDict.erase(str); //避免重复环路搜索; 避免重复入队,提高算法效率
                        }
                    }
                }
            } 
        }
        return 0; //不存在这样一条路径
    }
};
```
提交后效率如下，勉勉强强：
```cpp
执行用时：156 ms, 在所有 C++ 提交中击败了79.83% 的用户
内存消耗：11 MB, 在所有 C++ 提交中击败了79.69% 的用户
```
---
### 解法2 双向广搜
既然我们可以从 `beginWord` 正向搜索 `endWord` ，那么也可以从 `endWord` 反向搜索 `beginWord` ，这两者复杂度差别不大。然而，如果我们同时从两端向中间搜索，复杂度会大大降低。

一个示例：假设从 `beginWord` 转换为 `endWord` 时，第 `i` 层的队列，存在于字典中的中间单词节点有 `30` 个；而从 `endWord` 转换为 `beginWord` ，第 `k` 层的队列，存在于字典中的中间单词节点有 `3` 个。显然，从 `endWord` 开始会更快（和正向搜索相遇）。因此，**每次都从中间结果少的那一端出发**，能够剪去许多不必要的搜索分支。

```cpp
class Solution { 
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> wordDict(wordList.begin(), wordList.end()); //去重; 便于查找
        if (wordDict.find(endWord) == wordDict.end()) return 0; //Not Found
        unordered_set<string> beginSet{beginWord}, endSet{endWord}; //分别从开始端、结束端向中间搜索; 用哈希集合以去重
        int step = 1; //第一层单词已经包含
        while (!beginSet.empty()) {
            unordered_set<string> tempSet;
            ++step; //每搜索完下一层单词节点,+1
            for (auto s : beginSet) wordDict.erase(s); //删除已经搜索过的单词节点,避免重复入队; 避免环状搜索路径; //必须在这里删除而非插入后删除
            for (const string &s : beginSet) {
                for (int i = 0; i < s.size(); ++i) {
                    string str = s;
                    for (char c = 'a'; c <= 'z'; ++c) {
                        str[i] = c;
                        if (wordDict.find(str) == wordDict.end()) continue; //不存在这样的单词
                        if (endSet.find(str) != endSet.end()) return step; //在中间相遇
                        tempSet.insert(str);
                    }
                }
            }
            if (tempSet.size() < endSet.size()) beginSet = tempSet; //保证每次都从中间结果少的那一端向中间遍历
            else { beginSet = endSet; endSet = tempSet; }
        }
        return 0; //不存在这样一条路径
    }
}; 
```
效率如下，很高了：
```cpp
执行用时：36 ms, 在所有 C++ 提交中击败了99.45% 的用户
内存消耗：11.1 MB, 在所有 C++ 提交中击败了73.08% 的用户
```
