Given an integer, return its base 7 string representation.

**Example 1:**

```c
Input: 100
Output: "202"
```

**Example 2:**

```c
Input: -7
Output: "-10"
```

**Note:** The input will be in range of `[-1e7, 1e7]` . 

题意：给定一个整数，将其转化为 `7` 进制，并以字符串形式输出。

---
### 思路
进制转换。十进制到七进制的转换，很简单的题目，只是需要注意负数的处理。代码如下：
```cpp
class Solution {
public:
    string convertToBase7(int num) {
        string ans; 
        bool minus = false;
        if (num < 0) { 
            num = -num; 
            minus = true;
        }
        do {
            int t = num % 7;
            num /= 7;
            ans.push_back('0' + t);
        } while (num);
        if (minus) ans.push_back('-');
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.1 MB, 在所有 C++ 提交中击败了11.93% 的用户
```
