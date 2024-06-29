#ifndef __COMPLEX__
#define __COMPLEX__
#include <complex>
//guard declarations

#include <cmath>

//forward declarations
class ostream;
class complex;

complex&
	__doapl(complex* ths, const complex& r);

//class declaration
template<typename T>
class complex  //class head
{			   //class body
public:
	complex(T r = 0, T i = 0)
		: re(r), im(i)
	{	}
	complex& operator +=(const complex&);
	T real() const { return re; }
	T imag() const { return im; }
private:
	T re, im;
	//int func(const complex& param) {
	//    return param.re + param.im;
	//}
	friend complex& __doapl(complex*, const complex&);
};
ostream& 
operator <<(ostream& os, const complex& x) {
	return os << '(' << real(x) << ','
					 << imag(x) << ')';
}

//class definitions
//or doing like this
inline complex&
__doapl(complex* ths, const complex& r) {
	ths->re += r.re;
	ths->im += r.im;
	return *ths;  
}
inline complex&
complex::operator +=(const complex& r) {
	return __doapl(this, r);
} 

template<typename T>
inline T
real(const complex<T>& x) {
	return x.real();
}
template<typename T>
inline T
imag(const complex<T>& x) {
	return x.imag();
}

inline complex
operator +(const complex& x, const complex& y) {
	return complex(real(x) + real(y),
				   imag(x) + imag(y));
}
inline complex
operator +(const complex& x, double y) {
	return complex(real(x) + y, imag(x));
}
inline complex
operator +(double x, const complex& y) {
	return complex(x + real(y), imag(y));
}

inline complex
operator +(const complex& x) {
	return x;
}
inline complex
operator -(const complex& x) {
	return complex(-real(x), -imag(x));
}


#endif