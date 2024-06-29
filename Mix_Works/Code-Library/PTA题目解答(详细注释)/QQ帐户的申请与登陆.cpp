#include <bits/stdc++.h>
using namespace std;

int main() {
	int n; scanf("%d", &n);
	unordered_map<string, int> acc; //账号 
	unordered_map<string, string> psw; //密码
	char c;
	string a, p;
	for (int i = 0; i < n; i++) {
		cin >> c >> a >> p;
		switch (c) {
			case 'N':
				if (acc[a] == 0) {
					acc[a] = 1; //新申请帐户
					psw[a] = p; //存入密码 
					printf("New: OK\n");
				} else printf("ERROR: Exist\n"); //新申请的号码已经存在
				break;
			case 'L': 
				if (acc[a] == 0) printf("ERROR: Not Exist\n"); //老帐户QQ号码不存在
				else if (psw[a] != p) printf("ERROR: Wrong PW\n"); //老帐户密码错误
				else printf("Login: OK\n");
				break;
		}
	} 
	return 0;
}
