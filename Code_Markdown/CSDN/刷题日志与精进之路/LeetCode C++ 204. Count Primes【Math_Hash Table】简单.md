
Count the number of prime numbers less than a non-negative number, `n`.

 

**Example 1:**

```swift
Input: n = 10
Output: 4
Explanation: There are 4 prime numbers less than 10, they are 2, 3, 5, 7.
```

**Example 2:**

```swift
Input: n = 0
Output: 0
```

**Example 3:**

```swift
Input: n = 1
Output: 0
```
**Constraints:** `0 <= n <= 5 * 10^6`

题意：统计所有小于非负整数 `n` 的质数的数量。

---
### 解法 埃利特斯拉筛法
普通的埃式筛法：
```cpp
class Solution {
public:
    int countPrimes(int n) { //埃利特斯拉筛法
        if (n <= 1) return 0;
        int cnt = 0;
        const int maxn = 5 * 1e6;
        bitset<maxn> bst;
        for (int i = 2; i < n; ++i) {
            if (bst[i] == 0) {
                ++cnt;
                for (int j = i + i; j < n; j += i) bst[j] = 1;
            }
        }
        return cnt;
    }
};
```
运行效率如下：
```cpp
执行用时：184 ms, 在所有 C++ 提交中击败了69.29% 的用户
内存消耗：7 MB, 在所有 C++ 提交中击败了32.00% 的用户
```
优化的埃式筛法：
```cpp
class Solution {
public:
    int countPrimes(int n) {
        if (n <= 1) return 0;
        int cnt = 0;
        const int maxn = 5 * 1e6;
        bitset<maxn> bst;
        for (int i = 2; i * i < n; ++i) 
            if (bst[i] == 0) 
                for (int j = i * i; j < n; j += i) bst[j] = 1;
        for (int i = 2; i < n; ++i) 
            if (bst[i] == false) ++cnt;
        return cnt;
    }
};
```
运行效率如下：
```cpp
执行用时：164 ms, 在所有 C++ 提交中击败了70.36% 的用户
内存消耗：6.9 MB, 在所有 C++ 提交中击败了32.18% 的用户
```
