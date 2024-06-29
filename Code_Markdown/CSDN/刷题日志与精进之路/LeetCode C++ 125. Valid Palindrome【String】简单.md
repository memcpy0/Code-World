Given a string, determine if it is a palindrome, considering only **alphanumeric** characters and ignoring cases.

Note: For the purpose of this problem, we define empty string as valid palindrome.

Example 1:

```cpp
Input: "A man, a plan, a canal: Panama"
Output: true
```

Example 2:

```cpp
Input: "race a car"
Output: false
```
题意：判断给出的字符串在去除了空格/其他字符后剩下的字母数字字符串是否是回文串，忽略大小写。

思路：删去其他字符，然后改变大写字符为小写。最后判断即可。

代码：
```cpp
class Solution {
public:
    bool isPalindrome(string s) {
        for (auto it = s.begin(); it != s.end(); ) {
            if (!isalnum(*it)) it = s.erase(it);
            else { 
                if (isalpha(*it)) *it = tolower(*it); 
                ++it; 
            }
        }
        string rev = s;
        reverse(s.begin(), s.end());
        return rev == s;
    }
};
```
忙着打卡，随便写的，效率不高。
