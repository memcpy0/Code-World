#include <iostream>
using namespace std;

void message(char *msg);

int main()
{
    message("hello world!"); //�������� 
	return 0;	
}

void message(char *msg)
{
	cout << msg << endl;
}
