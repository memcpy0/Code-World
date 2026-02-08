> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个正整数数组 `spells` 和 `potions` ，长度分别为 `n` 和 `m` ，其中 `spells[i]` 表示第 `i` 个咒语的能量强度，`potions[j]` 表示第 `j` 瓶药水的能量强度。

同时给你一个整数 `success` 。一个咒语和药水的能量强度 **相乘** 如果 **大于等于** `success` ，那么它们视为一对 **成功** 的组合。

请你返回一个长度为 `n` 的整数数组 `pairs`，其中 `pairs[i]` 是能跟第 `i` 个咒语成功组合的 **药水** 数目。

**示例 1：**
```c
输入：spells = [5,1,3], potions = [1,2,3,4,5], success = 7
输出：[4,0,3]
解释：
- 第 0 个咒语：5 * [1,2,3,4,5] = [5,_10_,_15_,_20_,_25_] 。总共 4 个成功组合。
- 第 1 个咒语：1 * [1,2,3,4,5] = [1,2,3,4,5] 。总共 0 个成功组合。
- 第 2 个咒语：3 * [1,2,3,4,5] = [3,6,_9_,_12_,_15_] 。总共 3 个成功组合。
所以返回 [4,0,3] 。
```
**示例 2：**
```c
输入：spells = [3,1,2], potions = [8,5,8], success = 16
输出：[2,0,2]
解释：
- 第 0 个咒语：3 * [8,5,8] = [_24_,15,_24_] 。总共 2 个成功组合。
- 第 1 个咒语：1 * [8,5,8] = [8,5,8] 。总共 0 个成功组合。
- 第 2 个咒语：2 * [8,5,8] = [_16_,10,_16_] 。总共 2 个成功组合。
所以返回 [2,0,2] 。
```
**提示：**
- `n == spells.length`
- `m == potions.length`
- `1 <= n, m <= 10^5`
- `1 <= spells[i], potions[i] <= 10^5`
- `1 <= success <= 10^10`
---
### 方法一 排序+二分查找+原地修改
#### 写法一 使用浮点数
问题相当于给你 $n$ 个询问，每次问 $spells[i]$ 与 $potions$ 中的多少个数相乘，结果 $\ge success$ 。

