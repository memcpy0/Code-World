#include <iostream>
using namespace std;

int main()
{
	const int NUM = 5;
	int count = 0;
	bool quit = false;
	do {
		double sum = 0, g;
		cout << "Input 5 grades: ";
		for (int i = 0; i<NUM; i++) {
			cin >> g;
			sum += g;
		}
		count++;
		double avg = sum / NUM;
		cout << "No." << count << " AVG = " << avg << endl;
		char ch;
		cout << "Want to quit(YES='Y', 'y')?";
	 	cin >> ch;
		if (ch == 'Y' || ch == 'y') quit = true;
	} while (!quit);
	return 0;
}

