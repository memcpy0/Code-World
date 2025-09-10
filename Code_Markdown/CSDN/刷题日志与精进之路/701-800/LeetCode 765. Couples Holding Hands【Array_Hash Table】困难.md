<div class="notranslate"><p>
N couples sit in 2N seats arranged in a row and want to hold hands.  We want to know the minimum number of swaps so that every couple is sitting side by side.  A <i>swap</i> consists of choosing <b>any</b> two people, then they stand up and switch seats. 
</p><p>
The people and seats are represented by an integer from <code>0</code> to <code>2N-1</code>, the couples are numbered in order, the first couple being <code>(0, 1)</code>, the second couple being <code>(2, 3)</code>, and so on with the last couple being <code>(2N-2, 2N-1)</code>.
</p><p>
The couples' initial seating is given by <code>row[i]</code> being the value of the person who is initially sitting in the i-th seat.

</p><p><b>Example 1:</b> 

```css
Input: row = [0, 2, 1, 3]
Output: 1
Explanation: We only need to swap the second (row[1]) and third (row[2]) person.
```

 
 <b>Example 2:</b> 

```css
Input: row = [0, 2, 1, 3]
Output: 1
Explanation: We only need to swap the second (row[1]) and third (row[2]) person.
```

 
<b>Note:</b>
</p><ol> 
<li> <code>len(row)</code> is even and in the range of <code>[4, 60]</code>.</li>
<li> <code>row</code> is guaranteed to be a permutation of <code>0...len(row)-1</code>.</li>
</ol></div>



题意：`N` 对情侣随机分散坐在连续排列的 `2N$` 个座位上，想要牵到对方的手。一次交换可选择任意两人，让他们站起来交换座位。计算最少交换座位的次数，以便每对情侣可以并肩坐在一起。

人和座位用 `0` 到 `2N-1` 的整数表示，情侣们按顺序编号，第一对是 `(0, 1)` ，第二对是 `(2, 3)` ，以此类推，最后一对是 `(2N-2, 2N-1)` 。这些情侣的初始座位由 `row[]` 决定，其中 `row[i]` 表示编号为 `row[i]` 的人最开始坐在第 `i` 个位置上。


---
### 解法1 暴力
由于数据范围有限，可以暴力一波……$O(n^2)$ 的时间复杂度：
```cpp
#define another(x) (((x) & 1) ? ((x) - 1) : ((x) + 1))
class Solution {
public:
    int minSwapsCouples(vector<int>& row) {
        int ans = 0, len = row.size();
        for (int i = 0; i < len; i += 2) { // 每两个座位成一对，假定左边的人都是合法的不变
            int p1 = row[i], p2 = another(p1), j = i + 1;
            while (j < len && row[j] != p2) ++j; // 如果右边的人与其匹配则跳过
            if (j != i + 1) { //不匹配则找到它的匹配对象然后与它右边的人交换
                swap(row[i + 1], row[j]);
                ++ans;
            }
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了38.96% 的用户
内存消耗：7.1 MB, 在所有 C++ 提交中击败了95.09% 的用户
```
---
### 解法2 哈希表
首先记录 `row[]` 中每个情侣（编号）所在的初始位置。然后每两个座位为一对，假设左边的人都是合法的。如果右边的人不是匹配对象，则找到匹配对象所在的位置，然后交换使这对情侣坐在一起：
```cpp
#define another(x) (((x) & 1) ? ((x) - 1) : ((x) + 1))
class Solution {
public:
    int minSwapsCouples(vector<int>& row) {
        int len = row.size(), *rec = new int[len], ans = 0;
        for (int i = 0; i < len; ++i) rec[row[i]] = i; //情侣编号:初始位置
        for (int i = 0; i < len; i += 2) {
            int p1 = row[i], p2 = another(p1);
            if (row[i + 1] != p2) {
                int index = rec[p2], index2 = rec[row[i + 1]];
                ++ans;
                swap(row[i + 1], row[index]); 
                rec[row[index]] = index; //刷新情侣编号:位置记录
                rec[row[i + 1]] = index2;
            }
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7.1 MB, 在所有 C++ 提交中击败了92.86% 的用户
```


