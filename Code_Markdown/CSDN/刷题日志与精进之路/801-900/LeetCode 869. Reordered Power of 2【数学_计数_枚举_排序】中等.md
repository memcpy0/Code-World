> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>You are given an integer <code>n</code>. We reorder the digits in any order (including the original order) such that the leading digit is not zero.</p>

<p>Return <code>true</code> <em>if and only if we can do this so that the resulting number is a power of two</em>.</p>

 
<p><strong>Example 1:</strong></p>

```bash
Input: n = 1
Output: true
```
<p><strong>Example 2:</strong></p>

```bash
Input: n = 10
Output: false
```

<p><strong>Example 3:</strong></p>

```bash
Input: n = 16
Output: true 
```

<p><strong>Example 4:</strong></p>

```bash
Input: n = 24
Output: false
```

<p><strong>Example 5:</strong></p>

```bash
Input: n = 46
Output: true
```

 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= n &lt;= 10<sup>9</sup></code></li>
</ul>

题意：给定正整数 `N` ，我们按任何顺序（包括原始顺序）将数字重新排序，注意其前导数字不能为零。如果我们可以通过上述方式得到 `2` 的幂，返回 `true` ；否则，返回 `false` 。

---
### 解法1 词频统计+暴力DFS+剪枝+位操作
我们对 $n$ 中的数位进行计数统计，然后暴力DFS重排出所有的可能，并判断是否为 $2$ 的幂。由于正整数 $n$ 的排列可能较多，暴力循环检查最多为 $10^{10}$ 复杂度，我们需要**剪枝**——当前数位的个数已经被用完时，跳过并检查下一个数位的个数；如果当前遍历的数位不为零、或当前数不为零（即**前导数字不能为零**），则进行DFS；如果DFS返回的值为 `true` ，则结束DFS。

粗略分析一下时空复杂度。处理出 `cnts` 数组的复杂度为 $O(\log n)$ ，DFS的复杂度为 $O((\log n)!)$ ，整体复杂度为 $O((\log n)!)$ 。不计算递归用到的空间，空间复杂度为 $O(1)$ 。
```cpp
//C++ version
class Solution {
private:
    int m = 0, cnts[10] = {0}; // m:n的位数
    bool dfs(int k, int cur) {
        if (k >= m) return !(cur & (cur - 1)); // 判断是否为2的幂
        for (int i = 0; i < 10; ++i) {
            if (cnts[i]) {
                --cnts[i];
                if ((i || cur) && dfs(k + 1, cur * 10 + i)) // 前导数字不为0
                    return true;
                ++cnts[i];
            }
        }
        return false;
    }
public:
    bool reorderedPowerOf2(int n) {
        while (n) {
            ++cnts[n % 10]; // 数位统计
            n /= 10;
            ++m; // 计算n的位数
        }
        return dfs(0, 0);
    }
};
//执行用时：28 ms, 在所有 C++ 提交中击败了25.56% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了79.10% 的用户
```

---
### 解法2 枚举2的幂+词频统计
我们发现解法1中，复杂度上界取决于对 $n$ 的暴力DFS。另外，数据范围内 $2$ 的幂数量很少。因此有效降低复杂度的方法是，直接枚举所有 $2$ 的幂 $x$ ，并检查 $x$ 的数位词频是否与 $n$ 相同。

以下代码中，统计 $2$ 的幂以及 $n$ 的数位词频、比较二者词频是否相同，复杂度均为 $O(C \log n)$ ，$C$ 约为常数 $30$ ，因此整体时间复杂度为 $O(C\log n)$ 。空间复杂度为 $O(C)$ ，$C$ 约为常数 $20$ 。
```cpp
//C++ version
class Solution {
private:
    int cnts[10] = {0}, xcnts[10] = {0}; // m:n的位数
public:
    bool reorderedPowerOf2(int n) {
        while (n) {
            ++cnts[n % 10]; // 数位统计
            n /= 10;
        }
        for (int i = 1; i < 1e9; i <<= 1) { // 遍历数据范围内2的幂
            memset(xcnts, 0, sizeof(xcnts));
            int x = i; 
            while (x) { // 对2的幂进行数位词频统计
                ++xcnts[x % 10];
                x /= 10;
            }
            bool flag = true;
            for (int j = 0; j < 10; ++j) {
                if (cnts[j] != xcnts[j]) { 
                    flag = false;
                    break;
                }
            }
            if (flag) return true;
        }
        return false;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.7 MB, 在所有 C++ 提交中击败了97.27% 的用户
```
---
### 解法3 枚举+排序+打表
我注意到评论区中有的解法进行了打表，只是吧，有时候打表也是有技巧的，比如暴力DFS重排后，检查是否为 $2$ 的幂可用位操作 $O(1)$ 解决，此时「预先打表以加速检查」就毫无意义。关于本题，最高效的打表做法是，枚举所有 $2$ 的幂并转换为字符串，字符串内部对字符从小到大排序，所有这些字符串形成我们的表。然后将数字 $n$ 转换为字符串，同样排序内部字符，并一一与表中字符串进行比较。如有匹配则返回 `true` ，否则返回 `false` 。

以下代码中，遍历表并一一比较字符串所需的时间复杂度为 $O(C\log n)$ 。由于要存储这些数的字符串形式，会多用一点空间，不过空间复杂度不会超过 $O(C\log n)$ ，$C$ 约为常数 $30$ 。
```java
//Java versioin
class Solution {
    public boolean reorderedPowerOf2(int n) {
        String[] rec = {"1", "2", "4", "8", "16", "23", "46", "128", "256", "125", "0124", "0248", "0469", "1289", "13468", "23678", "35566", "011237", "122446", "224588", "0145678", "0122579", "0134449", "0368888", "11266777", "23334455", "01466788", "112234778", "234455668", "012356789"};
        char[] tn = String.valueOf(n).toCharArray();
        Arrays.sort(tn);
        String str = new String(tn);
        for (String p : rec)
            if (str.equals(p)) return true;
        return false;
    }
}
//执行用时：1 ms, 在所有 Java 提交中击败了97.34% 的用户
//内存消耗：35.2 MB, 在所有 Java 提交中击败了84.04% 的用户
```

