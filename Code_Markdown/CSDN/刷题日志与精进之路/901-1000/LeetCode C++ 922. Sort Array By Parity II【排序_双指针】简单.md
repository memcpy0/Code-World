Given an array `A` of non-negative integers, half of the integers in `A` are odd, and half of the integers are even. Sort the array so that **whenever `A[i]` is odd, `i` is odd**; and **whenever `A[i]` is even, `i` is even**.

You may return any answer array that satisfies this condition.

 **Example 1:**
```
Input: [4,2,5,7]
Output: [4,5,2,7]
Explanation: [4,7,2,5], [2,5,4,7], [2,7,4,5] would also have been accepted.
```
**Note:**
 - `2 <= A.length <= 20000`
 - `A.length % 2 == 0`
  - `0 <= A[i] <= 1000`
	
题意：给出一个非负整数的数组，一半元素为偶数，另一半为奇数，数组的长度为偶数。要求我们将数组“排序”，使得偶数位存放偶数，奇数位存放奇数。

---
### 解法1 辅助数组
使用辅助数组 `ans` ，遍历 `A` ，用 `e` 和 `o` 分别指向 `res` 中第一个偶数位置和奇数位置，每次遇到 `A` 中的偶数，就将 `res` 上的偶数位 `res[e]` 存放 `A` 的偶数，`e += 2` ；同样处理奇数。

时间效率 $O(n)$ ，空间效率 $O(n)$ 。效率可能浪费在：如果数组已经如题目中的“有序”了，那么我们就等于平白做了功夫，还多用了 $O(n)$ 的辅助空间。
```c
class Solution {
public:
    vector<int> sortArrayByParityII(vector<int>& A) {
        vector<int> res(A.size());
        int e = 0, o = 1; 
        for (const int v : A) {
            if (!(v & 1)) { //偶数值
                res[e] = v;
                e += 2;
            } else { //奇数值
                res[o] = v; 
                o += 2;
            }
        } 
        return res;
    }
};
```
效率如下：
```cpp
执行用时：44 ms, 在所有 C++ 提交中击败了81.20% 的用户
内存消耗：20.5 MB, 在所有 C++ 提交中击败了73.57% 的用户
```

> **20201113 Update** 打卡写的代码：
> ```cpp 
> class Solution { public:
>     vector<int> sortArrayByParityII(vector<int>& A) {
>         int n = A.size(), even = 0, odd = 1;
>         vector<int> ans(n);
>         for (int i = 0; i < n; ++i) {
>             if (A[i] & 1) { ans[odd] = A[i]; odd += 2; }
>             else { ans[even] = A[i]; even += 2; }
>         }
>         return ans;
>     } 
>  }; 
>  ```

 
如果这样写，效率会低一些：
```cpp
class Solution {
public:
    vector<int> sortArrayByParityII(vector<int>& A) {
        vector<int> ans; //odd指向奇数,even指向偶数
        int odd = 0, even = 0, n = A.size();
        for (int i = 0; i < n; ++i) {
            if (i & 1) { //奇数位置
                while (odd < n && !(A[odd] & 1)) ++odd; //寻找奇数值
                if (odd < n) ans.push_back(A[odd++]);
            } else { //偶数位置
                while (even < n && (A[even] & 1)) ++even; //寻找偶数值
                if (even < n) ans.push_back(A[even++]);
            }
        }
        return ans;
    }
};
```
执行结果如下：
```cpp
执行用时：48 ms, 在所有 C++ 提交中击败了60.05% 的用户
内存消耗：21.6 MB, 在所有 C++ 提交中击败了20.08% 的用户
```

---
### 解法2 双指针交换
用 `i` 从头扫描，如果发现偶数没有放在偶数位置上，就停下来；用 `j` 从尾扫描，如果发现奇数没有出现在奇数位置上面，就停下来；然后**交换两个元素**。如果 `i` 或 `j` 已经越过界限，就说明偶数或者奇数处在正确的位置上，**由于一半奇数一半偶数，可以推断出另外一半必然处在正确的位置上，及时退出循环**。$O(n)$ 时间，$O(1)$ 空间，避免了初始“有序”的浪费（这时仅仅用 `i` 扫描数组 `1` 遍）。代码如下：
```cpp
class Solution {
public:
    vector<int> sortArrayByParityII(vector<int>& A) {
        int len = A.size(), i = 0, j = len - 1; //i指向偶数位置,j指向奇数位置
        while (true) {
            while (i <= len - 2 && !(A[i] & 1)) i += 2;
            if (i > len - 2) break;
            while (j >= 1 && (A[j] & 1)) j -= 2;
            if (j < 1) break;
            swap(A[i], A[j]);
        }
        return A;
    }
};
```
效率反而比较低，可能是由于比较操作太多的缘故：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了36.91% 的用户
内存消耗：20.6 MB, 在所有 C++ 提交中击败了48.87% 的用户
```
改成如下形式：
```cpp
class Solution {
public:
    vector<int> sortArrayByParityII(vector<int>& A) {
        int len = A.size(), i = 0, j = 1; //i指向偶数位置,j指向奇数位置
        while (true) {
            while (i < len && !(A[i] & 1)) i += 2;
            while (j < len && (A[j] & 1)) j += 2;
            if (i < len && j < len) swap(A[i], A[j]);
            else break;
        }
        return A;
    }
};
```
效率高一点：
```cpp
执行用时：44 ms, 在所有 C++ 提交中击败了81.20% 的用户
内存消耗：20.4 MB, 在所有 C++ 提交中击败了84.63% 的用户
```
