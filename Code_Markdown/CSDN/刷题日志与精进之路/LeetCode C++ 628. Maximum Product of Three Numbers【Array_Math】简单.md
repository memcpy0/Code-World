
Given an integer array nums, find three numbers whose product is maximum and return the maximum product.

 

**Example 1:**

```swift
Input: nums = [1,2,3]
Output: 6
```

**Example 2:**

```swift
Input: nums = [1,2,3,4]
Output: 24
```

**Example 3:**

```swift
Input: nums = [-1,-2,-3]
Output: -6
```

 

**Constraints:**
- <code>3 <= nums.length <= 10<sup>4</sup></code>
 - `-1000 <= nums[i] <= 1000`


题意：给定一个整型数组，在数组中找出由三个数组成的最大乘积，并输出这个乘积。注意，输入的数组中**任意三个数的乘积**不会超出32位有符号整数的范围。

---
分类讨论，可知数组有几种情况：
- 全非负数、全非正数：最大三个数相乘
- 有正、有负、有零：最大三个数相乘，或者最小两个数相乘再乘以最大一个数
### 解法1 排序
先排序，之后很容易得到最大值、最小值等：
```cpp
class Solution {
public:
    int maximumProduct(vector<int>& nums) {
        if (nums.size() == 3) return nums[0] * nums[1] * nums[2];
        sort(nums.begin(), nums.end());
        int n = nums.size() - 1;
        if (nums[n] <= 0 || nums[0] >= 0) return nums[n] * nums[n - 1] * nums[n - 2]; //都<=0或>=0
        return max(nums[n] * nums[n - 1] * nums[n - 2], nums[n] * nums[0] * nums[1]);
    }
};
```
时间复杂度为 $O(n\log n)$ ，执行效率如下：
```cpp
执行用时：164 ms, 在所有 C++ 提交中击败了19.24% 的用户
内存消耗：27.5 MB, 在所有 C++ 提交中击败了6.24% 的用户
```
---
### 解法2 直接遍历
直接遍历整个数组，得到最小值、次小值、最大值、次大值、再次值：
```cpp
class Solution {
public:
    int maximumProduct(vector<int>& nums) {
        if (nums.size() == 3) return nums[0] * nums[1] * nums[2];
        int min1 = INT_MAX, min2 = INT_MAX; //最小值、次小值
        int max1 = INT_MIN, max2 = INT_MIN, max3 = INT_MIN; //最大值、次大值、再次值
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            int cur = nums[i];
            if (cur <= min1) { //更新最小值、次小值
                min2 = min1;
                min1 = cur;
            } else if (cur <= min2) //更新次小值
                min2 = cur;
            if (cur >= max1) { //更新最大值、次大值、再次值
                max3 = max2;
                max2 = max1;
                max1 = cur;
            } else if (cur >= max2) { //更新次大值、再次值
                max3 = max2;
                max2 = cur;
            } else if (cur >= max3) //更新再次值
                max3 = cur;
        }
        return max(max1 * max2 * max3, min1 * min2 * max1); //两种情况:最大三个数相乘、最大一个数*最小两个数
    }
};
```
线性解法，效率不错：
```cpp
执行用时：76 ms, 在所有 C++ 提交中击败了96.87% 的用户
内存消耗：27.5 MB, 在所有 C++ 提交中击败了6.24% 的用户
```
对应的Java代码如下：
```java
class Solution {
    public int maximumProduct(int[] nums) {
        if (nums.length == 3) return nums[0] * nums[1] * nums[2];
        int min1 = Integer.MAX_VALUE, min2 = Integer.MAX_VALUE; //min1 < min2
        int max1 = Integer.MIN_VALUE, max2 = Integer.MIN_VALUE, max3 = Integer.MIN_VALUE; //max1 > max2 > max3
        for (int v : nums) {
            if (v < min1) {
                min2 = min1;
                min1 = v;
            } else if (v < min2) min2 = v;
            if (v > max1) {
                max3 = max2;
                max2 = max1;
                max1 = v;
            } else if (v > max2) {
                max3 = max2;
                max2 = v;
            } else if (v > max3) max3 = v;
        }
        return Math.max(max1 * max2 * max3, min1 * min2 * max1);
    }
}
```
运行效率如下：
```java
执行用时：2 ms, 在所有 Java 提交中击败了99.13% 的用户
内存消耗：39.7 MB, 在所有 Java 提交中击败了75.25% 的用户
```
