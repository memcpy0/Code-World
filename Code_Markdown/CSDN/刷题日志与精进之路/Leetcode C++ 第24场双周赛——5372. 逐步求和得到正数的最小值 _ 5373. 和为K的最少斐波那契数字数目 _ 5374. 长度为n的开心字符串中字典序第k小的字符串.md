@[toc]
# 5372. 逐步求和得到正数的最小值
给你一个整数数组 `nums` 。你可以选定任意的 **正数** `startValue` 作为初始值。你需要从左到右遍历 `nums` 数组，并将 `startValue` 依次累加上 `nums` 数组中的值。
请你在确保**累加和始终大于等于 $1$** 的前提下，选出一个**最小的** `正数` 作为 `startValue` 。

 示例1：
```
输入：nums = [-3,2,-3,4,2]
输出：5
解释：如果你选择 startValue = 4，在第三次累加时，和小于 1 。
                累加求和
                startValue = 4 | startValue = 5 | nums
                  (4 -3 ) = 1  | (5 -3 ) = 2    |  -3
                  (1 +2 ) = 3  | (2 +2 ) = 4    |   2
                  (3 -3 ) = 0  | (4 -3 ) = 1    |  -3
                  (0 +4 ) = 4  | (1 +4 ) = 5    |   4
                  (4 +2 ) = 6  | (5 +2 ) = 7    |   2
```
示例 2：
```
输入：nums = [1,2]
输出：1
解释：最小的 startValue 需要是正数。
```
示例 3：
```
输入：nums = [1,-2,-3]
输出：5
```
 
提示：

-    `1 <= nums.length <= 100`
 -   `-100 <= nums[i] <= 100`

## (1) 思路
要找到一个最小的正整数，使得按顺序累加数组中的数时，每一步的结果都 $\ge 1$。很简单的一道题，贪心，从1开始，不断尝试，直到满足题意就退出。

尝试一个 `startValue` 时，如果在累计的中途发现不满足条件，可以提前终止，`++startValue`，尝试下一个正数。
## (2) 我的代码
```cpp
class Solution {
public:
    int minStartValue(vector<int>& nums) {
        int startValue = 1, sum;
        while (true) {
            sum = startValue;
            bool flag = false;
            for (auto v : nums) {
                sum += v; 
                if (sum < 1) {
                    flag = true;
                    break;
                }
            }
            if (!flag) break;
            ++startValue;  
        }
        return startValue;
    }
};
```
## (3) 更好的代码
从 $1$ 开始尝试，太低效了，如果要求的结果很大，整个算法就会尝试很多次。为此，我们需要更高效的算法。下面的做法可以达到 $O(n)$ 的时间复杂度。
```
sum = 0, ans = 0
从-3开始累加：
2:										 2											
1:										 |
      -3		2		-3		4		 2
0 :    |        |        |      0        
-1:    |       -1        |
-2:    |                 |
-3:   -3                 |
-4:                     -4
```
显然，为了让累加的过程中 `sum` 的值始终大于等于 $1$ ，我们需要有 $max\Big(-(-3), -(-1), -(-4)\Big) + 1 = 4 + 1 = 5$ 。代码如下：
```cpp
class Solution {
public:
    int minStartValue(vector<int>& nums) {
        int u = 0, s = 0;
        for(int i=0; i < nums.size(); ++i)
        {
            u += nums[i];
            s = max(s, -u);
        }
        return s + 1;
    }
};
```

---
# 5373. 和为 K 的最少斐波那契数字数目 
 给你数字 $k$ ，请你返回和为 $k$ 的斐波那契数字的**最少数目**，其中，每个斐波那契数字都可以被使用**多次**。

斐波那契数字定义为：
```
F1 = 1
F2 = 1
Fn = Fn-1 + Fn-2 ， 其中 n > 2 。
```
数据保证对于给定的 $k$ ，**一定能找到可行解**。
 示例 1：
```
输入：k = 7
输出：2 
解释：斐波那契数字为：1，1，2，3，5，8，13，……
对于 k = 7 ，我们可以得到 2 + 5 = 7 。
```
示例 2：
```
输入：k = 10
输出：2 
解释：对于 k = 10 ，我们可以得到 2 + 8 = 10 。
```
示例 3：
```
输入：k = 19
输出：3 
解释：对于 k = 19 ，我们可以得到 1 + 5 + 13 = 19 。
```
提示： 
-    `1 <= k <= 10^9`

## (1) 思路
对于一个由斐波拉契数组成的数列，从中找到最少的数，其和等于给出的 $k$ 。

可以很容易地想到，先找出小于等于 $k$ 的全部斐波拉契数 $f_i$，然后从大到小枚举，如果 $k - f_i \ge 0$，$k$ 就不断减去 $f_i$，同时要用的数的数目 $+1$ 。
## (2) 我的代码

```cpp
class Solution {
public:
    vector<int> fibo;   
    int findMinFibonacciNumbers(int k) {
        fibo.push_back(1);
        fibo.push_back(1);   
        while (fibo.back() <= k)  
            fibo.push_back(fibo.back() + fibo[fibo.size() - 2]); 
        int ans = 0;
        for (int i = fibo.size() - 1; i >= 0; --i) {
            while (k - fibo[i] >= 0) {
                ++ans;
                k -= fibo[i];
            } 
        }
        return ans;
    }
};
```
---
# 5374. 长度为n的开心字符串中字典序第k小的字符串(DFS)
一个 「开心字符串」定义为：

    仅包含小写字母 ['a', 'b', 'c'].
    对所有在 1 到 s.length - 1 之间的 i ，满足 s[i] != s[i + 1] （字符串的下标从 1 开始）。

