 > 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个嵌套的整数列表 `nestedList` 。每个元素要么是一个整数，要么是一个列表；该列表的元素也可能是整数或者是其他列表。请你实现一个迭代器将其扁平化，使之能够遍历这个列表中的所有整数。

实现扁平迭代器类 `NestedIterator` ：
- `NestedIterator(List<NestedInteger> nestedList)` 用嵌套列表 `nestedList` 初始化迭代器。
- `int next()` 返回嵌套列表的下一个整数。
- `boolean hasNext()` 如果仍然存在待迭代的整数，返回 `true` ；否则，返回 `false` 。

你的代码将会用下述伪代码检测：
```js
initialize iterator with nestedList
res = []
while iterator.hasNext()
    append iterator.next() to the end of res
return res
```
如果 `res` 与预期的扁平化列表匹配，那么你的代码将会被判为正确。

**示例 1：**
```js
输入：nestedList = [[1,1],2,[1,1]]
输出：[1,1,2,1,1]
解释：通过重复调用 next 直到 hasNext 返回 false，next 返回的元素的顺序应该是: `[1,1,2,1,1]`。
```
**示例 2：**
```js
输入：nestedList = [1,[4,[6]]]
输出：[1,4,6]
解释：通过重复调用 next 直到 hasNext 返回 false，next 返回的元素的顺序应该是: `[1,4,6]`。
```
**提示：**
- `1 <= nestedList.length <= 500`
- 嵌套列表中的整数值在范围 `[-10^6, 10^6]` 内

---
### 解法 深度优先搜索+存入数组
嵌套的整型列表是一个树形结构，**树上的叶子节点对应一个整数，非叶节点对应一个列表**。在这棵树上，**深度优先搜索的顺序就是迭代器遍历的顺序**。

我们可以先遍历整个嵌套列表，将所有整数存入一个数组，然后遍历该数组从而实现 $next$ 和 $hasNext$ 方法。
```cpp
class NestedIterator {
private:
    vector<int> vals;
    vector<int>::iterator cur;

    void dfs(const vector<NestedInteger> &nestedList) {
        for (auto &nest : nestedList) {
            if (nest.isInteger()) {
                vals.push_back(nest.getInteger());
            } else {
                dfs(nest.getList());
            }
        }
    }

public:
    NestedIterator(vector<NestedInteger> &nestedList) {
        dfs(nestedList);
        cur = vals.begin();
    }

    int next() {
        return *cur++;
    }

    bool hasNext() {
        return cur != vals.end();
    }
};
```
复杂度分析：
- 时间复杂度：初始化为 $O(n)$ ，$next$ 和 $hasNext$ 为 $O(1)$ 。其中 $n$ 是嵌套的整型列表中的元素个数。
- 空间复杂度：$O(n)$ 。需要一个数组存储嵌套的整型列表中的所有元素。

---
### 解法2 栈
我们可以用一个栈来代替方法一中的递归过程。

具体来说，==用一个栈来维护深度优先搜索时，从根节点到当前节点路径上的所有节点==。由于非叶节点对应的是一个列表，我们在栈中存储的是指向列表当前遍历的元素的**指针（下标）**。
- 每次向下搜索时，取出列表的当前指针指向的元素并将其入栈，同时将该指针向后移动一位。
- 如此反复直到找到一个整数。
- 循环时若栈顶指针指向了列表末尾，则将其从栈顶弹出。

下面的代码中C++的栈存储的是迭代器，迭代器可以起到指向元素的指针的效果。
```cpp
class NestedIterator {
private:
    // pair 中存储的是列表的当前遍历位置，以及一个尾后迭代器用于判断是否遍历到了列表末尾
    stack<pair<vector<NestedInteger>::iterator, vector<NestedInteger>::iterator>> stk;
public:
    NestedIterator(vector<NestedInteger> &nestedList) {
        stk.emplace(nestedList.begin(), nestedList.end());
    }

    int next() {
        // 由于保证调用 next 之前会调用 hasNext，直接返回栈顶列表的当前元素，然后迭代器指向下一个元素
        return stk.top().first++->getInteger();
    }

    bool hasNext() {
        while (!stk.empty()) {
            auto &p = stk.top();
            if (p.first == p.second) { // 遍历到当前列表末尾，出栈
                stk.pop();
                continue;
            }
            if (p.first->isInteger()) {
                return true;
            }
            // 若当前元素为列表，则将其入栈，且迭代器指向下一个元素
            auto &lst = p.first++->getList();
            stk.emplace(lst.begin(), lst.end());
        }
        return false;
    }
};
```
复杂度分析：
- 时间复杂度：初始化和 $next$ 为 $O(1)$ ，$hasNext$ 为均摊 $O(1)$ 。
- 空间复杂度：$O(n)$。最坏情况下嵌套的整型列表是一条链，我们需要一个 $O(n)$ 大小的栈来存储链上的所有元素。
