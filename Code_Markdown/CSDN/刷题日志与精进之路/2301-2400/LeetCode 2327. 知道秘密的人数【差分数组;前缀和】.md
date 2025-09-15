> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

在第 `1` 天，有一个人发现了一个秘密。

给你一个整数 `delay` ，表示每个人会在发现秘密后的 `delay` 天之后，**每天** 给一个新的人 **分享** 秘密。同时给你一个整数 `forget` ，表示每个人在发现秘密 `forget` 天之后会 **忘记** 这个秘密。一个人 **不能** 在忘记秘密那一天及之后的日子里分享秘密。

给你一个整数 `n` ，请你返回在第 `n` 天结束时，知道秘密的人数。由于答案可能会很大，请你将结果对 `10^9 + 7` **取余** 后返回。

**示例 1：**
```c
输入：n = 6, delay = 2, forget = 4
输出：5
解释：
第 1 天：假设第一个人叫 A 。（一个人知道秘密）
第 2 天：A 是唯一一个知道秘密的人。（一个人知道秘密）
第 3 天：A 把秘密分享给 B 。（两个人知道秘密）
第 4 天：A 把秘密分享给一个新的人 C 。（三个人知道秘密）
第 5 天：A 忘记了秘密，B 把秘密分享给一个新的人 D 。（三个人知道秘密）
第 6 天：B 把秘密分享给 E，C 把秘密分享给 F 。（五个人知道秘密）
```
**示例 2：**
```c
输入：n = 4, delay = 1, forget = 3
输出：6
解释：
第 1 天：第一个知道秘密的人为 A 。（一个人知道秘密）
第 2 天：A 把秘密分享给 B 。（两个人知道秘密）
第 3 天：A 和 B 把秘密分享给 2 个新的人 C 和 D 。（四个人知道秘密）
第 4 天：A 忘记了秘密，B、C、D 分别分享给 3 个新的人。（六个人知道秘密）
```
**提示：**
- `2 <= n <= 1000`
- `1 <= delay < forget <= n`
---
### 理解提议
恰好在第 $i$ 天得知秘密的人，会在 $[i +delay, i + forget -1]$ 中的每一天分享秘密，每天给一个新的人分享秘密。

新的得知秘密的人，会按照同样的规则，继续分享秘密。

在第 $i$ 天得知秘密的人，会在第 $i + forget$ 天忘记秘密。

我们计算的是第 $n$ 天结束时，还没有忘记秘密的人数。
### 方法一 差分数组
在第 $1$ 天，有 $1$ 个人知道了秘密。对于 $[1 + delay, forget]$ 中的每个整数 $j$ ，都会新增 $1$ 个【恰好在第 $j$ 天得知秘密的人】。

考虑维护一个数组 $known$ ，其中 $known[i]$ 表示**恰好**在第 $i$ 天得知秘密的人数。为什么是恰好？如果不这样定义，第 $i$ 天各种人（刚知道秘密，昨天知道秘密，前天知道秘密，……）混在一起，不好处理。

根据题意，恰好在第 $i$ 天得知秘密的人，会在第 $j = i +delay, i + delay + 1, \dots, i + forget - 1$ 天分享秘密，也就是把 $known[j]$ 增加 $known[i]$ 。

初始值 $known[1] = 1$ 。

答案为第 $n$ 天没有忘记秘密的人数。这要求 $i + forget - 1 \ge n$ ，解得 $i \ge n - forget + 1$ ，所以答案为 $known$ 中的 $[\max (n - forget + 1, 1), n]$ 的元素和。

注意取模。

