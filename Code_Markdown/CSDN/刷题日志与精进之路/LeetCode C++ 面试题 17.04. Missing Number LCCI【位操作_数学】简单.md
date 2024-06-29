

An array contains all the integers from 0 to n, except for one number which is missing.  Write code to find the missing integer. Can you do it in `O(n)` time?

**Note:** This problem is slightly different from the original one the book.

**Example 1:**

```swift
Input: [3,0,1]
Output: 2
```

 

**Example 2:**

```swift
Input: [9,6,4,2,3,5,7,0,1]
Output: 8
```


题意：数组 `nums` 包含从 `0` 到 `n` 的所有整数，但其中缺了一个。请编写代码找出那个缺失的整数。

---
### 解法1 位操作
简单题目，不多说：
```cpp
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int ans = nums.size();
        for (int i = 0; i < nums.size(); ++i) 
            ans ^= i ^ nums[i];
        return ans;
    }
};
```
执行效率如下：
```cpp
执行用时：40 ms, 在所有 C++ 提交中击败了76.30% 的用户
内存消耗：17.2 MB, 在所有 C++ 提交中击败了12.25% 的用户
```
一模一样的Java代码如下：
```java
class Solution {
    public int missingNumber(int[] nums) {
        int missingNum = nums.length;
        for (int i = 0; i < nums.length; ++i) 
            missingNum  ^= i ^ nums[i]; 
        return missingNum;
    }
}
```
对应的效率如下：
```cpp
执行用时：1 ms, 在所有 Java 提交中击败了44.67% 的用户
内存消耗：38.8 MB, 在所有 Java 提交中击败了90.97% 的用户
```
---
### 解法2 求和公式
```cpp
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int n = nums.size(), ans = n * (n + 1) / 2;
        for (const int &v : nums) ans -= v;
        return ans;
    }
};
```
效率如下，差不多：
```cpp
执行用时：40 ms, 在所有 C++ 提交中击败了76.30% 的用户
内存消耗：17.2 MB, 在所有 C++ 提交中击败了13.48% 的用户
```