对 $potions$ 排序后，就可以二分查找了:
- 设 $j$ 是最小的满足 $potions[j] \ge  \dfrac{ success }{spells[i]}$ 的下标。
- 由于数组已经排序，那么下标大于 $j$ 的数也同样满足不等式。
- 从 $j$ 到 $m -1$ ，一共有 $m - j$ 个满足不等式的数，其中 $m$ 是 $potions$ 的长度。
```cpp
class Solution {
public:
    vector<int> successfulPairs(vector<int>& spells, vector<int>& potions, long long success) {
        ranges::sort(potions);
        for (int& x : spells) { // 原地修改
            x = potions.end() - ranges::lower_bound(potions, 1.0 * success / x);
        }
        return spells;
    }
};
```
```java
class Solution {
    public int[] successfulPairs(int[] spells, int[] potions, long success) {
        Arrays.sort(potions);
        for (int i = 0; i < spells.length; i++) {
            spells[i] = potions.length - lowerBound(potions, (double) success / spells[i]);
        }
        return spells;
    }

    // 返回 nums 中的第一个大于等于 target 的元素下标
    private int lowerBound(int[] nums, double target) {
        int left = -1, right = nums.length; // 开区间 (left, right)
        while (left + 1 < right) { // 区间不为空
            // 循环不变量：
            // nums[left] < target
            // nums[right] >= target
            int mid = (left + right) >>> 1; // 比 left+(right-left)/2 更快的写法
            if (nums[mid] >= target) {
                right = mid; // 二分范围缩小到 (left, mid)
            } else {
                left = mid; // 二分范围缩小到 (mid, right)
            }
        }
        return right;
    }
}
```
```rust
impl Solution {
    pub fn successful_pairs(mut spells: Vec<i32>, mut potions: Vec<i32>, success: i64) -> Vec<i32> {
        potions.sort_unstable();
        let last = potions[potions.len() - 1] as i64;
        for x in spells.iter_mut() {
            let target = success as f64 / *x as f64;
            let j = potions.partition_point(|&x| (x as f64) < target);
            *x = (potions.len() - j) as i32;
        }
        spells
    }
}
```
```go
func successfulPairs(spells, potions []int, success int64) []int {
    slices.Sort(potions)
    for i, x := range spells {
        target := float64(success) / float64(x)
        j := sort.Search(len(potions), func(j int) bool { return float64(potions[j]) >= target })
        spells[i] = len(potions) - j
    }
    return spells
}
```
```python
class Solution:
    def successfulPairs(self, spells: List[int], potions: List[int], success: int) -> List[int]:
        potions.sort()
        m = len(potions)
        return [m - bisect_left(potions, success / x) for x in spells]
```
```c
int cmp(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

// 返回 nums 中的第一个大于等于 target 的元素下标
int lowerBound(int* nums, int numsSize, double target) {
    int left = -1, right = numsSize; // 开区间 (left, right)
    while (left + 1 < right) { // 区间不为空
        int mid = left + (right - left) / 2;
        if (nums[mid] >= target) {
            right = mid; // 二分范围缩小到 (left, mid)
        } else {
            left = mid; // 二分范围缩小到 (mid, right)
        }
    }
    return right;
}

int* successfulPairs(int* spells, int spellsSize, int* potions, int potionsSize, long long success, int* returnSize) {
    qsort(potions, potionsSize, sizeof(int), cmp);
    for (int i = 0; i < spellsSize; i++) {
        spells[i] = potionsSize - lowerBound(potions, potionsSize, 1.0 * success / spells[i]);
    }
    *returnSize = spellsSize;
    return spells;
}
```
```js
var successfulPairs = function(spells, potions, success) {
    potions.sort((a, b) => a - b);
    for (let i = 0; i < spells.length; i++) {
        const target = success / spells[i];
        spells[i] = potions.length - lowerBound(potions, target);
    }
    return spells;
};

var lowerBound = function(nums, target) {
    let left = -1, right = nums.length; // 开区间 (left, right)
    while (left + 1 < right) { // 区间不为空
        // 循环不变量：
        // nums[left] < target
        // nums[right] >= target
        const mid = Math.floor((left + right) / 2);
        if (nums[mid] >= target) {
            right = mid; // 范围缩小到 (left, mid)
        } else {
            left = mid; // 范围缩小到 (mid, right)
        }
    }
    return right;
}
// JS lodash
var successfulPairs = function(spells, potions, success) {
    potions.sort((a, b) => a - b);
    for (let i = 0; i < spells.length; i++) {
        const target = success / spells[i];
        spells[i] = potions.length - _.sortedIndex(potions, target);
    }
    return spells;
};
```
#### 写法二 不使用浮点数
浮点数有舍入误差，如果数据范围更大，上面的做法就不一定正确了。

更好的做法是，避免使用浮点数，只使用整数计算。一方面可以确保正确性，另一方面整数运算比浮点运算更快。

一种写法是，在二分函数里面，寻找第一个 $\times spells[i] \ge success$ 的 $potions[j]$ 。这种做法的问题是，二分查找 $potions$ 数组的时候，**每次都要转换为长整型进行运算和比较**。

由于**对于正整数乘法的不等式，有 $xy \ge success$ 等价于 $y \ge \lceil \dfrac{ success }  {x} \rceil$** 。

另一种写法是，我们自己使用整数运算，实现一个上取整函数，算出 $\lceil \dfrac{ success }  {x} \rceil$ 的值，这很简单：在 `a` 非负时，实现上取整，只需写成 `ceil(a, b) = a % b == 0 ? a / b : a / b + 1` 。这就只用到了整数取余和下取整运算。

有了这个上界，由于 $spells, potions$ 数组都为整型，**如果上界没超过 $potions$ 的最大值，则将上界转为整型，二分时每次就只用比较整型的大小了**。如果超过了，则 $0$ 种 $potion$ 可满足题意。
> 问：可以用库函数 `ceil` 来计算上取整吗？
> 答：这会把整数转成浮点数计算，有舍入误差，导致计算结果可能是错的，如 $\dfrac{a}{b}$ 的正确结果是 $2$ ，但浮点计算结果为 $2.0000000001$ ，前者上取整为 $2$ ，后者上取整是错误的 $3$ 。

还有一种（装B）做法是，由于**上取整难以处理**，我们证明**如何将其转换为下取整**。后续只需记住结论即可。
> **上下取整转换公式**：当 $a$ 为非负整数，$b$ 为正整数时，有恒等式（具体数学练习3.12）：$$\begin{matrix} \lceil \dfrac{a}{b} \rceil = \lfloor \dfrac{ a + b - 1} { b } \rfloor = \lfloor \dfrac{a-1}{b} \rfloor + 1 \quad ① \\
  \lfloor \dfrac{a}{b} \rfloor = \lceil \dfrac{ a - b + 1} { b } \rceil = \lceil \dfrac{a+1}{b} \rceil - 1\quad ② \\
   \lceil \dfrac{a}{b} \rceil - \lfloor \dfrac{a}{b} \rfloor = \lceil \dfrac{a \bmod b} {b} \rceil = \begin{cases} 0\quad 若a \bmod b = 0 \\ 1 \quad 若a \bmod b > 0\end{cases} \quad ③
   \end{matrix}$$
