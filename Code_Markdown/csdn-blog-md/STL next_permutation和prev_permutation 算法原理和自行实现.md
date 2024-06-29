@[toc]

## 一、本文目标
在【其一 排列组合和子集生成】这篇文章中，我列举了多种实现排列和组合的方法。但是，那是**以求出全部的结果为目标**的函数。

如果，要求给出某个排列的前/下一个排列（字典序），那我们该怎样做呢？

在STL中，有两个算法实现了这个目标，分别是`prev_permutation`和`next_permutation`。显然，这两个函数不是求全排列求到某个排列，然后返回下一个排列或前一个排列，这种做法的复杂度绝对爆炸！

而且这两个函数除了**复杂度低**外，还有很多别的优点，比如**按照字典序**，还有就是**不会列举出重复的全排列**：

```cpp
#include <iostream>
#include <algorithm>
using namespace std;

int main() {
	char data[] = "aabc";
	do {
		puts(data);
	} while (next_permutation(data, data + 4));
	return 0;	
}
```
如果我们用【其一 排列组合和子集生成】中实现的函数的话，就会列出24种情况，而且其中有12种重复的情况。这里用$next\_permutation$只列出了不重复的12种情况。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200311203241526.png)
那它使用了什么黑魔法呢？这里就剖析一下。

## 二、next_permutation算法和思想
先让我们来看一些排列：
```
1
---------------
1 2
2 1
---------------
1 2 3
1 3 2
2 1 3
2 3 1
3 1 2
3 2 1
----------------
1 2 3 4
1 2 4 3
1 3 2 4
1 3 4 2
1 4 2 3
1 4 3 2
2 1 3 4
```
我们如何从一个排列转移到下一个排列呢？

首先，我们要知道这样的一件事，对于一个排列，从小到大排序的是最小的排列，从大到小排序是最大的排列。

然后，依照我们前面写排列函数的经验，求大小为$n$的排列$P(n)$，实际上是在确定第一个数有$n$种取值后，再求大小为$n-1$的排列$P(n-1)$。这是相似的问题，区别仅在数据规模的大小。
- 对于规模为1的排列，就是它本身；
- 对于规模为2的排列`1 2`，1和2单独看是有序的，那么如何得到下一个呢，我们可以交换1和2得到最大的排列`2 1`；
- 对于规模为3的排列`1 3 2`，有`3 2`逆序，因此`3 2`这两个数不存在更大的排列，那么**必须往前看**，**因为不再逆序**，所以容易得到 $3$ 前面的数字小于 $3$（当然，我们知道是$1$）。

 	我们如何排列这些数，使得它增大，但是增大的幅度最小呢？我们**必须**而且**只能**换掉 $3$ 前面的数，换成一个更大的数。**必须**是因为要让排列变大；为什么是**只能**换掉这个数，而不是换掉`3 2`中的一个数呢？因为前面说过了，`3 2`是==逆序的，不可能再让它变大了；而且如果我们换掉3或者2，将3与前面的大于3或者将2与前面大于2 的数字交换，`3 2`变大了，但是整个排列变小了！==
 
 	那么，这个大数从哪里来呢？
 
	 从 $3$前面的数$1$ 的前面来，如果有的话？==毫无疑问这是错误的==。我们知道排列**从前往后**按照字典序比较，如果我们把 $3$ 前面的数 $1$ 往前面换，因为要换来一个大于 $1$ 的数（如果有的话），那么前面的数就变小了！整个排列就变小了！所以**我们只能换掉这个数1，往这个数的后面换**！
 	
 	现在，我们总结一下已经得到的事实——我们知道了：我们无论如何都无法让 $3$后面的排列变大；我们必须换掉 $3$ 前面的那个数 $1$；而且必须将这个数和 $3$或$3$后面的数 交换！我们要交换的那个数，可以说是**将1和3及3后面的数进行从小到大排序后，1的后继元素**。这个数刚好可以增大 $1$，但增大的幅度最小。
 
	曙光就在眼前！由于`3 2`是从大到小排列，因此我们可以从最后一个数往前看，容易而且**必然**会发现第一个大于 $1$ 的数 $X$ ！这里就是数 $2$ ！我们将 $1$ 和 $2$ 交换，得到的是`2 3 1`。
	
	当然，`2 3 1`还不是我们要的答案，由于`3 1`显然为逆序，为这部分排列的最大排列；而且我们在新排列的$2$这个位置就大于旧排列的$1$，为了让增长的幅度最小，刚好得到下一个排列，我们==必须让$2$后面的排列最小化==。我们翻转`3 1`，正好可以得到最小排列`1 3`。