题意解读：

一对情侣，两个座位，无须交换就可以牵手成功。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307042301879.png)

{:align=center}

两对情侣，如下图所示的时候，最少须要交换 111 次。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307042301776.png)


三对情侣，如果不能够彼此牵手，只可能出现下面这种 首尾相连 的情况。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307042303353.png)

{:align=center}

四对情侣、五对情侣以上的情况也可以类似看待。通过举例，可以知道把 坐错了位置、逻辑上连在一起的情侣 拆成所有的情侣都能彼此牵手的 「最少交换次数 = 情侣对数 - 1」。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307042303123.png)

方法：并查集
「首尾相连」这件事情可以使用 并查集 表示，将输入数组相邻位置的两个 编号 在并查集中进行合并。编写代码基于了下面的事实：

如果一对情侣恰好坐在了一起，并且坐在了成组的座位上，其中一个下标一定是偶数，另一个一定是奇数，并且「偶数的值 + 1 = 奇数的值」。例如编号数对 [2, 3]、[9, 8]，这些数对的特点是除以 222（下取整）得到的数相等。

输出是什么？
要求出「最少交换次数」。假设一共有 NNN 对情侣，逻辑上连在了一起的情侣（包括坐错位置和坐对位置的情况）分别有 N1,N2,⋯ ,NnN_1,N_2,\cdots,N_nN 
1
​
 ,N 
2
​
 ,⋯,N 
n
​
  对，这里 nnn 是并查集里连通分量的个数，并且 N1+N2+⋯Nn=NN_1 + N_2 + \cdots N_n = NN 
1
​
 +N 
2
​
 +⋯N 
n
​
 =N。把逻辑上连在一起的情侣拆开，每一个连通分量至少须要 N1−1,N2−1,⋯ ,Nn−1N_1 - 1,N_2 - 1,\cdots,N_n - 1N 
1
​
 −1,N 
2
​
 −1,⋯,N 
n
​
 −1 次。



这种规律对于初始的时候已经坐在一起的情侣同样成立，因为已经坐在一起的情侣在并查集里成为一个连通分量，无须交换，此时 1−1=01 - 1 = 01−1=0。综上所述，让所有的情侣都能牵手至少须要交换的次数为

(N1−1)+(N2−1)+⋯+(Nn−1)=(N1+N2+⋯+Nn)−n=N−n (N_1 - 1) + (N_2 - 1) + \cdots + (N_n - 1) = (N_1 + N_2 + \cdots + N_n) - n = N - n
(N 
1
​
 −1)+(N 
2
​
 −1)+⋯+(N 
n
​
 −1)=(N 
1
​
 +N 
2
​
 +⋯+N 
n
​
 )−n=N−n
故「至少交换的次数 = 所有情侣的对数 - 并查集里连通分量的个数」。

参考代码：

Java
public class Solution {

    public int minSwapsCouples(int[] row) {
        int len = row.length;
        int N = len / 2;
        UnionFind unionFind = new UnionFind(N);
        for (int i = 0; i < len; i += 2) {
            unionFind.union(row[i] / 2, row[i + 1] / 2);
        }
        return N - unionFind.getCount();
    }

    private class UnionFind {

        private int[] parent;

        private int count;

        public int getCount() {
            return count;
        }

        public UnionFind(int n) {
            this.count = n;
            this.parent = new int[n];
            for (int i = 0; i < n; i++) {
                parent[i] = i;
            }
        }

        public int find(int x) {
            while (x != parent[x]) {
                parent[x] = parent[parent[x]];
                x = parent[x];
            }
            return x;
        }

        public void union(int x, int y) {
            int rootX = find(x);
            int rootY = find(y);
            if (rootX == rootY) {
                return;
            }

            parent[rootX] = rootY;
            count--;
        }
    }
}
复杂度分析：