比方说，字符串 `"abc"`，`"ac"`，`"b"` 和 `"abcbabcbcb"` 都是开心字符串，但是 `"aa"`，`"baa"` 和 `"ababbc"` 都不是开心字符串。

给你两个整数 $n$ 和 $k$ ，你需要将长度为 $n$ 的所有开心字符串**按字典序排序**。

请你返回排序后的第 $k$ 个开心字符串，如果长度为 n 的开心字符串少于 $k$ 个，那么请你返回 **空字符串** 。

示例 1：
```
输入：n = 1, k = 3
输出："c"
解释：列表 ["a", "b", "c"] 包含了所有长度为 1 的开心字符串。按照字典序排序后第三个字符串为 "c" 。
```
示例 2：
```
输入：n = 1, k = 4
输出：""
解释：长度为 1 的开心字符串只有 3 个。
```
示例 3：
```
输入：n = 3, k = 9
输出："cab"
解释：长度为 3 的开心字符串总共有 12 个 ["aba", "abc", "aca", "acb", "bab", "bac", "bca", "bcb", "cab", "cac", "cba", "cbc"] 。第 9 个字符串为 "cab"
```
示例 4：
```
输入：n = 2, k = 7
输出：""
```
示例 5：
```
输入：n = 10, k = 100
输出："abacbabacb"
```
 

提示：

-    `1 <= n <= 10`
-    `1 <= k <= 100`

## (1) 思路
这个题要用 `a,b,c` 三个小写字母，按照字典序组成长度为 $n$ 的快乐字符串的序列，然后返回其中第 $k$ 个快乐字符串。

很简单的一道题，数据规模也不大，有点像**递归+剪枝**求出全排列(按照字典序)的写法，这种模板我写在了【搜索-排列组合】中。
## (2) 我的代码
```cpp
class Solution {
public:
    vector<string> dict;
    void dfs(string str, int level, int cur) {
        if (cur >= level) {
            dict.push_back(str); 
            return;
        }
        for (char ch = 'a'; ch <= 'c'; ++ch) {
            if (!str.empty() && str.back() == ch) continue;
            str.push_back(ch);
            dfs(str, level, cur + 1);
            str.pop_back();
        }
    }
        
    string getHappyString(int n, int k) {
        dfs(string(), n, 0); 
        if (dict.size() < k) return "";
        return dict[k - 1];
    }
};
```
## (3) 更好的代码
下面的代码可以到达 $O(n^2)$ 的时间复杂度。

1. $1$ 个字符可以形成 $3$ 种可能，$3$ 个字符可以形成 $12$ 种可能——$n$ 个字符形成快乐字符串，有 $3 * 2^{n-1}$ 个，我们要找到其中的第 $k$ 小的字符串。
2. 如果 $k > 2^{n-1} \cdot 3$，不可能存在，直接返回空串；
3. 如果 $k \le 2^{n-1} \cdot 3$，用最简单的例子：
	- $k \le 2^0 \cdot 3 = 3$，令 $k = 3$，要找第 $3$ 个长度为 $n=1$ 的快乐字符串，很显然，我们需要选择 $c$； 
	- $k <= 1$ ，说明是选择 $a$；
	- $k > 1, k = 2$，说明是选择 $b$；
	- $k > 2, k = 3$，说明是选择 $c$.

4. 选择第 $1$ 个字符时：
	- 如果 $k \le 1 \cdot 2^{n-1}$ ，说明要选择更小的字符，这样才使得字典序更小，因此第 $1$ 个字符选 $a$；
	-  $1\cdot 2^{n-1} \lt k \le 2 \cdot 2^{n-1}$ ，说明要选择**能够让 $k$ 属于中间一部分**的字符，因此第 $1$ 个字符选 $b$；
	- $2\cdot 2^{n-1} \lt k \le 3 \cdot 2^{n-1}$ ，说明要选择**能够让 $k$ 属于最大的那一部分**的字符，因此第 $1$ 个字符选择 $c$.

5. 选择第 $i, i \gt 1$ 个字符的时候，需要判断是否和前一个字符相等.

代码如下：
```cpp
class Solution {
public:
    int po[20];
    string getHappyString(int n, int k) {
        po[0]=1;
        for(int i = 1; i <= n; ++i) po[i] = po[i - 1] * 2;
        if(k > po[n-1] * 3) return ""; 
        string ans; 
        char r = 0;
        for(int i = 1; i <= n; ++i) {
            for(char j = 'a'; j <= 'c'; ++j) {
            	if (r != j) {
		            if(k > po[n - i]) k -= po[n-i];
	                else {
	                    ans.push_back(j);
	                    r = j;
	                    break;
	                } 
	            }
            }
        }
        return ans;
    }
};
```
## (4) 拓展
其实吧，这个题目和逆【康托展开】很像：
- 【康托展开】是求出一个排列的序为多少；
- 【逆康托展开】是根据一个序求出排列是多少。这里则是根据序 $k$ 求出相应的快乐字符串。

可以考虑出这么一个题，给出一个字符串，判断是否是快乐字符串，如果是，则求出这是第几小的快乐字符串，【简单】难度。
