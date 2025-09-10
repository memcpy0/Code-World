> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given an `m x n` matrix `M` initialized with all `0`'s and an array of operations `ops`, where `ops[i] = [ai, bi]` means `M[x][y]` should be incremented by one for all `0 <= x < ai` and `0 <= y < bi`.

Count and return *the number of maximum integers in the matrix after performing all the operations.*

 

**Example 1:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201111173503789.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
```swift
Input: m = 3, n = 3, ops = [[2,2],[3,3]]
Output: 4
Explanation: The maximum integer in M is 2, and there are four of it in M. So return 4.
```

**Example 2:**

```swift
Input: m = 3, n = 3, ops = [[2,2],[3,3],[3,3],[3,3],[2,2],[3,3],[3,3],[3,3],[2,2],[3,3],[3,3],[3,3]]
Output: 4
```

**Example 3:**

```swift
Input: m = 3, n = 3, ops = []
Output: 9
```

 

**Constraints:**
- `1 <= m, n <= 4 * 10^4`
- `1 <= ops.length <= 10^4`
 - `ops[i].length == 2`
- `1 <= ai <= m`
 - `1 <= bi <= n`
 

题意：给定一个初始元素全部为 `0`，大小为 `m * n` 的矩阵 `M` 以及在 `M` 上的一系列更新操作。操作用二维数组表示，每个操作是一个含有两个正整数 `a` 和 `b` 的数组，含义是将所有符合 `0 <= i < a` 以及 `0 <= j < b` 的元素 `M[i][j]` 的值都增加 `1` 。执行给定的一系列操作后，返回矩阵中含有最大整数的元素个数。

---
### 解法 遍历
乍一看以为需要用二维差分……其实只是一个脑筋急转弯。因为数据范围太大，即使可用二维差分将矩阵操作变为 $O(1)$ 复杂度，但最后恢复为原矩阵仍要 $O(mn)$ 的时间，由于 $m, n$ 的范围是 $[1, 40000]$ ，这样的代价是不能接受的。

仔细一看，每次的操作都针对从左上角 $(0, 0)$ 到 $(a, b)$ 的矩形，这些矩形必然重叠。要做的就是找到最小的 $a$ 和最小的 $b$ ，这一区域中的元素必然是最大整数。返回 $a \times b$ ，即最大整数的元素个数：
```cpp
//C++ version
class Solution {
public:
    int maxCount(int m, int n, vector<vector<int>>& ops) {
        int r = m, c = n;
        for (int i = 0, size = ops.size(); i < size; ++i) {
            r = min(r, ops[i][0]);
            c = min(c, ops[i][1]);
        } 
        return r * c;
    }
}; 
//执行用时：8 ms, 在所有 C++ 提交中击败了91.90% 的用户
//内存消耗：10.6 MB, 在所有 C++ 提交中击败了87.60% 的用户
```
