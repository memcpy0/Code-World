> 算法竞赛：从入门到进阶，罗勇军

BFS搜索处理的对象不仅可以是一个数，还可以是一种状态。八数码问题是典型的状态图搜索问题。
> **判断无解的情况**：一个状态表示成一维的形式，求出除 $0$ 之外所有数字的逆序数之和，也就是每个数字前面比它大的数字的个数的和，称为这个状态的逆序。**若两个状态的逆序奇偶性相同，则可相互到达，否则不可相互到达**。

# 1. 八数码问题
在一个 $3\times 3$ 的棋盘上放置编号为 $1\sim 8$ 的八个方格，每个一格，另外还有一个空格。与空格相邻的数字方块可以移动到空格里。
1. 任务1：指定初始棋局和目标棋局（如下图所示），计算出最少的移动步数；
2. 任务2：输出数码的移动序列。

把空格看做 $0$ ，一共有 $9$ 个数字。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202311230115125.png)
输入样例：
```
1 2 3 0 8 4 7 6 5
1 0 3 8 2 4 7 6 5
```
输出样例：
```
2
```

把一个棋局看做一个状态图，总共有 $9 ! = 362 880$ 个状态。从初始棋局开始，每次移动转到下一个状态，到达目标棋局后停止。

八数码问题是一个**经典BFS问题**。BFS是从近到远的扩散过程，适合解决最短距离问题。八数码从初始状态出发，每次转移都逐步逼近目标状态。每转移一次，步数+1，当到达目标时，经过的步数就是最短路径。

下图是样例的转移过程。该图中起点是 $(A, 0)$ ，$A$ 表示状态，即 `{1 2 3 0 8 4 7 6 5}` 这个棋局；$0$ 是距离起点的步数。从初始状态出发，移动数字 $0$ 到邻居位置，按左、上、右、小的顺时针顺序，有 $3$ 个转移状态 $B, C, D$ ；目标状态是 $F$ ，停止。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202311230142117.png)
用队列描述这个BFS过程：
1. $A$ 进队，当前队列是 $\{ A\}$ ；
2. $A$ 进队，$A$ 的邻居 $B, C, D$ 进队，当前队列是 $\{ B, C, D\}$ ，步数为 $1$ ；
3. $B$ 出队，$E$ 进队，当前队列是 $\{ C, D, E\}$ ，$E$ 的步数为 $2$ ；
4. $C$ 出队，转移到 $F$ ，检验 $F$ 是目标状态，停止，输出 $F$ 的步数 $2$ 。

仔细分析上述过程，发现从 $B$ 状态出发实际上有 $E, X$ 两个转移方向，而 $X$ 正好是初始状态 $A$ ，重复了。同理 $Y$ 状态也是重复的。如果不去掉这些重复的状态，程序会产生很多无效操作，复杂度大大增加。因此八数码的重要问题其实是判重。

如果用暴力的方法判重，每次把新状态与 $9!$ 个状态对比，可能有 $9! \times 9!$ 次检查，不可行。因此需要一个快速的判重方法。还可用哈希集合，存储序列字符串，每次检查该字符串是否出现，但也需要很多存储空间。

本题可以用数学方法**康托展开** *Cantor Expansion* 判重。

---
# 2. 康托展开、树状数组优化
康托展开是一种特殊的哈希函数，==它将一个排列映射为一个自然数，该自然数表示「该排列在**按字典序从小到大排序的全排列**中的顺序」==。本题中，康托展开完成了如下表所示的工作：

| 状态 | 012345678 | 012345687 | 012345768 | 012345786 | ... | 876543210 |
|:---|:---|:---|:---|:---|:---|:---|:---|
| Cantor | 0 | 1 | 2 | 3 | ... | 362 880 - 1 |

第 $1$ 行是 $0\sim 8$ 这 $9$ 个数字的全排列，共 $9!$ 个，按从小到大排序。第 $2$ 行是每个排列对应的位置，例如最小的 `{012345678}` 在第 $0$ 个位置，最大的 `{876543210}` 在最后的位置。

函数 `Cantor` 实现的功能是：输入一个排列，即第 $1$ 行的某个排列，计算出它的 `Cantor` 值，即第 $2$ 行对应的数。`Cantor` 的复杂度为 $O(n^2)$ ，$n$ 是集合中元素的个数。

