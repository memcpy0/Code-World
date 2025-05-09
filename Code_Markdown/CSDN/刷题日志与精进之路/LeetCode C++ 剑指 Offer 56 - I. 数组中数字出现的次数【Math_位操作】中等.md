一个整型数组 `nums` 里除两个数字之外，其他数字都出现了两次。请写程序找出这两个只出现一次的数字。要求时间复杂度是 $O(n)$ ，空间复杂度是 $O(1)$ 。

**示例 1：**
```cpp
输入：nums = [4,1,4,6]
输出：[1,6] 或 [6,1]
```
**示例 2：**
```cpp
输入：nums = [1,2,10,4,1,4,3,3]
输出：[2,10] 或 [10,2]
```
**限制：** `2 <= nums.length <= 10000`


题意：这道题是**一个数组只有一个数字出现了一次**的拓展。那道题应用**异或运算**：任何一个数字异或自己都等于零。于是从头到尾异或数组中的每个数字，那么最后的结果刚好是那个只出现一次的数字，**其他成对出现的数字都在异或中抵销了**。

既然如何，我们试着将原数组分成两个子数组，让每个子数组中包含一个只出现一次的数字，而其他数字都成对出现两次。如果能够这样拆分两个数组，就可以沿用前面的方法，**分别找出两个只出现一次的数字**。

为此，依次异或数组中的每个数字，最后得到的**肯定是两个只出现一次的数字的异或结果**，因为其他数字都出现了两次，在异或中被抵消了。由于存在两个数字作为结果，它们肯定不一样，那么它们的异或值二进制表示中至少有一位为 $1$ 。于是在结果数字中找到第一个为 $1$ 的位的位置，记为第 $k$ 位。注意：两个只出现一次的数字，它们在第 $k$ 位肯定不同，于是可以作为区分。

现在以第 $k$ 位是否为 $1$ 为标准，将原数组中的数字分成两个数组——第一个数组中每个数字的第 $k$ 位都是 $1$ ，第二个数组中的每个数字的第 $k$ 位都是 $0$ 。于是，**出现了两次的数字肯定被分配到同一个子数组**，因为两个相同的数字它们的任意一位都是相同的；**而出现了一次的两个数字被分配到两个子数组中**。此后，沿用之前的解法，就可以解决本问题了。

举例如 `{2, 4, 3, 6, 3, 2, 5, 5}` ，依次异或后得到的结果二进制表示是 `0010` ，于是倒数第二位是 `1` 。根据数字的倒数第二位进行划分，得到两个子数组： `{2, 3, 6, 3, 2}` 中所有数字的倒数第二位都是 `1` ；`{4, 5, 5}` 中所有数字的倒数第二位都是 `0` 。接下来分别异或就可以了。

代码如下：
```cpp
class Solution {
public:
    vector<int> singleNumbers(vector<int>& nums) {
        if (nums.size() == 2) return nums;
        int result = 0, first1 = 0;
        for (const int v : nums) result ^= v; 
        while (!(result & 1)) {
            result >>= 1;
            ++first1;
        }
        vector<int> ans(2, 0);
        for (const int v : nums) {
            if ((v >> first1) & 1)
                ans[0] ^= v;
            else 
                ans[1] ^= v;
        }
        return ans;
    }
};
```
