> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>Alice has <code>n</code> candies, where the <code>i<sup>th</sup></code> candy is of type <code>candyType[i]</code>. Alice noticed that she started to gain weight, so she visited a doctor.</p>

<p>The doctor advised Alice to only eat <code>n / 2</code> of the candies she has (<code>n</code> is always even). Alice likes her candies very much, and she wants to eat the maximum number of different types of candies while still following the doctor's advice.</p>

<p>Given the integer array <code>candyType</code> of length <code>n</code>, return <em>the <strong>maximum</strong> number of different types of candies she can eat if she only eats </em><code>n / 2</code><em> of them</em>.</p>
 
<p><strong>Example 1:</strong></p>

```cpp
Input: candyType = [1,1,2,2,3,3]
Output: 3
Explanation: Alice can only eat 6 / 2 = 3 candies. Since there are only 3 types, she can eat one of each type.
```

<p><strong>Example 2:</strong></p>

```cpp
Input: candyType = [1,1,2,3]
Output: 2
Explanation: Alice can only eat 4 / 2 = 2 candies. Whether she eats types [1,2], [1,3], or [2,3], she still can only eat 2 different types.
```
<p><strong>Example 3:</strong></p>

```cpp
Input: candyType = [6,6,6,6]
Output: 1
Explanation: Alice can only eat 4 / 2 = 2 candies. Even though she can eat 2 candies, she only has 1 type.
```
 <p><strong>Constraints:</strong></p>
<ul>
	<li><code>n == candyType.length</code></li>
	<li><code>2 &lt;= n &lt;= 10<sup>4</sup></code></li>
	<li><code>n</code>&nbsp;is even.</li>
	<li><code>-10<sup>5</sup> &lt;= candyType[i] &lt;= 10<sup>5</sup></code></li>
</ul>


题意：给你一个长度为 `n` 的整数数组 `candyType` ，返回Alice在仅吃掉 `n / 2` 枚糖的情况下，可以吃到糖的最多种类数。

---
### 解法1 哈希表+贪心
用 `unordered_set` 对糖果的种类进行去重，然后把这些种类的糖果尽可能分配给Alice，直到分给Alice $\dfrac{n}{2}$ 颗糖果或者没有更多种类为止。代码如下，时间复杂度为 $O(n)$ ，空间复杂度为 $O(n)$ ：
```cpp
//C++ version
class Solution {
public:
    int distributeCandies(vector<int>& candies) {
        int n = candies.size(), sisterCount = n / 2, ans = 0;
        unordered_set<int> record;
        for (int i = 0; i < n; ++i) record.insert(candies[i]);
        for (const auto &it : record) if (sisterCount-- > 0) ++ans;
        return ans; 
    }
};
//执行用时：624 ms, 在所有 C++ 提交中击败了53.61% 的用户
//内存消耗：112.8 MB, 在所有 C++ 提交中击败了37.95% 的用户
```
---
### 解法2 贪心（优化）+证明
由于题目规定糖果的数量即 `candyType[]` 的长度为偶数，因此一定能将糖果平均分成两份。假设糖果的数量为 $n$ 颗，则每份糖果的数量固定为 $\dfrac {n} {2}$ 颗。又假设「**糖果种类数量**」为 $m$ 种，那么单份中可使得「**糖果种类数量**」最大为 $\min(m, \dfrac{n}{2})$ ，这也是解法一展示出的朴素想法。我们可以「**分情况讨论**」来进行证明：
- $m > \dfrac{n}{2}$ ：糖果种类数量大于单份糖果数量。此时可从 $m$ 类糖果中找出 $\dfrac{n}{2}$ 类不同的糖果组成单份，**可取得的「最大糖果种类数量」受到「单份糖果数量」的限制**，即为 $\dfrac{n}{2}$ 种。
- $m = \dfrac{n}{2}$ ：糖果种类数量等于单份糖果数量。此时可从 $m$ 类糖果中找出 $\dfrac{n}{2}$ 类不同的糖果组成单份，可取得的「最大糖果种类数量」为 $m = \dfrac{n}{2}$ 种。
- $m < \dfrac{n}{2}$ ：糖果种类数量小于单份糖果数量，我们可以先从 $m$ 类糖果中找出 $m$ 类不同的糖果组成单份，再使用相同种类的糖果凑齐 $\dfrac{n}{2}$ 颗（根据**鸽巢原理**，这 $\dfrac{n}{2}$ 颗糖果中有相同种类的糖果）。此时**可取得的「最大糖果种类数量」受到「糖果种类数量」的限制**，为 $m$ 种。

前两种情况下，Alice得到的 $\dfrac{n}{2}$ 枚糖果都是不同的，但Alice无法得到所有种类的糖果；最后一种情况下，Alice最多得到 $m$ 颗糖果，但是她可以尝到所有种类的糖果。

```cpp
//C++ version
class Solution {
public:
    int distributeCandies(vector<int>& candyType) {
        unordered_set<int> rec(candyType.begin(), candyType.end());
        return min(candyType.size() / 2, rec.size());
    }
};
//执行用时：224 ms, 在所有 C++ 提交中击败了57.22% 的用户
//内存消耗：107.8 MB, 在所有 C++ 提交中击败了59.71% 的用户
```
由于 `candyType[i]` 的范围已知，我们可以使用 `vector<bool>` 或 `bitset` 来提高效率。如下所示，效率比起上面几乎翻倍：
```cpp
//C++ version
class Solution {
public:
    int distributeCandies(vector<int>& candies) {
        int n = candies.size(), size = 0;  
        vector<bool> bucket(200010);
        for (int i = 0; i < n; ++i) {
            if (!bucket[candies[i] + 100000]) {
                ++size;
                bucket[candies[i] + 100000] = true;
            }
        }
        return min(n / 2, size);
    }
};
//执行用时：120 ms, 在所有 C++ 提交中击败了97.23% 的用户
//内存消耗：86.1 MB, 在所有 C++ 提交中击败了75.68% 的用户
```
如果使用 `bitset` 呢？代码如下：
```cpp
//C++ version
class Solution {
public:
    int distributeCandies(vector<int>& candies) {
        int n = candies.size(), size = 0;  
        bitset<200010> bucket;
        for (int i = 0; i < n; ++i) {
            if (!bucket[candies[i] + 100000]) {
                ++size;
                bucket[candies[i] + 100000] = 1;
            }
        }
        return min(n / 2, size);
    }
};
//执行用时：120 ms, 在所有 C++ 提交中击败了97.23% 的用户
//内存消耗：79.8 MB, 在所有 C++ 提交中击败了81.38% 的用户
```
