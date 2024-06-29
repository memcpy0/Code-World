> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091139113.png)

Given an array of integers, return **indices** of the two numbers such that they add up to a specific target.

You may assume that each input would have **exactly** one solution, and you may not use the same element twice.

**Example:**
```java
Given nums = [2, 7, 11, 15], target = 9,  
Because nums[0] + nums[1] = 2 + 7 = 9,
return [0, 1].
```
题意：给定一个整数数组 `nums` 和一个目标值 `target` ，在该数组中找出和为目标值的那两个整数，并返回它们的数组下标。

如果这就是面试题，我们必须思考：
- 数组索引从 `0` 还是从 `1` 开始；
- 没有解的时候怎么办，返回异常吗；
- 是否有唯一解；
- 有多个解的时候，返回哪一个解；
- 是否能够使用同一个元素两次；
- ……

当然，本题题目中已经对上述问题做了规定。**不过在面试的时候，我们必须考虑，甚至必须和面试官沟通这些问题。**

---
很多涉及到「两个变量」的题目，都可以枚举其中一个变量，把它当成常量看待，从而转换成「一个变量」的问题。代码实现时，通常来说「**枚举右，寻找左**」是更加好写的。
- [219. 存在重复元素 II](https://leetcode.cn/problems/contains-duplicate-ii/)
- [1512. 好数对的数目](https://leetcode.cn/problems/number-of-good-pairs/)
- [1010. 总持续时间可被 60 整除的歌曲](https://leetcode.cn/problems/pairs-of-songs-with-total-durations-divisible-by-60/)
- [2748. 美丽下标对的数目](https://leetcode.cn/problems/number-of-beautiful-pairs/)

### 解法一 暴力搜索
$O(N^2)$，暴力搜索。对每个位置 `i` ，往前搜索看看是否存在值满足 `nums[j] + nums[i] == target` ：
```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        for (int i = 0; i < nums.size(); ++i) 
            for (int j = 0; j < i; ++j) 
                if (nums[i] + nums[j] == target) 
                    return vector<int>{j, i};
        return {};
    }
};
```
或者对每个位置 `i` ，往后搜索看看是否存在值满足 `nums[i] + nums[j] == target` ：
```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) { 
        for (int i = 0; i < nums.size(); i++) 
            for (int j = i + 1; j < nums.size(); j++) 
                if (nums[i] + nums[j] == target)  
                    return vector<int>{i, j};
        return {};
    }
};

class Solution {
public:
    vector<int> twoSum(vector<int> &nums, int target) {
        for (int i = 0;; i++) // 枚举 i
            for (int j = i + 1; j < nums.size(); j++) // 枚举 i 右边的 j
                if (nums[i] + nums[j] == target) // 满足要求
                    return {i, j}; // 返回两个数的下标
        // 题目保证有解，循环中一定会 return
        // 所以这里无需 return，毕竟代码不会执行到这里
    }
};

作者：灵茶山艾府
链接：https://leetcode.cn/problems/two-sum/solutions/2326193/dong-hua-cong-liang-shu-zhi-he-zhong-wo-0yvmj/
```

---
### 解法二 排序+双指针/二分
在暴力解法中，第二层遍历是对iii后面的元素进行无差别枚举，这样会导致非常多的无效枚举。

我们可以借助二分算法，来对内层循环从时间上进行优化。

因为二分算法的前提是“有序”，则我们需要先对numsnumsnums进行排序。

又因为返回答案需要原numsnumsnums顺序，所以我们不妨提前将numsnumsnums中的值与其下标做一个绑定，统一排序。

作者：Broncos
链接：https://leetcode.cn/problems/two-sum/solutions/2326170/python3-wu-chong-fang-fa-1-liang-shu-zhi-jt44/
```py
class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        n = len(nums)
        nums = sorted(zip(nums, range(n))) # O(nlogn)

        # 以下部分时间复杂度为：O(nlogn)
        for i in range(n):
            t = target - nums[i][0]
            j = bisect_left(nums, (t, ), i + 1, n)
            if j < n and nums[j][0] == t:
                return [nums[i][1], nums[j][1]]
        return []
```

在上面二分查找的优化中，我们可以发现“有序”可以极大的优化内层循环的时间复杂度。

那么我们不妨发散思维，摒弃内外层循环的思路，“有序”是否还可以发挥更大的作用呢？

答案是可以的，即使用双指针算法。 

为了避免重复使用同一个元素等问题，先排序，然后一个左指针，一个右指针，向中间移动。时间复杂度为 $O(n\log n)$ ：
```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) { 
        vector<pair<int,int>> temp; //记录原下标
        for (int i = 0; i < nums.size(); ++i) temp.push_back({nums[i], i}); 
        sort(temp.begin(), temp.end()); 
        // 以下部分时间复杂度为：O(n) 
        int lo = 0, hi = nums.size() - 1;
        while (lo < hi) {
            int v = temp[lo].first + temp[hi].first;
            if (v == target) return vector<int>{temp[lo].second, temp[hi].second};
            else if (v > target) --hi;
            else ++lo; 
        }
        return {};
    }
};
```

---
### 解法三 哈希表
在解法二中，我们用二分查找的算法来优化了解法一种的内层循环，将内层循环的时间复杂度成功从O(n)O(n)O(n)优化到了O(logn)O(logn)O(logn)，同时我们也付出了一些代价——排序的时间复杂度O(nlogn)O(nlogn)O(nlogn)，将整体复杂度优化到了O(nlogn)O(nlogn)O(nlogn)。

换句话说，解法二的优化方式更像是一种“均摊”的优化。

就比如山峰和平地，将山峰上的高度移到平地上，减少总体的高度。

注：当然这种说法并不准确，从某种程度来说例子的总量不变，而代码的时间总量却变化了。

事实上，我们还可以采用另一种优化思路，即“用空间换时间”的方法。即采用哈希表的方式。

我们将numsnumsnums中的所有信息保存到哈希表中，那么再内层循环再去寻找满足题意值时，我们只需要花费O(1)O(1)O(1)的代价了。

作者：Broncos
链接：https://leetcode.cn/problems/two-sum/solutions/2326170/python3-wu-chong-fang-fa-1-liang-shu-zhi-jt44/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307021216338.png)

哈希表，存储每个数对应的下标，$O(N)$ 。值得一提的是，如果写成了`mp[t] != mp[nums[i]]`，就会出错。原因在于数组中**可能存在相同的数**，而 `map` 中不能有相同的 `key` ，后面的数的下标会覆盖前面相同的数的下标。如 `[3, 3], target = 6` ，`map` 中就只有 `3:1` 这对值。**此外还需要检查**，比如 `[2,1], target = 4` ，就可能重复使用 `2` 。
```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) { 
        map<int, int> mp;
        for (int i = 0; i < nums.size(); i++) mp[nums[i]] = i; 
        for (int i = 0; i < nums.size(); i++) {
            int t = target - nums[i];
            if (mp.count(t) && mp[t] != i) 
                return vector<int>{i, mp[t]};
        } 
        return {};
    }
};
```
效率如下，还能更快吗？可以的！
```cpp
执行用时 :16 ms, 在所有 cpp 提交中击败了74.77% 的用户
内存消耗 :10.6 MB, 在所有 cpp 提交中击败了9.46%的用户
```
上面写得那么纠结，究其原因是：我们一开始就把所有的元素和下标存储到 `map` 中。改进一下，先查找再存到 `map` 里面，将前面的一个 `for` 循环合并。**同时也不需要比较是否是当前元素，因为当前的元素还没有存进去。** 

也就是说，当遍历到元素 `v` 的时候，只查找 `v` 前面的元素，看里面是否有 `target - v` 这个元素。此时，我们还只将 `v` 之前的值放入查找表中，如果 `v` 前面的某个元素 `u` 和 `v` 相等，那么 `v` 的位置就没有覆盖 `u` 。这种情况下查找成功，我们就找到了解。
<img src="https://img-blog.csdnimg.cn/20200917155053213.png#pic_center" width="45%">
如果在前面没有找到 `target - v` ，就将 `v` 的位置放入查找表中，可能会覆盖前面的和 `v` 相等的元素的位置，**但是这不会妨碍解的生成**。因为如果前面相等的元素和 `v` 相加等于 `target` 的话，就直接返回了；如果继续查找，说明不会使用相等的元素（无论哪一个），因为会造成解不唯一：
<img src="https://img-blog.csdnimg.cn/20200917155414205.png#pic_center" width="45%">
然后我们继续查找下一个 `v` ：
<img src="https://img-blog.csdnimg.cn/20200917155530739.png#pic_center" width="45%">


```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
       map<int,int> a;
       vector<int> b(2,-1);
       for(int i = 0; i < nums.size(); i++){
           if(a.count(target - nums[i]) > 0){
               b[1] = i;
               b[0] = a[target-nums[i]];
               break;
           }
           a[nums[i]] = i;
       }
       return b;
    };
};
```
```
执行用时 :8 ms, 在所有 cpp 提交中击败了98.06% 的用户
内存消耗 :10.2 MB, 在所有 cpp 提交中击败了30.43%的用户
```
答疑
问：是什么原因导致了哈希和暴力这两种算法的快慢？

答：我用「获取了多少信息」来解释。

暴力做法每次拿两个数出来相加，和 target\textit{target}target 比较，那么花费 O(1)\mathcal{O}(1)O(1) 的时间，只获取了 O(1)\mathcal{O}(1)O(1) 的信息。

而哈希表做法，每次查询都能知道 O(n)\mathcal{O}(n)O(n) 个数中是否有 target−nums[j]\textit{target}-\textit{nums}[j]target−nums[j]，那么花费 O(1)\mathcal{O}(1)O(1) 的时间，就获取了 O(n)\mathcal{O}(n)O(n) 的信息。

这就是为什么我们可以把暴力的 O(n2)\mathcal{O}(n^2)O(n 
2
 ) 优化成 O(n)\mathcal{O}(n)O(n)。

暴

 
**2020/09/20 Update** 回顾前面做过的题目，重新写的代码，更加紧凑：
```cpp
 class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> dict; //第二个int默认初始化为0
        for (int i = 0; i < nums.size(); ++i) { 
            if (dict[target - nums[i]]) //这个数存在
                return vector<int>{dict[target - nums[i]] - 1, i};
            dict[nums[i]] = i + 1;
        }
        return vector<int>();
    }
};


class Solution {
    public int[] twoSum(int[] nums, int target) {
        var dict = new HashMap<Integer, Integer>();
        for (int i = 0; ; ++i) {
            if (dict.containsKey(target - nums[i]))
                return new int[]{dict.get(target - nums[i]), i};
            dict.put(nums[i], i);
        } 
    }
}


class Solution {
public:
    vector<int> twoSum(vector<int> &nums, int target) {
        unordered_map<int, int> idx; // 创建一个空哈希表
        for (int j = 0;; j++) { // 枚举 j
            // 在左边找 nums[i]，满足 nums[i]+nums[j]=target
            auto it = idx.find(target - nums[j]);
            if (it != idx.end()) // 找到了
                return {it->second, j}; // 返回两个数的下标
            idx[nums[j]] = j; // 保存 nums[j] 和 j
        }
    }
}; 
```

### 解法五【集合视角分类讨论】
我们不妨从集合的角度来看待这个题。

我们构造两个集合：

aaa集合为nums中所有值构成的集合。

bbb集合为nums中所有值与target的距离值构成的集合（即target−nums[i]target - nums[i]target−nums[i]）。

视角一
假设答案有解，那么aaa集合与bbb集合的交集一定有值。

假设答案无解，那么有两种情况：

aaa集合与bbb集合的交集为空集。

aaa集合与bbb集合的交集只有一个值，值为target的一半，并且nums中该值仅存在一个。

视角二
此外，因为题目保证一定有解，所以我们还可以再换一个视角。

从答案的构成上来看，可以分为两种情况：

第一种为numsnumsnums中两个相同元素的和为targettargettarget。

第二种为numsnumsnums中两个不同元素的和为targettargettarget。

集合·普通版 
class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        # 分情况讨论
        # 情况1. nums中有两个相同的值，和为target
        a = target // 2
        if target % 2 == 0 and nums.count(a) >= 2:
            return [nums.index(a), nums.index(a, nums.index(a) + 1)]

        # 情况2. nums中有两个不同的值，和为target
        s = set(nums) & set([target - x for x in nums])
        if s != set():
            b = s.pop()
            return [nums.index(b), nums.index(target - b)]
        
        # 情况3. nums中没有满足题意的情况
        return []
时间复杂度：O(n)O(n)O(n)，空间复杂度：O(n)O(n)O(n)。

作者：Broncos
链接：https://leetcode.cn/problems/two-sum/solutions/2326170/python3-wu-chong-fang-fa-1-liang-shu-zhi-jt44/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。