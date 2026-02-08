The gray code is a binary numeral system where two successive values differ in only one bit.

Given a non-negative integer n representing the total number of bits in the code, print the sequence of gray code. A gray code sequence must begin with 0.

**Example 1:**

```swift
Input: 2
Output: [0,1,3,2]
Explanation:
00 - 0
01 - 1
11 - 3
10 - 2


For a given n, a gray code sequence may not be uniquely defined.
For example, [0,2,3,1] is also a valid gray code sequence.

00 - 0
10 - 2
11 - 3
01 - 1
```
**Example 2:**

```swift
Input: 0
Output: [0]
Explanation: We define the gray code sequence to begin with 0.
             A gray code sequence of n has size = 2n, which for n = 0 the size is 20 = 1.
             Therefore, for n = 0 the gray code sequence is [0].
```
题意：格雷编码是一个二进制数字系统，在该系统中，两个连续的数值**仅有一个位的差异**。给定一个代表编码总位数的非负整数 `n` ，打印其格雷编码序列。即使有多个不同答案，也只需要返回其中一种。格雷编码序列必须以 `0` 开头。 


---
### 解法1 迭代+哈希表
考虑以下生成过程，除了第一个数字 `0` 外，其他数字都在上一个数字的基础上，从最右最低位往左遍历，试探性地异或改变一位——如果新数字尚未加入格雷码序列中，就确定这个数字，并以此为基础生成下一个数字；否则继续往左遍历：
```cpp
000
001
011
010
110
111
101
100
```
具体代码如下：
```cpp
class Solution {
public:
    vector<int> grayCode(int n) {
        int tot = 1 << n, v = 0;
        vector<int> ans{0};
        unordered_set<int> rec{0};
        for (int i = 1; i < tot; ++i) {
            for (int j = 0; j < n; ++j) {
                if (rec.find(v ^ (1 << j)) == rec.end()) {
                    v ^= (1 << j);
                    rec.insert(v);
                    break;
                }
            }
            ans.push_back(v);
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了39.58% 的用户
内存消耗：7.3 MB, 在所有 C++ 提交中击败了9.31% 的用户
```
---
### 解法2 数学规律
格雷编码的生成过程，`G[i] = i ^ (i / 2)` 。举例如 `n = 3` ：
```cpp
G(0) = 000
G(1) = 1 ^ 0 = 001 ^ 000 = 001
G(2) = 2 ^ 1 = 010 ^ 001 = 011
G(3) = 3 ^ 1 = 011 ^ 001 = 010
G(4) = 4 ^ 2 = 100 ^ 010 = 110
G(5) = 5 ^ 2 = 101 ^ 010 = 111
G(6) = 6 ^ 3 = 110 ^ 011 = 101
G(7) = 7 ^ 3 = 111 ^ 011 = 100
```
因此实现代码就很简单了：
```cpp
class Solution {
public:
    vector<int> grayCode(int n) {
        int tot = 1 << n;
        vector<int> ans;
        for (int i = 0; i < tot; ++i) ans.push_back(i ^ (i >> 1));
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.8 MB, 在所有 C++ 提交中击败了36.64% 的用户
```
对应的Java代码如下：
```java
class Solution {
    public List<Integer> grayCode(int n) {
        List<Integer> ans = new ArrayList<>();
        int tot = 1 << n;
        for (int i = 0; i < tot; ++i) ans.add(i ^ (i >> 1));
        return ans;
    }
}
```
Java就不必看运行效率了，反正在LeetCode上Java吊打C++。