**优化前（不用差分数组）**
```java
class Solution {
    public int peopleAwareOfSecret(int n, int delay, int forget) {
        final int MOD = 1_000_000_007;
        // known[i] 表示恰好在第 i 天得知秘密的人数
        int[] known = new int[n + 1];
        known[1] = 1;
        long ans = 0;

        for (int i = 1; i <= n; i++) {
            // 统计在第 n 天没有忘记秘密的人数
            // 这要求 i+forget-1 >= n，解得 i >= n-forget+1
            if (i >= n - forget + 1) {
                ans += known[i];
            }
            // 恰好在第 i 天得知秘密的人，会在第 [i+delay, i+forget-1] 天分享秘密
            for (int j = i + delay; j <= Math.min(i + forget - 1, n); j++) {
                known[j] = (known[j] + known[i]) % MOD; // known[j] += known[i]
            }
        }

        return (int) (ans % MOD);
    }
}
```
```cpp
class Solution {
public:
    int peopleAwareOfSecret(int n, int delay, int forget) {
        const int MOD = 1'000'000'007;
        // known[i] 表示恰好在第 i 天得知秘密的人数
        vector<int> known(n + 1);
        known[1] = 1;
        long long ans = 0;

        for (int i = 1; i <= n; i++) {
            // 统计在第 n 天没有忘记秘密的人数
            // 这要求 i+forget-1 >= n，解得 i >= n-forget+1
            if (i >= n - forget + 1) {
                ans += known[i];
            }
            // 恰好在第 i 天得知秘密的人，会在第 [i+delay, i+forget-1] 天分享秘密
            for (int j = i + delay; j <= min(i + forget - 1, n); j++) {
                known[j] = (known[j] + known[i]) % MOD; // known[j] += known[i]
            }
        }

        return ans % MOD;
    }
};
```
```rust
impl Solution {
    pub fn people_aware_of_secret(n: i32, delay: i32, forget: i32) -> i32 {
        const MOD: i32 = 1_000_000_007;
        let n = n as usize;
        let delay = delay as usize;
        let forget = forget as usize;
        // known[i] 表示恰好在第 i 天得知秘密的人数
        let mut known = vec![0; n + 1];
        known[1] = 1;
        let mut ans = 0;

        for i in 1..=n {
            // 统计在第 n 天没有忘记秘密的人数
            // 这要求 i+forget-1 >= n，解得 i >= n-forget+1
            if i >= n - forget + 1 {
                ans = (ans + known[i]) % MOD;
            }
            // 恰好在第 i 天得知秘密的人，会在第 [i+delay, i+forget-1] 天分享秘密
            for j in i + delay..=n.min(i + forget - 1) {
                known[j] = (known[j] + known[i]) % MOD; // known[j] += known[i]
            }
        }

        ans
    }
}
```
```python
class Solution:
    def peopleAwareOfSecret(self, n: int, delay: int, forget: int) -> int:
        MOD = 1_000_000_007
        # known[i] 表示恰好在第 i 天得知秘密的人数
        known = [0] * (n + 1)
        known[1] = 1

        for i in range(1, n + 1):
            known[i] %= MOD
            # 恰好在第 i 天得知秘密的人，会在第 [i+delay, i+forget-1] 天分享秘密
            for j in range(i + delay, min(i + forget, n + 1)):
                known[j] += known[i]

        # 统计在第 n 天没有忘记秘密的人数
        # 这要求 i+forget-1 >= n，解得 i >= n-forget+1
        return sum(known[-forget:]) % MOD
```
```c
#define MOD 1000000007
#define MIN(a, b) ((b) < (a) ? (b) : (a))

int peopleAwareOfSecret(int n, int delay, int forget){
    // known[i] 表示恰好在第 i 天得知秘密的人数
    int* known = calloc(n + 1, sizeof(int));
    known[1] = 1;
    long long ans = 0;

    for (int i = 1; i <= n; i++) {
        // 统计在第 n 天没有忘记秘密的人数
        // 这要求 i+forget-1 >= n，解得 i >= n-forget+1
        if (i >= n - forget + 1) {
            ans += known[i];
        }
        // 恰好在第 i 天得知秘密的人，会在第 [i+delay, i+forget-1] 天分享秘密
        for (int j = i + delay; j <= MIN(i + forget - 1, n); j++) {
            known[j] = (known[j] + known[i]) % MOD; // known[j] += known[i]
        }
    }

    free(known);
    return (int) (ans % MOD);
}
```
```go
func peopleAwareOfSecret(n, delay, forget int) (ans int) {
	const mod = 1_000_000_007
	// known[i] 表示恰好在第 i 天得知秘密的人数
	known := make([]int, n+1)
	known[1] = 1

	for i := 1; i <= n; i++ {
		known[i] %= mod
		// 统计在第 n 天没有忘记秘密的人数
		// 这要求 i+forget-1 >= n，解得 i >= n-forget+1
		if i >= n-forget+1 {
			ans += known[i]
		}
		// 恰好在第 i 天得知秘密的人，会在第 [i+delay, i+forget-1] 天分享秘密
		for j := i + delay; j <= min(i+forget-1, n); j++ {
			known[j] += known[i]
		}
	}

	return ans % mod
}
```
```js
var peopleAwareOfSecret = function(n, delay, forget) {
    const MOD = 1_000_000_007;
    // known[i] 表示恰好在第 i 天得知秘密的人数
    const known = Array(n + 1).fill(0);
    known[1] = 1;
    let ans = 0;

    for (let i = 1; i <= n; i++) {
        known[i] %= MOD;
        // 统计在第 n 天没有忘记秘密的人数
        // 这要求 i+forget-1 >= n，解得 i >= n-forget+1
        if (i >= n - forget + 1) {
            ans += known[i];
        }
        // 恰好在第 i 天得知秘密的人，会在第 [i+delay, i+forget-1] 天分享秘密
        for (let j = i + delay; j <= Math.min(i + forget - 1, n); j++) {
            known[j] += known[i];
        }
    }

    return ans % MOD;
};
```
复杂度分析：
- 时间复杂度：$O(n ( forget - delay))$ 。
- 空间复杂度：$O(n)$ 。

