

Given a non-negative integer `num`, return the number of steps to reduce it to zero. If the current number is even, you have to divide it by 2, otherwise, you have to subtract 1 from it.

 

**Example 1:**

```swift
Input: num = 14
Output: 6
Explanation: 
Step 1) 14 is even; divide by 2 and obtain 7. 
Step 2) 7 is odd; subtract 1 and obtain 6.
Step 3) 6 is even; divide by 2 and obtain 3. 
Step 4) 3 is odd; subtract 1 and obtain 2. 
Step 5) 2 is even; divide by 2 and obtain 1. 
Step 6) 1 is odd; subtract 1 and obtain 0.
```

**Example 2:**

```swift
Input: num = 8
Output: 4
Explanation: 
Step 1) 8 is even; divide by 2 and obtain 4. 
Step 2) 4 is even; divide by 2 and obtain 2. 
Step 3) 2 is even; divide by 2 and obtain 1. 
Step 4) 1 is odd; subtract 1 and obtain 0.
```

**Example 3:**

```swift
Input: num = 123
Output: 12
```
**Constraints:**
- `0 <= num <= 10^6`

题意：给定一个非负整数 `num` 。如果当前数字是偶数，需要把它除以 `2` ；否则，减去 `1` 。返回将它变成 `0` 所需要的步数。


---
### 解法1 迭代
```cpp
class Solution {
public:
    int numberOfSteps (int num) {
        int ops = 0;
        while (num) {
            if (num & 1) num -= 1;
            else num >>= 1;
            ++ops;
        }
        return ops;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.1 MB, 在所有 C++ 提交中击败了6.86% 的用户
```
稍微提高一点效率的做法是：如果 `num` 为偶数则右移一位，相当于除以2；如果 `num` 为奇数，此时右移一位相当于减一和除以2一并操作。
```cpp
class Solution {
public:
    int numberOfSteps (int num) {
        if (num == 0) return 0; //特判0的情况
        int ops = 0;
        while (num) {
            ops += (num & 1) ? 2 : 1; 
            num >>= 1;
        }
        return --ops;
    }
};
```

---
### 解法2 递归
```cpp
class Solution {
public:
    int numberOfSteps (int num) {
        if (num <= 1) return num;
        return (num & 1) ? numberOfSteps(num - 1) + 1 : numberOfSteps(num / 2) + 1;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了5.11% 的用户
```
