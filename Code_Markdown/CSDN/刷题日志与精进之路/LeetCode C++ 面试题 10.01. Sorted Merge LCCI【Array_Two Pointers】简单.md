

You are given two sorted arrays, `A` and `B`, where `A` has a large enough buffer at the end to hold `B`. Write a method to merge `B` into `A` in sorted order.

Initially the number of elements in `A` and `B` are `m` and `n` respectively.

**Example:**

```swift
Input:
A = [1,2,3,0,0,0], m = 3
B = [2,5,6],       n = 3

Output: [1,2,2,3,5,6]
```

题意：给定两个排序后的数组 `A` 和 `B`，其中 `A` 的末端有足够的缓冲空间容纳 `B`。 编写一个方法，将 `B` 合并入 `A` 并排序。

---
### 解法 双指针
从 `A` 数组的后端往前归并：
```cpp
class Solution {
public:
    void merge(vector<int>& A, int m, vector<int>& B, int n) { //从后往前合并
        int idx = m + n - 1, pa = m - 1, pb = n - 1;
        while (pa >= 0 && pb >= 0) {
            if (A[pa] >= B[pb]) A[idx--] = A[pa--];
            else A[idx--] = B[pb--];
        }
        //下一行代码不必要
        //如果pa!=0而pb==0,而A数组前面已经有序,无需继续填入;
        //如果pa==0而pb!=0或者pb==0,同样不必要;
        //while (pa >= 0) A[idx--] = A[pa--]; 
        while (pb >= 0) A[idx--] = B[pb--];
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了80.19% 的用户
内存消耗：9.3 MB, 在所有 C++ 提交中击败了7.88% 的用户
```
