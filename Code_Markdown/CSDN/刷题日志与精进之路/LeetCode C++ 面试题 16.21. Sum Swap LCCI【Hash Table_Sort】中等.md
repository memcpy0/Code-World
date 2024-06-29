Given two arrays of integers, find a pair of values (one value from each array) that you can swap to give the two arrays the same sum.

Return an array, where the first element is the element in the first array that will be swapped, and the second element is another one in the second array. If there are more than one answers, return any one of them. If there is no answer, return an empty array.

**Example:**

```clike
Input: array1 = [4, 1, 2, 1, 1, 2], array2 = [3, 6, 3, 3]
Output: [1, 3]
```

**Example:**

```clike
Input: array1 = [1, 2, 3], array2 = [4, 5, 6]
Output: []
```

**Note:** `1 <= array1.length, array2.length <= 100000`

题意：给定两个整数数组，交换一对数值（每个数组中取一个数值），使得两个数组所有元素的和相等。返回一个数组，第一个元素是第一个数组中要交换的元素，第二个元素是第二个数组中要交换的元素。若有多个答案，返回任意一个均可。若无满足条件的数值，返回空数组

---
### 解法 哈希表
设数组一的和为 `sum1` ，数组二的和为 `sum2` ，假设存在解法能够在交换后令两个数组的和相等，即同为偶数或同为奇数，可得 `sum1 +/- sum2` 应为偶数（为奇数时无解）。假设数组一换出的值是 `a` ，数组二换出的值是 `b` ，有 `sum1 - a + b = sum2 - b + a` ，得 `a - b = (sum1 - sum2) / 2` 。于是遍历数组一，以当前元素为 `a` ，在数组二中寻找有没有元素值等于 `a - (sum1 - sum2) / 2` ，存在则返回解。这种做法需要两重循环，为 $O(n^2)$ 复杂度。使用哈希集合，降低复杂度至 $O(n)$ 。
```cpp
class Solution {
public:
    vector<int> findSwapValues(vector<int>& array1, vector<int>& array2) {
        int sum = 0;
        unordered_set<int> rec;
        for (const int &v : array1) sum += v;
        for (const int &v : array2) {
            sum -= v;
            rec.insert(v);
        }
        if (sum & 1) return {};
        for (const int &v : array1) {
            int target = v - sum / 2;
            if (rec.find(target) != rec.end()) return {v, target};   
        }
        return {};   
    }
};
```
运行效率如下：
```cpp
执行用时：44 ms, 在所有 C++ 提交中击败了91.41% 的用户
内存消耗：32 MB, 在所有 C++ 提交中击败了29.29% 的用户
```
