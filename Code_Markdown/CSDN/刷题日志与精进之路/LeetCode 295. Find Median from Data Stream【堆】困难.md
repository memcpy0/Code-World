> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>The <strong>median</strong> is the middle value in an ordered integer list. If the size of the list is even, there is no middle value and the median is the mean of the two middle values.</p>

<ul>
	<li>For example, for <code>arr = [2,3,4]</code>, the median is <code>3</code>.</li>
	<li>For example, for <code>arr = [2,3]</code>, the median is <code>(2 + 3) / 2 = 2.5</code>.</li>
</ul>

<p>Implement the MedianFinder class:</p>

<ul>
	<li><code>MedianFinder()</code> initializes the <code>MedianFinder</code> object.</li>
	<li><code>void addNum(int num)</code> adds the integer <code>num</code> from the data stream to the data structure.</li>
	<li><code>double findMedian()</code> returns the median of all elements so far. Answers within <code>10<sup>-5</sup></code> of the actual answer will be accepted.</li>
</ul>
 
<p><strong>Example 1:</strong></p>

```cpp
Input
["MedianFinder", "addNum", "addNum", "findMedian", "addNum", "findMedian"]
[[], [1], [2], [], [3], []]
Output
[null, null, null, 1.5, null, 2.0]

Explanation
MedianFinder medianFinder = new MedianFinder();
medianFinder.addNum(1);    // arr = [1]
medianFinder.addNum(2);    // arr = [1, 2]
medianFinder.findMedian(); // return 1.5 (i.e., (1 + 2) / 2)
medianFinder.addNum(3);    // arr[1, 2, 3]
medianFinder.findMedian(); // return 2.0
```
 
<p><strong>Constraints:</strong></p> 
<ul>
	<li><code>-10<sup>5</sup> &lt;= num &lt;= 10<sup>5</sup></code></li>
	<li>There will be at least one element in the data structure before calling <code>findMedian</code>.</li>
	<li>At most <code>5 * 10<sup>4</sup></code> calls will be made to <code>addNum</code> and <code>findMedian</code>.</li>
</ul>

 
<p><strong>Follow up:</strong></p>

<ul>
	<li>If all integer numbers from the stream are in the range <code>[0, 100]</code>, how would you optimize your solution?</li>
	<li>If <code>99%</code> of all integer numbers from the stream are in the range <code>[0, 100]</code>, how would you optimize your solution?</li>
</ul>


题意：中位数是有序列表中间的数。如果列表长度是偶数，中位数则是中间两个数的平均值。设计一个支持以下两种操作的数据结构：
- `void addNum(int num)` - 从数据流中添加一个整数到数据结构中。
- `double findMedian()` - 返回目前所有元素的中位数。
 
进阶:
- 如果数据流中所有整数都在 0 到 100 范围内，你将如何优化你的算法？
 - 如果数据流中 99% 的整数都在 0 到 100 范围内，你将如何优化你的算法？ 


---
### 解法 堆
这是一道相当经典的数据结构运用题。调用次数最多为 <code>5 * 10<sup>4</sup></code> ，因此要使用 $O(n\log n)$ 级别的算法。具体来说，就是用两个优先队列（堆）维护整个数据流，维护数据流左半边数据的堆为 `left` ，维护数据流右半边数据的堆为 `right` 。于是显然，令 `left` 为大根堆，`right` 为小根堆，**两个堆的堆顶元素就是当前中位数的候选**，可以用 $O(1)$ 复杂度得到当前中位数：
- 数据流长度为奇数时，**令 `left` 比 `right` 多一个元素**，此时的动态中位数为 **`left` 的堆顶元素**；
- 数据流长度为偶数时，**`left` 和 `right` 大小相同**，此时的动态中位数为**两堆的堆顶元素平均值**

为了保证两堆间的有序性（从而满足元素序列的有序性）和大小关系（便于获取中位数），在 `addNum` 时我们应该分奇偶情况处理：
-  **插入前左右两堆大小相同**，说明插入前数据流长度为偶数，插入后长度变为奇数，此时期望插入后**左堆元素比右堆多一个、双堆仍然维持有序**，因此进一步分类讨论：
	 - 如果都为空堆（可以只检查 `right` 是否为空），说明插入的是首个元素，直接加入 `left` ；
	 - 如果 `right` 不为空，且 `num <= right.top()` ，表明 `num` 的插入位置不会在后半部分的 `right` 中，也直接加入 `left` ；
	 - 如果 `right` 不为空，且 `num > right.top()` ，表明 `num` 的插入位置在后半部分的 `right` 中，此时先将 `right` 堆顶元素弹出并放入 `left` 中，再把 `num` 放到 `right` 中。
-  **插入前左右两堆大小不同**，即左堆元素多一个，说明插入前数据流长度为奇数，插入后长度变为偶数，此时期望插入后**两堆元素数量相等、双堆仍然维持有序**，因此进一步分类讨论：
	 - 如果 `num >= left.top()` ，表明 `num` 的插入位置不会在前半部分的 `left` 中，直接加入 `right` ；
	 - 如果 `num < left.top()` ，表明 `num` 的插入位置在前半部分的 `left` 中，此时先将 `left` 堆顶元素弹出并放入 `right` 中，再把 `num` 放到 `left` 中。 

