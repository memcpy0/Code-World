

Given a positive integer `n`, find and return *the **longest distance** between any two **adjacent** `1`'s in the binary representation of `n`. If there are no two adjacent `1`'s, return `0`.*

Two `1`'s are **adjacent** if there are only `0`'s separating them (possibly no `0`'s). The **distance** between two `1`'s is the absolute difference between their bit positions. For example, the two `1`'s in `"1001"` have a distance of 3.

 

**Example 1:**

```swift
Input: n = 22
Output: 2
Explanation: 22 in binary is "10110".
The first adjacent pair of 1's is "10110" with a distance of 2.
The second adjacent pair of 1's is "10110" with a distance of 1.
The answer is the largest of these two distances, which is 2.
Note that "10110" is not a valid pair since there is a 1 separating the two 1's underlined.
```

**Example 2:**

```swift
Input: n = 5
Output: 2
Explanation: 5 in binary is "101".
```

**Example 3:**

```swift
Input: n = 6
Output: 1
Explanation: 6 in binary is "110".
```

**Example 4:**

```swift
Input: n = 8
Output: 0
Explanation: 8 in binary is "1000".
There aren't any adjacent pairs of 1's in the binary representation of 8, so we return 0.
```

**Example 5:**

```swift
Input: n = 1
Output: 0
```
**Constraints:** `1 <= n <= 10^9`

题意：给定一个正整数 `n`，找到并返回 `n` 的二进制表示中两个相邻 `1`（之间只有 `0` 分隔开来，或者不存在 `0`）之间的**最长距离**。如果不存在两个相邻的 `1`，返回 `0` 。


---
### 解法 直接遍历
```cpp
class Solution {
public:
    int binaryGap(int n) {
        if (n & (n - 1) == 0) return 0; //没有相邻的1
        int maxDist = 0, prePos = -1;
        for (int i = 0; i < 32; ++i) {
            if ((n >> i) & 1) {
                if (prePos != -1) maxDist = max(i - prePos, maxDist);
                prePos = i;
            }
        }
        return maxDist;
    }
};
```
时空效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了7.93% 的用户
```
