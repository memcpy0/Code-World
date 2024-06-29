![在这里插入图片描述](https://img-blog.csdnimg.cn/41424ffd32a244a78f96c6273e38336c.png)

---
### 解法 模拟
这道题中，开一个大数组，然后用指数索引数组中的系数元素，这种做法是不可行的，因为指数可能很大。对于这种一元稀疏多项式（按照指数递增输入每一项），我们可以将每一项的系数和指数都存到结构体数组中，然后按照操作要求，归并地进行相同指数的项之间的加或减。最后，按照题目要求输出即可：
- 多项式中首项的指数为零时，只输出系数即可；
- 多项式中，对于首项，如果系数小于零，则要输出 `-` 号；如果系数大于零，不需要输出 `+` 号。对于其他项，要么输出 `-` 号，要么输出 `+` 号。
- 多项式中，如果当前项的指数不为零，且系数是 `1` 或 `-1` ，就只输出 `x` 符号（正负号在前面输出了）；
- 多项式中，如果当前项的系数不为1，则输出 `^` 和系数值。 

```cpp
#include <bits/stdc++.h>
using namespace std;
int n, m, t, co, ex;
struct myCe {
	int coef, expo;	
} p1[1111], p2[1111];
vector<myCe> ans;
int main() {
	cin >> n >> m >> t;
	for (int i = 0; i < n; ++i) cin >> p1[i].coef >> p1[i].expo;
	for (int i = 0; i < m; ++i) cin >> p2[i].coef >> p2[i].expo;
	int f = (!t ? 1 : -1), t1 = 0, t2 = 0;
	while (t1 < n && t2 < m) {
		if (p1[t1].expo < p2[t2].expo) ans.push_back( p1[t1++] );
		else if (p1[t1].expo > p2[t2].expo) {
			ans.push_back( { f * p2[t2].coef, p2[t2].expo } );
			++t2;
		}
		else {
			int co = p1[t1].coef + f * p2[t2].coef;
			if (co) ans.push_back( { co, p1[t1].expo } );
			++t1, ++t2;
		}
	}
	while (t1 < n) ans.push_back( p1[t1++] );
	while (t2 < m) {	
		ans.push_back( { f * p2[t2].coef, p2[t2].expo} );
		++t2;
	}
	for (int i = 0; i < ans.size(); ++i) {
		if (i == 0) {
			if (ans[i].expo == 0) { 
				printf("%d", ans[i].coef);
				continue;
			} else if (ans[i].coef < 0) printf("-");
		} else {
			if (ans[i].coef > 0) printf("+");
			else printf("-");
		}
		ans[i].coef = abs(ans[i].coef);
		if (ans[i].coef != 1) printf("%d", ans[i].coef);
		cout << "x";
		if (ans[i].expo != 1)
			cout << "^" << ans[i].expo;
	}
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/d21883d1367f42d1a0767036acc620cd.png)

