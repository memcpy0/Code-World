#include <iostream>
using namespace std;
class Time;
class Date
{
	public:
		Date(int y=2003, int m=1, int d=1):year(y), month(m), day(d) { }
	    void DateTime(const Time &t);
	private:
		int year, month, day;
}; 
class Time
{
	public:
		Time(int h=0, int m=0, int s=0):hour(h), minute(m), second(s) { }
		friend void Date::DateTime(const Time &t);    // ÓÑÔª¶ÔÏó 
	private:
		int hour, minute, second;
};
void Date::DateTime(const Time &t)
{
	cout << "Now is " << year << "." << month << "." << day << " "
	     << t.hour << ":" << t.minute << ":" << t.second << "." << endl;
}

int main()
{
	Date d(2003, 10, 10);
	Time t(10, 10, 10);
	d.DateTime(t);
	return 0; 
}
