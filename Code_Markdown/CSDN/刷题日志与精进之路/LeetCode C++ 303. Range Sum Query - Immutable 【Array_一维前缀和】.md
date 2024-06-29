Given an integer array nums, find the sum of the elements between indices i and j (i ≤ j), inclusive.

**Example:**
```css
Given nums = [-2, 0, 3, -5, 2, -1]

sumRange(0, 2) -> 1
sumRange(2, 5) -> -1
sumRange(0, 5) -> -3
```
**Note:**

-    You may assume that the array does not change.
 -   There are many calls to sumRange function.

题意：题目给了一个数组，多次询问一个区间的数据和。

---
### 解法1 暴力 
暴力法。区间最大可能为该数组的大小，因此多次调用复杂度为O(mn)，很高。我一看题目，用C++写了个，没想到的是就过了...

```cpp
class NumArray {
public:
   vector<int> arr;
   NumArray(vector<int>& nums) {   
   	arr = nums; 
   }
   
   int sumRange(int i, int j) {  
       int sum = 0;
       for (; i <= j; i++) sum += arr[i];
       return sum; 
   }
};

/**
* Your NumArray object will be instantiated and called as such:
* NumArray* obj = new NumArray(nums);
* int param_1 = obj->sumRange(i,j);
*/
```
运行效率如下：
```cpp
 执行用时：408 ms, 在所有 C++ 提交中击败了5.57% 的用户 
 内存消耗：17.2 MB, 在所有 C++ 提交中击败了44.35%的用户
 ```
 
--- 
### 解法2 前缀和 
本来想使用线段树的，但是太麻烦了，想这是道简单题...磨蹭了一会才想起来，可以用区间和/前缀和的方法做。处理出区间和数组O(n)，每次询问用时O(1)，因此m次询问为O(m)，大大减少了时间，从结果看，降低了二十倍。
     ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200215183127313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

```cpp
class NumArray {
public:
/* -2,0,3,-5,2,-1 原始数据
       * 0,-2,-2,1,-4,-2,-3 和数组 
       * 每一项都是原始数组该项之前的所有数据之和 [,)
       * 
       * [0, 2] = sums[2 + 1] - sum[0] = 1
       * [2, 5] = sums[5 + 1] - sums[2] = -3 + 2 = -1
       * [0, 5] = sums[5 + 1] - sum[0] = -3
       */
   vector<int> arr;
   NumArray(vector<int>& nums) {   
       int sum = 0;
       for (int i = 0; i < nums.size(); ++i) {
           sums.push_back(sum); 
           sum += nums[i];  
       }
       sums.push_back(sum);
   }
   
   int sumRange(int i, int j) { // [i, j]
       return sums[j + 1] - sums[i]; 
   }
}; 
```
运行效率如下：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了98.16% 的用户
内存消耗：17.2 MB, 在所有 C++ 提交中击败了35.93%的用户
```
---
### 解法3 STL
使用STL的 `partial_sum` 函数：
```cpp
class NumArray {
private:
    vector<int> presum;
public:
    NumArray(vector<int>& nums) {
        presum.resize(nums.size(), 0);
        partial_sum(nums.begin(), nums.end(), presum.begin());
    }
    
    int sumRange(int i, int j) {
        return i ? presum[j] - presum[i - 1] : presum[j];
    }
}; 
```
运行效率如下：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了89.94% 的用户
内存消耗：16.5 MB, 在所有 C++ 提交中击败了95.03% 的用户
```
