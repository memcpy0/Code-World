#include <bits/stdc++.h>
using namespace std;

int main() {
	int n; scanf("%d", &n);
	unordered_map<string, int> acc; //�˺� 
	unordered_map<string, string> psw; //����
	char c;
	string a, p;
	for (int i = 0; i < n; i++) {
		cin >> c >> a >> p;
		switch (c) {
			case 'N':
				if (acc[a] == 0) {
					acc[a] = 1; //�������ʻ�
					psw[a] = p; //�������� 
					printf("New: OK\n");
				} else printf("ERROR: Exist\n"); //������ĺ����Ѿ�����
				break;
			case 'L': 
				if (acc[a] == 0) printf("ERROR: Not Exist\n"); //���ʻ�QQ���벻����
				else if (psw[a] != p) printf("ERROR: Wrong PW\n"); //���ʻ��������
				else printf("Login: OK\n");
				break;
		}
	} 
	return 0;
}