**优化**
上面有个【把子数组的每个元素都增加 $known[i]$ 】的逻辑，这可以用**差分数组**优化。

$diff$ 数组是 $known$ 数组的差分数组。对于初始值 $known[1] = 1$ ，对应到差分数组上，就是 $diff[1] = 1$ 以及 $diff[2] = -1$ 。
```java
class Solution {
    public int peopleAwareOfSecret(int n, int delay, int forget) {
        final int MOD = 1_000_000_007;
        int[] diff = new int[n + 1];
        diff[1] = 1;
        diff[2] = -1;
        int known = 0;
        long ans = 0;

        for (int i = 1; i <= n; i++) {
            // 加上 diff[i] 后，known 表示恰好在第 i 天得知秘密的人数
            known = (known + diff[i]) % MOD;
            // 统计在第 n 天没有忘记秘密的人数
            if (i >= n - forget + 1) {
                ans += known;
            }
            // 恰好在第 i 天得知秘密的人，会在第 [i+delay, i+forget-1] 天分享秘密
            if (i + delay <= n) {
                diff[i + delay] = (diff[i + delay] + known) % MOD;
            }
            if (i + forget <= n) {
                diff[i + forget] = (diff[i + forget] - known + MOD) % MOD; // +MOD 保证结果非负
            }
        }
        return (int) (ans % MOD);
    }
}
```
```cpp
class Solution {
public:
    int peopleAwareOfSecret(int n, int delay, int forget) {
        const int MOD = 1'000'000'007;
        vector<int> diff(n + 1);
        diff[1] = 1;
        diff[2] = -1;
        int known = 0;
        long long ans = 0;

        for (int i = 1; i <= n; i++) {
            // 加上 diff[i] 后，known 表示恰好在第 i 天得知秘密的人数
            known = (known + diff[i]) % MOD;
            // 统计在第 n 天没有忘记秘密的人数
            if (i >= n - forget + 1) {
                ans += known;
            }
            // 恰好在第 i 天得知秘密的人，会在第 [i+delay, i+forget-1] 天分享秘密
            if (i + delay <= n) {
                diff[i + delay] = (diff[i + delay] + known) % MOD;
            }
            if (i + forget <= n) {
                diff[i + forget] = (diff[i + forget] - known + MOD) % MOD; // +MOD 保证结果非负
            }
        }

        return ans % MOD;
    }
};
```
```rust
impl Solution {
    pub fn people_aware_of_secret(n: i32, delay: i32, forget: i32) -> i32 {
        const MOD: i32 = 1_000_000_007;
        let n = n as usize;
        let delay = delay as usize;
        let forget = forget as usize;
        let mut diff = vec![0; n + 1];
        diff[1] = 1;
        diff[2] = -1;
        let mut known = 0;
        let mut ans = 0;

        for i in 1..=n {
            // 加上 diff[i] 后，known 表示恰好在第 i 天得知秘密的人数
            known = (known + diff[i]) % MOD;
            // 统计在第 n 天没有忘记秘密的人数
            if i >= n - forget + 1 {
                ans = (ans + known) % MOD;
            }
            // 恰好在第 i 天得知秘密的人，会在第 [i+delay, i+forget-1] 天分享秘密
            if i + delay <= n {
                diff[i + delay] = (diff[i + delay] + known) % MOD;
            }
            if i + forget <= n {
                diff[i + forget] = (diff[i + forget] - known + MOD) % MOD; // +MOD 保证结果非负
            }
        }
        ans
    }
}
```
```python
class Solution:
    def peopleAwareOfSecret(self, n: int, delay: int, forget: int) -> int:
        MOD = 1_000_000_007
        diff = [0] * (n + 2)
        diff[1] = 1
        diff[2] = -1
        ans = known = 0

        for i in range(1, n + 1):
            # 加上 diff[i] 后，known 表示恰好在第 i 天得知秘密的人数
            known = (known + diff[i]) % MOD
            # 统计在第 n 天没有忘记秘密的人数
            if i >= n - forget + 1:
                ans += known
            # 恰好在第 i 天得知秘密的人，会在第 [i+delay, i+forget-1] 天分享秘密
            diff[min(i + delay, n + 1)] += known
            diff[min(i + forget, n + 1)] -= known

        return ans % MOD
```
```c
#define MOD 1000000007

int peopleAwareOfSecret(int n, int delay, int forget) {
    int* diff = calloc(n + 1, sizeof(int));
    diff[1] = 1;
    diff[2] = -1;
    int known = 0;
    long long ans = 0;

    for (int i = 1; i <= n; i++) {
        // 加上 diff[i] 后，known 表示恰好在第 i 天得知秘密的人数
        known = (known + diff[i]) % MOD;
        // 统计在第 n 天没有忘记秘密的人数
        if (i >= n - forget + 1) {
            ans = (ans + known) % MOD;
        }
        // 恰好在第 i 天得知秘密的人，会在第 [i+delay, i+forget-1] 天分享秘密
        if (i + delay <= n) {
            diff[i + delay] = (diff[i + delay] + known) % MOD;
        }
        if (i + forget <= n) {
            diff[i + forget] = (diff[i + forget] - known + MOD) % MOD; // +MOD 保证结果非负
        }
    }

    free(diff);
    return ans % MOD;
}
```
```go
func peopleAwareOfSecret(n, delay, forget int) (ans int) {
	const mod = 1_000_000_007
	diff := make([]int, n+2)
	diff[1] = 1
	diff[2] = -1
	known := 0

	for i := 1; i <= n; i++ {
		// 加上 diff[i] 后，known 表示恰好在第 i 天得知秘密的人数
		known = (known + diff[i]) % mod
		// 统计在第 n 天没有忘记秘密的人数
		if i >= n-forget+1 {
			ans += known
		}
		// 恰好在第 i 天得知秘密的人，会在第 [i+delay, i+forget-1] 天分享秘密
		diff[min(i+delay, n+1)] += known
		diff[min(i+forget, n+1)] -= known // 注意这里有减法，这会导致上面累加 diff[i] 时，known 可能是负数
	}

	return (ans%mod + mod) % mod // 保证答案非负
}
```
```js
var peopleAwareOfSecret = function(n, delay, forget) {
    const MOD = 1_000_000_007;
    const diff = Array(n + 2).fill(0);
    diff[1] = 1;
    diff[2] = -1;
    let known = 0;
    let ans = 0;

    for (let i = 1; i <= n; i++) {
        // 加上 diff[i] 后，known 表示恰好在第 i 天得知秘密的人数
        known = (known + diff[i]) % MOD;
        // 统计在第 n 天没有忘记秘密的人数
        if (i >= n - forget + 1) {
            ans += known;
        }
        // 恰好在第 i 天得知秘密的人，会在第 [i+delay, i+forget-1] 天分享秘密
        diff[Math.min(i + delay, n + 1)] += known;
        diff[Math.min(i + forget, n + 1)] -= known; // 注意这里有减法，这会导致上面累加 diff[i] 时，known 可能是负数
    }

    return (ans % MOD + MOD) % MOD; // 保证答案非负
};
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(n)$

---
### 方法二 前缀和
横看成岭侧成峰，对于 $known[j]$ 来说，它会被哪些 $known[i]$ 更新？

根据 $i$ 和 $j$ 的关系：$$i + delay \le j \le i + forget - 1$$
解得：$$j - forget + 1 \le i \le j - delay$$
那么计算 $[j - forget + 1, j - delay]$ 的子数组和，就能直接得到 $known[j]$ 。这一子数组和，可以用**前缀和**优化。
```java
class Solution {
    public int peopleAwareOfSecret(int n, int delay, int forget) {
        final int MOD = 1_000_000_007;
        int[] sum = new int[n + 1]; // known 数组的前缀和
        sum[1] = 1;

        for (int j = 2; j <= n; j++) {
            int known = (sum[Math.max(j - delay, 0)] - sum[Math.max(j - forget, 0)]) % MOD;
            sum[j] = (sum[j - 1] + known) % MOD;
        }

        int ans = sum[n] - sum[Math.max(n - forget, 0)];
        return (ans % MOD + MOD) % MOD; // 保证答案非负
    }
}
```
```cpp

