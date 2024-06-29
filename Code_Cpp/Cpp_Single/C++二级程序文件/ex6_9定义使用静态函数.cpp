#include <iostream>
using namespace std;

class Point 
{
    public:
        Point(int xx=0, int yy=0, int zz=0):X(xx), Y(yy), Z(zz) 
		{
        	count++;
		}	
		static void Display(Point& p);
    private:
    	int X, Y, Z;
    	static int count;
}; 
int Point::count = 0;
void Point::Display(Point& p) 
{
	cout << "The point is (" << p.X << ", " << p.Y << ", " << p.Z << ")" << endl; 
	cout << "There are " << count << " point objects." << endl;
}
int main()
{
	Point p1(1,2,3), p2(4,5,6);
	Point::Display(p1);
	Point::Display(p2);
	return 0;
}
