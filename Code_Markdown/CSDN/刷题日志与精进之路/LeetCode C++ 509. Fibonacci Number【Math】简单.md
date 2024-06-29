The Fibonacci numbers, commonly denoted `F(n)` form a sequence, called the Fibonacci sequence, such that each number is the sum of the two preceding ones, starting from `0` and `1` . That is,
>F(0) = 0,   F(1) = 1
F(N) = F(N - 1) + F(N - 2), for N > 1.

Given `N` , calculate `F(N)` .

 **Example 1:**
```
Input: 2
Output: 1
Explanation: F(2) = F(1) + F(0) = 1 + 0 = 1.
```
**Example 2:**
```
Input: 3
Output: 2
Explanation: F(3) = F(2) + F(1) = 1 + 1 = 2.
```

**Example 3:**
```
Input: 4
Output: 3
Explanation: F(4) = F(3) + F(2) = 2 + 1 = 3.
```

**Note:**
-	`0 ≤ N ≤ 30` .
	
题意：老朋友了，斐波那契数列。数据范围水得很，看样子，哪怕只是最垃圾的简单递归求fib都可以过。

---
思路1：Recursion。代码量少，效率低。
```cpp
class Solution {
public:
    int fib(int N) {
        if (N <= 1) return N;
        return fib(N - 1) + fib(N - 2);
    }
};
```
代码效率：
```cpp
Runtime: 12 ms, faster than 26.02% of C++ online submissions for Fibonacci Number.
Memory Usage: 8.3 MB, less than 83.58% of C++ online submissions for Fibonacci Number.
```

---
思路2：记忆化递推法，使用数组。时间效率高，空间效率较低。
```cpp
class Solution {
public:
    int fib(int N) { 
        int arr[N + 1] = {0, 1};
        for (int i = 2; i <= N; ++i) arr[i] = arr[i - 2] + arr[i - 1];
        return arr[N];
    }
};
```
效率如下：
```cpp
执行用时 :0 ms, 在所有 cpp 提交中击败了100.00% 的用户
内存消耗 :8.1 MB, 在所有 cpp 提交中击败了93.37%的用户
```

---
思路3：交替递推法。
```cpp
class Solution {
public:
    int fib(int N) {
        if (N == 0 || N == 1) return N;
        int first = 0, second = 1, ans = 1;
        for (int i = 2; i <= N; ++i) {
            ans = first + second;
            first = second;
            second = ans;
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时 :0 ms, 在所有 cpp 提交中击败了100.00% 的用户
内存消耗 :8.1 MB, 在所有 cpp 提交中击败了86.04%的用户
```
---
思路4：斐波拉契通项公式。不过运行情况不如人意，可能是数学函数运算的复杂度导致的。$$a_n=\frac{1}{\sqrt{5}} \Big \lbrack \big( \frac{1+\sqrt{5}}{2}\big )^n - \big( \frac{1-\sqrt{5}}{2}\big)^n \Big \rbrack$$
 
```cpp
class Solution {
public:
    int fib(int N) {
        double sqrt5 = sqrt(5);
        return round(pow((1 + sqrt5) / 2, N) / sqrt5);
    }
};
```
效率如下：
```cpp
执行用时 :4 ms, 在所有 cpp 提交中击败了79.93% 的用户
内存消耗 :8.4 MB, 在所有 cpp 提交中击败了20.07%的用户
```
