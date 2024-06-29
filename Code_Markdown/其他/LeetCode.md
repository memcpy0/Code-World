前置知识：前缀和
对于数组 stones\textit{stones}stones，定义它的前缀和 s[0]=0\textit{s}[0]=0s[0]=0，s[i+1]=∑j=0istones[j]\textit{s}[i+1] = \sum\limits_{j=0}^{i}\textit{stones}[j]s[i+1]= 
j=0
∑
i
​
 stones[j]。

根据这个定义，有 s[i+1]=s[i]+stones[i]s[i+1]=s[i]+\textit{stones}[i]s[i+1]=s[i]+stones[i]。

例如 stones=[1,2,1,2]\textit{stones}=[1,2,1,2]stones=[1,2,1,2]，对应的前缀和数组为 s=[0,1,3,4,6]s=[0,1,3,4,6]s=[0,1,3,4,6]。

通过前缀和，我们可以把子数组的元素和转换成两个前缀和的差，即

∑j=leftrightstones[j]=∑j=0rightstones[j]−∑j=0left−1stones[j]=s[right+1]−s[left] \sum_{j=\textit{left}}^{\textit{right}}\textit{stones}[j] = \sum\limits_{j=0}^{\textit{right}}\textit{stones}[j] - \sum\limits_{j=0}^{\textit{left}-1}\textit{stones}[j] = \textit{s}[\textit{right}+1] - \textit{s}[\textit{left}]
j=left
∑
right
​
 stones[j]= 
j=0
∑
right
​
 stones[j]− 
j=0
∑
left−1
​
 stones[j]=s[right+1]−s[left]
例如 stones\textit{stones}stones 的子数组 [2,1,2][2,1,2][2,1,2] 的和就可以用 s[4]−s[1]=6−1=5s[4]-s[1]=6-1=5s[4]−s[1]=6−1=5 算出来。

注：s[0]=0s[0]=0s[0]=0 表示一个空数组的元素和。为什么要额外定义它？想一想，如果要计算的子数组恰好是一个前缀（从 stones[0]\textit{stones}[0]stones[0] 开始），你要用 s[right]s[\textit{right}]s[right] 减去谁呢？通过定义 s[0]=0s[0]=0s[0]=0，任意子数组（包括前缀）都可以表示为两个前缀和的差。

思路（优化前）
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304041002930.png)


什么时候输出 −1-1−1 呢？

从 nnn 堆变成 111 堆，需要减少 n−1n-1n−1 堆。而每次合并都会减少 k−1k-1k−1 堆，所以 n−1n-1n−1 必须是 k−1k-1k−1 的倍数。

代码实现时，由于整个递归中有大量重复递归调用（递归入参相同），且递归函数没有副作用（同样的入参无论计算多少次，算出来的结果都是一样的），因此可以用记忆化搜索来优化：

如果一个状态（递归入参）是第一次遇到，那么可以在返回前，把状态及其结果记到一个 memo\textit{memo}memo 数组（或哈希表）中。
如果一个状态不是第一次遇到，那么直接返回 memo\textit{memo}memo 中保存的结果。
答疑
问：为什么只考虑分出 111 堆和 p−1p-1p−1 堆，而不考虑分出 xxx 堆和 p−xp-xp−x 堆？

答：无需计算，因为 p−1p-1p−1 堆继续递归又可以分出 111 堆和 p−2p-2p−2 堆，和之前分出的 111 堆组合，就已经能表达出「分出 222 堆和 p−2p-2p−2 堆」的情况了。其他同理。所以只需要考虑分出 111 堆和 p−1p-1p−1 堆。

Python3
Java
C++
Go
class Solution {
public:
    int mergeStones(vector<int> &stones, int k) {
        int n = stones.size();
        if ((n - 1) % (k - 1)) // 无法合并成一堆
            return -1;

        int s[n + 1];
        s[0] = 0;
        for (int i = 0; i < n; i++)
            s[i + 1] = s[i] + stones[i]; // 前缀和

        int memo[n][n][k + 1];
        memset(memo, -1, sizeof(memo)); // -1 表示还没有计算过
        function<int(int, int, int)> dfs = [&](int i, int j, int p) -> int {
            int &res = memo[i][j][p]; // 注意这里是引用，下面会直接修改 memo[i][j][p]
            if (res != -1) return res;
            if (p == 1) // 合并成一堆
                return res = i == j ? 0 : dfs(i, j, k) + s[j + 1] - s[i];
            res = INT_MAX;
            for (int m = i; m < j; m += k - 1) // 枚举哪些石头堆合并成第一堆
                res = min(res, dfs(i, m, 1) + dfs(m + 1, j, p - 1));
            return res;
        };
        return dfs(0, n - 1, 1);
    }
};
复杂度分析
时间复杂度：O(n3)O(n^3)O(n 
3
 )，其中 nnn 为 stones\textit{stones}stones 的长度。动态规划的时间复杂度 === 状态个数 ×\times× 单个状态的计算时间。这里状态个数为 O(n2k)O(n^2k)O(n 
2
 k)，单个状态的计算时间为 O(nk)O\left(\dfrac{n}{k}\right)O( 
k
n
​
 )，因此时间复杂度为 O(n3)O(n^3)O(n 
3
 )。
