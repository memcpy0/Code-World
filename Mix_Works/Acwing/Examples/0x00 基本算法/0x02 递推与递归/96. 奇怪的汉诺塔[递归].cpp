/*
汉诺塔问题，条件如下：

1、这里有 A、B、C 和 D 四座塔。
2、这里有 n 个圆盘，n 的数量是恒定的。
3、每个圆盘的尺寸都不相同。
4、所有的圆盘在开始时都堆叠在塔 A 上，且圆盘尺寸从塔顶到塔底逐渐增大。

5、我们需要将所有的圆盘都从塔 A 转移到塔 D 上。

6、每次可以移动一个圆盘，当塔为空塔或者塔顶圆盘尺寸大于被移动圆盘时，可将圆盘移至这座塔上。

请你求出将所有圆盘从塔 A 移动到塔 D，所需的最小移动次数是多少。

河内塔.jpg
汉诺塔塔参考模型

输入格式
没有输入

输出格式
对于每一个整数 n ，输出一个满足条件的最小移动次数，每个结果占一行。

数据范围
1≤n≤12

输入样例：
没有输入

输出样例：
参考输出格式
*/
#include <bits/stdc++.h>
using namespace std;
const int maxn = 13, inf = 0x3f3f3f3f;
int n;
vector<int> dp4(maxn, inf); // dp4[i]表示将i个圆盘在四塔模式下、从塔A移动到另一座塔需要的最小移动次数
vector<int> dp3(maxn, inf); // dp3[i]表示将i个圆盘在三塔模式下、从塔A移动到另一座塔需要的最小移动次数
void calc() {
	dp4[0] = 0, dp4[1] = 1;
	dp3[0] = 0, dp3[1] = 1;
	for (int i = 2; i < maxn; ++i) dp3[i] = 2 * dp3[i - 1] + 1;
	for (int i = 2; i < maxn; ++i)
		for (int j = 1; j < i; ++j)
			dp4[i] = min(dp4[i], 2 * dp4[j] + dp3[i - j]);
	for (int i = 1; i < maxn; ++i) cout << dp4[i] << endl;
}
int main() {
	calc();
	return 0;
}