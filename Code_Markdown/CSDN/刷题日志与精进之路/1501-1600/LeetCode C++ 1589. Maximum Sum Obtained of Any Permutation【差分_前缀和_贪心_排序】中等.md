有一个整数数组 `nums` ，和一个查询数组 `requests` ，其中 <code>requests[i] = [start<sub>i</sub>, end<sub>i</sub>]</code> 。第 `i` 个查询求 <code>nums[start<sub>i</sub>] + nums[start<sub>i</sub> + 1] + ... + nums[end<sub>i</sub> - 1] + nums[end<sub>i</sub>]</code> 的结果 ，<code>start<sub>i</sub></code> 和 <code>end<sub>i</sub></code> 数组索引都是 **从 `0` 开始** 的。

你可以任意排列 `nums` 中的数字，请你返回所有查询结果之和的最大值。由于答案可能会很大，请你将它对 <code>10<sup>9</sup> + 7</code> **取余** 后返回。

 

**示例 1：**
```cpp
输入：nums = [1,2,3,4,5], requests = [[1,3],[0,1]]
输出：19
解释：一个可行的 nums 排列为 [2,1,3,4,5]，并有如下结果：
requests[0] -> nums[1] + nums[2] + nums[3] = 1 + 3 + 4 = 8
requests[1] -> nums[0] + nums[1] = 2 + 1 = 3
总和为：8 + 3 = 11。
一个总和更大的排列为 [3,5,4,2,1]，并有如下结果：
requests[0] -> nums[1] + nums[2] + nums[3] = 5 + 4 + 2 = 11
requests[1] -> nums[0] + nums[1] = 3 + 5  = 8
总和为： 11 + 8 = 19，这个方案是所有排列中查询之和最大的结果。
```

**示例 2：**
```cpp
输入：nums = [1,2,3,4,5,6], requests = [[0,1]]
输出：11
解释：一个总和最大的排列为 [6,5,4,3,2,1] ，查询和为 [11]。
```

**示例 3：**
```cpp
输入：nums = [1,2,3,4,5,10], requests = [[0,2],[1,3],[1,1]]
输出：47
解释：一个和最大的排列为 [4,10,5,3,2,1] ，查询结果分别为 [19,18,10]。
```

 

**提示：**
<ul>
	<li><code>n == nums.length</code></li>
	<li><code>1 &lt;= n &lt;= 10<sup>5</sup></code></li>
	<li><code>0 &lt;= nums[i]&nbsp;&lt;= 10<sup>5</sup></code></li>
	<li><code>1 &lt;= requests.length &lt;=&nbsp;10<sup>5</sup></code></li>
	<li><code>requests[i].length == 2</code></li>
	<li><code>0 &lt;= start<sub>i</sub>&nbsp;&lt;= end<sub>i</sub>&nbsp;&lt;&nbsp;n</code></li>
</ul>

题意：重新排列数组，得到所有查询结果之和的最大值。


---
### 解法 差分+前缀和+贪心+排序
这一道题不太好想，不过仔细思考可知，**如果我们把所有查询区间进行统计，就可以得到每个位置出现的次数。** 只要我们把出现次数最多的位置分配给最大的数，次多的位置分配给次大的数……就可以得到所有查询结果之和的最大值。也就是依次将**由大到小的数值**分给**由大到小的出现次数**。例如：

```cpp
输入：nums = [1,2,3,4,5], requests = [[1,3],[0,1]]
位置 出现次数
0    1
1    2
2    1
3    1
于是将位置1分配给数组中最大的数字5, 位置0、2、3依次分配给数组中的数字4、3、2, 位置5分配给1
得到: [4,5,3,2,1], 所有查询结果之和为: 4*1 + 5*2 + 3*1 + 2*1 = 19
```
代码如下，注意：查询数组长度可能有 $10^5$ ，每个查询区间的范围也可能很大，**为此使用差分+前缀和进行统计**。
```cpp
#define LL long long
const int MAXN = 1e5 + 10;
const int MOD = 1e9 + 7;
int cnt[MAXN];
class Solution {
public:
    int maxSumRangeQuery(vector<int>& nums, vector<vector<int>>& requests) {
        int n = nums.size();
        for (int i = 0; i <= n; ++i) cnt[i] = 0; //cnt清空
        for (const vector<int>& r : requests) { //0<=r[0]<=r[1]<n, 差分数组
            ++cnt[r[0]];
            --cnt[r[1] + 1];
        }
        for (int i = 1; i < n; ++i) cnt[i] += cnt[i - 1]; //前缀和,把原数组恢复过来 
        sort(nums.begin(), nums.end()); //两个数组排序
        sort(cnt, cnt + n);
        
        LL ans = 0; //对乘加即可
        for (int i = 0; i < n; ++i) ans = (ans + 1LL * nums[i] * cnt[i]) % MOD;
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：912 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：93.7 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
