在数组中的两个数字，如果前面一个数字大于后面的数字，则这两个数字组成一个逆序对。输入一个数组，求出这个数组中的逆序对的总数。

**示例 1:**
```swift
输入: [7,5,6,4]
输出: 5
```
**限制：** `0 <= 数组长度 <= 50000`

---
### 解法1 归并排序
先这么写，日后有时间就多更新几种解法：
```cpp
class Solution {
private:
    int ans = 0;
    vector<int> tmp;
    void mergeSort(vector<int>& nums, int l, int r) {
        if (l >= r) return;
        int m = l + (r - l) / 2;
        mergeSort(nums, l, m);
        mergeSort(nums, m + 1, r);
        int i = l, j = m + 1, k = l;
        while (i <= m && j <= r) {
            if (nums[i] <= nums[j]) tmp[k++] = nums[i++];
            else {
                tmp[k++] = nums[j++];
                ans += j - k;
            }
        }
        while (i <= m) tmp[k++] = nums[i++];
        while (j <= r) tmp[k++] = nums[j++];
        for (int x = l; x <= r; ++x) nums[x] = tmp[x];
    }
public:
    int reversePairs(vector<int>& nums) {
        if (nums.empty()) return 0;
        tmp.resize(nums.size());
        mergeSort(nums, 0, nums.size() - 1);
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：256 ms, 在所有 C++ 提交中击败了92.20% 的用户
内存消耗：43.9 MB, 在所有 C++ 提交中击败了47.48% 的用户
```
类似的题目还有：LeetCode 315，327，493。