有了这个函数，八数码的程序能很快判重：每转移到一个新状态，就用 `Cantor` 判断这个状态是否处理过，如果处理过，就不转移。

下面举例讲解康托展开的原理。例子：判断 `2143` 是 `{1, 2, 3, 4}` 的全排列中第几大的数。计算排在 `2143` 前面的排列数目，可以将问题转换为以下排列的和：
1. 首位小于 $2$ 的所有排列。后面比 $2$ 小的只有 $1$ 一个数，我们可以把 $1$ 放在 $2$ 的位置上，然后后面 $3$ 个数的排列有 $3 \times 2 \times 1 = 3! = 6$ 个（即 `1234, 1243, 1324, 1342, 1423, 1432` ），写成 $1 \times 3! = 6$ 。
2. 首位为 $2$ 、第二位小于 $1$ 的所有排列。无，写成 $0\times 2! = 0$ 。
3. 前两位为 $21$ 、第三位小于 $4$ 的所有排列。只有 $3$ 一个数可被放在 $4$ 的位置上（即 `2134` ），写成 $1\times 1! = 1$ 。
4. 前三位为 $214$ 、第四位小于 $3$ 的所有排列。无，写成 $0 \times 0! = 0$ 。

求和：$1 \times 3! + 0 \times 2! + 1 \times 1! + 0 \times 0 ! = 7$ ，所以 `2143` 是第八大的数。如果用 `int visited[24]` 数组记录各排列的位置，`{2143}` 就是 `visited[7]` ；第一次访问这个排列时，置 `visited[7] = 1` ；再次访问这个排列时，发现 `visited[7] = 1` ，说明已经处理过，判重。

根据上面的例子得到康托展开公式。把一个集合产生的全排列，按字典序排序，第 $X$ 个排列的计算公式如下：
$$X = a[n] \times (n - 1)! + a[n - 1] \times (n - 2)! + \dots + a[i] \times (i - 1)! + ... + a[2] \times 1! + a[1] \times 0!$$
其中 $a[i]$ 为「后面元素中小于当前元素的个数」，并且有 $0 \le a[i] < i\ (1 \le i \le n)$ 。
```cpp
int Cantor(int str[], int n) { // 用康托展开判重
    long result = 0;
    for (int i = 0; i < n; ++i) {
        int counted = 0;
        for (int j = i + 1; j < n; ++j) {
            if (str[i] > str[j]) // 可优化
                ++counted;
        }
        result += counted * factory[n - i - 1];
    }
    return result;
}
```
当 $n$ 特别大（如一百万）时，上述代码显然会超时。不难注意到，**对当前元素 $x$ 求出后面小于 $x$ 的元素个数**这一操作是可以优化的。我们完全可以**从后往前遍历求康托展开**，每次使用树状数组求出「已记录元素中小于 $x$ 的个数」，复杂度为 $O(\log n)$ ，然后将当前元素 $x$ 也记录在树状数组中。
```cpp
#include <bits/stdc++.h>
using namespace std;
constexpr int mod = 998244353;
constexpr int maxn = 1000010;
int n;
int arr[maxn]; 
int tree[maxn];
int factory[maxn];
void calFac(int n) {
    factory[0] = 0;
    factory[1] = 1; 
    for (int i = 2; i <= n; ++i) factory[i] = (long long)factory[i - 1] * i % mod;
}
void modify(int i) {
    while (i < maxn) {
        ++tree[i];
        i += (i & (-i));    
    }
}
int sum(int i) {
    int ans = 0;
    while (i) {
        ans += tree[i];
        i = i & (i - 1); // i -= (i & (-i))    
    }
    return ans;
}
int Cantor() {
    int ans = 0;
    for (int i = n - 1; i >= 0; --i) {
        int counted = sum(arr[i]);
        ans = (ans + (long long)counted * factory[n - i - 1]) % mod;     
        modify(arr[i]);
    }
    return ans + 1;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) scanf("%d", &arr[i]);
    calFac(n);
    printf("%d", Cantor());
    return 0;
}
```


