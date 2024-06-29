> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

请你设计一个队列，支持在前，中，后三个位置的 `push` 和 `pop` 操作。

请你完成 `FrontMiddleBack` 类：
- `FrontMiddleBack()` 初始化队列。
- `void pushFront(int val)` 将 `val` 添加到队列的 **最前面** 。
- `void pushMiddle(int val)` 将 `val` 添加到队列的 **正中间** 。
- `void pushBack(int val)` 将 `val` 添加到队里的 **最后面** 。
- `int popFront()` 将 **最前面** 的元素从队列中删除并返回值，如果删除之前队列为空，那么返回 `-1` 。
- `int popMiddle()` 将 **正中间** 的元素从队列中删除并返回值，如果删除之前队列为空，那么返回 `-1` 。
- `int popBack()` 将 **最后面** 的元素从队列中删除并返回值，如果删除之前队列为空，那么返回 `-1` 。

请注意当有 **两个** 中间位置的时候，选择靠前面的位置进行操作。比方说：
- 将 `6` 添加到 `[1, 2, 3, 4, 5]` 的中间位置，结果数组为 `[1, 2, 6, 3, 4, 5]` 。
- 从 `[1, 2, 3, 4, 5, 6]` 的中间位置弹出元素，返回 `3` ，数组变为 `[1, 2, 4, 5, 6]` 。

**示例 1：**
<pre><strong>输入：</strong>
["FrontMiddleBackQueue", "pushFront", "pushBack", "pushMiddle", "pushMiddle", "popFront", "popMiddle", "popMiddle", "popBack", "popFront"]
[[], [1], [2], [3], [4], [], [], [], [], []]
<strong>输出：</strong>
[null, null, null, null, null, 1, 3, 4, 2, -1]

<strong>解释：</strong>
FrontMiddleBackQueue q = new FrontMiddleBackQueue();
q.pushFront(1);   // [<strong>1</strong>]
q.pushBack(2);    // [1, <strong>2</strong>]
q.pushMiddle(3);  // [1, <strong>3</strong>, 2]
q.pushMiddle(4);  // [1, <strong>4</strong>, 3, 2]
q.popFront();     // 返回 1 -&gt; [4, 3, 2]
q.popMiddle();    // 返回 3 -&gt; [4, 2]
q.popMiddle();    // 返回 4 -&gt; [2]
q.popBack();      // 返回 2 -&gt; []
q.popFront();     // 返回 -1 -&gt; [] （队列为空）
</pre>

**提示：**
- `1 <= val <= 10^9`
- 最多调用 `1000` 次 `pushFront`， `pushMiddle`， `pushBack`， `popFront`， `popMiddle` 和 `popBack` 。

---
### 解法 两个双端队列
设置两个双端队列，并保持两个 `deque` 大小相等，或者右 `deque` 比左 `deque` 多一个元素。使用 `reBalance` 方法平衡左右 `deque` 。
```java
class FrontMiddleBackQueue {
    private Deque<Integer> left, right; // left的大小等于或比right少1个元素
    private void reBalance() {
        if (left.size() > right.size()) 
            right.addFirst(left.pollLast());
        else if (left.size() + 1 < right.size())
            left.addLast(right.pollFirst());
    }
    public FrontMiddleBackQueue() {
        left = new ArrayDeque<>();
        right = new ArrayDeque<>();
    }
    
    public void pushFront(int val) {
        left.addFirst(val);
        reBalance();
    }
    
    public void pushMiddle(int val) {
        if (left.size() == right.size()) right.addFirst(val);
        else left.addLast(val);
    }
    
    public void pushBack(int val) {
        right.addLast(val);
        reBalance();
    }
    
    public int popFront() {
        Integer ans = left.pollFirst();
        if (ans == null) {
            ans = right.pollFirst();
            return ans == null ? -1 : ans;
        } else {
            reBalance();
            return ans;
        }
    }
    
    public int popMiddle() {
        if (left.size() == right.size()) {
            Integer ans = left.pollLast();
            return ans == null ? -1 : ans;
        } else return right.pollFirst();
    }
    
    public int popBack() {
        Integer ans = right.pollLast();
        reBalance();
        return ans == null ? -1 : ans;
    }
}
```
复杂度分析：
- 时间复杂度：都是 $O(1)$ 
- 空间复杂度：$O(n)$

还可以维持一个双向链表，其元素个数 $size$ 和中间结点的指针 $mid$ 。