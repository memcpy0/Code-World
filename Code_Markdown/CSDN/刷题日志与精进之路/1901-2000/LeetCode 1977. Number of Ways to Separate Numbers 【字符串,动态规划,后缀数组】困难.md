> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

你写下了若干 **正整数** ，并将它们连接成了一个字符串 `num` 。但是你忘记给这些数字之间加逗号了。你只记得这一列数字是 **非递减** 的且 **没有** 任何数字有前导 0 。

请你返回有多少种可能的 **正整数数组** 可以得到字符串 `num` 。由于答案可能很大，将结果对 `109 + 7` **取余** 后返回。

**示例 1：**

**输入：**num = "327"
**输出：**2
**解释：**以下为可能的方案：
3, 27
327

**示例 2：**

**输入：**num = "094"
**输出：**0
**解释：**不能有数字有前导 0 ，且所有数字均为正数。

**示例 3：**

**输入：**num = "0"
**输出：**0
**解释：**不能有数字有前导 0 ，且所有数字均为正数。

**示例 4：**

**输入：**num = "9999999999999"
**输出：**101

**提示：**

- `1 <= num.length <= 3500`
- `num` 只含有数字 `'0'` 到 `'9'` 。

方法一：动态规划
本题较难。

容易想到O(N3)\mathcal{O}(N^3)O(N 
3
 )的动态规划：用dp[i][j]dp[i][j]dp[i][j]表示到第iii个位置，最后一个数字的长度为jjj时的方案数，则其对应的上一个数的结尾应该是i−ji-ji−j，我们就可以枚举dp[i−j][k]dp[i-j][k]dp[i−j][k]，k≤jk\le jk≤j。其中，k<jk<jk<j的部分可以直接加上（因为前面的数字长度较短，所以一定更小），但k=jk=jk=j时，我们需要判断两个数字的大小关系。

利用前缀和的方法可以将k<jk<jk<j的部分优化到O(1)\mathcal{O}(1)O(1)时间，但此时整体的复杂度还是O(N3)\mathcal{O}(N^3)O(N 
3
 )，因为最坏情况下，所有数字都相同（比如3500个9），则我们对于每一个k=jk=jk=j的情形都需要进行O(N)\mathcal{O}(N)O(N)时间的比较。

如何优化比较呢？这里，我们可以进行一次预处理的动态规划来加速比较。

令c[i][j]c[i][j]c[i][j]表示第一个串从iii开始，第二个串从jjj开始，且满足第一个串不大于第二个串的最长长度。显然c[i][j]c[i][j]c[i][j]可以由c[i+1][j+1]c[i+1][j+1]c[i+1][j+1]转移得到。

求得c[i][j]c[i][j]c[i][j]之后，我们在进行比较时就只需要判断c[i−2j][i−j]≥jc[i-2j][i-j]\ge jc[i−2j][i−j]≥j是否成立即可。

时间复杂度O(∣S∣2)\mathcal{O}(|S|^2)O(∣S∣ 
2
 )。
空间复杂度O(∣S∣2)\mathcal{O}(|S|^2)O(∣S∣ 
2
 )。
参考代码（C++）
using ll = long long;
const ll MOD = 1e9 + 7;

