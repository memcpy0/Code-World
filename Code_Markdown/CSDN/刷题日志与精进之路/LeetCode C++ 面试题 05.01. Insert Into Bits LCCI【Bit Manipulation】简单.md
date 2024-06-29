

You are given two 32-bit numbers, `N` and `M`, and two bit positions, `i` and `j`. Write a method to insert `M` into `N` such that `M` starts at bit `j` and ends at bit `i`. You can assume that the bits `j` through `i` have enough space to fit all of `M`. That is, if `M = 10011`, you can assume that there are at least `5` bits between `j` and `i`. You would not, for example, have `j = 3` and `i = 2`, because `M` could not fully fit between bit `3` and bit `2`.

**Example 1:**

```swift
Input: N = 10000000000, M = 10011, i = 2, j = 6
Output: N = 10001001100
```

**Example 2:**

```swift
Input:  N = 0, M = 11111, i = 0, j = 4
Output: N = 11111
```
题意：给定两个32位的整数 `N, M`，以及表示比特位置的 `i, j`。编写一种方法，将 `M` 插入 `N`，使得 `M` 从 `N` 的第 `j` 位开始，到第 `i` 位结束。假定从 `j` 位到 `i` 位足以容纳 `M` 。

---
### 解法 位操作
出题人的语文是体育老师教的吧？题目没说清楚 `j ~ i` 超过了 `M` 的长度之后，是填充 `0` 还是保留 `N` 原来的数字。Wa了一次之后，我才发现需要填充 `0` ：
```cpp
class Solution {
public:
    int insertBits(int N, int M, int i, int j) {
        if (N == 0) return M; //边界情况
        //i~j的长度(j-i+1)>M的二进制形式长度(如M=10=1010[2],长4位)时,M高位补零
        for (int k = i; k <= j; ++k) {
            int v = (M >> (k - i)) & 1; //从M的第0位开始
            if (v) N |= (1 << k); //v==1
            else N &= ~(1 << k);  //v==0
        } 
        return N;
    }
};
```
提交后效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了5.11% 的用户
```
递归版代码如下：
```cpp
class Solution {
public:
    int insertBits(int N, int M, int i, int j) {
        if (N == 0) return M; //特殊情况
        if (i > j) return N;  //递归边界
        return insertBits((M & 1) ? (N |= (1 << i)) : (N &= ~(1 << i)), M >> 1, i + 1, j);
    }
};  
```
更简单的写法是，先将 `N` 的 `i ~ j` 位Mask成 `0` ，再把 `M` Mask上去：
```cpp
class Solution {
public:
    int insertBits(int N, int M, int i, int j) {
        N &= ~(((1ll << (j - i + 1)) - 1) << i); //(1<<(j-i+1))-1得到(j-i+1)位长度的1...
        N |= (M << i);
        return N;
    }
};
```
这里需要转成 `long long` 型，是因为可能发生溢出——对测试用例 `1 -2 0 31` 而言，`1 << (j - i + 1)` 会越界，需要转换成64位运算。提交后，执行结果如下所示：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了10.23% 的用户
```
