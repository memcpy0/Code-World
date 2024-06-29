#include <iostream>
using namespace std;

class Date
{
	public:
		void SetDate(int y=2002, int m=10, int n=1);
		int IsLeapYear();
		void Print() { cout << year << "." << month << "." << day << endl;}
	private:
		int year, month, day;
};
void Date::SetDate(int y, int m, int d)
{
	year=y;
	month=m;
	day=d;
}
int Date::IsLeapYear()
{
	return (year%4==0 && year%100!=0) || (year%400==0);
}

int main()
{
	Date date1, date2;
	date1.SetDate();
	date2.SetDate(2000, 10, 1);
	cout << "date1:"; 
	date1.Print();
	cout << "date2:";
	date2.Print();
	if (date2.IsLeapYear()) cout << "date2 is a leapyear." << endl;
	else cout << "date2 is not a leapyear." << endl;
	return 0;
}
