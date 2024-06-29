#include <iostream>
#include <iomanip>
#include <cstdlib>  // 使用库函数exit()
using namespace std;

template <typename TYPE> 
class Array 
{
    public:
	    Array(int n);
		~Array() {delete []data;}
		TYPE &operator[](int i);
		int size() const { return _size;}	
    private:
    	int _size;
    	TYPE *data;  // 区分不同类模板实例的有关类型应使用虚拟参数 
};

template <typename TYPE>  // 类模板成员函数的开头声明 
Array<TYPE>::Array(int n) 
{
	if (n < 1) {
		cout << "Error dimension description.";
		exit(1);
	} 
	_size = n;
	data = new TYPE[_size];
} 

template <typename TYPE>
TYPE &Array<TYPE>::operator [](int i)
{
	if (i < 1 || i > _size) {
		cout << endl << "Subscript out of range.";
		delete []data;
		exit(2);
	}
	return data[i-1];
} 

int main()
{
	Array<int> a(5);
	for (int i=1; i<=a.size(); i++) a[i] = i;
	for (int i=1; i<=a.size(); i++) cout << setw(5) << a[i];
	cout << endl << endl;
	Array<double> b(5);
	for (int i=1; i<=b.size(); i++) b[i] = i+i/10.0;
	for (int i=1; i<=b.size(); i++) cout << setw(5) << b[i] << " ";
	cout << endl << endl;
}
