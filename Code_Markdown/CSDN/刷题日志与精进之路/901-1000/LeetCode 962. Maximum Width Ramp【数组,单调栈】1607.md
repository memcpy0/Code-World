> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定一个整数数组 `A`，_坡_是元组 `(i, j)`，其中  `i < j` 且 `A[i] <= A[j]`。这样的坡的宽度为 `j - i`。

找出 `A` 中的坡的最大宽度，如果不存在，返回 0 。

**示例 1：**

**输入：**[6,0,8,2,1,5]
**输出：**4
**解释：**
最大宽度的坡为 (i, j) = (1, 5): A[1] = 0 且 A[5] = 5.

**示例 2：**

**输入：**[9,8,1,0,1,9,4,0,4,1]
**输出：**7
**解释：**
最大宽度的坡为 (i, j) = (2, 9): A[2] = 1 且 A[9] = 1.

**提示：**

1. `2 <= A.length <= 50000`
2. `0 <= A[i] <= 50000`

解法一
首先把A数组中的以A[0]开头的递减序列抽取出来，我们最后要求的最大的宽度坡一定是以这个序列中的某一个i为坡底的，我们反证一下

假设存在某个元素位置k不存在于上面的递减序列中，且有最大宽度j-k，这也就说明k位置的元素一定是小于k前面所有的元素的，否则就会有更长的宽度，但是既然k小于前面所有的元素，那么k就一定会被加入到序列中，与假设矛盾，所以不存在k，解一定存在递减序列中

这样的话我们可以逆向遍历数组，每次遇到元素大于栈顶的就可以计算宽度，然后将栈顶弹出，因为是逆序遍历的，所以这个宽度一定是栈顶这个坡底i能形成的最大宽度了， 逆序遍历再往前的话即使大于这个栈顶，形成的宽度也只会减小，所以这个栈顶是可以直接pop出去的，我们遍历所有的坡底求最大值就行了，时间复杂度O(N)

Java
public int maxWidthRamp(int[] A) {
    Deque<Integer> stack=new ArrayDeque<>();
    int res=0;
    for(int i=0;i<A.length;i++){
        if(stack.isEmpty() || A[stack.peek()]>A[i]){
            stack.push(i);
        }
    }
    for(int i=A.length-1;i>=0;i--){
        while(!stack.isEmpty() && A[stack.peek()]<=A[i]){
            int cur=stack.pop();
            res=Math.max(res,i-cur);
        }
    }
    return res;
}
解法二
二分的思路，和上面一样，先构建一个以A[0]开头的递减序列，这里面就是我们所有的坡底，然后我们可以遍历所有的元素，然后在这个单调序列中寻找第一个小于等于当前元素的index，这两个构成的宽度就是当前元素所能形成的最大宽度，我们求出所有的最大宽度取一个最值就可以了,时间复杂度O(NlogN)

Go
func maxWidthRamp(A []int) int {
    var order [][]int
    order = append(order, []int{0, A[0]})
    //构建递减序列
    for i := 1; i < len(A); i++ {
        if A[i] < order[len(order)-1][1] {
            order = append(order, []int{i, A[i]})
        }
    }
    res := 0
    for j, target := range A {
        i := binarySearch(order, target)
        res = Max(res, j-i)
    }
    return res
}

//找第一个小于等于target的值
func binarySearch(num [][]int, target int) int {
    left := 0
    right := len(num) - 1
    for left < right {
        mid := left + (right-left)/2
        if num[mid][1] > target {
            left = mid + 1 //注意是递减序列
        } else {
            right = mid
        }
    }
    return num[left][0]
}

func Max(a, b int) int {
    if a > b {
        return a
    }
    return b
}

作者：Resolmi
链接：https://leetcode.cn/problems/maximum-width-ramp/solutions/243679/java-dan-diao-zhan-er-fen-jie-fa-chang-shi-jie-shi/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。