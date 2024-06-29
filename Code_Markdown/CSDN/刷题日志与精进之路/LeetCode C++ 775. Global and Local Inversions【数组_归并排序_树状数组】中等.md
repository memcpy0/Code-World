<p>You are given an integer array <code>nums</code> of length <code>n</code> which represents a permutation of all the integers in the range <code>[0, n - 1]</code>.</p>

<p>The number of <strong>global inversions</strong> is the number of the different pairs <code>(i, j)</code> where:</p>

<ul>
	<li><code>0 &lt;= i &lt; j &lt; n</code></li>
	<li><code>nums[i] &gt; nums[j]</code></li>
</ul>

<p>The number of <strong>local inversions</strong> is the number of indices <code>i</code> where:</p>

<ul>
	<li><code>0 &lt;= i &lt; n - 1</code></li>
	<li><code>nums[i] &gt; nums[i + 1]</code></li>
</ul>

<p>Return <code>true</code> <em>if the number of <strong>global inversions</strong> is equal to the number of <strong>local inversions</strong></em>.</p>
 
<p><strong>Example 1:</strong></p>

```clike
Input: nums = [1,0,2]
Output: true
Explanation: There is 1 global inversion and 1 local inversion.
```

 
<p><strong>Example 2:</strong></p>

```clike
Input: nums = [1,2,0]
Output: false
Explanation: There are 2 global inversions and 1 local inversion.
```
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>n == nums.length</code></li>
	<li><code>1 &lt;= n &lt;= 5000</code></li>
	<li><code>0 &lt;= nums[i] &lt; n</code></li>
	<li>All the integers of <code>nums</code> are <strong>unique</strong>.</li>
	<li><code>nums</code> is a permutation of all the numbers in the range <code>[0, n - 1]</code>.</li>
</ul>
 

题意：数组 `A` 是 `[0, 1, ..., N - 1]` 的一种排列，`N` 是数组 `A` 的长度。全局倒置指的是 `i, j` 满足 `0 <= i < j < N` 并且 `A[i] > A[j]` ，局部倒置指的是 `i` 满足 `0 <= i < N` 并且 `A[i] > A[i+1]` 。当数组 `A` 中全局倒置的数量等于局部倒置的数量时，返回 `true` 。 


---
全局倒置就是逆序对，局部倒置指的是邻近的两个值形成的逆序对。$O(n^2)$ 暴力计算所有的逆序对数目会超时。

由于局部倒置一定是全局倒置，全局倒置必然包含局部倒置，只要发现存在不相邻的两个值逆序，就必然有全局倒置数目大于局部倒置数目，就返回 `false` 。这种优化的暴力做法也会超时。
```cpp
class Solution {
public:
    bool isIdealPermutation(vector<int>& nums) {
        int n = nums.size(); //all=local+其他的
        for (int i = 0; i < n - 1; ++i)
            for (int j = i + 2; j < n; ++j) if (nums[i] > nums[j]) return false;
        return true;
    }
};
```
### 解法1 归并排序
用归并排序的写法可以计算全局倒置（即逆序对）的数目，不过无法计算局部倒置的数目，因为实际排序后元素的下标位置会变动。我们需要先 $O(n)$ 计算局部倒置的数目，然后再 $O(n\log n)$ 归并计算全局倒置的数目。比较两者即可：
```cpp
class Solution {
private:
    int local = 0, all = 0;
    void mergeSortAndCount(vector<int>& nums, vector<int>& temp, int l, int r) {
        if (l < r) {
            int mid = l + (r - l) / 2;
            mergeSortAndCount(nums, temp, l, mid);
            mergeSortAndCount(nums, temp, mid + 1, r);
            int i = l, j = mid + 1, k = l;
            while (i <= mid && j <= r) {
                if (nums[i] > nums[j]) { //i<j,是逆序对
                    all += mid - i + 1; //对于nums[j]来说大于nums[j]的有这些元素
                    temp[k++] = nums[j++];
                } else temp[k++] = nums[i++];
            }
            while (i <= mid) temp[k++] = nums[i++];
            while (j <= r) temp[k++] = nums[j++];
            for (int x = l; x <= r; ++x) nums[x] = temp[x];
        }
    }
public:
    bool isIdealPermutation(vector<int>& nums) {
        int n = nums.size(); //计算局部倒置
        for (int i = 0; i < n - 1; ++i) if (nums[i] > nums[i + 1]) ++local; 
        vector<int> temp(n); 
        mergeSortAndCount(nums, temp, 0, n - 1); //计算全局倒置 
        return local == all;
    }
};  
```
运行效率如下：
```cpp
执行用时：108 ms, 在所有 C++ 提交中击败了11.04% 的用户
内存消耗：36.3 MB, 在所有 C++ 提交中击败了9.09% 的用户
```

---
### 解法2 离散化+树状数组
由于元素范围没有说明，因此这里先离散化一波……然后就是熟悉的树状数组求逆序对数目的过程了：
```cpp
#define lowbit(x) ((x) & -(x))
const int maxn = 5010;
int tree[maxn] = {0};
void add(int i, int d) {
    while (i < maxn) {
        tree[i] += d;
        i += lowbit(i);
    }
}
int sum(int i) {
    int ans = 0;
    while (i) {
        ans += tree[i];
        i -= lowbit(i);
    }
    return ans;
}
class Solution {
private: 
    int local = 0, all = 0; 
public:
    bool isIdealPermutation(vector<int>& nums) {
        int n = nums.size(); //计算局部倒置
        for (int i = 0; i < n - 1; ++i) if (nums[i] > nums[i + 1]) ++local;  
        vector<int> temp(nums); //离散化
        sort(temp.begin(), temp.end()); //排序O(nlogn)
        int len = unique(temp.begin(), temp.end()) - temp.begin(); //O(n)
        memset(tree, 0, sizeof(tree));
        for (int i = 0; i < n; ++i) { //得到新的数组O(nlogn) //从1开始
            int v = lower_bound(temp.begin(), temp.begin() + len, nums[i]) - temp.begin() + 1; 
            all += i - sum(v); //前面i个元素-小于等于v的元素数目=大于v的元素数目(即全局倒置)
            add(v, 1); 
        }
        return local == all;
    }
}; 
```
运行效率如下：
```cpp
执行用时：116 ms, 在所有 C++ 提交中击败了10.39% 的用户
内存消耗：36.3 MB, 在所有 C++ 提交中击败了9.09% 的用户
```
---
### 解法3 判断有无不连续的全局倒置
我们前面的两种做法，都实际求出了全局倒置和局部倒置的数目，**但是这在本题中都是不必要的**。前面在优化的暴力中，寻找有无不连续的全局倒置，思想是完全正确且切合本题的，只是实现上可以优化——我们顺序计算当前位置 `i` 的前面 `i - 2` 个元素的最大值 `mx` ，如果 `mx > nums[i]` ，则意味着不连续的全局倒置的存在，于是返回 `false` 。代码也很简单：
```cpp
class Solution {
public:
    bool isIdealPermutation(vector<int>& nums) {
        int n = nums.size(), mx = nums[0];
        for (int i = 2; i < n; ++i) {
            if (mx > nums[i]) return false;
            mx = max(mx, nums[i - 1]);
        }
        return true;
    }
};
```
运行效率如下：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了71.43% 的用户
内存消耗：34.8 MB, 在所有 C++ 提交中击败了22.72% 的用户
```
