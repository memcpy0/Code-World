Given a string `S` and a character `C`, return an array of integers representing the shortest distance from the character `C` in the string.

**Example 1:**
```cpp
Input: S = "loveleetcode", C = 'e'
Output: [3, 2, 1, 0, 1, 0, 0, 1, 2, 2, 1, 0]
```
**Note:**
- `S` string length is in `[1, 10000]` .
- `C` is a single character, and guaranteed to be in string `S` .
- All letters in `S` and `C` are lowercase.

题意：给定一个字符串 `S` 和一个字符 `C` 。返回一个代表字符串 `S` 中每个字符到字符串 `S` 中的字符 `C` 的最短距离的数组。

---
### 解法 往两边遍历
对 `S` 中的每个字符，都从它往两边遍历，得到左向和右向离 `C` 的字符数，比较得到最短距离。具体代码如下：
```cpp
class Solution {
public:
    vector<int> shortestToChar(string S, char C) {
        vector<int> ans;
        int n = S.size(), l, r;
        for (int i = 0; i < n; ++i) {
            if (S[i] == C) { ans.push_back(0); continue; }
            for (l = i - 1; l >= 0; --l) if (S[l] == C) break;
            for (r = i + 1; r < n; ++r) if (S[r] == C) break;
            ans.push_back(l == -1 ? (r - i) : (r == n ? (i - l) : min(i - l, r - i)));
        }
        return ans;
    }
};
```
运行效率如下：
```css
执行用时：4 ms, 在所有 C++ 提交中击败了67.88% 的用户
内存消耗：6.6 MB, 在所有 C++ 提交中击败了97.19% 的用户
```
