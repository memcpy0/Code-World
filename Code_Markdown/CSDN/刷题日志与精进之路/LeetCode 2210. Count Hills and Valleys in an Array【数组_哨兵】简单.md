<p>You are given a <strong>0-indexed</strong> integer array <code>nums</code>. An index <code>i</code> is part of a <strong>hill</strong> in <code>nums</code> if the closest non-equal neighbors of <code>i</code> are smaller than <code>nums[i]</code>. Similarly, an index <code>i</code> is part of a <strong>valley</strong> in <code>nums</code> if the closest non-equal neighbors of <code>i</code> are larger than <code>nums[i]</code>. Adjacent indices <code>i</code> and <code>j</code> are part of the <strong>same</strong> hill or valley if <code>nums[i] == nums[j]</code>.</p>

<p>Note that for an index to be part of a hill or valley, it must have a non-equal neighbor on <strong>both</strong> the left and right of the index.</p>

<p>Return <i>the number of hills and valleys in </i><code>nums</code>.</p>
 
<p><strong>Example 1:</strong></p>

```cpp
Input: nums = [2,4,1,1,6,5]
Output: 3
Explanation:
At index 0: There is no non-equal neighbor of 2 on the left, so index 0 is neither a hill nor a valley.
At index 1: The closest non-equal neighbors of 4 are 2 and 1. Since 4 > 2 and 4 > 1, index 1 is a hill. 
At index 2: The closest non-equal neighbors of 1 are 4 and 6. Since 1 < 4 and 1 < 6, index 2 is a valley.
At index 3: The closest non-equal neighbors of 1 are 4 and 6. Since 1 < 4 and 1 < 6, index 3 is a valley, but note that it is part of the same valley as index 2.
At index 4: The closest non-equal neighbors of 6 are 1 and 5. Since 6 > 1 and 6 > 5, index 4 is a hill.
At index 5: There is no non-equal neighbor of 5 on the right, so index 5 is neither a hill nor a valley. 
There are 3 hills and valleys so we return 3.
```

<p><strong>Example 2:</strong></p>

```cpp
Input: nums = [6,6,5,5,4,1]
Output: 0
Explanation:
At index 0: There is no non-equal neighbor of 6 on the left, so index 0 is neither a hill nor a valley.
At index 1: There is no non-equal neighbor of 6 on the left, so index 1 is neither a hill nor a valley.
At index 2: The closest non-equal neighbors of 5 are 6 and 4. Since 5 < 6 and 5 > 4, index 2 is neither a hill nor a valley.
At index 3: The closest non-equal neighbors of 5 are 6 and 4. Since 5 < 6 and 5 > 4, index 3 is neither a hill nor a valley.
At index 4: The closest non-equal neighbors of 4 are 5 and 1. Since 4 < 5 and 4 > 1, index 4 is neither a hill nor a valley.
At index 5: There is no non-equal neighbor of 1 on the right, so index 5 is neither a hill nor a valley.
There are 0 hills and valleys so we return 0.
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>3 &lt;= nums.length &lt;= 100</code></li>
	<li><code>1 &lt;= nums[i] &lt;= 100</code></li>
</ul>

题意：给出一个下标从 $0$ 开始的整数数组 `nums` 。如果两侧距 `i` 最近的不相等邻居的值均小于 `nums[i]` ，则下标 `i` 是 `nums` 中，某个峰的一部分。类似地，如果两侧距 `i` 最近的不相等邻居的值均大于 `nums[i]` ，则下标 `i` 是 `nums` 中某个谷的一部分。对于相邻下标 `i` 和 `j` ，如果 `nums[i] == nums[j]` ， 则认为这两下标属于 **同一个** 峰或谷。注意，要使某个下标所做峰或谷的一部分，那么它左右两侧必须 **都** 存在不相等邻居。返回 `nums` 中峰和谷的数量。

---
### 解法1 去重+遍历统计
比赛的时候为了图快，先去重并将数据存储在新数组 `arr[]` ，之后从第一个元素开始遍历数组，判断当前元素是否小于前驱和后继元素（或大于），是则计数。
```cpp
// C++ version
class Solution {
public:
    int countHillValley(vector<int>& nums) {
        int arr[110] = {nums[0]}, len = 1, ans = 0;
        for (int i = 1; i < nums.size(); ++i)
            if (nums[i] != nums[i - 1]) arr[len++] = nums[i];
        for (int i = 1; i < len - 1; ++i) {
            if ((arr[i] > arr[i - 1] && arr[i] > arr[i + 1]) || 
            (arr[i] < arr[i - 1] && arr[i] < arr[i + 1])) ++ans;
        }
        return ans;
    }
};
// 执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
// 内存消耗：8.7 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
---
### 解法2 不去重+快慢指针
用 `i` 指向当前相同元素区域的第一个元素，`j` 指向下一个相同元素区域的第一个元素，然后进行比较即可：
```cpp
// C++ version
class Solution {
public:
    int countHillValley(vector<int>& nums) {
        int ans = 0;
        for (int i = 1, n = nums.size(); i < n - 1; ) {
            int j = i + 1;
            while (j < n - 1 && nums[j] == nums[i]) ++j; // [i,j-1]区域为相同元素
            if ((nums[i] > nums[i - 1] && nums[i] > nums[j]) || 
                (nums[i] < nums[i - 1] && nums[i] < nums[j])) ++ans;
            i = j;
        }
        return ans;
    }
};
// 执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
// 内存消耗：8.6 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
---
### 解法3 左右哨兵+峰谷交替
**峰和谷是交替变化的，山峰不会连着山峰，山谷不会连着山谷**。于是可以维护节点中左右哨兵的值，用来判断是否经过了（完整的）山峰或山谷，是则计数。
```cpp
// C++ version
class Solution {
public:
    int countHillValley(vector<int>& nums) {
        int ans = 0, ls = 0, rs; // left soldier, right soldier
        for (int i = 1, n = nums.size(); i < n; ++i) {
            if (nums[i] > nums[i - 1]) { // 上升边
                rs = -1; // 表示经过上升边(山谷的右侧)
                if (ls == rs) ++ans; // ls==rs==-1,表示之前是山谷
                ls = 1;  // 表示经过上升边(山峰的左侧)
            } else if (nums[i] < nums[i - 1]) { // 下降边
                rs = 1;  // 表示经过下降边(山峰的右侧)
                if (ls == rs) ++ans; // ls==rs==1,表示之前是山峰
                ls = -1; // 表示经过下降边(山谷的左侧)
            }
        }
        return ans;
    }
};
// 执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
// 内存消耗：8.5 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
