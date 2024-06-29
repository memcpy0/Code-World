#include <iostream>
using namespace std;
class Time;
class Date
{
	public:
		Date():month(0), day(0), year(0) 
		{
			cout << "Default constructor called." << endl;
		} 
		Date(int m, int d, int y):year(y), month(m), day(d) 
		{ 
	        cout << "Constructor called." << endl;
		}
		~Date()
		{
			cout << "Destructor called." << day << endl;
		}
		void Print() const
		{
			cout << "Month=" << month << ", Day="<< day << ", Year=" << year << endl;
		}
	private:
		
		int year, month, day;
}; 

int main()
{
	Date dates[5]={Date(10, 1, 2002), Date(10, 2, 2002), Date(10, 3, 2003)};
	dates[3] = Date(10, 4, 2002);
	dates[4] = Date(10, 5, 2002);
	for(int i=0; i<5; i++) dates[i].Print();
	return 0; 
} 
