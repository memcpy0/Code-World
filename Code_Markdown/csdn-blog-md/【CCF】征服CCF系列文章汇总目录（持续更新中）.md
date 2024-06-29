> 本文属于「征服CCF」系列文章的汇总目录，所刷题目主要集中于「CCF计算机职业资格认证考试」历年试题。在这一系列刷题文章中，将讲解多种解题思路及其优化，涉及到通用解法时还会归纳总结相应的算法模板。由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏本文以作备忘。


题目和解答如下所示：
| 题目名称 | 解题文章 |
|:----|:---
| [202104-1	灰度直方图](http://118.190.20.162/view.page?gpid=T128)  | [CCF 202104-1 灰度直方图【哈希计数】](https://memcpy0.blog.csdn.net/article/details/120028857)
| [202104-2	邻域均值](http://118.190.20.162/view.page?gpid=T127) | [CCF 202104-2 邻域均值【二维前缀和】](https://memcpy0.blog.csdn.net/article/details/120028990)
| [202104-3	DHCP服务器		
| [202104-4	校门外的树		
| [202104-5	疫苗运输		

---
| [202012-1	期末预测之安全指数		
| [202012-2	期末预测之最佳阈值		
| [202012-3	带配额的文件系统		
| [202012-4	食材运输		
| [202012-5	星际旅行		

---

| [202009-1	称检测点查询](http://118.190.20.162/view.page?gpid=T113)  | 

```cpp
#include <bits/stdc++.h>
using namespace std;
int n, x, y, a, b;
int dist[3] = {INT_MAX, INT_MAX, INT_MAX}, ans[3];
void helper(int i) {
	dist[i] = dist[i - 1];
	ans[i] = ans[i - 1];
}
void assign(int i, int d, int id) {
	dist[i] = d;
	ans[i] = id;
}
int main() {
	scanf("%d%d%d", &n, &x, &y);
	for (int i = 0; i < n; ++i) {
		scanf("%d%d", &a, &b);
		int d = (x - a) * (x - a) + (y - b) * (y - b);
		if (d < dist[0]) {
			helper(2);
			helper(1);
			assign(0, d, i + 1);
		} else if (d < dist[1]) {
			helper(2);
			assign(1, d, i + 1);
		} else if (d < dist[2]) {
			assign(2, d, i + 1);
		}
	}
	printf("%d\n%d\n%d\n", ans[0], ans[1], ans[2]);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/a17cac2758a248e19001ff4b6e9d9eb0.png)

| [202009-2	风险人群筛查		
| [202009-3	点亮数字人生		
| [202009-4	星际旅行		
| [202009-5	密信与计数		

---
| [202006-1	线性分类器		
| [202006-2	稀疏向量		
| [202006-3	Markdown渲染器		
| [202006-4	1246		
| [202006-5	乔乔和牛牛逛超市		

---
| [201912-1	报数](http://118.190.20.162/view.page?gpid=T100)

```cpp
#include <bits/stdc++.h>
using namespace std;
int n, a[4], idx = 0, cur = 1;
bool judge(int num) {
	if (num % 7 == 0) return false;
	while (num) {
		if (num % 10 == 7) return false;
		num /= 10;
	}
	return true;
}
int main() {
	scanf("%d", &n);
	while (n) {
		for (int i = 0; i < 4; ++i, ++cur) {
			if (judge(cur) == false) ++a[i];
			else {
				--n;
				if (!n) break;
			}
		}
	}
	printf("%d\n%d\n%d\n%d\n", a[0], a[1], a[2], a[3]);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/15bd4eed3d524b7a9043c2a9dcea3f3d.png)

| [201912-2	回收站选址		
| [201912-3	化学方程式		
| [201912-4	区块链		
| [201912-5	魔数		

---
| [201909-1	小明种苹果](http://118.190.20.162/view.page?gpid=T94) | 

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1010; //minust[]记录疏果的个数
int n, m, a[maxn], t, minust[maxn], sum, idx;
int main() {
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; ++i) {
		scanf("%d", &a[i]);
		for (int j = 0; j < m; ++j) {
			scanf("%d", &t);
			minust[i] += t;
		}
		a[i] += minust[i];
		sum += a[i]; //疏果后总苹果数
		minust[i] = -minust[i];
		if (minust[i] > minust[idx]) idx = i;
	}
	printf("%d %d %d\n", sum, idx, minust[idx]);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/29413f9864f842338e5c37ea32f3d3d0.png)

| [201909-2	小明种苹果（续）		
| [201909-3	字符画		
| [201909-4	推荐系统		
| [201909-5	城市规划		

---
| [201903-1	小中大		
| [201903-2	二十四点		
| [201903-3	损坏的RAID5		
| [201903-4	消息传递接口		
| [201903-5	317号子任务		

---

| [201812-1	小明上学](http://118.190.20.162/view.page?gpid=T80) | 

```cpp
#include <bits/stdc++.h>
using namespace std;
int n, r, y, g, k, t, ans = 0;
int main() {
	scanf("%d%d%d%d", &r, &y, &g, &n);
	for (int i = 0; i < n; ++i) {
		scanf("%d%d", &k, &t);
		if (k == 0) ans += t;
		else if (k == 1) ans += t;
		else if (k == 2) ans += t + r;
		else if (k == 3) continue;
	}
	printf("%d", ans);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/9c6285a7df5d4da1bc399abd65ba5ef4.png)


| [201812-2	小明放学]
| [201812-3	CIDR合并	
| [201812-4	数据中心		
| [201812-5	管道清洁			

---
| [201809-1	卖菜](http://118.190.20.162/view.page?gpid=T79) | 

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1010;
int n, a[maxn], t;
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) scanf("%d", &a[i]);
	for (int i = 0; i < n; ++i) {
		if (i == 0) t = (a[0] + a[1]) / 2;
		else if (i == n - 1) t = (a[n - 2] + a[n - 1]) / 2;
		else t = (a[i - 1] + a[i + 1] + a[i]) / 3;
		printf(" %d" + !i, t);
	}
	return 0;
}
```

| [201809-2	买菜		
| [201809-3	元素选择器		
| [201809-4	再卖菜		
| [201809-5	线性递推式		

---
| [201803-1	跳一跳](http://118.190.20.162/view.page?gpid=T73)

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 33;
int a, ans = 0, pre, i = 0;
int main() {
	while (~scanf("%d", &a)) {
		if (a == 0) break;
		if (a == 1) { ++ans; pre = 1; }
		if (a == 2) {
			if (i == 0 || pre == 1) { ans += 2; pre = 2; }
			else {	ans += pre + 2; pre += 2; }
		}
		++i;
	}
	printf("%d", ans);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/4bf9db0c7de1470685062d2443e27d9c.png)


		
| [201803-2	碰撞的小球		
| [201803-3	URL映射		
| [201803-4	棋局评估		
| [201803-5	二次求和		

---
| [201712-1	最小差值](http://118.190.20.162/view.page?gpid=T68)

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 10010;
int n, a[maxn], ans = INT_MAX;
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) scanf("%d", &a[i]);
	sort(a, a + n);
	for (int i = 0; i + 1 < n; ++i)
		ans = min(ans, a[i + 1] - a[i]);
	printf("%d", ans);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/7b2a3f1dbca74327b2a9737bc9bae242.png)

| [201712-2	游戏		
| [201712-3	Crontab		
| [201712-4	行车路线		
| [201712-5	商路		

---
| [201709-1	打酱油](http://118.190.20.162/view.page?gpid=T63) |

```cpp
#include <bits/stdc++.h>
using namespace std;
int n, ans;
int main() {
	scanf("%d", &n);
	int bottles = n / 10; //能买多少瓶
	for (int i = 0; i <= bottles; i += 3) {
		int b3 = i / 3, b5 = (bottles - i) / 5 * 2;
		ans = max(ans, bottles + b3 + b5);
	}
	printf("%d", ans);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/66a9f04a1e714f18a43b9048499a1ac1.png)

| [201709-2	公共钥匙盒		
| [201709-3	JSON查询		
| [201709-4	通信网络		
| [201709-5	除法		

---
| [201703-1	分蛋糕](http://118.190.20.162/view.page?gpid=T57) | 
  
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1010;
int n, a[maxn], k, ans = 0;
int main() {
	scanf("%d%d", &n, &k);
	for (int i = 0; i < n; ++i) scanf("%d", &a[i]);
	int sum = 0;
	for (int i = 0; i < n; ++i) {
		sum += a[i];
		if (sum < k) continue;
		++ans;
		sum = 0;
	}
	if (sum) ++ans;
	printf("%d", ans);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/e77d2703812345d287bf20b58df39f9d.png)

| [201703-2	学生排队		
| [201703-3	Markdown		
| [201703-4	地铁修建		
| [201703-5	引水入城		

---
| [201612-1	中间数](http://118.190.20.162/view.page?gpid=T52) |

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1010;
int n, t, a[maxn];
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) {
		scanf("%d", &t);
		++a[t];
	}
	int sum = 0;
	for (int i = 0; i < maxn; ++i) {
		if (!a[i]) continue; //不存在此数
		if (sum == n - sum - a[i]) { //是中位数
			printf("%d\n", i);
			return 0;
		}
		sum += a[i];
	}
	printf("-1\n");
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/acaaa6f90001484cb2284a12cd020b9f.png)

| [201612-2	工资计算		
| [201612-3	权限查询		
| [201612-4	压缩编码		
| [201612-5	卡牌游戏		

---
| [201609-1	最大波动](http://118.190.20.162/view.page?gpid=T47) |

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1010;
int n, pre, cur, ans;
int main() {
	scanf("%d%d", &n, &pre);
	for (int i = 1; i < n; ++i) {
		scanf("%d", &cur);
		ans = max(ans, abs(cur - pre));
		pre = cur;
	}
	printf("%d", ans);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/7e8841c4269548d68607f80daa182a88.png)

| [201609-2	火车购票](http://118.190.20.162/view.page?gpid=T46)  |  [CCF 201609-2 火车购票【数组/模拟】](https://memcpy0.blog.csdn.net/article/details/122063501)
| [201609-3	炉石传说		
| [201609-4	交通规划		
| [201609-5	祭坛		

---
| [201604-1	折点计数](http://118.190.20.162/view.page?gpid=T42)

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1010;
int n, a[maxn], ans;
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) scanf("%d", &a[i]);
	for (int i = 1; i + 1 < n; ++i) 
		if ((a[i] > a[i - 1] && a[i] > a[i + 1]) || (a[i] < a[i - 1] && a[i] < a[i + 1]))
			++ans;
	printf("%d", ans);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/f5cbb9494e1a469386230b14d243fef8.png)

		
| [201604-2	俄罗斯方块		
| [201604-3	路径解析		
| [201604-4	游戏		
| [201604-5	网络连接		

---
| [201512-1	数位之和](http://118.190.20.162/view.page?gpid=T37)

```cpp
#include <bits/stdc++.h>
using namespace std;
int main() {
	char s[15];
	int ans = 0;
	scanf("%s", s);
	for (int i = 0; s[i]; ++i) ans += (s[i] - '0');
	printf("%d", ans);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/5fbffa9d6b544f48905640ef58c5f08e.png)
| [201512-2	消除类游戏		
| [201512-3	画图		
| [201512-4	送货		
| [201512-5	矩阵		

---
| [201509-1	数列分段](http://118.190.20.162/view.page?gpid=T32)

```cpp
#include <bits/stdc++.h>
using namespace std;
int n, pre, cur, ans = 1;
int main() {
	scanf("%d%d", &n, &pre);
	for (int i = 1; i < n; ++i) {
		scanf("%d", &cur);
		if (cur != pre) ++ans;
		pre = cur;
	}
	printf("%d", ans);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20af7acd13084712b99cc9cd902b3df8.png)

| [201509-2	日期计算]()

	
| [201509-3	模板生成系统		
| [201509-4	高速公路		
| [201509-5	最佳文章		

---

| [201503-1	图像旋转](http://118.190.20.162/view.page?gpid=T27) |

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1010;
int n, m, a[maxn][maxn];
int main() {
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			scanf("%d", &a[i][j]);
	for (int i = m - 1; i >= 0; --i) {
		for (int j = 0; j < n; ++j)
			printf(" %d" + !j, a[j][i]);
		printf("\n");
	}
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/52b44edd3a0b494ea4b52f94f980d813.png)

| [201503-2	数字排序](http://118.190.20.162/view.page?gpid=T26) | 

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1010;
int n, t, a[maxn];
struct node {
	int val = 0, cnt = 0;
	node(int v, int c) : val(v), cnt(c) {	}
};
bool cmp(const node &a, const node &b) {
	return a.cnt != b.cnt ? a.cnt > b.cnt : a.val < b.val;
}
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) {
		scanf("%d", &t);
		++a[t];
	}
	vector<node> ans;
	for (int i = 0; i < maxn; ++i) {
		if (a[i])
			ans.push_back(node(i, a[i])); //不知原因,写node{i,a[i]}或{i,a[i]}线上会编译失败
	}
	sort(ans.begin(), ans.end(), cmp);
	for (int i = 0; i < ans.size(); ++i) printf("%d %d\n", ans[i].val, ans[i].cnt);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/7556ff36e53c41a0b1e92011758d0800.png)

| [201503-3	节日		
| [201503-4	网络延时		
| [201503-5	最小花费		


---
| [201412-1	门禁系统](http://118.190.20.162/view.page?gpid=T21) | 

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1010;
int n, t, cnt[maxn];
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) {
		scanf("%d", &t);
		++cnt[t];
		printf(" %d" + !i, cnt[t]);
	}
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/4f4659c049a44a90b2aa57575f935b77.png)

	
| [201412-2	Z字形扫描](http://118.190.20.162/view.page?gpid=T20) | 

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 510;
int n, a[maxn][maxn];
bool canGo(int i, int j) {
	return (i >= 0 && i < n && j >= 0 && j < n);
}
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			scanf("%d", &a[i][j]);
	if (n == 1) { printf("%d\n", a[0][0]); return 0; }
	int i = 0, j = 0;
	vector<int> ans;
	ans.push_back(a[0][0]);
	while (true) {
		if (canGo(i, j + 1)) ++j; //右移一步
		else if (canGo(i + 1, j)) ++i; //下移一步
		else break; //不能移动,到达终点
		ans.push_back(a[i][j]); //每走一步记录一格
		while (canGo(i + 1, j - 1)) {
			++i, --j;
			ans.push_back(a[i][j]);
		}
		if (canGo(i + 1, j)) ++i; //下移一步
		else if (canGo(i, j + 1)) ++j; //右移一步
		else break; //不能移动,到达终点
		ans.push_back(a[i][j]);
		while (canGo(i - 1, j + 1)) {
			--i, ++j;
			ans.push_back(a[i][j]);
		}
	}
	for (int i = 0; i < ans.size(); ++i) printf(" %d" + !i, ans[i]);
	return 0;
}
```
	
| [201412-3	集合竞价		
| [201412-4	最优灌溉		
| [201412-5	货物调度		

---
| [201409-1	相邻数对](http://118.190.20.162/view.page?gpid=T16) | 

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1010;
int n, a[maxn], ans = 0;
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) scanf("%d", &a[i]);
	for (int i = 0; i < n; ++i) 
		for (int j = i + 1; j < n; ++j) 
			if (abs(a[i] - a[j]) == 1) ++ans;
	printf("%d\n", ans);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/8f6b196a6be64c7cacd4b4f9cec3cbf8.png)
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 10010;
bool a[maxn];
int n, t, ans = 0;
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) { //n个不同的整数
		scanf("%d", &t);
		a[t] = true;
	}
	for (int i = 1; i < maxn; ++i)
		if (a[i] && a[i - 1]) ++ans;
	printf("%d\n", ans);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/3cb0fd0da0a342daa82a74df64fed833.png)

| [201409-2	画图]() | 

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 110;
int n, a1, b1, a2, b2, ans = 0;
bool a[maxn][maxn] = {false};
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) {
		scanf("%d%d%d%d", &a1, &b1, &a2, &b2);
		for (int j = a1; j < a2; ++j)
			for (int k = b1; k < b2; ++k)
				a[j][k] = true;
	}
	for (int i = 0; i < maxn; ++i)
		for (int j = 0; j < maxn; ++j)
			if (a[i][j]) ++ans;
	printf("%d\n", ans);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/f37174007b7a49eba7897c83aba6cd29.png)
二维差分：
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 110;
int n, a1, b1, a2, b2, ans = 0;
int a[maxn][maxn];
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) {
		scanf("%d%d%d%d", &a1, &b1, &a2, &b2);
		//二维差分,整体向上向右偏移一格
		++a[a1 + 1][b1 + 1], ++a[a2 + 1][b2 + 1];
		--a[a1 + 1][b2 + 1], --a[a2 + 1][b1 + 1];
	}
	for (int i = 1; i < maxn; ++i)
		for (int j = 1; j < maxn; ++j)
			a[i][j] += a[i][j - 1] + a[i - 1][j] - a[i - 1][j - 1];
	for (int i = 1; i < maxn; ++i)
		for (int j = 1; j < maxn; ++j)
			if (a[i][j]) ++ans;
	printf("%d\n", ans);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/7c6cf57cfdca42c197bf5dd900a1fa0e.png)



| [201409-3	字符串匹配]
| [201409-4	最优配餐]
| [201409-5	拼图]



---
| [201403-1	相反数](http://118.190.20.162/view.page?gpid=T10) | 

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1010;
int n, t, a[maxn], ans = 0;
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) {
		scanf("%d", &t); //所有t互不相同
		++a[abs(t)];
	}
	for (int i = 1; i < maxn; ++i)
		if (a[i] == 2) ++ans;
	printf("%d", ans);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/aabef39c5d714c88b15f890b22e6cbfd.png)
| [201403-2	窗口](http://118.190.20.162/view.page?gpid=T9) |

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 130;
int n, m, a1, b1, a2, b2, a, b;
struct node {
	int id, x1, y1, x2, y2;
};
vector<node> wins;
int main() {
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; ++i) {
		scanf("%d%d%d%d", &a1, &b1, &a2, &b2);
		wins.push_back(node{i + 1, a1, b1, a2, b2});
	}
	for (int i = 0; i < m; ++i) {
		scanf("%d%d", &a, &b);
		bool ignored = true;
		int j;
		for (j = n - 1; j >= 0; --j) {
			if (a >= wins[j].x1 && a <= wins[j].x2 && b >= wins[j].y1 && b <= wins[j].y2) {
				ignored = false;
				break;
			}
		}
		if (ignored) { printf("IGNORED\n"); continue; }
		node t = wins[j];
		printf("%d\n", t.id);
		for (int k = j; k + 1 < n; ++k) wins[k] = wins[k + 1];
		wins.pop_back();
		wins.push_back(t);
	}
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/bdd9417d548d47f1adbb39965437deda.png)


| [201403-3	命令行选项		
| [201403-4	无线网络		
| [201403-5	任务调度		

---
| [201312-1 出现次数最多的数](http://118.190.20.162/view.page?gpid=T5)  | []		

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 10010;
int n, t, a[maxn];
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) {
		scanf("%d", &t);
		++a[t];
	}
	int mxi = 0;
	for (int i = 1; i < maxn; ++i) 
		if (a[i] > a[mxi]) mxi = i;
	printf("%d\n", mxi);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/32ccfd541a4a431eba421bf1c565286b.png)


| [201312-2	ISBN号码](http://118.190.20.162/view.page?gpid=T4)	| 

```cpp
#include <bits/stdc++.h>
using namespace std;
char a, b, c, d, e, f, g, h, i, j;
//a-bcd-efghi-j
#define to(ch) ((ch) - '0')
int main() {
	scanf("%c-%c%c%c-%c%c%c%c%c-%c", &a, &b, &c, &d, &e, &f, &g, &h, &i, &j);
	int t = to(a) * 1 + to(b) * 2 + to(c) * 3 +
			to(d) * 4 + to(e) * 5 + to(f) * 6 + 
			to(g) * 7 + to(h) * 8 + to(i) * 9;
	char tch = (t % 11 < 10) ? (t % 11 + '0') : 'X';
	if (tch == j) printf("Right");
	else printf("%c-%c%c%c-%c%c%c%c%c-%c", a, b, c, d, e, f, g, h , i, tch); 
	return 0;
}
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/c2b5a0a187d24affb73b72eb7a34ad0b.png)

| [201312-3	最大的矩形]() | 

```cpp
#include <bits/stdc++.h>
using namespace std;
int n, ans = 0, height;
stack<pair<int, int>> st; //width,height
int main() {
	scanf("%d", &n);
	for (int i = 0; i <= n; ++i) {
		if (i < n) scanf("%d", &height);
		else height = 0;
		int totWidth = 0;
		while (!st.empty() && st.top().second > height) {
			int w = st.top().first;
			int h = st.top().second;
			st.pop();
			totWidth += w;
			ans = max(ans, h * totWidth);
		}
		st.push(pair<int, int>{totWidth + 1, height});
	}
	printf("%d\n", ans);
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/8376e36270ed4a9ba7d5091b92a5babb.png)


| [201312-4	有趣的数		
| [201312-5	I’m stuck!






