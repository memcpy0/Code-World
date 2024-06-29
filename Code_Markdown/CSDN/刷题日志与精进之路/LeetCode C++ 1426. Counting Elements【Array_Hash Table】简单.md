Given an integer array `arr`, count how many elements `x` there are, such that `x + 1` is also in `arr`.

If there're duplicates in `arr`, count them seperately.

 

**Example 1:**

```haskell
Input: arr = [1,2,3]
Output: 2
Explanation: 1 and 2 are counted cause 2 and 3 are in arr.
```

**Example 2:**

```haskell
Input: arr = [1,1,3,3,5,5,7,7]
Output: 0
Explanation: No numbers are counted, cause there's no 2, 4, 6, or 8 in arr.
```

**Example 3:**

```haskell
Input: arr = [1,3,2,3,5,0]
Output: 3
Explanation: 0, 1 and 2 are counted cause 1, 2 and 3 are in arr.
```

**Example 4:**

```haskell
Input: arr = [1,1,2,2]
Output: 2
Explanation: Two 1s are counted cause 2 is in arr.
```

**Example 5:**

```haskell
Input: arr = [1,1,2]
Output: 2
Explanation: Both 1s are counted because 2 is in the array.
```

**Constraints:**
- `1 <= arr.length <= 1000`
-  `0 <= arr[i] <= 1000`

题意：给出一个整数数组 `arr`， 对于元素 `x` ，只有当 `x + 1` 也在数组 `arr` 里时，才能记为 `1` 个数。如果数组 `arr` 里有重复的数，每个重复的数单独计算。

---
### 解法 哈希表
```cpp
class Solution {
public:
    int countElements(vector<int>& arr) {
        int cnt[1010] = {0}, ans = 0;
        for (const int &v : arr) ++cnt[v];
        for (int i = 0; i <= 1000; ++i) 
            if (cnt[i] && cnt[i + 1]) ans += cnt[i];
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7.6 MB, 在所有 C++ 提交中击败了48.15% 的用户
```
