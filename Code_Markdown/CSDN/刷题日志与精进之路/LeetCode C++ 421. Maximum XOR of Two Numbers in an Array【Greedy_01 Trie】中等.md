<p>Given an integer array <code>nums</code>, return <em>the maximum result of <code>nums[i] XOR nums[j]</code></em>, where <code>0 ≤ i ≤ j &lt; n</code>.</p>

<p><strong>Follow up:</strong> Could you do this in <code>O(n)</code> runtime?</p>

 
<p><strong>Example 1:</strong></p>

```clike
Input: nums = [3,10,5,25,2,8]
Output: 28
Explanation: The maximum result is 5 XOR 25 = 28.
```
 
<p><strong>Example 2:</strong></p>

```clike
Input: nums = [0]
Output: 0
```
 
<p><strong>Example 3:</strong></p>

```clike
Input: nums = [2,4]
Output: 6
```
 
<p><strong>Example 4:</strong></p>

```clike
Input: nums = [8,10,2]
Output: 10
```
 
<p><strong>Example 5:</strong></p>

```clike
Input: nums = [14,70,53,83,49,91,36,80,92,51,66,70]
Output: 127
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length &lt;= 2 * 10<sup>4</sup></code></li>
	<li><code>0 &lt;= nums[i] &lt;= 2<sup>31</sup> - 1</code></li>
</ul>


题意：给你一个整数数组 `nums` ，返回 `nums[i] XOR nums[j]` 的最大运算结果，其中 `0 ≤ i ≤ j < n` 。你可以在 `O(n)` 的时间解决这个问题吗？

---
### 解法 01字典树+贪心
暴力搜索二元组，计算所有二元组的最大异或值，这种做法的复杂度为 $O(n^2)$ ，绝对会超时。事实上，我们有更好的做法，能够达到 $O(n)$ 的时间复杂度。

具体步骤是，首先创建一棵01字典树，即字母表只有 `0, 1` 两个字母。然后我们依次将 `nums[]` 中的数字按照二进制表示从高位到低位插入到01字典树中。注意，**本题的所有元素都是非负整数**，如果出现了负数就要特别小心。然后遍历 `nums[]` ，对每个元素 `nums[i]` ，**从高位到低位**按照其二进制表示，遍历01字典树——如果存在和当前比特位相反的节点，就选择这条路径；否则选择原始比特位代表的路径。这种贪心策略的正确性在于，优先保障高位异或得到 `1` 。对于二进制数来说，为了得到更大的异或值，高位的 `1` 一定比低位更加重要。

具体代码如下，除了这种实现方法，还可以用数组实现01字典树（见本人的博客[HDU 4825 Xor Sum【01字典树/贪心】(两数最大/最小异或和)](https://memcpy0.blog.csdn.net/article/details/108318897)，这一题与本题除了数据范围和题意以外差不多相同，本题求的是<u>给出一组数，从中选两个数异或求最大值</u>，那一题求的是<u>从一组数中选一个数与给定数异或最大</u>，即本题对每一个 `nums[i]` 都使用那一题的方法）或者用静态的二叉树：
```cpp
class Trie01 {
private:
    Trie01 *next[2];
    int value;
public:
    Trie01() { memset(next, 0, sizeof(next)); value = 0; }
    void insert(int x) {
        Trie01 *cur = this;
        for (int i = 0; i < 32; ++i) {
            int d = ((x >> (31 - i)) & 1);
            if (cur->next[d] == nullptr) cur->next[d] = new Trie01;
            cur = cur->next[d]; 
        }
        cur->value = x;
    }
    int getMaxXorVal(int x) { //返回和x异或得到的值最大的value
        Trie01 *cur = this;
        for (int i = 0; i < 32; ++i) {
            int d = ((x >> (31 - i)) & 1);
            if (cur->next[d ^ 1]) cur = cur->next[d ^ 1]; //异或不同位
            else cur = cur->next[d];
        }
        return cur->value;
    }
};
class Solution {
public:
    int findMaximumXOR(vector<int>& nums) {
        int maxVal = 0, n = nums.size();
        Trie01 tree;
        for (int i = 0; i < n; ++i) tree.insert(nums[i]);
        for (int i = 0; i < n; ++i) {
            int temp = tree.getMaxXorVal(nums[i]);
            if ((nums[i] ^ temp) > maxVal) maxVal = nums[i] ^ temp;
        }
        return maxVal;
    }
};
```
运行效率如下：
```cpp
执行用时：88 ms, 在所有 C++ 提交中击败了72.06% 的用户
内存消耗：35 MB, 在所有 C++ 提交中击败了34.73% 的用户
```


与本题类似的进阶题有： 牛客的[子数组最大异或和](https://www.nowcoder.com/practice/43f62c52fbac47feaeabe40ac1ab9091?tpId=101&tqId=33209&tPage=1&rp=1&ru=/ta/programmer-code-interview-guide&qru=/ta/programmer-code-interview-guide/question-ranking)；LeetCode的[1707. Maximum XOR With an Element From Array](https://leetcode-cn.com/problems/maximum-xor-with-an-element-from-array/)；以及[洛谷P4551 最长异或路径](https://www.luogu.com.cn/problem/P4551)。


