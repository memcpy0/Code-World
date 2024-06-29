#include <iostream>
using namespace std;

int fact(int n)
{
	int f = 1;
	while (n) 
	{
		f *= n;
		n--;
	}
	return f; 
}

int getInt()
{
	int x;
	cout << "Please input a integer: " << endl;
	cin >> x;
	return x; 
}

void printGrade(int score) 
{
	cout << "your grade for the course is:";
	if (score >= 90) cout << "A" << endl;
	else if(score >= 80) cout << "B" << endl;
	else if(score >= 70) cout << "C" << endl;
	else if(score >= 60) cout << "D" << endl;
	else cout << "F" << endl;
}

void printStars()
{
	for (int i = 1; i <= 100; i++)
	{
		for (int j = 0; j < i; j++) cout << "*";
		cout << endl;
	}
}

int main()
{
	printStars(); 
	return 0;
}
