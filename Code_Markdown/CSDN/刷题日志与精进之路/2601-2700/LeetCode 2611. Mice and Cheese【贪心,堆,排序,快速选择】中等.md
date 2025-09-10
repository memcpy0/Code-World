> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

有两只老鼠和 `n` 块不同类型的奶酪，每块奶酪都只能被其中一只老鼠吃掉。

下标为 `i` 处的奶酪被吃掉的得分为：
-   如果第一只老鼠吃掉，则得分为 `reward1[i]` 。
-   如果第二只老鼠吃掉，则得分为 `reward2[i]` 。

给你一个正整数数组 `reward1` ，一个正整数数组 `reward2` ，和一个非负整数 `k` 。

返回第一只老鼠恰好吃掉 `k` 块奶酪的情况下，**最大** 得分为多少。

**示例 1：**
```java
输入：reward1 = [1,1,3,4], reward2 = [4,4,1,1], k = 2
输出：15
解释：这个例子中，第一只老鼠吃掉第 2 和 3 块奶酪（下标从 0 开始），第二只老鼠吃掉第 0 和 1 块奶酪。
总得分为 4 + 4 + 3 + 4 = 15 。
15 是最高得分。
```
**示例 2：**
```java
输入：reward1 = [1,1], reward2 = [1,1], k = 2
输出：2
解释：这个例子中，第一只老鼠吃掉第 0 和 1 块奶酪（下标从 0 开始），第二只老鼠不吃任何奶酪。
总得分为 1 + 1 = 2 。
2 是最高得分。
```
**提示：**
-   `1 <= n == reward1.length == reward2.length <= 10^5`
-   `1 <= reward1[i], reward2[i] <= 1000`
-   `0 <= k <= n`

--- 
### 解法1 贪心+排序
有 $n$ 块不同类型的奶酪，分别位于下标 $0$ 到 $n−1$ 。下标 $i$ 处的奶酪被第一只老鼠吃掉的得分为 $\textit{reward}_1[i]$ ，被第二只老鼠吃掉的得分为 $\textit{reward}_2[i]$ 。

**如果 $n$ 块奶酪都被第二只老鼠吃掉，则得分为数组 $\textit{reward}_2$ 的元素之和，记为 $\textit{sum}$** 。如果下标 $i$ 处的奶酪被第一只老鼠吃掉，则得分的变化量是 $\textit{reward}_1[i] - \textit{reward}_2[i]$ 。

创建长度为 $n$ 的数组 $\textit{diffs}$ ，其中 $\textit{diffs}[i] = \textit{reward}_1[i] - \textit{reward}_2[i]$ ​。题目要求计算第一只老鼠恰好吃掉 $k$ 块奶酪的情况下的最大得分，假设第一只老鼠吃掉的 $k$ 块奶酪的下标分别是 $i_1$ 到 $i_k$ ，则总得分为：
$$\textit{sum} + \sum_{j = 1}^k \textit{diffs}[i_j]$$
其中 $\textit{sum}$ 为确定的值。**根据贪心思想，为了使总得分最大化，应使下标 $i_1$ 到 $i_k$ ​对应的 $\textit{diffs}$ 的值最大，即应该选择 $\textit{diffs}$ 中的 $k$ 个最大值**。

==**贪心思想的正确性**==说明如下：假设下标 $i_1$ 到 $i_k$ 对应的 $\textit{diffs}$ 的值不是最大的 $k$ 个值，则一定存在下标 $i_j$ 和下标 $p$ 满足 $\textit{diffs}[p] \ge \textit{diffs}[i_j]$ 且 $p$ 不在 $i_1$ 到 $i_k$ 的 $k$ 个下标中，将 $\textit{diffs}[i_j]$  替换成 $\textit{diffs}[p]$ 之后的总得分不变或增加。因此，**使用贪心思想可以使总得分最大**。

