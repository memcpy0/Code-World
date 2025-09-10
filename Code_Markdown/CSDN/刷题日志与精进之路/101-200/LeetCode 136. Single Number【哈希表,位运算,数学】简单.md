> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个 **非空** 整数数组 `nums` ，除了某个元素只出现一次以外，其余每个元素均出现两次。找出那个只出现了一次的元素。

你必须设计并实现线性时间复杂度的算法来解决此问题，且该算法只使用常量额外空间。

**示例 1 ：**
```js
输入：nums = [2,2,1]
输出：1
```
**示例 2 ：**
```js
输入：nums = [4,1,2,1,2]
输出：4
```
**示例 3 ：**
```js
输入：nums = [1]
输出：1
```
**提示：**
- `1 <= nums.length <= 3 * 10^4`
- `-3 * 10^4 <= nums[i] <= 3 * 10^4`
- 除了某个元素只出现一次以外，其余每个元素均出现两次。
 
---
### 解法1 哈希表（额外空间）
朴素的做法是，利用哈希表或整型数组（不是很优雅）进行统计，最后返回出现一次的元素。时间复杂度为 $O(2n)$ ，空间复杂度为 $O(n)$ ：
```cpp
//C++ version
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int rec[60010] = {0};
        for (int i = 0, n = nums.size(); i < n; ++i) ++rec[nums[i] + 30000];
        for (int i = 0; i < 60010; ++i) if (rec[i] == 1) return i - 30000; 
        return 0;
    }
};
//执行用时：16 ms, 在所有 C++ 提交中击败了72.90% 的用户
//内存消耗：16.7 MB, 在所有 C++ 提交中击败了12.32% 的用户
```
```java
//Java version
class Solution {
    public int singleNumber(int[] nums) {
        Map<Integer, Integer> rec = new HashMap<>();
        for (int v : nums) rec.put(v, rec.getOrDefault(v, 0) + 1);
        for (int v : rec.keySet())
            if (rec.get(v) == 1) 
                return v;
        return 0;
    }
}
//执行用时：14 ms, 在所有 Java 提交中击败了6.05% 的用户
//内存消耗：39.1 MB, 在所有 Java 提交中击败了5.54% 的用户
```

---
### 解法2 位运算（最优）
最简单的方法是异或。利用「除答案外的元素均出现两次」的特性，对 `nums[]` 中的所有元素执行异或操作，得到的结果就是要求的答案。时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int ans = 0;
        for (auto &i : nums) ans ^= i;
        return ans;
    }
};
//执行用时：8 ms, 在所有 C++ 提交中击败了99.03% 的用户
//内存消耗：16.4 MB, 在所有 C++ 提交中击败了87.84% 的用户
```
另外一个版本的代码，利用了 `Deff's Device` 进行循环展开：
```cpp
//C++ version
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int ans = 0, cnt = nums.size(), n = (cnt + 7) / 8;
        vector<int>::iterator it = nums.begin();
        switch (cnt % 8) {
            case 0: do { ans ^= *it++;
            case 7:      ans ^= *it++;
            case 6:      ans ^= *it++;
            case 5:      ans ^= *it++;
            case 4:      ans ^= *it++;
            case 3:      ans ^= *it++;
            case 2:      ans ^= *it++;
            case 1:      ans ^= *it++;
                    } while (--n > 0);
        }
        return ans;
    }
};
//执行用时：8 ms, 在所有 C++ 提交中击败了99.03% 的用户
//内存消耗：16.4 MB, 在所有 C++ 提交中击败了94.52% 的用户
```
如果用C语言的话：
```c
int singleNumber(int* nums, int numsSize) {
    int ans = 0, n = (numsSize + 7) / 8;
    switch (numsSize % 8) {
        case 0: do { ans ^= *nums++;
        case 7:      ans ^= *nums++;
        case 6:      ans ^= *nums++;
        case 5:      ans ^= *nums++;
        case 4:      ans ^= *nums++;
        case 3:      ans ^= *nums++;
        case 2:      ans ^= *nums++;
        case 1:      ans ^= *nums++;
                } while (--n > 0);
    }
    return ans;
}
//执行用时：16 ms, 在所有 C 提交中击败了80.17% 的用户
//内存消耗：6.9 MB, 在所有 C 提交中击败了95.90% 的用户
```