时间复杂度： O(Nlog⁡N)O(N \log N)O(NlogN)，这里 NNN 是输入数组的长度，O(N2log⁡N2)=O(Nlog⁡N)O(\cfrac{N}{2} \log \cfrac{N}{2}) = O(N\log N)O( 
2
N
​
 log 
2
N
​
 )=O(NlogN) ；
空间复杂度：O(N)O(N)O(N)，并查集底层使用的数组长度为 N2\cfrac{N}{2} 
2
N
​
 ，O(N2)=O(N)O(\cfrac{N}{2})= O(N)O( 
2
N
​
 )=O(N)。
下一篇题解
情侣牵手
评论 (55)

排序:最热

预览
评论

沪里户外

L5
发布于 上海
2021.02.14
如果单身狗刷到这困难题，岂不暴击伤害？

85
回复

liweiwei1419
L6
发布于 福建
2021.02.14
牵手困难，所以值得被认真对待、值得珍惜，祝大家都拥有幸福美好的爱情！

51
展示 3 条回复
回复

有时间睡一觉
发布于 江西
2021.02.14
情人节来这种题目，我直接好家伙

42
回复

华山不是华生

L4
发布于 四川
(编辑过)
2021.02.14
C++ 原地交换超100%

class Solution {
public:
    int minSwapsCouples(vector<int>& row) {
        int ans = 0;
        for (int i = 0; i < row.size(); i += 2) { // 每两个数一组
            int a = row[i];
            int b = a ^ 1; // 当a为较大数时，b为较小数；当a 为较小数时，b为较大数
            for (int j = i + 1; j < row.size(); ++j) {
                if (row[j] == b) {
                    if (j != i + 1) ++ans;
                    swap(row[i + 1], row[j]);
                    break;
                }
            }
        }
        return ans;
    }
};
展开全部
22
展示 5 条回复
回复

seven
L3
发布于 上海
(编辑过)
2021.02.14
第一感觉应该是连通性问题，可以用并查集做，但按照自然智慧干预了几个例子后，总感觉可以贪心，稀里糊涂来了个贪心，抱着试试看的心态，结果就过了。牵手这么容易？？？严格证明没进行推理。

时间复杂度：O(N)，空间复杂度：O(N)，Java：0ms，100%

遍历数组，i +=2，两个数一对，先看第一个数是啥，固定他不动，寻找应该跟他配对的情侣的位置

如果他的情侣正好在 i+1 位置，则无需交换，直接看下一对；

否则，就交换：swap(i+1, 情侣位置)

public static int minSwapsCouples(int[] row) {
        int n = row.length;
        int[] indexMap = new int[n]; // 反向索引表，row[i] = num => index[num] = i
        for (int i = 0; i < n; i++) {
            indexMap[row[i]] = i;
        }
        int count = 0;
        for (int i = 0; i < n-1; i += 2) {
            int p1 = row[i];
            int p2 = (p1 & 1) == 0 ? p1 + 1 : p1 - 1; // p1的情侣
            if (row[i+1] == p2) continue; // p1和p2正好挨着，无需交换这一对
            int p2Index = indexMap[p2]; // p2的位置
            swap(row, indexMap, i+1, p2Index); // 交换 p2 <=> [i+1]
            count++;
        }
        return count;
    }

    private static void swap(int[] row, int[] indexMap, int i, int j) {
        int tmp = row[i];
        row[i] = row[j];
        row[j] = tmp;
        indexMap[row[i]] = i;
        indexMap[row[j]] = j;
    }
12
隐藏回复
回复

liweiwei1419
L6
发布于 福建
2021.02.14
@seven 反向查找这个思想针的棒极了，非常不戳！节日快乐呀！

0
回复

liweiwei1419
L6
发布于 福建
2021.02.14
@seven 其实并查集也用到了「贪心算法」的思想：假设某个连通块里有 A 对情侣，至少要交换 A - 1 次就可以让他们彼此牵手。

我接下来的证明是描述性的，并不能作为严谨的证明，仅供参考：

