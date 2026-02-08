> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

在神秘的地牢中，`n` 个魔法师站成一排。每个魔法师都拥有一个属性，这个属性可以给你提供能量。有些魔法师可能会给你负能量，即从你身上吸取能量。

你被施加了一种诅咒，当你从魔法师 `i` 处吸收能量后，你将被立即传送到魔法师 `(i + k)` 处。这一过程将重复进行，直到你到达一个不存在 `(i + k)` 的魔法师为止。

换句话说，你将选择一个起点，然后以 `k` 为间隔跳跃，直到到达魔法师序列的末端，**在过程中吸收所有的能量**。

给定一个数组 `energy` 和一个整数`k`，返回你能获得的 **最大** 能量。

**示例 1：**
```c
输入：energy = [5,2,-10,-5,1], k = 3
输出：3
解释：可以从魔法师 1 开始，吸收能量 2 + 1 = 3。
```
**示例 2：**
```c
输入：energy = [-2,-3,-1], k = 2
输出：-1
解释：可以从魔法师 2 开始，吸收能量 -1。
```
**提示：**
- `1 <= energy.length <= 10^5`
- `-1000 <= energy[i] <= 1000`
- `1 <= k <= energy.length - 1`
---
### 思考
**从特殊到一般**，先想一想，$k = 1$ 怎么做？

此时只能一步一步向右走。无论起点在哪，终点都是 $n -1$ 。

如果选择 $i$ 为起点，我们计算的是子数组 $[i, n - 1]$ 的元素和，即**后缀和**。

后缀和怎么算？我们可以倒着遍历 $energy$ ，同时累加元素和，即为后缀和。

答案等于**所有后缀和的最大值**。

再想一想，$k = 2$ 怎么做？

此时我们有两个终点：$n - 2$ 和 $n - 1$ 。对于终点 $n - 1$ ：
- 如果选择 $n - 3$ 为起点，那么我们累加的是下标为 $n - 3, n - 1$ 的元素和。
- 如果选择 $n - 5$ 为起点，那么我们累加的是下标为 $n - 5, n - 3, n - 1$ 的元素和。
- 如果选择 $n - 7$ 为起点，那么我们累加的是下标为 $n - 7, n - 5, n - 3, n - 1$ 的元素和。
- 一般地，从 $n - 1$ 开始倒着遍历，步长为 $-k = -2$ ，累加元素和，计算元素和的最大值。

对于终点 $n - 2$ ：
- 如果选择 $n - 4$ 为起点，那么我们累加的是下标为 $n - 4, n - 2$ 的元素和。
- 如果选择 $n - 6$ 为起点，那么我们累加的是下标为 $n - 6, n - 4, n - 2$ 的元素和。
- 如果选择 $n - 8$ 为起点，那么我们累加的是下标为 $n - 8, n - 6, n - 4, n - 2$ 的元素和。
- 一般地，从 $n - 2$ 开始倒着遍历，步长为 $-k = -2$ ，累加元素和，计算元素和的最大值。

是否可以从 $n - 3$ 开始倒着遍历？

不行，因为 $n - 3$ 还可以向右跳到 $n - 1$ ，所以 $n - 3$ 不是终点，不能作为倒着遍历的起点。

最后想一想，**一般情况**。

