

Given an initial array `arr`, every day you produce a new array using the array of the previous day.

On the i-th day, you do the following operations on the array of day `i - 1` to produce the array of day `i`:
- If an element is smaller than both its left neighbor and its right neighbor, then this element is incremented.
- If an element is bigger than both its left neighbor and its right neighbor, then this element is decremented.
 - The first and last elements never change.

After some days, the array does not change. Return that final array.

 

**Example 1:**

```haskell
Input: arr = [6,2,3,4]
Output: [6,3,3,4]
Explanation: 
On the first day, the array is changed from [6,2,3,4] to [6,3,3,4].
No more operations can be done to this array.
```

**Example 2:**

```haskell
Input: arr = [1,6,3,4,3,5]
Output: [1,4,4,4,4,5]
Explanation: 
On the first day, the array is changed from [1,6,3,4,3,5] to [1,5,4,3,4,5].
On the second day, the array is changed from [1,5,4,3,4,5] to [1,4,4,4,4,5].
No more operations can be done to this array.
```
 
**Constraints:**
- `3 <= arr.length <= 100`
 - `1 <= arr[i] <= 100`

题意：首先，给你一个初始数组 `arr`。然后，每天都要根据前一天的数组生成一个新的数组。返回最终所得到的数组。


