Given a list of sorted characters `letters` containing only lowercase letters, and given a target letter target, find the smallest element in the list that is larger than the given target.

Letters also wrap around. For example, if the target is `target = 'z'` and `letters = ['a', 'b']` , the answer is `'a'` . 

题意：给出一个升序字符列表 `letters` ，列表中只包含小写英文字母。另给出一个目标字母 `target` ，寻找在有序列表里比目标字母大的最小字母。

---
### 思路1 使用STL
注意，如果目标字母大于等于有序字符列表中的最大字母，需要返回第一个字母。代码如下：
```cpp
class Solution {
public:
    char nextGreatestLetter(vector<char>& letters, char target) {
        auto it = upper_bound(letters.begin(), letters.end(), target);
        if (it == letters.end()) return letters.front();
        return *it;
    }
};
```
效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了94.90% 的用户
内存消耗：6.7 MB, 在所有 C++ 提交中击败了11.99% 的用户
```
---
### 思路2 手写二分
```cpp
class Solution {
public:
    char nextGreatestLetter(vector<char>& letters, char target) {
        int lo = 0, hi = letters.size();
        while (lo < hi) {
            int mid = lo + ((hi - lo) >> 1);
            if (letters[mid] > target) 
                hi = mid;
            else //letters[mid] <= target
                lo = mid + 1;
        }
        if (lo == letters.size()) return letters[0];
        return letters[lo];
    }
};
```
效率如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了61.64% 的用户
内存消耗：6.5 MB, 在所有 C++ 提交中击败了74.91% 的用户
```
