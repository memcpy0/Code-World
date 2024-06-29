#include <iostream>
using namespace std;

class Point
{
	public:
		void InitPoint(int x, int y);
		void Move(int xOff, int yOff);
		int GetX() {return X;}
		int GetY() {return Y;}
	private:
		int X, Y;
};
void Point::InitPoint(int x, int y)
{
	X = x;
	Y = y; 
}
void Point::Move(int xOff, int yOff)
{
	X += xOff;
	Y += yOff; 
}

class Rectangle: public Point
{
	public:
		void InitRectangle(int x, int y, int w, int h);
		int GetW() {
			return W;
		}
		int GetH() {
			return H;
		}
	private:
		int W, H;
};
void Rectangle::InitRectangle(int x, int y, int w, int h)
{
	InitPoint(x, y);
	W = w;
	H = h;
}
int main()
{
	Rectangle rect;
	rect.InitRectangle(2,3,10,20);
	rect.Move(3, 2);
	cout << rect.GetX() << ", " << rect.GetY() << ", " << rect.GetW() << ", " << rect.GetH() << endl;
	return 0; 
}
