> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091343436.png)
给你一个整数数组 `nums` ，判断是否存在三元组 `[nums[i], nums[j], nums[k]]` 满足 `i != j`、`i != k` 且 `j != k` ，同时还满足 `nums[i] + nums[j] + nums[k] == 0` 。请你返回所有和为 `0` 且不重复的三元组。

**注意：** 答案中不可以包含重复的三元组。

**示例 1：**
```java
输入：nums = [-1,0,1,2,-1,-4]
输出：[[-1,-1,2],[-1,0,1]]
解释：
nums[0] + nums[1] + nums[2] = (-1) + 0 + 1 = 0 。
nums[1] + nums[2] + nums[4] = 0 + 1 + (-1) = 0 。
nums[0] + nums[3] + nums[4] = (-1) + 2 + (-1) = 0 。
不同的三元组是 [-1,0,1] 和 [-1,-1,2] 。
注意，输出的顺序和三元组的顺序并不重要。
```
**示例 2：**
```java
输入：nums = [0,1,1]
输出：[]
解释：唯一可能的三元组和不为 0 。
```
**示例 3：**
```java
输入：nums = [0,0,0]
输出：[[0,0,0]]
解释：唯一可能的三元组和为 0 。
```
**提示：**
- `3 <= nums.length <= 3000 
- <code>-10<sup>5</sup> <= nums[i] <= 10<sup>5</sup></code>

---
本题与 [1. 两数之和](https://leetcode-cn.com/problems/two-sum/) 类似，是非常经典的面试题，但是做法不尽相同。

题目中要求找到所有「不重复」且和为 000 的三元组，这个「不重复」的要求使得我们无法简单地使用三重循环枚举所有的三元组。这是因为在最坏的情况下，数组中的元素全部为 000，即

[0, 0, 0, 0, 0, ..., 0, 0, 0]
任意一个三元组的和都为 000。如果我们直接使用三重循环枚举三元组，会得到 O(N3)O(N^3)O(N 
3
 ) 个满足题目要求的三元组（其中 NNN 是数组的长度）时间复杂度至少为 O(N3)O(N^3)O(N 
3
 )。在这之后，我们还需要使用哈希表进行去重操作，得到不包含重复三元组的最终答案，又消耗了大量的空间。这个做法的时间复杂度和空间复杂度都很高，因此我们要换一种思路来考虑这个问题。

### 解法1 排序+双指针+set去重
「不重复」的本质是什么？我们保持三重循环的大框架不变，只需要保证：

第二重循环枚举到的元素不小于当前第一重循环枚举到的元素；

第三重循环枚举到的元素不小于当前第二重循环枚举到的元素。

也就是说，我们枚举的三元组 (a,b,c)(a, b, c)(a,b,c) 满足 a≤b≤ca \leq b \leq ca≤b≤c，保证了只有 (a,b,c)(a, b, c)(a,b,c) 这个顺序会被枚举到，而 (b,a,c)(b, a, c)(b,a,c)、(c,b,a)(c, b, a)(c,b,a) 等等这些不会，这样就减少了重复。要实现这一点，我们可以将数组中的元素从小到大进行排序，随后使用普通的三重循环就可以满足上面的要求。

同时，对于每一重循环而言，相邻两次枚举的元素不能相同，否则也会造成重复。举个例子，如果排完序的数组为

[0, 1, 2, 2, 2, 3]
 ^  ^  ^
我们使用三重循环枚举到的第一个三元组为 (0,1,2)(0, 1, 2)(0,1,2)，如果第三重循环继续枚举下一个元素，那么仍然是三元组 (0,1,2)(0, 1, 2)(0,1,2)，产生了重复。因此我们需要将第三重循环「跳到」下一个不相同的元素，即数组中的最后一个元素 333，枚举三元组 (0,1,3)(0, 1, 3)(0,1,3)。

下面给出了改进的方法的伪代码实现：

```python
nums.sort()
for first = 0 .. n-1
    // 只有和上一次枚举的元素不相同，我们才会进行枚举
    if first == 0 or nums[first] != nums[first-1] then
        for second = first+1 .. n-1
            if second == first+1 or nums[second] != nums[second-1] then
                for third = second+1 .. n-1
                    if third == second+1 or nums[third] != nums[third-1] then
                        // 判断是否有 a+b+c==0
                        check(first, second, third)
