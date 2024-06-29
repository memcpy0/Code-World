#include <iostream>
#include <cmath>
#ifndef ShapeH
#define ShapeH
#define PI 3.14159265357988
using namespace std;

class Shape {
	public:
		virtual double area() const=0;
		virtual void draw() const=0;
};
void Shape::draw() const { cout << "这里应该显示一个纯虚函数的不完善的实现版本.";}

class Point {
	double x, y;
	public:
		Point(double x0=0.0, double y0=0.0):x(x0),y(y0) { }
		double getX() const { return x;}
		double getY() const { return y;}
};

class Rectangle:public Shape {
	Point ul;          // 左上角 
	Point dr;          // 右下角 
	public:
	    Rectangle(double x1, double y1, double x2, double y2):ul(x1, y1), dr(x2, y2) { }
		virtual double area() const {
		    return fabs((dr.getX()-ul.getX())*(dr.getY()-ul.getY()));
		} 
		virtual void draw() const { Shape::draw(); cout << "矩形";}
		Point getUpleft() const { return ul;}
		Point getDownright() const { return dr;}
};

class Circle:public Shape {
	Point cen;
	double rad;
	public:
		Circle(double x, double y, double r):cen(x, y), rad(r) {	}
		virtual double area() const { return PI*rad*rad;}
		virtual void draw() const { Shape::draw(); cout << "圆形";}
		Point getCenter() const {return cen;}
		double getRadius() const { return rad;}
};
void showArea(Shape &s) { cout << s.area();}
void drawShape(Shape &s) { s.draw();}
#endif

int main(int argc, char **argv)
{
	Rectangle r(2.0, 2.0, 8.0, 8.0);
	showArea(r);
	drawShape(r);
	cout << endl;
	Circle c(8, 8, 4);
	showArea(c);
	drawShape(c);
	return 0;
} 

