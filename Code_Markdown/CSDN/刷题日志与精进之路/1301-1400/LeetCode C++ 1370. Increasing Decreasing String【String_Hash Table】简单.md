Given a string `s`. You should re-order the string using the following algorithm:
1. Pick the **smallest** character from `s` and **append** it to the result.
 2. Pick the **smallest** character from `s` which is greater than the last appended character to the result and **append** it.
 3. Repeat step 2 until you cannot pick more characters.
  4. Pick the **largest** character from `s` and **append** it to the result.
  5. Pick the **largest** character from `s` which is smaller than the last appended character to the result and **append** it.
  6. Repeat step 5 until you cannot pick more characters.
 7. Repeat the steps from 1 to 6 until you pick all characters from `s`.

In each step, If the smallest or the largest character appears more than once you can choose any occurrence and append it to the result.

Return *the result string* after sorting `s` with this algorithm.

 

**Example 1:**

```swift
Input: s = "aaaabbbbcccc"
Output: "abccbaabccba"
Explanation: After steps 1, 2 and 3 of the first iteration, result = "abc"
After steps 4, 5 and 6 of the first iteration, result = "abccba"
First iteration is done. Now s = "aabbcc" and we go back to step 1
After steps 1, 2 and 3 of the second iteration, result = "abccbaabc"
After steps 4, 5 and 6 of the second iteration, result = "abccbaabccba"
```

**Example 2:**

```swift
Input: s = "rat"
Output: "art"
Explanation: The word "rat" becomes "art" after re-ordering it with the mentioned algorithm.
```

**Example 3:**

```swift
Input: s = "leetcode"
Output: "cdelotee"
```

**Example 4:**

```swift
Input: s = "ggggggg"
Output: "ggggggg"
```

**Example 5:**

```swift
Input: s = "spo"
Output: "ops"
```

 

**Constraints:**
-  `1 <= s.length <= 500`
 - `s` contains only lower-case English letters.

题意：给出一个字符串 `s` ，返回将 `s` 中字符重新排序后的结果字符串 。


---
### 解法 哈希计数
计算每个小写字母的频数。然后循环所有字符，从 `'a'` 到 `'z'` ，如果该字符存在则添加，同时减少其频数；接着从 `'z'` 到 `'a'` ，做同样的工作。直至所有字母的频数归零。
 
```cpp
class Solution {
public:
    string sortString(string s) {
        int cnt[26] = {0};
        for (const char &c : s) ++cnt[c - 'a'];
        int n = s.size();
        string ans;
        while (n) {
            for (int i = 0; i < 26; ++i) {
                if (cnt[i]) {
                    --cnt[i];
                    --n;
                    ans.push_back(i + 'a');
                }
            }
            for (int i = 25; i >= 0; --i) {
                if (cnt[i]) {
                    --cnt[i];
                    --n;
                    ans.push_back(i + 'a');
                }
            }
        }
        return ans;
    }
};
```
执行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了97.13% 的用户
内存消耗：7.7 MB, 在所有 C++ 提交中击败了45.07% 的用户
```

