#include "String.h"
#include <iostream>
#include <string>
using namespace std; 

int main() {  
	char str[] = "helloworld";
	String s(str);
	cout << s << endl;
	
	String s2("this is my string");
	if (s < s2) cout << s;
	else cout << s2;
	cout << " is larger in lexicographical order." << endl;
	
	String s3(20, 10, 'a');
	cout << s3 << endl;
	
	String s4;
	s4 = "another function";
	cout << s4 << endl;
	s4.printStr();
	cout << endl;
	
	String arr[10];
	for (int i = 0; i < 8; ++i) arr[i].getline();
	for (int i = 0; i < 8; ++i) cout << arr[i] << endl;
		
//	String t;
//	freopen("input1.txt", "r", stdin);
//	for (int i = 0; i <= 41; ++i) {
//		t.getline();
//		cout << t << endl;
//	}
	return 0;
}
