#include <bits/stdc++.h>
using namespace std;
const int N = 10005;

struct node {
	char name[35];
	int price;	
};
vector<node> List[N];	//����ַ��,���ڽ����ͻ 

unsigned BKDRHash(const char *str) { //��ϣ���� 
	register unsigned seed = 31, key = 0;
	while (*str) key = key * seed + (*str++);	
	return key & 0x7fffffff;
}

int main() {
	int n, m, key, add, len, rank; //key�����ϣֵ,add��ʾÿ��ļ۸�����,rank���� 
	int p[N], memory_price;		   //��¼ÿ������е��̵ļ۸�, memory�۸� 
	char str[35];
	node t; 
	while (~scanf("%d", &n)) {
		for (int i = 0; i < N; ++i)
			List[i].clear();
		for (int i = 0; i < n; ++i) { 
			scanf("%s", t.name);
			key = BKDRHash(t.name) % N;	//����hashֵ��ȡ��
			List[key].push_back(t);		//hashֵ���ܳ�ͻ,�ѳ�ͻ�Ĺ�ϣֵ�洢���� 
		}
		scanf("%d", &m);
		while (m--) {
			rank = len = 0;
			for (int i = 0; i < n; ++i) {
				scanf("%d%s", &add, str);	
				key = BKDRHash(str) % N; //�����ϣֵ
				for (int j = 0; j < List[key].size(); ++j) { //�����ͻ���� 
					if (strcmp(List[key][j].name, str) == 0) { //������ĵ�������һ�� 
						List[key][j].price += add;			 //�޸ļ۸� 
						if (strcmp(str, "memory") == 0)		  
							memory_price = List[key][j].price;
						else
							p[len++] = List[key][j].price;   //����memory,��¼���� 
						break;								 //����޸�,ֱ���˳� 
					}
				}
			}
			for (int i = 0; i < len; ++i) 
				if (memory_price < p[i])
					++rank;
			printf("%d\n", rank + 1);
		}
	}
	return 0;
}
