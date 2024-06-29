#include <iostream>
using namespace std;

class Point 
{
    public:
        Point(int xx=0, int yy=0, int zz=0):X(xx), Y(yy), Z(zz) 
		{
        	count++;
		}	
		void Display() 
		{
			cout << "The point is (" << X << ", " << Y << ", " << Z << ")" << endl; 
			cout << "There are " << count << " point objects." << endl;
		}
    private:
    	int X, Y, Z;
    	static int count;
}; 
int Point::count = 0;

int main()
{
	Point p1(1,2,3), p2(4,5,6);
	p1.Display();
	p2.Display();
	return 0;
}
