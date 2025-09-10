<p>Given an array of integers <code>arr</code>, sort the array by performing a series of <strong>pancake flips</strong>.</p>

<p>In one pancake flip we do the following steps:</p>

<ul>
	<li>Choose an integer <code>k</code> where <code>1 &lt;= k &lt;= arr.length</code>.</li>
	<li>Reverse the sub-array <code>arr[0...k-1]</code> (<strong>0-indexed</strong>).</li>
</ul>

<p>For example, if <code>arr = [3,2,1,4]</code> and we performed a pancake flip choosing <code>k = 3</code>, we reverse the sub-array <code>[3,2,1]</code>, so <code>arr = [<u>1</u>,<u>2</u>,<u>3</u>,4]</code> after the pancake flip at <code>k = 3</code>.</p>

<p>Return <em>an array of the </em><code>k</code><em>-values corresponding to a sequence of pancake flips that sort </em><code>arr</code>. Any valid answer that sorts the array within <code>10 * arr.length</code> flips will be judged as correct.</p>
 
<p><strong>Example 1:</strong></p>

```cpp
Input: arr = [3,2,4,1]
Output: [4,2,4,3]
Explanation: 
We perform 4 pancake flips, with k values 4, 2, 4, and 3.
Starting state: arr = [3, 2, 4, 1]
After 1st flip (k = 4): arr = [1, 4, 2, 3]
After 2nd flip (k = 2): arr = [4, 1, 2, 3]
After 3rd flip (k = 4): arr = [3, 2, 1, 4]
After 4th flip (k = 3): arr = [1, 2, 3, 4], which is sorted. 
```

<p><strong>Example 2:</strong></p>

```cpp
Input: arr = [1,2,3]
Output: []
Explanation: The input is already sorted, so there is no need to flip anything.
Note that other answers, such as [3, 3], would also be accepted.
```

 
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= arr.length &lt;= 100</code></li>
	<li><code>1 &lt;= arr[i] &lt;= arr.length</code></li>
	<li>All integers in <code>arr</code> are unique (i.e. <code>arr</code> is a permutation of the integers from <code>1</code> to <code>arr.length</code>).</li>
</ul>



题意：给你一个整数数组 <code>arr</code> ，请使用 <strong>煎饼翻转</strong><em> </em>完成对数组的排序。一次煎饼翻转的执行过程如下：

<ul>
	<li>选择一个整数 <code>k</code> ，<code>1 &lt;= k &lt;= arr.length</code></li>
	<li>反转子数组 <code>arr[0...k-1]</code>（<strong>下标从 0 开始</strong>）</li>
</ul>

<p>以数组形式返回能使 <code>arr</code> 有序的煎饼翻转操作所对应的 <code>k</code> 值序列。任何将数组排序且翻转次数在&nbsp;<code>10 * arr.length</code> 范围内的有效答案都将被判断为正确。</p>

---
### 解法 排序
类似冒泡排序的思想，每次先找到剩余数组区间中的最大数组元素，然后通过翻转将其放置在区间末尾，直到整个数组都有序为止。这一做法的翻转次数最多为 `2 * arr.length` ，因为对每个最大数组元素，可能需要先翻转到数组首位，然后翻转到区间尾部。
```cpp
class Solution {
public:
    vector<int> pancakeSort(vector<int>& arr) {
        vector<int> ans;
        for (int i = arr.size() - 1; i >= 0; --i) {
            int maxj = 0;
            for (int j = 1; j <= i; ++j) if (arr[j] > arr[maxj]) maxj = j;
            if (maxj == i) continue; //最大值在末尾
            ans.push_back(maxj + 1); //翻转arr[0,maxj]这一区间,将最大值翻转到数组首位
            reverse(arr.begin(), arr.begin() + maxj + 1);
            ans.push_back(i + 1); //翻转arr[0,i]这一区间,将最大值翻转到i位
            reverse(arr.begin(), arr.begin() + i + 1);
        }
        return ans;
    }
};
```
运行效率如下所示：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了78.95% 的用户
内存消耗：10.8 MB, 在所有 C++ 提交中击败了76.12% 的用户
```
