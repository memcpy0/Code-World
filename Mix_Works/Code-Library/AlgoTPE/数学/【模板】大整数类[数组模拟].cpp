 #include <iostream>
#include <cstring>
using namespace std;
struct bign {
	int d[10000];
	int len;
	bign() {
		memset(d, 0, sizeof(d));
		len = 0;
	}
	bign(char str[]) { //以字符串初始化大整数类 
		len = strlen(str);  
		for (int i = 0; i < len; ++i) 
			d[i] = str[len - i - 1] - '0'; //逆着赋值 
	}
	int compare(const bign& b) { //比较和b的大小, 大于b、相等、小于b分别返回1、0、-1 
		if (len > b.len) return 1; //大于b
		else if (len < b.len) return -1; //小于b
		else {
			for (int i = len - 1; i >= 0; --i) { //从高位到低位比较
				if (d[i] > b.d[i]) return 1; //只要有1位比b大, 则大于b 
				else if (d[i] < b.d[i]) return -1; //只要有1位比b小, 则小于b 
			}
			return 0;
		}
	}
	void print() {
		for (int i = len - 1; i >= 0; --i)
			printf("%d", d[i]);	
	}
};
//高精度加法 
bign add(const bign& a, const bign& b) { //a+b 
	bign c;
	int carry = 0; //进位
	for (int i = 0; i < a.len || i < b.len; ++i) { //以较长的为界限
		int t = a.d[i] + b.d[i] + carry; //两个对应位 与进位相加
		c.d[c.len++] = t % 10; //个位数为该位结果
		carry = t / 10; //十位数为新的进位
	}
	if (carry != 0) //如果最后进位不为0, 则直接赋给结果的最高位 
		c.d[c.len++] = carry;
	return c;  
}
//高精度减法
bign sub(bign a, const bign& b) { //a-b 避免修改a,传递结构体而不使用引用 
	bign c;
	for (int i = 0; i < a.len || i < b.len; ++i) { //以较长的为界限
		if (a.d[i] < b.d[i]) {
			--a.d[i + 1]; //向高位借1
			a.d[i] += 10; //当前为+10 
		}
		c.d[c.len++] = a.d[i] - b.d[i]; //减法结果为当前位结果 
	} //修改了a, 但是没有修改a.len, 可能加了0 
	while (c.len - 1 >= 1 && c.d[c.len - 1] == 0) //去除高位的0, 但保证至少有一位最低位 
		--c.len;
	return c; 
}
//高精度和低精度的乘法
bign mul(const bign& a, int b) { //a*b bign*int
	bign c;
	int carry = 0; //进位 
	for (int i = 0; i < a.len; ++i) {
		int t = a.d[i] * b + carry;
		c.d[c.len++] = t % 10; //个位作为该位结果 
		carry = t / 10; //高位部分作为新的进位 
	}
	while (carry) { //这样执行乘法, 进位可能不止一位 
		c.d[c.len++] = carry % 10;
		carry /= 10;
	}
	return c;
}  
//高精度乘法 
bign superMul(const bign &a, const bign& b) { //a*b bign*bign
	bign c;
	for (int i = 0; i < b.len; ++i) {
		int carry = 0; //进位
		for (int j = 0; j < a.len; ++j) { //模拟乘法竖式，加上进位、错位
			int t = b.d[i] * a.d[j] + carry + c.d[i + j]; 
			c.d[i + j] = t % 10;
			carry = t / 10; //高位部分为新的进位 
		}
		if (carry) //b的每一位乘以a的最高位的数的进位 
			c.d[a.len + i] = carry % 10; 
	}  
	//两个非零因数相乘，积的位数要么是两个因数位数之和，要么是两个因数位数之和-1 
	c.len = a.len + b.len; 
	//检查最高位是否为0，如果为0，退一位
	//while循环可以保证即使因数是0也能输出正确的结果而不输出多余前导0 
	while (c.d[c.len - 1] == 0 && c.len - 1 >= 1) 
		--c.len;
	return c;
} 


/**
123456789050987654321 7
17636684150141093474 3
1 5
0 1
*/
//高精度与低精度除法, 余数也是低精度的 
bign div(const bign &a, int b, int &r) { //a/b 会修改a, 因此传递结构体而不使用引用
	bign c;
	c.len = a.len; //被除数和商的每一位都是一一对应的，因此先令长度相等
	for (int i = a.len - 1; i >= 0; --i) { //从高位开始
		r = r * 10 + a.d[i]; //和上一位遗留的余数相组合
		if (r < b) c.d[i] = 0; //不够除, 该位为0
		else { //够除
			c.d[i] = r / b; //商
			r = r % b; //获得新的余数
		}
	}
	while (c.len - 1 >= 1 && c.d[c.len - 1] == 0)
		--c.len; //去除高位的0，同时至少保留一位最低位 
	return c; 
}

const int maxn = 1010;
char s[maxn];
int b;
vector<int> q;
int main() {
    scanf("%s%d", s, &b);
    int slen = strlen(s), carry = 0;
    for (int i = 0; i < slen; ) {
    	int t = i;
        while (i < slen && carry < b) {
            carry = carry * 10 + (s[i++] - '0');
            if (i - t >= 2) q.push_back(0);
        }
        q.push_back(carry / b);
        carry %= b;
    }
    int i = 0, n = q.size();
    bool allZero = true;92
    for (; i < n; ++i) {
    	if (q[i] != 0) {
    		allZero = false;
			break;
		}
    }
	if (allZero) printf("0 %d", carry);
    else {
		for (; i < n; ++i) printf("%d", q[i]);
    	printf(" %d", carry);
    }
    return 0;
}

void numcpy(const bign& b, bign& t, int rmove) { //数组右移使两个数组右端对齐
	for (int i = 0; i < b.len; ++i) 
		t.d[i + rmove] = b.d[i]; //对应的值右移n位赋给t.d数组 
	t.len = b.len + rmove; //注意长度, 减法需要
}
//高精度除法, 余数也是高精度的
bign superDiv(bign& a, bign& b) { //这里最终的a就是余数 
	bign c, temp;
	//两个非零因数相乘=除，商的位数要么是两个因数位数之差，要么是两个因数位数之差+1 
	c.len = a.len - b.len + 1; 
	//将除数移动, 与被除数最高位对齐需要的位数 
	for (int i = a.len - b.len; i >= 0; --i) { //用i控制移动的位数, 先是移动对齐最高位 
		memset(temp.d, 0, sizeof(temp.d)); //temp数组清0 
		numcpy(b, temp, i); //将除数b右移后复制给tmp数组，注意用i控制除数位数; 
		while (a.compare(temp) >= 0) {
			++c.d[i];         //减法模拟除法，并计数 
			a = sub(a, temp); //从a中减去一个temp 
		}
	} 
	while (c.d[c.len - 1] == 0 && c.len >= 2) //控制最高位的0 
		--c.len;
	return c;
}
int main() {
	bign a("1234"), b("28"); 
	bign ans = superDiv(a, b);
	ans.print();
	return 0;
}
