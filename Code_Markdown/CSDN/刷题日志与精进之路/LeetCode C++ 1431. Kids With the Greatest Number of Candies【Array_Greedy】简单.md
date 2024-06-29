


Given the array `candies` and the integer `extraCandies`, where `candies[i]` represents the number of candies that the ***ith*** kid has.

For each kid check if there is a way to distribute `extraCandies` among the kids such that he or she can have the **greatest** number of candies among them. Notice that multiple kids can have the **greatest** number of candies.

 

**Example 1:**

```swift
Input: candies = [2,3,5,1,3], extraCandies = 3
Output: [true,true,true,false,true] 
Explanation: 
Kid 1 has 2 candies and if he or she receives all extra candies (3) will have 5 candies --- the greatest number of candies among the kids. 
Kid 2 has 3 candies and if he or she receives at least 2 extra candies will have the greatest number of candies among the kids. 
Kid 3 has 5 candies and this is already the greatest number of candies among the kids. 
Kid 4 has 1 candy and even if he or she receives all extra candies will only have 4 candies. 
Kid 5 has 3 candies and if he or she receives at least 2 extra candies will have the greatest number of candies among the kids. 
```

**Example 2:**

```swift
Input: candies = [4,2,1,1,2], extraCandies = 1
Output: [true,false,false,false,false] 
Explanation: There is only 1 extra candy, therefore only kid 1 will have the greatest number of candies among the kids regardless of who takes the extra candy.
```

**Example 3:**

```swift
Input: candies = [12,1,12], extraCandies = 10
Output: [true,false,true]
```
 

**Constraints:**
- `2 <= candies.length <= 100`
- `1 <= candies[i] <= 100`
- `1 <= extraCandies <= 50`


题意：给定一个数组 `candies` 和一个整数 `extraCandies` ，其中 `candies[i]` 代表第 `i` 个孩子拥有的糖果数目。对每一个孩子，检查是否存在一种方案，将额外的 `extraCandies` 个糖果分配给他之后，此孩子有**最多**的糖果。注意，允许有多个孩子同时拥有最多的糖果数目。

---
### 解法 顺序遍历+贪心
使用贪心方法，对于每个孩子，判断 `candies[i] + extraCandies >= maximum(Candies)` ，如果为 `true` 则存在一种方案，将额外的几个糖果给他之后，他也拥有最多的糖果：
```cpp
class Solution {
public:
    vector<bool> kidsWithCandies(vector<int>& candies, int extraCandies) {
        vector<bool> ans;
        int mx = *max_element(candies.begin(), candies.end());
        for (const int &v : candies) 
            ans.push_back(v + extraCandies >= mx);
        return ans;
    }
};
```
或者使用C++11的函数式风格：
```cpp
class Solution {
public:
    vector<bool> kidsWithCandies(vector<int>& candies, int extraCandies) {
        vector<bool> ans(candies.size(), false);
        generate(ans.begin(), ans.end(), 
            [&candies, &extraCandies, maxv = *max_element(candies.begin(), candies.end()), n = -1]()mutable -> bool {
                return candies[++n] + extraCandies >= maxv;
            }
        ); 
        return ans;
    }
};
```
执行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了38.85% 的用户
内存消耗：9.1 MB, 在所有 C++ 提交中击败了17.05% 的用户
```
