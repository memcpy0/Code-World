We have an array `A` of integers, and an array `queries` of queries.

For the `i`-th query `val = queries[i][0], index = queries[i][1]`, we add `val` to `A[index]`.  Then, the answer to the `i`-th query is the sum of the even values of `A`.

*(Here, the given `index = queries[i][1]` is a 0-based index, and each query permanently modifies the array `A`.)*

Return the answer to all queries.  Your `answer` array should have `answer[i]` as the answer to the `i`-th query.

**Example 1:**

```swift
Input: A = [1,2,3,4], queries = [[1,0],[-3,1],[-4,0],[2,3]]
Output: [8,6,2,4]
Explanation: 
At the beginning, the array is [1,2,3,4].
After adding 1 to A[0], the array is [2,2,3,4], and the sum of even values is 2 + 2 + 4 = 8.
After adding -3 to A[1], the array is [2,-1,3,4], and the sum of even values is 2 + 4 = 6.
After adding -4 to A[0], the array is [-2,-1,3,4], and the sum of even values is -2 + 4 = 2.
After adding 2 to A[3], the array is [-2,-1,3,6], and the sum of even values is -2 + 6 = 4.
```

 

**Note:**
- `1 <= A.length <= 10000`
-  `-10000 <= A[i] <= 10000`
 - `1 <= queries.length <= 10000`
 - `-10000 <= queries[i][0] <= 10000`
 - `0 <= queries[i][1] < A.length`
 
 题意：给出一个整数数组 `A` 和一个查询数组 `queries` 。对于第 `i` 次查询，有 `val = queries[i][0], index = queries[i][1]` ，我们会把 `val` 加到 `A[index]` 上。然后，第 `i` 次查询的答案是 `A` 中偶数值的和。注意，**每次查询都会永久修改数组 `A`**。最后，返回所有查询的答案数组 `answer` ，`answer[i]` 为第 `i` 次查询的答案。
 
---
### 解法1 暴力双重循环
每次查询和修改之后，遍历整个 `A` 数组，得到所有偶数的和：
```cpp
class Solution {
public: 
    vector<int> sumEvenAfterQueries(vector<int>& A, vector<vector<int>>& queries) {
        vector<int> ans;
        for (const vector<int> &q : queries) {
            A[q[1]] += q[0];
            int evenValSum = 0;
            for (const int &v : A) 
                evenValSum += (v & 1 ? 0 : v);
            ans.push_back(evenValSum);
        }
        return ans;
    }
};
```
这种做法超出时间限制了。

---
### 解法2 预处理
先计算出 `A` 中所有偶数的和 `preEvenValSum` 。然后遍历查询数组进行修改，存在四种情况：
- 原来是奇数，现在也是奇数：不做操作；
- 原来是奇数，现在是偶数：加上新的偶数；
- 原来是偶数，现在是奇数：减去旧的偶数；
- 原来是偶数，现在是偶数：加上对应的 `val = queries[i][0]` 。

具体代码如下：
```cpp
class Solution {
public:
    vector<int> sumEvenAfterQueries(vector<int>& A, vector<vector<int>>& queries) {
        vector<int> ans;
        int preEvenValSum = 0; //预先计算出A中偶数的和
        for (const int &v : A) preEvenValSum += (v & 1 ? 0 : v);
        for (const vector<int> &q : queries) {
            int val = q[0], index = q[1], t = A[index];
            A[index] += val; //永久修改数组A
            if ((t & 1)) { //原数是奇数
                if (A[index] & 1)             ; //新数是奇数
                else preEvenValSum += A[index]; //新数是偶数,预处理结果+新的偶数
            } else { //原数是偶数
                if (A[index] & 1) preEvenValSum -= t; //新数是奇数,预处理结果-原来的偶数
                else preEvenValSum += val;            //新数是偶数,预处理结果+新的变动
            } 
            ans.push_back(preEvenValSum); 
        }
        return ans;
    }
};
```
这种做法复杂度是线性的，提交后运行效率如下：
```cpp
执行用时：200 ms, 在所有 C++ 提交中击败了36.56% 的用户
内存消耗：37.2 MB, 在所有 C++ 提交中击败了19.60% 的用户
```
更进一步，我们可以缩减上述情况：对于每次查询，如果 `A[index]` 是偶数，预处理结果减去 `A[index]` （对应**原数是偶数、新数是奇数**的状态变化）；如果新的 `A[index] + val` 是偶数，预处理结果加上 `A[index] + val` （对应**原数是奇数、新数是偶数**和**原数是偶数、新数是偶数**的状态变化）。
```cpp
class Solution {
public:
    vector<int> sumEvenAfterQueries(vector<int>& A, vector<vector<int>>& queries) {
        vector<int> ans;
        int preEvenValSum = 0; //预先计算出A中偶数的和
        for (const int &v : A) preEvenValSum += (v & 1 ? 0 : v);
        for (const vector<int> &q : queries) {
            int val = q[0], index = q[1];
            if ((A[index] & 1) == 0) preEvenValSum -= A[index];
            A[index] += val;   //永久修改数组A
            if ((A[index] & 1) == 0) preEvenValSum += A[index];
            ans.push_back(preEvenValSum); 
        }
        return ans;
    }
};
```
执行效率如下：
```cpp
执行用时：176 ms, 在所有 C++ 提交中击败了89.72% 的用户
内存消耗：37.2 MB, 在所有 C++ 提交中击败了21.40% 的用户
```
