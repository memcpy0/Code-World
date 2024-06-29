/*
21世纪，许多人得了一种奇怪的病：起床困难综合症，其临床表现为：起床难，起床后精神不佳。

作为一名青春阳光好少年，atm 一直坚持与起床困难综合症作斗争。

通过研究相关文献，他找到了该病的发病原因： 在深邃的太平洋海底中，出现了一条名为 drd 的巨龙，它掌握着睡眠之精髓，能随意延长大家的睡眠时间。

正是由于 drd 的活动，起床困难综合症愈演愈烈， 以惊人的速度在世界上传播。

为了彻底消灭这种病，atm 决定前往海底，消灭这条恶龙。

历经千辛万苦，atm 终于来到了 drd 所在的地方，准备与其展开艰苦卓绝的战斗。

drd 有着十分特殊的技能，他的防御战线能够使用一定的运算来改变他受到的伤害。

具体说来，drd 的防御战线由 n 扇防御门组成。

每扇防御门包括一个运算 op 和一个参数 t，其中运算一定是 OR,XOR,AND中的一种，参数则一定为非负整数。

如果还未通过防御门时攻击力为 x，则其通过这扇防御门后攻击力将变为 x op t。

最终 drd 受到的伤害为对方初始攻击力 x 依次经过所有 n 扇防御门后转变得到的攻击力。

由于 atm 水平有限，他的初始攻击力只能为 0 到 m 之间的一个整数（即他的初始攻击力只能在 0,1,…,m 中任选，但在通过防御门之后的攻击力不受 m 的限制）。

为了节省体力，他希望通过选择合适的初始攻击力使得他的攻击能让 drd 受到最大的伤害，请你帮他计算一下，他的一次攻击最多能使 drd 受到多少伤害。
输入格式

第 1 行包含 2 个整数，依次为 n,m，表示 drd 有 n 扇防御门，atm 的初始攻击力为 0 到 m 之间的整数。

接下来 n 行，依次表示每一扇防御门。每行包括一个字符串 op 和一个非负整数 t，两者由一个空格隔开，且 op 在前，t 在后，op 表示该防御门所对应的操作，t 表示对应的参数。
输出格式

输出一个整数，表示 atm 的一次攻击最多使 drd 受到多少伤害。
数据范围
最大可能达到 2<=n<=10^5, 2<=m<=19^9, 0<=t<=10^9

输入样例：

3 10
AND 5
OR 6
XOR 7

输出样例：

1

样例解释

atm可以选择的初始攻击力为 0,1,…,10。假设初始攻击力为 4，最终攻击力经过了如下计算
4 AND 5 = 4
4 OR 6 = 6
6 XOR 7 = 1
类似的，我们可以计算出初始攻击力为 1,3,5,7,9 时最终攻击力为 0，初始攻击力为 0,2,4,6,8,10 时最终攻击力为 1，因此 atm 的一次攻击最多使 drd 受到的伤害值为 1。
*/
#include <bits/stdc++.h>
using namespace std;
struct g {
	int cmd = 0, t = 0; // cmd:0->or,1->xor,2->and
};
int n, m, t;
vector<g> gs;
int calcWhen(int k, int bit) { // 当atm攻击力val的第k二进制位为bit时,通过防御门后的值
	for (int i = 0; i < n; ++i) {
		int cmd = gs[i].cmd, b = (gs[i].t >> k) & 1;
		if (cmd == 0) bit |= b;
		else if (cmd == 1) bit ^= b;
		else bit &= b;
	}
	return bit;
}
int main() {
	char cmd[5];
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; ++i) {
		scanf("%s %d", cmd, &t);
		if (cmd[0] == 'O') gs.push_back(g{0, t});
		else if (cmd[0] == 'X') gs.push_back(g{1, t});
		else gs.push_back(g{2, t});	
	}
	int val = 0, ans = 0; // val为atm的攻击力,ans为atm能造成的最大伤害
	for (int i = 30; i >= 0; --i) {	// 每次计算攻击力val第i位二进制位取1或取0,哪个使得伤害值ans最大
		int b0 = calcWhen(i, 0); // 当攻击力val第i位为0时,伤害值ans的第i位为
		int b1 = calcWhen(i, 1); // 当攻击力val第i位为0时,伤害值ans的第i位为
		// b0,b1要么为0要么为1,四种组合
		if (b0 < b1 && val + (1 << i) <= m) { // (b0,b1)=(0,1)且不超过atm的攻击力范围
			val += (1 << i), ans += (b1 << i); // 攻击力明确取1
		} else { // 特别地,(b0,b1)=(1,1)时,攻击力取该位为0,更小的攻击力更大的伤害
			ans += (b0 << i);
		}
	}
	printf("%d\n", ans); // 可能有多个攻击力使得伤害值最大
	return 0;
}
/*
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<string>
using namespace std;
int n, m;
pair<string, int> a[100005];

int calc(int bit, int now) {
	for (int i = 1; i <= n; i++) {
		int x = a[i].second >> bit & 1;
		if (a[i].first == "AND") now &= x;
		else if (a[i].first == "OR") now |= x;
		else now ^= x;
	}
	return now;
}

int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		char str[5];
		int x;
		scanf("%s%d", str, &x);
		a[i] = make_pair(str, x);
	}
	int val = 0, ans = 0;
	for (int bit = 29; bit >= 0; bit--) {
		int res0 = calc(bit, 0);
		int res1 = calc(bit, 1);
		if (val + (1 << bit) <= m && res0 < res1)
			val += 1 << bit, ans += res1 << bit;
		else
			ans += res0 << bit;
	}
	cout << ans << endl;
}
*/