---
# 3. 康托逆展开
上述过程的反过程就是康托逆展开。输入一个数字 $k$ ，返回某个集合的全排列中第 $k$ 大的排列。
> 根据康托展开的描述：康托展开是一个全排列到一个自然数的**双射**，双射的意思也就是从康托展开可以得出从一个全排列到一个自然数的一个映射，也可以得出一个自然数到全排列的映射。**康托展开是可逆的**。

对于 `{1,2,3,4}` 的所有全排列，求解在所有由小到大全排列中、排在第 $15$ 位的排列是多少，由康托展开很容易逆推出这个排列，**从最高位开始依次求解每一位的数**。具体流程如下：
1. 求解第 $4$ 位：$\dfrac{15} {(4-1)!} = 15 / 6 = 2 \dots 3$ ，说明比第四位小的数字有 $2$ 个，则最高位数字从 `{1,2,3,4}` 里选第 $3$ 个数（注意：**选择前需要按照从小到大的顺序**），所以最高位为 $3$
2. 求解第 $3$ 位：$\dfrac{3} { (3-1)! } = 3 / 2 = 1 \dots 1$ ，说明比第三位小的数字有 $1$ 个，则百位数字从 `{1,2,4}` 里选第 $2$ 个数（注意：因为 $3$ 已经被最高位选过了，所以这里要将 $3$ 从待选择的集合里去除），所以百位数字为 $2$ 
3. 求解第 $2$ 位：$\dfrac{ 1} {(2-1)!} = 1 / 1 = 1 \dots 0$ ，说明比第二位小的数字有 $1$ 个，则十位数字从 `{1,4}` 里选第 $2$ 个数字，所以十位数字为 $4$ 
4. 求解第 $1$ 位，也就是最后一位：$\dfrac{0} {(1-1)!} = 0$ ，说明比第一位小的数字有 $0$ 个，则个位数字从 `{1}` 里选第 $1$ 个数字，所以个数位数为 $1$ 

所以对于 `{1,2,3,4}` 的、所有由小到大全排列中，排在第 $15$ 位的排列是 `3241` 。

通过上述流程，可以发现**逆康托的求解排列流程**就是下面两步（另一种推导方法见[[../刷题日志与精进之路/LeetCode 60. 排列序列【数学,逆康托展开】困难|LeetCode 60. 排列序列【数学,逆康托展开】困难]]）：
1. 求解第 $i$ 位的数字：$\dfrac{X} {(i-1)!} = a \dots b$ ，从 $(a_1, a_2, a_3, \dots , a_n)$ 里选择第 $a+1$ 个数，$a_1<a_2<a_3<\dots <a_n$ 。
2. 求解第 $i-1$ 位的数字：$\dfrac{b} {(i-2)!} = c \dots d$ ，假定上一步选择的第 $a+1$ 个数为 $a_2$ ，则这一步从 $(a_1, a_3, \dots ,a_n)$ 中选择第 $c+1$ 个数。
3. **每次求解第 $i$ 位的数字时，使用上一步的余数（初始时为给定的数字 $k$ ）** 除以 $(i-1)!$ 所得的商加 $1$ ，就是要从待选择的集合中选取的第几个数。**每次选择一个数后，将这个数从待选择的集合中删除**。


```java
class Solution {
    public static String getPermutation(int n, int k) {
        // 保存从0~n-1的所有阶乘结果
        int[] factorial = new int[n];
        factorial[0] = 1;
        //求阶乘
        for (int i = 1; i < n; i++) {
            factorial[i] = i * factorial[i - 1];
        }
        // 标记哪些数字是可以待选择的，false表示可以选择
        boolean[] used = new boolean[n + 1];
        // 保存结果
        StringBuilder result = new StringBuilder();
        // 从最高位开始依次求解排列的每一位
        for (int i = 1; i <= n; i++) {
            int num = k / factorial[n - i]; // 商
            k = k % factorial[n - i];       // 余数
            // 从待选择的集合中选择第num + 1个数
            for (int j = 1; j <= n; j++) {
                if (!used[j]) {
                    if (num == 0) {
                        result.append(j);
                        used[j] = true;
                        break;
                    }
                    num--;
                }
            }
        }
        return result.toString();
    }
}
```

