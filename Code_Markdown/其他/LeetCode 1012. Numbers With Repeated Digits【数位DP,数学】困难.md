> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an integer `n`, return _the number of positive integers in the range_ `[1, n]` _that have **at least one** repeated digit_.

**Example 1:**
```java
Input: n = 20
Output: 1
Explanation: The only positive number (<= 20) with at least 1 repeated digit is 11.
```
**Example 2:**
```java
Input: n = 100
Output: 10
Explanation: The positive numbers (<= 100) with atleast 1 repeated digit are 11, 22, 33, 44, 55, 66, 77, 88, 99, and 100.
```
**Example 3:**
```java
Input: n = 1000
Output: 262
```
**Constraints:**
-   `1 <= n <= 10^9`

题意：给定正整数 `n`，返回在 `[1, n]` 范围内具有 **至少 1 位** 重复数字的正整数的个数。

---
### 解法 状压+记忆化搜索（数位DP）
相似题目（基本可用通用数位DP模板来解决，但233和17.06可直接用计数原理解决，更简单）：
- [233. 数字 1 的个数](https://leetcode.cn/problems/number-of-digit-one/)（题解：[[CSDN/刷题日志与精进之路/LeetCode 233. Number of Digit One【计数模拟】困难]]）
- [面试题 17.06. 2出现的次数](https://leetcode.cn/problems/number-of-2s-in-range-lcci/)（题解：[[CSDN/刷题日志与精进之路/LeetCode 面试题 17.06. 2出现的次数【数位DP,计数原理】困难]]）
- [600. 不含连续1的非负整数](https://leetcode.cn/problems/non-negative-integers-without-consecutive-ones/)（题解：[[CSDN/刷题日志与精进之路/LeetCode 600. Non-negative Integers without Consecutive Ones【数位DP,斐波那契,位运算】困难]]）
- [902. 最大为 N 的数字组合](https://leetcode.cn/problems/numbers-at-most-n-given-digit-set/)（题解：[]）
- [1067. 范围内的数字计数](https://leetcode.cn/problems/digit-count-in-range/)（会员题）
- [1397. 找到所有好字符串](https://leetcode.cn/problems/find-all-good-strings/)（有难度，需要结合一个经典字符串算法）
- 更多题目见大佬[灵茶山艾府]模板库中的 [dp.go](https://leetcode.cn/link/?target=https%3A%2F%2Fgithub.com%2FEndlessCheng%2Fcodeforces-go%2Fblob%2Fmaster%2Fcopypasta%2Fdp.go%23L1924)。

用二进制表示集合，二进制从低到高第 $i$ 位为 $1$ 、表示 $i$ 在集合中，为 $0$ 表示 $i$ 不在集合中。​
 。设集合对应的二进制数为 $x$ ，两个位运算操作如下：
- 判断元素 $d$ 是否在集合中：`x >> d & 1` 可以取出 $x$ 的第 $d$ 个比特位，如果是 $1$ 就说明 $d$ 在集合中。
- 把元素 $d$ 添加到集合中：将 `x` 更新为 `x | (1 << d)` 。

求至少有一个重复数位的数字个数有点难，**不如正难则反，转换成求无重复数位数字的个数**。答案等于 $n$ 减去无重复数字的个数。将 $n$ 转换成字符串 $s$ ，**定义 $f(i,\textit{mask}, \textit{isNum},\textit{isLimit})$ 表示构造第 $i$ 位及其之后数位的合法方案数**，参数的含义为：
- $i$ 表示从第 $i$ 位开始填数字，**数位DP通用模板必备参数**！
- $\textit{mask}$ 表示前面选过的数字集合，换句话说，第 $i$ 位要选的数字不能在 $\textit{mask}$ 中，这是==为选出无重复数字做准备==。
- $\textit{isNum}$ 表示 $i$ 前面的数位是否填了数字。若为假表示前面没填数字，则当前位可以不填数字，或者要填入的数字至少为 $1$ ；若为真表示前面填了数字，则必须要填入从 $0$ 开始的数字。例如 $n=123$ ，在 $i=0$ 时跳过不填，相当于后面要构造的是一个 $99$ 以内的数字了，如果 $i=1$ 不跳过，那么相当于构造一个 $10$ 到 $99$ 的两位数，如果 $i=1$ 跳过，相当于构造的是一个 $9$ 以内的数字。其实是统一的，$isNum$ 为假时当前位可填 $0$（不填数字）或 $1$ 及以上（填数字），为真时必须填从 $0$ 开始的数字。==$isNum$ 的真实用处在于递归结束时判断填成的数是否不为0==（全跳过或者说全填 $0$ 就是 $0$ ，不能算有重复数位）。
- $\textit{isLimit}$ 表示当前是否受到了 $n$ 的约束（即==要构造的数字不能超过 $n$**==）。若为真，则第 $i$ 位填入的数字至多为 $s[i]$ ，否则可以是 $9$** 。**数位DP通用模板必备参数**！如果在受到约束的情况下填了 $s[i]$ ，那么后续填入的数字仍会受到 $n$ 的约束。例如 $n=123$ ，那么 $i=0$ 填的是 $1$ 的话，$i=1$ 的这一位至多填 $2$ 。

实现细节如下：
- 递归入口 `f(0, 0, true, false)` 表示：
	- 从 $s[0]$ 开始枚举；
	- 一开始集合中没有数字；
	- 一开始要受到 $n$ 的约束（否则就可以随意填了，这肯定不行）；
	- 一开始没有填数字。
- 递归中：
	- 如果 $\textit{isNum}$ 为假，说明前面没有填数字，那么**当前也可以不填数字，一旦从这里递归下去，$\textit{isLimit}$ 就可以置为 `false` 了**，因为前面的高位不填数字，后面无论怎么填都比 $n$ 小。
	- 如果 $\textit{isNum}$ 为真，那么当前必须填一个数字。**枚举填入的数字，要根据 $\textit{isNum}$ 和 $\textit{isLimit}$ 来决定填入数字的范围**。
	- 填了数字后，从这里递归下去，$isNum$ 置为真，且看情况决定 $isLimit$ 的值——如果现在的 $isLimit$ 为假，则递归下去也是假；如果现在的 $isLimit$ 为真，但填的数字小于 $s[i]$ ，则要将 $isLimit$ 置为假递归下去，否则后面的 $isLimit$ 仍为真。
- 递归终点：当 $i$ 等于 $s$ 长度时，如果 $\textit{isNum}$ 为真，则表示得到了一个合法数字（因为不合法的不会继续递归下去），返回 $1$ （即一个方案），否则返回 $0$ 。
```cpp
class Solution {
public:
    int numDupDigitsAtMostN(int n) {
        string s = to_string(n);
        // f(i,mask,isNum,isLimit)表示计算第i位及以后的合法方案数
        // 这里先计算无重复数字的正整数的个数,因此用mask表示已经使用了哪些数字
        // isNum则表示前面是否是数字,即前面填了数字没有,填了前面就是数字为true,否则前面不是数字为false
        // isLimit表示是否受到了n的约束,为true表示受到n的约束,即第i位填的数<=s[i];为false表示不受到s[i]约束,最大能填9
        // isNum为true,前面填了数字,则这里必须填数字,从0开始,看是否受到限制来填数字
        // isNum为false,前面没填数字,则这里也可不填数字,此后isNum还是false,isLimit为false(因为前面必小于s[i]前面);或者从1填起来,看是否受到限制来填数字
        // 填数字后,isNum变为true,看情况决定isLimit是否为true(现在受到限制&&填的数字是否等于s[i])
        // 如果现在不受限制,以后也不受限制;如果现在受限制,但填的数小于s[i],则后面不受限制;否则后面要受到限制
        int m = s.size(), dp[m][1 << 10][2][2];
        memset(dp, -1, sizeof(dp));
        function<int(int, int, bool, bool)> f = [&](int i, int mask, bool isNum, bool isLimit) -> int {
            if (i >= m) return isNum; // 为true表示是一个合法数字,否则不是
            if (dp[i][mask][isNum][isLimit] != -1)
                return dp[i][mask][isNum][isLimit];
            int ans = 0;
            if (!isNum) // 当前数位可以不填数字
                ans += f(i + 1, mask, false, false); // 后面不受限制了
            // 下面开始填数字
            int lower = isNum ? 0 : 1, upper = isLimit ? s[i] - '0' : 9;
            for (int d = lower; d <= upper; ++d) // 枚举要填入的数字
                if ((mask >> d & 1) == 0) // i前面没有使用,这里可用
                    ans += f(i + 1, mask | (1 << d), true, isLimit && d == upper);
            // 当前位填数字和不填数字得到的合法方案数都考虑了
            return dp[i][mask][isNum][isLimit] = ans;
        };
        return n - f(0, 0, false, true);
    }
};
```
> 大佬的解答：
> 问：记忆化四个状态有点麻烦，能不能只记忆化 $(i,\textit{mask})$ 这两个状态？
> 答：是可以的。比如 $n=234$ ，第一位填 $2$ ，第二位填 $3$ ，**后面无论怎么递归，都不会再次递归到第一位填 $2$ ，第二位填 $3$ 的情况**，所以不需要记录。又比如，第一位不填，第二位也不填，**后面无论怎么递归也不会再次递归到这种情况**，所以也不需要记录。
> 根据这个例子，**我们可以只记录<u><b>不受到</b></u> $\textit{isLimit}$ 或 $\textit{isNum}$ 约束时的状态 $(i,\textit{mask})$** 。比如 $n=234$ ，第一位（最高位）填的 $1$ ，那么继续递归，后面就可以随便填，所以状态 $(1,2)$ 就表示前面填了一个 $1$（对应的 $\textit{mask}$ ），从第二位往后随便填的方案数。
> 
> 问：$\textit{isNum}$ 这个参数可以去掉吗？
> 答：对于本题是可以的。由于 $\textit{mask}$ 中记录了数字，**可以通过判断 $\textit{mask}$ 是否为 $0$ 来判断前面是否填了数字**，所以 $\textit{isNum}$ 可以省略。代码保留了 $\textit{isNum}$ ，主要是为了方便大家掌握这个模板。因为有些题目不需要 $\textit{mask}$ ，但需要 $\textit{isNum}$ 。
> 
> 问：能不能只记忆化 $i$ ？
> 答：这是不行的。想一想，我们为什么要用记忆化？如果递归到同一个状态时，计算出的结果是一样的，那么第二次递归到同一个状态，就可以直接返回第一次计算的结果了。通过保存第一次计算的结果，来优化时间复杂度。
> 由于前面选的数字会影响后面选的数字，两次递归到相同的 $i$ ，如果前面选的数字不一样，计算出的结果就可能是不一样的。如果只记忆化 $i$ ，就可能会算出错误的结果。
> 也可以这样理解：记忆化搜索要求递归函数无副作用（除了修改 $dp$ 数组），从而保证递归到同一个状态时，计算出的结果是一样的。
 
```cpp
class Solution {
public:
    int numDupDigitsAtMostN(int n) {
        string s = to_string(n);
        int m = s.size(), dp[m][1 << 10];
        memset(dp, -1, sizeof(dp));
        function<int(int, int, bool, bool)> f = [&](int i, int mask, bool isNum, bool isLimit) -> int {
            if (i >= m) return isNum; // 为true表示是一个合法数字,否则不是
            if (isNum && !isLimit && dp[i][mask] != -1)
                return dp[i][mask];
            int ans = 0;
            if (!isNum) // 当前数位可以不填数字
                ans += f(i + 1, mask, false, false); // 后面不受限制了
            // 下面开始填数字
            int lower = isNum ? 0 : 1, upper = isLimit ? s[i] - '0' : 9;
            for (int d = lower; d <= upper; ++d) // 枚举要填入的数字
                if ((mask >> d & 1) == 0) // i前面没有使用,这里可用
                    ans += f(i + 1, mask | (1 << d), true, isLimit && d == upper);
            if (isNum && !isLimit)
	            dp[i][mask] = ans;
            // 当前位填数字和不填数字得到的合法方案数都考虑了
            return ans;
        };
        return n - f(0, 0, false, true);
    }
};
```
复杂度分析：
- 时间复杂度：$O(mD2^D)$ ，其中 $m$ 为 $s$ 的长度，即 $O(\log n)$ ；$D=10$ 。由于**每个状态只会计算一次**，因此动态规划的时间复杂度 = 状态个数 $\times$ 单个状态的计算时间。本题状态个数为 $O(m2^D)$ ，单个状态的计算时间为 $O(D)$ ，因此时间复杂度为 $O(mD2^D)$ 。
- 空间复杂度：$O(m2^D)$ 。 
 
 
