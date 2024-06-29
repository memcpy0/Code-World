> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

已知存在一个按非降序排列的整数数组 `nums` ，数组中的值不必互不相同。

在传递给函数之前，`nums` 在预先未知的某个下标 `k`（`0 <= k < nums.length`）上进行了 **旋转** ，使数组变为 `[nums[k], nums[k+1], ..., nums[n-1], nums[0], nums[1], ..., nums[k-1]]`（下标 **从 0 开始** 计数）。例如， `[0,1,2,4,4,4,5,6,6,7]` 在下标 `5` 处经旋转后可能变为 `[4,5,6,6,7,0,1,2,4,4]` 。

给你 **旋转后** 的数组 `nums` 和一个整数 `target` ，请你编写一个函数来判断给定的目标值是否存在于数组中。如果 `nums` 中存在这个目标值 `target` ，则返回 `true` ，否则返回 `false` 。

你必须尽可能减少整个操作步骤。

**示例 1：**
```java
输入：nums = [2,5,6,0,0,1,2], target = 0
输出：true
```
**示例 2：**
```java
输入：nums = [2,5,6,0,0,1,2], target = 3
输出：false
```
**提示：**
- `1 <= nums.length <= 5000`
- `-10^4 <= nums[i] <= 10^4`
- 题目数据保证 `nums` 在预先未知的某个下标上进行了旋转
- `-10^4 <= target <= 10^4`

**进阶：**
- 这是 [搜索旋转排序数组](https://leetcode-cn.com/problems/search-in-rotated-sorted-array/description/) 的延伸题目，本题中的 `nums`  可能包含重复元素。
- 这会影响到程序的时间复杂度吗？会有怎样的影响，为什么？

---
### 解法1 顺序查找
代码很简单：
```cpp
class Solution {
public:
    bool search(vector<int>& nums, int target) {
        for (int i = 0; i < nums.size(); ++i) 
            if (nums[i] == target)
                return true;
        return false;
    }
};
```
效率很奇特：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.7 MB, 在所有 C++ 提交中击败了7.84% 的用户
```

---
### 解法2 二分
如果直接用33题的代码，毫无疑问会出现错误。因为数组 `nums[]` 中可能存在重复元素，比如 `nums = [1,1,3,1], target = 3` ，第一次的二分 `nums[mid] = nums[1] = 1 <= nums[hi] = 1` ，会错误判断 `mid` 落入数组右半部有序部分，然后比较 `target > 1` 得出 `target` 不存在于这一"有序部分"的错误结论，进而得到错误的结果。

因此，有必要慎重讨论二分法的细节：
- 每次二分，左半部分和右半部分至少有一边是有序的，分成两部分讨论；
- 左半部分确定有序（`nums[lo] < nums[mid]`) ：
	- ① `target` 落在左半边，对左半边继续二分；
	- ② 其他情况，排除左半边；
- 右半部分确定有序 (`nums[mid] < nums[hi]`) ：
	- ① `target` 落在右半边，对右半边继续二分；
	- ② 其他情况，排除右半边；
- 处理重复情况。如果前面一个有序部分的情况都没判断出来，只能说明 `nums[lo] = nums[hi] = nums[mid]` 且不等于 `target` ，此时往中间压缩即可。

具体代码如下：
```cpp
class Solution {
public:
    bool search(vector<int>& nums, int target) {
        if (nums.empty()) return false;
        int lo = 0, hi = nums.size() - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] == target) return true;
            else if (nums[lo] < nums[mid]) { //[lo, mid)为有序部分
                if (nums[lo] <= target && target < nums[mid]) hi = mid - 1;
                else lo = mid + 1; 
            } 
            else if (nums[mid] < nums[hi]) { //(mid,hi]为有序部分
                if (nums[mid] < target && target <= nums[hi]) lo = mid + 1;
                else hi = mid - 1;
            } 
            else { //nums[lo] == nums[hi] || nums[hi] == nums[mid] 但都 != target // 恢复二段性
                while (lo <= hi && nums[lo] == nums[mid]) ++lo; // 一次走完同个元素的分裂
                while (lo <= hi && nums[hi] == nums[mid]) --hi;
            }
        }
        return false;
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了96.99% 的用户
内存消耗：6.7 MB, 在所有 C++ 提交中击败了5.30% 的用户
```
3  3 3 3 3 1 2 2 2

本篇题解基于「33. 搜索旋转排序数组的官方题解」，请读者在阅读完该题解后再继续阅读本篇题解。

方法一：二分查找
思路

对于数组中有重复元素的情况，二分查找时可能会有 a[l]=a[mid]=a[r]a[l]=a[\textit{mid}]=a[r]a[l]=a[mid]=a[r]，此时无法判断区间 [l,mid][l,\textit{mid}][l,mid] 和区间 [mid+1,r][\textit{mid}+1,r][mid+1,r] 哪个是有序的。

例如 nums=[3,1,2,3,3,3,3]\textit{nums}=[3,1,2,3,3,3,3]nums=[3,1,2,3,3,3,3]，target=2\textit{target}=2target=2，首次二分时无法判断区间 [0,3][0,3][0,3] 和区间 [4,6][4,6][4,6] 哪个是有序的。

对于这种情况，我们只能将当前二分区间的左边界加一，右边界减一，然后在新区间上继续二分查找。
```
class Solution {
public:
    bool search(vector<int> &nums, int target) {
        int n = nums.size();
        if (n == 0) {
            return false;
        }
        if (n == 1) {
            return nums[0] == target;
        }
        int l = 0, r = n - 1;
        while (l <= r) {
            int mid = (l + r) / 2;
            if (nums[mid] == target) {
                return true;
            }
            if (nums[l] == nums[mid] && nums[mid] == nums[r]) { // 多次走完同个元素的分裂
                ++l;
                --r;
            } else if (nums[l] <= nums[mid]) {
                if (nums[l] <= target && target < nums[mid]) {
                    r = mid - 1;
                } else {
                    l = mid + 1;
                }
            } else {
                if (nums[mid] < target && target <= nums[n - 1]) {
                    l = mid + 1;
                } else {
                    r = mid - 1;
                }
            }
        }
        return false;
    }
};
```

作者：力扣官方题解
链接：https://leetcode.cn/problems/search-in-rotated-sorted-array-ii/solutions/704686/sou-suo-xuan-zhuan-pai-xu-shu-zu-ii-by-l-0nmp/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
复杂度分析

时间复杂度：O(n)O(n)O(n)，其中 nnn 是数组 nums\textit{nums}nums 的长度。最坏情况下数组元素均相等且不为 target\textit{target}target，我们需要访问所有位置才能得出结果。

空间复杂度：O(1)O(1)O(1)。

作者：力扣官方题解
链接：https://leetcode.cn/problems/search-in-rotated-sorted-array-ii/solutions/704686/sou-suo-xuan-zhuan-pai-xu-shu-zu-ii-by-l-0nmp/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

 nums[lmax] >= nums[0] nums[lmax] >= nums[rmax]
 但可能有同个元素被分裂了！
**真正的恢复了二段性**！！！
二分解法
根据题意，我们知道，所谓的旋转其实就是「将某个下标前面的所有数整体移到后面，使得数组从整体有序变为分段有序」。

但和 33. 搜索旋转排序数组 不同的是，本题元素并不唯一。

这意味着我们无法直接根据与 nums[0]nums[0]nums[0] 的大小关系，将数组划分为两段，即无法通过「二分」来找到旋转点。

因为「二分」的本质是二段性，并非单调性。只要一段满足某个性质，另外一段不满足某个性质，就可以用「二分」。

如果你有看过我 【宫水三叶】严格 O(logN)，一起看清二分的本质 这篇题解，你应该很容易就理解上句话的意思。如果没有也没关系，我们可以先解决本题，在理解后你再去做 33. 搜索旋转排序数组，我认为这两题都是一样的，不存在先后关系。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307041718149.png)

举个🌰，我们使用数据 [0,1,2,2,2,3,4,5] 来理解为什么不同的旋转点会导致「二段性丢失」：
3 3 3 1 2 3 3 3 3 3 
```
class Solution {
public:
    bool search(vector<int>& nums, int t) {
        int n = nums.size();
        int l = 0, r = n - 1;
        // 恢复二段性
        while (l < r && nums[0] == nums[r]) r--;

        // 第一次二分，找旋转点
        while (l < r) {
            int mid = (l + r + 1) >> 1;
            if (nums[mid] >= nums[0]) {
                l = mid;
            } else {
                r = mid - 1;
            }
        }
        
        int idx = n;
        if (nums[r] >= nums[0] && r + 1 < n) idx = r + 1;

        // 第二次二分，找目标值
        int ans = find(nums, 0, idx - 1, t);
        if (ans != -1) return true;
        ans = find(nums, idx, n - 1, t);
        return ans != -1;
    }

