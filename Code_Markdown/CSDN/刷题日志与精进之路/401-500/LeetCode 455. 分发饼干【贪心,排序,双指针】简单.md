> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Assume you are an awesome parent and want to give your children some cookies. But, you should give each child **at most one cookie**. Each child i has a greed factor `gi` , which is **the minimum size** of a cookie that the child will be content with; and each cookie `j` has a size `sj` . If `sj >= gi` , we can assign the cookie `j` to the child `i` , and the child `i` will be content. Your goal is to **maximize** the number of your content children and output the maximum number.

**Note:**
- You may assume the greed factor is always **positive**.
- You cannot assign more than one cookie to one child.

**Example 1:**
```java
Input: [1,2,3], [1,1]

Output: 1

Explanation: You have 3 children and 2 cookies. The greed factors of 3 children are 1, 2, 3. 
And even though you have 2 cookies, since their size is both 1, you could only make the child whose greed factor is 1 content.
You need to output 1.
```
**Example 2:**
```java
Input: [1,2], [1,2,3]

Output: 2

Explanation: You have 2 children and 3 cookies. The greed factors of 2 children are 1, 2. 
You have 3 cookies and their sizes are big enough to gratify all of the children, 
You need to output 2.
```
题意：给孩子们一些小饼干，每个孩子**最多**只能给一块饼干。每个孩子都有一个胃口指数 `gi` ，这是能让他满足的饼干的最小尺寸。每块饼干 `j` ，都有一个尺寸 `sj` 。如果 `sj >= gi` ，我们可以将这个饼干 `j` 分配给孩子 `i` ，这个孩子会得到满足。求能够得到满足的最多的孩子数量。

---
### 解法 排序 + 双指针 + 贪心
为了尽可能满足最多数量的孩子，从贪心的角度考虑，**应按照孩子的胃口从小到大的顺序依次满足每个孩子**（，且对于每个孩子，**应选择可以满足这个孩子的胃口且尺寸最小的饼干**。证明如下。

假设有 $m$ 个孩子，胃口值分别是 $g_1$ 到 $g_m$ ，有 $n$ 块饼干，尺寸分别是 $s_1$ 到 $s_n$ ，满足 $g_i \le g_{i+1}$​ 和 $s_j \le s_{j+1}$ ，其中 $1 \le i < m$ ，$1 \le j < n$ 。

假设在对前 $i-1$ 个孩子分配饼干之后，可以满足第 $i$ 个孩子的胃口的最小的饼干是第 $j$ 块饼干，即 **$s_j$ 是剩下的饼干中满足 $g_i \le s_j$ 的最小值**，最优解是将第 $j$ 块饼干分配给第 $i$ 个孩子。如果不这样分配，考虑如下两种情形：
- 如果 $i<m$ 且 $g_{i+1} \le s_j$ 也成立，则**如果将第 $j$ 块饼干分配给第 $i+1$ 个孩子**，且**还有剩余的饼干**，则可以将第 $j+1$ 块饼干分配给第 $i$ 个孩子，**分配的结果不会让更多的孩子被满足**；
- 如果 $j<n$ ，则如果将第 $j+1$ 块饼干分配给第 $i$ 个孩子，当 $g_{i+1} \le s_j$​ 时，可以将第 $j$ 块饼干分配给第 $i+1$ 个孩子，分配的结果不会让更多的孩子被满足；当 $g_{i+1}>s_j$ 时，第 $j$ 块饼干无法分配给任何孩子，因此剩下的可用的饼干少了一块，因此分配的结果不会让更多的孩子被满足，甚至可能因为少了一块可用的饼干而导致更少的孩子被满足。

基于上述分析，可以使用贪心的方法尽可能满足最多数量的孩子。

首先对数组 $g$ 和 $s$ 排序，然后从小到大遍历 $g$ 中的每个元素，对于每个元素找到能满足该元素的 $s$ 中的最小的元素。具体而言，令 $i$ 是 $g$ 的下标，$j$ 是 $s$ 的下标，初始时 $i$ 和 $j$ 都为 $0$ ，进行如下操作：对于每个元素 $g[j]$ ，找到未被使用的最小的 $j$ 使得 $g[i] \le s[j]$ ，则 $s[j]$ 可以满足 $g[j]$ 。

由于 $g$ 和 $s$ 已经排好序，因此整个过程只需要对数组 $g$ 和 $s$ 各遍历一次。当两个数组之一遍历结束时，说明所有的孩子都被分配到了饼干，或所有的饼干都已经被分配或被尝试分配（可能有些饼干无法分配给任何孩子），此时被分配到饼干的孩子数量即为可以满足的最多数量。
```cpp
// cpp
class Solution {
public:
    int findContentChildren(vector<int>& g, vector<int>& s) {
        sort(g.begin(), g.end());
        sort(s.begin(), s.end());
        int child = 0, cookie = 0;
        while (child < g.size() && cookie < s.size()) { //当其中一个遍历就结束
            if (s[cookie] >= g[child]) ++child;  //用当前饼干可以满足当前孩子的需求，可以满足的孩子数量+1
            ++cookie;                            //不能满足时饼干太小,抛弃
        }
        return child;
    }
};
// java
class Solution {
    public int findContentChildren(int[] g, int[] s) {
        Arrays.sort(g);
        Arrays.sort(s);
        int m = g.length, n = s.length;
        int count = 0;
        for (int i = 0, j = 0; i < m && j < n; i++, j++) {
            while (j < n && g[i] > s[j]) {
                j++;
            }
            if (j < n) {
                count++;
            }
        }
        return count;
    }
}
// go
func findContentChildren(g []int, s []int) (ans int) {
    sort.Ints(g)
    sort.Ints(s)
    m, n := len(g), len(s)
    for i, j := 0, 0; i < m && j < n; i++ {
        for j < n && g[i] > s[j] {
            j++
        }
        if j < n {
            ans++
            j++
        }
    }
    return
}
```

> **20201225 Update** 反过来想，用大饼干满足大胃口的孩子，Python代码如下：
> ```py
> class Solution:
>     def findContentChildren(self, g: List[int], s: List[int]) -> int:
>         s.sort(reverse = True) 
>         g.sort(reverse = True) # s,g排序
>         lens, leng = len(s), len(g)
>         ans = si = sj = 0
>         while si < lens and sj < leng:
>             if s[si] >= g[sj]: # 足够大的饼干满足了这个孩子的胃口
>                 ans += 1 # 分给他
>                 si += 1
>                 sj += 1
>             else: # 这个孩子的胃口太大,不给他分配,将现在的饼干分给下一个
>                 sj += 1
>         return ans 
>  ```

**复杂度分析**：
- 时间复杂度：$O(m \log m + n \log n)$ ，其中 $m$ 和 $n$ 分别是数组 $g$ 和 $s$ 的长度。对两个数组排序的时间复杂度是 $O(m \log m + n \log n)$ ，遍历数组的时间复杂度是 $O(m+n)$ ，因此总时间复杂度是 $O(m \log m + n \log n)$ 。
- 空间复杂度：$O(\log m + \log n)$ ，其中 $m$ 和 $n$ 分别是数组 $g$ 和 $s$ 的长度。空间复杂度主要是排序的额外空间开销。
