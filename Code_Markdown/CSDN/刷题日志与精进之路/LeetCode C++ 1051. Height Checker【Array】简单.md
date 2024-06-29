### 
Students are asked to **stand in non-decreasing order of heights** for an annual photo.
Return **the minimum number of students not standing in the right positions**.  (This is the number of students that must move in order for **all students to be standing in non-decreasing order** of height.)

**Example 1:**
```
Input: [1,1,4,2,1,3]
Output: 3 
Explanation: 
Students with heights 4, 3 and the last 1 are not standing in the right positions.
```
**Note:**
- `1 <= heights.length <= 100` 
 - `1 <= heights[i] <= 100`


题意：给出一个高度数组，每个学生必须以高度升序排列，站在正确的位置上。而我们要求出没有站在正确位置上的学生的人数。这就是道阅读理解题。

---
思路1：排个序，每个人都站在正确的位置上了，然后将它和原数组依次比较，如果同一个位置的元素不同，说明该学生没有站在正确的位置上。$O(n\log n)$ ，时间效率不够。
```cpp
class Solution {
public:
   int heightChecker(vector<int>& heights) {
       vector<int> temp(heights.begin(), heights.end());
       sort(temp.begin(), temp.end());
       int ans = 0;
       for (int i = 0; i < temp.size(); i++) 
           if (temp[i] != heights[i]) ans++;
       return ans;
   }
};
```
效率：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了30.71% 的用户
内存消耗：8.3 MB, 在所有 C++ 提交中击败了61.45% 的用户
```

---
思路二：高度不会超过 `100` ，数组长度最大也只有 `100` ，那么还有更加高效的解法，桶排序。想象从 `1-100` 每个整数有一个桶，然后我们将对应元素出现的次数存放到相应的桶里面。如题中的例子，bulk为 `[0, 3, 1, 1, 2]`。

然后，从 `i = 1` 开始依次检查每个桶，如果桶中有元素，就看对应的 `heights` 数组中是否依次出现了 `bulk[i]` 个 `i` 元素，不相等的就是没有站在正确位置上的。 
```cpp
class Solution {
public:
    int heightChecker(vector<int>& heights) {
        vector<int> bulk(101, 0);
        for (auto i : heights) bulk[i]++;
        int result = 0, cur = 0;
        for (int i = 1; i < 101; i++)   //检查每个桶 
            if (bulk[i] != 0)   //如果桶中有元素
                for (int j = 0; j < bulk[i]; j++) //检查heights[]是否依次有bulk[i]个该元素
                    if (heights[cur++] != i) result++; //从小到大扫描heights数组
        return result; 
    }
};
```
更新：
```cpp
class Solution {
public:
   int heightChecker(vector<int>& heights) {
       int temp[101] = {0};
       for (const int v : heights) ++temp[v];
       int ans = 0, cur = 0;
       for (int i = 0; i < 101; ++i) 
           while (temp[i]-- > 0) 
               if (heights[cur++] != i) ++ans;
       return ans;
   }
};
```
