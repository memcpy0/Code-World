这是湖南科技大学OJ中的一道题目，因为在刷贴吧的时候看到了，所以就做了。

---
**题目描述**
红魔馆最近迎来了两块陨石，由于防范不到位，第一块陨石从天而降，红魔馆爆炸了。馆主蕾米莉亚有着强大的力量，她决定阻止第二块陨石，方法是将陨石打爆，使其分裂成若干个小石块。

为了简化问题，我们可以将一块陨石看作由连续的 `n` 个石块组成，每个石块包含了一定能量，蕾米莉亚需要毁掉其中某一段连续的石块(该段能量和为 `s` )，被毁掉的石块将会消失，且因连锁反应还会使剩余石块总能量减少 `s` ，蕾米莉亚需要保证剩余石块总能量不大于 `m` ，红魔馆就能保存下来。蕾米莉亚想知道她最短需要毁掉的连续石块长度。

**输入**
有多组输入。
每组输入第 `1` 行为两个数字 `n`、`m(1<=n<=10^5，1<=m<=10^9)`，表示总共有 `n` 个石块，蕾米莉亚需
要将剩余总能量控制在不大于 `m` 。接着 `n` 行，每行包含一个整数 `ai` ，表示第i个石块的能量 `(1<=ai<=10^4)` 。
各组数据的 `n` 总和不超过 `10^6` 。

**输出**
每组输出一行，只包含一个整数，表示最少需要毁掉的连续石块长度。

题意：找到最少需要毁掉的连续石块长度。

思路：连续石块是区间，最少的长度说明要找到的区间和最大且区间最短。存在单调性，**连续区间越长，最大的区间和越大**。因此，这种题目可以用二分来做，找到**第一个满足题意的区间长度**(最短)即可。代码中用的是**二分长度+滑动区间**(找到该固定长度的区间中最大的区间和)，也可以用前缀和维护区间和，更简单一点。

代码：
```cpp
#include <cstdio> 
#include <algorithm>
typedef long long ll;
using namespace std;
const int MAXN = 100100;
ll a[MAXN];

int main() {
	ll n, m;
	while (~scanf("%lld%lld", &n, &m)) {
		ll sum = 0, lo = 0, hi = n;
		for (int i = 0; i < n; ++i) {
			scanf("%lld", &a[i]);
			sum += a[i];
		} 
		while (lo < hi) {
			ll mid = (lo + hi) >> 1, maxWindow = 0, nowWindow = 0;
			for (int i = 0; i < n; ++i) {
				if (i < mid) maxWindow += a[i];
				else {
					nowWindow = (i == mid) ? maxWindow : nowWindow;
					nowWindow = nowWindow + a[i] - a[i - mid]; //当前连续区间的能量 
					maxWindow = max(nowWindow, maxWindow); //最大能量区间 
				}
			}
			if (sum - maxWindow * 2 <= m) hi = mid;
			else lo = mid + 1;
		}
		printf("%lld\n", lo);
	}
	return 0;
}
```
