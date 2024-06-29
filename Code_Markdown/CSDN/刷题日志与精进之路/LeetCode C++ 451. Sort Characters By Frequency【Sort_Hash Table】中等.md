Given a string, sort it in decreasing order based on the frequency of characters.

**Example 1:**

```cpp
Input:
"tree"

Output:
"eert"

Explanation:
'e' appears twice while 'r' and 't' both appear once.
So 'e' must appear before both 'r' and 't'. Therefore "eetr" is also a valid answer.
```

**Example 2:**

```cpp
Input:
"cccaaa"

Output:
"cccaaa"

Explanation:
Both 'c' and 'a' appear three times, so "aaaccc" is also a valid answer.
Note that "cacaca" is incorrect, as the same characters must be together.
```

**Example 3:**
```cpp
Input:
"Aabb"

Output:
"bbAa"

Explanation:
"bbaA" is also a valid answer, but "Aabb" is incorrect.
Note that 'A' and 'a' are treated as two different characters.
```
题意：对字符串 `s` 中的字符按照出现频率降序排序。

---
### 解法 哈希表+排序
思路一：用 `unordered_map` 记录次数，然后按照出现次数对 `s` 中的字符排序，最后返回 `s` 。注意：数据要求**同一个字符排在一起**，比如：
```java
Input:
"loveleetcode"
Output:
"eeeeoollvtdc"
Explanation: 
"eeeelolovtcd" is incorrect.
```
所以，我们将**频率相同的字符按照字典序从小到大进行排序**。代码如下：
```cpp
class Solution {
public:
    string frequencySort(string s) {
        if (s.size() <= 1) return s;
        unordered_map<char, int> dict;
        for (const char &c : s) ++dict[c];
        sort(s.begin(), s.end(), [&](const char &a, const char &b) {
            int an = dict[a], bn = dict[b];
            return (an != bn ? an > bn : a < b);
        });
        return s;
    }
};
```
可耻的效率，就不写出来了。**可能**是由于在 `sort` 排序使用比较函数时，**多次重复查询 `unordered_map` 带来的效率损失**。为此，可以将 `unordered_map` 中的字符和对应的频率记录到 `vector` 中，排序时直接取出。

---
思路2：`unordered_map` 记录次数，然后导入到 `vector<pair<char, int>>` ，对 `vector` 的元素按照频率排序，然后拼接字符串。代码如下：
```cpp
class Solution {
public: 
    string frequencySort(string s) {
        if (s.size() <= 1) return s;
        unordered_map<char, int> timeDic; 
        for (const char &c : s) ++timeDic[c]; 
        vector<pair<char, int>> temp(timeDic.begin(), timeDic.end()); //导入vector
        sort(temp.begin(), temp.end(), [&](const pair<char, int> &a, const pair<char, int> &b) {
            return a.second == b.second ? a.first < b.first : a.second > b.second;
        });  
        string ans;
        for (const auto &i : temp)  //拼接答案
            ans += string(i.second, i.first);
        return ans;
    }
};
```
令人惊讶的效率提升：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了72.94% 的用户
内存消耗：8.9 MB, 在所有 C++ 提交中击败了28.94% 的用户
```
进一步优化，按照字符集的大小使用 `pair<char, int>` 数组，进行计数排序：
```cpp
class Solution {
public: 
    string frequencySort(string s) {
        if (s.size() <= 1) return s;
        pair<char, int> timeDic[128];
        for (int i = 0; i < 128; ++i) timeDic[i].first = (char)i, timeDic[i].second = 0;
        for (const char &c : s) ++timeDic[c].second; 
        sort(timeDic, timeDic + 128, [&](const pair<char, int> &a, const pair<char, int> &b) {
            return a.second > b.second;
        });
        string ans;
        for (int i = 0; i < 128; ++i)  //拼接答案
            ans += string(timeDic[i].second, timeDic[i].first);
        return ans;
    }
};
```
更高的效率：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：8.9 MB, 在所有 C++ 提交中击败了25.81% 的用户
```
---
如果我们在这里，不进行字符串拼接，而是直接在原串上进行排序：
```cpp
class Solution {
public: 
    string frequencySort(string s) {
        if (s.size() <= 1) return s;
        pair<char, int> timeDic[128];
        for (int i = 0; i < 128; ++i) timeDic[i].first = (char)i, timeDic[i].second = 0;
        for (const char &c : s) ++timeDic[c].second; 
        sort(s.begin(), s.end(), [&](const char &a, const char &b) {
            return timeDic[a].second == timeDic[b].second ? a < b : timeDic[a].second > timeDic[b].second;
        }); 
        return s;
    }
};
```
效率仍然很低，说明**真正的效率差距**在于，对统计出的字符及其频数进行排序 $O(128 \log 128)$ ，和对字符串 `s` 中的字符、按照字符字典序和哈希表中字符出现次数进行排序 $O(slen \log slen)$ 之间的效率差距：
```cpp
执行用时：100 ms, 在所有 C++ 提交中击败了11.14% 的用户
内存消耗：8.1 MB, 在所有 C++ 提交中击败了93.23% 的用户
```
