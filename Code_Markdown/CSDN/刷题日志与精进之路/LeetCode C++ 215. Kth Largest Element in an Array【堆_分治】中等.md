Find the `kth` largest element in an unsorted array. Note that it is the `kth` largest element in the sorted order, not the kth **distinct** element.

Example 1:

```cpp
Input: [3,2,1,5,6,4] and k = 2
Output: 5
```

Example 2:

```cpp
Input: [3,2,3,1,2,4,5,5,6] and k = 4
Output: 4
```

Note: You may assume `k` is always valid, `1 ≤ k ≤ array's length` .

题意：求出数组中第 `k` 大的数字，不是第 `k` 大的独特的数字，说明重复的数字也会占据序次。

思路1：排序。

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end(), greater<int>());
        return nums[k - 1];
    }
};
```
思路2：优先队列(堆排序)。
```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        priority_queue<int, vector<int>, less<int>> pq;
        for (const auto &i : nums) pq.push(i);
        int ans; 
        for (int i = 0; i < k; ++i) {
            ans = pq.top();
            pq.pop();
        }
        return ans;
    }
};
```

思路3：利用快速排序的分治策略。如果枢纽元是第 `k` 个数的话，直接返回，否则递归找到第 `k` 大的数。
```cpp
class Solution {
public:
    int partition(vector<int>& nums, int l, int r) {        //划分函数
        if (l >= r) return l;
        int t = nums[l];
        while (l < r) {
            while (l < r && nums[r] < t) --r;
            nums[l] = nums[r];
            while (l < r && nums[l] >= t) ++l; 
            nums[r] = nums[l];
        }
        nums[l] = t;
        return l;
    }
    int select(vector<int>& nums, int l, int r, int k) {
        if (l == r) return nums[l];                          //递归边界
        int p = partition(nums, l, r);                       //划分后主元的位置
        int m = p - l + 1;                                   //nums[p]在[l,r]中的位置
        if (k == m) return nums[p];                          //说明确定的主元位置正好是k
        else if (k < m) return select(nums, l, p - 1, k);    //第k大的数在左侧
        else return select(nums, p + 1, r, k - m);           //第k大的数在右侧
    }
    int findKthLargest(vector<int>& nums, int k) {
        int kth = select(nums, 0, nums.size() - 1, k);
        return kth;
    }
};
```


