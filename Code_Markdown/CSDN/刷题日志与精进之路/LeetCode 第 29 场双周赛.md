@[toc]

# 1491. Average Salary Excluding the Minimum and Maximum Salary
## 1. 题目

给你一个整数数组 `salary` ，数组里每个数都是 **唯一** 的，其中 `salary[i]` 是第 `i` 个员工的工资。

请你返回去掉最低工资和最高工资以后，剩下员工工资的平均值。

 示例 1：
```cpp
输入：salary = [4000,3000,1000,2000]
输出：2500.00000
解释：最低工资和最高工资分别是 1000 和 4000 。
去掉最低工资和最高工资以后的平均工资是 (2000+3000)/2= 2500
```

示例 2：
```cpp
输入：salary = [1000,2000,3000]
输出：2000.00000
解释：最低工资和最高工资分别是 1000 和 3000 。
去掉最低工资和最高工资以后的平均工资是 (2000)/1= 2000
```

示例 3：

```cpp
输入：salary = [6000,5000,4000,3000,2000,1000]
输出：3500.00000
```

示例 4：

```cpp
输入：salary = [8000,9000,2000,3000,6000,1000]
输出：4750.00000
```

 

提示：
-   `3 <= salary.length <= 100`
-    `10^3 <= salary[i] <= 10^6`
-    `salary[i]` 是唯一的。
-    与真实值误差在 `10^-5` 以内的结果都将视为正确答案。

## 2. 思路和代码
暴力过就可以了。这里可以排序，或者直接比较出最大值和最小值。
```cpp
class Solution {
public:
    double average(vector<int>& salary) {
        sort(salary.begin(), salary.end());
        double aver = 0;
        for (int i = 1; i < salary.size() - 1; ++i) aver += salary[i];
        return aver / (salary.size() - 2);
    }
    //直接比较出最大值
    double average(vector<int>& salary) {
        long double s = 0;
        int n = salary.size() - 2, ma = 0, mi = 1 << 20;
        for(auto c : salary) {
            ma = max(ma, c);
            mi = min(mi, c);
            s += c;
        }
        return (double)((s - ma - mi) / n);
    } 
};
```

---

# 1492. n 的第 k 个因子
## 1. 题目
给你两个正整数 `n` 和 `k` 。如果正整数 `i` 满足 `n % i == 0` ，那么我们就说正整数 i 是整数 `n` 的因子。

考虑整数 `n` 的所有因子，将它们**升序**排列 。请你返回第 `k ` 个因子。如果 `n` 的因子数少于 `k` ，请你返回 `-1` 。

示例 1：
```cpp
输入：n = 12, k = 3
输出：3
解释：因子列表包括 [1, 2, 3, 4, 6, 12]，第 3 个因子是 3 。
```
示例 2：
```cpp
输入：n = 7, k = 2
输出：7
解释：因子列表包括 [1, 7] ，第 2 个因子是 7 。
```
示例 3：
```cpp
输入：n = 4, k = 4
输出：-1
解释：因子列表包括 [1, 2, 4] ，只有 3 个因子，所以我们应该返回 -1 。
```
示例 4：
```cpp
输入：n = 1, k = 1
输出：1
解释：因子列表包括 [1] ，第 1 个因子为 1 。
```
示例 5：
```cpp
输入：n = 1000, k = 3
输出：4
解释：因子列表包括 [1, 2, 4, 5, 8, 10, 20, 25, 40, 50, 100, 125, 200, 250, 500, 1000] 。
```
提示：`1 <= k <= n <= 1000`

## 2. 思路和代码
直接计算并存储所有 `n` 的因子，然后看第 `k` 个因子是哪个。
```cpp
class Solution {
public:
    int kthFactor(int n, int k) {
        vector<int> vi; 
        for (int i = 1; i <= n; ++i) if (n % i == 0) vi.push_back(i);
        if (k <= vi.size()) return vi[k - 1];
        return -1;
    }
};
```
或者，边计算边查找：
```cpp
class Solution {
public:
	int kthFactor(int n, int k) {
		for (int i = 1; i <= n; ++i) {
			if (n % i == 0) {
				--k;
				if (!k) return i;
			}
		}
		return - 1;
	}
};
```

---

# 1493. Longest Subarray of 1's After Deleting One Element
## 1. 题目
Given a binary array nums, you should **delete one element** from it.

Return the `size` of the **longest non-empty subarray** containing only `1's` in the resulting array. Return `0` if there is no such subarray.
 
Example 1:
```cpp
Input: nums = [1,1,0,1]
Output: 3
Explanation: After deleting the number in position 2, [1,1,1] contains 3 numbers with value of 1's.
```

Example 2:
```cpp
Input: nums = [0,1,1,1,0,1,1,0,1]
Output: 5
Explanation: After deleting the number in position 4, [0,1,1,1,1,1,0,1] longest subarray with value of 1's is [1,1,1,1,1].
```

