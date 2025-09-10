> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

在一个仓库里，有一排条形码，其中第 `i` 个条形码为 `barcodes[i]`。

请你重新排列这些条形码，使其中任意两个相邻的条形码不能相等。 你可以返回任何满足该要求的答案，此题保证存在答案。

**示例 1：**
```java
输入：barcodes = [1,1,1,2,2,2]
输出：[2,1,2,1,2,1]
```
**示例 2：**
```java
输入：barcodes = [1,1,1,1,2,2,3,3]
输出：[1,3,1,3,2,1,2,1]
```
**提示：**
-   `1 <= barcodes.length <= 10000`
-   `1 <= barcodes[i] <= 10000`

---
本题与[LeetCode 767. Reorganize String【贪心,堆,计数】中等](https://memcpy0.blog.csdn.net/article/details/110389170)类似，可参考对应题解。
### 解法1 基于最大堆的贪心
维护最大堆存储条形码，**堆顶元素为出现次数最多的条形码**。首先统计每个条形码的出现次数，然后「将出现过的条形码及其次数」加入最大堆。

当最大堆的元素个数大于 $1$ 时，每次从最大堆取出两个条形码，放入排列中，然后将两个条形码的出现次数分别减 $1$ ，并将「剩余出现次数大于 $0$ 的条形码」重新加入最大堆。==由于最大堆中的元素都是不同的，因此取出的两个条形码一定也是不同的，将两个不同的条形码放入排列尾部，可以确保相邻的条形码都不相同==。
> 这里其实有问题，每次从队列中取出 $2$ 个条形码，题目中**没有保证总是按照特定顺序取**。 比如数组有 $3$ 个 $1$ 、$3$ 个 $2$ 。万一第一次取到了 $1,2$ ，但第二次取到了 $2,1$ 呢？因为他们只是按照频次排序，没有按条形码值排序。
> <b></b> 
> 解决方案有两个：
> 1. 将取出的两个条形码分别和已经放入结果的最后一个条形码比较，先放不同的那个条形码。
> 2. ==重载堆的排序，出现次数相同时，按值的大小排列==。

如果最大堆变成空，则已经完成条形码的重新排列。如果最大堆剩下 $1$ 个元素，则取出最后一个条形码，放入排列中。

对于长度为 $n$ 的条形码数组，共有 $\Big\lfloor \dfrac{n}{2} \Big\rfloor$ 次每次从最大堆取出两个条形码的操作，当 $n$ 是奇数时，还有一次从最大堆取出一个条形码的操作，因此「重新排列后的条形码数组」的长度一定是 $n$ 。

==当 $n$ 是奇数时，是否可能出现重构数组的最后两个条形码相同的情况==？如果最后一个条形码在整个数组中至少出现了 $2$ 次，则在最后一次从最大堆取出两个条形码时（此时该条形码次数为 $2$ ，另一个条形码次数为 $1$ ），**该条形码会先被选出**，因此不会成为「重构的条形码数组」的倒数第二个条形码，也不可能出现最后两个条形码相同的情况。

因此，在一定存在答案的情况下，**基于最大堆的贪心可以确保得到正确答案**。
```java
class Solution {
    public int[] rearrangeBarcodes(int[] barcodes) {
        if (barcodes.length < 2) return barcodes;
        var counts = new HashMap<Integer, Integer>();
        for (int barcode : barcodes) counts.put(barcode, counts.getOrDefault(barcode, 0) + 1);
        var q = new PriorityQueue<Integer>(new Comparator<Integer>() {
            public int compare(Integer a, Integer b) {
                int c1 = counts.get(a), c2 = counts.get(b);
                // if (c1 == c2) return a - b; // 出现次数相同时，按值排序 // 这里可以不写
                return c2 - c1; // 大顶堆
            }
        });
        // for (int barcode : barcodes) q.offer(barcode); // 这样写q里面会有重复的值
        for (int barcode : counts.keySet()) q.offer(barcode);
        int[] ans = new int[barcodes.length];
        int idx = 0;
        while (q.size() > 1) {
            int b1 = q.poll();
            int b2 = q.poll();
            ans[idx++] = b1;
            ans[idx++] = b2;
            int c1 = counts.get(b1), c2 = counts.get(b2);
            counts.put(b1, c1 - 1);
            counts.put(b2, c2  -1);
            if (c1 > 1) q.offer(b1);
            if (c2 > 1) q.offer(b2);
        }
        if (q.size() > 0) ans[idx++] = q.poll();
        return ans;
    }
}
```
这样写，效率有点慢，每次从优先队列中取值时，都要操作哈希表。不如**一开始就在优先队列中存储每个条形码的出现次数**。但这也只是稍微提速一点。算法还需继续优化。
```java
class Solution {
    public int[] rearrangeBarcodes(int[] barcodes) {
        if (barcodes.length < 2) return barcodes;
        var counts = new HashMap<Integer, Integer>();
        for (int barcode : barcodes) counts.put(barcode, counts.getOrDefault(barcode, 0) + 1);
        var q = new PriorityQueue<int[]>((a, b) -> b[0] - a[0]);
        for (Map.Entry<Integer, Integer> entry : counts.entrySet()) 
            q.offer(new int[]{entry.getValue(), entry.getKey()}); // 次数,条形码
        int[] ans = new int[barcodes.length];
        int idx = 0;
        while (q.size() > 1) {
            int[] b1 = q.poll();
            int[] b2 = q.poll();
            ans[idx++] = b1[1];
            ans[idx++] = b2[1]; 
            --b1[0];
            --b2[0];
            if (b1[0] > 0) q.offer(b1);
            if (b2[0] > 0) q.offer(b2);
        }
        if (q.size() > 0) ans[idx++] = q.poll()[1];
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n\log n)$ ，$n$ 是 $barcodes$ 的长度。
- 空间复杂度：$O(n)$
 
---
### 解法2 基于计数的贪心
本题的思路是**交替放置最常见的条形码**。先统计每个条形码出现的次数，然后根据每个条形码的出现次数重构数组。

==当 $n$ 是奇数且出现最多的条形码的出现次数是 $\dfrac{n+1}{2}$ 时，出现次数最多的条形码必须全部放置在偶数下标，否则一定会出现相邻的条形码相同的情况==。其余情况下，每个条形码放置在偶数下标**或者**奇数下标都是**可行的**。

维护偶数下标 $evenIndex$ 和奇数下标 $oddIndex$ ，初始值分别为 $0$ 和 $1$ 。遍历每个条形码，**根据每个条形码的出现次数判断条形码应该放置在偶数下标还是奇数下标**。

**首先考虑是否可以放置在奇数下标**。根据上述分析可知，==只要条形码的出现次数不超过数组长度的一半（即出现次数小于或等于 $\Big\lfloor \dfrac{n}{2} \Big\rfloor$​ ，就可以放置在奇数下标==；**只有当条形码的出现次数超过数组长度的一半时，才必须放置在偶数下标**。而条形码的出现次数超过数组长度的一半，只可能发生在 $n$ 是奇数的情况下，且**最多只有一个条形码的出现次数会超过数组长度的一半**。

因此通过如下操作在重构数组中放置条形码：
- 如果条形码的出现次数 $> 0$ 且 $\le \lfloor \dfrac{n}{2} \rfloor$ ，且 $oddIndex$ 没有超过数组下标范围，则将条形码放置在 $oddIndex$ ，然后将 $oddIndex$ 的值加 $2$ ；
- **如果条形码的出现次数 $> \lfloor \dfrac{n}{2} \rfloor$ ，或 $oddIndex$ 超过数组下标范围**，则将条形码放置在 $evenIndex$ ，然后将 $evenIndex$ 的值加 $2$ 
- 如果一个条形码出现了多次，则重复上述操作，直到该条形码全部放置完毕。

那么上述做法**是否可以确保相邻的条形码都不相同**？考虑以下三种情况。
- 如果 $n$ 是奇数且存在一个条形码的出现次数为 $\dfrac{n+1}{2}$ ，则该条形码全部被放置在偶数下标，**其余的 $\dfrac{n-1}{2}$​个条形码都被放置在奇数下标**，因此**相邻的条形码一定不相同**。
- 不存在一个条形码的出现次数为 $\dfrac{n+1}{2}$ ，则按理来说，同一个条形码可全部被放置在奇数下标、或全部被放置在偶数下标，则该条形码不可能在相邻的下标出现。
- 当然，如果同个条形码先被放置在奇数下标、直到奇数下标超过数组下标范围，然后被放置在偶数下标，**由于该条形码的出现次数不会超过 $\lfloor \dfrac{n}{2} \rfloor$ ，因此该条形码的最小奇数下标与最大偶数下标之差不小于 $3$ ，不可能在相邻的下标出现**。证明如下：
	- 当 $n$ 是偶数时，如果该条形码的出现次数为 $\dfrac{n}{2}$ ，包括 $p$ 个奇数下标和 $q$ 个偶数下标，满足 $p + q = \dfrac{n}{2}$ ，最小奇数下标是 $n - 2p + 1$ ，最大偶数下标是 $2(q - 1)$ ，最小奇数下标与最大偶数下标的差是
	 $$\begin{align} 
    & (n−2p+1)−2(q−1)\\
     &= n−2p+1−2q+2\\
     &= n−2(p+q)+3\\
     &= n−2×\dfrac{n}{2} + 3\\
     &= n - n + 3 = 3\end{align}$$ 
	- 当 $n$ 是奇数时，如果该条形码的出现次数为 $\dfrac{n-1}{2}$ ，包括 $p$ 个奇数下标和 $q$ 个偶数下标，满足 $p + q = \dfrac{n-1}{2}$ ，最小奇数下标是 $n - 2p$ ，最大偶数下标是 $2(q - 1)$ ，最小奇数下标与最大偶数下标的差是
	 $$\begin{align} 
    & (n−2p)−2(q−1)\\
     &= n−2p−2q+2\\
     &= n−2(p+q)+2\\
     &= n−2×\dfrac{n-1}{2} + 2\\
     &= n - (n-1) + 2 = 3\end{align}$$ 
因此，在一定存在答案的情况下，基于计数的贪心可以确保相邻的条形码都不相同，得到正确答案。
```java
class Solution {
    public int[] rearrangeBarcodes(int[] barcodes) {
        int length = barcodes.length;
        if (length < 2) return barcodes;
        var counts = new HashMap<Integer, Integer>();
        for (int barcode : barcodes) counts.put(barcode, counts.getOrDefault(barcode, 0) + 1);
        int[] ans = new int[length];
        int evenIndex = 0, oddIndex = 1;
        int halfLength = length / 2;
        for (Map.Entry<Integer, Integer> entry : counts.entrySet()) {
            int x = entry.getKey(), c = entry.getValue();
            while (c > 0 && c <= halfLength && oddIndex < length) {
                ans[oddIndex] = x;
                --c;
                oddIndex += 2;
            }
            while (c > 0) {
                ans[evenIndex] = x;
                --c;
                evenIndex += 2;
            }
        } 
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n)$ ，$n$ 是 $barcodes$ 的长度。
- 空间复杂度：$O(n)$
