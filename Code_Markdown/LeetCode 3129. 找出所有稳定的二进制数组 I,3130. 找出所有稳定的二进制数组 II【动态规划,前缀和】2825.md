> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

> 3129题仅在数据范围上与本题不同。
# [3130. 找出所有稳定的二进制数组 II](https://leetcode.cn/problems/find-all-possible-stable-binary-arrays-ii/)
给你 3 个正整数 `zero` ，`one` 和 `limit` 。一个二进制数组 `arr` 如果满足以下条件，那么我们称它是 **稳定的** ：
- 0 在 `arr` 中出现次数 **恰好** 为 `zero` 。
- 1 在 `arr` 中出现次数 **恰好** 为 `one` 。
- `arr` 中每个长度超过 `limit` 的子数组都 **同时** 包含 0 和 1 。

请你返回 **稳定** 二进制数组的 _总_ 数目。

由于答案可能很大，将它对 `109 + 7` **取余** 后返回。

**示例 1：**
```c
输入：zero = 1, one = 1, limit = 2
输出：2
解释：
两个稳定的二进制数组为 `[1,0]` 和 `[0,1]` ，两个数组都有一个 0 和一个 1 ，且没有子数组长度大于 2 。
```
**示例 2：**
```c
输入：zero = 1, one = 2, limit = 1
输出：1
解释：
唯一稳定的二进制数组是 `[1,0,1]` 。

二进制数组 `[1,1,0]` 和 `[0,1,1]` 都有长度为 2 且元素全都相同的子数组，所以它们不稳定。
```
**示例 3：**
```cpp
输入：zero = 3, one = 3, limit = 2
输出：14
解释：
所有稳定的二进制数组包括 `[0,0,1,0,1,1]` ，`[0,0,1,1,0,1]` ，`[0,1,0,0,1,1]` ，`[0,1,0,1,0,1]` ，`[0,1,0,1,1,0]` ，`[0,1,1,0,0,1]` ，`[0,1,1,0,1,0]` ，`[1,0,0,1,0,1]` ，`[1,0,0,1,1,0]` ，`[1,0,1,0,0,1]` ，`[1,0,1,0,1,0]` ，`[1,0,1,1,0,0]` ，`[1,1,0,0,1,0]` 和 `[1,1,0,1,0,0]` 。
```
**提示：**
- `1 <= zero, one, limit <= 1000`

---
### 方法一：记忆化搜索
先解释 $limit$ ，意思是数组中**至多有连续 $limit$ 个 $0$** ，且**至多有连续 $limit$ 个 $1$** 。

看示例 3，$zero=3, one=3, limit=2$ 。考虑稳定数组的最后一个位置填 $0$ 还是 $1$：
- 填 $0$ ，问题变成剩下 $2$ 个 $0$ 和 $3$ 个 $1$ 怎么填。
- 填 $1$ ，问题变成剩下 $3$ 个 $0$ 和 $2$ 个 $1$ 怎么填。

这两个都是**和原问题相似的子问题**。

看上去，定义 $dfs(i,j)$ 表示用 $i$ 个 $0$ 和 $j$ 个 $1$ 构造稳定数组的方案数？但这样定义不方便计算 $limit$ 带来的影响。

改成**定义 $dfs(i,j,k)$ 表示用 $i$ 个 $0$ 和 $j$ 个 $1$ 构造稳定数组的方案数，其中第 $i+j$ 个位置要填 $k$ ，其中 $k$ 为 $0$ 或 $1$** 。

考虑 $dfs(i,j,0)$ 怎么算。现在，第 $i+j$ 个位置填的是 $0$ ，考虑第 $i+j−1$ 个位置要填什么：
- 填 $0$ ，方案数就是 $dfs(i−1,j,0)$ 。
- 填 $1$ ，方案数就是 $dfs(i−1,j,1)$ 。

看上去，把这两种情况加起来，我们就得到了 $dfs(i,j,0)$ 。但是，**这个过程中会产生不合法的情况**。

假设 $limit=3$ ，$dfs(i−1,j,0)$ 是一些以 $0$ 结尾的稳定数组（合法数组），考虑末尾 $0$ 的个数：
- 末尾有 $1$ 个 $0$ ：那么末尾必定是 $10$ 。
- 末尾有 $2$ 个 $0$ ：那么末尾必定是 $100$ 。
- 末尾有 $3$ 个 $0$ ：那么末尾必定是 $1000$ 。注意，由于末尾不能超过 $3$ 个 $0$ ，所以这样的稳定数组的倒数第 $4$ 个数一定是 $1$ ，也就是**在 $dfs(i−1,j,0)$ 中有 $dfs(i−4,j,1)$ 个末尾是 $1000$ 的稳定数组**。

