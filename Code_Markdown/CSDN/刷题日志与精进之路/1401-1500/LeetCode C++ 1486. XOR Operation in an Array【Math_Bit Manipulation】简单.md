

Given an integer `n` and an integer `start` .

Define an array `nums` where `nums[i] = start + 2 * i` (0-indexed) and `n == nums.length` .

Return the bitwise XOR of all elements of `nums`.

 

**Example 1:**

```swift
Input: n = 5, start = 0
Output: 8
Explanation: Array nums is equal to [0, 2, 4, 6, 8] where (0 ^ 2 ^ 4 ^ 6 ^ 8) = 8.
Where "^" corresponds to bitwise XOR operator.
```

**Example 2:**

```swift
Input: n = 4, start = 3
Output: 8
Explanation: Array nums is equal to [3, 5, 7, 9] where (3 ^ 5 ^ 7 ^ 9) = 8.
```

**Example 3:**

```swift
Input: n = 1, start = 7
Output: 7
```

**Example 4:**

```swift
Input: n = 10, start = 5
Output: 2
```

**Constraints:**
-  `1 <= n <= 1000`
- `0 <= start <= 1000`
-  `n == nums.length`


题意：两个整数 `n` 和 `start` 。数组 `nums` 定义为：`nums[i] = start + 2 * i`（下标从 0 开始）且 `n == nums.length` 。请返回 `nums` 中所有元素按位异或后得到的结果。
 

