Given two strings, `s1` and `s2` , write code to check if `s2` is a rotation of s1 (e.g., `"waterbottle"` is a rotation of `"erbottlewat"` ). Can you use only one call to the method that checks if one word is a substring of another?

**Example 1:**
```clike
Input: s1 = "waterbottle", s2 = "erbottlewat"
Output: True
```
**Example 2:**
```clike
Input: s1 = "aa", s2 = "aba"
Output: False
```
**Note:** `0 <= s1.length, s2.length <= 100000`


题意：给定两个字符串 `s1` 和 `s2` ，检查 `s2` 是否为 `s1` 旋转而成。

---
### 解法1 找规律
如果想象 `s2` 是 `s1` 的旋转，那么可以问旋转点是什么。例如在 `wat` 之后旋转 `waterbottle` ，就得到了 `erbottlewat` 。我们把 `s1` 分成两部分，即 `s1 = xy, x = wat, y = erbottle` ，然后重新排列得到 `s2 = yx` 。

所以，我们需要检查是否有办法把 `s1` 分成 `x` 和 `y` ，这样 `xy = s1, yx = s2` 。仔细思考可以发现，在 `s2` 是 `s1` 的旋转的前提下，`yx` 总是 `xyxy` 的子串，即 `s2` 始终是 `s1s1` 的子串；或者 `yxyx = s2s2` ，于是 `s1` 始终是 `s2s2` 的子串。具体代码很简单：
```cpp
class Solution {
public:
    bool isFlipedString(string s1, string s2) {
        if (s1.size() != s2.size()) return false;
        string &&t = s2 + s2;
        return t.find(s1) != string::npos;
    }
};
```
提交后效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7.6 MB, 在所有 C++ 提交中击败了89.57% 的用户
```
不过这个算法的具体效率取决于 `find()` 的实现，如果它是 $O(n + m)$ 的话，那么 `isFlipedString()` 就是线性时间复杂度。
