> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。



Given an array `nums`. We define a running sum of an array as `runningSum[i] = sum(nums[0]…nums[i])` . Return the running sum of `nums`.

**Example 1:**

```swift
Input: nums = [1,2,3,4]
Output: [1,3,6,10]
Explanation: Running sum is obtained as follows: [1, 1+2, 1+2+3, 1+2+3+4].
```

**Example 2:**

```swift
Input: nums = [1,1,1,1,1]
Output: [1,2,3,4,5]
Explanation: Running sum is obtained as follows: [1, 1+1, 1+1+1, 1+1+1+1, 1+1+1+1+1].
```

**Example 3:**

```swift
Input: nums = [3,1,2,10,1]
Output: [3,4,6,16,17]
```

**Constraints:**
- `1 <= nums.length <= 1000`
- `-10^6 <= nums[i] <= 10^6`

题意：返回整数数组 `nums` 的动态和。

---
### 解法 顺序遍历
创建答案数组，从头到尾模拟一遍，每次将当前前缀和的值赋给 `ans[i]` 即可。时间复杂度 $O(n)$ ，空间复杂度 $O(n)$ 。
```cpp
//C++ version
class Solution {
public:
    vector<int> runningSum(vector<int>& nums) {
        vector<int> ans;
        int sum = 0;
        for (const int &v : nums) {
            sum += v;
            ans.push_back(sum);
        }
        return ans;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：8.6 MB, 在所有 C++ 提交中击败了24.90% 的用户
```
```java
//Java version
class Solution {
    public int[] runningSum(int[] nums) {
        int[] ans = new int[nums.length];
        int sum = 0;
        for (int i = 0; i < nums.length; ++i) {
            sum += nums[i];
            ans[i] = sum;
        }
        return ans;
    }
}
//执行用时：0 ms, 在所有 Java 提交中击败了100.00% 的用户
//内存消耗：38.5 MB, 在所有 Java 提交中击败了56.13% 的用户
```
