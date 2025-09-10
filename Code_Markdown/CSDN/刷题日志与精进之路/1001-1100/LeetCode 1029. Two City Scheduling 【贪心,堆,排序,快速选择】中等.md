> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

公司计划面试 `2n` 人。给你一个数组 `costs` ，其中 `costs[i] = [aCosti, bCosti]` 。第 `i` 人飞往 `a` 市的费用为 `aCosti` ，飞往 `b` 市的费用为 `bCosti` 。

返回将每个人都飞到 `a` 、`b` 中某座城市的最低费用，要求每个城市都有 `n` 人抵达。

**示例 1：**
```java
输入：costs = [[10,20],[30,200],[400,50],[30,20]]
输出：110
解释：
第一个人去 a 市，费用为 10。
第二个人去 a 市，费用为 30。
第三个人去 b 市，费用为 50。
第四个人去 b 市，费用为 20。
最低总费用为 10 + 30 + 50 + 20 = 110，每个城市都有一半的人在面试。
```
**示例 2：**
```java
输入：costs = [[259,770],[448,54],[926,667],[184,139],[840,118],[577,469]]
输出：1859
```
**示例 3：**
```java
输入：costs = [[515,563],[451,713],[537,709],[343,819],[855,779],[457,60],[650,359],[631,42]]
输出：3086
```
**提示：**
-   `2 * n == costs.length`
-   `2 <= costs.length <= 100`
-   `costs.length` 为偶数
-   `1 <= aCosti, bCosti <= 1000`

---
> 本题与[2611. Mice and Cheese](https://leetcode.cn/problems/mice-and-cheese)几乎完全相同。额外说一下，本题由于数据量较小，还可使用动态规划解决。

假设公司首先将这 $2N$ 个人全部安排飞往 $B$ 市，再选出 $N$ 个人改变他们的行程，让他们飞往 $A$ 市。如果**选择改变一个人的行程**，那么公司将会**额外付出 `priceA - priceB` 的费用**，这个费用可正可负。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202306081038751.png)
因此最优的方案是，选出 `priceA - priceB` 最小的 $N$ 个人去 $A$ 市，其余人去 $B$ 市。
### 解法1 贪心+排序
具体实现来说，这个解法先**按照 `priceA - priceB` 从小到大排序**，将前 $N$ 个人飞往 $A$ 市，其余人飞往 $B$ 市，并计算出总费用。
```java
class Solution {
    public int twoCitySchedCost(int[][] costs) {
        // sort by a gain which company has by 
        // sending a person to city A and not to city B
        Arrays.sort(costs, (a, b) -> a[0] - a[1] - (b[0] - b[1]));
        int ans = 0, n = costs.length / 2;
        for (int i = 0; i < n; ++i) ans += costs[i][0] + costs[i + n][1];
        return ans;
    }
}
```
代码实现简单，分析一下时空复杂度：
- 时间复杂度：$O(2N \times \log 2N)$ 
- 空间复杂度：$O(1)$

---
### 解法2 贪心+堆
对上述解法使用堆进行优化，我们可以**使用小顶堆**，将差价 `priceA - priceB` 小的排在堆前面，`priceA - priceB` 最小的在堆顶，然后从堆中取出 $N$ 个当前差价 `priceA - priceB` 最小的项，**在 `priceB` 的和之上，加上改变行程付出的代价**。这种做法要将所有项入堆、并出堆 $N$ 个最小的项，因此时间复杂度为 $O(2N \times \log 2N + N\log 2N)$ 。

我们还可**使用大顶堆**，保持大顶堆的大小为 $N$ ：
- 当大顶堆大小小于 $N$ 时直接入堆；
- 否则看当前人员的差价 `priceA - priceB` 是否小于大顶堆的堆顶元素，小则先弹出堆顶元素，将当前差价入堆。
- 这样最后得到的堆就是 $N$ 个具有最小 `priceA - priceB` 的项。
- 最后，取出堆中所有项，**在 `priceB` 的和之上，加上改变行程付出的代价**。

