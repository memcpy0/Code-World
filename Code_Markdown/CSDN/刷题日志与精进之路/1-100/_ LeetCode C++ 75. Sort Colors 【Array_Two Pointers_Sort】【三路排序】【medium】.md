

Given an array with n objects colored red, white or blue, sort them `in-place` so that objects of the same color are **adjacent**, with the colors in the order **red, white and blue**.

Here, we will use the integers 0, 1, and 2 to represent the color red, white, and blue **respectively**.

Note: You are not suppose to use the library's sort function for this problem.

Example:
```
Input: [2,0,2,1,1,0]
Output: [0,0,1,1,2,2]
```
Follow up:

-  A rather straight forward solution is a **two-pass** algorithm using **counting sort**.
-   First, iterate the array counting number of 0's, 1's, and 2's, then **overwrite array** with total number of 0's, then 1's and followed by 2's.
-   Could you come up with a **one-pass algorithm** using only **constant space**?

> 这道题本问题被称为 荷兰国旗问题，最初由 Edsger W. Dijkstra提出。其主要思想是给每个数字设定一种颜色，并按照荷兰国旗颜色的顺序进行调整。

题意：给出一个n个元素的数组，数组仅含有0，1，2三种元素，要求从小到大排序。目标是原地算法。


1. 方法一：最简单的就是直接使用sort，O(NlogN)时间复杂度。
执行用时 :4 ms, 在所有 C++ 提交中击败了78.45% 的用户
内存消耗 :10.7 MB, 在所有 C++ 提交中击败了5.83%的用户 
	```cpp
	class Solution {
	public:
	    void sortColors(vector<int>& nums) { 
	        sort(nums.begin(), nums.end());
	    }
	};
	```
2. 方法二：正如提示所说，可以使用计数排序/桶排序，计算出0，1，2的个数，然后再按照出现次数将0，1，2依次重新赋值回数组。O(N)空间，O(2N)时间，要扫描数组两次。
	```cpp
	class Solution {
	public:
	    void sortColors(vector<int>& nums) {  
	        int freq[3] = {0}, len = 0;
	        for (const auto i : nums) ++freq[i];
	        for (int i = 0; i < 3; ++i) 
	            for (int j = 0; j < freq[i]; ++j) 
	                nums[len++] = i; 
	};
	  ```