若要通过 $dfs(i−1,j,0)$ 计算 $dfs(i,j,0)$ ，相当于往这 $dfs(i−1,j,0)$ 个稳定数组的末尾再加一个 $0$ ：
- 末尾有 $2$ 个 $0$ ，即 $100$ ，这是合法的。
- 末尾有 $3$ 个 $0$ ，即 $1000$ ，这是合法的。
- 末尾有 $4$ 个 $0$ ，即 $10000$ ，这是不合法的，**要全部去掉**！根据上面的分析，要减去 $dfs(i−4,j,1)$ 。

一般地，因为 $dfs$ 的定义是稳定数组的方案数，只包含合法方案，所以**在最后连续 $limit$ 个位置填 $0$ 的情况下，倒数第 $limit+1$ 个位置一定要填 $1$**，这个位置填 $0$ 的有 $dfs(i−limit−1,j,1)$ 种方案，对于 $dfs(i,j,0)$ 来说，这 $dfs(i−limit−1,j,1)$ 个方案就是不合法方案了，要减掉，得
$$dfs(i,j,0)=dfs(i−1,j,0)+dfs(i−1,j,1)−dfs(i−limit−1,j,1)$$
同理得
$$dfs(i,j,1)=dfs(i,j−1,0)+dfs(i,j−1,1)−dfs(i,j−limit−1,0)$$
递归边界 $1$ ：如果 $i<0$ 或者 $j<0$ ，返回 $0$ 。也可以在递归 $dfs(i−limit−1,j,1)$ 前判断 $i>limit$ ，在递归 $dfs(i,j−limit−1,0)$ 前判断 $j>limit$ 。下面代码在递归前判断。

递归边界 $2$ ：如果 $i=0$ ，那么当 $k=1$ 且 $j≤limit$ 的情况下返回 $1$ ，否则返回 $0$ ；如果 $j=0$ ，那么当 $k=0$ 且 $i≤limit$ 的情况下返回 $1$ ，否则返回 $0$ 。

递归入口：$dfs(zero,one,0)+dfs(zero,one,1)$ ，即答案。




看完了灵神的题解后，这里我做一个阐述，主要解释每一步的由来与细节分析。他的视频中讲解的挺好，题解就没有那么全面，我来补充一下。

**题意**：二进制数组长为 zero+onezero + onezero+one，限制连续 000 和连续 111 的数量均不超过 limitlimitlimit，求这样的数组有几个。

假如有一个空数组，我拿某个元素去填充它：

- 取一个 000，问题变成剩下 zero−1zero-1zero−1 个 000 和 oneoneone 个 111 怎么构造

- 取一个 111，问题变成剩下 zerozerozero 个 000 和 one−1one-1one−1 个 111 怎么构造

这两种情况都是和原问题相似的子问题，问题的本质没有变化，只是规模由大变小。遇到这种相似子问题，第一时间要联想到记忆化搜索和动规。

记忆化搜索

**思路**：每次都在上一个构造的数组末尾再拼接一个数。

最开始的想法是定义 dfs(i,j)dfs(i,j)dfs(i,j) 表示还剩下 iii 个 000 和 jjj 个 111 时的方案数，我原来也是这样，让 limitlimitlimit 在内部进行剪枝，结果喜提超时。

在这种方法下，每次递归都审查一遍数组，看看有没有超过限制。能不能快速判断出是否合法？添加一个参数，表示最后一位填的是 000 或者 111。

那么，这种方法是如何加快速度的？首先，我们看看三个参数下的方程含义。定义 dfs(i,j,k)dfs(i,j,k)dfs(i,j,k) 表示用 iii 个 000 和 jjj 个 111 构造稳定数组的方案数，其中第 i+ji+ji+j 个位置要填 kkk，其中 kkk 为 000 或 111。

这种定义与最开始的有一点不同，表示的是第 i+ji+ji+j 个位置填数字，相当于是从后面向前面填充数字，不过整体思路没变，注意一下方向就成。

**状态转移推导**：

如果 k=0k=0k=0，考虑第 i+j−1i+j−1i+j−1 个位置要填什么：

- 填 0，问题变成 dfs(i−1,j,0)dfs(i−1,j,0)dfs(i−1,j,0)

- 填 1，问题变成 dfs(i−1,j,1)dfs(i−1,j,1)dfs(i−1,j,1)

等一下，为啥都是 i−1i-1i−1？仔细看看， k=0k=0k=0 表示第 i+ji+ji+j 的位置填 000，意思就是将 000 拼接在数组中，所以消耗了一个 000。

这种思路是 **逆推**，从后往前填，定义中强调的也是第 i+ji+ji+j 位置 **要** 填数。所以填充了 i+ji+ji+j 后，再去考虑 i+j−1i+j-1i+j−1，前者是已经做了，后者是可能选择。

同理 k=1k=1k=1，同样是两种可能。

