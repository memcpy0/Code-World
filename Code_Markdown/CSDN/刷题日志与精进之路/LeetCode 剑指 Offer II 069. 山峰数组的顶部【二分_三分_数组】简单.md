> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>符合下列属性的数组 <code>arr</code> 称为 <strong>山峰数组</strong>（<strong>山脉数组）</strong> ：</p>

<ul>
	<li><code>arr.length &gt;= 3</code></li>
	<li>存在 <code>i</code>（<code>0 &lt; i &lt; arr.length - 1</code>）使得：
	<ul>
		<li><code>arr[0] &lt; arr[1] &lt; ... arr[i-1] &lt; arr[i] </code></li>
		<li><code>arr[i] &gt; arr[i+1] &gt; ... &gt; arr[arr.length - 1]</code></li>
	</ul>
	</li>
</ul>

<p>给定由整数组成的山峰数组 <code>arr</code> ，返回任何满足 <code>arr[0] &lt; arr[1] &lt; ... arr[i - 1] &lt; arr[i] &gt; arr[i + 1] &gt; ... &gt; arr[arr.length - 1]</code> 的下标 <code>i</code> ，即山峰顶部。</p>

 

<p><strong>示例 1：</strong></p>

```cpp
输入：arr = [0,1,0]
输出：1
```
 
<p><strong>示例 2：</strong></p>

```cpp
输入：arr = [1,3,5,4,2]
输出：2
```

<p><strong>示例 3：</strong></p>

```cpp
输入：arr = [0,10,5,2]
输出：1
```
 
<p><strong>示例 4：</strong></p>

```cpp
输入：arr = [3,4,5,1]
输出：2
```

<p><strong>示例 5：</strong></p>

```cpp
输入：arr = [24,69,100,99,79,78,67,36,26,19]
输出：2
```

 
<p><strong>提示：</strong></p>

<ul>
	<li><code>3 &lt;= arr.length &lt;= 10<sup>4</sup></code></li>
	<li><code>0 &lt;= arr[i] &lt;= 10<sup>6</sup></code></li>
	<li>题目数据保证 <code>arr</code> 是一个山脉数组</li>
</ul>
  
<p><strong>进阶：</strong>很容易想到时间复杂度 <code>O(n)</code> 的解决方案，你可以设计一个 <code>O(log(n))</code> 的解决方案吗？</p>

 

<p>注意：本题与主站 852 题相同：<a href="https://leetcode-cn.com/problems/peak-index-in-a-mountain-array/">https://leetcode-cn.com/problems/peak-index-in-a-mountain-array/</a></p>

---
### 解法1 顺序遍历
由于数组一定是山脉，因此只需要找到一个下标 `i` ，其对应的元素值 `arr[i - 1] < arr[i] && arr[i] > arr[i + 1]` 。代码如下，算法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int peakIndexInMountainArray(vector<int>& arr) {
        for (int i = 1, n = arr.size(); i < n - 1; ++i)
            if (arr[i] > arr[i - 1] && arr[i] > arr[i + 1])
                return i;
        return -1;
    }
};
//执行用时：8 ms, 在所有 C++ 提交中击败了83.17% 的用户
//内存消耗：11.2 MB, 在所有 C++ 提交中击败了57.11% 的用户
```
---
### 解法2 二分
由于 `arr` 是山峰数组，且数值各不相同，因此峰顶元素左侧必然严格单调递增，峰顶元素右侧必然严格单调递减。这样就确保了序列本身满足「**二段性**」：选定一个端点后，结合「**一段满足 & 另一段不满足**」的特性来实现**折半**的查找效果。

具体来说，以峰顶元素为分割点的 `arr` 数组，根据与前驱/后继元素的大小关系，二段性表现为：
- 峰顶元素左侧满足 `arr[i] > arr[i - 1]` ，右侧不满足；
- 峰顶元素右侧满足 `arr[i] > arr[i + 1]` ，左侧不满足

实际代码如下。算法的时间复杂度为 $O(\log n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int peakIndexInMountainArray(vector<int>& arr) {
        int l = 1, r = arr.size() - 2; // 题解下标的范围为[1,n-2]
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (arr[mid] > arr[mid - 1] && arr[mid] > arr[mid + 1]) return mid;
            else if (arr[mid] > arr[mid - 1]) l = mid + 1; // mid落在山脉左侧
            else r = mid - 1; // mid落在山脉右侧 
        }
        return -1;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：11.1 MB, 在所有 C++ 提交中击败了98.69% 的用户
```
二分代码的版本很多，比如还可以写成**二分下界**（求出最后一个满足 `arr[i - 1] < arr[i]` 的下标位置 `i` 或者最后一个满足 `arr[i] < arr[i + 1]` 的下标 `i` 的下一个位置）或**二分上界**（求出第一个满足 `arr[i] > arr[i + 1]` 的下标位置 `i` 或者第一个满足 `arr[i - 1] > arr[i]`  的下标 `i` 的前一个位置）的模式。

