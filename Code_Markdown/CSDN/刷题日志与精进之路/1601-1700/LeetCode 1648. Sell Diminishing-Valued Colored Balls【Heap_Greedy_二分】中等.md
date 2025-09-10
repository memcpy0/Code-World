<p>You have an <code>inventory</code> of different colored balls, and there is a customer that wants <code>orders</code> balls of <strong>any</strong> color.</p>

<p>The customer weirdly values the colored balls. Each colored ball's value is the number of balls <strong>of that color&nbsp;</strong>you currently have in your <code>inventory</code>. For example, if you own <code>6</code> yellow balls, the customer would pay <code>6</code> for the first yellow ball. After the transaction, there are only <code>5</code> yellow balls left, so the next yellow ball is then valued at <code>5</code> (i.e., the value of the balls decreases as you sell more to the customer).</p>

<p>You are given an integer array, <code>inventory</code>, where <code>inventory[i]</code> represents the number of balls of the <code>i<sup>th</sup></code> color that you initially own. You are also given an integer <code>orders</code>, which represents the total number of balls that the customer wants. You can sell the balls <strong>in any order</strong>.</p>

<p>Return <em>the <strong>maximum</strong> total value that you can attain after selling </em><code>orders</code><em> colored balls</em>. As the answer may be too large, return it <strong>modulo </strong><code>10<sup>9 </sup>+ 7</code>.</p>

 
<p><strong>Example 1:</strong></p>
<img style="width: 480px; height: 270px;" src="https://assets.leetcode.com/uploads/2020/11/05/jj.gif" alt="" width="60%">

```clike
Input: inventory = [2,5], orders = 4
Output: 14
Explanation: Sell the 1st color 1 time (2) and the 2nd color 3 times (5 + 4 + 3).
The maximum total value is 2 + 5 + 4 + 3 = 14.
```

 
<p><strong>Example 2:</strong></p>

```clike
Input: inventory = [3,5], orders = 6
Output: 19
Explanation: Sell the 1st color 2 times (3 + 2) and the 2nd color 4 times (5 + 4 + 3 + 2).
The maximum total value is 3 + 2 + 5 + 4 + 3 + 2 = 19.
```

 

<p><strong>Example 3:</strong></p>

```clike
Input: inventory = [2,8,4,10,6], orders = 20
Output: 110 
```

<p><strong>Example 4:</strong></p>