其他情况不再一一解释。

因此,就有了下面的算法$next\_permutation$：
<div align = "center">
 <img src="https://img-blog.csdnimg.cn/20200311202501895.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "110%"></img>
</div>

-    **从右到左**进行扫描，发现**第一个违背非递减趋势**的数字（==非递减在有重复元素时很重要==）,称之为 $PartitionNumber$， 如上图，$6$ 恰好是我们找到的 $PartitionNumber$。需要注意的是，这样的数一定是存在的；否则的话，就说明整个排列都是逆序的，不存在下一个排列，直接退出；
 -   **从右到左进行扫描**，发现**第一个比** $PartitionNumber$ **大的数**，称之为 $ChangeNumber$。而 $7$ 恰好是我们找到的 $ChangeNumber$；
-    交换 $PartitionNumber$ 和 $ChangeNumber$，这样一步，会使得**新的排列组成的数比旧的排列组成的数要大**。当然，新排列增长的幅度不一定是最小的.；
 -   反转在 $PartitionNumber$ 右侧的数：由于此时 $PartitionNumber$ 位置右侧的排列已经是**严格的从大到小排列**了。如此反转之后，必然是**严格的从小到大排序**，可以保证**新的排列组成的数的增长幅度在所有的可能中最小**。

## 三、next_permutation具体实现
对应的 cpp 实现:
```cpp
#include <iostream>
#include <algorithm>
using namespace std;

template<typename T>
bool myNextPermutation(T *start, T *end) { //[start,end)
	T *cur = end - 1, *pre = cur - 1; //pre指向partitionNumber 
	while (cur > start && *pre >= *cur) 
		--cur, --pre; //从右到左进行扫描，发现第一个违背非递减趋势的数字
	if (cur <= start) return false; //整个排列逆序, 不存在更大的排列 
	//从右到左进行扫描，发现第一个比partitionNumber大的数
	for (cur = end - 1; *cur <= *pre; --cur); //cur指向changeNumber  
	swap(*pre, *cur);
	reverse(pre + 1, end); //将尾部的逆序变成正序 
	return true; 
}

int main() { 
	
	char data[] = "aabc";
	do {
		puts(data);
	} while (myNextPermutation(data, data + 4));
	return 0;	
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200311222137263.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
## 四、prev_permutation算法和思想
-    **从右到左**进行扫描，发现**第一个违背非递增趋势**的数字（==非递增在有重复元素时很重要==）,称之为 $PartitionNumber$ 。需要注意的是，这样的数一定是存在的；否则的话，就说明整个排列都是正序的，不存在上一个排列，直接退出；
 -   **从右到左进行扫描**，发现**第一个比** $PartitionNumber$ **小的数**，称之为 $ChangeNumber$；
-    交换 $PartitionNumber$ 和 $ChangeNumber$，这样一步，会使得**新的排列组成的数比旧的排列组成的数要小**。当然，新排列减少的幅度不一定是最小的.；
 -   反转在 $PartitionNumber$ 右侧的数：由于此时 $PartitionNumber$ 位置右侧的排列已经是**严格的从小到大排列**了，如此反转之后，必然是**严格的从大到小排序**，可以保证**新的排列组成的数的减少幅度在所有的可能中最小**。
## 五、prev_permutation具体实现
将上述代码的$next\_permutation$中的所有有关$pre$和$cu$r指针内容比较的部分的符号反转，就得到了$prev\_permutation$：
```cpp
#include <iostream>
#include <algorithm>
using namespace std;

template<typename T>
bool myPrevPermutation(T *start, T *end) { //[start,end)
	T *cur = end - 1, *pre = cur - 1; //pre指向partitionNumber 
	while (cur > start && *pre <= *cur) 
		--cur, --pre; //从右到左进行扫描，发现第一个违背非递增趋势的数字
	if (cur <= start) return false; //整个排列逆序, 不存在更小的排列 
	//从右到左进行扫描，发现第一个比partitionNumber小的数
	for (cur = end - 1; *cur >= *pre; --cur); //cur指向changeNumber  
	swap(*pre, *cur);
	reverse(pre + 1, end); //将尾部的逆序变成正序 
	return true; 
}

int main() { 
	char data[] = "cbaa"; 
	do {
		puts(data); 
	} while (myPrevPermutation(data, data + 4));
	return 0;	
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200311223845880.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
## 六、总结和分析
每次调用$next\_permutation$和$prev\_permutation$的时间复杂度为：
> Up to **linear in half the distance** between first and last (in terms of actual swaps)

就是线性的时间复杂度。