---
### 解法3 三分
由于是离散形式的峰函数，可以用三分法——**使用两个端点将区间分成三份，然后每次否决三份之一的区间来逼近目标值**。

具体来说，由于峰顶元素为全局最大值，因此每次将区间分成 `[l, lmid], [lmid, rmid], [rmid, r]` 三段：
- 如果满足 `arr[lmid] > arr[rmid]` ，说明**峰顶元素不可能存在于 `[rmid, r]` 中**，令 `r = rmid - 1` 即可
- 否则满足 `arr[lmid] < arr[rmid]` ，说明**峰顶元素不可能存在于 `[l, lmid]` 中**，令 `l = lmid + 1` 即可。

实际代码如下。算法的时间复杂度为 $O(\log n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int peakIndexInMountainArray(vector<int>& arr) {
        int l = 1, r = arr.size() - 2; // 题解下标的范围为[1,n-2]
        while (l < r) {
            int lmid = l + (r - l) / 3, rmid = r - (r - l) / 3;
            if (arr[lmid] > arr[rmid]) r = rmid - 1;
            else l = lmid + 1;
        }
        return r;
    }
};
//执行用时：8 ms, 在所有 C++ 提交中击败了83.17% 的用户
//内存消耗：11.2 MB, 在所有 C++ 提交中击败了73.17% 的用户
```
进一步优化（？不推荐的写法），同时再利用左右坡的特征来排除区间：
```cpp
//C++ version
class Solution {
public:
    int peakIndexInMountainArray(vector<int>& arr) {
        int l = 1, r = arr.size() - 2; // 题解下标的范围为[1,n-2]
        while (l <= r) {
            int lmid = l + (r - l) / 3, rmid = r - (r - l) / 3;
            if (arr[lmid] > arr[lmid - 1] && arr[lmid] > arr[lmid + 1]) return lmid;
            if (arr[rmid] > arr[rmid - 1] && arr[rmid] > arr[rmid + 1]) return rmid;
            
            if (arr[lmid] > arr[rmid]) r = rmid - 1; // 缩小解空间
            else l = lmid + 1; // 缩小解空间

            if (arr[lmid] > arr[lmid - 1]) l = lmid; // lmid在左坡
            if (arr[rmid] > arr[rmid + 1]) r = rmid; // rmid在右坡  
        }
        return -1;
    }
}; 
//执行用时：4 ms, 在所有 C++ 提交中击败了99.66% 的用户
//内存消耗：11.1 MB, 在所有 C++ 提交中击败了90.71% 的用户
```

---
### 题外话：二分、三分还是 $k$ 分？
二分和三分在渐进复杂度上是一致的，都可以通过换底公式来转换为可忽略的常数，因此二者的复杂度都是 $O(\log n)$ 。

于是，我们选择二分还是三分，取决于要解决的是什么问题：
- 二分通常用来解决「**单调性**」问题。通过进一步深入的练习，我们发现只需要满足「**二段性**」，就能使用二分上界或下界来寻找分割点；
- 三分则用来解决单峰/单谷函数的极值问题，做法是「**通过比较两个中间端点，每次排除 `1 / 3` 区间来解决极值问题**」，而不是简单根据单个循环内、将区间分成三份来判断是否为三分。如下代码本质上还是利用「二段性」进行分割，只能算是「变形二分」：
	```cpp
	class Solution {
	public:
	    int peakIndexInMountainArray(vector<int>& arr) {
	        int l = 1, r = arr.size() - 2; // 题解下标的范围为[1,n-2]
	        while (l < r) {
	            int lmid = l + (r - l) / 3, rmid = r - (r - l) / 3;
	            if (arr[lmid] > arr[lmid + 1]) { // 跨越两个区间
	            	r = lmid;
	            } else if (arr[rmid] < arr[rmid + 1]) {
	            	l = rmid + 1;
	            } else { // lmid在左坡,rmid在右坡
	            	l = lmid;
	            	r = rmid;
	            }
	        }
	        return r;
	    }
	};
	```
	要注意的是，**不存在所谓的 $k$ 分**，因为这需要增加同等数量的分支判断以决定进入哪个分支（就跟上述代码有三个分支进行判断一样），单次 `while` 循环的复杂度将由 $O(1)$ 变为 $O(k)$ ，且实现思路将类似于上述代码，也只能算是「变形二分」。