```
这种方法的时间复杂度仍然为 O(N3)O(N^3)O(N 
3
 )，毕竟我们还是没有跳出三重循环的大框架。然而它是很容易继续优化的，可以发现，如果我们固定了前两重循环枚举到的元素 aaa 和 bbb，那么只有唯一的 ccc 满足 a+b+c=0a+b+c=0a+b+c=0。当第二重循环往后枚举一个元素 b′b'b 
′
  时，由于 b′>bb' > bb 
′
 >b，那么满足 a+b′+c′=0a+b'+c'=0a+b 
′
 +c 
′
 =0 的 c′c'c 
′
  一定有 c′<cc' < cc 
′
 <c，即 c′c'c 
′
  在数组中一定出现在 ccc 的左侧。也就是说，我们可以从小到大枚举 bbb，同时从大到小枚举 ccc，即第二重循环和第三重循环实际上是并列的关系。

有了这样的发现，我们就可以保持第二重循环不变，而将第三重循环变成一个从数组最右端开始向左移动的指针，从而得到下面的伪代码：
```python
nums.sort()
for first = 0 .. n-1
    if first == 0 or nums[first] != nums[first-1] then
        // 第三重循环对应的指针
        third = n-1
        for second = first+1 .. n-1
            if second == first+1 or nums[second] != nums[second-1] then
                // 向左移动指针，直到 a+b+c 不大于 0
                while nums[first]+nums[second]+nums[third] > 0
                    third = third-1
                // 判断是否有 a+b+c==0
                check(first, second, third)
