> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>Design an algorithm to find the smallest K numbers in an array.</p>

<p><strong>Example: </strong></p>

```cpp
Input:  arr = [1,3,5,7,2,4,6,8], k = 4
Output:  [1,2,3,4]
```
<p><strong>Note: </strong></p>
<ul>
	<li><code>0 &lt;= len(arr) &lt;= 100000</code></li>
	<li><code>0 &lt;= k &lt;= min(100000, len(arr))</code></li>
</ul>



题意：设计一个算法，找出数组中最小的 `k` 个数。以任意顺序返回这 `k` 个数均可。


---
### 解法1 快速排序
直接使用排序算法，再构造答案。排序的时间复杂度为 $O(n\log n)$ ，构造答案的复杂度为 $O(k)$ ，整体仍为 $O(n\log n)$ ；空间复杂度为 $O(k)$ ： 
```cpp
//C++ version
class Solution {
public:
    vector<int> smallestK(vector<int>& arr, int k) {
        sort(arr.begin(), arr.end());
        return vector<int>(arr.begin(), arr.begin() + k);
    }
};
//执行用时：32 ms, 在所有 C++ 提交中击败了46.44% 的用户
//内存消耗：17.6 MB, 在所有 C++ 提交中击败了80.57% 的用户
```
```java
//Java version
class Solution {
    public int[] smallestK(int[] arr, int k) {
        Arrays.sort(arr);
        int[] ans = new int[k];
        for (int i = 0; i < k; ++i) ans[i] = arr[i];
        return ans;
    }
}
//执行用时：7 ms, 在所有 Java 提交中击败了62.27% 的用户
//内存消耗：47.8 MB, 在所有 Java 提交中击败了64.31% 的用户
```

---
### 解法2 优先队列（小根堆）
使用小根堆，将所有元素放入堆中，再从堆中取出 $k$ 个元素并顺序构造答案。建堆的时间复杂度为 $O(n\log n)$ ，构造答案的复杂度为 $O(k \log n)$ ，整体仍为 $O(n\log n)$ ，空间复杂度为 $O(n + k)$ ：
```cpp
//C++ version
class Solution {
public:
    vector<int> smallestK(vector<int>& arr, int k) {
        priority_queue<int, vector<int>, greater<int>> pq;
        for (const int &v : arr) pq.push(v);
        vector<int> ans;
        while (k-- > 0) {
            ans.push_back(pq.top());
            pq.pop();
        }
        return ans;
    }
};
//执行用时：36 ms, 在所有 C++ 提交中击败了33.46% 的用户
//内存消耗：19.2 MB, 在所有 C++ 提交中击败了5.01% 的用户
```
```java
//Java version
class Solution {
    public int[] smallestK(int[] arr, int k) {
        PriorityQueue<Integer> pq = new PriorityQueue<>();
        for (int v : arr) pq.add(v);
        int[] ans = new int[k];
        for (int i = 0; i < k; ++i) ans[i] = pq.poll();
        return ans;
    }
}
//执行用时：21 ms, 在所有 Java 提交中击败了29.89% 的用户
//内存消耗：46.2 MB, 在所有 Java 提交中击败了98.35% 的用户
```

---
### 解法3 优先队列（大根堆）
比起简单粗暴的快速排序和小根堆而言，使用大根堆就需要一点技巧性了。小根堆中我们把所有元素都放入堆中，导致复杂度上界为 $O(n\log n)$ ，实际上只需要放入 $k$ 个元素，即使用大小为 $k$ 的大根堆。

具体来说，遍历到 `arr[i]` 时，根据堆中元素个数、`arr[i]` 与堆顶元素的关系进行分类讨论：
- 堆内元素不足 `k` 个时，直接将 `arr[i]` 放入堆中；
- 堆内元素为 `k` 个时，根据 `arr[i]` 与堆顶元素的关系进行分类讨论：
	- 如果 `arr[i]` 大于等于堆顶元素，则 `arr[i]` 不可能属于前 `k` 小的数（已经有了 `k` 个元素在堆中了，它们都小于等于 `arr[i]` ），继续遍历；
	- 否则，`arr[i]` 可能属于前 `k` 小的数，先弹出堆顶元素，再将 `arr[i]` 作为备选答案放入堆中。

