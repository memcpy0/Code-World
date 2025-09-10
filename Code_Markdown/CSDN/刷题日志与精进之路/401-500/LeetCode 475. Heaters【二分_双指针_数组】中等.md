> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>Winter is coming! During the contest, your first job is to design a standard heater with a fixed warm radius to warm all the houses.</p>

<p>Every house can be warmed, as long as the house is within the heater's warm radius range. </p>

<p>Given the positions of <code>houses</code> and <code>heaters</code> on a horizontal line, return <em>the minimum radius standard of heaters so that those heaters could cover all houses.</em></p>

<p><strong>Notice</strong> that all the <code>heaters</code> follow your radius standard, and the warm radius will the same.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: houses = [1,2,3], heaters = [2]
Output: 1
Explanation: The only heater was placed in the position 2, and if we use the radius 1 standard, then all the houses can be warmed.
```
<p><strong>Example 2:</strong></p>

```cpp
Input: houses = [1,2,3,4], heaters = [1,4]
Output: 1
Explanation: The two heater was placed in the position 1 and 4. We need to use radius 1 standard, then all the houses can be warmed.
```
<p><strong>Example 3:</strong></p>

```cpp
Input: houses = [1,5], heaters = [2]
Output: 3
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= houses.length, heaters.length &lt;= 3 * 10<sup>4</sup></code></li>
	<li><code>1 &lt;= houses[i], heaters[i] &lt;= 10<sup>9</sup></code></li>
</ul>


题意：这道题有点复杂，说的是：冬季已经来临。任务是设计一个有固定加热半径的标准供暖器，以便向所有房屋供暖。

现在，给出位于一条水平线上的房屋和供暖器的位置，找到可以覆盖所有房屋的最小加热半径，输出供暖器所需要的最小加热半径。

---
### 解法1 二分（供暖器加热半径）+双指针
本题中，需要求出最小加热半径 `ans` ，覆盖所有的 `houses[i]` 。经过分析，**显然在以 `ans` 为分割点的数轴上具有「单调性」和「二段性」**：
- 最小加热半径 `ans` 越大，覆盖的数轴范围越大。
- 小于 `ans` 的加热半径，无法覆盖所有的房子；大于等于 `ans` 的加热半径，可以覆盖所有房子

因此，**我们可采用二分法查找答案**。考虑到房屋和供暖器的位置范围非负，且不超过 `10^9` ，可用 `[0, 1e9]` 作为二分的下界和上界。在这一范围内，进行最多 $30$ 来次判定，就可以确定需要的最小加热范围。

为了实现判定函数，先要对 `houses[]` 和 `heaters[]` 进行排序，然后使用 `i` 指向当前的房子 `houses[i]` ，`j` 指向**可能覆盖到** `houses[i]` 的最小位置 `heaters[j]` ，用 `mid` 代表当前要判定的最小加热半径。如果 `houses[i] >= heaters[j] - mid && houses[i] <= heaters[j] + mid` ，则说明 `houses[i]` 可被 `heaters[j]` 覆盖到，`i` 自增以继续处理下个房子；否则 `j` 自增，尝试判定下个供暖器能否覆盖 `houses[i]` ，直到找到合适的供暖器，或超出 `heaters[]` 的范围为止。最后，==如果所有房子都被供暖器覆盖，则说明加热半径 `mid` 符合要求，可尝试继续缩小加热半径，否则要扩展加热半径==。

