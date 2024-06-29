#include <iostream>
#include <cstdio>

using namespace std;

int strlen_v(char s[])
{
    int n = 0; 	
    while (s[n] != '\0') n++;
    return n;
}

int strlen_p(char *s)
{
	int n = 0;
	for (; *s != '\0'; n++,s++);
	return n;
}

int main()
{
	char st[100];
	cin >> st;
	cout << strlen_v(st) << endl;
	return 0;
}