---
# 4. 八数码问题解法1：BFS+Cantor 
本题中用朴素 `Cantor` 写法，进行搜索和判重的总复杂度是 $O(n! n^2)$ ，远比用暴力判重的总复杂度 $O(n! n!)$ 小。
```cpp
#include <bits/stdc++.h>
const int LEN = 362880; // 状态共9!=362880种
using namespace std;
struct node {
    int state[9]; // 记录一个八数码的排列,即一个状态
    int dis;      // 记录到起点的距离
};
int dir[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

int visited[LEN] = {0}; // 与每个状态对应的记录,Cantor函数对它进行置数,并判重
int start[9];           // 开始状态
int goal[9];            // 结束状态
long int factory[] = {1,1,2,6,24,120,720,5040,40320,362880}; // Cantor用到的常数

bool Cantor(int str[], int n) { // 用康托展开判重
    long result = 0;
    for (int i = 0; i < n; ++i) {
        int counted = 0;
        for (int j = i + 1; j < n; ++j) {
            if (str[i] > str[j]) 
                ++counted;
        }
        result += counted * factory[n - i - 1];
    }
    if (!visited[result]) { // 没有被访问过
        visited[result] = 1;
        return 1;
    }
    return 0;
}
int bfs() {
    node head;
    memcpy(head.state, start, sizeof(head.state)); // 复制起点的状态
    head.dis = 0;
    queue<node> q;         // 队列中的内容记录状态
    Cantor(head.state, 9); // 用康托展开判重,目的是对起点的visited[]赋初值
    q.push(head);          // 第一个进队列的是起点状态

    while (!q.empty) {     // 处理队列
        head = q.front();
        q.pop();           // 可在此处打印head.state,看弹出队列的情况
        int z;
        for (z = 0; z < 9; ++z) // 找这个状态中元素0的位置
            if (head.state[z] == 0) // 找到了
                break;
        // 转换为二维,左上角是原点(0,0)
        int x = z % 3;    
        int y = z / 3;
        for (int i = 0; i < 4; ++i) { // 上下左右最多可能有4个新状态
            int newx = x + dir[i][0];
            int newy = y + dir[i][1];
            int nz = newx + 3 * newy; // 转换为一维
            if (newx >= 0 && newx < 3 && newy >= 0 && newy < 3) { // 未越界
                node newnode;
                memcpy(&newnode, &head, sizeof(struct node)); // 复制新的状态
                swap(newnode.state[z], newnode.state[nz]);    // 把零移到新的位置
                newnode.dis++;
                if (memcmp(newnode.state, goal, sizeof(goal)) == 0) // 与目标状态对比
                    return newnode.dis;                             // 到达目标状态,返回距离,结束
                if (Cantor(newnode.state, 9)) // 用康托展开判重
                    q.push(newnode);          // 把新的状态放进队列
            }
        }
    }
    return -1;
}
int main() {
    for (int i = 0; i < 9; ++i) cin >> start[i];
    for (int i = 0; i < 9; ++i) cin >> goal[i];
    int num = bfs();
    if (num != -1) cout << num << endl;
    else cout << "Impossible" << endl;
    return 0;
}
```
**15数码问题**。八数码问题只有 $9!$ 种状态。对于更大的问题，例如 $4\times 4$ 棋盘的 $15$ 数码问题，有 $16!  \approx 2\times 10^{13}$ 种状态，如果仍然用数组存储状态，远远不够，此时需要更好的算法。
> 八数码问题的多种解法，例如双向广搜，`A*` ，`IDA*` 等，参考 https://www.cnblogs.com/zufezzt/p/5659276.html 

---
# 习题
康托展开的题目：
- 洛谷：[P5367 【模板】康托展开](https://www.luogu.com.cn/problem/P5367) ，这里的排列长度最高为 $1000000$ ，$O(n^2)$ 的朴素 `Cantor` 方法是不行的。

逆康托展开的应用题目：
- LeetCode第60题：排列序列。注意：这里的排列编号是从 $1$ 开始的，而康托展开中编号是从 $0$ 开始的，所以只要在求解前，对 $k$ 进行减 $1$ 操作就可以了。

数码问题：
- POJ 1077 "Eight"，八数码问题。还可用别的算法完成这道题，如A星算法。

