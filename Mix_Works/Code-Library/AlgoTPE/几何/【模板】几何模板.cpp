#include <bits/stdc++.h>
using namespace std;
//����ģ��
const double pi = acos(-1.0); //�߾���Բ����
const double eps = 1e-8;	  //ƫ��ֵ
const int maxp = 1010;		  //�������
int sgn(double x) {           //�ж�x�Ƿ����0,sgn����
	if (fabs(x) < eps) return 0;
	else return x < 0 ? -1 : 1;
}
int dcmp(double x, double y) { //�Ƚ�����������:0���,-1ΪС��,1Ϊ����
	if (fabs(x - y) < eps) return 0;
	else return x < y ? -1 : 1;
}
//------------------------------------ƽ�漸��:�����--------------------------------------------- 
struct Point { //�����ͻ������� 
	double x, y;  
	Point() {}
	Point(double x, double y) : x(x), y(y) {};
	Point operator+ (Point B) { return Point(x + B.x, y + B.y); }
	Point operator- (Point B) { return Point(x - B.x, y - B.y); }
	Point operator* (double k) { return Point(x * k, y * k); } //��������k��
	Point operator/ (double k) { return Point(x / k, y / k)}; } //������Сk��
	bool operator== (Point B) { return sgn(x - B.x) == 0 && sgn(y - B.y) == 0; } 
	bool operator<  (Point B) { //�Ƚ�������,����͹������ 
		return sgn(x - B.x) < 0 || (sgn(x - B.x) == 0 && sgn(y - B.y) < 0); 
	}
};
typedef Point Vector; //��������
double Dot(const Vector &A, const Vector &B) { return A.x * B.x + A.y * B.y; } //���
double Len(const Vector &A) { return sqrt(Dot(A, A)); } 					   //�����ĳ���
double Len2(const Vector &A) { return Dot(A, A); } 							   //�������ȵ�ƽ��
//A��B�ļн�
double Angle(const Vector &A, const Vector &B) { return acos(Dot(A, B) / Len(A) / Len(B)); }
double Cross(const Vector &A, const Vector &B) { return A.x * B.y - A.y * B.x; } //���
//������ABC���������
double Area2(const Point &A, const Point &B, const Point &C) { return Cross(B - A, C - A); }
//����ľ���,�����ַ�ʽʵ��
double Distance(const Point &A, const Point &B) { return hypot(A.x - B.x, A.y - B.y); }
double Dist(const Point &A, const Point &B) { 
	return sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y)); 
} 
//����A�ĵ�λ������
Vector Normal(const Vector &A) { return Vector(-A.y / Len(A), A.x / Len(A)); }
//�����Ƿ�ƽ�л��غ�
bool Parallel(const Vector &A, const Vector &B) { return sgn(cross(A, B)) == 0; }
//����A��ʱ����תrad��
Vector Rotate(const Vector &A, double rad) {
	return Vector(A.x * cos(rad) - A.y * sin(rad), A.x * sin(rad) + A.y * cos(rad)); 
}

