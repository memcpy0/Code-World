Given scores of **N** athletes, find their relative ranks and the people with the top three highest scores, who will be awarded medals: `"Gold Medal"` , `"Silver Medal"` and `"Bronze Medal"` .

**Example 1:**
```c
Input: [5, 4, 3, 2, 1]
Output: ["Gold Medal", "Silver Medal", "Bronze Medal", "4", "5"]
Explanation: The first three athletes got the top three highest scores, so they got "Gold Medal", "Silver Medal" and "Bronze Medal". 
For the left two athletes, you just need to output their relative ranks according to their scores.
```

**Note:**
-  `N` is a **positive** integer and won't exceed `10,000` .
- All the scores of athletes are guaranteed to be **unique**.


题意：给出 `N` 名运动员的成绩，根据成绩返回他们的名次字符串数组，前 `3` 名特定返回 `Gold Medal, Silver Medal, Bronze Medal` 。注意，分数越高的选手，排名越靠前；没有重复的分数。

----
### 思路1 记录位置
使用 `pair<int, int>` 数组记录每个成绩的原始位置，然后对成绩从大到小排序，最后根据成绩的顺序，将名次按照原始位置填入返回的字符串数组中。代码如下：
```cpp
class Solution {
public:
    vector<string> findRelativeRanks(vector<int>& nums) {
        vector<string> ans;
        if (nums.empty()) return ans;
        vector<pair<int, int>> temp;
        for (int i = 0; i < nums.size(); ++i)
            temp.push_back(make_pair(nums[i], i));
        sort(temp.begin(), temp.end(), [&](const pair<int, int> &a, const pair<int, int> &b) {
            return a.first > b.first;
        });
        ans.resize(temp.size());
        for (int i = 0; i < temp.size(); ++i) {
            if (i == 0) ans[temp[i].second] = "Gold Medal";
            else if (i == 1) ans[temp[i].second] = "Silver Medal";
            else if (i == 2)  ans[temp[i].second] = "Bronze Medal";
            else  ans[temp[i].second] = to_string(i + 1);
        }
        return ans;
    }
};
```
提交后效率如下：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了92.63% 的用户
内存消耗：10.1 MB, 在所有 C++ 提交中击败了69.70% 的用户
```
其实可以不用 `pair<int, int>` 的，直接使用 `nums[i]` 对 `temp[]` 数组中的位置进行排序就可以了：
```cpp
class Solution {
public:
    vector<string> findRelativeRanks(vector<int>& nums) {
        vector<string> ans;
        if (nums.empty()) return ans;
        vector<int> temp;
        for (int i = 0; i < nums.size(); ++i) temp.push_back(i);
        sort(temp.begin(), temp.end(), [&](const int &a, const int &b) { return nums[a] > nums[b]; });
        ans.resize(temp.size());
        for (int i = 0; i < temp.size(); ++i) {
            switch (i) {
                case 0: ans[temp[i]] = "Gold Medal"; break;
                case 1: ans[temp[i]] = "Silver Medal"; break;
                case 2: ans[temp[i]] = "Bronze Medal"; break;
                default: ans[temp[i]] = to_string(i + 1); break;
            }
        }
        return ans;
    }
};
```
时间效率提高了一点：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了98.30% 的用户
内存消耗：9.8 MB, 在所有 C++ 提交中击败了86.23% 的用户
```
---
### 思路2 离散化
这道题类似离散化——**用名次数组代替值数组**，值相同的名次也相同。需要注意的是：这里使用 `sort` 从小到大排序，是为了符合 `lower_bound` 的默认行为；不然从大到小排序，使用 `lower_bound` 时需要一个 `Comp` 比较函数。
```cpp
class Solution {
public:
    vector<string> findRelativeRanks(vector<int>& nums) {
        vector<string> ans;
        if (nums.empty()) return ans; 
        vector<int> temp(nums.begin(), nums.end());
        int n = nums.size();
        sort(temp.begin(), temp.end()); 
        ans.resize(n);
        for (int i = 0; i < n; ++i) {
            int rank = lower_bound(temp.begin(), temp.end(), nums[i]) - temp.begin(); 
            if (rank == n - 1) ans[i] = "Gold Medal"; 
            else if (rank == n - 2) ans[i] = "Silver Medal";  
            else if (rank == n - 3) ans[i] = "Bronze Medal";  
            else ans[i] = to_string(n - rank);
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了80.15% 的用户
内存消耗：9.7 MB, 在所有 C++ 提交中击败了88.70% 的用户
```
---
### 思路3 计数排序
由于数据范围整体不大，可以使用计数排序，得到 $O(n)$ 的算法复杂度。算法过程是：
- 假设原始数组为 `[10,3,8,9,4]` ，可以很容易得到最大分值为 `10` ；
- 申请长度为 `11` 的数组 `numsIndex` ，元素全是 `0` ；
- 接下来操作 `numIndex` ，把原始数组 `nums` 中的每个元素的值 `nums[i]`（运动员的成绩，是**正整数**）作为 `numsIndex` 的下标，把 `numsIndex[nums[i]]` 位置的值设置为原始数组的下标 `+ 1` 。
- 这样处理后，`numsIndex = [0, 0, 0, 2, 5, 0, 0, 0, 3, 4, 1]` ，成为记录运动员名次（从后往前名次从高到低）到原始数组位置的一个映射。如果一个位置 `i` 的值 `numsIndex[i]` 大于 `0` ，说明这个位置代表的运动员成绩 `i` 在原始数组中的位置为 `numsIndex[i] - 1` 。
- 所以第一名在原数组的 `0` 下标处，第二名在原数组的 `3` 下标处，……

具体代码如下：
```cpp
class Solution {
public:
    vector<string> findRelativeRanks(vector<int>& nums) {
        vector<string> ans;
        if (nums.empty()) return ans; 
        int maxval = INT_MIN, n = nums.size(), rank = 1; 
        for (const int &v : nums) maxval = max(maxval, v); //找到最大值,作为计数数组的范围
        vector<int> numsIndex(maxval + 1); 
        //把原数组nums中每个元素的值当做numsIndex的下标,把该下标处的值置为原数组的下标+1
        for (int i = 0; i < n; ++i) numsIndex[nums[i]] = i + 1; 
        //从后往前看>0的值
        ans.resize(n);
        for (int i = maxval; i >= 0; --i) {
            if (numsIndex[i] > 0) { 
                string &s = ans[numsIndex[i] - 1];
                if (rank == 1) s = "Gold Medal";
                else if (rank == 2) s = "Silver Medal";
                else if (rank == 3) s = "Bronze Medal";
                else s = to_string(rank);
                ++rank;
            }
        }
        return ans;
    }
};
```
效率很感人：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：10.6 MB, 在所有 C++ 提交中击败了66.11% 的用户
```
