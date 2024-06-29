> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091350057.png)

给你一个数组 `nums` 和一个值 `val`，你需要 **[原地](https://baike.baidu.com/item/%E5%8E%9F%E5%9C%B0%E7%AE%97%E6%B3%95)** 移除所有数值等于 `val` 的元素，并返回移除后数组的新长度。

不要使用额外的数组空间，你必须仅使用 `O(1)` 额外空间并 **[原地](https://baike.baidu.com/item/%E5%8E%9F%E5%9C%B0%E7%AE%97%E6%B3%95) 修改输入数组**。

元素的顺序可以改变。你不需要考虑数组中超出新长度后面的元素。

**说明:**

为什么返回数值是整数，但输出的答案是数组呢?

请注意，输入数组是以**「引用」**方式传递的，这意味着在函数里修改输入数组对于调用者是可见的。

你可以想象内部操作如下:
```java
// nums 是以“引用”方式传递的。也就是说，不对实参作任何拷贝
int len = removeElement(nums, val);

// 在函数里修改输入数组对于调用者是可见的。
// 根据你的函数返回的长度, 它会打印出数组中 该长度范围内 的所有元素。
for (int i = 0; i < len; i++) {
    print(nums[i]);
}
```
**示例 1：**
```java
输入：nums = [3,2,2,3], val = 3
输出：2, nums = [2,2]
```
解释：函数应该返回新的长度 **2**, 并且 nums 中的前两个元素均为 **2**。你不需要考虑数组中超出新长度后面的元素。例如，函数返回的新长度为 2 ，而 nums = [2,2,3,3] 或 nums = [2,2,0,0]，也会被视作正确答案。

**示例 2：**
```java
输入：nums = [0,1,2,2,3,0,4,2], val = 2
输出：5, nums = [0,1,4,0,3]
```
解释：函数应该返回新的长度 **`5`**, 并且 nums 中的前五个元素为 **`0`**, **`1`**, **`3`**, **`0`**, **4**。注意这五个元素可为任意顺序。你不需要考虑数组中超出新长度后面的元素。

**提示：**
- `0 <= nums.length <= 100`
- `0 <= nums[i] <= 50`
- `0 <= val <= 100`

---
思路：根据不同的题目要求，我们可以有不同的解法。比如说：如何定义删除——从数组中去除？放到原数组末尾？剩余元素的顺序要不要保证？是否有空间复杂度和时间复杂度的要求？...... 当然，不管怎么样，对于这题，实际删除元素都是不必要的。

方法一：用O(N)空间，O(N)时间。创建一个新数组，将非val的元素放进去，然后复制回nums数组，返回新的size()。
  - 执行用时 :4 ms, 在所有 C++ 提交中击败了82.03% 的用户
     内存消耗 :11.3 MB, 在所有 C++ 提交中击败了5.01%的用户
```cpp
class Solution {
public:   
    int removeElement(vector<int>& nums, int val) {
        if (nums.empty()) return 0;
        vector<int> t;
        for (const auto i : nums) 
            if (i != val) t.push_back(i);
        nums = t;
        return nums.size();  
    }
}; 
```
---
方法一：双指针
思路及算法

由于题目要求删除数组中等于 val\textit{val}val 的元素，因此输出数组的长度一定小于等于输入数组的长度，我们可以把输出的数组直接写在输入数组上。可以使用双指针：右指针 right\textit{right}right 指向当前将要处理的元素，左指针 left\textit{left}left 指向下一个将要赋值的位置。

如果右指针指向的元素不等于 val\textit{val}val，它一定是输出数组的一个元素，我们就将右指针指向的元素复制到左指针位置，然后将左右指针同时右移；

如果右指针指向的元素等于 val\textit{val}val，它不能在输出数组里，此时左指针不动，右指针右移一位。

整个过程保持不变的性质是：区间 [0,left)[0,\textit{left})[0,left) 中的元素都不等于 val\textit{val}val。当左右指针遍历完输入数组以后，left\textit{left}left 的值就是输出数组的长度。

这样的算法在最坏情况下（输入数组中没有元素等于 val\textit{val}val），**左右指针各遍历了数组一次**。
 
方法二：双指针——删除的元素比较多时：由于题目中要求不使用额外的空间，元素的顺序不用保证，而且**超出新的数组长度的值不被考虑**。所以我们连把val值放到数组后面都不必了。不用像[Move Zeroes](https://blog.csdn.net/myRealization/article/details/104425589)这题一样还要把0放到数组后面。因此，我们综合考虑，可以**用一个指针k标明非val值区间的下一个元素，同时也能记录非val值的个数**，再用一个遍历数组的快指针i，每次遇到一个非val的值都将其写到nums[k]中，k++，非val值的区间扩展，然后i++，继续遍历。O(N)时间， O(1)空间。**最多可能遍历2N步**（全是非val的元素，i和k加起来）。
- 执行用时 :0 ms, 在所有 C++ 提交中击败了100.00% 的用户
   内存消耗 :11.1 MB, 在所有 C++ 提交中击败了5.01%的用户
```cpp
class Solution {
public:   
    int removeElement(vector<int>& nums, int val) {  
        int k = 0;  //标记非val值区间[0, k)的下一个元素
        for (int i = 0; i < nums.size(); ++i) 
            if (nums[i] != val) nums[k++] = nums[i];
        return k; 
    }
};  
```
如果整个数组都没有非val的元素，或者要删除的元素很少，那么还是需要写入O(N)次，做不必要的复制操作。比如说：`nums = [1, 2, 3, 4, 5], val = 6`。可以稍微优化一下，只有`i != k`的时候才写入。 
```cpp
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {     
        int k = 0; //标记非val值区间[0, k)的下一个元素, 同时也记录个数
        for (int i = 0; i < nums.size(); ++i) 
            if (nums[i] != val) {
                if (i != k) nums[k] = nums[i];
                ++k;  //记录非val元素的个数
            }
        return k;
     }
};        
```

不过，==这种优化不是甚麽时候都管用==。一个例子是 `num=[4，1，2，3，5]，Val=4`。即使要删除的只有第一个，也要将 [1，2，3，5] 这几个元素左移一步。[Move Zeroes](https://blog.csdn.net/myRealization/article/details/104425589)那里没提，但是写的也是`i != k`时才交换...那道题需要保持元素顺序，所以虽然有时候不太好，但也是最优的方案了。而这题，还可以更加优化。

其实是两种思路，一种是判断元素值与目标值相等。另一种是判断元素值与目标值不相等。

---
方法二：双指针优化
思路

如果要移除的元素恰好在数组的开头，例如序列 [1,2,3,4,5][1,2,3,4,5][1,2,3,4,5]，当 val\textit{val}val 为 111 时，我们需要把每一个元素都左移一位。注意到题目中说：「元素的顺序可以改变」。实际上我们可以直接将最后一个元素 555 移动到序列开头，取代元素 111，得到序列 [5,2,3,4][5,2,3,4][5,2,3,4]，同样满足题目要求。这个优化在序列中 val\textit{val}val 元素的数量较少时非常有效。

实现方面，我们依然使用双指针，两个指针初始时分别位于数组的首尾，向中间移动遍历该序列。

算法

如果左指针 left\textit{left}left 指向的元素等于 val\textit{val}val，此时将右指针 right\textit{right}right 指向的元素复制到左指针 left\textit{left}left 的位置，然后右指针 right\textit{right}right 左移一位。如果赋值过来的元素恰好也等于 val\textit{val}val，可以继续把右指针 right\textit{right}right 指向的元素的值赋值过来（左指针 left\textit{left}left 指向的等于 val\textit{val}val 的元素的位置继续被覆盖），直到左指针指向的元素的值不等于 val\textit{val}val 为止。

当左指针 left\textit{left}left 和右指针 right\textit{right}right 重合的时候，左右指针遍历完数组中所有的元素。

这样的方法**两个指针在最坏的情况下合起来只遍历了数组一次**。与方法一不同的是，方法二避免了**需要保留的元素的重复赋值操作**。如果都是val，也只会遍历一次并全部赋值一次！

代码

C++
Java
JavaScript
Golang
C
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int left = 0, right = nums.size();
        while (left < right) {
            if (nums[left] == val) {
                nums[left] = nums[right - 1];
                right--;
            } else {
                left++;
            }
        }
        return left;
    }
};
复杂度分析

时间复杂度：O(n)O(n)O(n)，其中 nnn 为序列的长度。我们只需要遍历该序列至多一次。

空间复杂度：O(1)O(1)O(1)。我们只需要常数的空间保存若干变量。
 
                            
方法三：双指针 —— 当要删除的元素很少时：因为问题描述中提到**元素的顺序可以更改**。 当我们遇到`nums[i]=val`我们可以**将当前元素与最后一个元素进行交换，并释放最后一个元素**。这实际上使数组的大小减少了 1。请注意，被交换的最后一个元素可能是想要移除的值。但在下一次迭代中，**我们仍然会检查这个元素**。
```cpp
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int l = 0, r = nums.size();
        while (l < r) {
            if (nums[l] == val) 
                nums[l] = nums[--r];
            else ++l;
        }
        return r;
    }
};
```
复杂度分析：
时间复杂度：O(n) ，i 和 n 最多总共遍历 n 步。在这个方法中，**赋值操作的次数等于要删除的元素的数量**。因此，如果要移除的元素很少，效率会更高。 空间复杂度：O(1) 。 

 

总结：这道题和[Move Zeroes](https://blog.csdn.net/myRealization/article/details/104425589)很像，是简单版本。但也有不同。
 
