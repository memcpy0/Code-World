#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	char grade;
	cout << "Please input a grade(A, B, C or D): ";
	cin >> grade;
	switch (grade) {
		case 'A':
			cout << "100 -- 90" << endl;
			break;
		case 'B':
			cout << "89 -- 80" << endl;
			break;
		case 'C':
			cout << "79 -- 70" << endl;
			break;
	    case 'D':
	    	cout << "69 -- 60" << endl;
	    	break;
    	case 'E':
    		cout << "Less than 60" << endl;
    		break;
   		default:
   			cout << "Input error!\n";
	}
	return 0;
}
