Implement an algorithm to determine if a string has all unique characters. What if you cannot use additional data structures?

Example 1:
```cpp
Input: s = "leetcode"
Output: false
```

Example 2:

```clike
Input: s = "abc"
Output: true
```
**Note**: 如果你不使用额外的数据结构，会很加分。


题意：实现一个算法，确认一个字符串是否所有字符都不相同。

---
思路1：哈希表。

代码：
```cpp
class Solution {
public:
    bool isUnique(string astr) {
        bool f[130] = {false};
        for (int i = 0; i < astr.size(); ++i) {
            if (f[astr[i]]) return false;
            f[astr[i]] = true;
        }
        return true;
    }
};
```

---
思路2：排序，不依赖额外的数据结构。

代码：
```cpp
class Solution {
public:
    bool isUnique(string astr) {
        if (astr.empty()) return true;
        sort(astr.begin(), astr.end());
        for (size_t i = 0; i < astr.size() - 1; ++i) {
            if (astr[i] == astr[i + 1]) return false;
        }
        return true;
    }
};
```

思路3：对字符串中的每个字符，都和其他所有字符进行比较，花费 $\text{O(n}^2)$ 时间。

代码就不写了。

---
看书，发现我考虑的还不够。因为这是面试题。

我们应该首先询问面试官，字符串是 `ASCII` 字符串还是 `Unicode` 字符串，或者是 `extended ASCII` 字符串；一般假设是 `ASCII` 字符。如果不是 `ASCII` 字符，我们的存储容量就要变化。

于是，方法是创建一个大小为 `128/256` 的 `bool` 数组，其中下标为 `i` 的位置标记字符 `i` 是否已经包含在字符串中，如果是第二次看到这个字符，我们就可以**立刻返回** `false` 。总得来说，和我的思路1差不多。

不过，这里优化了一下：基于编码的码字个数，如果字符串的长度超过 `128/256` ，就可以直接返回 `false` ，因为这超过了字符集的大小。

实现的代码如下，时间复杂度是 $\text{O(n)}$ ，空间复杂度是 $\text{O(1)}$ —— 或者说时间复杂度也是 $\text{O(1)}$ ，因为最多只扫描128个字符。
```java
class Solution {
    public boolean isUnique(String str) {
        if (str.length() > 128) return false;
        boolean[] charSet = new boolean[128];
        for (int i = 0; i < str.length(); ++i) {
            int val = str.charAt(i);
            if (charSet[val]) return false;
            charSet[val] = true;
        }
        return true;
    }
}
```
或者，可以写成 $\text{O(c)}$ 空间和 $\text{O(min(c, n))}$ 时间，其中，`c` 是字符集的大小。

 
我们可以进一步假设，字符串中只会出现 `a-z` 小写字母，于是可以使用一个**整数**作为位向量：
```java
class Solution {
    public boolean isUnique(String str) {
        int checker = 0;
        for (int i = 0; i < str.length(); ++i) {
            int val = str.charAt(i) - 'a';
            if ((checker & (1 << val)) > 0) return false;
            checker |= (1 << val);
        }
        return true;
    }
}
```

