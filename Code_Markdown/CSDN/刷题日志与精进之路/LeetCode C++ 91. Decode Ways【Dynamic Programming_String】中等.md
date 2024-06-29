
A message containing letters from `A - Z` is being encoded to numbers using the following mapping:
```swift
'A' -> 1
'B' -> 2
...
'Z' -> 26
```
Given a **non-empty** string containing only digits, determine the total number of ways to decode it.

The answer is guaranteed to fit in a **32-bit** integer.
 
**Example 1:**

```swift
Input: s = "12"
Output: 2
Explanation: It could be decoded as "AB" (1 2) or "L" (12).
```

**Example 2:**

```swift
Input: s = "226"
Output: 3
Explanation: It could be decoded as "BZ" (2 26), "VF" (22 6), or "BBF" (2 2 6).
```

**Example 3:**

```swift
Input: s = "0"
Output: 0
Explanation: There is no character that is mapped to a number starting with '0'. We cannot ignore a zero when we face it while decoding. So, each '0' should be part of "10" --> 'J' or "20" --> 'T'.
```

**Example 4:**

```swift
Input: s = "1"
Output: 1
```
 
**Constraints:**
- `1 <= s.length <= 100`
- `s` contains only digits and may contain leading zero(s).

题意：一条包含字母 `A - Z` 的消息通过以下方式进行了编码：

```swift
'A' -> 1
'B' -> 2
...
'Z' -> 26
```
给定一个只包含数字的**非空**字符串，请计算解码方法的总数。题目数据保证答案肯定是一个 32 位的整数。

---
### 解法 动态规划
这一题其实是**上楼梯**的复杂版，重点在于**边界处理**。一开始用Java代码写的解法如下：
```java
class Solution {
    public int numDecodings(String s) {
        int n = s.length();
        int[] dp =  new int[n + 1]; //dp[i]表示字符串s[0:i)有多少种解码方法
        dp[0] = 1;
        for (int i = 0; i < n; ++i) {
            if (i == 0) dp[i + 1] = (s.charAt(i) != '0' ? 1 : 0);
            else { 
                if (s.charAt(i) != '0') {
                    dp[i + 1] = dp[i];
                    if (s.charAt(i - 1) == '1' || (s.charAt(i - 1) == '2' && s.charAt(i) <= '6'))
                        dp[i + 1] += dp[i - 1]; 
                } else if (s.charAt(i - 1) == '1' || s.charAt(i - 1) == '2') dp[i + 1] = dp[i - 1]; //10,20 
            } 
        } 
        return dp[n];
    }
}    
```
后来用C++简写了代码：
```cpp
class Solution {
public:
    int numDecodings(string s) { 
        if (s.size() == 1 && s[0] == '0') return 0; //不存在空字符串,只有一个0时
        if (s.size() == 1) return 1; //只有一个非0字符
        int n = s.size();
        vector<int> dp(n + 1, 0); //dp[i]表示长度为0的字符串有多少种解码方法
        dp[0] = 1;
        for (int i = 0; i < n; ++i) {
            dp[i + 1] = (s[i] == '0') ? 0 : dp[i]; 
            if (i > 0 && (s[i - 1] == '1' || (s[i - 1] == '2' && s[i] <= '6')))
                dp[i + 1] += dp[i - 1];
        }
        return dp.back(); 
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.5 MB, 在所有 C++ 提交中击败了27.20% 的用户
```