空间复杂度：O(n2k)O(n^2k)O(n 
2
 k)。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304041004204.png)

class Solution {
public:
    int mergeStones(vector<int> &stones, int k) {
        int n = stones.size();
        if ((n - 1) % (k - 1)) // 无法合并成一堆
            return -1;

        int s[n + 1];
        s[0] = 0;
        for (int i = 0; i < n; i++)
            s[i + 1] = s[i] + stones[i]; // 前缀和

        int memo[n][n];
        memset(memo, -1, sizeof(memo)); // -1 表示还没有计算过
        function<int(int, int)> dfs = [&](int i, int j) -> int {
            if (i == j) return 0; // 只有一堆石头，无需合并
            int &res = memo[i][j]; // 注意这里是引用，下面会直接修改 memo[i][j]
            if (res != -1) return res;
            res = INT_MAX;
            for (int m = i; m < j; m += k - 1)
                res = min(res, dfs(i, m) + dfs(m + 1, j));
            if ((j - i) % (k - 1) == 0) // 可以合并成一堆
                res += s[j + 1] - s[i];
            return res;
        };
        return dfs(0, n - 1);
    }
};
1:1 翻译成递推
把 dfs\textit{dfs}dfs 改成 fff 数组，把递归改成循环就好了。相当于原来是用递归计算每个状态 (i,j)(i,j)(i,j)，现在改用循环去计算每个状态 (i,j)(i,j)(i,j)。

需要注意循环的顺序：

由于 i<m+1i<m+1i<m+1，f[i]f[i]f[i] 要能从 f[m+1]f[m+1]f[m+1] 转移过来，必须先计算出 f[m+1]f[m+1]f[m+1]，所以 iii 要倒序枚举；
由于 j>mj>mj>m，f[i][j]f[i][j]f[i][j] 要能从 f[i][m]f[i][m]f[i][m] 转移过来，必须先计算出 f[i][m]f[i][m]f[i][m]，所以 jjj 要正序枚举。
Python3
Java
C++
Go
class Solution {
public:
    int mergeStones(vector<int> &stones, int k) {
        int n = stones.size();
        if ((n - 1) % (k - 1)) // 无法合并成一堆
            return -1;

        int s[n + 1];
        s[0] = 0;
        for (int i = 0; i < n; i++)
            s[i + 1] = s[i] + stones[i]; // 前缀和

        int f[n][n];
        for (int i = n - 1; i >= 0; --i) {
            f[i][i] = 0;
            for (int j = i + 1; j < n; ++j) {
                f[i][j] = INT_MAX;
                for (int m = i; m < j; m += k - 1)
                    f[i][j] = min(f[i][j], f[i][m] + f[m + 1][j]);
                if ((j - i) % (k - 1) == 0) // 可以合并成一堆
                    f[i][j] += s[j + 1] - s[i];
            }
        }
        return f[0][n - 1];
    }
};
复杂度分析
时间复杂度：O(n3k)O\left(\dfrac{n^3}{k}\right)O( 
k
n 
3
 
​
 )，其中 nnn 为 stones\textit{stones}stones 的长度。动态规划的时间复杂度 === 状态个数 ×\times× 单个状态的计算时间。这里状态个数为 O(n2)O(n^2)O(n 
2
 )，单个状态的计算时间为 O(nk)O\left(\dfrac{n}{k}\right)O( 
k
n
​
 )，因此时间复杂度为 O(n3k)O\left(\dfrac{n^3}{k}\right)O( 
k
n 
3
 
​
 )。
空间复杂度：O(n2)O(n^2)O(n 
2
 )。
相似题目（区间 DP）
375. 猜数字大小 II
516. 最长回文子序列
1039. 多边形三角剖分的最低得分
1312. 让字符串成为回文串的最少插入次数
1547. 切棍子的最小成本

作者：灵茶山艾府
链接：https://leetcode.cn/problems/minimum-cost-to-merge-stones/solutions/2207235/tu-jie-qu-jian-dpzhuang-tai-she-ji-yu-yo-ppv0/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。