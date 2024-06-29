We define the Perfect Number is a **positive** integer that is equal to the sum of all its **positive** divisors except itself.
Now, given an **integer** `n` , write a function that returns true when it is a perfect number and false when it is not.

**Example:**
```cpp
Input: 28
Output: True
Explanation: 28 = 1 + 2 + 4 + 7 + 14
```

**Note:** The input number `n` will not exceed `100,000,000` . (`1e8`)

题意：定义完美数是一个正整数，它和除了它自身以外的所有正因子之和相等。


---
### 思路1 实际运算
简单题目，需要注意的是：给出的 `n` 是整数，但是完美数必定大于 `1` ，所以如果 `num <= 1` 就直接返回 `false` ；另外，如果是奇数，必然不是完美数，也直接返回 `false` 。具体代码如下，算法复杂度是 $O(\sqrt n)$ ：
```cpp
class Solution {
public:
    bool checkPerfectNumber(int num) {
        if (num <= 1 || num & 1) return false;
        int sqr = sqrt(num), ans = 1;
        for (int i = 2; i <= sqr; ++i) 
            if (num % i == 0) ans += (i + num / i);
        return ans == num;
    }
};
```
提交后结果如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了59.29% 的用户
内存消耗：5.9 MB, 在所有 C++ 提交中击败了31.11% 的用户
```
---
### 思路2 打表
给出打表的代码，得到目标范围内的完美数只有：
```cpp 
#include <bits/stdc++.h>
using namespace std;
void printTable() { 
    int t = 1e8 + 1;
    for (int k = 2; k < t; k += 2) { //奇数不是完美数
        int sqr = sqrt(k), ans = 1;
        for (int i = 2; i <= sqr; ++i) 
            if (k % i == 0) ans += (i + k / i);
        if (ans == k) cout << k << endl;
    }
} 
int main() {
	printTable();
	return 0;
}
```
$O(n\sqrt n)$ 的算法效率属实不行，运行了蛮久，最后得到只有下面几个数是完美数：
```cpp
6
28
496
8128
33550336
```
于是 $O(1)$ 的算法出现了：
```cpp
class Solution {
public: 
    bool checkPerfectNumber(int num) {
        return (num == 6 || num == 28 || num == 496 || num == 8128 || num == 33550336);
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.7 MB, 在所有 C++ 提交中击败了87.96% 的用户
```