具体做法是，将数组 $\textit{diffs}$ 排序，然后计算 $\textit{sum}$ 与数组 $\textit{diffs}$ 的 $k$ 个最大值之和，即为第一只老鼠恰好吃掉 $k$ 块奶酪的情况下的最大得分。
```java
class Solution {
    public int miceAndCheese(int[] reward1, int[] reward2, int k) {
        int ans = 0, n = reward1.length;
        int[] diffs = new int[n];
        for (int i = 0; i < n; ++i) {
            ans += reward2[i];
            diffs[i] = reward1[i] - reward2[i];
        }
        Arrays.sort(diffs);
        for (int i = 1; i <= k; ++i) ans += diffs[n - i];
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n\log n)$ ，其中 $n$ 是数组 $reward_1$ 和 $reward_2$ 的长度。
- 空间复杂度：$O(n)$

---
### 解法2 贪心+堆
对上述解法使用堆进行优化，我们可以**使用大顶堆**，将分差 $reward_1[i] - reward_2[i]$ 大的排在堆前面，分差最大的在堆顶，然后从堆中取出 $k$ 个当前分差最大的项，**在 $reward_2$ 元素之和 $sum$ 上，加上改变「吃奶酪的老鼠」的代价**。这种做法要将所有项入堆、并出堆 $k$ 个最大的项，因此时间复杂度为 $O(n \log n + k\log n)$ 。

我们还可**使用小顶堆**，保持小顶堆的大小为 $k$ ：
- **当小顶堆大小小于 $k$ 时直接入堆**；
- 否则**看当前的分差 $reward_1[i] - reward_2[i]$ 是否大于小顶堆的堆顶元素，大则先弹出堆顶元素，将当前分差值入堆**。
    ```java
    if (pq.size() < k) pq.offer(diff);
    else if (!pq.isEmpty() && pq.peek() < diff) {
        pq.poll();
        pq.offer(diff, i);
    }
    ```
- **更简洁的写法是**，==直接入堆，当小顶堆大小超过 $k$ 时，弹出堆顶元素==。
- 这样最后得到的堆就是 $k$ 个具有最大分差的项。
- 最后，取出堆中所有项，这些项为 $reward_1, reward_2$ 的 $k$ 个最大差值。计算 $\textit{sum}$ 与堆中的 $k$ 个元素之和，即为第一只老鼠恰好吃掉 $k$ 块奶酪的情况下的最大得分。

这种做法不用将所有项入堆和出堆，因此降低了时间复杂度。代码实现如下：
```java
class Solution {
    public int miceAndCheese(int[] reward1, int[] reward2, int k) {
        PriorityQueue<Integer> pq = new PriorityQueue<>(); // 默认小顶堆
        int n = reward1.length;
        int ans = 0;
        for (int i = 0; i < n; ++i) { 
            ans += reward2[i]; // 老鼠2吃掉所有奶酪的总分
            pq.offer(reward1[i] - reward2[i]);
            if (pq.size() > k) pq.poll();
        }
        while (!pq.isEmpty()) ans += pq.poll(); // +diff=+reward1[i]-reward2[i]
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n \times \log k + k\log k)$  
- 空间复杂度：$O(k)$ 

上述写法相当简单易懂，但**还可稍微优化**：
- 我们令 $ans$ 每次加上 $reward_1[i]$ ，这样 $ans$ 就是老鼠1吃掉所有奶酪的总分；
- 当堆的大小超过 $k$ 时，抛出的说明不是老鼠1吃的，用 $ans - pq.poll()$ 即为 $ans - reward_1[i] + reward_2[i]$ ，**这样就让老鼠2吃掉那块奶酪了**。
```java
class Solution {
    public int miceAndCheese(int[] reward1, int[] reward2, int k) {
        PriorityQueue<Integer> pq = new PriorityQueue<>(); // 默认小顶堆
        int n = reward1.length;
        int ans = 0;
        for (int i = 0; i < n; ++i) { 
            ans += reward1[i]; // 老鼠1吃掉所有奶酪的总分
            pq.offer(reward1[i] - reward2[i]);
            if (pq.size() > k) ans -= pq.poll(); // 从堆中出来,说明不是老鼠1吃的
        } 
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n \times \log k)$  
- 空间复杂度：$O(k)$ 

---
### 解法3 贪心+快速选择
事实上，只需要优化排序部分，就可达到 $O(n)$ 。优化思路如下：
- 只需要知道哪 $k$ 个奶酪由老鼠1吃掉更划算；
- 无需对所有的分差进行排序，甚至不需要用堆获取「$k$ 个奶酪的最大分差」。
- 实现：分治思想，利用快速排序的 `partition` 算法。

```java
class Solution {
    private int pos;
    public int miceAndCheese(int[] reward1, int[] reward2, int k) {
        int n = reward1.length, ans = 0;
        int[] arr = new int[n];
        pos = n - k; // 前n-k个分差小的奶酪
        for (int i = 0; i < n; ++i) { 
            ans += reward1[i]; // 老鼠1吃掉所有奶酪的总分
            arr[i] = reward1[i] - reward2[i];
        } 
        quickSort(arr, 0, n - 1);
        for (int i = 0; i < pos; ++i) ans -= arr[i]; // 前n-k个应该由老鼠2吃掉
        return ans;
    }
    private void quickSort(int[] arr, int l, int r) {
        if (l >= r) return;
        int p = partition(arr, l, r);
        if (p == pos || p == pos - 1) return;
        else if (p > pos) quickSort(arr, l, p - 1);
        else quickSort(arr, p + 1, r);
    }
    private int partition(int[] arr, int l, int r) {
        int c = l;
        for (int i = l; i < r; ++i) 
            if (arr[i] < arr[r])
                swap(arr, i, c++);
        swap(arr, r, c);
        return c;
    }
    private void swap(int[] arr, int i, int j) {
        int t = arr[i];
        arr[i] = arr[j];
        arr[j] = t;
    }
}
```
还可**利用C++的 `nth_elemnt` 函数进行快速选择**（本题最佳推荐！）：
```cpp
class Solution {
public:
    int miceAndCheese(vector<int>& reward1, vector<int>& reward2, int k) {
        int n = reward1.size(); 
        for (int i = 0; i < n; ++i) reward1[i] -= reward2[i]; // 原地修改
        nth_element(reward1.begin(), reward1.end() - k, reward1.end()); // 快速选择
        return accumulate(reward2.begin(), reward2.end(), 0) + // 先全部给第2只老鼠
            accumulate(reward1.end() - k, reward1.end(), 0); // 再加上增量 
    }
};
```