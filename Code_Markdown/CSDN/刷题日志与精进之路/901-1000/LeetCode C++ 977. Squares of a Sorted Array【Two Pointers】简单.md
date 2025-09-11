Given an array of integers `A` sorted in non-decreasing order, return an array of the squares of each number, also in sorted non-decreasing order.

 **Example 1:**
```
Input: [-4,-1,0,3,10]
Output: [0,1,9,16,100]
```

**Example 2:**
```
Input: [-7,-3,2,3,11]
Output: [4,9,9,49,121]
```
**Note:**
- `1 <= A.length <= 10000`
 - `-10000 <= A[i] <= 10000`
 - `A` is sorted in non-decreasing order.

题意：给出一个递增排序的数列 `A` ，平方其中的每个数，求出有序的平方数序列。。

---
思路1：注意到 `A` 的大小最大仅为 `10000` ，所以第一种思路就是排序。$O(n\log n)$ ，效率比较低。
```c
class Solution {
public:
    vector<int> sortedSquares(vector<int>& A) {
        for (auto &v : A) v *= v;
        sort(A.begin(), A.end());
        return A;
    }
};
```
效率：
```cpp
执行用时：132 ms, 在所有 C++ 提交中击败了35.72% 的用户
内存消耗：25.3 MB, 在所有 C++ 提交中击败了40.62% 的用户
```

---
思路2：**双指针+辅助数组**。这个题中的数组元素是整数范围，可能全为正数、负数或者有正有负。`j` 指向 `A` 数组中的第一个正数，`i` 指向 `A` 中的最后一个负数：
- 考虑全为正数，那么 `j` 就为 `0` ，`i` 为 `-1` ，那么只需要从前往后将数组元素全部平方即可；
- 全为负数，那么 `j` 指向 `n` ，`i` 为 `n-1` ，只需要从后往前平方，然后翻转数组即可；
- 有正有负，**此时需要使用辅助数组**。 `j` 指向正数的第一个数，`i` 指向负数的最后一个数，然后不断比较平方数的大小，**按照正负数平方从小到大**的顺序进行复制，分别将 `i` 向前和 `j` 向后移动。最后将剩下的元素依次复制过来即可。
```c
class Solution {
public:
    vector<int> sortedSquares(vector<int>& A) {
        int n = A.size(), i, j = 0;
        while (j < n && A[j] < 0) ++j; //j指向第一个非负数
        i = j - 1; //i指向负数的最后一个数
        if (i == -1 && j == 0) { //全是正数
            for (int &v : A) v *= v;
            return A;
        }
        if (i == n - 1 && j == n) { //全是负数
            for (int &v : A) v *= v;
            reverse(A.begin(), A.end());
            return A;
        } 
        vector<int> B; //有正有负
        while (i >= 0 && j < n) {
            int u = A[i] * A[i], v = A[j] * A[j];
            if (u <= v) {
                B.push_back(u);
                --i;
            } else {
                B.push_back(v);
                ++j;
            }
        }
        while (i >= 0) { //还有负数
            B.push_back(A[i] * A[i]);
            --i;
        }
        while (j < n) { //还有正数
            B.push_back(A[j] * A[j]);
            ++j;
        }
        return B;
    }
};
```
写得简单一点，按照**正负数平方从大到小**的顺序。双指针 `l, r` 分别指向 `A` 数组的最左端（最小的数）和最右端（最大的数）：
- 如果指向的数都为负数（即整个数组为负数），此时 `A[l]` 的平方是最大的数，复制到 `B` 数组的最右端，然后不断将 `l` 向 `r` 移动；
- 如果指向的数都为正数，则 `A[r]` 的平方是最大值，复制到 `B` 数组的最右端，然后不断将 `r` 向左移动；
- 有正有负，直接比较它们所指的数的平方（最大的数存在于两者之一）的大小，然后复制到 `B` 数组的最右端。
- **事实上归纳起来只有一种情况**，比较 `A[l] * A[l]` 和 `A[r] * A[r]` 的大小，然后复制到 `B` 数组的右端。
```c
class Solution {
public:
    vector<int> sortedSquares(vector<int>& A) {
        int l = 0, r = A.size() - 1, t = r;
        vector<int> B(A.size());
        while (l <= r) {
            int u = A[l] * A[l], v = A[r] * A[r];
            if (u >= v) {
                B[t--] = u;
                ++l;
            } else {
                B[t--] = v;
                --r;
            }
        }
        return B;
    }
};
```
这样更加简单，不用考虑可能剩下的数，效率更高：
```cpp
执行用时：64 ms, 在所有 C++ 提交中击败了71.70% 的用户
内存消耗：25 MB, 在所有 C++ 提交中击败了59.94% 的用户
```

**20201016 Update**：今天打卡的题目，写的Java代码如下：
```java
class Solution {
    public int[] sortedSquares(int[] A) {
        int[] ans = new int[A.length];
        int left = 0, right = A.length - 1, index = right;
        while (left <= right) {
            int v2l = A[left] * A[left], v2r = A[right] * A[right];
            if (v2l >= v2r) { ans[index--] = v2l; ++left; }
            else { ans[index--] = v2r; --right; }
        }
        return ans;
    }
}
```
效率如下：
```java
执行用时：1 ms, 在所有 Java 提交中击败了100.00% 的用户
内存消耗：40.6 MB, 在所有 Java 提交中击败了40.95% 的用户
```