```clike
Input: inventory = [1000000000], orders = 1000000000
Output: 21
Explanation: Sell the 1st color 1000000000 times for a total value of 500000000500000000. 500000000500000000 modulo 109 + 7 = 21.
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= inventory.length &lt;= 10<sup>5</sup></code></li>
	<li><code>1 &lt;= inventory[i] &lt;= 10<sup>9</sup></code></li>
	<li><code>1 &lt;= orders &lt;= min(sum(inventory[i]), 10<sup>9</sup>)</code></li>
</ul>




题意：你有一些球的库存&nbsp;<code>inventory</code>&nbsp;，里面包含着不同颜色的球。一个顾客想要&nbsp;<strong>任意颜色</strong> 总数为&nbsp;<code>orders</code>&nbsp;的球。 

<p>这位顾客有一种特殊的方式衡量球的价值：每个球的价值是目前剩下的&nbsp;<strong>同色球</strong>&nbsp;的数目。比方说还剩下&nbsp;<code>6</code>&nbsp;个黄球，那么顾客买第一个黄球的时候该黄球的价值为&nbsp;<code>6</code>&nbsp;。这笔交易以后，只剩下&nbsp;<code>5</code>&nbsp;个黄球了，所以下一个黄球的价值为&nbsp;<code>5</code>&nbsp;（也就是球的价值随着顾客购买同色球是递减的）</p>

<p>给你整数数组&nbsp;<code>inventory</code>&nbsp;，其中&nbsp;<code>inventory[i]</code>&nbsp;表示第&nbsp;<code>i</code>&nbsp;种颜色球一开始的数目。同时给你整数&nbsp;<code>orders</code>&nbsp;，表示顾客总共想买的球数目。你可以按照 <strong>任意顺序</strong>&nbsp;卖球。返回卖了 <code>orders</code>&nbsp;个球以后 <strong>最大</strong>&nbsp;总价值之和。由于答案可能会很大，请你返回答案对 <code>10<sup>9</sup>&nbsp;+ 7</code>&nbsp;<strong>取余数</strong>&nbsp;的结果。</p>

---
### 解法1 堆+贪心
贪心策略易知，只要每次都卖出剩下同色球数目最多的球，就能够得到卖了 `orders` 个球之后**最大总价值之和**。不过在具体的实现上，需要仔细斟酌。
- 如果每次选择同色球数目最多的球，卖出一个球后再次插入到堆中，做法是对的但是效率很低，对于 `inventory = [1000000000], orders = 1000000000` 这种示例就无计可施。
- 如果每次选择同色球数目最多的球，卖出部分球，直到和数目第二多的球一样多，然后再插入到堆中……下一个数目最多的球就不好处理。

因此，我们的做法是，令**同色球数目最多的球**的种类数为 `m = 1` ：
- 每次出堆数目最多的球，如果存在其他球，且还无法完全满足订单数时——卖出部分球，直到和数目第二多的球一样多，**不再插入到堆中**。此时，同色球数目最多的球的种类数 `m` 加一。下次如果还进入这一分支，考虑到数目第二多的球，**就可以将 `m` 种球分别卖出部分，直到和数目第二多的球一致，而后 `m` 加一**。
- 如果不存在其他球，或者可以满足订单数而不需要考虑时，此时同色球数目最多的球的种类数为 `m` ，我们将剩余的订单数分为 `orders / m` 份，每种球分别卖出 `orders / m` 份，如果还有多的 `orders % m` 份，就让 `orders % m` 种球分别卖出一个。接着退出循环。

实际代码如下，有一点复杂：
```cpp
class Solution { 
public:
    int maxProfit(vector<int>& inventory, int orders) {
        const int mod = 1e9 + 7;
        using ull = unsigned long long;
        const vector<int>& inv = inventory;
        function<ull(ull, ull)> getVals = [&mod](ull b, ull e) -> ull { //首项,末项都<=1e9
            ull m = b + e, n = e - b + 1; //m为首项和末项和,n为项数
            return m * n / 2 % mod; //m*n可能溢出int范围
        };
        if (inv.size() == 1) return getVals(inv[0] - orders + 1, inv[0]); //只有一种球时
        priority_queue<int> pq(inventory.begin(), inventory.end());
        int ans = 0, m = 1; //m是同色球数目最多的球的种类数
        while (orders > 0) {
            int e = pq.top(); pq.pop();
            if (!pq.empty() && (e - pq.top()) * m <= orders) { //还有其他球时
                ans = (ans + getVals(pq.top() + 1, e) * m) % mod; //getVals*m可能溢出int范围
                orders -= (e - pq.top()) * m;
            } else { //没有其他球,或者可以满足订单数不需要考虑其他球
                int x = orders / m; //剩余订单可以分成x份
                int y = orders % m; //多出来的
                ull b = e - x;
                ans = (ans + getVals(b + 1, e) * m) % mod; //getVals*m可能溢出int范围
                ans = (ans + b * y) % mod;
                break;
            }
            ++m;
        }
        return ans;
    }
}; 
```
运行效率如下：
```cpp
执行用时：168 ms, 在所有 C++ 提交中击败了54.26% 的用户
内存消耗：66 MB, 在所有 C++ 提交中击败了14.73% 的用户
```

---
### 解法2 二分
其实我们还可以在这道题中使用二分，原因在于，题目说明==我们能够满足订单数，而且只要卖出的球越多，得到的总价值和就越大==，**满足二分需要的单调性**。不过，具体的二分策略需要仔细考虑。

我们将各种球的数目绘制成一个柱形图，如解法1中所做的那样，算法的最后，同色球数目最多的 `m` 种球数目几乎相等，基本相当于在柱形图上画出一条平行于 `x` 轴的直线或者下界，**卖出横线之上的球**。

**我们不断二分试探下界的位置**，直到找到满足横线 `T` 之上球数 `<= orders` 的第一个下界 `T` ，计算这些球卖出的价值之和。如果有剩下的球，就以横线 `T` 的单价值卖出。
```cpp
class Solution { 
public:
    int maxProfit(vector<int>& inventory, int orders) {
        const int mod = 1e9 + 7;
        using ull = unsigned long long;
        const vector<int>& inv = inventory;
        function<ull(ull, ull)> getVals = [&mod](ull b, ull e) -> ull { //首项,末项都<=1e9
            ull m = b + e, n = e - b + 1; //m为首项和末项和,n为项数
            return m * n / 2 % mod; //m*n可能溢出int范围
        };
        if (inv.size() == 1) return getVals(inv[0] - orders + 1, inv[0]); //只有一种球时
        int lo = 0, hi = *max_element(inv.begin(), inv.end()); //lo和hi都是y轴的标尺
        while (lo < hi) { //找到满足横线之上球数<=orders的第一个下界
            int mid = lo + (hi - lo) / 2, t = 0; 
            for (int v : inv) {
                if (v > mid) {
                    t += v - mid;
                    if (t > orders) break;
                }
            }
            if (t <= orders) hi = mid;
            else lo = mid + 1;
        } 
        ull T = lo, ans = 0, rest = orders;
        for (int v : inv) {
            if (v > T) {
                ans += getVals(T + 1, v);
                rest -= (v - T); //这种球中卖出v-T个球
            }
        }
        ans += rest * T;
        return ans % mod;
    }
}; 
```
运行效率如下：
```cpp
执行用时：128 ms, 在所有 C++ 提交中击败了93.80% 的用户
内存消耗：63.2 MB, 在所有 C++ 提交中击败了79.07% 的用户
```
使用 O(n)O(n)O(n) 的快速选择算法。

代码
/***************************************************
Author: hqztrue
https://github.com/hqztrue/LeetCodeSolutions
Complexity: O(n)
If you find this solution helpful, plz give a star:)
***************************************************/
typedef long long ll;
const int P=1000000007;
int tot,val; ll num,ans;
template<class T>
void Q(T a[],int s,int t){
	if (s>t){
		int x=tot/num;
		ans+=num*(val*2-x+1)*x/2;
		ans+=tot%num*(val-x);
		return;
	}
	int i=s,j=t,tmp,m=a[s+rand()%(t-s+1)];
	do {
		while (a[i]<m)++i;
		while (a[j]>m)--j;
		if (i<=j){
			tmp=a[i]; a[i]=a[j]; a[j]=tmp;
			++i; --j;
		}
	}while (i<=j);
	ll c=num*(val-m)-(ll)m*(t-j);
	for (int k=j+1;k<=t;++k)c+=a[k];
	if (c<=tot){
		tot-=c; ans+=num*(val+m+1)*(val-m)/2;
		for (int k=j+1;k<=t;++k)ans+=(ll)(a[k]+m+1)*(a[k]-m)/2;
		val=m; num+=t-j;
		Q(a,s,j);
	}
	else {
		if (a[j+1]==m)++j;
		Q(a,j+1,t);
	}
}
class Solution {
public:
	int maxProfit(vector<int>& a, int orders) {
		int n=a.size(); tot=orders; ans=0; num=0;
		Q(&a[0],0,n-1);
		return ans%P;
	}
};