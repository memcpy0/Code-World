> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


---
### 解法 图论
`A` 信任 `B` 可以抽象为 `A` 到 `B` 的一条有向路径，即 `A -> B` 。因此题目所求为——是否存在一个顶点，其入度为 `N - 1` 、出度为 `0` 。~~下面是以前写的乱七八糟的东西~~ ：
> 可能存在歧义的是，第三点对法官的要求——`只有一个人同时满足属性1和属性2` 。一种理解是，题目给定的 `trust[]` 中只有一个人满足这两个要求；另一种理解是，`trust[]` 中可能有多个人满足这两个要求，如果是这样，那么这些人都不是法官，需要返回 `-1` 。
> <b></b>
> 事实上，只有第一种理解是正确的。因此，如果找到一个入度为 `N - 1` 、出度为 `0` 的顶点，毫无疑问他就是法官，不存在多个可能的顶点。简单的证明：
> - 设有 `x` 个疑似法官的人，即 `x` 个顶点入度为 `N - 1` 、出度为 `0` ；
> - 剩下 `N - x` 个人，每个人都可以信任多个人，但是不可以多次信任一个人（不存在多个相同的 `trust[i]` ），也就是说每个人到某个人至多只有一条有向路径；
> - 于是对于疑似法官而言，如果他得到了剩下所有 `N - x` 人的信任，也就只有 `N - x` 入度。显然，只有 `x = 1` 时他才是法官。

具体代码如下。使用入度和出度数组，先遍历 `trust` 并统计每个节点的入度和出度，再遍历所有点，若存在入度数量为 `N - 1` 、出度数量为 `0` 的节点，即是法官。
```cpp
//C++ version
class Solution {
public:
    int findJudge(int N, vector<vector<int>>& trust) { 
        int n = trust.size();
        vector<int> indegrees(N + 1), outdegrees(N + 1); //入度数组
        for (int i = 0; i < n; ++i) {
            ++outdegrees[trust[i][0]];  //a->b, 对于a是出边,对于b是入边
            ++indegrees[trust[i][1]];   //b的入度+1
        }
        for (int i = 1; i <= N; ++i) 
            if (indegrees[i] == N - 1 && outdegrees[i] == 0) //N-1个人都信任他,他不信任任何人
                return i;
        return -1;
    }
};
//执行用时：136 ms, 在所有 C++ 提交中击败了88.56% 的用户
//内存消耗：59.2 MB, 在所有 C++ 提交中击败了82.76% 的用户
```
简单写成Java：
```java
class Solution {
    public int findJudge(int N, int[][] trust) {
        int n = trust.length;
        int[] indegrees = new int[N + 1];
        int[] outdegrees = new int[N + 1];
        for (int i = 0; i < n; ++i) {
            ++outdegrees[trust[i][0]];
            ++indegrees[trust[i][1]];
        }
        for (int i = 1; i <= N; ++i) 
            if (indegrees[i] == N - 1 && outdegrees[i] == 0) 
                return i;
        return -1;
    }
}
//执行用时：2 ms, 在所有 Java 提交中击败了98.57% 的用户
//内存消耗：45.5 MB, 在所有 Java 提交中击败了95.12% 的用户
```
这不吊打C++？看来我大Java的时代已经到来了啊😄
