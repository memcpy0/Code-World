// C++ Program to add two numbers without using arithmetic operator  
#include <bits/stdc++.h> 
using namespace std; 

//判断奇数或偶数 
//returns true if n is even, else odd 
bool isEven(int n) { 
	//n & 1 is 1, then odd, else even 
	return (!(n & 1)); 
} 

//无算术运算符加法 
int add(int x, int y)  
{  
    // Iterate till there is no carry  
    while (y != 0)  
    {  
        // carry now contains common  
        //set bits of x and y  
        int carry = x & y;  
  
        // Sum of bits of x and y where at  
        //least one of the bits is not set  
        x = x ^ y;  
  
        // Carry is shifted by one so that adding  
        // it to x gives the required sum  
        y = carry << 1;  
    }  
    return x;  
}  
//无算术运算符加法递归版 
int add2(int x, int y) {
	if (y == 0) return x;
	return add(x ^ y, (x & y) << 1);
}
//无算术运算符加1
int addOne(int x) {
	int m = 1;
	//flip all the set bits until we find rightmost 0 
	while (x & m) { 
		x = x ^ m;
		m <<= 1;
	}
	//flip the rightmost 0 
	x = x ^ m;
	return x;
}
//无算术运算符加1
int addOne2(int x) {
	return (-(~x));
}
//判断两个数是否异号 
//C++ Program to Detect if two integers have opposite signs
bool oppositeSigns(int x, int y) {
	return ((x ^ y) < 0);
}

//判断两个数是否相等 
//compare two integers without any comparison operator
//return true if (A ^ B) == 0 else return false
bool equalNumber(int A, int B) {
	return !(A ^ B);
}

//快速翻转最右边的1 
//unsets the rightmost set bit of n and returns the result
int unsetRight1(unsigned int n) {
	return n & (n - 1);
}

//快速判断是否是3的倍数, 十进制下也有类似的思路可以判断是3、11的倍数 
//Check if a integer is multiple of 3 efficently 
int isMultipleOf3(int n) {
	int oddCount = 0, evenCount = 0;
	
	/*make the number positive if it's negative.
	We are doing this to avoid stackoverflow is recursion. */
	if (n < 0) n = -n; 
	if (n == 0) return true; 
	if (n == 1) return false;
	while (n) {
		if (n & 1) ++oddCount;
		if (n & 2) ++evenCount;
		n = n >> 2;
	}
	return isMultipleOf3(abs(oddCount - evenCount));
}

//快速乘法: 这里是乘以7 
long long multiplyBySeven(long long n) {
	return ((n << 3) - n);
}

//乘3.5 Multiply a given integer with 3.5 
int multiplyWith3Point5(int x) {
	return (x << 1) + x + (x >> 1); //return ((x << 3) - x) >> 1; 
}

//判断数是否是4的幂 find whether a number is the power of 4 or not
bool isPowerOf4(int n) {
	if (n == 0) return false;
	while (n != 1) {
		if (n % 4 != 0) return false;
		n /= 4;
	}
	return true;
}
//判断数是否是4的幂 二进制形式 
bool isPowerOfFour(int n) {
	int count = 0;
	//check n is 0 or not and if there is only one bit set in n
	if (n && !(n & (n - 1))) {
		//count 0 bits before set bit  
		while (n > 1) {
			n >>= 1;
			count += 1;
		}
	} else return false;
	//if count is even then return true else false. 
	return (count % 2 == 0) ? true : false;
}
//判断数是否是4的幂 二进制形式 n & 0xAAAAAAAA
bool isPowerOfFour2(unsigned int n) {
	return n && !(n & (n - 1)) && !(n & 0xAAAAAAAA);
}

//判断数是否是8的幂 Check if power of 8
bool checkPowerOf8(int n) {
	double i = log(n) / log(8);
	//check if i is an integer or not
	return (i - trunc(i) < 0.000001);
}
bool checkPowerOfEight(unsigned int n) {
	return (n && !(n & (n - 1)) && !(n & 0xB6DB6DB6));
}
bool checkPowerOfEight2(unsigned int n) {
	int i = 0; //i will denote the bit that we're currently at
	unsigned long long l = 1;
	while (i <= 63) {
		l <<= i;
		//if only set bit in n is at position i
		if (l == n) return true;
		//to next valid bit position
		i += 3;
		l = 1;
	}
	return false;
}

//Check for integer overflow 整型溢出 
bool addOvf(int &result, int a, int b) {
	result = a + b;
//	if (a > 0 && b > 0 && result < 0) return false;
//	if (a < 0 && b < 0 && result > 0) return false;
	if (a > INT_MAX - b || a < INT_MIN - b) return false; 
	return true;
}


//计数1's bits 
//Count set bits in an integer
unsigned int countSetBits0(unsigned int n) {
	unsigned int count = 0;
	while (n) {
		n &= (n - 1);
		++count;
	}
	return count;
}

unsigned int lookuptable[256];
//generate lookup table
void initialize() {
	lookuptable[0] = 0;
	for (int i = 0; i < 256; ++i)
		lookuptable[i] = (i & 1) + lookuptable[i >> 1];
}
//count set bits using lookup table and return set bits count
unsigned int countSetBits(int N) {
	unsigned int count = lookuptable[N & 0xff] + 
						 lookuptable[(N >> 8) & 0xff] + 
						 lookuptable[(N >> 16) & 0xff] + 
						 lookuptable[(N >> 24) & 0xff];
	return count;
}

int num_to_bits[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};                       
unsigned int countSetBitsRec(unsigned int num) {
	int nibble = 0;
	if (num == 0) return num_to_bits[0];
	nibble = num & 0xf; 
	//use map to find count in last nibble
	//then plus recursively the counts of remaining nibbles
	return num_to_bits[nibble] + countSetBitsRec(num >> 4);
}

int main() {  
	int n = 101; 
	isEven(n)? cout << "Even" : cout << "Odd"; 
	
//  cout << add(15, 32) << endl << add2(15, 32); 
//	cout << addOne(13);  //0000 1011->0000 1100
//	cout << addOne(-13);

//	int x = 123, y = -100;
//	if (oppositeSigns(x, y)) cout << "Signs are opposite" << endl;
//	else cout << "Signs are not opposite" << endl;

//	int A = 5, B = 6;
//	cout << equalNumber(A, B) << endl;

//	cout << "7 after unsetting the  rightmost set bit " << unsetRight1(7) << endl;
//  cout << "-1 after unsetting the  rightmost set bit " << unsetRight1(-1) << endl;

//	int num = 18;
//	if (isMultipleOf3(num)) printf("%d is multiple of 3.", num);
//	else printf("%d is not a multiple of 3.", num);

//	long long n = 4;
//	cout << multiplyBySeven(n) << endl;

//	cout << multiplyWith3Point5(2) << endl << multiplyWith3Point5(5) << endl; 

//	int test = 20; 
//	if (isPowerOfFour2(test)) cout << test << " is a power of 4";
//	else cout << test << " is not a power of 4";
//	int n= 64; 
//  checkPowerOfEight2(n) ? cout << "Yes" : cout << "No";
     
//	int res, x = -2147483640, y = -10;   
//    cout << addOvf(res, x, y) << endl;
//	cout << res; 

//	int i = 13; 
//	cout << countSetBits0(i);
//	initialize();
//	unsigned int n = 354;
//	cout << countSetBitsRec(n) << endl;
	return 0;  
}
