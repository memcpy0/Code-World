The k-digit number `N` is an Armstrong number if and only if the k-th power of each digit sums to `N` . Given a positive integer `N` , return true if and only if it is an Armstrong number.

**Example 1:**

```haskell
Input: 153
Output: true
Explanation: 
153 is a 3-digit number, and 153 = 1^3 + 5^3 + 3^3.
```

**Example 2:**

```haskell
Input: 123
Output: false
Explanation: 
123 is a 3-digit number, and 123 != 1^3 + 2^3 + 3^3 = 36.
```

**Note:** `1 <= N <= 10^8`

题意：假设存在一个 `k` 位数 `N` ，其每一位上的数字的 `k` 次幂的总和也是 `N` ，那么这个数是阿姆斯特朗数。给出一个正整数 `N` ，判定是否是阿姆斯特朗数，是则返回 `true` ，不是则返回 `false` 。 

---
### 解法 数学
```cpp
class Solution {
public:
    bool isArmstrong(int N) {
        int k = log10(N) + 1, ans = 0, t = N;
        while (t) { ans += pow(t % 10, k); t /= 10; }
        return ans == N;
    }
};
```
提交后执行结果如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了34.33% 的用户
```