    int find(vector<int>& nums, int l, int r, int t) {
        while (l < r) {
            int mid = l + r >> 1;
            if (nums[mid] >= t) {
                r = mid;
            } else {
                l = mid + 1;
            }
        }
        return nums[r] == t ? r : -1;
    }
};
```
时间复杂度：恢复二段性处理中，最坏的情况下（考虑整个数组都是同一个数）复杂度是 O(n)O(n)O(n)，而之后的找旋转点和目标值都是「二分」，复杂度为 O(logn)O(log{n})O(logn)。整体复杂度为 O(n)O(n)O(n) 的。
空间复杂度：O(1)O(1)O(1)。
进阶
如果真正理解「二分」的话，本题和 33. 搜索旋转排序数组 区别不大。

建议大家在完成两题的基础上试试 面试题 10.03. 搜索旋转数组 。
 
33. 搜索旋转排序数组（找目标值） : 严格 O(logN)，一起看清二分的本质https://leetcode-cn.com/problems/search-in-rotated-sorted-array/solution/shua-chuan-lc-yan-ge-ologn100yi-qi-kan-q-xifo/

81. 搜索旋转排序数组 II（找目标值） : 详解为何元素相同会导致 O(n)，一起看清二分的本质https://leetcode-cn.com/problems/search-in-rotated-sorted-array-ii/solution/gong-shui-san-xie-xiang-jie-wei-he-yuan-xtam4/

153. 寻找旋转排序数组中的最小值（找最小值） : 严格 O(logN)，一起看清二分的本质https://leetcode-cn.com/problems/find-minimum-in-rotated-sorted-array/solution/gong-shui-san-xie-yan-ge-olognyi-qi-kan-6d969/

154. 寻找旋转排序数组中的最小值 II（找最小值） : 详解为何元素相同会导致 O(n)，一起看清二分的本质https://leetcode-cn.com/problems/find-minimum-in-rotated-sorted-array-ii/solution/gong-shui-san-xie-xiang-jie-wei-he-yuan-7xbty/

 