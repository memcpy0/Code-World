Given an array nums, there is a sliding window of size k which is moving from the very left of the array to the very right. You can only see the k numbers in the window. Each time the sliding window moves right by one position. Return the max sliding window.

**Follow up**: Could you solve it in `linear time`?

Example:
```cpp
Input: nums = [1,3,-1,-3,5,3,6,7], and k = 3
Output: [3,3,5,5,6,7] 
Explanation: 

Window position                Max
---------------               -----
[1  3  -1] -3  5  3  6  7       3
 1 [3  -1  -3] 5  3  6  7       3
 1  3 [-1  -3  5] 3  6  7       5
 1  3  -1 [-3  5  3] 6  7       5
 1  3  -1  -3 [5  3  6] 7       6
 1  3  -1  -3  5 [3  6  7]      7
```

 

**Constraints**:
-  `1 <= nums.length <= 10^5`
 - `-10^4 <= nums[i] <= 10^4`
 -  `1 <= k <= nums.length`

题意：给出一个固定大小的区间，区间从数组头移动到数组尾，返回区间移动过程中的所有**区间内部最大值**组成的向量。

思路1：暴力做法，超出时间限制。

代码1：$\text{O(n}^2\text{)}$ 的时间复杂度。
```cpp
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector<int> ans;
        if (nums.empty()) return ans;
        for (int i = 0; i <= nums.size() - k; ++i) {
            int maxVal = nums[i];
            for (int j = i + 1; j < i + k; ++j) {
                if (nums[j] > maxVal) maxVal = nums[j];
            }
            ans.push_back(maxVal);
        }
        return ans;
    }
};
```
每次都要和区间中的前 `k-1` 个数比较，时间浪费巨大。如果高效排除不可能成为答案的数字，就能够提高效率。

思路2：改进方法一是：使用优先队列+记录数字下标的数组，每次将数字进队，然后取优先队列的队首元素。这将时间提升到 $\text{O(nlogn)}$ ，能够通过。

代码2：
```cpp
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector<int> ans;
        priority_queue<int, vector<int>, less<int> > q;
        map<int, int> mp;
        for (int i = 0; i < nums.size(); ++i) {
            q.push(nums[i]);
            //记录元素的下标, 如果该元素重复出现, 以最大的下标为准
            mp[nums[i]] = i; 
            while (mp[q.top()] + k <= i) q.pop();
            if (i >= k - 1) ans.push_back(q.top()); 
        } 
        return ans;
    }
};
```

思路3：**使用单调队列**，将普通队列和优先队列结合起来，遵循：**先进和优先级越高**越先出队的原则。可以达到 $\text{O(n)}$ 的时间要求。

代码3：
```cpp
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector<int> ans;
        int *st, *idx, front = 0, rear = -1;
        st = new int[nums.size()], idx = new int[nums.size()];
        for (int i = 0; i < nums.size(); ++i) {
            while (front <= rear && nums[i] > st[rear]) --rear;
            st[++rear] = nums[i];
            idx[rear] = i; //记录位于rear的元素的原数组下标
            while (idx[front] + k <= i) ++front; //清除超出范围的首元素
            if (i >= k - 1) ans.push_back(st[front]);
        }
        return ans;
    }
};
```
另外，这里可以改成使用 `deque` 双端队列。

