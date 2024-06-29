> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个链表的头节点 `head`，请你编写代码，反复删去链表中由 **总和** 值为 `0` 的连续节点组成的序列，直到不存在这样的序列为止。

删除完毕后，请你返回最终结果链表的头节点。你可以返回任何满足题目要求的答案。（注意，下面示例中的所有序列，都是对 `ListNode` 对象序列化的表示。）

**示例 1：**
```java
输入：head = [1,2,-3,3,1]
输出：[3,1]
提示：答案 [1,2,1] 也是正确的。
```
**示例 2：**
```java
输入：head = [1,2,3,-3,4]
输出：[1,2,4]
```
**示例 3：**
```java
输入：head = [1,2,3,-3,-2]
输出：[1]
```
**提示：**
- 给你的链表中可能有 `1` 到 `1000` 个节点。
- 对于链表中的每个节点，节点的值：`-1000 <= node.val <= 1000`.

---
### 解法1 链表+递归
以示例1为例：
```java
输入：head = [1,2,-3,3,1]
输出：[3,1]
```
**如果我们在「递去」的过程中**，从前往后扫描，从当前节点开始，以一个for循环累计当前节点及其后的节点值，直到：
- 存在值之和为0，则**返回**对「该结束值对应的节点的 $next$ 」递归调用 `removeZeroSumSublists` 方法得到的结果。
- 否则，说明以当前节点的值出发，不能获得一个总和为0的连续节点组成的序列。
- 这样处理下来，例1的输出是 `[3,1]` 。

**如果我们在「归来」的过程中**，先递归到链表尾，然后从后往前扫描，从当前节点开始，以一个for循环累计当前节点及其后的节点值，直到：
- 存在值之和为0，则**返回**对「该结束值对应的节点的 $next$ 」递归调用 `removeZeroSumSublists` 方法得到的结果。
- 否则，说明以当前节点的值出发，不能获得一个总和为0的连续节点组成的序列。
- 这样处理下来，例1的输出是 `[1,2,1]` 。

这两种处理的结果都可行，且方法的效率都一致。递去的代码如下：
```java
class Solution {
    public ListNode removeZeroSumSublists(ListNode head) { 
        if (head == null) return null;
        int sum = 0;
        for (ListNode p = head; p != null; p = p.next) {
            sum += p.val;
            if (sum == 0) return removeZeroSumSublists(p.next);
        }
        head.next = removeZeroSumSublists(head.next);
        return head; 
    }
}
```
复杂度分析：
- 时间复杂度：$O(n^2)$ 。最坏情况下，链表中不存在总和为0的连续节点组成的序列，就要从每个节点开始扫描整个链表。
- 空间复杂度：$O(n)$ 。方法递归时所需的栈空间为 $O(n)$ 。如果改为迭代形式，则所需空间为 $O(1)$ 。

---
### 解法2 前缀和 + 哈希表
**若链表节点的两个前缀和相等，说明两个前缀和之间的连续节点序列的和为 $0$** ，那么可以消去这部分连续节点。

我们第一次遍历链表，用哈希表 $sum$ 记录前缀和以及对应的链表节点，**对于同一前缀和 $s$ ，令后面出现的节点覆盖前面的节点**。

为了方便链表删除，我们用一个虚拟的头节点 $dummy$ ，令其前缀和值为 $0$ ，$dummy.next = head$ 。接下来再次遍历链表，从虚拟头节点开始，当前节点 $cur$ 的前缀和 $s$ 必然会在 $sum$ 出现：
- 如果当前节点 $cur$ 的前缀和 $s$ 没有被覆盖过，得到的节点就是 $s$ 自身；
- 否则说明「$cur.next$ 与 $sum[s]$ 之间的所有节点」和为 $0$ 。

**对这两种情况，我们统一修改 $cur$ 的指向**，即 $cur.next=sum[s].next$ ，这样就删去了这部分和为 $0$ 的连续节点。继续往后遍历，删除所有和为 $0$ 的连续节点。

最后返回链表的头节点 $dummy.next$ 。
```java
class Solution {
    public ListNode removeZeroSumSublists(ListNode head) { 
        ListNode dummy = new ListNode(0, head);
        var sum = new HashMap<Integer, ListNode>();
        ListNode cur = dummy; 
        int s = 0;
        while (cur != null) {
            s += cur.val;
            sum.put(s, cur); // 对于同一前缀和,后面的节点覆盖前面的节点
            cur = cur.next;
        }
        s = 0;
        cur = dummy;
        while (cur != null) {
            s += cur.val;
            cur.next = sum.get(s).next; // 如果没有相同的前缀和,得到的节点就是s
            cur = cur.next;
        }
        return dummy.next; 
    }
}
```
复杂度分析：
- 时间复杂度：$O(n)$ 
- 空间复杂度：$O(n)$ 。其中 $n$ 为链表的长度。