在一个连通块中的情侣，呈现的座位分布恰好是「错位」的，因此把它们「拆开」，每交换一次，「除了最后一次交换以外，最多只能使得一对情侣坐到正确的位置上」。所以需要 A 对在一个连通块中的情侣，至少需要 A - 1 次交换让他们彼此牵手。

贪心的思想是：让每一个连通块中最优，整体就最优。

再夸一下，您这个「反向查找」我真的是太喜欢了，我学过「索引堆」我都没想到「反向查找」可以用在这题上。

3
回复

seven
L3
发布于 上海
2021.02.14
@liweiwei1419 这个并查集思路真不错，这种连通方式没想到，大佬的视频题解做的真好，经常被醍醐灌顶。节日快乐！

1
回复

liweiwei1419
L6
发布于 福建
2021.02.14
@seven 感谢认可，我会再接再厉的！

0
回复

seven
L3
发布于 上海
2021.02.14
@liweiwei1419 👍 我想的其实比较简单，最开始就是为了找到couple的位置，交换过来，不加反向索引，就O(N^2)了，自然就想到了反向索引

0
回复
展示更多
隐藏

qyaaaa

L3
发布于 山西
2021.02.14
好家伙，我刷个题也杀🐕啊！

9
回复

菜
L1
发布于 湖北
2021.02.14
也就是说做错位置的情侣会使图中连通分量的个数减小

6
展示 1 条回复
回复

syn17
发布于 内蒙古
2021.02.14
不是，，情侣牵手，困难（下面的难度标识） 几个意思.......

6
回复

JXJXHC
L1
发布于 中国香港
2021.02.14
对于其中“交换之后连通分量的个数 - 交换之前连通分量的个数 = 交换的最少次数”，也就是最后return N - uf.getCount() 这个一个点，大佬没有作更多说明，我看的时候有点疑惑，现在来给出一个自己的简单解释吧。 我们最终需要的是N对情侣全部分开，所以 “交换之后的连通分量”就是N无疑，交换之前的连通分量就是我们uf.getCount(), 假设为m，即交换之前有m个连通分量。其中对于每个连通分量，假设各自的成员数（情侣对数）分别为 cluster c1, c2, c3......cm。对于每一个连通分量，其最少交换次数都是n - 1次（都是循环交换），因此总最少交换次数就是 (c1 - 1) + (c2 - 1) + (c3 - 1) +......+ (cm - 1)。括号拆出来，再合并一下就变成了 (c1 + c2 + c3 +....+ cm) - (1 + 1 + 1 +...... + 1)，其中左边括号加起来就相当于是总情侣对数N, 右边共有m个，如此最后最少交换次数就应该是 N - m 了。

作者：LeetCode
链接：https://leetcode.cn/problems/couples-holding-hands/solutions/599958/qing-lu-qian-shou-by-leetcode-gl1c/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。


并查集


首先，我们总是以「情侣对」为单位进行设想：

当有两对情侣相互坐错了位置，ta们两对之间形成了一个环。需要进行一次交换，使得每队情侣独立（相互牵手）

如果三对情侣相互坐错了位置，ta们三对之间形成了一个环，需要进行两次交换，使得每队情侣独立（相互牵手）

如果四对情侣相互坐错了位置，ta们四对之间形成了一个环，需要进行三次交换，使得每队情侣独立（相互牵手）

也就是说，如果我们有 k 对情侣形成了错误环，需要交换 k - 1 次才能让情侣牵手。

于是问题转化成 n / 2 对情侣中，有多少个这样的环。

可以直接使用「并查集」来做。

由于 0和1配对、2和3配对 ... 因此互为情侣的两个编号除以 2 对应同一个数字，可直接作为它们的「情侣组」编号：

Java
 class Solution {
    int[] p = new int[70];
    void union(int a, int b) {
        p[find(a)] = p[find(b)];
    }
    int find(int x) {
        if (p[x] != x) p[x] = find(p[x]);
        return p[x];
    }
    public int minSwapsCouples(int[] row) {
        int n = row.length, m = n / 2;
        for (int i = 0; i < m; i++) p[i] = i;
        for (int i = 0; i < n; i += 2) union(row[i] / 2, row[i + 1] / 2);
        int cnt = 0;
        for (int i = 0; i < m; i++) {
            if (i == find(i)) cnt++;
        }
        return m - cnt;
    }
}
时间复杂度：O(n)O(n)O(n)
空间复杂度：O(n)O(n)O(n)
贪心


