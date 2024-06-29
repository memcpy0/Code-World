<p>Given two integers <code>a</code> and <code>b</code>, return <strong>any</strong> string <code>s</code> such that:</p>

<ul>
	<li><code>s</code> has length <code>a + b</code> and contains exactly <code>a</code> <code>'a'</code> letters, and exactly <code>b</code> <code>'b'</code> letters,</li>
	<li>The substring <code>'aaa'</code> does not occur in <code>s</code>, and</li>
	<li>The substring <code>'bbb'</code> does not occur in <code>s</code>.</li>
</ul>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: a = 1, b = 2
Output: "abb"
Explanation: "abb", "bab" and "bba" are all correct answers.
```
 
<p><strong>Example 2:</strong></p>

```cpp
Input: a = 4, b = 1
Output: "aabaa"
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>0 &lt;= a, b &lt;= 100</code></li>
	<li>It is guaranteed such an <code>s</code> exists for the given <code>a</code> and <code>b</code>.</li>
</ul>



题意：给定两个整数&nbsp;<code>A</code>&nbsp;和&nbsp;<code>B</code>，返回<strong>任意</strong>字符串 <code>S</code>，要求满足：
<ul>
	<li><code>S</code> 的长度为&nbsp;<code>A + B</code>，且正好包含&nbsp;<code>A</code>&nbsp;个 <code>'a'</code>&nbsp;字母与&nbsp;<code>B</code>&nbsp;个 <code>'b'</code>&nbsp;字母；</li>
	<li>子串&nbsp;<code>'aaa'</code>&nbsp;没有出现在&nbsp;<code>S</code>&nbsp;中；</li>
	<li>子串&nbsp;<code>'bbb'</code> 没有出现在&nbsp;<code>S</code>&nbsp;中。</li>
</ul>

---
### 解法 贪心
首先想到的是贪心，哪个字母剩余的数量多，就优先使用哪个。不过这里需要分类讨论一下：
- 两个字母的数量之差小于等于 `2` 时，比如 `A = 4, B = 4; A = 4, B = 5; A = 4, B = 6` ，这些情况下，对于剩余数量更多的字母每次使用两个，对于另一种字母也每次使用两个，如果剩下一个就全部用完；
- 两个字母的数量之差大于 `2` 时，比如 `A = 4, B = 7, ans = bb a bb aa bb a` ，此时对于剩余数量更多的字母，先每次使用两个，对于另一种字母则每次使用一个（使用两个的话，最后会剩下连续多个前一种字母，违背题意），直到两个字母的剩余数量变成第一种情况。

具体代码如下：
```cpp
class Solution {
public:
    string strWithout3a3b(int a, int b) {
        string ans;
        char x = 'a', y = 'b';
        if (a < b) { swap(x, y); swap(a, b); } //使得a>=b,a:x,b:y
        bool diff = a >= b + 3; 
        while (a || b) { 
            ans += string(min(a, 2), x);
            a -= min(a, 2);
            if (b) {
                if (!diff) { 
                    ans += string(min(b, 2), y);
                    b -= min(b, 2);
                } else { //是否差3个及以上
                    ans += string(1, y);
                    b -= 1;
                }
            } 
            diff = a >= b + 3;
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.8 MB, 在所有 C++ 提交中击败了88.47% 的用户
```
**或者这样写**：
```cpp
class Solution {
public:
    string strWithout3a3b(int a, int b) {
        string ans;
        char x = 'a', y = 'b';
        if (a < b) { swap(x, y); swap(a, b); } //使得a>=b,a:x,b:y 
        while (a || b) { 
            if (a) { ans.push_back(x); --a; }
            if (a && a + 1 >= b) { ans.push_back(x); --a; }
            if (b) { ans.push_back(y); --b; }
            //此前a==b,此时a+1==b;
            //此前a==b+1,此时a==b
            //此前a==b+2,此时a==b+1
            //此前a>=b+3,此时a>=b+2 
            if (a && b && a <= b + 1) { ans.push_back(y); --b; }
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.9 MB, 在所有 C++ 提交中击败了85.41% 的用户
```
当然，具体的填充策略不止一种，还可以：
- 两个字母的数量之差小于等于 `2` 时，比如 `A = 4, B = 4; A = 4, B = 5; A = 4, B = 6` ，这些情况下，对于剩余数量更多的字母每次使用一个（另一种字母数量为零时全部使用），对于另一种字母也每次使用一个；
- 两个字母的数量之差大于 `2` 时，比如 `A = 4, B = 7, ans = bba ba ba ba bb` ，此时对于剩余数量更多的字母，先每次使用两个，对于另一种字母则每次使用一个（使用两个的话，最后会剩下连续多个前一种字母，违背题意），直到两个字母的剩余数量变成第一种情况。

