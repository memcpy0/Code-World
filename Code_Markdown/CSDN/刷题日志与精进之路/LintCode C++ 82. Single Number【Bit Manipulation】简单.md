Given `2 * n + 1` numbers, every numbers occurs twice except one, find it.


Example 1:
```clike
Input：[1,1,2,2,3,4,4]
Output：3
Explanation:
Only 3 appears once
```
Example 2:
```cpp
Input：[0,0,1]
Output：1
Explanation:
Only 1 appears once
```

**Challenge**：One-pass, constant extra space.
**Notice**： `n ≤ 100`


题意：找到只出现一次的数。

---
思路：无脑异或。

代码：
```cpp
class Solution {
public:
    /**
     * @param A: An integer array
     * @return: An integer
     */
    int singleNumber(vector<int> &A) {
        int ans = 0;
        for (auto & i : A) ans ^= i;
        return ans;
    }
};
```
