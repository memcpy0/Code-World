Given a **non-empty** string check if it can be constructed by taking a **substring** of it and appending multiple copies of the substring together. You may assume the given string consists of lowercase English letters only and its length will not exceed `10000` .

**Example 1:**
```cpp
Input: "abab"
Output: True
Explanation: It's the substring "ab" twice.
```
**Example 2:**
```cpp
Input: "aba"
Output: False
```
**Example 3:**
```py
Input: "abcabcabcabc"
Output: True
Explanation: It's the substring "abc" four times. (And the substring "abcabc" twice.)
```

题意：给出一个非空字符串，判断它是否能够由它的一个子串**重复多次**构成。给出的字符串只有小写字母。

---
思路1：一时半会我还真没有想到如何做这道题。于是用上了**正则表达式**。然后发现了这些特点：
- 单个字符会返回 `false` ，因为是重复多次，或者说不存在某个子串重复多次构成原串，该字符串只能由自己构成，需要返回 `false` ；
- 如果一个字符串 `S` 可以被子串 `s` 多次连接构成，那么子串的字符范围必然为 `[S[0], S[len / 2])` ，即子串必然包含 `S[0]` ，或者说必然是以 `S[0]` 开头的一段字符串。
- 我们从开头检查起，如果发现**当前子串长度是原串的整数倍**且正则**匹配成功**，就返回 `false` ；检查到 `S[len / 2]` 还不成功，就返回 `false` 。

Java的正则写起来更舒服：
```java
class Solution {
	public boolean repeatedSubstringPattern(String s) {
		int len = s.length();
		StringBuilder pattern = new StringBuilder();
		for (int i = 0; i < len / 2; ++i) {
			pattern.append(s.charAt(i));
			if (len % (i + 1) == 0 && s.matches("(" + pattern.toString() + ")+")) 
				return true;
		}
		return false;
	}
}
```
效率：
```handlebars
执行用时：58 ms, 在所有 Java 提交中击败了70.88% 的用户
内存消耗：40.3 MB, 在所有 Java 提交中击败了45.61% 的用户
```

---
思路2：仔细想了想后，发现可以这样做：每次将字符串 `s` 的首字符删去再添加到 `s` 尾部，如果某处修改后的 `s` 和原 `s` 相等，就说明 `s` 可以被子串重复构造出来；如果到了 `s[len / 2]` 还不能和原串相等，就返回 `false` 。

代码如下：
```cpp
class Solution {
public:
    bool repeatedSubstringPattern(string s) {
        list<char> source(s.begin(), s.end()), vari(s.begin(), s.end());
        int len = s.size();
        for (int i = 0; i < len / 2; ++i) {
            vari.push_back(vari.front());
            vari.pop_front();
            if (len % (i + 1) == 0 && source == vari) return true;
        }
        return false;
    }
};
```
效率低到爆炸：
```cpp
执行用时：520 ms, 在所有 C++ 提交中击败了5.12% 的用户
内存消耗：98.2 MB, 在所有 C++ 提交中击败了8.71% 的用户
```
---
思路3：暴力，这是上面那个思路的另一种版本，不用 `list` 进行添加删除，而是用 `s == s.substr(i + 1) + s.substr(0, i + 1)` 。

代码如下：
```cpp
class Solution {
public:
    bool repeatedSubstringPattern(string s) { 
        int len = s.size();
        string p;
        for (int i = 0; i < len / 2; ++i) {
            p.push_back(s[i]); 
            if (len % (i + 1) == 0 && s == (s.substr(i + 1) + p))
                return true;
        }
        return false;
    }
};
```
效率高许多：
```cpp
执行用时：32 ms, 在所有 C++ 提交中击败了43.54% 的用户
内存消耗：77 MB, 在所有 C++ 提交中击败了10.00% 的用户
```
---
思路4：看了看相似的题，发现这是一道字符串匹配的题目，emmmm……假设原串 `s` 由子串 `t` 重复 `N` 次而成，那么 `s + s` 就会让 `t` 重复 `2N` 次。于是有 `s = Nt, s + s = 2Nt` 。

