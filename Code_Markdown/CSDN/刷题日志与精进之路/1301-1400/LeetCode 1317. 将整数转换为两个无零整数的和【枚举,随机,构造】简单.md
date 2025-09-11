> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

「无零整数」是十进制表示中 **不含任何 0** 的正整数。

给你一个整数 `n`，请你返回一个 **由两个整数组成的列表** `[a, b]`，满足：
- `a` 和 `b` 都是无零整数
- `a + b = n`

题目数据保证至少有一个有效的解决方案。

如果存在多个有效解决方案，你可以返回其中任意一个。

**示例 1：**
```c
输入：n = 2
输出：[1,1]
解释：a = 1, b = 1。a + b = n 并且 a 和 b 的十进制表示形式都不包含任何 0。
```
**示例 2：**
```c
输入：n = 11
输出：[2,9]
```
**示例 3：**
```c
输入：n = 10000
输出：[1,9999]
```
**示例 4：**
```c
输入：n = 69
输出：[1,68]
```
**示例 5：**
```c
输入：n = 1010
输出：[11,999]
```
**提示：**
- `2 <= n <= 10^4`
 
---
### 方法一 暴力枚举
枚举 $a = 1, 2, 3, \dots, n - 1$ ，如果 $a$ 和 $n - a$ 都不包含 $0$ ，那么返回 $[a, n - a]$ 。

由于方法三给出了具体的构造方法，所以答案一定是存在的。注意，题目保证 $n \ge 2$ 。
```cpp
class Solution {
public:
    vector<int> getNoZeroIntegers(int n) {
        for (int a = 1; ; a++) {
            if (to_string(a).find('0') == string::npos &&
                to_string(n - a).find('0') == string::npos)
                return {a, n - a};
        }
    }
};
```
```java
class Solution {
    public int[] getNoZeroIntegers(int n) {
        for (int a = 1; ; a++) {
            if (!Integer.toString(a).contains("0") && 
                !Integer.toString(n - a).contains("0")) {
                return new int[]{a, n - a};
            }
        }
    }
}
```
```rust
impl Solution {
    pub fn get_no_zero_integers(n: i32) -> Vec<i32> {
        for a in 1.. {
            if !a.to_string().contains('0') && !(n - a).to_string().contains('0') {
                return vec![a, n - a];
            }
        }
        unreachable!()
    }
}
```
```python
class Solution:
    def getNoZeroIntegers(self, n: int) -> List[int]:
        for a in count(1):  # 枚举 a=1,2,3,...
            if '0' not in str(a) and '0' not in str(n - a):
                return [a, n - a]
```
```c
bool has_zero(int x) {
    while (x) {
        if (x % 10 == 0) {
            return true;
        }
        x /= 10;
    }
    return false;
}

int* getNoZeroIntegers(int n, int* returnSize) {
    for (int a = 1; ; a++) {
        if (!has_zero(a) && !has_zero(n - a)) {
            *returnSize = 2;
            int* ans = malloc(2 * sizeof(int));
            ans[0] = a;
            ans[1] = n - a;
            return ans;
        }
    }
}
```
```go
func getNoZeroIntegers(n int) []int {
	for a := 1; ; a++ {
		if !strings.ContainsRune(strconv.Itoa(a), '0') &&
			!strings.ContainsRune(strconv.Itoa(n-a), '0') {
			return []int{a, n - a}
		}
	}
}
```
```js
var getNoZeroIntegers = function(n) {
    for (let a = 1; ; a++) {
        if (!a.toString().includes("0") && !(n - a).toString().includes("0")) {
            return [a, n - a];
        }
    }
};
```
复杂度分析：
- 时间复杂度：$O(n\log n)$ ，每个数字需要 $O(\log n)$ 的时间判断是否包含 $0$ 。
- 空间复杂度：$O(\log n)$ 或 $O(1)$ ，取决于是否用到字符串。
### 方法二 随机
在 $[1, n - 1]$ 中随机整数 $a$ ，如果 $a, n - a$ 都不含 $0$ ，那么返回 $[a, n - a]$ 。
```cpp
class Solution {
public:
    vector<int> getNoZeroIntegers(int n) {
        // 可以先 srand 一下，但没必要写
        while (true) {
            int a = rand() % (n - 1) + 1;
            if (to_string(a).find('0') == string::npos && 
                to_string(n - a).find('0') == string::npos) {
                return {a, n - a};
            }
        }
    }
};
```
```java
class Solution {
    public int[] getNoZeroIntegers(int n) {
        Random rand = new Random();
        while (true) {
            int a = rand.nextInt(n - 1) + 1;
            if (!Integer.toString(a).contains("0") && 
                !Integer.toString(n - a).contains("0")) {
                return new int[]{a, n - a};
            }
        }
    }
}
```
```rust
use rand::Rng;

impl Solution {
    pub fn get_no_zero_integers(n: i32) -> Vec<i32> {
        let mut rng = rand::thread_rng();
        loop {
            let a = rng.gen_range(1..n);
            if !a.to_string().contains('0') && !(n - a).to_string().contains('0') {
                return vec![a, n - a];
            }
        }
    }
}
```
```python
class Solution:
    def getNoZeroIntegers(self, n: int) -> List[int]:
        while True:
            a = randint(1, n - 1)
            if '0' not in str(a) and '0' not in str(n - a):
                return [a, n - a]
```
```c
bool has_zero(int x) {
    while (x) {
        if (x % 10 == 0) {
            return true;
        }
        x /= 10;
    }
    return false;
}
int* getNoZeroIntegers(int n, int* returnSize) {
    // 可以先 srand 一下，但没必要写
    while (true) {
        int a = rand() % (n - 1) + 1;
        if (!has_zero(a) && !has_zero(n - a)) {
            *returnSize = 2;
            int* ans = malloc(2 * sizeof(int));
            ans[0] = a;
            ans[1] = n - a;
            return ans;
        }
    }
}
```
```go
func getNoZeroIntegers(n int) []int {
	for {
		a := rand.Intn(n-1) + 1
		if !strings.ContainsRune(strconv.Itoa(a), '0') &&
			!strings.ContainsRune(strconv.Itoa(n-a), '0') {
			return []int{a, n - a}
		}
	}
}
```
```js
var getNoZeroIntegers = function(n) {
    while (true) {
        const a = Math.floor(Math.random() * (n - 1)) + 1;
        if (!a.toString().includes("0") && !(n - a).toString().includes("0")) {
            return [a, n - a];
        }
    }
};
```
复杂度分析：
- 时间复杂度：期望 $O\big (\dfrac{ \log n } { 0.8^{\log_{10}n }} \big )$ 。**近似估计**：考虑 $n$ 的每一位，比如 $n$ 某一位是 $5$ ，则 $a$ 这一位有 $0$ 到 $9$ 共 $10$ 种可能，其中有 $2$ 种会让 $a$ 或 $b$ 包含 $0$ ，即 $5 = 0 + 5 = 5 + 0$ ，其他 $8$ 种情况 $a, b$ 的这一位都不包含 $0$（可以借位），概率是 $\dfrac{8}{10} = 0.8$ 。每一位都不包含 $0$ 的概率是 $P = 0.8^{ \log_{10} n}$ ，期望循环 $\dfrac{1}{P}$ 次就能找到答案。在本题数据范围内，平均循环次数 $\dfrac{1}{P} < 3$ 。
- 空间复杂度：$O(\log n)$ 或 $O(1)$ ，取决于是否用到字符串。
### 方法三 构造（最佳）
比如 $n = 666$ ，将每一位分成两个数，如 $6 = 3+3$ ，所以 $666 = 333+333$ 。
比如 $n = 777$ ，由于 $7 = 3 + 4$ ，所以 $777 = 333+444$ 。
但是，$n = 400$ 怎么分呢？如果分成 $400 = 200+200$ ，就不符合题目要求。
可以把 $400$ 看成 $390 + 10$ ，也就是把 $400$ 的个位数看做 $10$ ，十位数看做 $9$ ，百位数看做 $3$ 。每一位再分为两个数，就得到 $400 = 145 + 255$ 。

