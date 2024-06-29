#ifndef fractionH
#define fractionH
#include <iostream>
#include <cmath>
using namespace std;
static long gcd(long m1, long m2) { // �����Լ�� 
	while (m2) {
		long m = m1;
		m1 = m2;
		m2 = m % m2;
	}
	return labs(m1);
}
class fraction {
	long num;            // ���� 
	long den;            // ��ĸ 
	void standardize();  // ������׼������
    public:
        fraction(long n=0, long d=1); // ���캯�� 
        fraction operator-() const{                           /* ����ȡ�������-Ϊ��Ա����, �βα����޲��� */ /* ���Ƶ�һԪ�������++ --���ɲο����� */
        	return fraction(-num, den);
		}
		fraction operator+(fraction f) const{                 /* ���ؼӺ������+Ϊ��Ա����, �βα���һ������ */ /* ����ֵ���������Ķ�Ԫ��������ɲο� */ 
	        return fraction(num*f.den+den*f.num, den*f.den);
		}
		fraction& operator++() {                             /* ����ǰ׺++, ��ԭ����++������ԭ��������� */ /* ����ǰ��׺--, ���Բο� */ 
			num += den;
			return *this;
		}
		fraction operator++(int) {                           /* ���غ�׺++, ��ԭ����++, ����֮ǰ��ֵ�Ķ��� */
			num += den;
			return fraction(num-den, den);
		}
	    operator long() const { return num/den; }            /* ��������ת����ʱ,������Ѿ���ʾ���˷���ֵ����,����Ҫ����ֵ���͵�����, �������͵�����ת�����ɲ��� */
	    fraction& operator=(fraction f) {                    /* ����ת�����͸�ֵ�������=ֻ����Ϊ��Ա�������� */
	    	num = f.num;                                     /* �������ظ�ֵ���������������а���ָ��̬�ռ��ָ�� */ 
	    	den = f.den;
	    	return *this;                         
		} 
		fraction& operator+=(fraction f) {           /* ����ȡ�������+=Ϊ��Ա����, �βα���һ������ */ /* ���Ƶ������-= /=���ɲο����� */
			*this = *this + f;
			return *this;
		} 
		bool operator>(fraction f) {                          /* ���ع�ϵ�����>�����߼�ֵ */
			return num*f.den>f.num*den;
		}		
		friend istream& operator>>(istream& is, fraction& f);
		friend ostream& operator<<(ostream& os, const fraction& f);
};
#endif
/* �����������>>��<< */
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
/* ����ȡ�������-Ϊ�ǳ�Ա����, �βα�����һ������ */
/* friend fraction operator-(fraction f); ����Ϊ��Ԫ���� 
  fraction operator-(fraction f) {
	return fraction(-f.num, f.den)
} */

/* ���ؼӷ������+Ϊ�ǳ�Ա����, �βα������������� */
/* friend fraction operator+(fraction f1, fraction f2);
   fraction operator+(fraction f1, fraction f2) {
   return fraction(f1.num * f2.den + f1.den * f2.num, f1.den * f2.den);
}
*/ 

/* ����ǰ׺++Ϊ�ǳ�Ա����, �βα�����һ������ */
/* friend fraction& operator++(fraction &f);
  fraction& operator++(fraction &f) {
      f.num += f.den;
      f.standardize();
      return f;
}
*/ 
/* ���غ�׺++Ϊ�ǳ�Ա����, �βα������������� */
/* friend fraction operator++(fraction &f, int);
  fraction operator++(fraction &f, int) {
      f.num += f.den;
      f.standardize();
      return fraction(f.num-f.den, f.den);
}
*/ 
 /* ����ȡ�������+=Ϊ�ǳ�Ա����, �βα����������� */
/* fraction& operator+=(fraction& f1, fractionf2)  ����ǰ�����ص������, �Ӷ���f1+=f2���ͳ���f1=f1+f2; 
{   // Ϊ�˱���ԭ�е�����, ��һ������������Ϊ����, �����޷��ı�����ֵ, ��Ϊ��Ա����ʱ, ��һ������thisָ��ָ��; 
    // ����ֵҲӦ������Ϊ����,������󽫻����ֵ�ĵ�һ�������� 
	f1 = f1+f2;                                 
	return f1;
} 
*/
/* ���ع�ϵ�����>Ϊ�ǳ�Ա���� */ 
/* friend bool operator>(fraction f1, fraction f2);
   bool operator>(fraction f1, fraction f2)
{
    return f1.num*f2.den > f2.num*f1.den;
}
*/
void fraction::standardize(){     // ������׼�� 
	if (num == 0L || den == 0L) { // ȷ����ĸ��Ϊ�� 
		num = 0L;
		den = 1L;
		return;
	} 
	if (den < 0L) {          // �����ķ����÷��ӵķ��ű�ʾ 
		num = -num;
		den = -den;
	}
	long g = gcd(num, den);  // �ܻ���Ϊ������ 
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