具体代码如下：
```cpp
class Solution {
public:
    string strWithout3a3b(int a, int b) {
        string ans;
        char x = 'a', y = 'b';
        if (a < b) { swap(x, y); swap(a, b); } //使得a>=b,a:x,b:y
        bool diff = a >= b + 3; 
        while (a || b) { 
            if (!diff) {  
                if (b) {
                    ans += string(1, x);
                    --a;
                    ans += string(1, y);
                    --b; 
                } else {
                    ans += string(a, x);
                    a = 0;
                } 
            } else { //是否差3个及以上
                ans += string(2, x);
                a -= 2;
                ans += string(1, y);
                --b;
            }
            diff = a >= b + 3;
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6 MB, 在所有 C++ 提交中击败了42.59% 的用户
```
**或者这样写**：
```cpp
class Solution {
public:
    string strWithout3a3b(int a, int b) {
        string ans;
        char x = 'a', y = 'b';
        if (a < b) { swap(x, y); swap(a, b); } //使得a>=b,a:x,b:y
        while (a || b) { 
            if (a) { ans.push_back(x); --a; };
            if (a >= b + 2) { ans.push_back(x); --a; }
            if (b) { ans.push_back(y); --b; }
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.8 MB, 在所有 C++ 提交中击败了89.41% 的用户
```
**还可以这样写**： 
- 两个字母的数量之差小于 `2` 时，比如 `A = 4, B = 4; A = 4, B = 5` ，这些情况下，对于剩余数量更多的字母每次使用一个（另一种字母数量为零时全部使用），对于另一种字母也每次使用一个；
- 两个字母的数量之差大于等于 `2` 时，比如 `A = 4, B = 7, ans = bba ba ba ba bb` ，此时对于剩余数量更多的字母，先每次使用两个，对于另一种字母则每次使用一个，直到两个字母的剩余数量变成第一种情况。

对应的简化代码如下：
```cpp
class Solution {
public:
    string strWithout3a3b(int a, int b) {
        string ans;
        char x = 'a', y = 'b';
        if (a < b) { swap(x, y); swap(a, b); } //使得a>=b,a:x,b:y
        while (a || b) { 
            if (a) { ans.push_back(x); --a; }; 
            if (a > b) { ans.push_back(x); --a; }           
            if (b) { ans.push_back(y); --b; } 
        }
        return ans;
    }
}; 
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.1 MB, 在所有 C++ 提交中击败了20.23% 的用户
```
**本题的递归写法如下**：
```cpp
class Solution {
public:
    string strWithout3a3b(int a, int b) {
        if (a == 0) return string(b, 'b');
        if (b == 0) return string(a, 'a');
        if (a == b) return "ab" + strWithout3a3b(a - 1, b - 1);
        return a > b ? "aab" + strWithout3a3b(a - 2, b - 1) : "bba" + strWithout3a3b(a - 1, b - 2);
    }
}; 
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.1 MB, 在所有 C++ 提交中击败了16.94% 的用户
```
