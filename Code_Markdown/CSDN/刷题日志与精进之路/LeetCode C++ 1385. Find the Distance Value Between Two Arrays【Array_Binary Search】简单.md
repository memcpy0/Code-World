

Given two integer arrays `arr1` and `arr2`, and the integer `d`, return the **distance** value between the two arrays.

The **distance** value is defined as **the number of elements** `arr1[i]` such that there is not any element `arr2[j]` where `|arr1[i]-arr2[j]| <= d`.

 

**Example 1:**

```swift
Input: arr1 = [4,5,8], arr2 = [10,9,1,8], d = 2
Output: 2
Explanation: 
For arr1[0]=4 we have: 
|4-10|=6 > d=2 
|4-9|=5 > d=2 
|4-1|=3 > d=2 
|4-8|=4 > d=2 
For arr1[1]=5 we have: 
|5-10|=5 > d=2 
|5-9|=4 > d=2 
|5-1|=4 > d=2 
|5-8|=3 > d=2
For arr1[2]=8 we have:
|8-10|=2 <= d=2
|8-9|=1 <= d=2
|8-1|=7 > d=2
|8-8|=0 <= d=2
```

**Example 2:**

```swift
Input: arr1 = [1,4,2,3], arr2 = [-4,-3,6,10,20,30], d = 3
Output: 2
```

**Example 3:**

```swift
Input: arr1 = [2,1,100,3], arr2 = [-5,-2,10,-3,7], d = 6
Output: 1
```

 

**Constraints:**
- `1 <= arr1.length, arr2.length <= 500`
- `-10^3 <= arr1[i], arr2[j] <= 10^3`
-  `0 <= d <= 100`

题意：两个整数数组 `arr1` ，`arr2` 和一个整数 `d` ，返回两个数组之间的**距离值**。「距离值」定义为符合此距离要求的**元素数目**：对于元素 `arr1[i]` ，不存在任何元素 `arr2[j]` 满足 `|arr1[i]-arr2[j]| <= d` 。


---
### 解法1 暴力
暴力双重循环：
```cpp
class Solution {
public:
    int findTheDistanceValue(vector<int>& arr1, vector<int>& arr2, int d) {
        int dist = 0, n = arr1.size(), m = arr2.size();
        for (int i = 0; i < n; ++i) {
            bool flag = true;
            for (int j = 0; j < m; ++j) {
                if (abs(arr1[i] - arr2[j]) <= d) {
                    flag = false;
                    break;
                }
            }
            if (flag) ++dist;
        }
        return dist;
    }
};
```
运行效率如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了93.61% 的用户
内存消耗：13.2 MB, 在所有 C++ 提交中击败了18.77% 的用户
```
或者写成下面这种，不使用标记：
```cpp
class Solution {
public:
    int findTheDistanceValue(vector<int>& arr1, vector<int>& arr2, int d) {
        int result = arr1.size();
        for (const int &u : arr1) { 
            for (const int &v : arr2) {
                if (abs(u - v) <= d) {
                    --result; //符合条件的值减少一个
                    break;
                }
            } 
        }
        return result;
    }
};
```
---
### 解法2 二分
为了二分，需要对 `arr2[]` 进行排序。然后对于 `arr1[]` 的每个值 `v` ，都在 `arr2[]` 中找到最接近 `v` 的元素值——如果其差的绝对值都 `> d` ，则 `arr2[]` 其他所有元素值也一定符合距离要求；否则不符合距离要求：
```cpp
class Solution {
private:
    bool isAbove(vector<int>& arr, int v, int d) { //二分,找到最接近v的值,计算差的绝对值
        int lo = 0, hi = arr.size() - 1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (abs(v - arr[mid]) <= d) return false; //差的绝对值<=d -> false
            if (arr[mid] < v) lo = mid + 1; //arr[mid]太小
            else hi = mid - 1; //arr[mid]太大
        }
        return true; //最接近v的值,相互之差的绝对值都>d -> true
    }
public:
    int findTheDistanceValue(vector<int>& arr1, vector<int>& arr2, int d) { 
        sort(arr2.begin(), arr2.end()); //令arr2有序
        int count = 0;
        for (const int &v : arr1) 
            if (isAbove(arr2, v, d)) //如果v和arr2中的所有元素的差的绝对值都>d
                ++count;
        return count;
    }
};
```
运行结果如下：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了54.94% 的用户
内存消耗：13.1 MB, 在所有 C++ 提交中击败了29.57% 的用户
```