Example 3:
```cpp
Input: nums = [1,1,1]
Output: 2
Explanation: You must delete one element.
```

Example 4:
```cpp
Input: nums = [1,1,0,0,1,1,1,0,1]
Output: 4
```

Example 5:
```cpp
Input: nums = [0,0,0]
Output: 0
```
Constraints:
-   `1 <= nums.length <= 10^5`
-   `nums[i]` is either `0 or 1`.

## 2. 思路
这道题目有点麻烦。需要注意的是，这不是一道动态规划题目。而是一道【滑动窗口】型题目。我们保持一个滑动的窗口，其中最多容许一个 `0` 的存在。

如果全部是 `1` ，则我们删除一个 `1` ；如果存在 `0` ，则必须删除 `0` ：
- 初始化变量 `nowLen = preLen = 0, maxLen = 0` ；
- 我们扫描过一个连续 `1` 区间，同时计算其长度为 `nowLen` ，并比较保存 `maxLen = max(maxLen, nowLen + preLen)` ；遇到一个 `0` 时比较保存一下前面 `1` 区间的长度 `preLen = nowLen` ，`nowLen = 0` ；
- 如果接下来遇到的是 `1` 区间，**继续上一步**，计算其长度为 `nowLen` ，并比较保存 `maxLen = max(maxLen, nowLen + preLen)` ，相当于删除中间的一个 `0` 合并两个连续的 `1` 区间；
- 如果遇到的又是 `0` ，即遇到连续多个 `0` ，则保存的上个 `1` 连续区间长度 `preLen = 0` 清零。
## 3. 代码
这应该是最简单的代码：
```cpp
class Solution {
public:
    int longestSubarray(vector<int>& nums) {
        int maxLen = 0, preLen = 0, nowLen = 0;
        for (const auto &i : nums) {
            if (i) ++nowLen; //i非零
            else {           //i为零
                preLen = nowLen; //保存上一个连续1区间的长度
                nowLen = 0;      //计算下一个连续1区间的长度
            }
            maxLen = max(maxLen, preLen + nowLen);
        }
        if (maxLen == nums.size()) return maxLen - 1; //全1则删除1
        return maxLen;
    }
};
```
效率：
```cpp
执行用时：108 ms, 在所有 C++ 提交中击败了67.20% 的用户
内存消耗：36.5 MB, 在所有 C++ 提交中击败了100.00% 的用户
```

---
# 1494. Parallel Courses II
## 1. 题意

Given the integer n representing the number of courses at some university labeled from 1 to n, and the array dependencies where dependencies[i] = [xi, yi]  represents a prerequisite relationship, that is, the course xi must be taken before the course yi.  Also, you are given the integer k.

In one semester you can take at most k courses as long as you have taken all the prerequisites for the courses you are taking.

Return the minimum number of semesters to take all courses. It is guaranteed that you can take all courses in some way.

 

Example 1:

Input: n = 4, dependencies = [[2,1],[3,1],[1,4]], k = 2
Output: 3 
Explanation: The figure above represents the given graph. In this case we can take courses 2 and 3 in the first semester, then take course 1 in the second semester and finally take course 4 in the third semester.

Example 2:

Input: n = 5, dependencies = [[2,1],[3,1],[4,1],[1,5]], k = 2
Output: 4 
Explanation: The figure above represents the given graph. In this case one optimal way to take all courses is: take courses 2 and 3 in the first semester and take course 4 in the second semester, then take course 1 in the third semester and finally take course 5 in the fourth semester.

Example 3:

Input: n = 11, dependencies = [], k = 2
Output: 6

 

Constraints:

    1 <= n <= 15
    1 <= k <= n
    0 <= dependencies.length <= n * (n-1) / 2
    dependencies[i].length == 2
    1 <= xi, yi <= n
    xi != yi
    All prerequisite relationships are distinct, that is, dependencies[i] != dependencies[j].
    The given graph is a directed acyclic graph.


```cpp
#include <bit/stdc++.h>
using namespace std;

 
class Solution {
    int a[15],f[32768],o[32768];
public:
    int minNumberOfSemesters(int n, vector<vector<int>>& dependencies, int k) {
        memset(a,0,sizeof(a));
        int i,j,l;
        for(auto e:dependencies)a[e[1]-1]|=1<<e[0]-1;
        for(i=1;i<1<<n;i++)o[i]=o[i>>1]+(i&1);
        memset(f,127,sizeof(f));
        for(i=f[0]=0;i<1<<n;i++)if(f[i]<=n)
        {
            for(j=l=0;j<n;j++)if(!(i>>j&1)&&(a[j]&i)==a[j])l|=1<<j;
            for(j=l;j;j=j-1&l)if(o[j]<=k)f[i|j]=min(f[i|j],f[i]+1);
        }
        return f[(1<<n)-1];
    }
};
```



