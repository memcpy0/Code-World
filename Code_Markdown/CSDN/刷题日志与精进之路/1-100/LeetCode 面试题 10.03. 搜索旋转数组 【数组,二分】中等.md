> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

搜索旋转数组。给定一个排序后的数组，包含n个整数，但这个数组已被旋转过很多次了，次数不详。请编写代码找出数组中的某个元素，假设数组元素原先是按升序排列的。若有多个相同元素，返回索引值最小的一个。

**示例1:**

 **输入**: arr = [15, 16, 19, 20, 25, 1, 3, 4, 5, 7, 10, 14], target = 5
 **输出**: 8（元素5在该数组中的索引）

**示例2:**

 **输入**：arr = [15, 16, 19, 20, 25, 1, 3, 4, 5, 7, 10, 14], target = 11
 **输出**：-1 （没有找到）

**提示:**

1. arr 长度范围在[1, 1000000]之间

```cpp
class Solution {
public:
    int search(vector<int>& arr, int target) {
        int l = 0, r = arr.size() - 1;
        while (l < r && arr[0] == arr[r]) --r;
        while (l <= r) {
            int m = l + (r - l) / 2;
            if (arr[m] == target) {
                if (arr[m] == arr[l]) return l;
                int i = m;
                while (i >= l && arr[i] == target) --i;
                if (arr[i] != target) return i + 1;
            } 
            if (arr[m] > arr[l]) {
                if (target >= arr[l] && target < arr[m]) r = m - 1;
                else l = m + 1;
            } else if (arr[m] < arr[r]) {
                if (target > arr[m] && target <= arr[r]) l = m + 1;
                else r = m - 1;
            } else {
                while (l <= r && arr[l] == arr[m]) ++l;
                while (l <= r && arr[r] == arr[m]) --r;
            }
        }
        return -1;
    }
};
```
见LeetCode 81. Search in Rotated Sorted Array II【数组,二分】中等

Java 100% 二分法搜寻（超详尽，包看懂）

每次取中间索引判断是否和target相等，如果相等，则继续将target往左遍历，遍历到最小和target相等的索引然后返回

如果不等：则判断midVal和rVal(右边界的值)，此时有三种可能：

1）midVal<rVal,则可以说明mid~right一定是递增的，则判断target是否是大于midVal并且小于等于rVal，即是否在(mid,right]这个区间，如果是，压缩区间left=mid+1,否则说明不在(mid,right]区间，压缩区间 [left,mid)

2)midVal>rVal，则可以说明leftmid是递增的，因为原数组是非递减的，要满足中间数值大于最右边数值，那么最大值一定不在midVal的左侧(假如在左侧，那么有leftk)递增，然后k+1（k<mid）~right递增。处理方式同上

3）midVal==rVal，则有两种可能 right0mid 的值都相等，或者 mid~right的值相等，无论是哪种，都可以将right舍去，压缩区间

class Solution {
    public int search(int[] arr, int target) {
        if(arr[0]==target)
            return 0;
        int l=0;
        int r=arr.length-1;
        int mid=0;
        while(l<=r){
            mid=l+(r-l)/2;
            //mid值==target,则继续往左搜寻，找到最小的索引，最小索引一定不为0
            if(arr[mid]==target){
                while(mid>0&&arr[mid-1]==arr[mid])  mid--;
                return mid;
            }
            //说明mid~r是递增序列，判读target是否在中间
            if(arr[mid]<arr[r]){
                if(arr[mid]<target&&target<=arr[r]) l=mid+1;
                else    r=mid-1;
            }
            //说明 l~mid 是递增序列，判读target是否在中间
            else if(arr[mid]>arr[r]){
                if(arr[l]<=target&&target<arr[mid]) r=mid-1;
                else l=mid+1;
            }
            //arr[mid]==arr[r]说明要么r~0~mid都相等，要么mid~r都相等，无论哪种r 都可以舍去
            else{
                r--;
            }
        }
        return -1;
    }
}

作者：zhou
链接：https://leetcode.cn/problems/search-rotate-array-lcci/solutions/1322803/java-er-fen-cha-zhao-chao-guo-100-chao-x-l69j/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

看**快手的面经**遇到了这道题，需要考虑的边界条件比较多，这里从易到难总结一下旋转数组相关的题，都是二分法的套路，看了这篇题解，一次搞定6道题！

189. 旋转数组
题目：题解：

class Solution:
    def rotate(self, nums: List[int], k: int) -> None:
        """
        Do not return anything, modify nums in-place instead.
        """
        def reverse(left,right):
            while left<right:
                nums[left],nums[right]=nums[right],nums[left]
                left+=1
                right-=1
        n=len(nums)
        # 向右移动的位置k可能会大于n，因此对n取余
        k=k%n
        if k==0 or n<2:
            return 
        # 以此为例：nums = [1,2,3,4,5,6,7], k = 3
        # 先整个数组反转：[7,6,5,4,3,2,1]
        reverse(0,n-1)
        # 前k个反转：[5,6,7,4,3,2,1]
        reverse(0,k-1)
        # 后n-k个反转：[5,6,7,1,2,3,4]
        reverse(k,n-1)
153. 寻找旋转排序数组中的最小值
题目：

nums 中的所有整数都是 唯一 的
nums 原来是一个升序排序的数组，但在预先未知的某个点上进行了旋转
题解：