这种做法不用将所有项入堆和出堆，因此降低了时间复杂度。代码实现如下：
```java
class Solution {
    public int twoCitySchedCost(int[][] costs) {
        PriorityQueue<Integer> pq = new PriorityQueue<>((a, b) -> b - a);
        int n = costs.length, halfN = n >> 1;
        int ans = 0;
        for (int i = 0; i < n; ++i) { 
            ans += costs[i][1]; // 所有人都去b城市的费用总和
            pq.offer(costs[i][0] - costs[i][1]);
            if (pq.size() > halfN) pq.poll();
        }
        while (!pq.isEmpty()) ans += pq.poll(); // +diff
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(2N \times \log N + N\log N)$  
- 空间复杂度：$O(N)$ 

上述写法相当简单易懂，但还可稍微优化：
- 我们令 $ans$ 每次加上 $costs[i][0]$ ，这样 $ans$ 就是所有人都去 $A$ 市的费用总和；
- 当堆的大小超过 $N$ 时，弹出的都是不去 $A$ 市的，用 $ans - pq.poll()$ 即为 $ans - costs[i][0] + costs[i][1]$ ，**这样就让他去 $B$ 市了**。
```java
class Solution {
    public int twoCitySchedCost(int[][] costs) {
        PriorityQueue<Integer> pq = new PriorityQueue<>((a, b) -> b - a);
        int n = costs.length, halfN = n >> 1;
        int ans = 0;
        for (int i = 0; i < n; ++i) { 
            ans += costs[i][0]; // 所有人都去A市的费用总和
            pq.offer(costs[i][0] - costs[i][1]);
            if (pq.size() > halfN) ans -= pq.poll(); // 弹出的都是不去A市的
        } 
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(2N \times \log N)$  
- 空间复杂度：$O(N)$ 

---
### 解法3 贪心+快速选择
事实上，只需要优化排序部分，就可达到 $O(N)$ ，严格来说是 $O(2N)$ 。优化思路如下：
- 只需要知道哪一半人去 $A$ 更划算；
- 无需对所有人的差价进行排序，甚至不需要用堆获取「$N$ 个人的最小差价」。
- 实现：分治思想，利用快速排序的 `partition` 算法。
    - 标准快速排序是全排序，划分后左右都需要排序，则 $T(n) = 2T(n/2) +O(n)$ ；
    - 本题只需要排一半，则 $T(n) = T(n/2) + O(n) \to O(2n)$ 。

> 可参考这位大佬在「题目 136. 只出现一次的数字」提出的全新思路：[非位运算，分治思想，时间O(n)，空间O(1)](https://leetcode-cn.com/problems/single-number/solution/fei-wei-yun-suan-fen-zhi-si-xiang-shi-jian-onkong-/)，很有趣的解法。

```java
class Solution {
    public int twoCitySchedCost(int[][] costs) {
        int ans = 0, n = costs.length;
        int[] arr = new int[n];
        for (int i = 0; i < n; ++i) {
            ans += costs[i][0]; // 都去A市的费用
            arr[i] = costs[i][0] - costs[i][1];
        }
        quickSort(arr, 0, n - 1);
        for (int i = n / 2; i < n; ++i) ans -= arr[i]; // 差价大的,说明去B市更好
        return ans;
    }
    private void quickSort(int[] arr, int l, int r) {
        if (l >= r) return;
        int p = partition(arr, l, r), mid = arr.length / 2;
        if (p == mid) return; // 已经是中点
        if (p > mid) quickSort(arr, l, p - 1);
        else quickSort(arr, p + 1, r);
    } 
    private int partition(int[] arr, int l, int r) {
        int c = l;
        for (int i = l; i < r; ++i)
            if (arr[i] < arr[r]) swap(arr, i, c++);
        swap(arr, r, c);
        return c;
    }
    private void swap(int[] arr, int i, int j) {
        if (i == j) return;
        int swap = arr[i];
        arr[i] = arr[j];
        arr[j] = swap;
    }
} 
```
复杂度分析：
- 时间复杂度：$O(N)$
- 空间复杂度：$O(N)$ 