这时候再来考虑 limitlimitlimit 的限制。假如一直在数组上拼接 000 或 111，会导致非法。如果费劲心思的去想当前填什么与前面的关系，那就违背了初衷，浪费大量时间。

**转换思路**，既然我担心非法，那我就找出所有非法的情况，排除它不就行了。不再拘泥于各种边界情况，防止非法的出现，跳脱一下，用 所有−非法所有-非法所有−非法 就是合法。

假如初始情况为 dfs(i,j,k)dfs(i,j,k)dfs(i,j,k)，此时连续填 limitlimitlimit 个 000，如果再多填一个 000 就非法，也就是说倒数第 limit+1limit+1limit+1 个位置必须填 111，即 dfs(i−limit−1,j,1)dfs(i-limit-1,j,1)dfs(i−limit−1,j,1)，这种情况才合法。

假如初始位置填 000，即 dfs(i,j,0)dfs(i,j,0)dfs(i,j,0) 时，相当于多填了一个 000，那上面的那个方案就不合法了，必须排除。转移方程为：

dfs(i,j,0)=dfs(i−1,j,0)+dfs(i−1,j,1)−dfs(i−limit−1,j,1)dfs(i,j,0)=dfs(i−1,j,0)+dfs(i−1,j,1)−dfs(i−limit−1,j,1)dfs(i,j,0)=dfs(i−1,j,0)+dfs(i−1,j,1)−dfs(i−limit−1,j,1)

有小伙伴肯定好奇，那 dfs(i−limit−1,j,0)dfs(i-limit-1,j,0)dfs(i−limit−1,j,0) 去哪里了？我们知道，定义的 dfsdfsdfs 函数是 **合法** 方案数。那么，已经填了 limitlimitlimit 个 000 时，怎么可能接着填 000？换句话来说， dfs(i−limit−1,j,0)dfs(i-limit-1,j,0)dfs(i−limit−1,j,0) 这种方案就是非法，根本就不存在这种定义。

那为什么减去 dfs(i−limit−1,j,0)dfs(i-limit-1,j,0)dfs(i−limit−1,j,0) 呢？这种方案是合法的，只不过在开始填 000 的情况下才非法，也就是说定义仍然存在，它是有数值的，完全可以放在 dfs(i,j,1)dfs(i,j,1)dfs(i,j,1) 中保持合法！

如初始位置填 111，即 dfs(i,j,1)dfs(i,j,1)dfs(i,j,1) 时，同理，转移方程为：

dfs(i,j,1)=dfs(i,j−1,0)+dfs(i,j−1,1)−dfs(i,j−limit−1,0)dfs(i,j,1)=dfs(i,j−1,0)+dfs(i,j−1,1)−dfs(i,j−limit−1,0)dfs(i,j,1)=dfs(i,j−1,0)+dfs(i,j−1,1)−dfs(i,j−limit−1,0)

**递归边界**：当所有的 000 或 111 用完时，后续只能填一种数字，返回的方案数为 111，不过要注意别出现连续 limitlimitlimit 的情况，即 iii 或 jjj 小于 limitlimitlimit。

**注意点** 111：减去非法的时候要注意别随便减，不能让 i−limit−1i-limit-1i−limit−1 出现负数，所以要进行提前判断再去进行递归。

**注意点** 222：使用 +MOD+MOD+MOD 保证答案非负。两个 dfsdfsdfs 相加后的结果可能小于后一个 dfsdfsdfs （取余导致的）。所以为了防止负数的出现，加上一个 MODMODMOD。比如 (−5+1_000_000_007)%1_000_000_007(-5 + 1\_000\_000\_007) \% 1\_000\_000\_007(−5+1_000_000_007)%1_000_000_007 就会得到 1_000_000_0021\_000\_000\_0021_000_000_002，这是 −5-5−5 在模 1_000_000_0071\_000\_000\_0071_000_000_007 下的正确正数表示。如果不进行 +MOD+MOD+MOD 操作，用例 570570570 通过不了。

**递归入口**： dfs(zero,one,0)+dfs(zero,one,1)dfs(zero,one,0)+dfs(zero,one,1)dfs(zero,one,0)+dfs(zero,one,1)，表示第一个数取值为 000 或 111。

代码来自灵神，附加了注释。

Python3

Java

