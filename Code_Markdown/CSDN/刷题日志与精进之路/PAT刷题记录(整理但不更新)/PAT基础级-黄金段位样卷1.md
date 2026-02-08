@[toc]

# 7-1 装睡 (10 分)

你永远叫不醒一个装睡的人 —— 但是通过分析一个人的呼吸频率和脉搏，你可以发现谁在装睡！医生告诉我们，正常人睡眠时的呼吸频率是每分钟15-20次，脉搏是每分钟50-70次。下面给定一系列人的呼吸频率与脉搏，请你找出他们中间有可能在装睡的人，即至少一项指标不在正常范围内的人。

**输入格式：** 
输入在第一行给出一个正整数 N（≤10）。随后 N 行，每行给出一个人的名字（仅由英文字母组成的、长度不超过3个字符的串）、其呼吸频率和脉搏（均为不超过100的正整数）。

**输出格式：**
按照输入顺序检查每个人，如果其至少一项指标不在正常范围内，则输出其名字，每个名字占一行。

**输入样例：**
```cpp
4
Amy 15 70
Tom 14 60
Joe 18 50
Zoe 21 71
```
**输出样例：**

```cpp
Tom
Zoe
```
代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
int n, a, b;
string s;
int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        cin >> s >> a >> b;
        if (!(a >= 15 && a <= 20) || !(b >= 50 && b <= 70)) cout << s << endl;
    }
    return 0;
}
```
# 7-2 电子汪 (10 分)
据说汪星人的智商能达到人类 4 岁儿童的水平，更有些聪明汪会做加法计算。比如你在地上放两堆小球，分别有 1 只球和 2 只球，聪明汪就会用“汪！汪！汪！”表示 1 加 2 的结果是 3。

本题要求你为电子宠物汪做一个模拟程序，根据电子眼识别出的两堆小球的个数，计算出和，并且用汪星人的叫声给出答案。

**输入格式：**
输入在一行中给出两个 [1, 9] 区间内的正整数 A 和 B，用空格分隔。

**输出格式：** 
在一行中输出 A + B 个 `Wang!` 。

**输入样例：**

```cpp
2 1
```

**输出样例：**

```cpp
Wang!Wang!Wang!
```
代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
int a, b;
int main() {
    scanf("%d%d", &a, &b);
    for (int i = a + b; i > 0; --i) cout << "Wang!";
    return 0;
}
```
# 7-3 到底是不是太胖了 (10 分)

据说一个人的标准体重应该是其身高（单位：厘米）减去100、再乘以0.9所得到的公斤数。真实体重与标准体重误差在10%以内都是完美身材（即 | 真实体重 − 标准体重 | < 标准体重×10%）。已知市斤是公斤的两倍。现给定一群人的身高和实际体重，请你告诉他们是否太胖或太瘦了。

**输入格式：**

输入第一行给出一个正整数N（≤ 20）。随后N行，每行给出两个整数，分别是一个人的身高H（120 < H < 200；单位：厘米）和真实体重W（50 < W ≤ 300；单位：市斤），其间以空格分隔。

**输出格式：**
为每个人输出一行结论：如果是完美身材，输出 `You are wan mei!` ；如果太胖了，输出 `You are tai pang le!` ；否则输出 `You are tai shou le!` 。

**输入样例：**

```cpp
3
169 136
150 81
178 155
```

**输出样例：**

```cpp
You are wan mei!
You are tai shou le!
You are tai pang le!
```
代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
int n;
double h, w;
int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%lf%lf", &h, &w);
        double p = (h - 100) * 0.9; //公斤,市斤
        if (abs(w / 2.0 - p) * 10 < p) cout << "You are wan mei!\n";
        else if (w > p * 2) cout << "You are tai pang le!\n";
        else cout << "You are tai shou le!\n";
    }
    return 0;
}
```
# 7-4 到底有多二 (15 分)

一个整数“**犯二的程度**”定义为该数字中包含2的个数与其位数的比值。如果这个数是负数，则程度增加0.5倍；如果还是个偶数，则再增加1倍。例如数字 `-13142223336` 是个11位数，其中有3个2，并且是负数，也是偶数，则它的犯二程度计算为：$3/11×1.5×2×100\%$ ，约为81.82%。本题就请你计算一个给定整数到底有多二。

**输入格式：** 
输入第一行给出一个不超过50位的整数 `N` 。

**输出格式：**
在一行中输出 `N` 犯二的程度，保留小数点后两位。

**输入样例：**

```cpp
-13142223336
```

**输出样例：**

```cpp
81.82%
```
这道题倒是出得有点意思。代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
char s[52];
int main() {
    scanf("%s", s);
    double t = 1.0;
    int n2 = 0, n = strlen(s), flag = 0; //2的个数,位数
    for (int i = 0; s[i]; ++i) {
        if (s[i] == '-') { t *= 1.5; flag = 1; }
        else if (s[i] == '2') ++n2;
    }
    if ((s[n - 1] - '0') % 2 == 0) t *= 2; //是偶数
    printf("%.2lf%%", t * n2 / (n - flag) * 100); //负数的话位数减一
    return 0;
}
```
# 7-5 一帮一 (15 分)
“一帮一学习小组”是中小学中常见的学习组织方式，老师把学习成绩靠前的学生跟学习成绩靠后的学生排在一组。本题就请你编写程序帮助老师自动完成这个分配工作，即在得到全班学生的排名后，在当前尚未分组的学生中，将名次最靠前的学生与名次最靠后的**异性**学生分为一组。

**输入格式：**
输入第一行给出正偶数N（≤50），即全班学生的人数。此后N行，按照名次从高到低的顺序给出每个学生的性别（0代表女生，1代表男生）和姓名（不超过8个英文字母的非空字符串），其间以1个空格分隔。这里保证本班男女比例是1:1，并且没有并列名次。

**输出格式：**
每行输出一组两个学生的姓名，其间以1个空格分隔。名次高的学生在前，名次低的学生在后。小组的输出顺序按照前面学生的名次从高到低排列。

**输入样例：**
```cpp
8
0 Amy
1 Tom
1 Bill
0 Cindy
0 Maya
1 John
1 Jack
0 Linda
```

**输出样例：**

```cpp
Amy Jack
Tom Linda
Bill Maya
Cindy John
```
模拟题，代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
using gn = pair<int, string>; //gender,name
int n, g;
string name;
vector<gn> stu;
int main() { 
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        cin >> g >> name;
        stu.push_back({g, name});
    } //双指针
    int f = n - 1, m = n - 1;
    for (int i = 0; i < n && (i < f || i < m); ++i) { //男和女,女和男都匹配完成
        if (stu[i].first == 0) { //高名次女性
            while (m > i && stu[m].first != 1) --m;
            cout << stu[i].second << " " << stu[m--].second << endl;
        } else {
            while (f > i && stu[f].first != 0) --f;
            cout << stu[i].second << " " << stu[f--].second << endl;
        }
    }
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210531005548453.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

