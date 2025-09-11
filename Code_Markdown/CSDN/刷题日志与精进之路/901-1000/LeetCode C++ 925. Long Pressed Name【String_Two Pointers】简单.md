z 
Your friend is typing his `name` into a keyboard.  Sometimes, when typing a character `c` , the key might get *long pressed*, and the character will be typed `1` or more times.

You examine the `typed` characters of the keyboard.  Return `True` if it is possible that it was your friends name, with some characters (possibly none) being long pressed.

 

**Example 1:**

```swift
Input: name = "alex", typed = "aaleex"
Output: true
Explanation: 'a' and 'e' in 'alex' were long pressed.
```

**Example 2:**

```swift
Input: name = "saeed", typed = "ssaaedd"
Output: false
Explanation: 'e' must have been pressed twice, but it wasn't in the typed output.
```

**Example 3:**

```swift
Input: name = "leelee", typed = "lleeelee"
Output: true
```

**Example 4:**

```swift
Input: name = "laiden", typed = "laiden"
Output: true
Explanation: It's not necessary to long press any character.
```

 

**Constraints:**
- `1 <= name.length <= 1000`
-  `1 <= typed.length <= 1000`
 - The characters of `name` and `typed` are lowercase letters.

题意：朋友正在使用键盘输入他的名字 `name` 。偶尔在键入字符时，按键可能会被长按，因此字符可能被输入 `1` 次或多次。本题要求检查键盘输入的字符 `typed` ，如果它对应的可能是朋友的名字（其中一些字符可能被长按），那么就返回 `True` 。

---
### 解法 双指针
一个小的剪枝是：如果 `name` 的长度大于 `typed` 的长度时，直接返回 `false` 。之后维护两个指针，比较指针指向的两个字符，如果不相等，则返回 `false` ；如果相等，就分别计算此处这个字符连续出现的长度，如果 `name` 中连续出现的长度大于 `typed` 中连续出现的长度，则不可能是长按导致的错误，返回 `false` ，否则继续往后遍历比较。具体代码如下：
```cpp
class Solution {
public:
    bool isLongPressedName(string name, string typed) {
        if (name.size() > typed.size()) return false; 
        int pn = 0, pt = 0, n = name.size(), m = typed.size();
        while (pn < n && pt < m) { 
            if (name[pn] != typed[pt]) return false;
            int i = pn + 1, j = pt + 1;
            while (i < n && name[i] == name[pn]) ++i;
            while (j < m && typed[j] == typed[pt]) ++j;
            if (i - pn > j - pt) return false; 
            pn = i;
            pt = j; 
        }
        return pn >= n && pt >= m;
    }
}; 
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了36.09% 的用户
```
