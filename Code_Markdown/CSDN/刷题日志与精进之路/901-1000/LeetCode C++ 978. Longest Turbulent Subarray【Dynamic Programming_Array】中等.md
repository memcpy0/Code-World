<p>Given an integer array <code>arr</code>, return <em>the length of a maximum size turbulent subarray of</em> <code>arr</code>.</p>

<p>A subarray is <strong>turbulent</strong> if the comparison sign flips between each adjacent pair of elements in the subarray.</p>

<p>More formally, a subarray <code>[arr[i], arr[i + 1], ..., arr[j]]</code> of <code>arr</code> is said to be turbulent if and only if:</p>


- For `i <= k < j`:
	 - arr[k] > arr[k + 1] when k is odd, and
 	- arr[k] < arr[k + 1] when k is even.
- Or, for `i <= k < j` :
    - `arr[k] > arr[k + 1]` when k is even, and
    - `arr[k] < arr[k + 1]` when k is odd.

 
<p><strong>Example 1:</strong></p>

```swift
Input: arr = [9,4,2,10,7,8,8,1,9]
Output: 5
Explanation: arr[1] > arr[2] < arr[3] > arr[4] < arr[5]
```

 
<p><strong>Example 2:</strong></p>

```swift
Input: arr = [4,8,12,16]
Output: 2 
```

<p><strong>Example 3:</strong></p>

```swift
Input: arr = [100]
Output: 1 
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= arr.length &lt;= 4 * 10<sup>4</sup></code></li>
	<li><code>0 &lt;= arr[i] &lt;= 10<sup>9</sup></code></li>
</ul>

题意：如果比较符号在子数组中的每个相邻元素对之间翻转，则该子数组是湍流子数组。返回 `A` 的最大湍流子数组的长度。

---
### 解法 动态规划
这题应该算是**376. 摆动序列**的变形，只不过此题要求的是连续子数组。`dp[i][0]` 表示以 `A[i]` 为上升结尾的最长湍流子数组的长度，`dp[i][1]` 表示以 `A[i]` 为下降结尾的最长湍流子数组的长度：
```cpp
class Solution {
public:
    int maxTurbulenceSize(vector<int>& A) {
        int n = A.size(), ans = 1;
        vector<vector<int>> dp(n, vector<int>(2));
        //dp[i][0]表示以A[i]作为上升结尾的最长湍流子数组的长度
        //dp[i][1]表示以A[i]作为下降结尾的最长湍流子数组的长度        
        dp[0][0] = dp[0][1] = 1;
        for (int i = 1; i < n; ++i) {
            if (A[i] > A[i - 1]) { //从A[i-1]到A[i]上升
                dp[i][0] = dp[i - 1][1] + 1;
                dp[i][1] = 1; //以A[i]为下降结尾的最长湍流子数组长度为1
            } else if (A[i] < A[i - 1]) { //从A[i-1]到A[i]下降
                dp[i][0] = 1; //以A[i]为上升结尾的最长湍流子数组长度为1
                dp[i][1] = dp[i - 1][0] + 1;
            } else dp[i][0] = dp[i][1] = 1; //A[i-1]==A[i]
            ans = max(ans, max(dp[i][0], dp[i][1]));
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：300 ms, 在所有 C++ 提交中击败了14.70% 的用户
内存消耗：63.5 MB, 在所有 C++ 提交中击败了5.03% 的用户
```
状态压缩，空间优化一下：
```cpp
class Solution {
public:
    int maxTurbulenceSize(vector<int>& A) {
        int n = A.size(), up = 1, down = 1, ans = 1;
        for (int i = 1; i < n; ++i) {
            if (A[i] > A[i - 1]) up = down + 1, down = 1;
            else if (A[i] < A[i - 1]) down = up + 1, up = 1;
            else up = down = 1;
            ans = max(ans, max(up, down));
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：156 ms, 在所有 C++ 提交中击败了81.03% 的用户
内存消耗：39.9 MB, 在所有 C++ 提交中击败了44.85% 的用户
```
对应的Java代码如下：
```java
class Solution {
    public int maxTurbulenceSize(int[] arr) {
        int n = arr.length;
        int[] inc = new int[n + 1]; //inc[i]表示arr[i]相对于前者上升的、以arr[i]结尾的最长湍流子数组长度
        int[] dec = new int[n + 1]; //dec[i]表示arr[i]相对于前者下降的、以arr[i]结尾的最长湍流子数组长度
        inc[0] = dec[0] = 1;
        int ans = 1;
        for (int i = 1; i < n; ++i) {
            if (arr[i] > arr[i - 1]) {
                inc[i] = dec[i - 1] + 1;
                dec[i] = 1;
            } else if (arr[i] < arr[i - 1]) {
                dec[i] = inc[i - 1] + 1;
                inc[i] = 1;
            } else inc[i] = dec[i] = 1;
            ans = Math.max(ans, Math.max(inc[i], dec[i]));
        }
        return ans;
    }
}
```
运行效率如下：
```java
执行用时：7 ms, 在所有 Java 提交中击败了39.54% 的用户
内存消耗：40.9 MB, 在所有 Java 提交中击败了98.87% 的用户
```