class Solution {
public:
    int numberOfCombinations(string num) {
        if (num[0] == '0')
            return 0;
        
        int n = num.size();
        vector<vector<ll>> dp(n + 1, vector<ll>(n + 1));
        dp[1] = vector<ll>(n + 1, 1);
        dp[1][0] = 0;
        
        vector<vector<int>> c(n, vector<int>(n));
        for (int len = 1; len <= n; ++len)
            for (int i = n - 1 - len; i >= 0; --i) {
                if (num[i] < num[i + len])
                    c[i][i + len] = n - i - len;
                else if (num[i] == num[i + len]) {
                    if (i + len == n - 1)
                        c[i][i + len] = 1;
                    else
                        c[i][i + len] = c[i + 1][i + len + 1] + 1;
                }
            }
        
        auto cmp = [&](int l, int r, int len) {
            if (l < 0)
                return false;
            return c[l][r] >= len;
        };
        
        for (int i = 2; i <= n; ++i) {
            dp[i][i] = 1;
            for (int j = 1; j < i; ++j) {
                if (num[i - j] == '0')
                    continue;
                if (cmp(i - 2 * j, i - j, j))
                    dp[i][j] = (dp[i][j] + dp[i - j][j]) % MOD;
                else
                    dp[i][j] = (dp[i][j] + dp[i - j][j - 1]) % MOD;   
            }
            for (int j = 1; j <= n; ++j)
                dp[i][j] = (dp[i][j] + dp[i][j - 1]) % MOD;
        }
        
        return dp[n][n];
    }
};

作者：吴自华
链接：https://leetcode.cn/problems/number-of-ways-to-separate-numbers/solutions/950412/liang-ci-dong-tai-gui-hua-qian-zhui-he-b-r29b/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。


定义 f[i][j]f[i][j]f[i][j] 表示 num\textit{num}num 的前 jjj 个字符划分出的最后一个整数的起始位置为 iii 时的方案数。

我们所求的答案即为 ∑i=0n−1f[i][n−1]\sum\limits_{i=0}^{n-1} f[i][n-1] 
i=0
∑
n−1
​
 f[i][n−1]。

定义 lcp[i][j]\textit{lcp}[i][j]lcp[i][j] 表示后缀 num[i:]\textit{num}[i:]num[i:] 和后缀 num[j:]\textit{num}[j:]num[j:] 的最长公共前缀的长度。

对于倒数第二个划分出的整数，记其起始位置为 kkk，结束位置为 i−1i-1i−1。我们可以通过比较倒数第二个划分出的整数和最后一个划分出的整数的大小，来计算状态转移，也就是在满足题目要求时，将 f[k][i−1]f[k][i-1]f[k][i−1] 加到 f[i][j]f[i][j]f[i][j] 上。

具体地，我们按倒数第二个划分出的整数的长度分类讨论：

若其长度小于最后一个划分出的整数，则可以将其方案数加到 f[i][j]f[i][j]f[i][j] 上，即 f[i][j]+=∑f[k][i−1]f[i][j] += \sum f[k][i-1]f[i][j]+=∑f[k][i−1]，这里 i−k<j−i+1i-k<j-i+1i−k<j−i+1；在代码实现时，我们可以从 iii 出发，向左向右同时扩展 kkk 和 jjj，并累加 f[k][i−1]f[k][i-1]f[k][i−1]；
若其长度等于最后一个划分出的整数，则需要比较两个整数的大小，这可以通过比较两个整数（子串）最长公共前缀的下一个字符得出；
若其长度大于最后一个划分出的整数，由于不满足题目要求，无法转移。
对于 lcp\textit{lcp}lcp，可以用一个 O(n2)O(n^2)O(n 
2
 ) 的简单转移预处理出来，这样就可以做到 O(1)O(1)O(1) 的状态转移。总的时间复杂度为 O(n2)O(n^2)O(n 
2
 )。

const mod int = 1e9 + 7

