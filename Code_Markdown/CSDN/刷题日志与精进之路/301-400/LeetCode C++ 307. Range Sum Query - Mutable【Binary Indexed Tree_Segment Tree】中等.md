

Given an integer array `nums`, find the sum of the elements between indices `i` and `j` (`i ≤ j`), inclusive.

The `update(i, val)` function modifies `nums` by updating the element at index `i` to val.

**Example:**

```swift
Given nums = [1, 3, 5]

sumRange(0, 2) -> 9
update(1, 2)
sumRange(0, 2) -> 8
```
**Constraints:**
- The array is only modifiable by the `update` function.
-  You may assume the number of calls to `update` and `sumRange` function is distributed evenly.
-  `0 <= i <= j <= nums.length - 1`


题意：给定一个整数数组  `nums`，求出数组从索引 `i` 到 `j`  (`i ≤ j`) 范围内元素的总和，包含 `i, j` 两点。`update(i, val)` 函数可以通过将下标为 `i` 的数值更新为 `val`，从而对数列进行修改。

---
### 解法1 暴力
这里就不写了。

---
### 解法2 树状数组
```cpp
class NumArray {
private:
    int lowbit(int x) { return x & (-x); }
    vector<int> ranges; //树状数组,下标0为虚拟结点
    void add(int x, int d) {
        while (x < ranges.size()) {
            ranges[x] += d;
            x += lowbit(x);
        }
    }
    int sum(int x) {
        int ans = 0;
        while (x) {
            ans += ranges[x];
            x -= lowbit(x);
        }
        return ans;
    }
public:
    NumArray(vector<int>& nums) {
        int n = nums.size(); 
        ranges.resize(n + 1); //必须要n+1大,不然查询nums[n - 1]时会越界
        for (int i = 0; i < n; ++i) add(i + 1, nums[i]); 
    }
    void update(int i, int val) { //O(logn)
        int preval = sum(i + 1) - sum(i); //之前的值
        add(i + 1, val - preval); //修改为当前的值(加上一个差值)
    }
    
    int sumRange(int i, int j) { //O(logn)
        return sum(j + 1) - sum(i);
    }
};
```
运行效率如下：
```cpp
执行用时：56 ms, 在所有 C++ 提交中击败了81.59% 的用户
内存消耗：19.3 MB, 在所有 C++ 提交中击败了36.30% 的用户
```