> 思路是程序中
> 因此**①的证明**，针对 $a \bmod b$ 的结果分类讨论如下：
> - 如果 $a \bmod b = 0$ ，则恒等式左边等于 $\dfrac{a}{b}$ ，右边等于 $\lfloor \dfrac{a}{b} + \dfrac{b - 1}{b} \rfloor = \dfrac{a}{b} + \lfloor \dfrac{ b - 1} {b}\rfloor = \dfrac{a}{b}$ 。恒等式成立。
> - 如果 $a \bmod b > 0$ ，则恒等式左边等于 $\lceil \dfrac{a}{b} \rceil = \lfloor \dfrac{a}{b} \rfloor + \lceil \dfrac{a \bmod b}{b} \rceil = \lfloor \dfrac{a}{b} \rfloor + 1$ ，右边由于 $\lfloor \dfrac {a } {b} \rfloor = \lfloor \dfrac{a - 1}{b} \rfloor$ ，所以右边等于 $\lfloor \dfrac{ a - 1 + b } { b} \rfloor = \lfloor \dfrac{a - 1}{b} + \dfrac{b}{b} \rfloor = \lfloor \dfrac{a}{b} \rfloor + 1$ ，

根据这一转换公式，我们有 $$y \ge \lceil \dfrac{success}{x} \rceil = \lfloor \dfrac{success - 1}{x} \rfloor + 1$$
或：$$y > \lfloor \dfrac {success -1}{x} \rfloor$$
为什么不等式一定要这样变形？好处是只需要在二分之前做一次减法+一次除法，避免**在二分循环内计算乘法**，也避免了**取余和分支预测**，效率更高。另外的好处是部分语言可以直接调用库函数二分。
```cpp
class Solution {
public:
    vector<int> successfulPairs(vector<int>& spells, vector<int>& potions, long long success) {
        ranges::sort(potions);
        for (int& x : spells) { // 原地修改
            long long target = (success - 1) / x;
            if (target < potions.back()) {
                // 这样写每次二分就只用比两个 int 的大小，避免把 potions 中的元素转成 long long 比较
                x = potions.end() - ranges::upper_bound(potions, (int) target);
            } else {
                x = 0;
            }
        }
        return spells;
    }
};
```
```java
class Solution {
    public int[] successfulPairs(int[] spells, int[] potions, long success) {
        Arrays.sort(potions);
        for (int i = 0; i < spells.length; i++) {
            long target = (success - 1) / spells[i];
            if (target < potions[potions.length - 1]) {
                // 这样写每次二分就只用比两个 int 的大小，避免把 potions 中的元素转成 long 比较
                spells[i] = potions.length - upperBound(potions, (int) target);
            } else {
                spells[i] = 0;
            }
        }
        return spells;
    }

    // 返回 nums 中的第一个大于 target 的元素下标
    private int upperBound(int[] nums, int target) {
        int left = -1, right = nums.length; // 开区间 (left, right)
        while (left + 1 < right) { // 区间不为空
            // 循环不变量：
            // nums[left] <= target
            // nums[right] > target
            int mid = (left + right) >>> 1; // 比 left+(right-left)/2 更快的写法
            if (nums[mid] > target) {
                right = mid; // 二分范围缩小到 (left, mid)
            } else {
                left = mid; // 二分范围缩小到 (mid, right)
            }
        }
        return right;
    }
}
```
```rust
impl Solution {
    pub fn successful_pairs(mut spells: Vec<i32>, mut potions: Vec<i32>, success: i64) -> Vec<i32> {
        potions.sort_unstable();
        let last = potions[potions.len() - 1] as i64;
        for x in spells.iter_mut() {
            let target = (success - 1) / *x as i64;
            if target < last { // 防止 i64 转成 i32 截断（这样不需要把 potions 中的数转成 i64 比较）
                let j = potions.partition_point(|&x| x <= target as i32);
                *x = (potions.len() - j) as i32;
            } else {
                *x = 0;
            }
        }
        spells
    }
}
```
```go
func successfulPairs(spells, potions []int, success int64) []int {
	slices.Sort(potions)
	for i, x := range spells {
		spells[i] = len(potions) - sort.SearchInts(potions, (int(success)-1)/x+1)
	}
	return spells
}
```
```python
class Solution:
    def successfulPairs(self, spells: List[int], potions: List[int], success: int) -> List[int]:
        potions.sort()
        m = len(potions)
        success -= 1  # 提前减一，避免在循环中反复减一
        return [m - bisect_right(potions, success // x) for x in spells]
```
```c
int cmp(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

// 返回 nums 中的第一个大于 target 的元素下标
int upperBound(int* nums, int numsSize, int target) {
    int left = -1, right = numsSize; // 开区间 (left, right)
    while (left + 1 < right) { // 区间不为空
        int mid = left + (right - left) / 2;
        if (nums[mid] > target) {
            right = mid; // 二分范围缩小到 (left, mid)
        } else {
            left = mid; // 二分范围缩小到 (mid, right)
        }
    }
    return right;
}

int* successfulPairs(int* spells, int spellsSize, int* potions, int potionsSize, long long success, int* returnSize) {
    qsort(potions, potionsSize, sizeof(int), cmp);
    for (int i = 0; i < spellsSize; i++) {
        long long target = (success - 1) / spells[i];
        if (target < potions[potionsSize - 1]) {
            // 这样写每次二分就只用比两个 int 的大小，避免把 potions 中的元素转成 long long 比较
            spells[i] = potionsSize - upperBound(potions, potionsSize, target);
        } else {
            spells[i] = 0;
        }
    }
    *returnSize = spellsSize;
    return spells;
}
```
```js
var successfulPairs = function(spells, potions, success) {
    potions.sort((a, b) => a - b);
    for (let i = 0; i < spells.length; i++) {
        const target = Math.ceil(success / spells[i]);
        spells[i] = potions.length - lowerBound(potions, target);
    }
    return spells;
};

var lowerBound = function(nums, target) {
    let left = -1, right = nums.length; // 开区间 (left, right)
    while (left + 1 < right) { // 区间不为空
        // 循环不变量：
        // nums[left] < target
        // nums[right] >= target
        const mid = Math.floor((left + right) / 2);
        if (nums[mid] >= target) {
            right = mid; // 范围缩小到 (left, mid)
        } else {
            left = mid; // 范围缩小到 (mid, right)
        }
    }
    return right;
}
// JS lodash
var successfulPairs = function(spells, potions, success) {
    potions.sort((a, b) => a - b);
    for (let i = 0; i < spells.length; i++) {
        const target = Math.ceil(success / spells[i]);
        spells[i] = potions.length - _.sortedIndex(potions, target);
    }
    return spells;
};
```
复杂度分析：
- 时间复杂度：$O((n + m)\log m)$ ，其中 $n$ 为 $spells$ 的长度，$m$ 为 $potions$ 的长度。排序 $O(m \log m)$ ，二分 $n$ 次，每次 $O(\log m)$ 。
- 空间复杂度：$O(1)$ 。忽略排序的栈开销，仅仅用到若干额外变量。
---
### 方法二 计数+值域后缀和
方法一得出的结论是，统计满足 $potions[j] \ge \lfloor \dfrac {success - 1}{ spell[i] \rfloor } + 1$ 的药水的个数。比如，$potions = [1,2,2,3,5,5,5]$ ，要计算 $\ge 2$ 的药水的个数，我们可以统计每个数出现了多少次，记在一个 $cnt$ 数组中。在这个例子中，$cnt = [0, 1, 2, 1, 0, 3]$ ，比如 $cnt[5] = 3$ 表示 $5$ 出现了 $3$ 次。

