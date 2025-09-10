> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

你有一辆货运卡车，你需要用这一辆车把一些箱子从仓库运送到码头。这辆卡车每次运输有 **箱子数目的限制** 和 **总重量的限制** 。

给你一个箱子数组 `boxes` 和三个整数 `portsCount`, `maxBoxes` 和 `maxWeight` ，其中 `boxes[i] = [ports​​i​, weighti]` 。

- `ports​​i` 表示第 `i` 个箱子需要送达的码头， `weightsi` 是第 `i` 个箱子的重量。
- `portsCount` 是码头的数目。
- `maxBoxes` 和 `maxWeight` 分别是卡车每趟运输箱子数目和重量的限制。

箱子需要按照 **数组顺序** 运输，同时每次运输需要遵循以下步骤：

- 卡车从 `boxes` 队列中按顺序取出若干个箱子，但不能违反 `maxBoxes` 和 `maxWeight` 限制。
- 对于在卡车上的箱子，我们需要 **按顺序** 处理它们，卡车会通过 **一趟行程** 将最前面的箱子送到目的地码头并卸货。如果卡车已经在对应的码头，那么不需要 **额外行程** ，箱子也会立马被卸货。
- 卡车上所有箱子都被卸货后，卡车需要 **一趟行程** 回到仓库，从箱子队列里再取出一些箱子。

卡车在将所有箱子运输并卸货后，最后必须回到仓库。

请你返回将所有箱子送到相应码头的 **最少行程** 次数。

**示例 1：**

**输入：**boxes = [[1,1],[2,1],[1,1]], portsCount = 2, maxBoxes = 3, maxWeight = 3
**输出：**4
**解释：**最优策略如下：
- 卡车将所有箱子装上车，到达码头 1 ，然后去码头 2 ，然后再回到码头 1 ，最后回到仓库，总共需要 4 趟行程。
所以总行程数为 4 。
注意到第一个和第三个箱子不能同时被卸货，因为箱子需要按顺序处理（也就是第二个箱子需要先被送到码头 2 ，然后才能处理第三个箱子）。

**示例 2：**

**输入：**boxes = [[1,2],[3,3],[3,1],[3,1],[2,4]], portsCount = 3, maxBoxes = 3, maxWeight = 6
**输出：**6
**解释：**最优策略如下：
- 卡车首先运输第一个箱子，到达码头 1 ，然后回到仓库，总共 2 趟行程。
- 卡车运输第二、第三、第四个箱子，到达码头 3 ，然后回到仓库，总共 2 趟行程。
- 卡车运输第五个箱子，到达码头 2 ，回到仓库，总共 2 趟行程。
总行程数为 2 + 2 + 2 = 6 。

**示例 3：**

**输入：**boxes = [[1,4],[1,2],[2,1],[2,1],[3,2],[3,4]], portsCount = 3, maxBoxes = 6, maxWeight = 7
**输出：**6
**解释：**最优策略如下：
- 卡车运输第一和第二个箱子，到达码头 1 ，然后回到仓库，总共 2 趟行程。
- 卡车运输第三和第四个箱子，到达码头 2 ，然后回到仓库，总共 2 趟行程。
- 卡车运输第五和第六个箱子，到达码头 3 ，然后回到仓库，总共 2 趟行程。
总行程数为 2 + 2 + 2 = 6 。

**示例 4：**

**输入：**boxes = [[2,4],[2,5],[3,1],[3,2],[3,7],[3,1],[4,4],[1,3],[5,2]], portsCount = 5, maxBoxes = 5, maxWeight = 7
**输出：**14
**解释：**最优策略如下：
- 卡车运输第一个箱子，到达码头 2 ，然后回到仓库，总共 2 趟行程。
- 卡车运输第二个箱子，到达码头 2 ，然后回到仓库，总共 2 趟行程。
- 卡车运输第三和第四个箱子，到达码头 3 ，然后回到仓库，总共 2 趟行程。
- 卡车运输第五个箱子，到达码头 3 ，然后回到仓库，总共 2 趟行程。
- 卡车运输第六和第七个箱子，到达码头 3 ，然后去码头 4 ，然后回到仓库，总共 3 趟行程。
- 卡车运输第八和第九个箱子，到达码头 1 ，然后去码头 5 ，然后回到仓库，总共 3 趟行程。
总行程数为 2 + 2 + 2 + 2 + 3 + 3 = 14 。

**提示：**

