@[toc]


---
# 第1章
## 1.1 Course Introduction
### 阅读材料 Welcome to Algorithms, Part I
介绍本课程的一些资料，不过涵盖第一部分和第二部分的[syllabus](https://www.coursera.org/learn/algorithms-part1/resources/CrmR4)要详细地多：
- 第一部分（关注 `elementary data structure, sorting, searching` ，主题包括 `union-find, binary search, stacks, queues, bags, insertion sort, selection sort, shellsort, quicksort, 3-way quicksort, mergesort, heapsort, binary heaps, binary search trees, red-black trees, separate-chaining and linear-probing hash tables, Graham scan, kd-trees`）
- 第二部分（关注 `graph, string-processing algorithms` ，主题包括 `depth-first search, breadth-first search, topological sort, Kosaraju-Sharir, Kruskal, Prim, Dijkstra, Bellman-Ford, Ford-Fulkerson, LSD radix sort, MSD radix sort, 3-way radix quicksort, multiway tries, ternary search tries, Knuth-Morris-Pratt, Boyer-Moore, Rabin-Karp, regular expression matching, run-length coding, Huffman coding, LZW compression, Burros-Wheeler transform, B-tree, suffix array, maxflow` 以及 `reductions, intractability` 即 `P = NP Problem`）
- 本课程的前置条件：对Java的 `loops, arrays, functions, recursion, objects` 的熟悉和高中代数知识
- 以算法第四版作为 `basic reference` ，详尽地覆盖了这门课的主题，实际上包括了比这门课能讲的多得多的主题。算法第四版的[booksite](https://algs4.cs.princeton.edu/home/)（包括本书作者自己写的库、各章节的Java代码、[其他高级数据结构](https://algs4.cs.princeton.edu/code/)以及非常多的信息，可能是书中内容的十倍，看书的同时记得常来这里逛逛）。虽然 `lectures` 被设计为 `self-contained` ，但会在书中指定可选的 `readings` 
- 每周两个75分钟的 `lecture` ，每个 `lecture` 被交互性的 `quiz` 分成4-6段，lecture videos、lecture slides、programming assignments会发布在课程网站上
- 每个 `lecture` 都有一个 `exercise` ，包含3个问题，每个问题需要10-30分钟。每个 `exercise` 可以回答十次，最佳成绩被保留
- `programming  assignments` 用Java，推荐用[IntelliJ-based programming environment](https://lift.cs.princeton.edu/java/windows)
- `Final exam` ：不包括Java编程，许多问题基于 `exercises` 和课程中的 `quizzes` ，可以做三次，最佳成绩被保留

最后就是建议 `being an active participant who writes and debugs code, solves problems, studies the available resources, and engages in the discussion forums` 。

### 阅读材料 Lecture Slides
见[课程网站](https://d3c33hcgiwev3.cloudfront.net/_f61d4a32d33d174594ef36ee4f08611c_00Intro.pdf?Expires=1632960000&Signature=cgTz1aVaXSO8c7m9tjXOZiOpyIMFrCz1Ruvuoco7j54ZK7ZBlia8qWdRXK6MGsgovCzDdkRUpTS2wLs~zPagWx2N27avsvjOGfv5ZTHcGoff~eS~iWHYJi8uPxM08q~OLG7li7ZPL-qKzXWe3c10wdJbXT6aGgIJDZBB1Z4zKLo_&Key-Pair-Id=APKAJLTNE6QMUY6HBC5A)。

### Course Introduction
课程内容的简要介绍，为什么要学习算法等等。交互式 `quiz` 是：
![在这里插入图片描述](https://img-blog.csdnimg.cn/8a11a41641dd4a7ea7ae5c20c9301824.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

## 1.2 Union-FInd
### 1.2.1 Dynamic Connectivity
UF类的测试客户端Client

### 1.2.2 Quick Find
### 1.2.3 Quick Union
### 1.2.4 Quick-Union(by size) Improvements
### 1.2.5 Union-Find Applications
Percolation Model
![在这里插入图片描述](https://img-blog.csdnimg.cn/f67da21d122b4aa1999b01b8194e50e7.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

### Practice Quiz: Interview Questions: Union-Find
![在这里插入图片描述](https://img-blog.csdnimg.cn/f71346f0922444c4b2eaad28af0e62c2.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16) Question 1

**Social network connectivity**. Given a social network containing nnn members and a log file containing mmm timestamps at which times pairs of members formed friendships, design an algorithm to determine the earliest time at which all members are connected (i.e., every member is a friend of a friend of a friend ... of a friend). Assume that the log file is sorted by timestamp and that friendship is an equivalence relation. The running time of your algorithm should be mlog⁡nm \log nmlogn or better and use extra space proportional to nnn.

Note: these interview questions are ungraded and purely for your own enrichment. To get a hint, submit a solution.


---

---
Lecture: Union−Find.
several implementations:
quick find, quick union, weighted quick union, and weighted quick union with path compression 
apply the union–find data type to the percolation problem from physical chemistry.
物理化学中的渗流问题。
### Programming Assignment: Percolation.
**https://www.cnblogs.com/anne-vista/p/4841732.html**

a fundamental problem in physical chemistry
the first of many examples where a good algorithm makes the difference


**UF  (by rank of height) without path compression 为什么不实现为抽象类？
QuickFindUF  Find: O(1) Union:O(n)
QuickUnionUF Find: O(lg N) Union O(lg N) 普通实现
WeightedQuickUnionUF** with path compression (by rank of size)

## 1.3 Analysis of Algorithms
**BinarySearch**
**ThreeSum**
**ThreeSumFast**

Lecture: Analysis of Algorithms.
The basis of our approach for analyzing the performance of algorithms is
the scientific method

begin : performing computational experiments to **measure the running times** of out programs, to develop hypotheses about performance.

create mathematical models to explain their behavior

analyzing the memory usage of our Java programs.

## Job Interview Questions. 
**Algorithmic interview questions** based on the lecture material.
## Suggested Readings. 
Section 1.4 and 1.5 in Algorithms, 4th edition.

![在这里插入图片描述](https://img-blog.csdnimg.cn/ab8c05b6a7bb442c88f35856c82580c2.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

---
# 第2章
## 2.1 Stacks and Queues
 
 
### 阅读材料: Overview
 
### 阅读材料: Lecture Slides
### 视频: 课程视频Stacks
 
### 视频: 课程视频Resizing Arrays
 
### 视频: 课程视频Queues
 
### 视频: 课程视频Generics
 
### 视频: 课程视频Iterators

### 视频: 课程视频Stack and Queue Applications (optional)

### 练习测验: Interview Questions: Stacks and Queues (ungraded)1.
第 1 个问题

Queue with two stacks. Implement a queue with two stacks so that each queue operations takes a constant amortized number of stack operations.

Note: these interview questions are ungraded and purely for your own enrichment. To get a hint, submit a solution.

2.
第 2 个问题

Stack with max. Create a data structure that efficiently supports the stack operations (push and pop) and also a return-the-maximum operation. Assume the elements are real numbers so that you can compare them.

3.
第 3 个问题

Java generics. Explain why Java prohibits generic array creation.
###   编程作业: Deques and Randomized Queues
 

## 2.2 Elementary Sorts
### 阅读材料: Lecture Slides
### 视频: 课程视频Sorting Introduction
 
### 视频: 课程视频Selection Sort
 
### 视频: 课程视频Insertion Sort
 
### 视频: 课程视频Shellsort
 
### 视频: 课程视频Shuffling
 
### 视频: 课程视频Convex Hull
 
### 练习测验: Interview Questions: Elementary Sorts (ungraded)
1.
第 1 个问题
![在这里插入图片描述](https://img-blog.csdnimg.cn/e6e8d11013254769a3c8720c62783756.png)

Intersection of two sets. Given two arrays a[]\mathtt{a[]}a[] and b[]\mathtt{b[]}b[], each containing nnn distinct 2D points in the plane, design a subquadratic algorithm to count the number of points that are contained both in array a[]\mathtt{a[]}a[] and array b[]\mathtt{b[]}b[].

Note: these interview questions are ungraded and purely for your own enrichment. To get a hint, submit a solution.

 
第 2 个问题
![在这里插入图片描述](https://img-blog.csdnimg.cn/769ebddf8c554e1da714ab0030599788.png)

Permutation. Given two integer arrays of size nnn, design a subquadratic algorithm to determine whether one is a permutation of the other. That is, do they contain exactly the same entries but, possibly, in a different order.
 
 
第 3 个问题
![在这里插入图片描述](https://img-blog.csdnimg.cn/24baf36b88f0423c992fb69a90cfafa0.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

Dutch national flag. Given an array of nnn buckets, each containing a red, white, or blue pebble, sort them by color. The allowed operations are:

    swap(i,j)swap(i, j)swap(i,j):  swap the pebble in bucket iii with the pebble in bucket jjj.

    color(i)color(i)color(i): determine the color of the pebble in bucket iii.

The performance requirements are as follows:

    At most nnn calls to color()color()color().

    At most nnn calls to swap()swap()swap().

    Constant extra space.

1 分


3 个问题

---
# 第3章
## 3.1 Mergesort
## 3.2 Quicksort

---
# 第4章
## 4.1 Priority Queues
## 4.2 Elementary Symbol Tabls

---
# 第5章
## 5.1 Balanced Search Trees
## 5.2 Geometric Applications of BSTs
---
# 第6章
## 6.1 Hash Tables
## 6.2 Symbol Table Applications