那么计算 $cnt[2] + cnt[3] + cnt[4] + cnt[5] = 2 + 1 + 0 + 3 = 6$ ，就是 $\ge 2$ 的药水的个数。但这样太慢了，如何加速？

借鉴前缀和的思想，我们可以倒着遍历 $cnt$ ，原地计算 $cnt$ 的**后缀和**，把 $cnt[i]$ 更新为 $\ge i$ 的药水的个数。上面的 $cnt$ 可以更新为 $[7, 7, 6, 4, 3, 3]$ ，比如 $cnt[2] = 6$ 表示 $\ge 2$ 的药水的个数。
```cpp

```
```java
class Solution {
    public int[] successfulPairs(int[] spells, int[] potions, long success) {
        int mx = 0;
        for (int y : potions) {
            mx = Math.max(mx, y);
        }

        int[] cnt = new int[mx + 1];
        for (int y : potions) {
            cnt[y]++; // 统计每种药水的出现次数
        }

        // 计算 cnt 的后缀和
        for (int i = mx - 1; i >= 0; i--) {
            cnt[i] += cnt[i + 1];
        }
        // 计算完毕后，cnt[i] 就是 potions 值 >= i 的药水个数

        for (int i = 0; i < spells.length; i++) {
            long low = (success - 1) / spells[i] + 1;
            spells[i] = low <= mx ? cnt[(int) low] : 0;
        }
        return spells;
    }
}
```
```rust
impl Solution {
    pub fn successful_pairs(mut spells: Vec<i32>, potions: Vec<i32>, success: i64) -> Vec<i32> {
        let mx = *potions.iter().max().unwrap() as usize;
        let mut cnt = vec![0; mx + 1];
        for y in potions {
            cnt[y as usize] += 1; // 统计每种药水的出现次数
        }

        // 计算 cnt 的后缀和
        for i in (0..mx).rev() {
            cnt[i] += cnt[i + 1];
        }
        // 计算完毕后，cnt[i] 就是 potions 值 >= i 的药水个数

        let success = success as usize;
        for x in spells.iter_mut() {
            let low = (success - 1) / *x as usize + 1;
            *x = if low <= mx { cnt[low] } else { 0 };
        }
        spells
    }
}
```
```go
func successfulPairs(spells, potions []int, success int64) []int {
	mx := slices.Max(potions)
	cnt := make([]int, mx+1)
	for _, y := range potions {
		cnt[y]++ // 统计每种药水的出现次数
	}

	// 计算 cnt 的后缀和
	for i := mx - 1; i >= 0; i-- {
		cnt[i] += cnt[i+1]
	}
	// 计算完毕后，cnt[i] 就是 potions 值 >= i 的药水个数

	for i, x := range spells {
		low := (int(success)-1)/x + 1
		if low <= mx {
			spells[i] = cnt[low]
		} else {
			spells[i] = 0
		}
	}
	return spells
}
```
```python
class Solution:
    def successfulPairs(self, spells: List[int], potions: List[int], success: int) -> List[int]:
        mx = max(potions)
        cnt = [0] * (mx + 1)
        for y in potions:
            cnt[y] += 1  # 统计每种药水的出现次数

        # 计算 cnt 的后缀和
        for i in range(mx - 1, -1, -1):
            cnt[i] += cnt[i + 1]
        # 计算完毕后，cnt[i] 就是 potions 值 >= i 的药水个数

        for i, x in enumerate(spells):
            low = (success - 1) // x + 1
            spells[i] = cnt[low] if low <= mx else 0
        return spells
```
```c
#define MAX(a, b) ((b) > (a) ? (b) : (a))

int* successfulPairs(int* spells, int spellsSize, int* potions, int potionsSize, long long success, int* returnSize) {
    int mx = 0;
    for (int i = 0; i < potionsSize; i++) {
        mx = MAX(mx, potions[i]);
    }

    int* cnt = calloc(mx + 1, sizeof(int));
    for (int i = 0; i < potionsSize; i++) {
        cnt[potions[i]]++; // 统计每种药水的出现次数
    }

    // 计算 cnt 的后缀和
    for (int i = mx - 1; i >= 0; i--) {
        cnt[i] += cnt[i + 1];
    }
    // 计算完毕后，cnt[i] 就是 potions 值 >= i 的药水个数

    for (int i = 0; i < spellsSize; i++) {
        long long low = (success - 1) / spells[i] + 1;
        spells[i] = low <= mx ? cnt[low] : 0;
    }

    *returnSize = spellsSize;
    free(cnt);
    return spells;
}
```
```js
var successfulPairs = function(spells, potions, success) {
    const mx = Math.max(...potions);
    const cnt = Array(mx + 1).fill(0);
    for (const y of potions) {
        cnt[y]++; // 统计每种药水的出现次数
    }

    // 计算 cnt 的后缀和
    for (let i = mx - 1; i >= 0; i--) {
        cnt[i] += cnt[i + 1];
    }
    // 计算完毕后，cnt[i] 就是 potions 值 >= i 的药水个数

    for (let i = 0; i < spells.length; i++) {
        const low = Math.ceil(success / spells[i]);
        spells[i] = low <= mx ? cnt[low] : 0;
    }
    return spells;
};
```
复杂度分析：
- 时间复杂度：$O(n + m +U)$ ，其中 $n$ 为 $spells$ 的长度，$m$ 为 $potions$ 的长度，$U= \max(potions)$ 。
- 空间复杂度：$O(U)$ 。
### 思考题
把乘法改成**异或**要怎么做？对应的题目是：是 [1803. 统计异或值在范围内的数对有多少](https://leetcode.cn/problems/count-pairs-with-xor-in-a-range/)。