---
3. 方法三：我们还有更多的优化空间吗？看题意，我们应该还有，可以做到O(N)时间，one-pass，和O(1)的常量空间。思路和三路快排差不多。

	二路快排我们都比较清楚，是双指针的应用，**lo从前往后扫描，hi从后往前扫描，将所有大于pivot的元素放到hi的位置，将所有小于等于pivot的元素放到lo的位置上**。而且...我要说的是，==**这个题其实也可以用二路快排的方法解决的**==，第一次以1为pivot进行划分，小于1的移动到左边，>=1的移动到右边。然后对lo和hi相遇的位置之后 (之后的元素要么是1，要么是2) 开始又一次快排，以2为pivot，小于2的放到左边，>=2的放到右边。大概是O(2N)的时间复杂度，O(1)的空间。

	那么三路快排又该怎么做呢？ 它将元素分为三类，<v，==v，>v，**lt从前往后扫描，gt从后往前**，`arr[l, lt-1]`是小于v的元素区间，`arr[lt, gt-1]`是等于v的元素区间，`arr[gt, r]`是大于v的元素区间。从这里可以看出lt是等于v的元素区间的开头，gt是大于v的元素区间的开头，**lt初始化为0，gt初始化为n**，我们真正操作的就是这两个指针，还有一个遍历数组的变量`i`。
	
	（1）首先移动`i`，如果`arr[i]==v`，那么i++，继续向后扫描；
	  （2）如果`arr[i] > v`，那么就交换`arr[i]和arr[--gt]`，这样gt指向的元素符合要求，其区间也扩展，但是由于我们不知道交换来的元素是什么，i不变；
	  （3）如果`arr[i] < v`，而且我们知道`arr[lt]==v`，就交换`arr[lt]和arr[i]`，然后lt++，相当于其整个区间整体向右移动。i也++，扫描下一个元素。
	  （4）这样i所经过的地方最后都会成为v，直到`i==gt`，说明v区间和大于v的区间合并了。整个过程结束。
	![在这里插入图片描述](https://img-blog.csdnimg.cn/20200221224247855.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)	
	对于题目而言，我使用的是`zero和two、i`三个指针，**分别追踪0的最右边界，2的最左边界和当前考虑的元素**，语义和上面不太一样，但也没有太大的区别，只不过zero指向零元素的最后一个位置。下面用图片展示一下整个过程： 
	![在这里插入图片描述](https://img-blog.csdnimg.cn/20200221234029806.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)	
	（1）首先移动`i`，如果`arr[i]==1`，那么i++，继续向后扫描；  ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200221234059712.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200221234247472.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
		  （2）如果`arr[i] > 1`，即`arr[i]==2`，那么就交换`arr[i]和arr[--two]`，这样gt指向的元素符合要求，其区间也扩展，但是由于我们不知道交换来的元素是什么，i不变； ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200221234358509.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200221234509309.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200221234543452.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
	  （3）如果`arr[i] < 1`，即`arr[i]==0`，而且我们知道`arr[zero+1]==1`，就交换`arr[++zero]和arr[i]`，相当于其整个1区间整体向右移动，0区间扩展。i也++，扫描下一个元素。
	  ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200221235051618.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200221235130674.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200221235201524.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
	  （4）这样`i`所经过的地方最后都会成为1(最前面会有0的部分)，直到`i==two`，说明1区间和2的区间接在一起了。整个过程结束。

	执行用时 :0 ms, 在所有 C++ 提交中击败了100.00% 的用户
	内存消耗 :10.7 MB, 在所有 C++ 提交中击败了5.83%的用户
	```cpp
	class Solution {
	public:
	    void sortColors(vector<int>& nums) {    
	        //[0, zero]==0, [two, n-1]=2, [zero+1, i-1]=1
	        int zero = -1, two = nums.size(); 
	        //nums[0, zero]==0, [two, nums.size()]==2
	        for (int i = 0; i < two; ) { //i撞到two, two后面的元素已经确定了
	             if (nums[i] == 1) ++i;
	             else if (nums[i] == 2) swap(nums[i], nums[--two]);
	             else if (nums[i] == 0) {
	                 swap(nums[i], nums[++zero]);
	                 ++i;
				 }
	         }
         }
	};
	  ```

---
4. 方法四：解法三中总共只有三种数，可以自然的分成三部分，用两个指针作为间隔，那么如果有5种数呢？K种数呢？下面的解法更简单一点。假设zero，one，two指向已排好的数组部分当前0的末尾，1的末尾，2的末尾。然后，当前指针遍历到i的位置。
	```
	0 0 1 2 2 2 0 2 1
	  ^ ^     ^ ^
	  z o     t i
    ```
	nums[i]等于0，那么就将two指针后移，并将当前数字置为2，one指针后移并将当前数字置为1，将zero指针后移并将当前数字置为0，这样整体移动1和2的区间，扩展0区间。**即使0和2中间没有1区间，也要移动，因为1就是夹在0和2区间中的**。
	```
	//two向后移并置2：
	0 0 1 2 2 2 2 2 1
	  ^ ^       ^
	  z o       i/t
	//one向后移并置2：
	0 0 1 1 2 2 2 2 1
	  ^   ^     ^
	  z   o     i/t
	//zero向后移并置2：
	0 0 0 1 2 2 2 2 1
	    ^ ^     ^
	    z o     i/t
    ```
    最后i++，处理下一个元素。这里不详细解释。
    
    如果nums[i]等于1，那么也简单，将two指针后移置为2，将one指针后移置为1，zero指针不动。如果nums[i]等于2，仅将two往后移动即可。当然，**其后i都要加1**。
    
    虽然这个算法也是O(N)时间，O(1)空间，但是反复的赋值降低了一部分效率。
    
	执行用时 :4 ms, 在所有 C++ 提交中击败了78.45% 的用户
	内存消耗 :10.8 MB, 在所有 C++ 提交中击败了5.83%的用户

	```cpp
	class Solution {
	public:
	    void sortColors(vector<int>& nums) { 
	        int zero = -1, one = -1, two = -1;
	        for (int i = 0; i < nums.size(); ++i) {
	            if (nums[i] == 2) ++two;
	            else if (nums[i] == 1) {
	                nums[++two] = 2;
	                nums[++one] = 1;
	            } else if (nums[i] == 0) {
	                nums[++two] = 2;
	                nums[++one] = 1;
	                nums[++zero] = 0;
	            }
	        }
	    }
	};
	```