时间复杂度：令 $m,n$ 分别为  `houses[]` 和 `heaters[]` 的长度，排序的复杂度为 $O(m\log m +n\log n)$ 。在 $[0, 1e9]$ 范围内二分，并使用双指针判定每个 `houses[i]` 是否被覆盖，复杂度为 $O(\min(n, m) \times \log L)\ (L = 1e9)$ 。整体复杂度为 $O(m\log m + n\log n + \min(n, m) \times \log L)$ 。
```cpp
//C++ version
class Solution {
public:
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        sort(houses.begin(), houses.end());
        sort(heaters.begin(), heaters.end());
        int m = houses.size(), n = heaters.size(), l = 0, r = 1e9; // 0~1e9的范围
        while (l < r) {
            int mid = l + r >> 1, i, j;
            for (i = 0, j = 0; i < m && j < n; ++j) // 遍历供暖器数组,判断多少房屋在供暖器范围内
                while (i < m && houses[i] >= heaters[j] - mid && houses[i] <= heaters[j] + mid) ++i;
            if (i >= m) r = mid; //如果全都在
            else l = mid + 1;
        }
        return l;
    }
}; 
//执行用时：96 ms, 在所有 C++ 提交中击败了11.19% 的用户
//内存消耗：24.6 MB, 在所有 C++ 提交中击败了75.64% 的用户
```
**为了提高效率，需要缩小搜索范围**，做法是对排序后的 `heaters.back(), houses.back()` 取其最大位置作为最大范围，不能取 `heaters.front(), houses.front()` 的最小位置，因为最小半径可能为 `0` ，比如供暖器和房屋位置相同时。代码如下：
```cpp
//C++ version
class Solution {
public:
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        sort(houses.begin(), houses.end());
        sort(heaters.begin(), heaters.end());
        int m = houses.size(), n = heaters.size(), l = 0, r = max(houses.back(), heaters.back());
        while (l < r) {
            int mid = l + r >> 1, i, j;
            for (i = 0, j = 0; i < m && j < n; ++j)
                while (i < m && houses[i] >= heaters[j] - mid && houses[i] <= heaters[j] + mid) ++i;
            if (i >= m) r = mid;
            else l = mid + 1;
        }
        return l;
    }
};
//执行用时：76 ms, 在所有 C++ 提交中击败了27.28% 的用户
//内存消耗：24.6 MB, 在所有 C++ 提交中击败了79.49% 的用户
```

---
### 解法2 二分查找 `houses[i]` 在有序 `heaters[]` 中的位置+极小化极大
还有办法更高效吗？有！我们只需要从小到大排序 `heaters[]` 数组，然后对 `houses[]` 中的每间房屋的位置 `houses[i]` ，在 `heaters[]` 数组中二分寻找第一个大于等于 `houses[i]` 的供暖器，从而得到房子右边最近的供暖器，如果房子左边也有供暖器，就**比较两者到房屋的距离而取较小值**（如果只有一边则直接取其与房子的距离），这就是**这间房屋所需要的加热半径**。==取所有房屋所需的最小加热半径的最大值，就是要求的结果==。

时间复杂度：令 $m,n$ 分别为  `houses[]` 和 `heaters[]` 的长度，排序的复杂度为 $O(n\log n)$ 。遍历每间房屋，在 $[0, n]\ (n \le 3\times 10^4)$ 范围内二分查找 `heaters` 数组，复杂度为 $O(m \log n)$ 。整体复杂度为 $O(n\log n + m \log n)$ 。
```cpp
//C++ version
class Solution {
public:
    // 对于每座房子的前后两个供暖器(只一个供暖器时另外判断),
    // 寻找供暖器离房屋的较小距离,即这座房子需要的最小加热半径
    // 在这一过程中,不断更新当前最大加热半径 
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        sort(heaters.begin(), heaters.end());
        int m = houses.size(), ans = 0;
        for (int i = 0; i < m; ++i) {
            int needRadius = INT_MAX; // houses[i]需要的最小供热半径
            // 找到heaters[]中第一个>=houses[i]的供暖器位置
            // 即右侧离这座房子最近的供暖器
            auto &&right = lower_bound(heaters.begin(), heaters.end(), houses[i]);
            if (right != heaters.end())          // 说明房子右侧有供暖器
                needRadius = *right - houses[i]; // 它们之间的距离
            if (right != heaters.begin()) { // 说明房子左侧有供暖器
                auto &&left = right - 1;    // 左边供暖器的位置就是当前larger位置-1
                needRadius = min(needRadius, houses[i] - *left); // 比较二者较房子的距离
            }
            ans = max(ans, needRadius);
        }
        return ans;
    }
}; 
//执行用时：52 ms, 在所有 C++ 提交中击败了58.98% 的用户
//内存消耗：24.6 MB, 在所有 C++ 提交中击败了84.48% 的用户
```
