

Given the **API** `rand7()` that generates a uniform random integer in the range `[1, 7]`, write a function `rand10()` that generates a **uniform random** integer in the range `[1, 10]`. You can only call the API `rand7()`, and you shouldn't call any other API. Please **do not** use a language's built-in random API.

Each test case will have one **internal** argument `n`, the number of times that your implemented function `rand10()` will be called while testing. Note that this is **not an argument** passed to `rand10()`.

**Follow up:**

-  What is the **expected value** for the number of calls to `rand7()` function?
- Could you minimize the number of calls to `rand7()`?

 

**Example 1:**

```swift
Input: n = 1
Output: [2]
```

**Example 2:**

```swift
Input: n = 2
Output: [2,8]
```

**Example 3:**

```swift
Input: n = 3
Output: [3,8,10]
```

**Constraints:** `1 <= n <= 10^5`

题意：已有方法 `rand7` 可生成 `1` 到 `7` 范围内的均匀随机整数，试写一个方法 `rand10` 生成 `1` 到 `10` 范围内的均匀随机整数。不要使用系统的 `Math.random()` 方法。

---
### 解法 数学
这是一道概率题：
```cpp
// The rand7() API is already defined for you.
// int rand7();
// @return a random integer in the range 1 to 7 
class Solution {
public:
    int rand10() { 
        int res;
        while (true) {
            res = (rand7() - 1) * 7 + rand7(); //(0~6)*7+(1~7)=1~49均匀分布
            if (res <= 40) break; //剔除大于40的值,1~40等概率出现
        }
        return res % 10 + 1; //构造1~10的均匀分布
    }
};
```
执行结果如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了82.20% 的用户
内存消耗：8.6 MB, 在所有 C++ 提交中击败了5.01% 的用户
```