struct Line {
	Point p1, p2;   //���ϵ�������
	Line() {}
	Line(const Point &p1, const Point &p2) : p1(p1), p2(p2) {}
	//����һ�������б�� angleȷ��ֱ�� 0 <= angle < pi
	Line(Point p, double angle) {
		p1 = p;
		if (sgn(angle - pi / 2) == 0) p2 = (p1 + Point(0, 1)); 
		else p2 = (p1 + Point(1, tan(angle))); 
	}
	//ax + by + c = 0
	Line(double a, double b, double c) {
		if (sgn(a) == 0) {
			p1 = Point(0, -c / b);
			p2 = Point(1, -c / b);
		}
		else if (sgn(b) == 0) {
			p1 = Point(-c / a, 0);
			p2 = Point(-c / a, 1);
		}
		else {
			p1 = Point(0, -c / b);
			p2 = Point(1, (-c - a) / b);
		}
	}
};
typedef Line Segment; //�����߶�,�����˵���Point p1, p2;
//����ֱ����б��, 0<= angle < pi
double Line_angle(const Line &v) {
	double k = atan2(v.p2.y - v.p1.y, v.p2.x - v.p1.x);
	if (sgn(k) < 0) k += pi;
	if (sgn(k - pi) == 0) k -= pi;
	return k;
}
//���ֱ�ߵĹ�ϵ:1Ϊ�����,2Ϊ���Ҳ�,0Ϊ��������
int Point_line_relation(const Point &p, const Line &v) {
	int c = sgn(Cross(p - v.p1, v.p2 - v.p1));
	if (c < 0) return 1; //p��v�����
	if (c > 0) return 2; //p��v���ұ�
	return 0; 			 //p��v��
}
//����߶εĹ�ϵ:0Ϊ��p�����߶���,1Ϊ��p���߶���
bool Point_on_seg(const Point &p, const Segment &v) {
	return sgn(Cross(p - v.p1, v.p1 - v.p2)) == 0 && sgn(Dot(p - v.p1, p - v.p2)) <= 0;
}
//��ֱ�ߵĹ�ϵ:0Ϊƽ��,1Ϊ�غ�,2Ϊ�ཻ
int Line_relation(const Line &v1, const Line &v2) {
	if (sgn(Cross(v1.p2 - v1.p1, v2.p2 - v2.p1)) == 0) {
		if (Point_line_relation(v1.p1, v2) == 0) return 1; //�غ�
		return 0;										   //ƽ��
	}
	return 2;											   //�ཻ
}
//�㵽ֱ�ߵľ���
double Dist_point_line(const Point &p, const Line &v) {
	return fabs(Cross(p - v.p1, v.p2 - v.p1)) / Distance(v.p1, v.p2);
}
//����ֱ���ϵ�ͶӰ
Point Point_line_projection(const Point &p, const Line &v) {
	double k = Dot(v.p2 - v.p1, p - v.p1) / Len2(v.p2 - v.p1);
	return v.p1 + (v.p2 - v.p1) * k;
}
//���ֱ�ߵĶԳƵ�
Point Point_line_symmetry(const Point &p, const Line &v) {
	Point q = Point_line_projection(p, v);
	return Point(2 * q.x - p.x, 2 * q.y - p.y);
}
//�㵽�߶εľ���
double Dist_point_seg(const Point &p, const Segment &v) {
	if (sgn(Dot(p - v.p1, v.p2 - v.p1)) < 0 || sgn(Dot(p - v.p2, v.p1 - v.p2)) < 0) //���ͶӰ�����߶���
		return min(Distance(p, v.p1), Distance(p, v.p2));
	return Dist_point_line(p, v); 													//���ͶӰ���߶���
}
//����ֱ��ab��cd�Ľ���,�ڵ���ǰ��֤��ֱ�߲�ƽ�л��غ�
Point Cross_point(const Point &a, const Point &b, const Point &c, const Point &d) { //Line:ab, cd
	double s1 = Cross(b - a, c - a);
	double s2 = Cross(b - a, d - a);
	return Point(c.x * s2 - d.x * s1, c.y * s2 - d.y * s1) / (s2 - s1);
}
//���߶��Ƿ��ཻ: 1Ϊ�ཻ,0Ϊ���ཻ
bool Cross_segment(const Point &a, const Point &b, const Point &c, const Point &d) { //Line:ab, cd
	double c1 = Cross(b - a, c - a), c2 = Cross(b - a, d - a);
	double d1 = Cross(d - c, a - c), d2 = Cross(d - c, b - c);
	return sgn(c1) * sgn(c2) < 0 && sgn(d1) * sgn(d2) < 0; //ע�⽻���Ƕ˵��������������� 
}
//------------------------------------ƽ�漸��:�����--------------------------------------------- 
struct Polygon {  
	int n; 			//����εĶ�����
	Point p[maxp];  //����εĵ�
	Line v[maxp];	//����εı�
};
//�жϵ���������εĹ�ϵ: 3Ϊ����; 2Ϊ����; 1Ϊ�ڲ�; 0Ϊ�ⲿ 
int Point_in_polygon(const Point &pt, const Point *&p, int n) { //��pt,�����Point * p
	for (int i = 0; i < n; ++i)    //���ڶ���εĶ����� 
		if (p[i] == pt) return 3;
	for (int i = 0; i < n; ++i) {  //���ڶ���εı���
		Line v = Line(p[i], p[(i + 1) % n]);
		if (Point_on_seg(pt, v)) return 2;
	}
	int num = 0;
	for (int i = 0; i < n; ++i) {
		int j = (i + 1) % n;
		int c = sgn(Cross(pt - p[j], p[i] - p[j]));
		int u = sgn(p[i].y - pt.y);
		int v = sgn(p[j].y - pt.y);
		if (c > 0 && u < 0 && v >= 0) ++num;
		if (c < 0 && u >= 0 && v < 0) --num;
	}
	return num != 0;  //1Ϊ�ڲ�; 0Ϊ�ⲿ
}
//��������
double Polygon_area(const Point *&p, int n) { //��ԭ�㿪ʼ����������
	double area = 0;
	for (int i = 0; i < n; ++i) 
		area += Cross(p[i], p[(i + 1) % n]);
	return area / 2; //��������и�,���ܼ�ȡ����ֵ
}
//�����ε�����
Point Polygon_center(const Point *&p, int n) {
	Point ans(0, 0);
	if (Polygon_area(p, n) == 0) return ans;
	for (int i = 0; i < n; ++i) 
		ans = ans + (p[i] + p[(i + 1) %n]) * Cross(p[i], p[(i + 1) % n]);
	return ans / Polygon_area(p, n) / 6.0;
}
//Convex_hull() ��͹��,͹���������ch��,����ֵ��͹���Ķ�����
int Convex_hull(Point *p, int n, const Point *&ch) {
	sort(p, p + n); 	//�Ե�����:��x��С��������,���x��ͬ,����y����
	n = unique(p, p + n) - p; //ȥ���ظ���
	int v = 0;
	//����͹��,���p[i]���ҹ����,����㲻����͹����,������
	for (int i = 0; i < n; ++i) {
		while (v > 1 && sgn(Cross(ch[v - 1] - ch[v - 2], p[i] - ch[v - 2])) <= 0)
			--v;
		ch[v++] = p[i];
	}
	int j = v;
	//����͹��
	for (int i = n - 2; i >= 0; --i) {
		while (v > j && sgn(Cross(ch[v - 1] - ch[v - 2], p[i] - ch[v - 2])) <= 0)
			--v;
		ch[v++] = p[i];
	}
	if (n > 1) --v;
	return v;  //����ֵ��͹���Ķ�����
}
//----------------------------------ƽ�漸��:Բ-------------------------------------------
struct Circle {
	Point c;  //Բ�� 
	double r; //�뾶
	Circle() {}
	Circle(const Point &c, double r): c(c), r(r) {}
	Circle(double x, double y, double _r) { c = Point(x, y); r = _r; }
};
//���Բ�Ĺ�ϵ: 0Ϊ����Բ��,1Ϊ����Բ��,2Ϊ����Բ��
int Point_circle_relation(const Point &p, const Circle &c) {
	double dst = Distance(p, C.c);
	if (sgn(dst - C.r) < 0) return 0;  //����Բ��
	if (sgn(dst - C.r) == 0) return 1; //Բ��
	return 2;						   //Բ�� 
}
//ֱ�ߺ�Բ�Ĺ�ϵ: 0Ϊֱ����Բ��, 1Ϊֱ�ߺ�Բ����, 2Ϊֱ����Բ��
int Line_circle_relation(const Line &v, const Circle &C) {
	double dst = Dist_point_line(C.c, v);
	if (sgn(dst - C.r) < 0) return 0;  //ֱ����Բ��
	if (sgn(dst - C.r) == 0) return 1; //ֱ�ߺ�Բ����
	return 2;						   //ֱ����Բ�� 
}
//�߶κ�Բ�Ĺ�ϵ: 0Ϊ�߶���Բ��, 1Ϊ�߶κ�Բ����, 2Ϊ�߶���Բ��
int Seg_circle_relation(const Segment &v, const Circle &C) {
	double dst = Dist_point_seg(C.c, v);
	if (sgn(dst - C.r) < 0) return 0;  //�߶���Բ�� 
	if (sgn(dst - C.r) == 0) return 1; //�߶κ�Բ���� 
	return 2; 						   //ֱ����Բ�� 
}
//ֱ�ߺ�Բ�Ľ���: pa, pb�ǽ���, ����ֵ�ǽ���ĸ���
int Line_cross_circle(const Line &v, const Circle &C, const Point &pa, const Point &pb) {
	if (Line_circle_relation(v, C) == 2) return 0; //�޽���
	Point q = Point_line_projection(C.c, v);       //Բ����ֱ���ϵ�ͶӰ��
	double d = Dist_point_line(C.c, v); 		   //Բ�ĵ�ֱ�ߵľ���
	double k = sqrt(C.r * C.r - d * d);
	if (sgn(k) == 0) { //һ������,ֱ�ߺ�Բ����
		pa = q;
		pb = q;
		return 1;
	}
	Point n = (v.p2 - v.p1) / Len(v.p2 - v.p1); //��λ����
	pa = q + n * k;
	pb = q - n * k;								//�������� 
	return 2;
}

