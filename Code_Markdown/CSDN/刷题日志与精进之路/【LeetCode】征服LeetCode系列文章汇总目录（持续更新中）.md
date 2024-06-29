> 本文属于「征服LeetCode」系列文章的汇总目录，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏本文以作备忘。

---
注意，标记有:star:的题目，表示其是一系列相似题目的起点/终点；标记有:fire:的题目，表示其能激起对算法的热情。

# 1. LeetCode题解文章汇总
| 原题链接 | 题解链接|
|:---|:---
| [29. Divide Two Integers](https://leetcode-cn.com/problems/divide-two-integers/)  | [29. Divide Two Integers【数学/二分/倍增/位操作】中等](https://memcpy0.blog.csdn.net/article/details/120764444)
| [36. Valid Sudoku](https://leetcode-cn.com/problems/valid-sudoku/) | [36. Valid Sudoku【哈希表/位操作】中等](https://memcpy0.blog.csdn.net/article/details/120580502)
| [37. Sudoku Solver](https://leetcode-cn.com/problems/sudoku-solver/) | [37. Sudoku Solver【回溯/递归/数组】困难](https://memcpy0.blog.csdn.net/article/details/120581936)
|   [38. Count and Say](https://leetcode-cn.com/problems/count-and-say/) |  [38. Count and Say【字符串/递归/迭代/模拟/打表】中等](https://memcpy0.blog.csdn.net/article/details/106726954) 
| :star::fire:[42. Trapping Rain Water](https://leetcode-cn.com/problems/trapping-rain-water/) | [42. Trapping Rain Water【数组/动态规划/单调栈/栈/双指针】困难](https://memcpy0.blog.csdn.net/article/details/121136143)
|  [58. Length of Last Word](https://leetcode-cn.com/problems/length-of-last-word/)  | [58. Length of Last Word【模拟/字符串】简单](https://memcpy0.blog.csdn.net/article/details/120545600)   
|:star:[69. Sqrt(x)]()  |  [69. Sqrt(x)【数学/二分】简单](https://memcpy0.blog.csdn.net/article/details/121135932)
| :star::fire:[72. Edit Distance](https://leetcode-cn.com/problems/edit-distance/) | 
|  [85. Maximal Rectangle](https://leetcode-cn.com/problems/maximal-rectangle/)  |   [85. Maximal Rectangle【单调栈/悬线法（动态规划）】困难](https://memcpy0.blog.csdn.net/article/details/116775219)
| :star:[136. Single Number](https://leetcode-cn.com/problems/single-number/) | [136. Single Number【哈希表/位运算/数学】简单](https://memcpy0.blog.csdn.net/article/details/106988806)
|  [137. Single Number II](https://leetcode-cn.com/problems/single-number-ii/) | [137. Single Number II【哈希表/位运算/数学】中等](https://memcpy0.blog.csdn.net/article/details/121064459)
| :star:[141. Linked List Cycle]() | 
| [142. Linked List Cycle II](https://leetcode-cn.com/problems/linked-list-cycle-ii/) |
| [162. Find Peak Element](https://leetcode-cn.com/problems/find-peak-element/)  | [162. Find Peak Element【数组/二分】中等](https://memcpy0.blog.csdn.net/article/details/120793457)
|    [165. Compare Version Numbers](https://leetcode-cn.com/problems/compare-version-numbers/) | [165. Compare Version Numbers【字符串/模拟】中等](https://memcpy0.blog.csdn.net/article/details/120057739)
| [166. Fraction to Recurring Decimal](https://leetcode-cn.com/problems/fraction-to-recurring-decimal/) | [166. Fraction to Recurring Decimal【哈希表/模拟/字符串】中等](https://memcpy0.blog.csdn.net/article/details/121299458)
|  :fire:[187. Repeated DNA Sequences](https://leetcode-cn.com/problems/repeated-dna-sequences/)    |   [187. Repeated DNA Sequences【哈希表/滑动窗口/字符串哈希/滚动哈希/位操作】中等](https://memcpy0.blog.csdn.net/article/details/120662605)
| [221. Maximal Square](https://leetcode-cn.com/problems/maximal-square/) |  [221. Maximal Square【单调栈/悬线法/动态规划】中等](https://memcpy0.blog.csdn.net/article/details/120205494)
| [223. Rectangle Area](https://leetcode-cn.com/problems/rectangle-area/)  |  [223. Rectangle Area【数学/计算几何】中等](https://memcpy0.blog.csdn.net/article/details/120574580)
| [231. Power of Two](https://leetcode-cn.com/problems/power-of-two/)  | [231. Power of Two【迭代/位操作/数学/打表】简单](https://memcpy0.blog.csdn.net/article/details/120535883)
| [233. Number of Digit One](https://leetcode-cn.com/problems/number-of-digit-one/)   | [233. Number of Digit One【计数模拟】困难](https://memcpy0.blog.csdn.net/article/details/119744426)
|   [237. Delete Node in a Linked List](https://leetcode-cn.com/problems/delete-node-in-a-linked-list/) | [237. Delete Node in a Linked List【链表】简单](https://memcpy0.blog.csdn.net/article/details/108183127)
| [238. Product of Array Except Self](https://leetcode-cn.com/problems/product-of-array-except-self/) | [238. Product of Array Except Self【前缀和/数组】中等](https://memcpy0.blog.csdn.net/article/details/106559388)
|  [260. Single Number III](https://leetcode-cn.com/problems/single-number-iii/) | [260. Single Number III【哈希表/位运算/数学】中等](https://memcpy0.blog.csdn.net/article/details/121064507)
| :star:[268. Missing Number](https://leetcode-cn.com/problems/missing-number/) | [268. Missing Number【位操作/数学/哈希表】简单](https://memcpy0.blog.csdn.net/article/details/106988398) 
| [273. Integer to English Words](https://leetcode-cn.com/problems/integer-to-english-words/) | [273. Integer to English Words【字符串/模拟】困难](https://memcpy0.blog.csdn.net/article/details/120714218)
| :star:[284. Peeking Iterator](https://leetcode-cn.com/problems/peeking-iterator/) | [284. Peeking Iterator【设计/迭代器/数组】中等](https://memcpy0.blog.csdn.net/article/details/121270384)
|   [287. Find the Duplicate Number](https://leetcode-cn.com/problems/find-the-duplicate-number/) |
|  [295. Find Median from Data Stream](https://leetcode-cn.com/problems/find-median-from-data-stream/)  | [295. Find Median from Data Stream【堆】困难](https://memcpy0.blog.csdn.net/article/details/120076311)
|  [299. Bulls and Cows](https://leetcode-cn.com/problems/bulls-and-cows/) | [299. Bulls and Cows【哈希表/字符串/计数】中等](https://memcpy0.blog.csdn.net/article/details/121216063)
| :star:[319. Bulb Switcher](https://leetcode-cn.com/problems/bulb-switcher/)   |  [319. Bulb Switcher【数论】中等](https://memcpy0.blog.csdn.net/article/details/122116523)
|  [326. Power of Three](https://leetcode-cn.com/problems/power-of-three/)  | [326. Power of Three【迭代/打表/数学】简单](https://memcpy0.blog.csdn.net/article/details/120536998)
| [335. Self Crossing](https://leetcode-cn.com/problems/self-crossing/) | [335. Self Crossing【计算几何/数学/数组】困难](https://memcpy0.blog.csdn.net/article/details/121039478)
|  [342. Power of Four](https://leetcode-cn.com/problems/power-of-four/)  | [342. Power of Four【迭代/位操作/数学/打表】简单](https://memcpy0.blog.csdn.net/article/details/117408777)
| [345. Reverse Vowels of a String](https://leetcode-cn.com/problems/reverse-vowels-of-a-string/) | [345. Reverse Vowels of a String【双指针】简单](https://memcpy0.blog.csdn.net/article/details/108963434)
| [367. Valid Perfect Square](https://leetcode-cn.com/problems/valid-perfect-square/)  | [367. Valid Perfect Square【数学/二分】简单](https://memcpy0.blog.csdn.net/article/details/109349499)
| [371. Sum of Two Integers](https://leetcode-cn.com/problems/sum-of-two-integers/)  | [371. Sum of Two Integers【递归/迭代/位运算】中等](https://memcpy0.blog.csdn.net/article/details/109349471)
| [372. Super Pow](https://leetcode-cn.com/problems/super-pow/)  | [372. Super Pow【数学/递归/快速幂】中等](https://memcpy0.blog.csdn.net/article/details/110154274)
| :star:[375. Guess Number Higher or Lower II](https://leetcode-cn.com/problems/guess-number-higher-or-lower-ii/) | []()

| [389. Find the Difference](https://leetcode-cn.com/problems/find-the-difference/)  | [389. Find the Difference【哈希表/位运算】简单](https://memcpy0.blog.csdn.net/article/details/106988949)  
| [398. Random Pick Index](https://leetcode-cn.com/problems/random-pick-index/) | [398. Random Pick Index【蓄水池抽样/哈希表/随机化】中等](https://memcpy0.blog.csdn.net/article/details/120022542)
| [405. Convert a Number to Hexadecimal](https://leetcode-cn.com/problems/convert-a-number-to-hexadecimal/) | [405. Convert a Number to Hexadecimal【位操作】简单](https://memcpy0.blog.csdn.net/article/details/108908349)
| [407. Trapping Rain Water II](https://leetcode-cn.com/problems/trapping-rain-water-ii/) | [407. Trapping Rain Water II【数组/BFS/堆/最短路】困难](https://memcpy0.blog.csdn.net/article/details/121136265)
| [412. Fizz Buzz](https://leetcode-cn.com/problems/fizz-buzz/) | [412. Fizz Buzz【字符串/模拟】简单](https://memcpy0.blog.csdn.net/article/details/120756690)
| [413. Arithmetic Slices](https://leetcode-cn.com/problems/arithmetic-slices/)  | [413. Arithmetic Slices【数组/双指针/动态规划】中等](https://memcpy0.blog.csdn.net/article/details/119641437)
| [414. Third Maximum Number](https://leetcode-cn.com/problems/third-maximum-number/)  | 
|     [419. Battleships in a Board](https://leetcode-cn.com/problems/battleships-in-a-board/) |  [419. Battleships in a Board【BFS/DFS/脑筋急转弯】中等](https://memcpy0.blog.csdn.net/article/details/122033533)
| [430. Flatten a Multilevel Doubly Linked List](https://leetcode-cn.com/problems/flatten-a-multilevel-doubly-linked-list/)  |  [430. Flatten a Multilevel Doubly Linked List【链表/DFS/递归/迭代】中等](https://memcpy0.blog.csdn.net/article/details/120468312)
| [434. Number of Segments in a String](https://leetcode-cn.com/problems/number-of-segments-in-a-string/) | [434. Number of Segments in a String【字符串/模拟】简单](https://memcpy0.blog.csdn.net/article/details/109349573)
|  [437. Path Sum III](https://leetcode-cn.com/problems/path-sum-iii/)  |  [437. Path Sum III【递归/前缀和/哈希表/回溯】中等](https://memcpy0.blog.csdn.net/article/details/107207793)
| [441. Arranging Coins](https://leetcode-cn.com/problems/arranging-coins/) | [441. Arranging Coins【数学/二分】简单](https://memcpy0.blog.csdn.net/article/details/119641437) 
| [443. String Compression](https://leetcode-cn.com/problems/string-compression/) |  [443. String Compression【字符串/双指针】中等](https://memcpy0.blog.csdn.net/article/details/119845202)
| [447. Number of Boomerangs](https://leetcode-cn.com/problems/number-of-boomerangs/)  | [447. Number of Boomerangs【哈希表】中等](https://memcpy0.blog.csdn.net/article/details/120275350)
| [470. Implement Rand10() Using Rand7()](https://leetcode-cn.com/problems/implement-rand10-using-rand7/) | 
|  :fire:  [475. Heaters](https://leetcode-cn.com/problems/heaters/)  | [475. Heaters【二分/双指针/数组】中等](https://memcpy0.blog.csdn.net/article/details/108963774)

| [476. Number Complement](https://leetcode-cn.com/problems/number-complement/) | [476. Number Complement【位运算】简单](https://memcpy0.blog.csdn.net/article/details/108866890)
| [482. License Key Formatting](https://leetcode-cn.com/problems/license-key-formatting/) | [482. License Key Formatting【字符串/模拟】简单](https://memcpy0.blog.csdn.net/article/details/108871763)
 
| [488. Zuma Game](https://leetcode-cn.com/problems/zuma-game/) | []()
|  [495. Teemo Attacking](https://leetcode-cn.com/problems/teemo-attacking/) | [495. Teemo Attacking【模拟】简单](https://memcpy0.blog.csdn.net/article/details/121259058)
| [497. Random Point in Non-overlapping Rectangles](https://leetcode-cn.com/problems/random-point-in-non-overlapping-rectangles/) | [497. Random Point in Non-overlapping Rectangles【数学/前缀和/二分/随机化/有序集合/蓄水池抽样】](https://memcpy0.blog.csdn.net/article/details/120005910)
|  [500. Keyboard Row](https://leetcode-cn.com/problems/keyboard-row/) | [500. Keyboard Row【模拟/哈希表】简单](https://memcpy0.blog.csdn.net/article/details/108820492)
| [516. Longest Palindromic Subsequence](https://leetcode-cn.com/problems/longest-palindromic-subsequence/) | [516. Longest Palindromic Subsequence【字符串/动态规划】中等](https://memcpy0.blog.csdn.net/article/details/120108474)
| [520. Detect Capital](https://leetcode-cn.com/problems/detect-capital/) | [520. Detect Capital【字符串/模拟】简单](https://memcpy0.blog.csdn.net/article/details/109555817)

|  [528. Random Pick with Weight](https://leetcode-cn.com/problems/random-pick-with-weight/) | [528. Random Pick with Weight【数学/前缀和/随机化/二分】中等](https://memcpy0.blog.csdn.net/article/details/120005763)
| [541. Reverse String II](https://leetcode-cn.com/problems/reverse-string-ii/) | [541. Reverse String II【字符串】简单](https://memcpy0.blog.csdn.net/article/details/108963124)
| [551. Student Attendance Record I](https://leetcode-cn.com/problems/student-attendance-record-i/) | [551. Student Attendance Record I【字符串】简单](https://memcpy0.blog.csdn.net/article/details/109556361)
| [575. Distribute Candies](https://leetcode-cn.com/problems/distribute-candies/) | [575. Distribute Candies【哈希表/贪心】简单](https://memcpy0.blog.csdn.net/article/details/108810251)
|  [598. Range Addition II](https://leetcode-cn.com/problems/range-addition-ii/)   |   [598. Range Addition II【脑筋急转弯】简单](https://memcpy0.blog.csdn.net/article/details/109627465)
| :fire:[629. K Inverse Pairs Array](https://leetcode-cn.com/problems/k-inverse-pairs-array/)   | [629. K Inverse Pairs Array【动态规划/前缀和】困难](https://memcpy0.blog.csdn.net/article/details/121276035)
| [633. Sum of Square Numbers](https://leetcode-cn.com/problems/sum-of-square-numbers/)   |
|  [653. Two Sum IV - Input is a BST](https://leetcode-cn.com/problems/two-sum-iv-input-is-a-bst/)   |   []
| [704. Binary Search](https://leetcode-cn.com/problems/binary-search/) | [704. Binary Search【二分】简单](https://memcpy0.blog.csdn.net/article/details/108890175) 
| [709. To Lower Case](https://leetcode-cn.com/problems/to-lower-case/) |  [709. To Lower Case【字符串】简单](https://memcpy0.blog.csdn.net/article/details/107632888)
| [748. Shortest Completing Word](https://leetcode-cn.com/problems/shortest-completing-word/)  | [748. Shortest Completing Word【哈希表】简单](https://memcpy0.blog.csdn.net/article/details/108890484)

| [807. Max Increase to Keep City Skyline](https://leetcode-cn.com/problems/max-increase-to-keep-city-skyline/)  |  [807. Max Increase to Keep City Skyline【数组/贪心】中等](https://memcpy0.blog.csdn.net/article/details/112853811)
| [852. Peak Index in a Mountain Array](https://leetcode-cn.com/problems/peak-index-in-a-mountain-array/)  | [852. Peak Index in a Mountain Array【二分/三分/数组】简单](https://memcpy0.blog.csdn.net/article/details/109695803) 
| [836. Rectangle Overlap](https://leetcode-cn.com/problems/rectangle-overlap/) | [836. Rectangle Overlap【数学/计算几何】简单](https://memcpy0.blog.csdn.net/article/details/120574576)
|  [869. Reordered Power of 2](https://leetcode-cn.com/problems/reordered-power-of-2/)   | [869. Reordered Power of 2【数学/计数/枚举/排序】中等](https://memcpy0.blog.csdn.net/article/details/121026490)
| [881. Boats to Save People] | [881. Boats to Save People【贪心/双指针/排序】中等]
| [997. Find the Town Judge](https://leetcode-cn.com/problems/find-the-town-judge/) | [997. Find the Town Judge【图论】简单](https://memcpy0.blog.csdn.net/article/details/109577572)
|  [1005. Maximize Sum Of Array After K Negations](https://leetcode-cn.com/problems/maximize-sum-of-array-after-k-negations/)   |  [1005. Maximize Sum Of Array After K Negations【数组/排序/贪心】简单](https://memcpy0.blog.csdn.net/article/details/121940697)
|  [1009. Complement of Base 10 Integer](https://leetcode-cn.com/problems/complement-of-base-10-integer/) |  [1009. Complement of Base 10 Integer【位运算】简单](https://memcpy0.blog.csdn.net/article/details/108867646)
|   [1034. Coloring A Border](https://leetcode-cn.com/problems/coloring-a-border/)   |  [1034. Coloring A Border【图/DFS/BFS】中等](https://memcpy0.blog.csdn.net/article/details/109457222)

|   [1109. Corporate Flight Bookings](https://leetcode-cn.com/problems/corporate-flight-bookings/) | [1109. Corporate Flight Bookings【差分/树状数组/线段树】中等](https://memcpy0.blog.csdn.net/article/details/116857235)
| [1137. N-th Tribonacci Number](https://leetcode-cn.com/problems/n-th-tribonacci-number/) | 
| [1154. Day of the Year](https://leetcode-cn.com/problems/day-of-the-year/) | [1154. Day of the Year【模拟/前缀和】简单](https://memcpy0.blog.csdn.net/article/details/122117185)

| :fire:[1218. Longest Arithmetic Subsequence of Given Difference](https://leetcode-cn.com/problems/longest-arithmetic-subsequence-of-given-difference/) | [1218. Longest Arithmetic Subsequence of Given Difference【动态规划/哈希表/贪心】中等](https://memcpy0.blog.csdn.net/article/details/121185552)
| [1221. Split a String in Balanced Strings](https://leetcode-cn.com/problems/split-a-string-in-balanced-strings/) |
|  [1436. Destination City](https://leetcode-cn.com/problems/destination-city/)    |   [1436. Destination City【哈希表/字符串】简单](https://memcpy0.blog.csdn.net/article/details/109640507)
|  [1480. Running Sum of 1d Array](https://leetcode-cn.com/problems/running-sum-of-1d-array/) |  [1480. Running Sum of 1d Array【数组】简单](https://memcpy0.blog.csdn.net/article/details/109634506) |
| [1518. Water Bottles](https://leetcode-cn.com/problems/water-bottles/) | [1518. Water Bottles【数学/模拟】简单](https://memcpy0.blog.csdn.net/article/details/122007151)

| [1583. Count Unhappy Friends](https://leetcode-cn.com/problems/count-unhappy-friends/)  | [1583. Count Unhappy Friends【模拟/数组】中等](https://memcpy0.blog.csdn.net/article/details/108651283)
| [1588. Sum of All Odd Length Subarrays](https://leetcode-cn.com/problems/sum-of-all-odd-length-subarrays/) | [1588. Sum of All Odd Length Subarrays【前缀和/模拟/数学】简单](https://memcpy0.blog.csdn.net/article/details/108710821)
| :fire: [1610. Maximum Number of Visible Points](https://leetcode-cn.com/problems/maximum-number-of-visible-points/)   | [1610. Maximum Number of Visible Points【计算几何/排序/滑动窗口/数学】困难](https://memcpy0.blog.csdn.net/article/details/121986872)

| [1646. Get Maximum in Generated Array](https://leetcode-cn.com/problems/get-maximum-in-generated-array/) | [1646. Get Maximum in Generated Array【数组/模拟/打表】简单](https://memcpy0.blog.csdn.net/article/details/119879925)
| [1816. Truncate Sentence](https://leetcode-cn.com/problems/truncate-sentence/)  |  [1816. Truncate Sentence【字符串】简单](https://memcpy0.blog.csdn.net/article/details/121747086)
| [2006. Count Number of Pairs With Absolute Difference K](https://leetcode-cn.com/problems/count-number-of-pairs-with-absolute-difference-k/) | [2006. Count Number of Pairs With Absolute Difference K【数组/哈希表】简单](https://memcpy0.blog.csdn.net/article/details/120475001)


| [2011. Final Value of Variable After Performing Operations](https://leetcode-cn.com/problems/final-value-of-variable-after-performing-operations/)  | [2011. Final Value of Variable After Performing Operations【模拟】简单](https://memcpy0.blog.csdn.net/article/details/120476304)
|  [2012. Sum of Beauty in the Array](https://leetcode-cn.com/problems/sum-of-beauty-in-the-array/) | [2012. Sum of Beauty in the Array【数组/递推】中等](https://memcpy0.blog.csdn.net/article/details/120476571)
| [2013. Detect Squares](https://leetcode-cn.com/problems/detect-squares/)   | [2013. Detect Squares【哈希表/设计/计数】中等](https://memcpy0.blog.csdn.net/article/details/120478693)
| [2014. Longest Subsequence Repeated k Times](https://leetcode-cn.com/problems/longest-subsequence-repeated-k-times/)  | [2014. Longest Subsequence Repeated k Times【BFS/字符串】困难](https://memcpy0.blog.csdn.net/article/details/120557243)

| [2016. Maximum Difference Between Increasing Elements](https://leetcode-cn.com/problems/maximum-difference-between-increasing-elements/)  | [2016. Maximum Difference Between Increasing Elements【数组】简单](https://memcpy0.blog.csdn.net/article/details/120559756)
|  [2017. Grid Game](https://leetcode-cn.com/problems/grid-game/) | [2017. Grid Game【前缀和/数组】中等](https://memcpy0.blog.csdn.net/article/details/120559913)
|  [2018. Check if Word Can Be Placed In Crossword](https://leetcode-cn.com/problems/check-if-word-can-be-placed-in-crossword/) |  [2018. Check if Word Can Be Placed In Crossword【数组/枚举/DFS】中等](https://memcpy0.blog.csdn.net/article/details/120559958)
| [2019. The Score of Students Solving Math Expression](https://leetcode-cn.com/problems/the-score-of-students-solving-math-expression/)  | [2019. The Score of Students Solving Math Expression【栈/记忆化/数学/动态规划/字符串】困难]()
|  [2039. The Time When the Network Becomes Idle](https://leetcode-cn.com/problems/the-time-when-the-network-becomes-idle/) | [2039. The Time When the Network Becomes Idle【图/BFS】中等](https://memcpy0.blog.csdn.net/article/details/123624797)

| [剑指 Offer II 069. 山峰数组的顶部](https://leetcode-cn.com/problems/B1IidL/) | [剑指 Offer II 069. 山峰数组的顶部【二分/三分/数组】简单](https://memcpy0.blog.csdn.net/article/details/120792269)
| [剑指 Offer 10- I. 斐波那契数列](https://leetcode-cn.com/problems/fei-bo-na-qi-shu-lie-lcof/) |    |
| [剑指 Offer 22. 链表中倒数第k个节点](https://leetcode-cn.com/problems/lian-biao-zhong-dao-shu-di-kge-jie-dian-lcof/) | [剑指 Offer 22. 链表中倒数第k个节点【链表/双指针】简单](https://memcpy0.blog.csdn.net/article/details/109687213)
| [面试题 17.14. Smallest K LCCI](https://leetcode-cn.com/problems/smallest-k-lcci/) | [面试题 17.14. Smallest K LCCI【堆/排序/分治】中等](https://memcpy0.blog.csdn.net/article/details/120108542)

……


---
# 2. 建立和改造GitHub仓库
本节将记录我对相关仓库的建立和日后的改造工作。先创建一个新的空仓库：
![在这里插入图片描述](https://img-blog.csdnimg.cn/fceaae8c7af441ee9b4634dbfbb9c2fa.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
由于是空仓库，所以什么都没有，白茫茫一片真干净：
![](https://img-blog.csdnimg.cn/9ed2357e817f439d81e692b594b8d938.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)  

接着我在个人代码库 `CodeWorld` 中的分类文件夹 `Mix_Works` 下建立项目 `LeetCode` 。原因是，考虑到将会用多种编程语言实现题解，要自定义项目文件夹结构，即需要书写自己的 `settings.json, launch.json, tasks.json` 文件，就在 `Mix_Works` 这个分类文件夹下工作。目前，我预想的项目文件夹结构如下，不排除日后大修的可能：
```cpp
LeetCode
├─.vscode
    ├─settings.json
    ├─launch.json
    └─tasks.json
├─Solutions
    ├─1-10
    ├─2-20
    ├─...
└─Templates
	├─...
```
其中，`.vscode` 是为了方便VS Code调试运行使用的，`Solutions` 以每十道题为一个文件夹存储题解代码文件，`Templates` 则会总结通用题解模板。接着提交整个本地仓库到远程，中途报错了：

```bash
$ git push -u origin main
remote: Support for password authentication was removed on August 13, 2021. Please use a personal access token instead.
remote: Please see https://github.blog/2020-12-15-token-authentication-requirements-for-git-operations/ for more information.
fatal: unable to access 'https://github.com/memcpy0/LeetCode-Conquest.git/': The requested URL returned error: 403
```
好像要给GitHub添加 `PAT (Personal Access Token)` ，具体来说就是：
- 在GitHub中，按照 `Settings => Developer Settings => Personal Access Token => Generate New Token (Give your password) => Fillup the form => click Generate token => Copy the generated Token` ，结果是生成一串字符如 `ghp_sFhFsSHhTzMDreGRLjmks4Tzuzgthdvfsrta` ；
- 在Windows上，要打开凭据管理器 `Credential Manager` ，它被用来存储凭据，例如网站登录和主机远程连接的用户名、密码。如果用户选择存储凭据，那么当用户再次使用对应的操作时，系统会自行填入凭据、实现自动登录。和Firefox的密码管理器差不多：
![在这里插入图片描述](https://img-blog.csdnimg.cn/a008500689bd46b7ad19cec56a4906a6.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
	进入Windows凭据 `Windows Credentials` ，找到 `git:https://github.com` 并进行编辑，替换密码为GitHub的PAT。

接着就可以推送了，之后的GitHub仓库页面如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/11059dd3ecef4d55b9109b52f6de7e5e.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)尴尬，发现有两个文件夹没有提交，啊不对，原来是Git不支持空文件夹的提交，不过可以在想要提交的空目录新建一个以 `.` 开头的隐藏文件，这样就不是空文件夹了，文件名约定俗成的是 `.gitkeep` 。具体来说，步骤如下：
- `vim .gitkeep` 创建.gitkeep文件，内容如下
	```bash
	# Ignore everything in this directory 
	* 
	# Except this file !.gitkeep 
	```
- 回到上级目录，运行如下命令：
	```bash
	git add .
	git commit -m 'Commit empty directories'
	git push origin main
	```

好了，现在的GitHub页面如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/f5582ae2bc634a55a14311c808b92cae.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
工作还远没有结束，还要写项目的Wiki……这也是件麻烦事，慢慢…慢慢来吧。

……





