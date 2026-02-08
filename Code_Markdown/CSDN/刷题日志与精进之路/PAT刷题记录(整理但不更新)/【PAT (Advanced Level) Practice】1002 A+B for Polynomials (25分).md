This time, you are supposed to find $A+B$ where $A$ and $B$ are two polynomials.
### Input Specification:
Each input file contains one test case. Each case occupies 2 lines, and each line contains the information of a polynomial:

$$K\ N_1\ a_{N_1}\ N_2\ a_{N_2}\dots N_k\ a_{N_k}$$

where $K$ is the number of nonzero terms in the polynomial, $N_​i$​​ and $a​_{N_​i}\ (i=1,2,⋯,K)$ are the exponents and coefficients, respectively. It is given that $1≤K≤10，0≤ N_K​​<⋯< N_2 < N_1 ≤1000$ .
### Output Specification:

For each test case you should output the sum of $A$ and $B$ in one line, with the same format as the input. Notice that there must be NO extra space at the end of each line. Please be accurate to 1 decimal place.
### Sample Input:

```clike
2 1 2.4 0 3.2
2 2 1.5 1 0.5
```

### Sample Output:

```clike
3 2 1.5 1 2.9 0 3.2
```

```cpp
#include <bits/stdc++.h>
using namespace std;

const int maxn = 1002;
struct polynomial {
	int highPower;
	double coefArray[1002];
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
	p.highPower = maxPower;
}
void AddPolynomial(polynomial &p1, polynomial &p2) {
	int cnt = 0; //系数非零项的个数 
	ans.highPower = max(p1.highPower, p2.highPower);
	for (int i = ans.highPower; i >= 0; i--) {
		ans.coefArray[i] = p1.coefArray[i] + p2.coefArray[i];
		if (ans.coefArray[i] != 0.0) cnt++;
	} 
	if (cnt == 0) printf("0\n");
	else {
		printf("%d", cnt);
		for (int i = ans.highPower; i >= 0; i--) {
			if (ans.coefArray[i] != 0.0) printf(" %d %.1lf", i, ans.coefArray[i]);
		} 
	} 
}
int main() {
	int c, e, maxPower = -1;
	Init(p1); Init(p2);
	AddPolynomial(p1, p2);
	return 0;
}
```
```cpp
#include <bits/stdc++.h>
using namespace std;
double eps = 1e-8;
void read(double a[]) {
    int k, e;
    double c;
    scanf("%d", &k);
    for (int i = 0; i < k; ++i) {
        scanf("%d%lf", &e, &c);
        a[e] = c;
    }
}
bool equalZero(double a) { return abs(a) < eps; }
int main() {
    double a[1010], b[1010], c[1010] = {0}; //指数
    int nonZeroCoff = 0;
    read(a); read(b);
    for (int i = 1000; i >= 0; --i) {
        c[i] = a[i] + b[i]; //系数相加,指数相等
        if (equalZero(c[i])) continue; //系数为0
        ++nonZeroCoff;  //系数非零的项数
    }
    if (nonZeroCoff == 0) { printf("0"); return 0; }
    printf("%d", nonZeroCoff); //打印非零项
    for (int i = 1000; i >= 0; --i) {
        if (equalZero(c[i])) continue; 
        printf(" %d %.1lf", i, c[i]); 
    }
    return 0;
}
```