func numberOfCombinations(s string) (ans int) {
	if s[0] == '0' {
		return
	}

	n := len(s)
	// 计算 lcp
	lcp := make([][]int, n+1)
	for i := range lcp {
		lcp[i] = make([]int, n+1)
	}
	for i := n - 1; i >= 0; i-- {
		for j := n - 1; j >= 0; j-- {
			if s[i] == s[j] {
				lcp[i][j] = lcp[i+1][j+1] + 1
			}
		}
	}
	// 返回 s[l1:l2] <= s[l2:r2]
	lessEq := func(l1, l2, r2 int) bool {
		l := lcp[l1][l2]
		return l >= r2-l2 || s[l1+l] < s[l2+l]
	}

	f := make([][]int, n)
	for i := range f {
		f[i] = make([]int, n)
	}
	for j := 0; j < n; j++ {
		f[0][j] = 1
	}
	for i := 1; i < n; i++ {
		if s[i] == '0' {
			continue
		}
		// k 和 j 同时向左向右扩展
		for j, k, sum := i, i-1, 0; j < n; j++ {
			f[i][j] = sum // 对应上面所说的长度小于最后一个划分出的整数
			if k < 0 {
				continue
			}
			if s[k] > '0' && lessEq(k, i, j+1) {
				f[i][j] = (f[i][j] + f[k][i-1]) % mod // 对应上面所说的长度等于最后一个划分出的整数
			}
			sum = (sum + f[k][i-1]) % mod
			k--
		}
	}
	for _, row := range f {
		ans = (ans + row[n-1]) % mod
	}
	return
}

作者：灵茶山艾府
链接：https://leetcode.cn/problems/number-of-ways-to-separate-numbers/solutions/950429/yu-chu-li-dong-tai-gui-hua-by-endlessche-7am2/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。


前言
本题思维难度较大，需要一些动态规划的预处理和优化技巧，并且细节很多。

方法一：动态规划
思路与算法

我们用 f[i][j]f[i][j]f[i][j] 表示对于字符串 num\textit{num}num 的第 0,1,⋯ ,j0, 1, \cdots, j0,1,⋯,j 个字符进行划分，并且最后一个数使用了第 i,i+1,⋯ji, i+1, \cdots ji,i+1,⋯j 个字符的方案数。为了叙述方便，我们用 num(i,j)\textit{num}(i, j)num(i,j) 表示该数。

那么如何进行状态转移呢？我们可以基于如下的一个事实：

对于数 aaa 和 bbb，如果 aaa 的位数严格大于 bbb 的位数，那么 aaa 一定严格大于 bbb。

由于 f[i][j]f[i][j]f[i][j] 中的最后一个数的位数为 j−i+1j-i+1j−i+1，那么上一个数的位数小于等于 j−ij-ij−i 即可进行转移。上一个数的结尾在位置 i−1i - 1i−1，那么其开始下标需要大于等于：

(i−1)−(j−i)+1=2i−j (i - 1) - (j - i) + 1 = 2i - j
(i−1)−(j−i)+1=2i−j
对应的状态即为：

f[2i−j][i−1],f[2i−j+1,i−1],⋯ ,f[i−1][i−1] f[2i - j][i - 1], f[2i - j + 1, i - 1], \cdots, f[i - 1][i - 1]
f[2i−j][i−1],f[2i−j+1,i−1],⋯,f[i−1][i−1]
此外，我们还需要比较 num(2i−j−1,i−1)\textit{num}(2i - j - 1, i - 1)num(2i−j−1,i−1) 和 num(i,j)\textit{num}(i, j)num(i,j) 的值的大小关系，此时这两个数的位数都是 j−i+1j-i+1j−i+1。如果前者小于等于后者，那么 f[i][j]f[i][j]f[i][j] 还可以从 f[2i−j−1][i−1]f[2i-j-1][i-1]f[2i−j−1][i−1] 转移而来。因此，状态转移方程为：