以下代码中，`addNum` 方法的时间复杂度为 $O(\log n)$ ，`findMedian` 方法的时间复杂度为 $O(1)$ ，总体空间复杂度为 $O(n)$ ：
```cpp
//C++ version
class MedianFinder {
private:
    priority_queue<int, vector<int>, less<int>> left; //大根堆
    priority_queue<int, vector<int>, greater<int>> right; //小根堆
public:
    /** initialize your data structure here. */
    MedianFinder() {}
    /* left维护序列的左半部分,right维护右半部分
    ** 0 <= left.size() - right.size() <= 1
    ** 左右两堆大小相等时,加入新的数,数据流长度为奇数,取中位数就取left.top()
    **   - 都为空(右堆空)时,直接加入left堆
    **   - 都不为空时:
    **       - 如果num<=right.top(),应该放入left堆
    **       - 如果num>right.top(),应该取出right.top()放入left堆,将num放入right堆
    ** 左右两堆大小差1时(左>右),加入新的数,数据流长度为偶数,
    ** 取中位数就取(left.top()+right.top())/2.0
    **   - 如果num>=left.top(),应该放入right堆
    **   - 如果num<left.top(),应该取出left.top()放入right堆,将num放入left堆
    */
    void addNum(int num) {
        if (left.size() == right.size()) {
            if (right.empty() || num <= right.top()) left.push(num);
            else {
                left.push(right.top()); right.pop();
                right.push(num);
            }
        } else {
            if (num >= left.top()) right.push(num);
            else {
                right.push(left.top()); left.pop();
                left.push(num);
            }
        }
    }
    double findMedian() {
        return left.size() == right.size() ? 
                ((left.top() + right.top()) / 2.0) : left.top();
    }
};
//执行用时：284 ms, 在所有 C++ 提交中击败了58.06% 的用户
//内存消耗：114.3 MB, 在所有 C++ 提交中击败了41.31% 的用户
```
```java
//Jave version
class MedianFinder {
    private PriorityQueue<Integer> left = new PriorityQueue<>((a, b) -> b - a);
    private PriorityQueue<Integer> right = new PriorityQueue<>((a, b) -> a - b);
    /** initialize your data structure here. */
    public MedianFinder() {}
    
    public void addNum(int num) {
        if (left.size() == right.size()) {
            if (right.isEmpty() || num <= right.peek()) {
                left.add(num);
            } else {
                left.add(right.poll());
                right.add(num);
            } 
        } else {
            if (num >= left.peek()) {
                right.add(num);
            } else {
                right.add(left.poll());
                left.add(num);
            }
        }
    }
    
    public double findMedian() {
        if (left.size() == right.size()) return (left.peek() + right.peek()) / 2.0;
        else return left.peek();
    }
}
//执行用时：91 ms, 在所有 Java 提交中击败了63.61% 的用户
//内存消耗：58.5 MB, 在所有 Java 提交中击败了74.80% 的用户
```
---
### 进阶
==如果数据流中所有整数都在 0 到 100 范围内，你将如何优化你的算法？==

具体来说就是，建立长度为 `101` 的桶序列，分别统计每个整数的出现次数，同时记录数据流的总长度。每次查找中位数时，先计算它是第几位，然后从前往后扫描所有的桶得到答案。

这一做法，**在时间上没有太大优化**，因为双堆解法中 `addNum` 方法为 $O(\log n)$ 、`findMedian` 方法为 $O(1)$ ，桶计数解法中 `addNum` 方法为 $O(1)$ 、`findMedian` 方法为 $O(101)$ 。极限调用 $5 \times 10^4$ 次数下，双堆解法的 `addNum` 计算量仍然低于桶计数 `findMedian` 计算量。如果再考虑到常数的影响，两者的时间差距也不会很大，**更多的是空间优化**，从 $O(n)$ 降低到 $O(101)$ 。

==如果数据流中 99% 的整数都在 0 到 100 范围内，你将如何优化你的算法？==

在前一问解法的基础上，对 $1\%$ 的数据采用哨兵机制解决，即在最小桶和最大桶两侧各自维护一个有序序列，代表负无穷和正无穷。实际代码如下：
```java
//Java version
class MedianFinder {
    int[] arr = new int[101];
    int a, b, c; //a,c为哨兵桶中元素的数量,b为[0,100]范围中元素的数量
    TreeMap<Integer, Integer> h = new TreeMap<>(), t = new TreeMap<>();
    public void addNum(int num) {
        if (num >= 0 && num <= 100) { //进阶第1问和第2问的99%数据
            ++arr[num];
            ++b; //计数
        } else if (num < 0) { //加入到最左侧的哨兵桶中
            h.put(num, h.getOrDefault(num, 0) + 1);
            ++a; //计数
        } else if (num > 100) { //加入到最右侧的哨兵桶中
            t.put(num, t.getOrDefault(num, 0) + 1);
            ++c; //计数
        }
    }
    public double findMedian() {
        int size = a + b + c; //数据流的总长度
        if (size % 2 == 0) return (find(size / 2) + find(size / 2 + 1)) / 2.0;
        return find(size / 2 + 1) * 1.0;
    }
    private int find(int n) { //找到第n个位置的元素
        if (n <= a) {
            for (int num : h.keySet()) {
                n -= h.get(num);
                if (n <= 0) return num;
            }
        } else if (n <= a + b) { 
            n -= a;
            for (int i = 0; i <= 100; ++i) { //进阶第一问的查找方法
                n -= arr[i];
                if (n <= 0) return i;
            }
        } else {
            n -= a + b;
            for (int num : t.keySet()) {
                n -= t.get(num);
                if (n <= 0) return num;
            }
        }
        return -1; //never visited
    }
}
```
由于真实数据不是绝大多数分布在 `[0, 100]` 范围中，所以会TLE，只能通过 `17 / 21` 个测试用例。
 
