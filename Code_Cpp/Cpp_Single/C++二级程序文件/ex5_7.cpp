#include <iostream>
using namespace std;

void grade(char ch) 
{
	switch(ch) {
		case 'A':
		case 'a':
			cout << "excellent!" << endl;
			break;
		case 'B':
		case 'b':
			cout << "good!" << endl;
			break;
		case 'C':
		case 'c':
			cout << "pass!" << endl;
			break;
		default:
			cout << "bad!" << endl;
	}
}

int main()
{
	char a1 = 'b', a2 = 'c', a3 = 'f';
	grade(a1);
	grade(a2);
	grade(a3);
	return 0;
}
