Given an integer `n`. No-Zero integer is a positive integer which d**oesn't contain any 0** in its decimal representation.

Return a list of two integers `[A, B]`where:
- `A` and `B` are No-Zero integers.
 - `A + B = n`

It's guarateed that there is at least one valid solution. If there are many valid solutions you can return any of them.

 

**Example 1:**

```swift
Input: n = 2
Output: [1,1]
Explanation: A = 1, B = 1. A + B = n and both A and B don't contain any 0 in their decimal representation.
```

**Example 2:** 

```swift
Input: n = 11
Output: [2,9]
```
 
**Example 3:**

```swift
Input: n = 10000
Output: [1,9999]
```

**Example 4:**

```swift
Input: n = 69
Output: [1,68]
```

**Example 5:**

```swift
Input: n = 1010
Output: [11,999]
```

 

**Constraints:**  `2 <= n <= 10^4`
 
题意：「无零整数」是十进制表示中不含任何 `0` 的正整数。给出一个整数 `n`，返回一个**由两个无零整数组成的列表** `[A, B]` ，满足 `A + B = n` 。如果存在多个有效解决方案，可以返回其中任意一个。

---
### 解法 双指针
一个数从 `1` 开始，另一个从 `n - 1` 开始，向中间搜索：
```cpp
class Solution {
private:
    bool isNoZeroNum(int n) {
        do {
            if (n % 10 == 0) return true;
            n /= 10;
        } while (n);
        return false;
    }
public:
    vector<int> getNoZeroIntegers(int n) {
        int lo = 1, hi = n - 1;
        while (lo <= hi) {
            if (!isNoZeroNum(lo) && !isNoZeroNum(hi)) return {lo, hi};
            ++lo, --hi; 
        }
        return {lo, hi};
    }
};
```
执行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了49.71% 的用户
内存消耗：6.4 MB, 在所有 C++ 提交中击败了30.35% 的用户
```
