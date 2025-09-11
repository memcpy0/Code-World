Given an array arr, replace every element in that array with the **greatest** element among the elements to its right, and replace the **last** element with `-1` .

After doing so, return the array.
 
**Example 1:** 
```cpp
Input: arr = [17,18,5,4,6,1]
Output: [18,6,6,6,1,-1]
```
**Constraints:**
- `1 <= arr.length <= 10^4`
- `1 <= arr[i] <= 10^5`

题意：将给出的数组中，每个元素替换为右侧最大的元素，最后一个元素，用 `-1` 替换。

---
思路：原地修改，从后往前替换数组元素，需要定义一个变量跟踪最大值。

代码：
```cpp
class Solution {
public:
    vector<int> replaceElements(vector<int>& arr) { 
        if (arr.empty()) return arr;
        int maxValue = -1; 
        for (int i = arr.size() - 1; i >= 0; --i) { 
            int t = arr[i];
            arr[i] = maxValue;
            maxValue = max(maxValue, t);
        } 
        return arr;
    }
};
```

