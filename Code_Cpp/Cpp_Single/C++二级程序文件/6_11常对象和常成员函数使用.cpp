#include <iostream>
using namespace std;

class Point
{
	public:
		Point(int xx=0, int yy=0):X(xx), Y(yy){}
		void Print() {
			cout << "The Point is (" << X << ":" << Y << ")" << endl;
		} 
		void Print() const { cout << "The const Point is (" << X << "," << Y << ")" << endl;}
	private:
		int X, Y; 
};

int main()
{
	Point p1(1, 2);
	const Point p2(3, 4);
	p1.Print();
	p2.Print();
	return 0;
}
