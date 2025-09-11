A string `S` of lowercase English letters is given. We want to partition this string into as many parts as possible so that each letter appears in at most one part, and return a list of integers representing the size of these parts.

 

**Example 1:**

```swift
Input: S = "ababcbacadefegdehijhklij"
Output: [9,7,8]
Explanation:
The partition is "ababcbaca", "defegde", "hijhklij".
This is a partition so that each letter appears in at most one part.
A partition like "ababcbacadefegde", "hijhklij" is incorrect, because it splits S into less parts.
```

 

**Note:**
- `S` will have length in range `[1, 500]` .
- `S` will consist of lowercase English letters (`'a'` to `'z'`) only.

题意：给出一个小写英文字母的字符串 `S` 。我们希望将这个字符串尽可能多地划分成多个部分，以便**每个字母**最多出现在一个部分中，并返回一个表示这些部分大小的整数列表。

---
### 解法 贪心
这个题目有点意思，它希望把字符串分成尽可能多个部分，使得每个字母最多出现在其中一个部分，比如 `ababcbacadefegdehijhklij` 分成 `ababcbaca, defegde, hijhklij` 三个子串，第一个子串包含所有的 `a,b,c` ，第二个子串包含所有的 `d,e,f,g` ，第三个子串包含所有的 `h,i,j,k,l` 。

很容易想到贪心，我们要分出下一部分时，**尽可能贪心地选择包含第一个字母的最小子串分区**——比如对 `abaccbdeffed` ，先贪心选择第一个字母 `a` ，它最后出现的位置是 `2` ，因此需要包含 `ba` 来扩展这一最小分区，于是得到 `aba` ；但是 `b` 最后出现的位置是 `5` ，因此需要接着包含 `ccb` 来扩展这一分区；此时原串中的所有字母 `a,b,c` 都出现在这一子串分区中，这样就得到了第一个分区 `abaccb` ，长度为 `6` 。接着继续这一步骤。

具体代码如下：
```cpp
class Solution {
public:
    vector<int> partitionLabels(string s) {
        vector<int> lenlist;
        int charset[26] = {0};
        for (int i = 0; i < s.size(); ++i) charset[s[i] - 'a'] = i;
        for (int i = 0; i < s.size(); ) { 
            int nextPos = charset[s[i] - 'a'];
            for (int j = i + 1; j <= nextPos; ++j) 
                nextPos = max(nextPos, charset[s[j] - 'a']);
            lenlist.push_back(nextPos - i + 1);
            i = nextPos + 1;
        }
        return lenlist;
    }
};
```
提交结果如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.6 MB, 在所有 C++ 提交中击败了67.34% 的用户
```