还是以「情侣对」为单位进行分析：

由于题目保证有解，我们也可以从前往后（每两格作为一步）处理，对于某一个位置而言，如果下一个位置不是应该出现的情侣的话。

则对下一个位置进行交换。

同时为了方便我们找到某个值的下标，需要先对 row 进行预处理（可以使用哈希表或数组）。

Java
class Solution {
    public int minSwapsCouples(int[] row) {
        int n = row.length;
        int ans = 0;
        int[] cache = new int[n];
        for (int i = 0; i < n; i++) cache[row[i]] = i;
        for (int i = 0; i < n - 1; i += 2) {
            int a = row[i], b = a ^ 1;
            if (row[i + 1] != b) {
                int src = i + 1, tar = cache[b];
                cache[row[tar]] = src;
                cache[row[src]] = tar;
                swap(row, src, tar);
                ans++;
            }
        }
        return ans;
    }
    void swap(int[] nums, int a, int b) {
        int c = nums[a];
        nums[a] = nums[b];
        nums[b] = c;
    }
}
时间复杂度：O(n)O(n)O(n)
空间复杂度：O(n)O(n)O(n)
证明/分析
我们这样的做法本质是什么？

其实相当于，当我处理到第 k 个位置的时候，前面的 k - 1 个位置的情侣已经牵手成功了。我接下来怎么处理，能够使得总花销最低。

分两种情况讨论：

a. 现在处理第 k 个位置，使其牵手成功：

那么我要使得第 k 个位置的情侣也牵手成功，那么必然是保留第 k 个位置的情侣中其中一位，再进行修改，这样的成本是最小的（因为只需要交换一次）。

而且由于前面的情侣已经牵手成功了，因此交换的情侣必然在 k 位置的后面。

然后我们再考虑交换左边或者右边对最终结果的影响。

分两种情况来讨论：

与第 k 个位置的匹配的两个情侣不在同一个位置上：这时候无论交换左边还是右边，后面需要调整的「情侣对数量」都是一样。假设处理第 k 个位置前需要调整的数量为 n 的话，处理完第 k 个位置（交换左边或是右边），需要调整的「情侣对数量」都为 n - 1 。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307042317233.png)

与第 k 个位置的匹配的两个情侣在同一个位置上：这时候无论交换左边还是右边，后面需要调整的「情侣对数量」都是一样。假设处理第 k 个位置前需要调整的数量为 n 的话，处理完第 k 个位置（交换左边或是右边），需要调整的「情侣对数量」都为 n - 2 。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307042317185.png)

因此对于第 k 个位置而言，交换左边还是右边，并不会影响后续需要调整的「情侣对数量」。

b. 现在先不处理第 k 个位置，等到后面的情侣处理的时候「顺便」处理第 k 位置：

由于我们最终都是要所有位置的情侣牵手，而且每一个数值对应的情侣数值是唯一确定的。

因此我们这个等“后面”的位置处理，其实就是等与第 k 个位置互为情侣的位置处理（对应上图的就是我们是在等 【0 x】和【8 y】或者【0 8】这些位置被处理）。

由于被处理都是同一批的联通位置，因此和「a. 现在处理第 k 个位置」的分析结果是一样的。

不失一般性的，我们可以将这个分析推广到第一个位置，其实就已经是符合「当我处理到第 k 个位置的时候，前面的 k - 1 个位置的情侣已经牵手成功了」的定义了。

综上所述，我们只需要确保从前往后处理，并且每次处理都保留第 k 个位置的其中一位，无论保留的左边还是右边都能得到最优解。

作者：宫水三叶
链接：https://leetcode.cn/problems/couples-holding-hands/solutions/603505/liang-chong-100-de-jie-fa-bing-cha-ji-ta-26a6/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。