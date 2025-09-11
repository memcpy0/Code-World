<div class="notranslate"><p>Given four lists A, B, C, D of integer values, compute how many tuples <code>(i, j, k, l)</code> there are such that <code>A[i] + B[j] + C[k] + D[l]</code> is zero.</p>

<p>To make problem a bit easier, all A, B, C, D have same length of N where <code>0 ≤ N ≤ 500</code>. All integers are in the range of -2<sup>28</sup> to 2<sup>28</sup> - 1 and the result is guaranteed to be at most 2<sup>31</sup> - 1.</p>

<p><b>Example:</b></p>

```swift
Input:
A = [ 1, 2]
B = [-2,-1]
C = [-1, 2]
D = [ 0, 2]

Output:
2

Explanation:
The two tuples are:
1. (0, 0, 0, 1) -> A[0] + B[0] + C[0] + D[1] = 1 + (-2) + (-1) + 2 = 0
2. (1, 1, 0, 0) -> A[1] + B[1] + C[0] + D[0] = 2 + (-1) + (-1) + 0 = 0
```

 
题意：给定四个包含整数的数组列表 `A, B, C, D`，计算有多少个元组 `(i, j, k, l)` ，使得 `A[i] + B[j] + C[k] + D[l] = 0` 。

---
这一题是[18. 四数之和](https://leetcode-cn.com/problems/4sum/)的变种，由于不要求得到最终的元组集合，我们可以进一步降低时间复杂度，使用 $O(n^2)$ 时间完成本题。

### 解法1 暴力
$O(n^4)$ 的做法，绝对会超时，写在这里只是为了显示思考过程：
```cpp
class Solution {
public:
    int fourSumCount(vector<int>& A, vector<int>& B, vector<int>& C, vector<int>& D) {
        int ans = 0, na = A.size(), nb = B.size(), nc = C.size(), nd = D.size();
        for (int i = 0; i < na; ++i) {
            for (int j = 0; j < nb; ++j) {
                for (int k = 0; k < nc; ++k) {
                    for (int l = 0; l < nd; ++l)
                        if (A[i] + B[j] + C[k] + D[l] == 0) ++ans;
                }
            }
        }
        return ans;
    }
};
```
---
### 解法2 哈希表
话不多说，看到代码就明白了：
```cpp
class Solution {
public:
    int fourSumCount(vector<int>& A, vector<int>& B, vector<int>& C, vector<int>& D) {
        int ans = 0;
        unordered_map<int, int> rec;
        for (const int& va : A) for (const int& vb : B) ++rec[va + vb];
        for (const int& vc : C) for (const int& vd : D) if (rec.find(-vc - vd) != rec.end()) ans += rec[-vc - vd]; 
        return ans;
    }
};
```
运行结果如下所示：
```cpp
执行用时：400 ms, 在所有 C++ 提交中击败了66.83% 的用户
内存消耗：28.7 MB, 在所有 C++ 提交中击败了66.73% 的用户
```
Python代码如下：
```py
class Solution:
    def fourSumCount(self, A: List[int], B: List[int], C: List[int], D: List[int]) -> int:
        dic = collections.Counter(a + b for a in A for b in B)
        return sum(dic.get(- c - d, 0) for c in C for d in D)
```
对应的运行结果如下……悲/(ㄒoㄒ)/~~，我大C++连Python都打不过了：
```cpp
执行用时：276 ms, 在所有 Python3 提交中击败了87.80% 的用户
内存消耗：33.9 MB, 在所有 Python3 提交中击败了40.93% 的用户
```