```
```rust

```
```python
class Solution:
    def peopleAwareOfSecret(self, n: int, delay: int, forget: int) -> int:
        MOD = 1_000_000_007
        s = [0] * (n + 1)  # known 数组的前缀和
        s[1] = 1

        for j in range(2, n + 1):
            known = s[max(j - delay, 0)] - s[max(j - forget, 0)]
            s[j] = (s[j - 1] + known) % MOD

        return (s[n] - s[max(n - forget, 0)]) % MOD
```
```c
#define MOD 1000000007
#define MAX(a, b) ((b) > (a) ? (b) : (a))

int peopleAwareOfSecret(int n, int delay, int forget){
    int* sum = malloc((n + 1) * sizeof(int)); // known 数组的前缀和
    sum[0] = 0;
    sum[1] = 1;

    for (int j = 2; j <= n; j++){
        int known = (sum[MAX(j - delay, 0)] - sum[MAX(j - forget, 0)]) % MOD;
        sum[j] = (sum[j - 1] + known) % MOD;
    }

    int ans = sum[n] - sum[MAX(n - forget, 0)];
    free(sum);
    return (ans % MOD + MOD) % MOD; // 保证答案非负
}
```
```go
func peopleAwareOfSecret(n, delay, forget int) int {
	const mod = 1_000_000_007
	sum := make([]int, n+1) // known 数组的前缀和
	sum[1] = 1

	for j := 2; j <= n; j++ {
		known := sum[max(j-delay, 0)] - sum[max(j-forget, 0)]
		sum[j] = (sum[j-1] + known) % mod
	}

	ans := sum[n] - sum[max(n-forget, 0)]
	return (ans%mod + mod) % mod // 保证答案非负
}
```
```js
var peopleAwareOfSecret = function(n, delay, forget) {
    const MOD = 1_000_000_007;
    const sum = Array(n + 1).fill(0); // known 数组的前缀和
    sum[1] = 1;

    for (let j = 2; j <= n; j++) {
        const known = sum[Math.max(j - delay, 0)] - sum[Math.max(j - forget, 0)];
        sum[j] = (sum[j - 1] + known) % MOD;
    }

    const ans = sum[n] - sum[Math.max(n - forget, 0)];
    return (ans % MOD + MOD) % MOD; // 保证答案非负
};
```
复杂度分析：
- 时间复杂度：$O(n)$ 。
- 空间复杂度：$O(n)$ 。
---
### 专题训练
1. 数据结构题单中的【一、前缀和】和【2.1 二维差分】
2. 动态规划题单中的【11.1 前缀和优化DP】