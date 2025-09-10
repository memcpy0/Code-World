> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `citations` ，其中 `citations[i]` 表示研究者的第 `i` 篇论文被引用的次数。计算并返回该研究者的 **`h` 指数**。

根据维基百科上 [h 指数的定义](https://baike.baidu.com/item/h-index/3991452?fr=aladdin)：`h` 代表“高引用次数” ，一名科研人员的 `h` **指数** 是指他（她）至少发表了 `h` 篇论文，并且每篇论文 **至少** 被引用 `h` 次。如果 `h` 有多种可能的值，**`h` 指数** 是其中最大的那个。

**示例 1：**
```js
输入：`citations = [3,0,6,1,5]`
输出：3 
解释：给定数组表示研究者总共有 `5` 篇论文，每篇论文相应的被引用了 `3, 0, 6, 1, 5` 次。
     由于研究者有 `3` 篇论文每篇 至少 被引用了 `3` 次，其余两篇论文每篇被引用 不多于 `3` 次，所以她的 h 指数是 `3`。
```
**示例 2：**
```js
输入：citations = [1,3,1]
输出：1
```
**提示：**
- `n == citations.length`
- `1 <= n <= 5000`
- `0 <= citations[i] <= 1000`

---
### 解法1 排序
这一题的题意有点绕口，不过其实很简单。我们可以将给出的**引用次数数组** `citations[]` 从小到大排序，然后从头开始扫描整个数组——对于给定的 `i` ，我们已知有 `citations.length - i` 篇论文的引用次数都大于等于 `citations[i]` ，`i` 篇论文的引用数小于等于 `citations[i]` 。

不妨设 `h = citations.length - i` ，即有 `h` 篇论文分别引用了至少 `citations[i]` 次，其他 `citations.length - h` 篇论文的引用次数不超过 `citations[i]` 次。只要 `citations[i] >= h` ，就满足题意，有 `h` 篇论文的引用次数至少为 `h` ： 
```cpp
//cplusplus
class Solution {
public:
    int hIndex(vector<int>& citations) {
        sort(citations.begin(), citations.end());
        for (int i = 0, n = citations.size(); i < n; ++i) 
            if (citations[i] >= n - i) return n - i;
        return 0;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：8.4 MB, 在所有 C++ 提交中击败了27.70% 的用户
```
```java
//java
class Solution {
    public int hIndex(int[] citations) {
        Arrays.sort(citations);
        for (int i = 0; i < citations.length; ++i) {
            int H = citations.length - i;
            if (citations[i] >= H) return H;
        }
        return 0; 
    }
}
//执行用时：1 ms, 在所有 Java 提交中击败了85.63% 的用户
//内存消耗：36.4 MB, 在所有 Java 提交中击败了29.30% 的用户
```
```py
#python3
class Solution:
    def hIndex(self, citations: List[int]) -> int:
        citations.sort()
        for i in range(0, len(citations)):
            h = len(citations) - i
            if citations[i] >= h:
                return h
        return 0
#执行用时：40 ms, 在所有 Python3 提交中击败了60.07% 的用户
#内存消耗：14.9 MB, 在所有 Python3 提交中击败了86.88% 的用户
```
如果用Ruby的话……可怕的空间消耗：
```ruby
#ruby
# @param {Integer[]} citations
# @return {Integer}
def h_index(citations)
    citations.sort!() # 原地排序数组
    n = citations.size()
    for i in 0..(n - 1)
        if citations[i] >= n - i 
            return n - i
        end
    end
    return 0
end
#执行用时：40 ms, 在所有 Ruby 提交中击败了100.00% 的用户
#内存消耗：205.1 MB, 在所有 Ruby 提交中击败了100.00% 的用户
```
---
### 解法2 计数排序+前缀和
由于引用次数的值范围比较小，可以利用额外空间，使用计数排序来提升效率：
```cpp
//cpp
class Solution {
public:
    int hIndex(vector<int>& citations) {
        int cnt[1010] = {0};
        for (const int &v : citations) ++cnt[v];
        for (int i = 999; i >= 0; --i) cnt[i] += cnt[i + 1];
        for (int h = 1000; h >= 0; --h) {
            //cnt[h]:引用次数至少为h的论文数目为cnt[h]
            if (cnt[h] >= h) return h; //说明至少有h篇(共有cnt[h]篇)文章的引用次数至少为h
        }
        return 0;
    }
};  
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：8.4 MB, 在所有 C++ 提交中击败了31.54% 的用户
```