我们枚举终点 $n - k, n - k + 1, \dots, n - 1$ ，倒着遍历，步长为 $-k$ 。遍历的同时，累加元素和 $sufSum$ ，计算 $sufSum$ 的最大值，即为答案。
### 写法一
```java
class Solution {
    public int maximumEnergy(int[] energy, int k) {
        int n = energy.length;
        int ans = Integer.MIN_VALUE;
        for (int i = n - k; i < n; i++) { // 枚举终点 i
            int sufSum = 0;
            for (int j = i; j >= 0; j -= k) {
                sufSum += energy[j]; // 计算后缀和
                ans = Math.max(ans, sufSum);
            }
        }
        return ans;
    }
}
```
```cpp
class Solution {
public:
    int maximumEnergy(vector<int>& energy, int k) {
        int n = energy.size();
        int ans = INT_MIN;
        for (int i = n - k; i < n; i++) { // 枚举终点 i
            int suf_sum = 0;
            for (int j = i; j >= 0; j -= k) {
                suf_sum += energy[j]; // 计算后缀和
                ans = max(ans, suf_sum);
            }
        }
        return ans;
    }
};
```
```rust
impl Solution {
    pub fn maximum_energy(energy: Vec<i32>, k: i32) -> i32 {
        let n = energy.len();
        let k = k as usize;
        let mut ans = i32::MIN;
        for i in n - k..n { // 枚举终点 i
            let mut suf_sum = 0;
            for j in (0..=i).rev().step_by(k) {
                suf_sum += energy[j]; // 计算后缀和
                ans = ans.max(suf_sum);
            }
        }
        ans
    }
}
```
```python
class Solution:
    def maximumEnergy(self, energy: List[int], k: int) -> int:
        n = len(energy)
        ans = -inf
        for i in range(n - k, n):  # 枚举终点 i
            suf_sum = accumulate(energy[j] for j in range(i, -1, -k))  # 计算后缀和
            ans = max(ans, max(suf_sum))
        return ans
```
```go
func maximumEnergy(energy []int, k int) int {
	n := len(energy)
	ans := math.MinInt
	for i := n - k; i < n; i++ { // 枚举终点 i
		sufSum := 0
		for j := i; j >= 0; j -= k {
			sufSum += energy[j] // 计算后缀和
			ans = max(ans, sufSum)
		}
	}
	return ans
}
```
```c
#define MAX(a, b) ((b) > (a) ? (b) : (a))

int maximumEnergy(int* energy, int energySize, int k){
    int ans = INT_MIN;
    for (int i = energySize - k; i < energySize; i++) { // 枚举终点 i
        int suf_sum = 0;
        for (int j = i; j >= 0; j -= k) {
            suf_sum += energy[j]; // 计算后缀和
            ans = MAX(ans, suf_sum);
        }
    }
    return ans;
}
```
```js
var maximumEnergy = function(energy, k) {
    const n = energy.length;
    let ans = -Infinity;
    for (let i = n - k; i < n; i++) { // 枚举终点 i
        let sufSum = 0;
        for (let j = i; j >= 0; j -= k) {
            sufSum += energy[j]; // 计算后缀和
            ans = Math.max(ans, sufSum);
        }
    }
    return ans;
}
```
### 写法二
原地计算后缀和，把后缀和保存到 $energy$ 中。最后返回 $energy$ 的最大值，即为所有后缀和的最大值。
```java
class Solution {
    public int maximumEnergy(int[] energy, int k) {
        for (int i = energy.length - k - 1; i >= 0; i--) {
            energy[i] += energy[i + k];
        }
        return Arrays.stream(energy).max().getAsInt();
    }
}
```
```cpp
class Solution {
public:
    int maximumEnergy(vector<int>& energy, int k) {
        int n = energy.size();
        for (int i = n - k - 1; i >= 0; i--) {
            energy[i] += energy[i + k];
        }
        return ranges::max(energy);
    }
};
```
```rust
impl Solution {
    pub fn maximum_energy(mut energy: Vec<i32>, k: i32) -> i32 {
        let k = k as usize;
        for i in (0..energy.len() - k).rev() {
            energy[i] += energy[i + k];
        }
        *energy.iter().max().unwrap()
    }
}
```
```python
class Solution:
    def maximumEnergy(self, energy: List[int], k: int) -> int:
        for i in range(len(energy) - k - 1, -1, -1):
            energy[i] += energy[i + k]
        return max(energy)
```
```go
func maximumEnergy(energy []int, k int) int {
	for i := len(energy) - k - 1; i >= 0; i-- {
		energy[i] += energy[i+k]
	}
	return slices.Max(energy)
}
```
```c
#define MAX(a, b) ((b) > (a) ? (b) : (a))

int maximumEnergy(int* energy, int energySize, int k) {
    int ans = INT_MIN;
    for (int i = energySize - 1; i >= 0; i--) {
        if (i + k < energySize) {
            energy[i] += energy[i + k];
        }
        ans = MAX(ans, energy[i]);
    }
    return ans;
}
```
```js
var maximumEnergy = function(energy, k) {
    for (let i = energy.length - k - 1; i >= 0; i--) {
        energy[i] += energy[i + k];
    }
    return Math.max(...energy);
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是 $energy$ 的长度。
- 空间复杂度：$O(1)$ 。
### 专题训练
见贪心与思维题单的「**§5.3 逆向思维**」。
[贪心与思维（基本贪心策略/反悔/区间/字典序/数学/思维/脑筋急转弯/构造）](https://leetcode.cn/circle/discuss/g6KTKL/)