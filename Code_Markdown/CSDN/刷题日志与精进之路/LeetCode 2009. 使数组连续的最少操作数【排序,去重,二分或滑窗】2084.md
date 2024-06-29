> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310110221159.png)
给你一个整数数组 `nums` 。每一次操作中，你可以将 `nums` 中 **任意** 一个元素替换成 **任意** 整数。

如果 `nums` 满足以下条件，那么它是 **连续的** ：
- `nums` 中所有元素都是 **互不相同** 的。
- `nums` 中 **最大** 元素与 **最小** 元素的差等于 `nums.length - 1` 。

比方说，`nums = [4, 2, 5, 3]` 是 **连续的** ，但是 `nums = [1, 2, 3, 5, 6]` **不是连续的** 。

请你返回使 `nums` **连续** 的 **最少** 操作次数。

**示例 1：**
```js
输入：nums = [4,2,5,3]
输出：0
解释：nums 已经是连续的了。
```
**示例 2：**
```js
输入：nums = [1,2,3,5,6]
输出：1
解释：一个可能的解是将最后一个元素变为 4 。
结果数组为 [1,2,3,5,4] ，是连续数组。
```
**示例 3：**
```js
输入：nums = [1,10,100,1000]
输出：3
解释：一个可能的解是：
- 将第二个元素变为 2 。
- 将第三个元素变为 3 。
- 将第四个元素变为 4 。
结果数组为 [1,2,3,4] ，是连续数组。
```
**提示：**
- `1 <= nums.length <= 10^5`
- `1 <= nums[i] <= 10^9`

---
### 解法 排序+去重+二分/滑窗
考虑**最多可以保留多少个元素不变**。由于元素的位置不影响答案，且**要求所有元素互不相同**，我们可以将 $nums$ 排序，并去掉重复元素。

对排序去重后的 $nums'$ 中的一段区间 $[l,r]$ ，若要**保留这段区间内的所有元素**，我们需要**替换区间外的元素、并填充到 $[nums'[l], nums'[r]]$ 内缺失的元素上**。需要填充的元素个数为
$$\textit{nums}'[r]-\textit{nums}'[l]+1-(r-l+1)$$
$\textit{nums}'[r]-\textit{nums}'[l]+1$ 是 $nums'[r]$ 和 $nums'[l]$ 在数值上的距离，其中可以容纳多少个不同的连续整数；$r - l + 1$ 是 $nums$ 中这段区间的元素个数。相减就是需要额外填充的元素个数。

记原数组长度为 $n$ ，则区间外有 $n-(r-l+1)$ 个元素可以用来填充。由于**区间外的元素个数不能少于需要填充的元素个数**，所以有
$$\textit{nums}'[r]-\textit{nums}'[l]+1-(r-l+1) \le n-(r-l+1)$$
上式可化简为
$$\textit{nums}'[l]\ge\textit{nums}'[r]-n+1$$
根据该式，我们可以枚举 $\textit{nums}'[r]$ ，二分（或者用双指针）得到**最小的**满足该式的 $l$ ，此时 $[l,r]$ 区间内的元素均可以保留。**用 $n$ 减去最多可以保留的元素个数，就是答案**。
```cpp
class Solution {
public:
    int minOperations(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int uniqueLen = 1, n = nums.size();
        for (int i = 1; i < n; ++i)
            if (nums[i] != nums[i - 1])
                nums[uniqueLen++] = nums[i];
        int ans = n; // 最多需要的操作数
        // 二分,枚举nums[r],并找到最小的满足nums[r]-num[l]<=n-1的l
        // 于是无重复元素的区间nums[l:r]中,最多可以保留的元素个数为r-l+1
        // 最少需要的操作数为n-(r-l+1)
        for (int i = 0; i < uniqueLen; ++i) {
            int l = 0, r = i;
            while (l < r) {
                int mid = l + r >> 1;
                if (nums[i] - nums[mid] <= n - 1) r = mid;
                else l = mid + 1;
            }
            ans = min(ans, n - (i - l + 1));
        }
        return ans;
    }
};
```
用双指针也可以**枚举 $nums'[l]$** ，然后找到最大的满足 $nums'[r] - n + 1 \le nums'[l]$ 的 $r$ ：
```cpp
class Solution {
public:
    int minOperations(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int uniqueLen = 1, n = nums.size();
        for (int i = 1; i < n; ++i)
            if (nums[i] != nums[i - 1])
                nums[uniqueLen++] = nums[i];
        int ans = n; // 最多需要的操作数
        // 双指针,枚举nums[i],并找到最大的满足nums[j]-num[i]<=n-1的j(再+1)
        // 于是无重复元素的区间nums[i:j)中,最多可以保留的元素个数为j-i
        // 最少需要的操作数为n-(j-i)
        for (int i = 0, j = 0; i < uniqueLen; ++i) {
            while (j < uniqueLen && nums[j] - nums[i] <= n - 1) ++j;
            ans = min(ans, n - (j - i));
        }
        return ans;
    }
};
```
---
### 解法2 计数排序：最优O(n)
排序+滑动窗口的做法比较显然，但要做到线性复杂度，我们需要避免对整个数组进行排序。 注意到将滑动窗口的起点从值 $x$ 向右移到 $x+n$ 的过程中，我们只需考虑落在 $[x,x+2n)$ 内的元素，这一区间外的元素一定不在滑窗内。所以我们可以把 $nums[i]$ 放入编号为 $\lfloor\dfrac{nums[i]}{n}\rfloor$ 的桶内，第 $i$ 个桶包含区间 $[i\cdot n,(i+1)\cdot n)$ ，然后整体用一遍计数排序按照 $nums[i]\bmod n$ 对所有桶进行内部元素的排序。总复杂度 $O(n)$ 。
> 注：令 $U$ 表示数字范围，如果直接对所有数字进行基数排序，则复杂度为 $O(n \log_n U)$ 。而用以上的方法我们只需对低 $\log n$ 位进行排序，而不需要对高位排序，所以复杂度是 $O(n)$ ，是有本质区别的。