//-------------------------------��ά����--------------------------------------------
//��ά:��
struct Point3 {
	double x, y, z;
	Point3() {}
	Point3(double x, double y, double z) : x(x), y(y), z(z) {}
	Point3 operator+ (const Point3 &B) { return Point3(x + B.x, y + B.y, z + B.z); }
	Point3 operator- (const Point3 &B) { return Point3(x - B.x, y - B.y, z - B.z); }
	Point3 operator* (double k) { return Point3(x * k, y * k, z * k); }
	Point3 operator/ (double k) { return Point3(x / k, y / k, z / k); }
	bool operator==  (const Point3 &B) { 
		return sgn(x - B.x) == 0 && sgn(y - B.y) == 0 && sgn(z - B.z) == 0; 
	}
};
typedef Point3 Vector3;
//���(�Ͷ�ά�������ͬ��)
double Dot(const Vector3 &A, const Vector3 &B) { return A.x * B.x + A.y * B.y + A.z * B.z; }
//��� 
const Vector3 Cross(const Vector3 &A, const Vector3 &B) { 
	return Point3(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x); 
} 
//�����ĳ��� 
double Len(const Vector3 &A, const Vector3 &B) {
	return sqrt(Dot(A, A)); 
} 
//�������ȵ�ƽ��  
double Len2(const Vector3 &A, const Vector3 &B) {
	return Dot(A, A);
}
//A��B�ľ���
double Distance(const Point3 &A, const Point3 &B) {
	return sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y) + (A.z - B.z) * (A.z - B.z)); 
}
//A��B�ļн�
double Angle(const Vector3& A, const Vector3 &B) {
	return acos(Dot(A, B) / Len(A) / Len(B)); 
}
//��ά:��
struct Line3 {
	Point3 p1, p2;
	Line3() {}
	Line3(const Point3 &p1, const Point3 &p2) : p1(p1), p2(p2) {}
};
typedef Line3 Segment3; //�����߶�,���˵���Point p1, p2
//��ά:���������������
double Area2(const Point3 &A, const Point3 &B, const Point3 &C) { return Len(Cross(B - A, C - A)); }
//��ά:�㵽ֱ�ߵľ���
double Dist_point_line(const Point3 &p, const Line3 &v) {
	return Len(Cross(v.p2 - v.p1, p - v.p1)) / Distance(v.p1, v.p2);
}
//��ά:����ֱ����
bool Point_line_relation(const Point3 &p, const Line3 &V) {
	return sgn(Len(Cross(v.p1 - p, v.p2 - p))) == 0 && sgn(Dot(v.p1 - p, v.p2 - p)) == 0;
}
//��ά:�㵽�߶εľ���
double Dist_point_seg(const Point3 &p, const Segment3 &v) {
	if (sgn(Dot(p - v.p1, v.p2 - v.p1)) < 0 || sgn(Dot(p - v.p2, v.p1 - v.p2)) < 0 
		return min(Distance(p, v.p1), Distance(p, v.p2));
	return Dist_point_line(p, v);
}
//��ά:����ֱ���ϵ�ͶӰ
Point3 Point_line_projection(const Point3 &p, const Line3 &v) {
	double k = Dot(v.p2 - v.p1, p - v.p1) / Len2(v.p2 - v.p1);
	return v.p1 + (v.p2 - v.p1) * k;
}
//��ά:ƽ��
struct Plane {
	Point3 p1, p2 ,p3; //ƽ���ϵ�������
	Plane() {}
	Plane(const Point3 &p1, const Point3 &p2, const Point3 &p3) : p1(p1), p2(p2), p3(p3) {}
};
//ƽ�淨����
Point3 Pvec(const Point3 &A, const Point3 &B, const Point3 &C) { return Cross(B - A, C - A); } 
Point3 Pvec(const Plane &f) { return Cross(f.p2 - f.p1, f.p3 - f.p1); }
//�ĵ㹲ƽ��
bool Point_on_plane(const Point3 &A, const Point3 &B, const Point3 &C, const Point3 &D) {
	return sgn(Dot(Pvec(A, B, C), D - A)) == 0;
}
//��ƽ��ƽ��
int Parallel(const Plane &f1, const Plane &f2) {
	return Len(Cross(Pvec(f1), Pvec(f2))) < eps;
}
//��ƽ�洹ֱ
int Vertical(const Plane &f1, const Plane &f2) {
	return sgn(Dot(Pvec(f1), Pvec(f2))) == 0;
}
//ֱ����ƽ��Ľ���p, ����ֵ�ǽ���ĸ���
int Line_cross_plane(const Line &u, const Plane &f, Point3 &p) {
	Point3 v = Pvec(f);    //ƽ��ķ����� 
	double x = Dot(v, u.p2 - f.p1);
	double y = Dot(v, u.p1 - f.p1);
	double d = x - y;
	if (sgn(x) == 0 && sgn(y) == 0) return -1;  //-1: v��f��
	if (sgn(d) == 0) return 0;					//0: v��fƽ��
	p = ((u.p1 * x) - (u.p2 * y)) / d;			//1: v��f�ཻ
	return 1; 
}
//�������������*6
double volume4(const Point3 &A, const Point3 &B, const Point3 &C, const Point3 &D) {
	return Dot(Cross(B - A, C - A), D - A);
}

int main() {
    freopen(".in","r",stdin);
    freopen(".out","w",stdout);

    return 0;
}

