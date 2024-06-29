Given an array of integers `A` , return the largest integer that only occurs once. If no integer occurs once, return `-1` .

 

**Example 1:**

```haskell
Input: [5,7,3,9,4,9,8,3,1]
Output: 8
Explanation: 
The maximum integer in the array is 9 but it is repeated. The number 8 occurs only once, so it's the answer.
```

**Example 2:**

```haskell
Input: [9,9,8,8]
Output: -1
Explanation: 
There is no number that occurs only once.
```
 
**Note:**
- `1 <= A.length <= 2000`
- `0 <= A[i] <= 1000`


题意：找出并返回在整数数组 `A` 中仅出现一次的最大整数。如果不存在这个只出现一次的整数，则返回 `-1` 。

---
### 解法 哈希表
```cpp
class Solution {
public:
    int largestUniqueNumber(vector<int>& A) {
        int cnt[1010] = {0};
        for (const int &v : A) ++cnt[v];
        for (int i = 1000; i >= 0; --i) if (cnt[i] == 1) return i;
        return -1;
    }
};
```
运行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了82.81% 的用户
内存消耗：8.8 MB, 在所有 C++ 提交中击败了71.43% 的用户
```