实际的处理不是"把nums[i]放入桶内，然后对所有桶进行内部排序"。
而是
- 把nums[i]按编号nums[i] % n进行分组，
- 第一步的所有分组按编号从小到大处理，将每个分组中的每个元素t放到对应的编号为 int(t/n)的桶内去。
- 这样处理的结果是，每个桶内的元素，按从小到大的顺序进行加入，最终也保持了有序。
  这是因为同一个桶内可能出现的元素值是固定的， 对应桶i来说， "% n = 0的元素"只可能是 in, "% n = 4的元素"只可能是 in + 4，一定有 "% n = 0 的元素" < "% n = 1 的元素" < "% n = 2 的元素" < ... < "% n = n - 1 的元素"。
- 第一步和第二步中，对每个元素的处理复杂度都是O(1)， 所以复杂度是O(n)。
```
/***************************************************
Author: hqztrue
https://github.com/hqztrue/LeetCodeSolutions
Complexity: O(n)
If you find this solution helpful, plz give a star:)
***************************************************/
const int N=100005;
vector<int> v[N];
unordered_map<int,vector<int>> b;
class Solution {
public:
	int minOperations(vector<int>& a) {
		int n=a.size(),ans=0; b.clear();
		for (int i=0;i<n;++i)v[i].clear();
		for (auto &x:a)v[x%n].push_back(x);
		for (int i=0;i<n;++i)
			for (auto &x:v[i]){
				auto &t=b[x/n];
				if (!t.size()||t.back()!=x)t.push_back(x);
			}
		for (auto &t:b){
			vector<int> &a=t.second,c=a;
			auto it=b.find(t.first+1);
			if (it!=b.end())
				for (auto &x:it->second)c.push_back(x);
			int m=c.size(),r=1;
			for (int l=0;l<a.size();++l){
				while (r<m&&c[r]-c[l]<=n-1)++r;
				ans=max(ans,r-l);
			}
		}
		return n-ans;
	}
};
```

先分桶再计数排序那不就是标准的O(n)排序吗，也没有怎么利用到题目条件啊……
这里利用的条件是我们只需对低log n位进行排序，而不需要对高位排序，所以复杂度是O(n)而不是对所有数字基数排序的O(n log_n U)。

 但是我理解桶排序的过程就等于对高bit排序了啊

 而且我感觉这个复杂度还是哪里有问题啊，如果就是计数排序，按理是每个桶都是O(n)吧，我理解这里应该是需要分个类，数量多的计数排序，数量少的朴素排序？但要最终是O(n)的话，这个阈值感觉不好取。

@灵剑2012 不需要对桶进行排序，只需要对桶内部的元素排序。

@灵剑2012 只用一遍计数排序就可以对于所有桶将其中的元素排序。先把元素x丢到第x mod n号列表里，然后for i=0~n-1，把列表i里的每个元素x丢到第x/n号桶里。

那直接用计数排序不也一样-。- 你拿n个桶，这数据范围也就是排一次和排两次的差别。。。。。。。

hqztrue
@还没上岸的转码人 在本题的数字范围中只需要排两次，但是理论上数字范围U可以远大于n，是两个无关的参数。

@hqztrue🐸 但本质上还是计数排序，您这么处理完还是相当于对整个数组进行排序了啊。。。。。。只不过在数据十分稀疏的情况下您对排序的过程进行了优化，用了两个大小不一样的桶-。-

@还没上岸的转码人 这个方法是不对整个数组进行排序的，相当于对高位作哈希，对低位排序。这里的桶编号代表高位数字，是不需要对桶编号排序的。

@hqztrue🐸 但是当你高位哈希出来的桶都是连续的时候，这不就直接退化成事实上的整体排序了-。-

@还没上岸的转码人 只有当数字范围比较小(<n^2)的时候才会发生高位对应的桶编号连续的情况。举个例子，如果数字范围是int64，那么直接基数排序需要4次(65536进制)，我这个方法相当于把前64-log n位拿来做哈希，后log n位排序，一次计数排序就够了。

桶的大小虽然是确定的，桶的边界却需要枚举吧，有没有可能一个最好的覆盖跨了两个桶呢

@凶猛的嘟嘟 最多跨越两个相邻的桶。归并一下就好了。


作者：hqztrue
链接：https://leetcode.cn/problems/minimum-number-of-operations-to-make-array-continuous/solutions/4151960/simple-solution-beginner-friendly-easy-to-understand/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
