> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `nums`，其中恰好有两个元素只出现一次，其余所有元素均出现两次。 找出只出现一次的那两个元素。你可以按 **任意顺序** 返回答案。

你必须设计并实现线性时间复杂度的算法且仅使用常量额外空间来解决此问题。

**示例 1：**
```js
输入：nums = [1,2,1,3,2,5]
输出：[3,5]
解释：[5, 3] 也是有效的答案。
```
**示例 2：**
```js
输入：nums = [-1,0]
输出：[-1,0]
```
**示例 3：**
```js
输入：nums = [0,1]
输出：[1,0]
```
**提示：**
- `2 <= nums.length <= 3 * 10^4`
- `-2^31 <= nums[i] <= 2^31 - 1`
- 除两个只出现一次的整数外，`nums` 中的其他数字都出现两次

---
### 解法1 哈希表（额外空间）
朴素的做法是，利用哈希表进行统计，最后将出现一次的元素加入答案。算法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(n)$ ：
```cpp
//Java version
class Solution {
    public int[] singleNumber(int[] nums) {
        Map<Integer, Integer> rec = new HashMap<>();
        for (int i : nums) rec.put(i, rec.getOrDefault(i, 0) + 1);
        int[] ans = new int[2];
        int idx = 0;
        for (int i : nums)
            if (rec.get(i) == 1) ans[idx++] = i;
        return ans;
    }
}
//执行用时：7 ms, 在所有 Java 提交中击败了6.48% 的用户
//内存消耗：39.4 MB, 在所有 Java 提交中击败了14.89% 的用户
```

---
### 解法2 位运算（最优）
利用「除答案以外的元素均出现两次」的特性，我们可以先对 `nums[]` 中所有元素进行异或，得到值 `axorb` ，即为两个答案 `a, b` 的异或值。显然 **`axorb` 必然不为零**（因为两个答案 `a` 和 `b` 必然不相等），此时 `axorb` 二进制表示中第 `i` 位的置位 `1` ，意味着两答案二进制表示中的第 `i` 位互不相同。我们遍历 `nums[]` ，对第 `i` 位为 `0` 或 `1` 的元素分别求异或和（**两个答案必然会被分到不同的组**），这两个异或和即为答案 `a, b` 。

事实上，我们不需要实际计算出 `axorb` 中某个置位的位置，取而代之的，**用 `axorb & (~axorb + 1)` 可以算出 `axorb` 最低置位代表的值**。只是要注意，对 `0x8000 0000` 取反后得到 `0x7FFF FFFF` ，再加一会整型溢出，因此要用 `unsigned int` 。

最终代码如下所示。算法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    vector<int> singleNumber(vector<int>& nums) {
        unsigned axorb = 0;
        for (const int &v : nums) axorb ^= v;
        unsigned diffNum = axorb & (~axorb + 1); // axorb中最低置位1代表的值
        int a = 0, b = 0;
        for (const int &v : nums) { // 将nums分为两组
            if (v & diffNum) a ^= v; // 一组元素diffNum对应的位为1
            else b ^= v; // 另一组元素diffNum对应的位为0
        }
        return {a, b};
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了98.43% 的用户
//内存消耗：9.6 MB, 在所有 C++ 提交中击败了92.26% 的用户
```