class Solution:
    def findMin(self, nums: List[int]) -> int:
        n=len(nums)
        left=0
        right=n-1
        # 这里控制条件没取等号，取等号大多是为了在while中直return mid，不取等号就跳出while返回l的值。
        while left<right:
            mid=left+(right-left)//2
            if nums[mid]>nums[right]:
                # 中间数字大于右边数字，比如[3,4,5,1,2]，则左侧是有序上升的，最小值在右侧
                left=mid+1
            else:
                # 中间数字小于等于右边数字，比如[6,7,1,2,3,4,5]，则右侧是有序上升的，最小值在左侧
                right=mid
        return nums[left]
154. 寻找旋转排序数组中的最小值 II
题目：

这道题是 153.寻找旋转排序数组中的最小值 的延伸题目。
允许重复会影响算法的时间复杂度吗？会如何影响，为什么？
题解：

class Solution:
    def findMin(self, nums: List[int]) -> int:
        n=len(nums)
        left=0
        right=n-1
        # 这里控制条件没取等号，取等号大多是为了在while中直return mid，不取等号就跳出while返回l的值。
        while left<right:
            mid=left+(right-left)//2
            if nums[mid]>nums[right]:
                # 中间数字大于右边数字，比如[3,4,5,1,2]，则左侧是有序上升的，最小值在右侧
                left=mid+1
            elif nums[mid]<nums[right]:
                # 中间数字小于等于右边数字，比如[6,7,1,2,3,4,5]，则右侧是有序上升的，最小值在左侧
                right=mid
            else:
                # 中间数字等于右边数字，比如[2,3,1,1,1]或者[4,1,2,3,3,3,3]
                # 则重复数字可能为最小值，也可能最小值在重复值的左侧
                # 所以将right左移一位
                right-=1
        return nums[left]        
平均时间复杂度为 O(logn)，而在最坏情况下，如果数组中的元素完全相同，那么 while 循环就需要执行 n次，每次忽略区间的右端点，时间复杂度为 O(n)。

上面两道题是在旋转数组里寻找最小值，下面两道题是在旋转数组里寻找指定的值，这两道题的区别也是存不存在重复值。

33. 搜索旋转排序数组
题目：

题解：

class Solution:
    def search(self, nums: List[int], target: int) -> int:
        if not nums:
            return -1
        l,r=0,len(nums)-1
        # 这里控制条件取等号，取等号大多是为了在while中直return mid，不取等号就跳出while返回l的值。
        while l<=r:
            mid=l+(r-l)//2
            # 中间值即为target，直接返回
            if nums[mid]==target:
                return mid
            # 左半部分是有序
            if nums[0]<=nums[mid]:
                # target落在左半部分有序区域内
                if nums[0]<=target<nums[mid]:
                    r=mid-1
                else:
                    # target落在右半部分无序区域内
                    l=mid+1
            else: # 右半部分是有序
                # target落在右半部分有序区域内
                if nums[mid]<target<=nums[len(nums)-1]:
                    l=mid+1
                else:
                    # target落在左半部分无序区域内
                    r=mid-1
        return -1
81. 搜索旋转排序数组 II
题目：

题解：

class Solution:
    def search(self, nums: List[int], target: int) -> int:
        if not nums:
            return False
        l,r=0,len(nums)-1
        while l<=r:
            # 重点在于处理重复数字
            # 左边有重复数字，将左边界右移
            while l<r and nums[l]==nums[l+1]:
                l+=1
            # 右边有重复数字，将右边界左移
            while l<r and nums[r]==nums[r-1]:
                r-=1
            mid=(l+r)//2
            if nums[mid]==target:
                return True
            # 左半部分有序
            if nums[0]<=nums[mid]:
                if nums[0]<=target<nums[mid]:
                    r=mid-1
                else:
                    l=mid+1
            else:# 右半部分有序
                if nums[mid]<target<=nums[len(nums)-1]:
                    l=mid+1
                else:
                    r=mid-1
        return False
面试题 10.03. 搜索旋转数组
这道题与81题很像，唯一的区别就是81题要求只要存在target就返回true，而这道题要 返回多个重复target中最靠前的那个 题目：

题解： 此题边界case很多，与上面的几道题相比，注释里给出了三个重点改变，仔细体会。

class Solution:
    def search(self, arr: List[int], target: int) -> int:
        n=len(arr)
        left=0
        right=n-1
        while left<=right:
            # 重点1：当left符合时直接返回, 因为找的是最小的索引
            if arr[left]==target:
                return left
            mid=left+(right-left)//2
            # 重点2：当中间值等于目标值，将右边界移到中间，因为左边可能还有相等的值
            if arr[mid]==target:
                right=mid
            elif arr[0]<arr[mid]:
                if arr[0]<=target<arr[mid]:
                    right=mid-1
                else:
                    left=mid+1
            elif arr[0]>arr[mid]:
                if arr[mid]<target<=arr[n-1]:
                    left=mid+1
                else:
                    right=mid-1
            else:
                # 重点3：当中间数字与左边数字相等时，将左边界右移
                    left+=1
        return -1

作者：乐清
链接：https://leetcode.cn/problems/search-rotate-array-lcci/solutions/623648/xuan-zhuan-shu-zu-cong-yi-dao-nan-ge-ge-dcv7a/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。