遍历完 `arr` 数组后，将堆中元素依次弹出，逆序构造答案。建堆的时间复杂度为 $O(n\log k)$ ，构造答案的时间为 $O(k\log k)$ ，整体复杂度为 $O(n\log k)$ ；空间复杂度为 $O(k)$ ：
```cpp
//C++ version
class Solution {
public:
    vector<int> smallestK(vector<int>& arr, int k) {
        if (k == 0) return {}; //注意k=0时,需要直接返回{}
        priority_queue<int, vector<int>, less<int>> pq;
        for (const int &v : arr) {
            if (pq.size() < k) pq.push(v);
            else if (v < pq.top()) { //不然此处会出错
                pq.pop();
                pq.push(v);
            }
        }
        vector<int> ans(k);
        while (k-- > 0) {
            ans[k] = pq.top();
            pq.pop();
        }
        return ans;
    }
};
//执行用时：24 ms, 在所有 C++ 提交中击败了82.05% 的用户
//内存消耗：18.2 MB, 在所有 C++ 提交中击败了29.69% 的用户
```
```java
//Java version
class Solution {
    public int[] smallestK(int[] arr, int k) {
        if (k == 0) return new int[k];
        PriorityQueue<Integer> pq = new PriorityQueue<>((a, b) -> b - a);
        for (int v : arr) {
            if (pq.size() < k) pq.add(v);
            else if (v < pq.peek()) { pq.poll(); pq.add(v); }
        }
        int[] ans = new int[k];
        for (int i = k - 1; i >= 0; --i) ans[i] = pq.poll();
        return ans;
    }
}
//执行用时：25 ms, 在所有 Java 提交中击败了18.29% 的用户
//内存消耗：46.7 MB, 在所有 Java 提交中击败了74.93% 的用户
```
从结果来看，比起小根堆解法，提高了部分时空效率。

---
### 解法4 分治（快排划分）
如果看题看得仔细，就能发现，题目要求「**任意顺序返回这 `k` 个数**」，因此只需要**确保前 `k` 个数出现在区间 $[1,k)$ 中即可**。利用快速排序中的**划分算法**，可以实现这一点。

快速排序每次都选择一个 `pivot` 基准值，并将小于等于基准值的元素放到左边，将大于基准值的元素放到右边。因此，通过判断基准值的下标 `idx` 与 `k` 的关系，即可确定是否结束递归的快排划分：
-  `idx < k` 时，基准值左侧不足 `k` 个，需要递归处理右边，让基准点右移——这时所有 `<= arr[idx]` 的数都放在 `idx` 左边，`> arr[idx]` 的数都放在 `idx` 右边，
- `idx == k` 时，基准值左边恰好 `k` 个，此时 `<= arr[k]` 的数都在 `k` 左边，`> arr[k]` 的数都在 `k` 右边，刚好符合题目要求，可以输出基准点左侧元素；
- `idx > k` 时，基准值左边超过 `k` 个，需要递归处理左边，让基准点左移——这时所有 `<= arr[idx]` 的数都放在 `idx` 左边，`> arr[idx]` 的数都放在 `idx` 右边，但是不确定前 `k` 个元素是否为最小的 `k` 个元素，可能有些更大的元素跑到了前 `k` 个里面也说不定。

具体代码如下所示，算法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(k)$ ：
```cpp
//C++ versioin
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);
class Solution {
private:
    int targetK;
    uniform_int_distribution<int> ud;
    void quickSort(vector<int>& arr, int l, int r) {
        if (l >= r) return;
        ud.param(uniform_int_distribution<>::param_type {l, r});
        int idx = ud(generator);
        swap(arr[l], arr[idx]);
        int x = arr[l], i = l, j = r;
        while (i < j) {
            while (i < j && arr[j] >= x) --j;
            while (i < j && arr[i] <= x) ++i;
            if (i < j) swap(arr[i], arr[j]);
        }
        swap(arr[l], arr[i]);
        if (i > targetK) quickSort(arr, l, i - 1);
        else if (i < targetK) quickSort(arr, i + 1, r);
    }
public:
    vector<int> smallestK(vector<int>& arr, int k) {
        if (k == 0) return {}; //注意k=0时,需要直接返回{}
        vector<int> ans;
        targetK = k;
        quickSort(arr, 0, arr.size() - 1);
        for (int i = 0; i < k; ++i) ans.push_back(arr[i]);
        return ans;
    }
};
//执行用时：20 ms, 在所有 C++ 提交中击败了93.83% 的用户
//内存消耗：18 MB, 在所有 C++ 提交中击败了40.90% 的用户
```
```java
//Java version
class Solution {
    private int targetK;
    private void swap(int[] arr, int l, int r) {
        int temp = arr[l]; arr[l] = arr[r]; arr[r] = temp;
    }
    private void quickSort(int[] arr, int l, int r) {
        if (l >= r) return;
        int idx = new Random().nextInt(r - l + 1) + l;
        swap(arr, idx, l); //基准值在arr[l]处
        int x = arr[l], i = l, j = r;
        while (i < j) {
            while (i < j && arr[j] >= x) --j; //改用了>=,效率高一些
            while (i < j && arr[i] <= x) ++i;
            if (i < j) swap(arr, i, j);
        }
        swap(arr, l, i);
        if (i > targetK) quickSort(arr, l, i - 1);
        else if (i < targetK) quickSort(arr, i + 1, r);
    }
    public int[] smallestK(int[] arr, int k) {
        if (k == 0) return new int[k];
        int n = arr.length;
        int[] ans = new int[k];
        targetK = k; //记录k值
        quickSort(arr, 0, n - 1);
        for (int i = 0; i < k; ++i) ans[i] = arr[i];
        return ans;
    }
}
//执行用时：3 ms, 在所有 Java 提交中击败了87.12% 的用户
//内存消耗：47.9 MB, 在所有 Java 提交中击败了62.51% 的用户
```
