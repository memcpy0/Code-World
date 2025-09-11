
Given an array A of non-negative integers, return an array consisting of all the even elements of A, followed by all the odd elements of A. You may return **any answer array that satisfies this condition**.
Example 1:
```
Input: [3,1,2,4]
Output: [2,4,3,1]
The outputs [4,2,3,1], [2,4,1,3], and [4,2,1,3] would also be accepted.
```
Note:
-  1 <= A.length <= 5000
-  0 <= A[i] <= 5000


题意：将一个数组中的偶数全部排在数组前半部分，奇数全部排在后半部分，不要求保持原来的偶数或奇数内部的元素顺序。

---
思路1：排序。时间 $O(n\log n)$ 。代码如下：
```cpp
class Solution {
public:
    vector<int> sortArrayByParity(vector<int>& A) {
        sort(A.begin(), A.end(), [](const int &a, const int &b) {
            if (!(a & 1) && !(b & 1)) return a < b; //a,b都是偶数
            if (!(a & 1)) return true;              //a是偶数
            if (!(b & 1)) return false;             //b是偶数
            return a < b;                           //都是奇数
        });
        return A;
    }
};
```
---
思路2：开一个辅助 `vector` ，先将所有的偶数存进去，再将所有的奇数存进去。时间 $O(2n)$，空间 $O(n)$ 。**好处在于，可以保持原数组中奇数内部、偶数内部的顺序**。代码如下：
```cpp
class Solution {
public:
    vector<int> sortArrayByParity(vector<int>& A) {
        vector<int> B;
        for (const int v : A) if (!(v & 1)) B.push_back(v);
        for (const int v : A) if (v & 1) B.push_back(v);
        return B;
    }
};
```
效率如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了97.23% 的用户
内存消耗：16.1 MB, 在所有 C++ 提交中击败了24.96% 的用户
```

---
思路3：变形一下，用一个循环就可以完成，可以叫做双指针吗？代码如下：
```cpp
class Solution {
public:
    vector<int> sortArrayByParity(vector<int>& A) {
        vector<int> B(A.size());
        int f = 0, b = A.size() - 1;
        for (const int v : A) {
            if (v & 1) B[b--] = v;
            else B[f++] = v;
        }
        return B;
    }
};
```
效率如下，可能是申请 `B` 数组时进行初始化耗费了一部分时间：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了63.41% 的用户
内存消耗：15.9 MB, 在所有 C++ 提交中击败了68.65% 的用户
```

---
思路4：沿用上例的思想，双指针的做法如下。时间 $O(n)$ ，空间 $O(1)$ 。还可以写的更加简单一点，不过没有这么清晰。
```c
class Solution {
public:
    vector<int> sortArrayByParity(vector<int>& A) {
        int i = 0, j = A.size() - 1;
        while (i < j) {
            int f1 = A[i] & 1, f2 = A[j] & 1;
            if (f1 && !f2) {         //i指向奇数, j指向偶数
                swap(A[i], A[j]);
                i++, j--;
            } else if (!f1 && !f2) { //i和j都指向偶数
                i++; //寻找后面的奇数
            } else if (f1 && f2) {   //i和j都指向奇数
                j--; //寻找前面的偶数
            } else i++, j--;         //i指向偶数, j指向奇数
        }
        return A;
    }
};
```