接着对 `s + s` 掐头去尾，则这两处的子串被破坏，只包含 `2N - 2` 个 `t` ，而 `s` 中包含 `N` 个 `t` 。
- 如果 `(s + s)[1:-1]` 中包含 `s` ，就有 `2N - 2 >= N` ，可得 `N >= 2` 。由此我们可知字符串 `s` 可以由一个子串 `t` 重复 `N` 次（至少两次）构成，返回 `true` ；
- 反之，如果 `(s + s)[1:-1]` 中不包含 `s` ，则有 `2N - 2 < N` ，于是 `N < 2, N = 1` ，此时字符串 `s == t` ，假定的子串为 `s` 本身，返回 `false` 。
 
以题目给出的例子示范：
```py
s = "abab" = "ab" * 2
s + s = "abababab"
x = "bababa" 
s in x, return true

s = "aba"
s + s = "abaaba"
x = "baab"
s not int x, return false
```
这时应该明白这个做法的原理了：假设 `s` 可以被 `N >= 2` 个子串 `t` 构成，那么 `s + s` 中就有 `2N` 个 `t` ，破坏掉前后两个子串还有 `2N - 2` 个 `t` 在其中，仍然有足够的子串构成 `s` 。

朴素的字符串匹配代码如下：
```cpp
class Solution {
public:
    bool repeatedSubstringPattern(string s) { 
        string s2 = s + s;
        s2 = s2.substr(1, s2.size() - 2);
        if (s2.find(s) != string::npos)
            return true;
        return false;
    }
};
```
效率：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了62.40% 的用户
内存消耗：13.7 MB, 在所有 C++ 提交中击败了21.29% 的用户
```
使用KMP后的代码如下：
```cpp
class Solution {
public:
    void getNext(const string &p, int *next) {
        next[0] = -1, next[1] = 0;
        int cn = 0, pos = 2, len = p.size();
        while (pos < len) {
            if (p[pos - 1] == p[cn]) next[pos++] = ++cn;
            else if (cn > 0) cn = next[cn];
            else next[pos++] = 0;
        }
    }

    int kmp(const string &s, const string &p) {
        if (s.size() < p.size()) return -1;
        int slen = s.size(), plen = p.size(), i = 0, j = 0;
        int *next = new int[plen];
        getNext(p, next);
        while (i < slen && j < plen) {
            if (s[i] == p[j]) ++i, ++j;
            else if (next[j] == -1) ++i;
            else j = next[j];
        }
        return j >= plen ? i - j : -1;
    }

    bool repeatedSubstringPattern(string s) { 
        string s2 = s + s;
        s2 = s2.substr(1, s2.size() - 2);
        if (kmp(s2, s) != -1)
            return true;
        return false;
    }
};
```
效率如下：
```cpp
执行用时：32 ms, 在所有 C++ 提交中击败了43.54% 的用户
内存消耗：16.7 MB, 在所有 C++ 提交中击败了15.80% 的用户
```
另外写一个KMP，也是这样的效率：
```cpp
class Solution {
public:
    void getNext(const string &p, int *next) {
        next[0] = 0, next[1] = 0;
        int plen = p.size();
        for (int i = 1; i < plen; ++i) {
            int j = next[i];
            while (j && p[i] != p[j]) j = next[j];
            next[i + 1] = (p[i] == p[j]) ? j + 1 : 0;
        }
    }

    int kmp(const string &s, const string &p) {
        if (s.size() < p.size()) return -1;
        int slen = s.size(), plen = p.size(), j = 0;
        int *next = new int[plen + 1];
        getNext(p, next); 
        for (int i = 0; i < slen; ++i) {
            while (j && s[i] != p[j]) j = next[j];
            if (s[i] == p[j]) ++j;
            if (j >= plen) return i + 1 - j;
        }
        return -1;
    }

    bool repeatedSubstringPattern(string s) { 
        string s2 = s + s;
        s2 = s2.substr(1, s2.size() - 2);
        if (kmp(s2, s) != -1)
            return true;
        return false;
    }
};
```
