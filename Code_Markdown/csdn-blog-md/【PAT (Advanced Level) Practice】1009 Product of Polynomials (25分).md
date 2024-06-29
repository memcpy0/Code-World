This time, you are supposed to find $A×B$ where $A$ and $B$ are two polynomials.
### Input Specification:

Each input file contains one test case. Each case occupies 2 lines, and each line contains the information of a polynomial:
$$K\ N_1\ a_{N_1}\ N_2\ a_{N_2}\ \dots N_K\ a_{N_k}$$
where $K$ is the number of nonzero terms in the polynomial, $N_i$​​ and  $a_{N_i}\ (i = 1, 2, \dots, K)$ are the exponents and coefficients, respectively. It is given that $1\le K \le 10,\ 0\le N_k\lt \dots \lt N_2\lt N_1\le 1000$.

### Output Specification:

For each test case you should output the product of $A$ and $B$ in one line, with the same format as the input. Notice that there must be **NO** extra space at the end of each line. Please be accurate up to 1 decimal place.

### Sample Input:

```cpp
2 1 2.4 0 3.2
2 2 1.5 1 0.5
```

### Sample Output:

```groovy
3 3 3.6 2 6.0 1 1.6
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201101164437246.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

---
### 解法1 指数作为索引
```cpp
#include <bits/stdc++.h>
using namespace std;

const int maxn = 2002;
struct polynomial {
	int highPower;
	double coefArray[maxn];
	polynomial() {
		memset(coefArray, 0.0, sizeof(coefArray));
		highPower = 0;
	}
} p1, p2, ans;
void Init(polynomial &p) {
	int n, e, maxPower = -1; double c;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d%lf", &e, &c);
		p.coefArray[e] = c;
		if (e > maxPower) maxPower = e;
	}
	p.highPower = maxPower; //得到最高次项
}
void MultiPolynomial(polynomial &p1, polynomial &p2) {
	int cnt = 0; //系数非零项的个数 
	ans.highPower = p1.highPower + p2.highPower;
	for (int i = 0; i <= p1.highPower; i++) { //相同指数的系数项相加
		for (int j = 0; j <= p2.highPower; j++) 
			ans.coefArray[i + j] += p1.coefArray[i] * p2.coefArray[j];   
	} 
    for (int i = 0; i <= ans.highPower; i++) {
        if (ans.coefArray[i] != 0.0) cnt++;
    }
	if (cnt == 0) printf("0\n"); //特判零多项式
	else {
		printf("%d", cnt);
		for (int i = ans.highPower; i >= 0; i--) {
			if (ans.coefArray[i] != 0.0) printf(" %d %.1lf", i, ans.coefArray[i]);
		} 
	} 
}
int main() {
	Init(p1); Init(p2);
	MultiPolynomial(p1, p2);
	return 0;
}
```

**20201101 Update** 把第一种方法的代码简化一下，**不存储最大指数 `highpower`**，**在读入第二个多项式的同时计算多项式乘积**：

```cpp
#include <bits/stdc++.h>
using namespace std; 
int main() {
    //p1,p2是两个多项式, ans是它们的乘积, 指数作为下标, 系数作为值, 结果指数最高为2000
    double p1[1010] = {0}, ans[2020] = {0};
    int k, e; //k几个非零项, e指数
    double c; //c系数
    scanf("%d", &k);
    while (k--) { //读入p1
        scanf("%d%lf", &e, &c);
        p1[e] = c;
    }
    scanf("%d", &k);
    while (k--) { //读入p2的同时计算结果
        scanf("%d%lf", &e, &c);
        for (int i = 0; i < 1001; ++i)
            ans[e + i] += p1[i] * c; //对某一系数经过多次操作后,可能为零
    }
    int cnt = 0; //计算多项式乘积中非零项的个数
    for (int i = 0; i < 2001; ++i) if (ans[i] != 0.0) ++cnt;
    printf("%d", cnt); //打印,末尾无多余空格,按照指数从高到低,系数保留一位小数
    for (int i = 2000; i >= 0; --i) 
        if (ans[i] != 0.0) //系数不为0才需要输出
            printf(" %d %.1lf", i, ans[i]);
    return 0;
}
```
**20201105 Update** 使用 `map` 存储键值对的同等代码如下，注意**如何按照 `key` 值从大到小输出**：
```cpp
#include <bits/stdc++.h>
using namespace std; 
int main() {
    //p1,p2是两个多项式, ans是它们的乘积, 指数作为key, 系数作为val
    map<int, double> p1, ans;
    int k, e; //k几个非零项, e指数
    double c; //c系数
    scanf("%d", &k);
    while (k--) { //读入p1
        scanf("%d%lf", &e, &c);
        p1[e] = c;
    }
    scanf("%d", &k);
    while (k--) { //读入p2的同时计算结果
        scanf("%d%lf", &e, &c);
        for (auto &it : p1)
            ans[e + it.first] += it.second * c; //对某一系数经过多次操作后,可能为零
    }
    int cnt = 0; //计算多项式乘积中非零项的个数
    for (auto &it : ans) if (it.second != 0.0) ++cnt;
    printf("%d", cnt); //打印,末尾无多余空格,按照指数从高到低,系数保留一位小数
    for (auto it = ans.rbegin(); it != ans.rend(); ++it) 
        if (it->second != 0.0) //系数不为0才需要输出
            printf(" %d %.1lf", it->first, it->second);
    return 0;
}
```

 ---
### 解法2 排序+合并同类项
这一次新写的代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std; 
struct term { int e; double c; };
vector<term> p1, p2, res, temp;
void read(vector<term> &p) {
    int k, e;
    double c;
    scanf("%d", &k);
    for (int i = 0; i < k; ++i) {
        scanf("%d%lf", &e, &c);
        p.push_back(term{e, c});
    }
}
int main() { 
    read(p1); read(p2);
    for (int i = 0; i < p1.size(); ++i) 
        for (int j = 0; j < p2.size(); ++j) 
            temp.push_back(term{p1[i].e + p2[j].e, p1[i].c * p2[j].c});
    sort(temp.begin(), temp.end(), [&](const term &a, const term &b) { return a.e > b.e; });
    res.push_back(term{temp[0].e, temp[0].c});
    for (int i = 1; i < temp.size(); ++i) {
        if (temp[i].e == res.back().e) res.back().c += temp[i].c; //合并同指数的项,系数相加
        else {
            if (res.back().c == 0.0) res.pop_back();   //如果前一部分同类项系数相加后为0,则删除
            res.push_back(term{temp[i].e, temp[i].c}); 
        }    
    }
    printf("%d", res.size());
    for (int i = 0; i < res.size(); ++i) printf(" %d %.1lf", res[i].e, res[i].c);
    return 0;
}
```
这一代码看起来简洁一点，但是用到了**排序**，而且还要**合并同指数的项**（**坑点在于，如果所有同指数的项其系数相加为零，需要删除，不能粗心大意算在非零项的个数中**，不然过不了测试点0），所以有点麻烦。