- `1 <= boxes.length <= 10^5`
- `1 <= portsCount, maxBoxes, maxWeight <= 10^5`
- `1 <= ports​​i <= portsCount`
- `1 <= weightsi <= maxWeight`

---
前言

为了叙述方便，我们记箱子的数量为 nnn，它们的目的地分别为 p1,⋯ ,pnp_1, \cdots, p_np 
1
​
 ,⋯,p 
n
​
 ，重量分别为 w1,⋯ ,wnw_1, \cdots, w_nw 
1
​
 ,⋯,w 
n
​
 。

记 WiW_iW 
i
​
  表示 www 的前缀和，即：

Wi={0,i=0∑k=1iwi,i>0 W_i = \left\{ \begin{aligned} & 0, && i = 0 \\ & \sum_{k=1}^i w_i, && i > 0 \end{aligned} \right.
W 
i
​
 = 
⎩
⎨
⎧
​
  
​
  
0,
k=1
∑
i
​
 w 
i
​
 ,
​
  
​
  
i=0
i>0
​
 
这样我们可以用 Wi−Wj−1W_i - W_{j-1}W 
i
​
 −W 
j−1
​
  方便地表示第 iii 个到第 jjj 个箱子的重量，并与 maxWeight\textit{maxWeight}maxWeight 进行比较。

记示性函数 I(i)\mathbb{I}(i)I(i) 表示 pip_ip 
i
​
  和 pi+1p_{i+1}p 
i+1
​
  是否不等，即：

I(i)={0,pi=pi+11,pi≠pi+1 \mathbb{I}(i) = \begin{cases} 0, \quad p_i = p_{i+1} \\ 1, \quad p_i \neq p_{i+1} \end{cases}
I(i)={ 
0,p 
i
​
 =p 
i+1
​
 
1,p 
i
​
 

=p 
i+1
​
 
​
 
记 neg(i,j)\textit{neg}(i, j)neg(i,j) 表示 pi,⋯ ,pjp_i, \cdots, p_jp 
i
​
 ,⋯,p 
j
​
  相邻两项不等的次数，即：

neg(i,j)=∑k=ij−1I(k) \textit{neg}(i, j) = \sum_{k=i}^{j-1} \mathbb{I}(k)
neg(i,j)= 
k=i
∑
j−1
​
 I(k)
这样我们可以用 neg(i,j)+2\textit{neg}(i, j) + 2neg(i,j)+2 方便地求出一次性运送第 iii 个到第 jjj 个箱子需要的行程次数，这里的 +2+2+2 表示来回需要的 222 次。

为了便于快速计算 neg(i,j)\textit{neg}(i, j)neg(i,j)，我们也可以使用前缀和的方式进行存储。记 negi=neg(1,i)\textit{neg}_i = \textit{neg}(1, i)neg 
i
​
 =neg(1,i) 表示前缀和，那么 neg(i,j)=negj−negi\textit{neg}(i, j) = \textit{neg}_j - \textit{neg}_{i}neg(i,j)=neg 
j
​
 −neg 
i
​
  可以在 O(1)O(1)O(1) 的时间求出。

注意：这里是 negj−negi\textit{neg}_j - \textit{neg}_{i}neg 
j
​
 −neg 
i
​
  而不是 negj−negi−1\textit{neg}_j - \textit{neg}_{i-1}neg 
j
​
 −neg 
i−1
​
 ，读者可以思考一下其原因。

思路与算法

我们可以使用动态规划解决本题。

记 fif_if 
i
​
  表示运送前 iii 个箱子需要的最少行程次数，这里的「前 iii 个箱子」指的是目的地为 p1,⋯ ,pip_1, \cdots, p_ip 
1
​
 ,⋯,p 
i
​
  的 iii 个箱子。我们可以写出状态转移方程：

fi=min⁡{fj+neg(j+1,i)+2}subject to{0≤j<ii−j≤maxBoxesWi−Wj≤maxWeight \begin{aligned} & f_i = \min \big\{ f_j + \textit{neg}(j+1,i) + 2 \big\} \\\\ \text{subject to} \quad & \begin{cases} 0 \leq j < i \\ i-j \leq \textit{maxBoxes} \\ W_i-W_j \leq \textit{maxWeight} \end{cases} \end{aligned}
subject to
​
  
f 
i
​
 =min{f 
j
​
 +neg(j+1,i)+2}
⎩
⎨
⎧
​
  
0≤j<i
i−j≤maxBoxes
W 
i
​
 −W 
j
​
 ≤maxWeight
​
 
​
 
即枚举上一次运送的最后一个箱子为 jjj（这里的 jjj 可以为 000，表示这一次是第一次运送箱子），那么这一次运送的箱子为 [j+1,i][j+1, i][j+1,i]。箱子的数量不超过 maxBoxes\textit{maxBoxes}maxBoxes，重量之和不能超过 maxWeight\textit{maxWeight}maxWeight。运送的行程次数即为 pj+1,⋯ ,pip_{j+1}, \cdots, p_ip 
j+1
​
 ,⋯,p 
i
​
  相邻两项不等的次数 neg(j+1,i)\textit{neg}(j+1, i)neg(j+1,i) 加上来回的 222 次。

边界条件为 f0=0f_0 = 0f 
0
​
 =0，最终答案即为 fnf_nf 
n
​
 。

优化

然而上述动态规划的时间复杂度为 O(n2)O(n^2)O(n 
2
 )，我们需要进行优化。我们将 neg(j+1,i)\textit{neg}(j+1, i)neg(j+1,i) 拆分成两个前缀和的差，即：

neg(j+1,i)=negi−negj+1 \textit{neg}(j+1, i) = \textit{neg}_i - \textit{neg}_{j+1}
neg(j+1,i)=neg 
i
​
 −neg 
j+1
​
 
带入原状态转移方程：

fi=min⁡{fj+neg(j+1,i)+2}=min⁡{fj+negi−negj+1+2}=min⁡{fj−negj+1}+negi+2 \begin{aligned} f_i &= \min \big\{ f_j + \textit{neg}(j+1,i) + 2 \big\} \\ &= \min \big\{ f_j + \textit{neg}_i - \textit{neg}_{j+1} + 2 \big\} \\ &= \min \big\{ f_j - \textit{neg}_{j+1} \big\} + \textit{neg}_i + 2 \end{aligned}
f 
i
​
 
​
  
=min{f 
j
​
 +neg(j+1,i)+2}
=min{f 
j
​
 +neg 
i
​
 −neg 
j+1
​
 +2}
=min{f 
j
​
 −neg 
j+1
​
 }+neg 
i
​
 +2
​
 
由于 negi\textit{neg}_ineg 
i
​
  和 222 都是与 jjj 无关的项，因此可以从 min⁡{⋅}\min\{ \cdot \}min{⋅} 中提取出来。

记 gj=fj−negj+1g_j = f_j - \textit{neg}_{j+1}g 
j
​
 =f 
j
​
 −neg 
j+1
​
 ，状态转移方程即为：

fi=min⁡{gj}+neg(i)+2 f_i = \min \{ g_j \} + \textit{neg}(i) + 2
f 
i
​
 =min{g 
j
​
 }+neg(i)+2
如果只有 0≤j<i0 \leq j < i0≤j<i 的限制条件，那么我们实时维护 gjg_jg 
j
​
  的最小值进行 O(1)O(1)O(1) 的转移即可。但现在有 i−j≤maxBoxesi-j \leq \textit{maxBoxes}i−j≤maxBoxes 和 Wi−Wj≤maxWeightW_i-W_j \leq \textit{maxWeight}W 
i
​
 −W 
j
​
 ≤maxWeight 这两个额外的限制条件，最小的 gjg_jg 
j
​
  对应的 jjj 不一定满足限制。

我们可以将两个额外的限制看成：

{j≥i−maxBoxesWj≥Wi−maxWeight \begin{cases} j \geq i - \textit{maxBoxes} \\ W_j \geq W_i - \textit{maxWeight} \end{cases}
{ 
j≥i−maxBoxes
W 
j
​
 ≥W 
i
​
 −maxWeight
​
 
注意到两个不等式右侧的值都是随着 iii 的递增而递增的，因此如果当 i=i0i=i_0i=i 
0
​
  时，某个 j0j_0j 
0
​
  不满足不等式限制，那么当 i>i0i>i_0i>i 
0
​
  时，j0j_0j 
0
​
  将永远不可能重新满足条件。

因此我们就可以使用单调队列对动态规划进行优化，对于两个可以进行转移的 gj0g_{j_0}g 
j 
0
​
 
​
  和 gj1g_{j_1}g 
j 
1
​
 
​
 ，在 j0<j1j_0 < j_1j 
0
​
 <j 
1
​
  的前提下：

如果 gj0<gj1g_{j_0} < g_{j_1}g 
j 
0
​
 
​
 <g 
j 
1
​
 
​
 ，那么我们需要将 gj0g_{j_0}g 
j 
0
​
 
​
  和 gj1g_{j_1}g 
j 
1
​
 
​
  都保留下来，这是因为当 j0j_0j 
0
​
  还满足限制时，gj0g_{j_0}g 
j 
0
​
 
​
  比 gj1g_{j_1}g 
j 
1
​
 
​
  更优；而当 j0j_0j 
0
​
  不满足限制后，gj1g_{j_1}g 
j 
1
​
 
​
  可能会代替 gj0g_{j_0}g 
j 
0
​
 
​
 ，成为新的最优转移；

如果 gj0≥gj1g_{j_0} \geq g_{j_1}g 
j 
0
​
 
​
 ≥g 
j 
1
​
 
​
 ，那么我们只需要将 gj1g_{j_1}g 
j 
1
​
 
​
  保留下来即可。这是因为当 j0j_0j 
0
​
  还满足限制时，选择 gj1g_{j_1}g 
j 
1
​
 
​
  并不会更差，并且 j1j_1j 
1
​
  可以满足限制的时间（即随着 iii 的递增）更久。

因此，我们使用一个队列存储所有需要被保留的 gjg_jg 
j
​
 （存储下标 jjj 即可），从队首到队尾，jjj 的值单调递增，gjg_jg 
j
​
  的值也单调递增。在进行状态转移求解 fif_if 
i
​
  时：

首先我们不断从队首弹出元素，直到队首的 jjj 是满足额外限制的；

使用队首的 jjj 进行转移，得到 fif_if 
i
​
 ；

计算出 gig_ig 
i
​
 ，并不断从队尾弹出元素，直到队列为空或者队尾元素对应的 ggg 值严格小与 gig_ig 
i
​
 ；

将 gig_ig 
i
​
  放入队列。

状态转移需要的时间为 O(1)O(1)O(1)。而对于单调队列的部分，每一个 gig_ig 
i
​
  会被加入队列恰好一次，并且被从队列中弹出最多一次，因此均摊时间为 O(1)O(1)O(1)。这样一来，动态规划的时间复杂度降低为 O(n)O(n)O(n)。

代码

代码中很多变量都是为了和文字部分保持一致而添加的，如果熟练了掌握了本题使用的方法，可以优化掉一些变量。

C++
Java
Python3
C
JavaScript
Golang
class Solution {
public:
    int boxDelivering(vector<vector<int>>& boxes, int portsCount, int maxBoxes, int maxWeight) {
        int n = boxes.size();
        vector<int> p(n + 1), w(n + 1), neg(n + 1);
        vector<long long> W(n + 1);
        for (int i = 1; i <= n; ++i) {
            p[i] = boxes[i - 1][0];
            w[i] = boxes[i - 1][1];
            if (i > 1) {
                neg[i] = neg[i - 1] + (p[i - 1] != p[i]);
            }
            W[i] = W[i - 1] + w[i];
        }
        
        deque<int> opt = {0};
        vector<int> f(n + 1), g(n + 1);
        
        for (int i = 1; i <= n; ++i) {
            while (i - opt.front() > maxBoxes || W[i] - W[opt.front()] > maxWeight) {
                opt.pop_front();
            }
            
            f[i] = g[opt.front()] + neg[i] + 2;
            
            if (i != n) {
                g[i] = f[i] - neg[i + 1];
                while (!opt.empty() && g[i] <= g[opt.back()]) {
                    opt.pop_back();
                }
                opt.push_back(i);
            }
        }
        
        return f[n];
    }
};
复杂度分析

时间复杂度：O(n)O(n)O(n)，其中 nnn 是数组 boxes\textit{boxes}boxes 的长度。

空间复杂度：O(n)O(n)O(n)，即为动态规划的数组 fff 和 ggg，单调队列以及前缀和数组需要使用的空间。

---
                                                            解法一： 朴素版本
通过题目发现，我们可以很简单的抽象出一个集合状态,dp[i]dp[i]dp[i]即运送前i个箱子需要的最小行程次数，那么怎么进行状态计算呢？我们可以枚举最后一次运送的状态，包括[1,2,3,....maxBoxeds]个箱子，那么枚举运送这些箱子能够产生的最小次数即可。



状态集合：
    dp[i]:运送前i个箱子需要的最少行程次数
状态计算：
    dp[i] = dp[j - 1] + cost[j, i],  (i - maxB + 1 <= j <= i)
    cost[j, i]代表第k~第i个箱子的行程次数
时间复杂度：O(n3)O(n^3)O(n 
3
 )
空间复杂度：O(n)O(n)O(n)
Java
class Solution {
    public int boxDelivering(int[][] boxes, int portsCount, int maxBoxes, int maxWeight) {
        int n = boxes.length;
        int[] dp = new int[n + 5];
        Arrays.fill(dp, 0x3f3f3f3f);
        dp[0] = 0; //初始状态为0
        for (int i = 1; i <= n; i++) {
            int sum = 0;
            for (int j = i; j >= 1 && j >= i - maxBoxes + 1; j--) {
                sum += boxes[j - 1][1]; //累加箱子的种类之和
                if (sum > maxWeight) break; //超过了最大重量
                dp[i] = Math.min(dp[i], dp[j - 1] + cost(boxes, j, i));
            }
        }  
        return dp[n];
    } 
    int cost(int[][] boxes, int l, int r) {
        int ans = 2, port = boxes[l - 1][0]; //初始话为2,因为返回仓库算一次行程
        while (++l <= r) {
            if (boxes[l - 1][0] == port) continue; //只要相同，那么次数不会增加
            ans++;  //码头不相同运输次数增加1
            port = boxes[l - 1][0];
        }
        return ans;
    }
}
解法二：时间优化
我们首先从状态计算的角度去优化：



dp[]dp[]dp[]数组右边的所有式子可以看作在一个窗口内，窗口的大小为maxBoxes,而我们现在要求的是窗口中的最小值，并且随着iii的增加窗口会向右移动。那么即转化为求滑动窗口的最小值，使用单调队列求解。

但是我们发现两段蓝色部分其实是有些地方不一样的，不一样的地方在于costcostcost的右端点是不相同的。相比于前一层来说，当前层多了一个iii端点。那么如何弥补这个差异呢，我们可以使用difdifdif来表示costcostcost的差异值,若前一个箱子i−1i-1i−1于当前箱子iii的码头相同，那么并不会增加运输次数，那么这次的dif为0，否则就会增加1。由于我们无法直接在队列中进行修改，那么可以考虑增加一个累加值dif，具体看代码实现。

例如：若之前的窗口里面保存的次数为[1,2,3][1, 2, 3][1,2,3]，那么相对于当前进来值dp[i−1]+cost[i,i]dp[i - 1] + cost[i, i]dp[i−1]+cost[i,i]来说要加上以前的差异dif进行比较后，继续构造一个单调递增的队列求解窗口的最小值。最后，将当前的次数dp[i−1]+cost[i,i]−difdp[i - 1] + cost[i, i] - difdp[i−1]+cost[i,i]−dif放入队列中，减去一个dif是因为队列中保存的是一个相对的运输次数。

同理，我们还要判断重量是否超过了maxWeightmaxWeightmaxWeight, 一样的道理，我们创建一个变量weiweiwei来代表重量的偏差值，每次比较时，队列里面的重量要加上偏差值。

那么最后我们的队列里面就存放3个元素值，a,b,c{a,b,c}a,b,c, 其中aaa为该点的编号用来判断是否在窗口外，bbb为当前值的行程数，ccc为当前的重量之和。

surprisesurprisesurprise,至此可以发现我们不仅优化了第二个循环，顺带将cost函数也进行了优化。

时间复杂度：O(n)O(n)O(n)
空间复杂度：O(n)O(n)O(n)
class Solution {
    public int boxDelivering(int[][] boxes, int portsCount, int maxBoxes, int maxWeight) {
        int n = boxes.length;
        int[] dp = new int[n + 5];
        Arrays.fill(dp, 0x3f3f3f3f);
        dp[0] = 0;
        Deque<int[]> q = new ArrayDeque<int[]>(); //双端队列
        int dif = 0, wei = 0;
        for (int i = 1; i <= n; i++) {
            int cur = dp[i - 1] + 2;//cur为每次滑动窗口增加的值即dp[i-1]+cost[i,i]
            dif += i >= 2 && boxes[i - 1][0] != boxes[i - 2][0] ? 1 : 0;//dif为运输累加值，由于我们无法直接在队列中进行修改，那么可以考虑增加一个累加值
            wei += boxes[i - 1][1]; //重量要加上当前箱子的重量
            while (!q.isEmpty() && q.peekLast()[1] + dif >= cur) q.pollLast(); //构造一个单调递增的队列
            q.add(new int[]{i, cur - dif, boxes[i - 1][1] - wei}); 
            //判断左端队头是否在窗口外 并且重量不能超过最大重量
            while (q.peekFirst()[0] <= i - maxBoxes || q.peekFirst()[2] + wei > maxWeight) q.pollFirst(); 
            dp[i] = q.peekFirst()[1] + dif; 
        }  
        return dp[n];
    } 
}
解法三：空间优化
利用变量优化dpdpdp数组

时间复杂度：O(n)O(n)O(n)
空间复杂度：O(k),k为滑动窗口大小O(k), k为滑动窗口大小O(k),k为滑动窗口大小
Java
class Solution {
    public int boxDelivering(int[][] boxes, int portsCount, int maxBoxes, int maxWeight) {
        int n = boxes.length, dp = 0;
        Deque<int[]> q = new ArrayDeque<int[]>();
        int dif = 0, wei = 0;
        for (int i = 1; i <= n; i++) {
            int cur = dp + 2;
            dif += i >= 2 && boxes[i - 1][0] != boxes[i - 2][0] ? 1 : 0;//cost[i, i] = 2
            wei += boxes[i - 1][1];
            while (!q.isEmpty() && q.peekLast()[1] + dif >= cur) q.pollLast();
            q.add(new int[]{i, cur - dif, boxes[i - 1][1] - wei}); 
            while (q.peekFirst()[0] <= i - maxBoxes || q.peekFirst()[2] + wei > maxWeight) q.pollFirst();
            dp = q.peekFirst()[1] + dif; 
        }  
        return dp;
    } 
}
解法四：优先队列
除了使用单调队列求解滑动窗口，那么还可以直接使用单调队列求解其中的最小值。

时间复杂度：O(nlogn)O(nlogn)O(nlogn)
空间复杂度：O(n)O(n)O(n)
Java
class Solution {
    public int boxDelivering(int[][] boxes, int portsCount, int maxBoxes, int maxWeight) {
        int n = boxes.length, dp = 0;
        PriorityQueue<int[]> q = new PriorityQueue<int[]>((a, b)->a[1] - b[1]);
        int dif = 0, wei = 0;
        for (int i = 1; i <= n; i++) {
            int cur = dp + 2;
            dif += i >= 2 && boxes[i - 1][0] != boxes[i - 2][0] ? 1 : 0;//cost[i, i] = 2
            wei += boxes[i - 1][1]; 
            q.add(new int[]{i, cur - dif, boxes[i - 1][1] - wei}); 
            while (q.peek()[0] <= i - maxBoxes || q.peek()[2] + wei > maxWeight) q.poll();
            dp = q.peek()[1] + dif; 
        }  
        return dp;
    } 
}
Note: 另一种思路优化：
我们首先去观察如何优化cost数组，发现可以使用前缀和进行优化。
例如：[1,2,2,3,4,3,3][1,2,2,3,4,3,3][1,2,2,3,4,3,3]，这分别是不同码头的箱子，那么怎么快速计算[l,r][l,r][l,r]的运输次数。
那么我们可以首先初始化第一个箱子的运输次数sum[1]=0sum[1] = 0sum[1]=0, 若当前箱子与前一个箱子相同，那么次数不会增加sum[i]=sum[i−1]sum[i] = sum[i -1]sum[i]=sum[i−1]，否则sum[i]=sum[i−1]+1。sum[i] = sum[i - 1] + 1。sum[i]=sum[i−1]+1。
最后，sum=[0,1,1,2,3,4,4]sum=[0, 1, 1, 2, 3, 4, 4]sum=[0,1,1,2,3,4,4], 那么cost[l,r]=sum[r]−sum[l]+2cost[l, r] = sum[r] - sum[l] + 2cost[l,r]=sum[r]−sum[l]+2。我们更新我们的状态计算如下：

那么利用前缀和数组计算的话，我们队列里面就只需要存储一下每个点的下标即可，例如i−maxB+1,...，i−1i-maxB+1,...，i-1i−maxB+1,...，i−1，每次通过下标来计算运输次数和重量即可。而解法二是直接优化前缀和数组，通过遍历答案时继续计算。

作者：Tizzi
链接：https://leetcode.cn/problems/delivering-boxes-from-storage-to-ports/solutions/2006449/by-tizzi-4ubq/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。