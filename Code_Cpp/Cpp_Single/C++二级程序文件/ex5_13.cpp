#include <iostream>
#include <cstring>
using namespace std;
int myMax(int x, int y);
char myMax(char first, char second);
double myMax(double u, double v);
char *myMax(char *first, char *second);

int main()
{
	cout << "In 3 and 5, the Max number is: " << myMax(3, 5) << endl;
	cout << "In 'c' and 'T', the Max char is: " << myMax('c', 'T') << endl;
	cout << "In 3.1 and -5.3, the Max double number is: " << myMax(3.1, -5.3) << endl;
	cout << "In 'what' and 'where', the Max string is: " << myMax("what", "where") << endl;
}

int myMax(int x, int y)
{
	if (x > y) return x;
	else return y;
}

char myMax(char first, char second)
{
	if (first > second) return first;
	else return second;
 } 

double myMax(double u, double v)
{
	if (u > v) return u;
	else return v;
}

char *myMax(char *first, char *second)
{
	if (strcmp(first, second) > 0) return first;
	else return second;
}
