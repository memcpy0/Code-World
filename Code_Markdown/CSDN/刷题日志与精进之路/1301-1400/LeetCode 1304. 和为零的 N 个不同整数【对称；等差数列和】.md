> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数 `n`，请你返回 **任意** 一个由 `n` 个 **各不相同** 的整数组成的数组，并且这 `n` 个数相加和为 `0` 。

**示例 1：**
```cpp
输入：n = 5
输出：[-7,-1,1,3,4]
解释：这些数组也是正确的 [-5,-1,1,2,3]，[-3,-1,2,-2,4]。
```
**示例 2：**
```cpp
输入：n = 3
输出：[-1,0,1]
```
**示例 3：**
```cpp
输入：n = 1
输出：[0]
```
**提示：**
- `1 <= n <= 1000`
---
此题属于[[贪心与思维（基本贪心策略,反悔,区间,字典序,数学,思维,脑筋急转弯,构造)]]题中的【构造题】。
### 方法1 对称构造
考虑到 $n$ 个数的和为 $0$ ，且 $x + (-x) = 0$ 恒成立，可以把 $1$ 和 $-1$ ，$2$ 和 $-2$ ，$3$ 和 $-3$ 等成对添加到答案中。如果 $n$ 是奇数，则还需再加一个 $0$ ，凑出奇数个数。

方便编程实现的一种构造方案如下，其中 $m = \lfloor \dfrac{n}{2} \rfloor$ 。第 $0$ 个元素是 $1$ ，第 $m$ 个元素是 $-1$ ，依次类推，最后一个可能的 $0$ 会在一开始创建数组时就赋值：
- 当 $n$ 为偶数时，则 $[1,2,..., m - 1, m, -1, -2, ..., -(m - 1), -m]$ 
- 当 $n$ 为奇数时，则 $[1, 2, ..., m - 1, m, -1, -2, ..., -(m - 1), -m, 0]$ 

```java
class Solution {
    public int[] sumZero(int n) {
        int[] a = new int[n];
        int m = n / 2;
        for (int i = 0; i < m; i++) {
            a[i] = i + 1;
            a[i + m] = -i - 1;
        }
        return a;
    }
}
```
```cpp
class Solution {
public:
    vector<int> sumZero(int n) {
        vector<int> a(n);
        int m = n / 2;
        for (int i = 0; i < m; i++) {
            a[i] = i + 1;
            a[i + m] = -i - 1;
        }
        return a;
    }
};
```
```rust
impl Solution {
    pub fn sum_zero(n: i32) -> Vec<i32> {
        let n = n as usize;
        let mut a = vec![0; n];
        let m = n / 2;
        for i in 0..m {
            a[i] = i as i32 + 1;
            a[i + m] = -a[i]; // 直接赋值更好
        }
        a
    }
}
```
```python
class Solution:
    def sumZero(self, n: int) -> List[int]:
        a = [0] * n
        m = n // 2
        for i in range(m):
            a[i] = i + 1
            a[i + m] = -i - 1
        return a

class Solution:
    def sumZero(self, n: int) -> List[int]:
        m = n // 2
        if n % 2:
            return list(range(-m, m + 1)) # 奇数时带上0
        return list(range(-m, 0)) + list(range(1, m + 1))
```
```c
int* sumZero(int n, int* returnSize) {
    *returnSize = n;
    int* a = calloc(n, sizeof(int));
    int m = n / 2;
    for (int i = 0; i < m; i++) {
        a[i] = i + 1;
        a[i + m] = -i - 1;
    }
    return a;
}
```
```go
func sumZero(n int) []int {
	a := make([]int, n)
	m := n / 2
	for i := range m {
		a[i] = i + 1
		a[i+m] = -i - 1
	}
	return a
}
```
```js
var sumZero = function(n) {
    const a = Array(n).fill(0);
    const m = Math.floor(n / 2);
    for (let i = 0; i < m; i++) {
        a[i] = i + 1;
        a[i + m] = -i - 1;
    }
    return a;
};
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(1)$ ，返回值不计入。

### 方法2 等差数列和
设 $S = 1 + 2 + \dots + (n - 1) = \dfrac{n (n -1)} {2}$ ，则构造方案为：$$[-S, 1, 2, \dots, n - 1]$$

```java
class Solution {
    public int[] sumZero(int n) {
        int[] a = new int[n];
        a[0] = -n * (n - 1) / 2;
        for (int i = 1; i < n; i++) {
            a[i] = i;
        }
        return a;
    }
}
```
```cpp
class Solution {
public:
    vector<int> sumZero(int n) {
        vector<int> a(n);
        ranges::iota(a, 0);
        a[0] = -n * (n - 1) / 2;
        return a;
    }
};
```
```rust
impl Solution {
    pub fn sum_zero(n: i32) -> Vec<i32> {
        let mut a = vec![0; n as usize];
        a[0] = -n * (n - 1) / 2;
        for i in 1..n {
            a[i as usize] = i;
        }
        a
    }
}
```
```python
class Solution:
    def sumZero(self, n: int) -> List[int]:
        return [-n * (n - 1) // 2] + list(range(1, n))
```
```c
int* sumZero(int n, int* returnSize) {
    *returnSize = n;
    int* a = malloc(n * sizeof(int));
    a[0] = -n * (n - 1) / 2;
    for (int i = 1; i < n; i++) {
        a[i] = i;
    }
    return a;
}
```
```go
func sumZero(n int) []int {
	a := make([]int, n)
	a[0] = -n * (n - 1) / 2
	for i := 1; i < n; i++ {
		a[i] = i
	}
	return a
}
```
```js
var sumZero = function(n) {
    const a = Array(n).fill(0);
    a[0] = -n * (n - 1) / 2;
    for (let i = 1; i < n; i++) {
        a[i] = i;
    }
    return a;
};
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(1)$ ，返回值不计入。
