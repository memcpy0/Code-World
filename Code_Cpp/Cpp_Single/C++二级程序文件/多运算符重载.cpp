#ifndef fractionH
#define fractionH
#include <iostream>
#include <cmath>
using namespace std;
static long gcd(long m1, long m2) { // 求最大公约数 
	while (m2) {
		long m = m1;
		m1 = m2;
		m2 = m % m2;
	}
	return labs(m1);
}
class fraction {
	long num;            // 分子 
	long den;            // 分母 
	void standardize();  // 分数标准化函数
    public:
        fraction(long n=0, long d=1); // 构造函数 
        fraction operator-() const{                           /* 重载取负运算符-为成员函数, 形参表中无参数 */ /* 类似的一元运算符除++ --均可参考这种 */
        	return fraction(-num, den);
		}
		fraction operator+(fraction f) const{                 /* 重载加号运算符+为成员函数, 形参表中一个参数 */ /* 除赋值类运算符外的二元运算符均可参考 */ 
	        return fraction(num*f.den+den*f.num, den*f.den);
		}
		fraction& operator++() {                             /* 重载前缀++, 即原对象++并返回原对象的引用 */ /* 重载前后缀--, 可以参考 */ 
			num += den;
			return *this;
		}
		fraction operator++(int) {                           /* 重载后缀++, 对原对象++, 返回之前的值的对象 */
			num += den;
			return fraction(num-den, den);
		}
	    operator long() const { return num/den; }            /* 重载类型转换符时,运算符已经表示出了返回值类型,不需要返回值类型的声明, 其他类型的类型转换符可参照 */
	    fraction& operator=(fraction f) {                    /* 类型转换符和赋值运算符的=只能作为成员函数重载 */
	    	num = f.num;                                     /* 真正重载赋值运算符的情况是类中包含指向动态空间的指针 */ 
	    	den = f.den;
	    	return *this;                         
		} 
		fraction& operator+=(fraction f) {           /* 重载取负运算符+=为成员函数, 形参表中一个参数 */ /* 类似的运算符-= /=均可参考这种 */
			*this = *this + f;
			return *this;
		} 
		bool operator>(fraction f) {                          /* 重载关系运算符>返回逻辑值 */
			return num*f.den>f.num*den;
		}		
		friend istream& operator>>(istream& is, fraction& f);
		friend ostream& operator<<(ostream& os, const fraction& f);
};
#endif
/* 重载流运算符>>和<< */
istream& operator>>(istream& is, fraction& f)
{
	char c;
	is>>f.num>>c>>f.den;
	f.standardize();
	return is;
}
ostream& operator<<(ostream& os, const fraction& f)
{
	os<<f.num<<'/'<<f.den;
	return os;
}
/* 重载取负运算符-为非成员函数, 形参表中有一个参数 */
/* friend fraction operator-(fraction f); 声明为友元函数 
  fraction operator-(fraction f) {
	return fraction(-f.num, f.den)
} */

/* 重载加法运算符+为非成员函数, 形参表中有两个参数 */
/* friend fraction operator+(fraction f1, fraction f2);
   fraction operator+(fraction f1, fraction f2) {
   return fraction(f1.num * f2.den + f1.den * f2.num, f1.den * f2.den);
}
*/ 

/* 重载前缀++为非成员函数, 形参表中有一个参数 */
/* friend fraction& operator++(fraction &f);
  fraction& operator++(fraction &f) {
      f.num += f.den;
      f.standardize();
      return f;
}
*/ 
/* 重载后缀++为非成员函数, 形参表中有两个参数 */
/* friend fraction operator++(fraction &f, int);
  fraction operator++(fraction &f, int) {
      f.num += f.den;
      f.standardize();
      return fraction(f.num-f.den, f.den);
}
*/ 
 /* 重载取负运算符+=为非成员函数, 形参表中两个参数 */
/* fraction& operator+=(fraction& f1, fractionf2)  利用前面重载的运算符, 从而将f1+=f2解释成了f1=f1+f2; 
{   // 为了保持原有的特性, 第一参数必须声明为引用, 否则无法改变它的值, 作为成员函数时, 第一参数由this指针指向; 
    // 返回值也应当声明为引用,并在最后将获得新值的第一参数返回 
	f1 = f1+f2;                                 
	return f1;
} 
*/
/* 重载关系运算符>为非成员函数 */ 
/* friend bool operator>(fraction f1, fraction f2);
   bool operator>(fraction f1, fraction f2)
{
    return f1.num*f2.den > f2.num*f1.den;
}
*/
void fraction::standardize(){     // 分数标准化 
	if (num == 0L || den == 0L) { // 确保分母不为零 
		num = 0L;
		den = 1L;
		return;
	} 
	if (den < 0L) {          // 分数的符号用分子的符号表示 
		num = -num;
		den = -den;
	}
	long g = gcd(num, den);  // 总化简为最简分数 
	num /= g;
	den /= g; 
} 

fraction::fraction(long n, long d):num(n), den(d) {
	standardize();    
} 

int main(int argc, char **argv)
{
	fraction f0(7, 2), f1(9, 5);
	cout << long(f0) << '\t' << long(f1) << endl;
	cout << f0 << ", " << f1;
	return 0;
} 
