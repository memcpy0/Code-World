

Given an integer `n`, return *the number of trailing zeroes in `n!`.*

**Follow up:** Could you write a solution that works in logarithmic time complexity?

 

**Example 1:**

```swift
Input: n = 3
Output: 0
Explanation: 3! = 6, no trailing zero.
```

**Example 2:**

```swift
Input: n = 5
Output: 1
Explanation: 5! = 120, one trailing zero.
```

**Example 3:**

```swift
Input: n = 0
Output: 0
```

 

**Constraints:**  `1 <= n <= 10^4`

题意：给定一个整数 `n`，返回 `n!` 结果尾数中零的数量。

---
### 解法1 计算因子5的个数
```cpp
class Solution {
public:
    int trailingZeroes(int n) {
        int count = 0;
        for(long long i = 5; i <= (long long)n; i += 5){
            long long temp = i;
            while(temp % 5 == 0){
                count++;
                temp /= 5;
            }
        }
        return count; 
    }
}; 
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了41.20% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了5.12% 的用户
```
---
### 解法2 数学
```cpp
class Solution {
public:
    int trailingZeroes(int n) {
        int ans = 0;
        while (n / 5 > 0) {
            ans += n / 5;
            n /= 5;
        }
        return ans;
    }
}; 
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了10.35% 的用户
```