f[i][j]={∑k=2i−ji−1f[k][i−1],num(2i−j−1,i−1)>num(i,j)∑k=2i−j−1i−1f[k][i−1],num(2i−j−1,i−1)≤num(i,j) f[i][j] = \left\{ \begin{aligned} & \sum_{k=2i-j}^{i-1} f[k][i-1], & \textit{num}(2i-j-1,i-1) > \textit{num}(i, j) \\ & \sum_{k=2i-j-1}^{i-1} f[k][i-1], & \textit{num}(2i-j-1,i-1) \leq \textit{num}(i, j) \end{aligned} \right.
f[i][j]= 
⎩
⎨
⎧
​
  
​
  
k=2i−j
∑
i−1
​
 f[k][i−1],
k=2i−j−1
∑
i−1
​
 f[k][i−1],
​
  
num(2i−j−1,i−1)>num(i,j)
num(2i−j−1,i−1)≤num(i,j)
​
 
需要注意的是：为了防止状态转移方程显得过于复杂，我们在状态转移方程中：

没有考虑 2i−j2i-j2i−j 和 2i−j−12i-j-12i−j−1 是否超出边界。但在实际的代码编写中，需要保证求和式中 kkk 的最小值不能小于 000；

没有考虑 num(i,j)\textit{num}(i, j)num(i,j) 是否包含前导零。如果 num[i]=0\textit{num}[i] = 0num[i]=0，那么 f[i][j]=0f[i][j] = 0f[i][j]=0。特别地，如果 num[0]=0\textit{num}[0] = 0num[0]=0，那么不会有任何满足要求的划分方案，直接返回 000 作为答案，无需进行动态规划。

动态规划的边界条件为 f[0][..]=1f[0][..] = 1f[0][..]=1，其余的状态的初始值均为 000。最终的答案即为所有 f[..][n−1]f[..][n - 1]f[..][n−1] 的和，其中 nnn 是字符串 num\textit{num}num 的长度。

前缀和优化

即使我们不考虑如何快速地比较 num(2i−j−1,i−1)\textit{num}(2i-j-1,i-1)num(2i−j−1,i−1) 和 num(i,j)\textit{num}(i, j)num(i,j) 的大小关系，上述动态规划的时间复杂度也为 O(n3)O(n^3)O(n 
3
 )：即我们需要 O(n2)O(n^2)O(n 
2
 ) 的时间枚举 iii 和 jjj，还需要 O(n)O(n)O(n) 的时间枚举 kkk 计算对应项的和。

然而我们可以发现，这些和是「连续」的，因此我们可以使用前缀和进行优化。设 pre[i][j]\textit{pre}[i][j]pre[i][j] 表示：

pre[i][j]=∑k=0if[i][j] \textit{pre}[i][j] = \sum_{k=0}^{i} f[i][j]
pre[i][j]= 
k=0
∑
i
​
 f[i][j]
那么状态转移方程可以改写为：

f[i][j]={pre[i−1][i−1]−pre[2i−j−1][i−1],num(2i−j−1,i−1)>num(i,j)pre[i−1][i−1]−pre[2i−j−2][i−1],num(2i−j−1,i−1)≤num(i,j) f[i][j] = \left\{ \begin{aligned} & \textit{pre}[i-1][i-1] - \textit{pre}[2i-j-1][i-1], & \textit{num}(2i-j-1,i-1) > \textit{num}(i, j) \\ & \textit{pre}[i-1][i-1] - \textit{pre}[2i-j-2][i-1], & \textit{num}(2i-j-1,i-1) \leq \textit{num}(i, j) \end{aligned} \right.
f[i][j]={ 
​
  
pre[i−1][i−1]−pre[2i−j−1][i−1],
pre[i−1][i−1]−pre[2i−j−2][i−1],
​
  
num(2i−j−1,i−1)>num(i,j)
num(2i−j−1,i−1)≤num(i,j)
​
 
只要在计算 fff 的过程中维护 pre\textit{pre}pre，就可以将动态规划的时间复杂度优化至 O(n2)O(n^2)O(n 
2
 )。

此外，我们也可以无需显式地使用前缀和数组：如果我们按照先枚举 iii 再枚举 jjj 的顺序计算 f[i][j]f[i][j]f[i][j]，那么有：

f[i][j]=∑k=2i−ji−1f[k][i−1] f[i][j] = \sum_{k=2i-j}^{i-1} f[k][i-1]
f[i][j]= 
k=2i−j
∑
i−1
​
 f[k][i−1]
这里我们不考虑 num(2i−j−1,i−1)\textit{num}(2i-j-1,i-1)num(2i−j−1,i−1) 和 num(i,j)\textit{num}(i, j)num(i,j) 的大小关系，即使前者小于等于后者，多出的 f[2i−j−1][i−1]f[2i-j-1][i-1]f[2i−j−1][i−1] 这一项也可以 O(1)O(1)O(1) 的时间累加进 f[i][j]f[i][j]f[i][j]，无需刻意前缀和进行优化。

当 j→j+1j \to j+1j→j+1 时：

f[i][j+1]=∑k=2i−j−1i−1f[k][i−1] f[i][j+1] = \sum_{k=2i-j-1}^{i-1} f[k][i-1]
f[i][j+1]= 
k=2i−j−1
∑
i−1
​
 f[k][i−1]
可以发现，f[i][j+1]f[i][j+1]f[i][j+1] 只比 f[i][j]f[i][j]f[i][j] 多出了 f[2i−j−1][i−1]f[2i-j-1][i-1]f[2i−j−1][i−1] 这一项，因此在求得 f[i][j]f[i][j]f[i][j] 的前提下，我们需要 O(1)O(1)O(1) 的时间即可得到 f[i][j+1]f[i][j+1]f[i][j+1]。

快速比较两个数的大小关系

此时，我们只剩最后一步，也就是快速比较 num(2i−j−1,i−1)\textit{num}(2i-j-1,i-1)num(2i−j−1,i−1) 和 num(i,j)\textit{num}(i, j)num(i,j) 的大小关系了。这一步可以使用预处理巧妙地解决。

记 lcp[i][j]\textit{lcp}[i][j]lcp[i][j] 表示在字符串 nums\textit{nums}nums 中，以 iii 开始的后缀与以 jjj 开始的后缀的「最长公共前缀」的长度。直观上看，它表示：

num(i,i+lcp[i][j]−1)=num(j,j+lcp[i][j]−1)\textit{num}(i, i + \textit{lcp}[i][j] - 1) = \textit{num}(j, j + \textit{lcp}[i][j] - 1)num(i,i+lcp[i][j]−1)=num(j,j+lcp[i][j]−1)；

num[i+lcp[i][j]]≠num[j+lcp[i][j]]\textit{num}[i + \textit{lcp}[i][j]] \neq \textit{num}[j + \textit{lcp}[i][j]]num[i+lcp[i][j]]

=num[j+lcp[i][j]] 或者其中某一下标超出边界。

lcp[i][j]\textit{lcp}[i][j]lcp[i][j] 可以很方便地使用动态规划求出，即：

lcp[i][j]={lcp[i+1][j+1]+1,num[i]=num[j]0,num[i]≠num[j] \textit{lcp}[i][j] = \begin{cases} \textit{lcp}[i+1][j+1] + 1, & \quad \textit{num}[i] = \textit{num}[j] \\ 0, & \quad \textit{num}[i] \neq \textit{num}[j] \end{cases}
lcp[i][j]={ 
lcp[i+1][j+1]+1,
0,
​
  
num[i]=num[j]
num[i]

=num[j]
​
 
当我们求出了 lcp\textit{lcp}lcp 后，就可以方便地比较 num\textit{num}num 中两个子串的大小关系了。对于 num(2i−j−1,i−1)\textit{num}(2i-j-1,i-1)num(2i−j−1,i−1) 和 num(i,j)\textit{num}(i, j)num(i,j)：

如果 lcp[2i−j−1][i]≥j−i+1\textit{lcp}[2i-j-1][i] \geq j-i+1lcp[2i−j−1][i]≥j−i+1，那么 num(2i−j−1,i−1)\textit{num}(2i-j-1,i-1)num(2i−j−1,i−1) 一定等于 num(i,j)\textit{num}(i, j)num(i,j)；

如果 lcp[2i−j−1][i]<j−i+1\textit{lcp}[2i-j-1][i] < j-i+1lcp[2i−j−1][i]<j−i+1，那么 num(2i−j−1,i−1)\textit{num}(2i-j-1,i-1)num(2i−j−1,i−1) 和 num(i,j)\textit{num}(i, j)num(i,j) 的大小关系，等同于 num[2i−j−1+lcp[2i−j−1][i]]\textit{num}[2i-j-1+\textit{lcp}[2i-j-1][i]]num[2i−j−1+lcp[2i−j−1][i]] 与 num[i+lcp[2i−j−1][i]]\textit{num}[i+\textit{lcp}[2i-j-1][i]]num[i+lcp[2i−j−1][i]] 的大小关系。

这样做的原因在于，两个长度相等的数的「数值」大小关系是等同于它们「字典序」的大小关系的。因此我们找出这两个数的最长公共前缀，再比较最长公共前缀的下一个字符的大小关系即可。

至此，我们就将动态规划的时间复杂度完全优化至 O(n2)O(n^2)O(n 
2
 )，也就可以通过本题了。

注解

lcp\textit{lcp}lcp 来源于 Longest Common Prefix\text{\textbf{L}ongest \textbf{C}ommon \textbf{P}refix}Longest Common Prefix，即最长公共前缀。如果读者研究过算法竞赛，学习过「后缀数组」，那么上述的 lcp\textit{lcp}lcp 是可以通过后缀数组 + ST\text{ST}ST 表在 O(nlog⁡n)O(n \log n)O(nlogn) 的时间内预处理得到的。但这已经远远超出了面试和笔试的难度，因此这里不再深入解释。

代码

C++
Python3
class Solution {
private:
    static constexpr int mod = 1000000007;

public:
    int numberOfCombinations(string num) {
        if (num[0] == '0') {
            return 0;
        }

        int n = num.size();

        // 预处理 lcp
        vector<vector<int>> lcp(n, vector<int>(n));
        for (int i = n - 1; i >= 0; --i) {
            lcp[i][n - 1] = (num[i] == num[n - 1]);
            for (int j = i + 1; j < n - 1; ++j) {
                lcp[i][j] = (num[i] == num[j] ? lcp[i + 1][j + 1] + 1 : 0);
            }
        }

        // 辅助函数，计算 x = (x + y) % mod
        auto update = [&](int& x, int y) {
            x += y;
            if (x >= mod) {
                x -= mod;
            }
        };

        // 动态规划
        vector<vector<int>> f(n, vector<int>(n));
        // 边界 f[0][..] = 1
        for (int i = 0; i < n; ++i) {
            f[0][i] = 1;
        }
        for (int i = 1; i < n; ++i) {
            // 有前导零，无需转移
            if (num[i] == '0') {
                continue;
            }
            // 前缀和
            int presum = 0;
            for (int j = i; j < n; ++j) {
                int length = j - i + 1;
                f[i][j] = presum;
                if (i - length >= 0) {
                    // 使用 lcp 比较 num(2i-j-1,i-1) 与 num(i,j) 的大小关系
                    if (lcp[i - length][i] >= length || num[i - length + lcp[i - length][i]] < num[i + lcp[i - length][i]]) {
                        update(f[i][j], f[i - length][i - 1]);
                    }
                    // 更新前缀和
                    update(presum, f[i - length][i - 1]);
                }
            }
        }

        // 最终答案即为所有 f[..][n-1] 的和
        int ans = 0;
        for (int i = 0; i < n; ++i) {
            update(ans, f[i][n - 1]);
        }
        return ans;
    }
};
复杂度分析

时间复杂度：O(n2)O(n^2)O(n 
2
 )，其中 nnn 是字符串 num\textit{num}num 的长度。

空间复杂度：O(n2)O(n^2)O(n 
2
 )，即为数组 fff 和 lcp\textit{lcp}lcp 需要使用的空间。

作者：力扣官方题解
链接：https://leetcode.cn/problems/number-of-ways-to-separate-numbers/solutions/951911/hua-fen-shu-zi-de-fang-an-shu-by-leetcod-env6/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。