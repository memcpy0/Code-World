Given three integer arrays `arr1`, `arr2` and `arr3` **sorted** in **strictly increasing** order, return a sorted array of **only** the integers that appeared in **all** three arrays. 

**Example 1:**

```haskell
Input: arr1 = [1,2,3,4,5], arr2 = [1,2,5,7,9], arr3 = [1,3,4,5,8]
Output: [1,5]
Explanation: Only 1 and 5 appeared in the three arrays.
```

**Constraints:**
-  `1 <= arr1.length, arr2.length, arr3.length <= 1000`
-  `1 <= arr1[i], arr2[i], arr3[i] <= 2000`

题意：给出三个均为 **严格递增排列** 的整数数组 `arr1`，`arr2` 和 `arr3`。返回一个由 **仅** 在这三个数组中 **同时出现** 的整数所构成的有序数组。
