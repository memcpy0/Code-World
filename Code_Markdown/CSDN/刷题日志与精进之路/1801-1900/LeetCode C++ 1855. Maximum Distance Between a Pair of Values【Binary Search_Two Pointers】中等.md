<p>You are given two <strong>non-increasing 0-indexed </strong>integer arrays <code>nums1</code>​​​​​​ and <code>nums2</code>​​​​​​.</p>

<p>A pair of indices <code>(i, j)</code>, where <code>0 &lt;= i &lt; nums1.length</code> and <code>0 &lt;= j &lt; nums2.length</code>, is <strong>valid</strong> if both <code>i &lt;= j</code> and <code>nums1[i] &lt;= nums2[j]</code>. The <strong>distance</strong> of the pair is <code>j - i</code>​​​​.</p>

<p>Return <em>the <strong>maximum distance</strong> of any <strong>valid</strong> pair </em><code>(i, j)</code><em>. If there are no valid pairs, return </em><code>0</code>.</p>

<p>An array <code>arr</code> is <strong>non-increasing</strong> if <code>arr[i-1] &gt;= arr[i]</code> for every <code>1 &lt;= i &lt; arr.length</code>.</p>
 
<p><strong>Example 1:</strong></p>

```clike
Input: nums1 = [55,30,5,4,2], nums2 = [100,20,10,10,5]
Output: 2
Explanation: The valid pairs are (0,0), (2,2), (2,3), (2,4), (3,3), (3,4), and (4,4).
The maximum distance is 2 with pair (2,4).
```

<p><strong>Example 2:</strong></p>

```clike
Input: nums1 = [2,2,2], nums2 = [10,10,1]
Output: 1
Explanation: The valid pairs are (0,0), (0,1), and (1,1).
The maximum distance is 1 with pair (0,1). 
```

<p><strong>Example 3:</strong></p>

```clike
Input: nums1 = [30,29,19,5], nums2 = [25,25,25,25,25]
Output: 2
Explanation: The valid pairs are (2,2), (2,3), (2,4), (3,3), and (3,4).
The maximum distance is 2 with pair (2,4).
```

 
<p><strong>Example 4:</strong></p>

