#include <iostream>
using namespace std;

class Point
{
	public:
		void SetPoint(int x, int y);
		void Move(int xOff, int yOff);
		int GetX() { return X;}
		int GetY() { return Y;}
	private:
		int X, Y; 
}; 
void Point::SetPoint(int x, int y)
{
	X = x;
	Y = y;
 } 
void Point::Move(int xOff, int yOff)
{
	X += xOff;
	Y += yOff; 
}

int main()
{
	Point p1, p2;
	p1.SetPoint(1, 2);
	p2.SetPoint(3, 4);
	p1.Move(5, 6);
	p2.Move(7, 8);
	cout << "Point1 is (" << p1.GetX() << ", " << p1.GetY() << ")" << endl;
	cout << "Point2 is (" << p2.GetX() << ", " << p2.GetY() << ")" << endl;
	return 0;
}
