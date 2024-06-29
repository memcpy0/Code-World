> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091401726.png)

给定 `n` 个非负整数表示每个宽度为 `1` 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。

**示例 1：**

![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/10/22/rainwatertrap.png)

**输入：**height = [0,1,0,2,1,0,1,3,2,1,2,1]
**输出：**6
**解释：**上面是由数组 [0,1,0,2,1,0,1,3,2,1,2,1] 表示的高度图，在这种情况下，可以接 6 个单位的雨水（蓝色部分表示雨水）。 

**示例 2：**

**输入：**height = [4,2,0,3,2,5]
**输出：**9

**提示：**

- `n == height.length`
- `1 <= n <= 2 * 10^4`
- `0 <= height[i] <= 10^5`

---
相似题目

[11. 盛最多水的容器](https://leetcode.cn/problems/container-with-most-water/)

中等

[除自身以外数组的乘积](https://leetcode.cn/problems/product-of-array-except-self/)

中等

[407. 接雨水 II](https://leetcode.cn/problems/trapping-rain-water-ii/) 三维接;

困难

方法一：动态规划
对于下标 iii，下雨后水能到达的最大高度等于下标 iii 两边的最大高度的最小值，下标 iii 处能接的雨水量等于下标 iii 处的水能到达的最大高度减去 height[i]\textit{height}[i]height[i]。

朴素的做法是对于数组 height\textit{height}height 中的每个元素，分别向左和向右扫描并记录左边和右边的最大高度，然后计算每个下标位置能接的雨水量。假设数组 height\textit{height}height 的长度为 nnn，该做法需要对每个下标位置使用 O(n)O(n)O(n) 的时间向两边扫描并得到最大高度，因此总时间复杂度是 O(n2)O(n^2)O(n 
2
 )。

上述做法的时间复杂度较高是因为需要对每个下标位置都向两边扫描。如果已经知道每个位置两边的最大高度，则可以在 O(n)O(n)O(n) 的时间内得到能接的雨水总量。使用动态规划的方法，可以在 O(n)O(n)O(n) 的时间内预处理得到每个位置两边的最大高度。

创建两个长度为 nnn 的数组 leftMax\textit{leftMax}leftMax 和 rightMax\textit{rightMax}rightMax。对于 0≤i<n0 \le i<n0≤i<n，leftMax[i]\textit{leftMax}[i]leftMax[i] 表示下标 iii 及其左边的位置中，height\textit{height}height 的最大高度，rightMax[i]\textit{rightMax}[i]rightMax[i] 表示下标 iii 及其右边的位置中，height\textit{height}height 的最大高度。

显然，leftMax[0]=height[0]\textit{leftMax}[0]=\textit{height}[0]leftMax[0]=height[0]，rightMax[n−1]=height[n−1]\textit{rightMax}[n-1]=\textit{height}[n-1]rightMax[n−1]=height[n−1]。两个数组的其余元素的计算如下：

当 1≤i≤n−11 \le i \le n-11≤i≤n−1 时，leftMax[i]=max⁡(leftMax[i−1],height[i])\textit{leftMax}[i]=\max(\textit{leftMax}[i-1], \textit{height}[i])leftMax[i]=max(leftMax[i−1],height[i])；

当 0≤i≤n−20 \le i \le n-20≤i≤n−2 时，rightMax[i]=max⁡(rightMax[i+1],height[i])\textit{rightMax}[i]=\max(\textit{rightMax}[i+1], \textit{height}[i])rightMax[i]=max(rightMax[i+1],height[i])。

因此可以正向遍历数组 height\textit{height}height 得到数组 leftMax\textit{leftMax}leftMax 的每个元素值，反向遍历数组 height\textit{height}height 得到数组 rightMax\textit{rightMax}rightMax 的每个元素值。

在得到数组 leftMax\textit{leftMax}leftMax 和 rightMax\textit{rightMax}rightMax 的每个元素值之后，对于 0≤i<n0 \le i<n0≤i<n，下标 iii 处能接的雨水量等于 min⁡(leftMax[i],rightMax[i])−height[i]\min(\textit{leftMax}[i],\textit{rightMax}[i])-\textit{height}[i]min(leftMax[i],rightMax[i])−height[i]。遍历每个下标位置即可得到能接的雨水总量。

动态规划做法可以由下图体现。

 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307051156142.png)

class Solution {
    public int trap(int[] height) {
        int n = height.length;
        if (n == 0) {
            return 0;
        }

        int[] leftMax = new int[n];
        leftMax[0] = height[0];
        for (int i = 1; i < n; ++i) {
            leftMax[i] = Math.max(leftMax[i - 1], height[i]);
        }

        int[] rightMax = new int[n];
        rightMax[n - 1] = height[n - 1];
        for (int i = n - 2; i >= 0; --i) {
            rightMax[i] = Math.max(rightMax[i + 1], height[i]);
        }

        int ans = 0;
        for (int i = 0; i < n; ++i) {
            ans += Math.min(leftMax[i], rightMax[i]) - height[i];
        }
        return ans;
    }
}
复杂度分析

时间复杂度：O(n)O(n)O(n)，其中 nnn 是数组 height\textit{height}height 的长度。计算数组 leftMax\textit{leftMax}leftMax 和 rightMax\textit{rightMax}rightMax 的元素值各需要遍历数组 height\textit{height}height 一次，计算能接的雨水总量还需要遍历一次。

空间复杂度：O(n)O(n)O(n)，其中 nnn 是数组 height\textit{height}height 的长度。需要创建两个长度为 nnn 的数组 leftMax\textit{leftMax}leftMax 和 rightMax\textit{rightMax}rightMax。

方法二：单调栈
除了计算并存储每个位置两边的最大高度以外，也可以用单调栈计算能接的雨水总量。

维护一个单调栈，单调栈存储的是下标，满足从栈底到栈顶的下标对应的数组 height\textit{height}height 中的元素递减。

从左到右遍历数组，遍历到下标 iii 时，如果栈内至少有两个元素，记栈顶元素为 top\textit{top}top，top\textit{top}top 的下面一个元素是 left\textit{left}left，则一定有 height[left]≥height[top]\textit{height}[\textit{left}] \ge \textit{height}[\textit{top}]height[left]≥height[top]。如果 height[i]>height[top]\textit{height}[i]>\textit{height}[\textit{top}]height[i]>height[top]，则得到一个可以接雨水的区域，该区域的宽度是 i−left−1i-\textit{left}-1i−left−1，高度是 min⁡(height[left],height[i])−height[top]\min(\textit{height}[\textit{left}],\textit{height}[i])-\textit{height}[\textit{top}]min(height[left],height[i])−height[top]，根据宽度和高度即可计算得到该区域能接的雨水量。

为了得到 left\textit{left}left，需要将 top\textit{top}top 出栈。在对 top\textit{top}top 计算能接的雨水量之后，left\textit{left}left 变成新的 top\textit{top}top，重复上述操作，直到栈变为空，或者栈顶下标对应的 height\textit{height}height 中的元素大于或等于 height[i]\textit{height}[i]height[i]。

在对下标 iii 处计算能接的雨水量之后，将 iii 入栈，继续遍历后面的下标，计算能接的雨水量。遍历结束之后即可得到能接的雨水总量。

下面用一个例子 height=[0,1,0,2,1,0,1,3,2,1,2,1]\textit{height}=[0,1,0,2,1,0,1,3,2,1,2,1]height=[0,1,0,2,1,0,1,3,2,1,2,1] 来帮助读者理解单调栈的做法。


1 / 21
Java
Golang
JavaScript
Python3
C++
C
class Solution {
    public int trap(int[] height) {
        int ans = 0;
        Deque<Integer> stack = new LinkedList<Integer>();
        int n = height.length;
        for (int i = 0; i < n; ++i) {
            while (!stack.isEmpty() && height[i] > height[stack.peek()]) {
                int top = stack.pop();
                if (stack.isEmpty()) {
                    break;
                }
                int left = stack.peek();
                int currWidth = i - left - 1;
                int currHeight = Math.min(height[left], height[i]) - height[top];
                ans += currWidth * currHeight;
            }
            stack.push(i);
        }
        return ans;
    }
}
复杂度分析

时间复杂度：O(n)O(n)O(n)，其中 nnn 是数组 height\textit{height}height 的长度。从 000 到 n−1n-1n−1 的每个下标最多只会入栈和出栈各一次。

空间复杂度：O(n)O(n)O(n)，其中 nnn 是数组 height\textit{height}height 的长度。空间复杂度主要取决于栈空间，栈的大小不会超过 nnn。

方法三：双指针
动态规划的做法中，需要维护两个数组 leftMax\textit{leftMax}leftMax 和 rightMax\textit{rightMax}rightMax，因此空间复杂度是 O(n)O(n)O(n)。是否可以将空间复杂度降到 O(1)O(1)O(1)？

注意到下标 iii 处能接的雨水量由 leftMax[i]\textit{leftMax}[i]leftMax[i] 和 rightMax[i]\textit{rightMax}[i]rightMax[i] 中的最小值决定。由于数组 leftMax\textit{leftMax}leftMax 是从左往右计算，数组 rightMax\textit{rightMax}rightMax 是从右往左计算，因此可以使用双指针和两个变量代替两个数组。

维护两个指针 left\textit{left}left 和 right\textit{right}right，以及两个变量 leftMax\textit{leftMax}leftMax 和 rightMax\textit{rightMax}rightMax，初始时 left=0,right=n−1,leftMax=0,rightMax=0\textit{left}=0,\textit{right}=n-1,\textit{leftMax}=0,\textit{rightMax}=0left=0,right=n−1,leftMax=0,rightMax=0。指针 left\textit{left}left 只会向右移动，指针 right\textit{right}right 只会向左移动，在移动指针的过程中维护两个变量 leftMax\textit{leftMax}leftMax 和 rightMax\textit{rightMax}rightMax 的值。

当两个指针没有相遇时，进行如下操作：

使用 height[left]\textit{height}[\textit{left}]height[left] 和 height[right]\textit{height}[\textit{right}]height[right] 的值更新 leftMax\textit{leftMax}leftMax 和 rightMax\textit{rightMax}rightMax 的值；

如果 height[left]<height[right]\textit{height}[\textit{left}]<\textit{height}[\textit{right}]height[left]<height[right]，则必有 leftMax<rightMax\textit{leftMax}<\textit{rightMax}leftMax<rightMax，下标 left\textit{left}left 处能接的雨水量等于 leftMax−height[left]\textit{leftMax}-\textit{height}[\textit{left}]leftMax−height[left]，将下标 left\textit{left}left 处能接的雨水量加到能接的雨水总量，然后将 left\textit{left}left 加 111（即向右移动一位）；

如果 height[left]≥height[right]\textit{height}[\textit{left}] \ge \textit{height}[\textit{right}]height[left]≥height[right]，则必有 leftMax≥rightMax\textit{leftMax} \ge \textit{rightMax}leftMax≥rightMax，下标 right\textit{right}right 处能接的雨水量等于 rightMax−height[right]\textit{rightMax}-\textit{height}[\textit{right}]rightMax−height[right]，将下标 right\textit{right}right 处能接的雨水量加到能接的雨水总量，然后将 right\textit{right}right 减 111（即向左移动一位）。

当两个指针相遇时，即可得到能接的雨水总量。

下面用一个例子 height=[0,1,0,2,1,0,1,3,2,1,2,1]\textit{height}=[0,1,0,2,1,0,1,3,2,1,2,1]height=[0,1,0,2,1,0,1,3,2,1,2,1] 来帮助读者理解双指针的做法。


1 / 12
Java
Golang
JavaScript
Python3
C++
C
class Solution {
    public int trap(int[] height) {
        int ans = 0;
        int left = 0, right = height.length - 1;
        int leftMax = 0, rightMax = 0;
        while (left < right) {
            leftMax = Math.max(leftMax, height[left]);
            rightMax = Math.max(rightMax, height[right]);
            if (height[left] < height[right]) {
                ans += leftMax - height[left];
                ++left;
            } else {
                ans += rightMax - height[right];
                --right;
            }
        }
        return ans;
    }
}
复杂度分析

时间复杂度：O(n)O(n)O(n)，其中 nnn 是数组 height\textit{height}height 的长度。两个指针的移动总次数不超过 nnn。

空间复杂度：O(1)O(1)O(1)。只需要使用常数的额外空间。

作者：力扣官方题解
链接：https://leetcode.cn/problems/trapping-rain-water/solutions/692342/jie-yu-shui-by-leetcode-solution-tuvc/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

可算看懂了，原来双指针同时开两个柱子接水。大家题解没说清楚，害得我也没看懂。 对于每一个柱子接的水，那么它能接的水=min(左右两边最高柱子）-当前柱子高度，这个公式没有问题。同样的，两根柱子要一起求接水，同样要知道它们左右两边最大值的较小值。

问题就在这，假设两柱子分别为 i，j。那么就有 iLeftMax,iRightMax,jLeftMx,jRightMax 这个变量。由于 j>i ，故 jLeftMax>=iLeftMax，iRigthMax>=jRightMax.

那么，如果 iLeftMax>jRightMax，则必有 jLeftMax >= jRightMax，所有我们能接 j 点的水。

如果 jRightMax>iLeftMax，则必有 iRightMax >= iLeftMax，所以我们能接 i 点的水。

而上面我们实际上只用到了 iLeftMax，jRightMax 两个变量，故我们维护这两个即可。（题解都没说清楚，就说个 LeftMax，RightMax，谁知道为什么就可以这么做了。)

490

展示 40 条回复

回复

[![](https://assets.leetcode.cn/aliyun-lc-upload/users/nan-shou-3/avatar_1560160629.png)](https://leetcode.cn/u/nan-shou-3/)

[南兽也要努力变强](https://leetcode.cn/u/nan-shou-3/)L1

发布于 北京

2022.08.16

对双指针的理解：

left从左向右遍历，right从右向左遍历；

则对left来说，leftMax一定准确，rightMax不一定准确，因为区间（left, right）的值还没有遍历，但是left的rightMax一定 >= right的rightMax，所以只要leftMax < rightMax时，我们不关系left的rightMax是多少了，因为它肯定比leftMax大，我们可以直接计算出left的存水量leftMax - nums[left];

对right来说，rightMax一定准确，leftMax不一定准确，因为区间（left, right）的值还没有遍历，但是right的leftMax一定 >= left的leftMax，所以只要leftMax >= rightMax时，我们不关系right的leftMax是多少了，因为它肯定比rightMax大，我们可以直接计算出right的存水量rightMax - nums[right];

136

展示 8 条回复

回复

[![](https://assets.leetcode.cn/aliyun-lc-upload/users/inuter/avatar_1588929713.png)](https://leetcode.cn/u/inuter/)

[inuter](https://leetcode.cn/u/inuter/)

![2022 年度勋章](https://pic.leetcode.cn/1672038422-MIefjQ-2022%E5%B9%B4%E5%BA%A6%E5%BE%BD%E7%AB%A0%20sm.png)

L2

发布于 江西

2022.05.06

【方法三必有的解释】不知道你是否和我一样，对方法三的 “必有” 感到困惑：

- 如果 `height[left] < height[right]`，则必有 `leftMax < rightMax`
- 如果 `height[left] >= height[right]`，则必有 `leftMax >= rightMax`

这是因为我们忽视了代码中的一个细节：

```javascript
if (height[left] < height[right]) {
    left++;
} else {
    right--;
}
```

如果当前轮的双指针为 `left` 和 `right`，那么上一轮要么是 `left - 1` 和 `right`，要么是 `left` 和 `right + 1`。

- 如果移动的是 `left` 指针，即 `left - 1` -> `left`，那么 `height[0, left- 1] < height[right]` 且 `height[right] = rightMax`
- 如果移动的是 `right` 指针，即 `right` <- `right + 1`，那么 `height[left] >= height[right + 1, n - 1]` 且 `height[left] = leftMax`

现在，我们再回看官方的描述： 如果 `height[left] < height[right]`，则必有 `leftMax < rightMax`

- 假设上一轮移动的是 `left` 指针，有 `height[0, left- 1] < height[right]`，又 `height[left] < height[right]`，则 `height[0, left] < height[right] = rightMax`
- 假设上一轮移动的是 `right` 指针，有 `height[left] >= height[right + 1, n - 1]`，又 `height[left] < height[right]`，则 `leftMax = height[left] < height[right]`

最后，我觉得虽然官方的代码没有问题，但还是直接比较 `leftMax` 和 `rightMax` 的写法会更好理解：

```javascript
if (leftMax < rightMax) {
    ans += leftMax - height[left];
    left++;
} else {
    ans += rightMax - height[right];
    right--;
}
```
```cpp
class Solution {
public:
    int trap(vector<int>& height) { 
        int ans = 0;
        int left = 0, right = height.size() - 1;
        int leftMax = 0, rightMax = 0;
        while (left < right) {
            leftMax = max(leftMax, height[left]);
            rightMax = max(rightMax, height[right]);
            if (leftMax < rightMax) {
                ans += leftMax - height[left++];
            } else {
                ans += rightMax - height[right--];
            }
        }
        return ans;
    }
};
```