Given two strings,write a method to decide if one is a permutation of the other.

Example 1:
```cpp
Input: s1 = "abc", s2 = "bca"
Output: true
```

Example 2:
```cpp
Input: s1 = "abc", s2 = "bad"
Output: false
```

Note:
- `0 <= len(s1) <= 100`
- `0 <= len(s2) <= 100`

题目：判断一个字符串是否是另一个字符串的某种排列。

---
思路1：使用额外的空间，哈希表对字符进行计数，然后比较即可。一个小的优化是：**如果两个字符串大小不同，就直接返回false** 。$\text{O(n)}$ 时间，$\text{O(n)}$ 空间或者说 $\text{O(1)}$ 空间，因为也是比较小的常量（逃

代码：
```cpp
class Solution {
public:
    bool CheckPermutation(string s1, string s2) { 
        if (s1.size() != s2.size()) return false;
        int checker[128] = {0}, checker2[128] = {0}, n = s1.size();
        for (int i = 0; i < n; ++i) ++checker[s1[i]];
        for (int i = 0; i < n; ++i) ++checker2[s2[i]];
        for (int i = 0; i < 128; ++i)
            if (checker[i] != checker2[i])
                return false;
        return true;
    }
};
```

---

思路2：对两个字符串进行排序，然后比较。$\text{O(nlogn)}$ 时间。

代码：
```cpp
class Solution {
public:
    bool CheckPermutation(string s1, string s2) {
        if (s1.size() != s2.size()) return false;
        sort(s1.begin(), s1.end());
        sort(s2.begin(), s2.end());
        for (int i = 0; i < s1.size(); ++i) 
            if (s1[i] != s2[i]) return false;
        return true; 
    }
};
```
看讨论的时候，发现有一些 `fake` 算法，比如将两个字符串中的字符都异或的、比如每个字符串中的全部字符相加后判断是否相等的……只能说，LeetCode在这些题目上，用例太不严谨了。

---
同样，看书，发现仍然有一些没有仔细考虑到，也就是 `confirm some details with our interviewer` ：
- 排列比较是否是大小写敏感 `case sensitive` 的？比如 `God` 是 `dog` 的某种排列吗？
- 空格是有意义的吗？
- 同样，和上一个面试题一样，是 `ASCII` 字符串、`extended ASCII` 字符串，还是 `Unicode` 字符串？

这里，题目的假设是 `ASCII` 字符串，空格有意义，大小写敏感。

做法1：`Sort the strings` 。我也想到了，因为互为排列的字符串都有着同样的 `characters` 只是顺序不同。通过排序将它们的字符变成同样的顺序，然后比较即可。Java代码如下：
```java
class Solution {
    private String sort(String s) {
        char[] content = s.toCharArray();
        Arrays.sort(content);
        return new String(content);
    }
    public boolean CheckPermutation(String s1, String s2) {
        if (s1.length() != s2.length()) return false;
        return sort(s1).equals(sort(s2));
    }
}
```
做法2：判断两个字符串有同样的字符和字符数。Java代码如下：
```java
class Solution {
    public boolean CheckPermutation(String s1, String s2) {
        if (s1.length() != s2.length()) return false;
        int[] letters = new int[128];
        char[] s_array = s1.toCharArray();
        for (char c : s_array) letters[c]++;

        for (int i = 0; i < s2.length(); ++i) {
            int c = (int)s2.charAt(i);
            letters[c]--;
            if (letters[c] < 0) return false;
        }
        return true;
    }
}
```