---
### 解法1 遍历
$O(n)$ 时间，$O(1)$ 空间：
```cpp
class Solution {
public:
    int xorOperation(int n, int start) {
        int ans = 0;
        for (int i = 0; i < n; ++i) ans ^= (start + 2 * i); 
        return ans;
    }
};
```
执行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了7.13% 的用户
```
---
### 解法2 数学
本题涉及到异或，可以设计出一个数学解法。首先看一下异或的性质：
1. $2x \oplus (2x + 1) = 1$ 。这里 $2x$ 为偶数，二进制形式最后一位是 $0$ ，$2x + 1$ 二进制形式最后一位是 $1$ ，其他位相同；
2. $2x \oplus 1 = 2x + 1$ ，即偶数异或 $1$ 得到大 $1$ 的奇数。由于 $2x \oplus (2x + 1) = 1$ ，可得 $2x \oplus (2x + 1) \oplus (2x + 1) \oplus 1 = 1 \oplus (2x + 1) \oplus 1$ ，即 $2x \oplus 1 = 2x + 1$ ；
3. $(2x + 1) \oplus 1 = 2x$ ，即奇数异或 $1$ 得到小 $1$ 的偶数。同样可由性质1推出。

本题中，$nums$ 数组实际上是一个以 $start$ 为首项、$2$ 为公差、项数为 $n$ 的等差数列。设本题最终结果是 $f(n)$ ，令 $a = start$ ，于是根据题意：$$f(n) = a\oplus (a + 2) \oplus \dots  \oplus (a + 2(n - 1))$$

由于 $nums$ 数组中元素的变动——$a, a + 2, \dots, a + 2(n - 1)$ 只涉及到对二进制形式中从右数第二位的 $+1$ 操作，不涉及到最右一位，所以将这些元素**相同的最右一位**（记 $e = a\ \&\ 1$）都提出来计算：
 $$f(n) = [((a\gg 1) \oplus ((a + 2)\gg 1) \oplus \dots \oplus ((a + 2(n-1)) \gg 1)) \ll 1] \oplus \underbrace{ (e\oplus e\oplus \dots \oplus e)}_{n个e}\\ = [((a \gg 1)\oplus ((a \gg 1) + 1) \oplus \dots \oplus ((a \gg 1) + n - 1)) \ll 1] \oplus \underbrace{(e\oplus e\oplus \dots \oplus e)}_{n个e}$$

再记 $A = (a \gg 1),\ h(e, n) = \underbrace{(e\oplus e\oplus \dots \oplus e)}_{n个e}$ ，则有：
$$f(n) = [(A\oplus (A+1)\oplus \dots \oplus (A+n-1))\ll 1] \oplus h(e, n)$$

再令 $g(A, n) = A\oplus (A+1)\oplus \dots \oplus (A+n-1)$ ，即把所有项的最后一位和其他位分开后异或其他位，则有：
$$f(n) = [g(A, n)\ll 1] \oplus h(e, n) \   \ \ \ \ (1)$$

接着只考虑 $A, n, e$ 这三个数，寻找它们之间的规律。当 $A$ 是偶数时，求出 $g(A, n)$ 的前几项：
$$\begin{cases}
g(A, 1) = A\\
g(A, 2) = A\oplus (A+1) = 1\\
g(A, 3) = g(A, 2) \oplus (A + 2) = A+3\\
g(A, 4) = g(A, 3) \oplus (A+3) = 0 \\
g(A, 5) = g(A, 4) \oplus (A+4) = A+4\\
g(A, 6) = g(A, 5)\oplus (A+5) = 1\\
g(A, 7) = g(A, 6) \oplus (A + 6) = A+7\\
g(A, 8) = g(A, 7) \oplus (A+7) = 0\\
g(A, 9) = g(A, 8) \oplus (A+8) = A+8
\end{cases}
$$
不难发现这样的规律：
$$g(A, n) = \begin{cases}
0 \quad &n = 4k\\ 
A + n - 1\quad &n = 4k + 1\\
1 \quad &n = 4k + 2\\
A + n\quad &n = 4k + 3
\end{cases}
$$
当 $A$ 是奇数时，求出 $g(A, n)$ 的前几项：
$$\begin{cases}
g(A, 1) = A\\
g(A, 2) = A\oplus (A+1)\\
g(A, 3) = g(A, 2) \oplus (A + 2) = A\oplus 1 = A- 1\\
g(A, 4) = g(A, 3) \oplus (A+3) =(A-1)\oplus(A+3) \\
g(A, 5) = g(A, 4) \oplus (A+4) = (A-1)\oplus 1 = A\\
g(A, 6) = g(A, 5)\oplus (A+5) = A\oplus(A+5)\\
g(A, 7) = g(A, 6) \oplus (A + 6) = A \oplus 1 = A - 1\\
g(A, 8) = g(A, 7) \oplus (A+7) =(A-1)\oplus(A+7)\\
g(A, 9) = g(A, 8) \oplus (A+8) = A
\end{cases}
$$
不难发现这样的规律：
$$g(A, n) = \begin{cases}
(A-1)\oplus(A+n-1) \quad &n = 4k\\ 
A \quad &n = 4k + 1\\
A\oplus(A+n-1) \quad &n = 4k + 2\\
A - 1\quad &n = 4k + 3
\end{cases}
$$
总结可得：
$$g(A, n) = \begin{cases}
0 \quad &A为偶数,\ n = 4k\\ 
A + n - 1\quad &A为偶数,\ n = 4k + 1\\
1 \quad &A为偶数,\ n = 4k + 2\\
A + n\quad &A为偶数,\ n = 4k + 3\\
(A-1)\oplus(A+n-1) \quad &A为奇数,\ n = 4k\\ 
A \quad &A为奇数,\ n = 4k + 1\\
A\oplus(A+n-1) \quad &A为奇数,\ n = 4k + 2\\
A - 1\quad &A为奇数,\ n = 4k + 3
\end{cases}
$$

至于 $h(e, n)$ ：
$$h(e, n) = \begin{cases}
0 \quad &e = 1,\ n为偶数\\
1\quad &e=1,\ n为奇数\\
0\quad &e = 0
\end{cases}
$$

现在求解 $f(n) = [g(A, n) \ll 1] \oplus h(e, n)$ 就很简单了。$O(1)$ 时间和 $O(1)$ 空间解法如下：
```cpp
class Solution {
private:
    int g(int A, int n) {
        int k = n % 4;
        if (A & 1) {
            if (k == 0) return (A - 1) ^ (A + n - 1);
            else if (k == 1) return A;
            else if (k == 2) return A ^ (A + n - 1);
            else return A - 1;
        } else {
            if (k == 0) return 0;
            else if (k == 1) return A + n - 1;
            else if (k == 2) return 1;
            else return A + n;
        }
        return 0;
    }
    int h(int e, int n) {
        return e == 1 ? ((n & 1) ? 1 : 0) : 0; 
    }
public:
    int xorOperation(int n, int start) { 
        int A = start >> 1, e = start & 1;
        return (g(A, n) << 1) ^ h(e, n);
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.8 MB, 在所有 C++ 提交中击败了46.37% 的用户
```