```clike
Input: nums1 = [5,4], nums2 = [3,2]
Output: 0
Explanation: There are no valid pairs, so return 0.
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums1.length &lt;= 10<sup>5</sup></code></li>
	<li><code>1 &lt;= nums2.length &lt;= 10<sup>5</sup></code></li>
	<li><code>1 &lt;= nums1[i], nums2[j] &lt;= 10<sup>5</sup></code></li>
	<li>Both <code>nums1</code> and <code>nums2</code> are <strong>non-increasing</strong>.</li>
</ul>

题意：给你两个 <strong>非递增</strong> 的整数数组 <code>nums1</code>​​​​​​ 和 <code>nums2</code>​​​​​​ ，数组下标均 <strong>从 0 开始</strong> 计数。下标对 <code>(i, j)</code> 中 <code>0 &lt;= i &lt; nums1.length</code> 且 <code>0 &lt;= j &lt; nums2.length</code> 。如果该下标对同时满足 <code>i &lt;= j</code> 且 <code>nums1[i] &lt;= nums2[j]</code> ，则称之为 <strong>有效</strong> 下标对，该下标对的 <strong>距离</strong> 为 <code>j - i</code>​​ 。​​</p>

<p>返回所有 <strong>有效</strong> 下标对<em> </em><code>(i, j)</code><em> </em>中的 <strong>最大距离</strong> 。如果不存在有效下标对，返回 <code>0</code> 。</p>

---
### 解法1 二分
由于两个数组有序，因此对于每个 `0 <= i < nums1.length` ，都可以在 `i <= j < nums2.length` 范围内寻找最后一个大于等于 `nums1[i]` 的 `nums2[j]` ，即为相对于 `nums1[i]` 的最远有效下标对 `(i, j)` 。这种做法的时间复杂度为 $O(n\log n)$ 。

手写二分如下，由于个人习惯了 `lower_bound, upper_bound` 这样的API，所以这里将**寻找最后一个 `>= nums1[i]` 的 `nums2[j]` 的位置**，转换为**寻找第一个 `< nums1[i]` 的 `nums2[j]` 的位置 `lo`** ，`lo - 1` 就是目标位置：
```cpp
class Solution {
public:
    int maxDistance(vector<int>& nums1, vector<int>& nums2) {
        int ans = 0, m = nums1.size(), n = nums2.size();
        for (int i = 0; i < m; ++i) { //找到最后一个>=nums1[i]的nums2[j]
            int lo = i, hi = n; //找到第一个<nums1[i]的nums2[j]
            while (lo < hi) {
                int mid = lo + (hi - lo) / 2;
                if (nums2[mid] < nums1[i]) hi = mid;
                else lo = mid + 1;
            }
            if (lo == i) continue; //不存在i<=j且nums2[j]>=nums1[i]的j
            if (lo - 1 - i > ans) ans = lo - 1 - i;
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：224 ms, 在所有 C++ 提交中击败了27.78% 的用户
内存消耗：96.1 MB, 在所有 C++ 提交中击败了50.00% 的用户
```
当然，写成**寻找最后一个 `>= nums1[i]` 的 `nums2[j]` 的位置** 也不难，只是不太符合我的编程习惯：
```cpp
class Solution {
public:
    int maxDistance(vector<int>& nums1, vector<int>& nums2) {
        int ans = 0, m = nums1.size(), n = nums2.size();
        for (int i = 1; i <= m; ++i) { //找到最后一个>=nums1[i]的nums2[j]
            int lo = i - 1, hi = n; //位置的解的范围应该在[i-1,n]
            while (lo < hi) {
                int mid = (lo + hi + 1) / 2;
                if (nums2[mid - 1] >= nums1[i - 1]) lo = mid;
                else hi = mid - 1;
            } 
            if (lo < i) continue; //不存在i<=j且nums2[j]>=nums1[i]的j
            if (lo - i > ans) ans = lo - i;
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：232 ms, 在所有 C++ 提交中击败了22.06% 的用户
内存消耗：96 MB, 在所有 C++ 提交中击败了81.50% 的用户
```
使用STL提供的 `upper_bound` ，注意有如下情况：
- `lower_bound(v.begin(), v.end(), target, less<int>());` 默认选项，在非降序序列中寻找第一个大于等于 `target` 的值的位置；
- `upper_bound(v.begin(), v.end(), target, less<int>());` 默认选项，在非降序序列中寻找第一个大于 `target` 的值的位置；
- `lower_bound(v.begin(), v.end(), target, greater<int>());` 在非升序序列中寻找第一个小于等于 `target` 的值的位置；
- `upper_bound(v.begin(), v.end(), target, greater<int>());` 在非升序序列中寻找第一个小于 `target` 的值的位置。

因此代码如下：
```cpp
class Solution {
public:
    int maxDistance(vector<int>& nums1, vector<int>& nums2) {
        int ans = 0, m = nums1.size(), n = nums2.size();
        for (int i = 0; i < m; ++i) { //找到最后一个>=nums1[i]的nums2[j]
            auto it = upper_bound(nums2.begin() + i, nums2.end(), nums1[i], greater<int>()); //找到第一个<nums1[i]的nums2[j]
            int j = it - nums2.begin() - 1; 
            if (j < i) continue; //不存在i<=j且nums2[j]>=nums1[i]的j
            if (j - i > ans) ans = j - i;
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：240 ms, 在所有 C++ 提交中击败了17.02% 的用户
内存消耗：96.2 MB, 在所有 C++ 提交中击败了7.42% 的用户
```
---
### 解法2 双指针
代码很简单：
```cpp
class Solution {
public:
    int maxDistance(vector<int>& nums1, vector<int>& nums2) {
        int ans = 0, m = nums1.size(), n = nums2.size();
        for (int i = 0; i < m; ++i) 
            while (i + ans < n && nums2[i + ans] >= nums1[i]) ++ans;
        return ans == 0 ? 0 : ans - 1;
    }
};
```
运行效率如下：
```cpp
执行用时：164 ms, 在所有 C++ 提交中击败了96.51% 的用户
内存消耗：96.2 MB, 在所有 C++ 提交中击败了5.20% 的用户
```
