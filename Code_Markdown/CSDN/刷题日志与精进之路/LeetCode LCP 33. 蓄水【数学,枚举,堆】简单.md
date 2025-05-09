给定 N 个无限容量且初始均空的水缸，每个水缸配有一个水桶用来打水，第 `i` 个水缸配备的水桶容量记作 `bucket[i]`。小扣有以下两种操作：

-   升级水桶：选择任意一个水桶，使其容量增加为 `bucket[i]+1`
-   蓄水：将全部水桶接满水，倒入各自对应的水缸

每个水缸对应最低蓄水量记作 `vat[i]`，返回小扣至少需要多少次操作可以完成所有水缸蓄水要求。

注意：实际蓄水量 **达到或超过** 最低蓄水量，即完成蓄水要求。
**示例 1：**
> 输入：`bucket = [1,3], vat = [6,8]`
> 
> 输出：`4`
> 
> 解释： 第 1 次操作升级 bucket[0]； 第 2 ~ 4 次操作均选择蓄水，即可完成蓄水要求。![vat1.gif](https://pic.leetcode-cn.com/1616122992-RkDxoL-vat1.gif)

**示例 2：**
> 输入：`bucket = [9,0,1], vat = [0,2,2]`
> 
> 输出：`3`
> 
> 解释： 第 1 次操作均选择升级 bucket[1] 第 2~3 次操作选择蓄水，即可完成蓄水要求。

**提示：**
-   `1 <= bucket.length == vat.length <= 100`
-   `0 <= bucket[i], vat[i] <= 10^4`

---
### 解法1 贪心+数学+枚举
这道题目的核心是，意识到==升级水桶的次数与蓄水次数之间存在的约束关系==。对这种两个值相关的情况，往往枚举一个值，就可以推出另一个值——**当我们确定了一个蓄水的次数，就可以反推出需要升级水桶的次数**。

为什么是**蓄水次数**？通过题目，我们可得如下关键信息：
- 显然，**应该把所有「升级水桶」的操作放在「蓄水」操作之前**。因为要使总操作次数最少，就应先升级水桶，蓄更多的水，使每次蓄水时的增益最大。
- 由于蓄水是将全部水桶接满水，倒入各自对应的水缸，因此**蓄水次数都是一致的**；
- 而升级水桶则是分别升级，每个水桶的升级次数都不同，我们无法枚举
- 因此，枚举「蓄水」的次数，倒推出每个水桶应有的容量，再减去当前的容量，就可计算出「升级水桶」的操作次数。即先升级水桶，再一次性完成蓄水。

以示例1为例，可以如下推算：
- 假设蓄水次数为 $1$ 次，则我们需要把第一个水桶的容量升级到 $6$ ，则需要升级 $5$ 次，同理要把第二个水桶升级到 $8$ ，需要升级 $5$ 次，则总共需要升级 $5+5=10$ 升级，再加上 $1$ 次蓄水的次数，总的操作数是 $11$ 次；
- 假设蓄水次数为 $2$ 次，则需要把第一个水桶升级到 $3$ ，则需要升级 $2$ 次，同理需要把第二个水桶升级到 $4$ ，需要升级 $1$ 次，总共需要升级 $3$ 次，再加上 $2$ 次的蓄水次数，所以总的操作数为 $5$ 次；
- 假设蓄水次数为 $3$ 次，则需要把第一个水桶升级到 $2$ ，则需要升级 $1$ 次；此时第二个水桶不需要再升级，升级次数为 $0$ 次，总共需要升级 $1$ 次，再加上 $3$ 次的蓄水次数，所以总的操作数为 $4$ 次；
- 假设蓄水次数为 $4$ 次，则需要把第一个水桶升级到 $2$ ，则需要升级 $1$ 次；此时第二个水桶不需要再升级，升级次数为 $0$ 次，总共需要升级 $1$ 次，再加上 $4$ 次的蓄水次数，所以总的操作数为 $5$ 次；
- 假设蓄水次数为 $5$ 次，则需要把第一个水桶升级到 $2$ ，则需要升级 $1$ 次；此时第二个水桶不需要再升级，升级次数为 $0$ 次，总共需要升级 $1$ 次，再加上 $5$ 次的蓄水次数，所以总的操作数为 $6$ 次；
- 假设蓄水次数为 $6$ 次，则第一个水桶和第二个水桶都不需要再升级，总共需要升级 $0$ 次，再加上 $6$ 次的蓄水次数，所以总的操作数为 $6$ 次；

显然**在第三种情况时总的操作数最小为 $4$ 次，对应 $1$ 次水桶升级，$3$ 次蓄水**。一般地，假设我们的蓄水次数为 $p$ ，则第 $i$ 个水桶的容量应为 $\left\lceil \dfrac{v_{i}}{p}\right\rceil$ ，由于 $v_i$ 可能小于 $p$ ，此时第 $i$ 个水桶的容量至少应为 $1$ ，可能小于原有的容量。所以升级次数更准确表述为 $$u_{i} = \max \left(\left\lceil \dfrac{v_{i}}{p}\right\rceil -b_i,\ 0\right)$$
，以使得该结果始终非负。

求出了每个水桶的升级次数，我们就可以得到总的操作数如下：
$$op=p+\sum _{i=1}^{len( v)}\left( \max\left(\left\lceil \frac{v_{i}}{p}\right\rceil -b_{i} ,0\right)\right)$$

接下来我们需要考虑 $p$ 的取值范围，当 $v[i]$ 全为零时，总的操作数也为零；当 $v[i]$ 不全为零时，$p$ 至少应为 $1$ 次，即至少要蓄水 $1$ 次；而 **$p$ 的上限应为 $\left\lceil \dfrac{v_{i}}{b_{i}}\right\rceil$ ​中的最大值，即在不升级任何水桶的情况下，所需要的最大的蓄水次数**。因此，最小的操作数应为：
$$\min( op_{j}) =\min\left( p_{j} +\sum _{i=1}^{len( v)}\left( \max\left(\left\lceil \frac{v_{i}}{p_{j}}\right\rceil -b_{i} ,0\right)\right) \ \right) ,where\ \ 1\leq p_{j} \leq \max\left(\left\lceil \frac{v_{i}}{b_{i}}\right\rceil \right)$$ 

代码如下所示：
```cpp
class Solution {
public:
    int storeWater(vector<int>& b, vector<int>& v) {
        int n = b.size();
        if (count(v.begin(), v.end(), 0) == n) return 0;
        int maxv = *max_element(v.begin(), v.end());
        int ans = 100000000;
        for (int i = 1; i <= maxv; ++i) { // 枚举蓄水的次数
            int p = i;
            for (int j = 0; j < n; ++j)
                // 每个水桶v[j]需要蓄水i次，则每个桶的容量应为v[j]/i上取整，和当前容量的差就是要升级的次数
                p += max(0, (v[j] + i - 1) / i - b[j]);
            ans = min(ans, p); // 蓄水的次数和升级水桶的次数
        } 
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(maxv \times n)$ ，其中 $maxv$ 是 $v$ 数组中最大值，$n$ 是 $b$ 数组的长度。
- 空间复杂度：$O(1)$ 

---
### 解法2 贪心+优先队列
因为「升级水桶」操作都在「蓄水」操作之前，所以如果在「蓄水」操作前第 $i$ 个水桶的容量为 $\textit{b}'[i],\ 0 \le i < n$ ，则**需要「蓄水」的操作至少为**
$$\max\{\lceil{\frac{\textit{v}[i]}{\textit{b}'[i]}}\rceil\}$$
即此时总的操作次数为：
$$\sum_{j = 0}^{n - 1}{(\textit{b}'[i] - \textit{b}[i])} + \max\{\lceil{\frac{\textit{v}[i]}{\textit{b}'[i]}}\rceil\}$$

即==**需要「蓄水」的操作次数只与 $n$ 个水缸中需要「蓄水」操作的最大值决定**==。所以如果我们想减少总操作次数，就要**选择需要最多次「蓄水」操作的水缸**，尝试减少其蓄水次数：
- 要么，在进行「蓄水」前，**尝试使其需要的「蓄水」操作减少 $1$** ，为此可能要对其水桶进行多次「升级水桶」操作。
- 要么，在进行「蓄水」前，**对其配置的水桶进行一次「升级水桶」操作**，使其需要的「蓄水」操作减少。

这样一直减少「需要最多蓄水操作的水缸」的「蓄水」次数（伴随着「升级水桶」次数的增加），直到获得最小的总操作次数。

为了**获得最多次「蓄水」操作的水缸**，可用「最大堆」来实现以上操作。以二元组 $(\textit{cnt}_i, \textit{i})$ 来表示第 $i$ 个水缸需要的「蓄水」操作为 $\textit{cnt}_i$ 次。
1. **从初始时将每一个水缸对应的二元组加入「最大堆」**。注意，如果一个水缸需要的蓄水要求为 $0$ 时，可以直接忽略该水缸；如果一个水缸配备的水桶初始容量为 $0$ 时且水缸的蓄水要求大于 $0$ 时，为了避免无法达到蓄水要求，此时需要进行一次「升级水桶」操作。
2. **然后每次尝试进行减小总操作次数**——从「最大堆」中取出需要「蓄水」操作最多的水桶；
    1. 更新当前需要的总操作次数最小值——比较当前「升级水桶」次数+最多次「蓄水」次数和答案的大小，并取最小值
    2. 再使需要的「蓄水」操作减少 $1$ ，为此可能进行多次「升级水桶」操作；或进行一次「升级水桶」操作，从而使需要的蓄水次数减少。
    3. 然后再次放入「最大堆」中，直到「升级水桶」已经不能再减少总的操作次数(减少蓄水操作次数）为止：==「升级水桶」的次数已经大于等于当前已得的总操作次数最小值；此时需要的最多「蓄水」操作次数等于 $1$ ==。

```cpp
class Solution {
public:
    int storeWater(vector<int>& b, vector<int>& v) {
        int n = b.size();
        priority_queue<pair<int, int>> q;
        int cnt = 0;
        for (int i = 0; i < n; ++i) {
            if (b[i] == 0 && v[i]) { // 桶容量为0,要蓄满v[i]
                ++cnt; // 提前升级一次水桶
                ++b[i];
            }
            if (v[i] > 0) 
                q.emplace((v[i] + b[i] - 1) / b[i], i);
        }
        if (q.empty()) return 0; // 说明v[i]都是0，蓄水次数为0，更不用升级桶
        int ans = INT_MAX;
        while (cnt < ans) {
            auto [tv, i] = q.top();
            ans = min(ans, cnt + tv);
            if (tv == 1) break; // 蓄水次数是1时退出,不可能更少了
            q.pop();
            
            // int t = (v[i] + tv - 2) / (tv - 1); // 尝试对蓄水次数减1,得到应有的桶容量
            // cnt += t - b[i]; // 对应的升级水桶次数
            // b[i] = t;
            // q.emplace(tv - 1, i);
            // 或者尝试升级一次水桶b[i],++cnt,然后重新计算蓄水次数
            ++b[i]; ++cnt;
            q.emplace((v[i] + b[i] - 1) / b[i], i);
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n\times \log n + n \times \log n \times \sqrt{C})$ ，其中 $n$ 为数组 $\textit{bucket}$ 的长度，$C$ 为数组 $\textit{vat}$ 的范围。「最大堆」中的一次存取操作的时间操作为 $O(\log n)$ ，初始化「最大堆」的时间复杂度为 $O(n \times \log n)$ 。每一个水桶的「蓄水」次数收敛到 $1$ 的复杂度渐进为 $O(\sqrt{C})$ 。
- 空间复杂度：$O(n)$ ，其中 $n$ 为数组 $\textit{bucket}$ 的长度，主要为「最大堆」的空间开销。
 