一般地：
1. 从低到高遍历 $n$ 的每一位数字 $d$ 。
2. 如果 $d \ge 2$ ，那么可以把 $d$ 分成 $\lfloor \dfrac{d}{2} \rfloor$ 和 $\lceil \dfrac{d}{2} \rceil$ ，这两个数都不是 $0$ ，分配给 $a$ 和 $b$ 。代码实现时，可以只考虑 $a$ 如何构造，最后用 $n - a$ 得到 $b$ 。
3. 如果 $d \le 1$ ，那么借位，把 $d$ 变成 $d + 10$ ，这样就能和上面一样分成两个非零数字，分配给 $a$ 和 $b$ 。
4. 如果遍历到了最高位，且最高位是 $1$ ，那么把 $1$ 分配给 $b$ ，$a$ 相当于分配到了 $0$ ，但这个其实是 $a$ 的前导 $0$ ，不算在 $a$ 内。
```cpp
class Solution {
public:
    vector<int> getNoZeroIntegers(int n) {
        int a = 0;
        int base = 1; // 10^k
        for (int x = n; x > 1; x /= 10) {
            int d = x % 10;
            if (d <= 1) {
                d += 10;
                x -= 10; // 借位
            }
            // a 这一位填 d/2，比如百位数就是 d/2 * 100
            a += d / 2 * base;
            base *= 10;
        }
        return {a, n - a};
    }
};
```
```java
class Solution {
    public int[] getNoZeroIntegers(int n) {
        int a = 0;
        int base = 1; // 10^k
        for (int x = n; x > 1; x /= 10) {
            int d = x % 10;
            if (d <= 1) {
                d += 10;
                x -= 10; // 借位
            }
            // a 这一位填 d/2，比如百位数就是 d/2 * 100
            a += d / 2 * base;
            base *= 10;
        }
        return new int[]{a, n - a};
    }
}
```
```rust
impl Solution {
    pub fn get_no_zero_integers(n: i32) -> Vec<i32> {
        let mut a = 0;
        let mut base = 1; // 10^k
        let mut x = n;
        while x > 1 {
            let mut d = x % 10;
            if d <= 1 {
                d += 10;
                x -= 10; // 借位
            }
            // a 这一位填 d/2，比如百位数就是 d/2 * 100
            a += d / 2 * base;
            base *= 10;
            x /= 10;
        }
        vec![a, n - a]
    }
}
```
```python
class Solution:
    def getNoZeroIntegers(self, n: int) -> List[int]:
        a = 0
        base = 1  # 10**k
        x = n
        while x > 1:
            x, d = divmod(x, 10)
            if d <= 1:
                d += 10
                x -= 1  # 借位
            # a 这一位填 d//2，比如百位数就是 d//2 * 100
            a += d // 2 * base
            base *= 10
        return [a, n - a]
```
```c
int* getNoZeroIntegers(int n, int* returnSize) {
    int a = 0;
    int base = 1; // 10^k
    for (int x = n; x > 1; x /= 10) {
        int d = x % 10;
        if (d <= 1) {
            d += 10;
            x -= 10; // 借位
        }
        // a 这一位填 d/2，比如百位数就是 d/2 * 100
        a += d / 2 * base;
        base *= 10;
    }
    *returnSize = 2;
    int* ans = malloc(2 * sizeof(int));
    ans[0] = a;
    ans[1] = n - a;
    return ans;
}
```
```go
func getNoZeroIntegers(n int) []int {
	a := 0
	base := 1 // 10^k
	for x := n; x > 1; x /= 10 {
		d := x % 10
		if d <= 1 {
			d += 10
			x -= 10 // 借位
		}
		// a 这一位填 d/2，比如百位数就是 d/2 * 100
		a += d / 2 * base
		base *= 10
	}
	return []int{a, n - a}
}
```
```js
var getNoZeroIntegers = function(n) {
    let a = 0;
    let base = 1; // 10^k
    for (let x = n; x > 1; x = Math.floor(x / 10)) {
        let d = x % 10;
        if (d <= 1) {
            d += 10;
            x -= 10; // 借位
        }
        // a 这一位填 d/2，比如百位数就是 d/2 * 100
        a += Math.floor(d / 2) * base;
        base *= 10;
    }
    return [a, n - a];
};
```
复杂度分析：
- 时间复杂度：$O(\log n)$
- 空间复杂度：$O(1)$
### 思考题
如果要求 $a$ 尽量小呢？想出一个 $O(\log n)$ 的做法。
答：$a=0$ ，$b=n$ 。  
找到 $b$ 最左边的 $0$ ，把余数全部给 $a$ ，再 $+1$ ，$b$ 变成 $x9999$ ，如果此时 $a$ 没有 $0$ ，返回结果。  
否则，找到 $a$ 最左边的 $0$ ，把 $a$ 变成 $x11111$ ，如果此时 $b$ 没有 $0$ ，返回结果。  
否则，重复上面的操作。复杂度应该是 $O(\log^2 n)$ 
```cpp
class Solution {
public:
    const int ALLONE[10]{0, 1, 11, 111, 1111, 11111, 111111, 1111111, 11111111, 111111111};
    int leftZero(int n) const {
        int res = 0, i = 0;
        while (n) {
            i++;
            if (n % 10 == 0)
                res = i;
            n /= 10;
        }
        return res;
    }
    vector<int> _NoZero(int n) {
        int a = 0;
        for (int x = n; x > 1; x /= 10) {
            if (x % 10 > 1)
                break;
            a++;
        }
        return vector<int>{ALLONE[a] + 1, n - ALLONE[a] - 1};
    }
    vector<int> getNoZeroIntegers(int n) {
        int a = 0, b = n;
        int i = leftZero(b);
        if (i == 0)
            return _NoZero(b);

        while (true) {
            i = leftZero(b);
            if (i == 0)
                break;
            a += b % (ALLONE[i] - ALLONE[i - 1]) + 1;
            b = n - a;
            i = leftZero(a);
            if (i == 0)
                break;
            a = a - a % (ALLONE[i] - ALLONE[i - 1]) + ALLONE[i];
            b = n - a;
        }
        return vector<int>{a, b};
    }
};
```
### 专题训练
见[[贪心与思维（基本贪心策略,反悔,区间,字典序,数学,思维,脑筋急转弯,构造)]]的【构造题】。