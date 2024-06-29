#include <bits/stdc++.h>
using namespace std;
const int N = 10005;

struct node {
	char name[35];
	int price;	
};
vector<node> List[N];	//链地址法,用于解决冲突 

unsigned BKDRHash(const char *str) { //哈希函数 
	register unsigned seed = 31, key = 0;
	while (*str) key = key * seed + (*str++);	
	return key & 0x7fffffff;
}

int main() {
	int n, m, key, add, len, rank; //key计算哈希值,add表示每天的价格增长,rank排名 
	int p[N], memory_price;		   //记录每天的所有店铺的价格, memory价格 
	char str[35];
	node t; 
	while (~scanf("%d", &n)) {
		for (int i = 0; i < N; ++i)
			List[i].clear();
		for (int i = 0; i < n; ++i) { 
			scanf("%s", t.name);
			key = BKDRHash(t.name) % N;	//计算hash值并取余
			List[key].push_back(t);		//hash值可能冲突,把冲突的哈希值存储起来 
		}
		scanf("%d", &m);
		while (m--) {
			rank = len = 0;
			for (int i = 0; i < n; ++i) {
				scanf("%d%s", &add, str);	
				key = BKDRHash(str) % N; //计算哈希值
				for (int j = 0; j < List[key].size(); ++j) { //处理冲突问题 
					if (strcmp(List[key][j].name, str) == 0) { //和输入的店铺名字一样 
						List[key][j].price += add;			 //修改价格 
						if (strcmp(str, "memory") == 0)		  
							memory_price = List[key][j].price;
						else
							p[len++] = List[key][j].price;   //不是memory,记录下来 
						break;								 //完成修改,直接退出 
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