```
这个方法就是我们常说的「双指针」，当我们需要枚举数组中的两个元素时，如果我们发现随着第一个元素的递增，第二个元素是递减的，那么就可以使用双指针的方法，将枚举的时间复杂度从 O(N2)O(N^2)O(N 
2
 ) 减少至 O(N)O(N)O(N)。为什么是 O(N)O(N)O(N) 呢？这是因为在枚举的过程每一步中，「左指针」会向右移动一个位置（也就是题目中的 bbb），而「右指针」会向左移动若干个位置，这个与数组的元素有关，但我们知道它一共会移动的位置数为 O(N)O(N)O(N)，均摊下来，每次也向左移动一个位置，因此时间复杂度为 O(N)O(N)O(N)。

注意到我们的伪代码中还有第一重循环，时间复杂度为 O(N)O(N)O(N)，因此枚举的总时间复杂度为 O(N2)O(N^2)O(N 
2
 )。由于排序的时间复杂度为 O(Nlog⁡N)O(N \log N)O(NlogN)，在渐进意义下小于前者，因此算法的总时间复杂度为 O(N2)O(N^2)O(N 
2
 )。

上述的伪代码中还有一些细节需要补充，例如我们需要保持左指针一直在右指针的左侧（即满足 b≤cb \leq cb≤c），具体可以参考下面的代码，均给出了详细的注释。
 
```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        int n = nums.size();
        sort(nums.begin(), nums.end());
        vector<vector<int>> ans;
        // 枚举 a
        for (int first = 0; first < n; ++first) {
            // 需要和上一次枚举的数不相同
            if (first > 0 && nums[first] == nums[first - 1]) {
                continue;
            }
            // c 对应的指针初始指向数组的最右端
            int third = n - 1;
            int target = -nums[first];
            // 枚举 b
            for (int second = first + 1; second < n; ++second) {
                // 需要和上一次枚举的数不相同
                if (second > first + 1 && nums[second] == nums[second - 1]) {
                    continue;
                }
                // 需要保证 b 的指针在 c 的指针的左侧
                while (second < third && nums[second] + nums[third] > target) {
                    --third;
                }
                // 如果指针重合，随着 b 后续的增加
                // 就不会有满足 a+b+c=0 并且 b<c 的 c 了，可以退出循环
                if (second == third) {
                    break;
                }
                if (nums[second] + nums[third] == target) {
                    ans.push_back({nums[first], nums[second], nums[third]});
                }
            }
        }
        return ans;
    }
};
```
复杂度分析

时间复杂度：O(N2)O(N^2)O(N 
2
 )，其中 NNN 是数组 nums\textit{nums}nums 的长度。

空间复杂度：O(log⁡N)O(\log N)O(logN)。我们忽略存储答案的空间，额外的排序的空间复杂度为 O(log⁡N)O(\log N)O(logN)。然而我们修改了输入的数组 nums\textit{nums}nums，在实际情况下不一定允许，因此也可以看成使用了一个额外的数组存储了 nums\textit{nums}nums 的副本并进行排序，空间复杂度为 O(N)O(N)O(N)。

**可以不用SET**！
^
|
|
先排序，然后固定每个数 `nums[i]` ，在 `nums[i+1:size()-1]` 的范围内双指针搜索，寻找和为 `-nums[i]` 的两个元素，找到后插入集合中去重。最后返回结果： 
```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        if (nums.size() < 3) return {};
        set<vector<int>> ans;
        sort(nums.begin(), nums.end());
        for (int i = 0; i < nums.size(); ++i) {
            int x = nums[i], val = 0 - x, f = i + 1, e = nums.size() - 1;
            while (f < e) {
                if (nums[f] + nums[e] > val) --e;
                else if (nums[f] + nums[e] < val) ++f;
                else {
                    ans.insert({x, nums[f], nums[e]});
                    ++f, --e;
                }
            }
        }
        vector<vector<int>> res(ans.begin(), ans.end());
        return res;
    }
};
```
效率很低：
```cpp
执行用时：3084 ms, 在所有 C++ 提交中击败了5.00% 的用户
内存消耗：178 MB, 在所有 C++ 提交中击败了5.00% 的用户
```
---
### 解法2 排序+双指针+去重优化（本题的难点在于如何去重！）
思考发现，如果避免使用集合去重，可以大大降低时间复杂度。我们的做法是：**使用连续重复元素的第一个元素，然后在后续区间内进行双指针；得到某一个解后，跳过已经使用的解元素；双指针移动时跳过无用的重复元素**。这种做法，既顾及到了可能出现重复元素的三元组如 `[2, 2, -4]` ，也考虑到了重复使用同一元素的问题，而且避免了多次得到重复三元组：
- 假设数 `x` 重复了 `k` 次，我们不必要执行 `k` 次两数之和然后去重。对于重复的数，我们只需要找到该数第一次出现的位置：
	```cpp
	if (i > 0 && nums[i] == nums[i-1]) continue
	```
	就可以让我们跳过第 `2, 3,..., k` 次访问同一个数的情况。
- 这里的去重思路是：第一个数的三数之和的解，必然包括它之后相同的数的三数之和解。于是我们只需要得到**连续重复元素序列中第一个数的解**即可。

具体代码如下：
```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        if (nums.size() < 3) return {};
        vector<vector<int>> ans;
        sort(nums.begin(), nums.end()); 
        for (int i = 0; i < nums.size(); ++i) { 
            if (i > 0 && nums[i] == nums[i - 1]) continue; //使用连续重复元素的第一个
            int newTarget = -nums[i], lo = i + 1, hi = nums.size() - 1; //三数变两数
            while (lo < hi) {
                if (nums[lo] + nums[hi] == newTarget) {
                    ans.push_back({nums[i], nums[lo], nums[hi]});
                    while (lo < hi && nums[lo] == nums[lo + 1]) ++lo; //已经使用过的解元素
                    while (lo < hi && nums[hi] == nums[hi - 1]) --hi; //已经使用过的解元素
                    ++lo;
                    --hi;
                } else if (nums[lo] + nums[hi] < newTarget) ++lo;
                else --hi;
            }
        } 
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：144 ms, 在所有 C++ 提交中击败了31.17% 的用户
内存消耗：19.8 MB, 在所有 C++ 提交中击败了39.40% 的用户
```
更多优化如下：
- 不符合条件时，在 `lo` 或者 `hi` 移动时跳过重复元素；
- 如果当前的固定数 `nums[i]` 和后续数组最小的两个数 `nums[i + 1], nums[i + 2]` 相加，结果大于零，说明后面已经没有结果，直接退出；
- 如果当前的固定数 `nums[i]` 和后续数组最大的两个数 `nums[size - 1], nums[size - 2]` 相加，结果小于零，跳过此次双指针搜索。

```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> ans;
        sort(nums.begin(), nums.end()); 
        const int n = nums.size();
        for (int i = 0; i < n - 2; ++i) { //只搜索到倒数第三个数为止
            if (i > 0 && nums[i] == nums[i - 1]) continue; //去重剪枝,使用连续重复元素的第一个
            if (nums[i] + nums[i + 1] + nums[i + 2] > 0) break; //后续循环不会有解
            if (nums[i] + nums[n - 2] + nums[n - 1] < 0) continue; //本次循环不会有解
            int newTarget = -nums[i], lo = i + 1, hi = n - 1; //三数变两数
            while (lo < hi) {
                int t = nums[lo] + nums[hi];
                if (t == newTarget) {
                    ans.push_back({nums[i], nums[lo], nums[hi]});
                    while (lo < hi && nums[lo] == nums[lo + 1]) ++lo; //已经使用过的解元素
                    while (lo < hi && nums[hi] == nums[hi - 1]) --hi; //已经使用过的解元素
                    ++lo;
                    --hi;
                } else if (t < newTarget) {
                    while (lo < hi && nums[lo] == nums[lo + 1]) ++lo; //跳过无用的重复值
                    ++lo;
                } else {
                    while (lo < hi && nums[hi] == nums[hi - 1]) --hi; //跳过无用的重复值
                    --hi;
                }
            }
        } 
        return ans;
    }
};
```
结果如下：
```cpp
执行用时：112 ms, 在所有 C++ 提交中击败了62.12% 的用户
内存消耗：19.8 MB, 在所有 C++ 提交中击败了34.94% 的用户
```
复杂度分析

时间复杂度：O(N2)O(N^2)O(N 
2
 )，其中 NNN 是数组 nums\textit{nums}nums 的长度。

空间复杂度：O(log⁡N)O(\log N)O(logN)。我们忽略存储答案的空间，额外的排序的空间复杂度为 O(log⁡N)O(\log N)O(logN)。然而我们修改了输入的数组 nums\textit{nums}nums，在实际情况下不一定允许，因此也可以看成使用了一个额外的数组存储了 nums\textit{nums}nums 的副本并进行排序，空间复杂度为 O(N)O(N)O(N)。
 