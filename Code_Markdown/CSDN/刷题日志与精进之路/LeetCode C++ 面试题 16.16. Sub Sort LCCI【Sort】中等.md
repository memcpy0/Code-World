

Given an array of integers, write a method to find indices m and n such that if you sorted elements m through n, the entire array would be sorted. Minimize `n - m` (that is, find the smallest such sequence).

Return `[m,n]`. If there are no such m and n (e.g. the array is already sorted), return `[-1, -1]`.

**Example:**

```clike
Input:  [1,2,4,7,10,11,7,12,6,7,16,18,19]
Output:  [3,9]
```
**Note:**  `0 <= len(array) <= 1000000`

题意：给定一个整数数组，编写一个函数，找出索引 `m` 和 `n` ，只要将索引区间 `[m,n]` 的元素排好序，整个数组就是有序的。注意：`n - m` 尽量最小，找出符合条件的最短序列。函数返回值为 `[m, n]` ，若不存在这样的 `m` 和 `n`（例如整个数组是有序的），请返回 `[-1, -1]` 。

---
### 解法 排序
朴实解法是对原数组进行排序，然后对比即可：
```cpp
class Solution {
public:
    vector<int> subSort(vector<int>& array) {
        if (array.empty()) return {-1, -1};
        vector<int> temp = array;
        sort(temp.begin(), temp.end());
        int m = -1, n = -1, size = temp.size();
        for (int i = 0; i < size; ++i) {
            if (temp[i] != array[i]) {
                if (m == -1) m = i;
                else n = i;
            }
        }
        return {m, n};
    }
};
```
运行效率如下：
```cpp
执行用时：96 ms, 在所有 C++ 提交中击败了31.23% 的用户
内存消耗：39.5 MB, 在所有 C++ 提交中击败了11.27% 的用户
```
 此外还有 $O(n)$ 算法，这里暂时没有时间写。
