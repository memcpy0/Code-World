> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个字符串数组 `event1` 和 `event2` ，表示发生在同一天的两个闭区间时间段事件，其中：
-   `event1 = [startTime1, endTime1]` 且
-   `event2 = [startTime2, endTime2]`

事件的时间为有效的 24 小时制且按 `HH:MM` 格式给出。

当两个事件存在某个非空的交集时（即，某些时刻是两个事件都包含的），则认为出现 **冲突** 。

如果两个事件之间存在冲突，返回 `true` ；否则，返回 `false` 。

**示例 1：**
```java
输入：event1 = ["01:15","02:00"], event2 = ["02:00","03:00"]
输出：true
解释：两个事件在 2:00 出现交集。
```
**示例 2：**
```java
输入：event1 = ["01:00","02:00"], event2 = ["01:20","03:00"]
输出：true
解释：两个事件的交集从 01:20 开始，到 02:00 结束。
`
```
**示例 3：**
```java
输入：event1 = ["10:00","11:00"], event2 = ["14:00","15:00"]
输出：false
解释：两个事件不存在交集。
```
**提示：**
-   `evnet1.length == event2.length == 2.`
-   `event1[i].length == event2[i].length == 5`
-   `startTime1 <= endTime1`
-   `startTime2 <= endTime2`
-   所有事件的时间都按照 `HH:MM` 格式给出

---
### 解法 比较两对字符串
很简单的题目，**不冲突的情况是：某个事件的结束时间小于另一个事件的开始时间**；对不冲突的情况取反，就是冲突了。如下按照字典序比较两个字符串：
```cpp
class Solution {
public:
    bool haveConflict(vector<string>& event1, vector<string>& event2) {
        return !(event1[1] < event2[0] || event2[1] < event1[0]);
    }
};
```
上面是反向思考，下面是正面思考。画出有交集的多种情况，发现都满足：
- 第一个事件的开始时间不晚于第二个事件的结束时间
- 第二个事件的开始时间不晚于第一个事件的结束时间

```java
class Solution {
    public boolean haveConflict(String[] event1, String[] event2) {
        return event1[0].compareTo(event2[1]) <= 0 && event2[0].compareTo(event1[1]) <= 0;
    }
}
```