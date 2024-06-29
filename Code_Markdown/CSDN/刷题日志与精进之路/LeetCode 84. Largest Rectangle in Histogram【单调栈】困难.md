<p>Given an array of integers <code>heights</code> representing the histogram's bar height where the width of each bar is <code>1</code>, return <em>the area of the largest rectangle in the histogram</em>.</p>

 
<p><strong>Example 1:</strong></p>
<img style="width: 522px; height: 242px;" src="https://assets.leetcode.com/uploads/2021/01/04/histogram.jpg" alt="">
 

```clike
Input: heights = [2,1,5,6,2,3]
Output: 10
Explanation: The above is a histogram where width of each bar is 1.
The largest rectangle is shown in the red area, which has an area = 10 units.
```

 
<p><strong>Example 2:</strong></p>
<img style="width: 202px; height: 362px;" src="https://assets.leetcode.com/uploads/2021/01/04/histogram-1.jpg" alt="">

```clike
Input: heights = [2,4]
Output: 4
```
<p><strong>Constraints:</strong></p>
 
<ul>
	<li><code>1 &lt;= heights.length &lt;= 10<sup>5</sup></code></li>
	<li><code>0 &lt;= heights[i] &lt;= 10<sup>4</sup></code></li>
</ul>
 

题意：给定 $n$ 个非负整数，用来表示柱状图中各个柱子的高度。每个柱子彼此相邻，且宽度为 $1$ 。求在该柱状图中，能够勾勒出来的矩形的最大面积。

---
暴力法一定会超时——具体来说就是以当前的柱子高度 `heights[i]` 作为矩形的高 `h` ，向两边遍历，直到两边遇到高度 `< heights[i]` 的柱子为止，此时统计出矩形的宽度 `w` ，求出矩形的面积 `w * h` 。计算这些矩形面积的最大值即可。时间复杂度为 $O(n^2)$ 。

这种做法的正确性来自于，最大矩形的高度一定是某个柱子的高度，不然相当于没有充分计算矩形的面积，确定了柱子的高度，向两边遍历就能得到矩形的宽度。

### 解法 单调栈
在考虑更优的解法之前，先考虑一个简单的情形，柱形图的柱子从左到右高度单调递增，如例二所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210510001214192.png)
那么答案是多少？显而易见，我们可以尝试以每个柱子的高度作为最终矩形的高度，然后**把宽度扩展到右边界**，得到一个矩形并计算其面积，在所有这样的矩形面积中取最大值。我们可以从左往右计算，第 $0$ 个柱子扩展形成的矩形的宽度是柱子的数量 $n$ ，……，第 $i$ 个柱子扩展形成的矩形的宽度是 $n - i$ ……或者**从右往左计算**也是一样的，此时第 $n - 1$ 个柱子的矩形宽度是 $1$ ，第 $n-2$ 个柱子的矩形宽度**向右扩展**为 $2$ ，……

现在考虑更复杂的情况，如果下一个柱子 `i + 1` 的高度 `heights[i + 1]` 比当前这个小，那么当前柱子形成的矩形就无法利用下个柱子向右扩展，它被**终结**于此——或者说，所有遍历过的、高度大于 `heights[i + 1]` 的矩形，其向右扩展的进程都**终结**于此，**它们的面积已经可以被计算出来**。当然，我们也可以换个角度来想，对于下个柱子形成的矩形，由于高度更低，它反而可以利用到**左侧高度更高的柱子**形成的面积（不过这种面积的利用只限于下个柱子的高度，**超过这个高度的左侧的柱子面积都没有用处**），它向左扩展到高度低于 `heights[i + 1]` 的柱子为止（不过这个左侧高度更低的柱子的面积还无法计算，同样的，柱子 `i + 1`  的右侧可以扩展的面积也无法预料）。

综上所述，如果下个柱子的高度更低，那么左侧所有高度更高的柱子形成的矩形面积，都可以被计算出来，**这些柱子的信息和它们不被需要的那部分面积，都可以被丢弃了**——取而代之的，我们可以使用一个宽度为其自身及左侧所有高度更高的柱子的累计宽度、高度为下个柱子高度的新矩形/柱子加以替代，这样对计算不会产生不利影响，还能够**加速对左侧剩余柱子形成的矩形面积的计算**。如下图所示，柱子 $T$ 的高度更低，因此计算出左侧的两个柱子形成矩形的面积，然后将其信息丢弃，替代以高度为 $2$ 、宽度为 $3$ 的新矩形：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210510010118283.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
都说到这份上了，相信可以看出题目的做法——具体而言，我们维持一个栈，从左至右依次扫描每个柱子，保存高度单调递增的柱子的高度信息，如果当前柱子的高度比栈顶柱子高度更低，则不断取出栈顶，直到栈为空、或者栈顶柱子高度更小。出栈的过程中，我们**从右到左不断累计被弹出的柱子的宽度之和**，每弹出一个柱子，就用其高度乘以当前宽度之和去更新答案。整个出栈过程结束后，我们把一个高度等于当前柱子高度、宽度为累计值的新矩形入栈。

扫描结束后，我们把栈中剩余的矩形依次弹出，同样计算和更新答案。**为简化代码实现，也可以增加一个高度为 $0$ 的柱子，避免扫描结束后栈中有剩余矩形**。整个过程的代码实现如下：
```cpp
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        int n = heights.size() + 1, ans = 0;
        vector<pair<int, int>> st;
        for (int i = 0; i < n; ++i) {
            int h = i != n - 1 ? heights[i] : 0;
            if (st.empty() || h > st.back().first) {
                st.push_back({h, 1});
            } else {
                int width = 0;
                while (!st.empty() && st.back().first >= h) {
                    width += st.back().second;
                    ans = max(ans, width * st.back().first);
                    st.pop_back();  
                }
                st.push_back({h, width + 1}); 
            }
        }
        return ans;
    }
};
```
随手一提交，效率如下：
```cpp
执行用时：112 ms, 在所有 C++ 提交中击败了96.79% 的用户
内存消耗：60 MB, 在所有 C++ 提交中击败了96.51% 的用户
```