class Solution:

    def numberOfStableArrays(self, zero: int, one: int, limit: int) -> int:

        # 记忆化搜索

        MOD = 1_000_000_007

        # 参数：还剩下i个0，j个1，第i+j个位置要填k的方案数

        @cache

        def dfs(i: int, j: int, k: int) -> int:

            # 没有0了，只能填1才是合法

            if i == 0:

                return 1 if k == 1 and j <= limit else 0

            # 没有1了，同理

            if j == 0:

                return 1 if k == 0 and i <= limit else 0

            # 第i+j个位置填0

            if k == 0:

                return (dfs(i - 1, j, 0) + dfs(i - 1, j, 1) - (dfs(i - limit - 1, j, 1) if i > limit else 0)) % MOD

            else:  # 第i+j个位置填1

                return (dfs(i, j - 1, 0) + dfs(i, j - 1, 1) - (dfs(i, j - limit - 1, 0) if j > limit else 0)) % MOD

        # 开始递归

        ans = (dfs(zero, one, 0) + dfs(zero, one, 1)) % MOD

        dfs.cache_clear()  # 防止爆内存

        return ans

- 时间复杂度： O(zero×one)O(zero\times one)O(zero×one)，动态规划的时间复杂度 = 状态个数 × 单个状态的计算时间。本题状态个数等于 O(zero×one)O(zero\times one)O(zero×one)，单个状态的计算时间为 O(1)O(1)O(1)，每个状态只计算一次，所以总时间复杂度为 O(zero×one)O(zero\times one)O(zero×one)

- 空间复杂度： O(zero×one)O(zero\times one)O(zero×one)。保存多少状态，就需要多少空间

动规递推

将递归 1:11:11:1 改造为递推。定义 f[i][j][k]f[i][j][k]f[i][j][k] 表示用 iii 个 000 和 jjj 个 111 构造稳定数组的方案数，其中第 i+ji+ji+j 个位置要填 kkk，其中 kkk 为 000 或 111。

**状态转移方程**：

f[i][j][0]=f[i−1][j][0]+f[i−1][j][1]−f[i−limit−1][j][1]f[i][j][1]=f[i][j−1][0]+f[i][j−1][1]−f[i][j−limit−1][0]​ f[i][j][0]=f[i−1][j][0]+f[i−1][j][1]−f[i−limit−1][j][1]\\ f[i][j][1]=f[i][j−1][0]+f[i][j−1][1]−f[i][j−limit−1][0] ​f[i][j][0]=f[i−1][j][0]+f[i−1][j][1]−f[i−limit−1][j][1]f[i][j][1]=f[i][j−1][0]+f[i][j−1][1]−f[i][j−limit−1][0]​

**边界**：将递归边界进行转换，得到 f[i][0][0]=f[0][j][1]=1f[i][0][0]=f[0][j][1]=1f[i][0][0]=f[0][j][1]=1。

**入口**：同理，改为 f[zero][one][0]+f[zero][one][1]f[zero][one][0]+f[zero][one][1]f[zero][one][0]+f[zero][one][1]。

注意点 111 进行修改，转为超过界限的视为 000，将初始空间设置更大防止下标越界。注意点 222 的 MODMODMOD 继续使用。

**递推方向**：两个维度都从小到大枚举。从转移方程中可以看出 f[i]f[i]f[i] 需要 f[i−1]f[i-1]f[i−1] 的值，所以先将小的算出，再向后推导。

记忆化搜索非常卡时间，不是万能通用的，尽量学会递推式写法吧。

Python3

Java

class Solution:

    def numberOfStableArrays(self, zero: int, one: int, limit: int) -> int:

        # 定义 f[i][j][k] 表示用 i 个 0 和 j 个 1 构造稳定数组的方案数,其中第 i+j 个位置填 k,k=0或1

        MOD = 1_000_000_007

        f = [[[0, 0] for _ in range(one + 1)] for _ in range(zero + 1)]

        # 初始值

        for i in range(1, min(limit, zero) + 1):

            f[i][0][0] = 1

        for j in range(1, min(limit, one) + 1):

            f[0][j][1] = 1

        # 开始枚举 

        for i in range(1, zero + 1):

            for j in range(1, one + 1):

                # + MOD 保证答案非负

                f[i][j][0] = (f[i - 1][j][0] + f[i - 1][j][1] - (f[i - limit - 1][j][1] if i > limit else 0)) % MOD

                f[i][j][1] = (f[i][j - 1][0] + f[i][j - 1][1] - (f[i][j - limit - 1][0] if j > limit else 0)) % MOD

        return sum(f[-1][-1]) % MOD

- 时间复杂度： O(zero×one)O(zero\times one)O(zero×one)，同理

- 空间复杂度： O(zero×one)O(zero\times one)O(zero×one)

题外话，看到视频中有人提出 dfsdfsdfs 不需要第三个参数 kkk，因为对称性，其实 dfs(i，j，0)==dfs(j，i，1)dfs(i，j，0)==dfs(j，i，1)dfs(i，j，0)==dfs(j，i，1)。优化后，可以省一半的时间。当然同理递推，感兴趣的小伙伴可以自己试试。

同时，灵神的题解中还有第三种解法，用的是 容斥原理+乘法原理。数学方法解决组合问题，不过着实难以理解，欢迎大家去围观。