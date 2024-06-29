/*
13.19 ����13.5�����۵����������⣬Ӧ�÷�֧�޽��㷨������ʵ����
	inf    5	2	10 
	2	  inf   5   12
	3      7   inf   5
	8      2    4    inf
�����ʵ��������4�����к��໥֮����ʵĺķ�, �ҳ��ķ���С������ 
*/  
#include <iostream> 
#include <queue>
#include <vector> 
using namespace std;  
struct Node {
	int lcost, cc, rcost;  //lcost = cc + rcost���������½�, cc��ǰ����, x[s:n-1]�ж�����С���߷���   
	int s, *x;	//���ڵ㵽��ǰ�ڵ��·��Ϊx[0:s], ��Ҫ��һ�������Ķ�����x[s+1:n-1]  
};
 
class cmp{
public:
	bool operator()(const Node &a1,const Node &a2)  
	{
		return a1.lcost>a2.lcost;
	}
};
 
class TSP {
private:		
	int n;		//ͼ�Ķ�������  
 	int *a;		//ͼ���ڽӾ���  
 	int NoEdge;	//�ޱ߱�� 
 	 
 	int *x ;	//��ǰ�� 
 	int cc;		//��ǰ�ķ� 
 	int *bestx;	//���Ž⡢ 
 	int bestc;	//���·�� 
 
	
public:
    int Solve(int n_, int *a_, int NoEdge_, int v[])
    {
    	n = n_;
    	a = a_;
    	NoEdge = NoEdge_;
    	priority_queue<Node,vector<Node>,cmp> q;
    	int  MinOut[n + 1]; //i�ڵ��������ڵ��·������С�ĺķ� 
    	int MinSum = 0;
    	for(int i = 1; i <= n; ++i) {
    		int Min = NoEdge;
    		for(int j = 1; j <= n; ++j) {
    			if(a[i * (n + 1) + j] != NoEdge && (a[i * (n+1) + j] < Min || Min == NoEdge))
    				Min = a[i*(n+1)+j];
    		}
    		if(Min == NoEdge) return NoEdge;			//�޻�· 
    		MinOut[i] = Min;
			MinSum += Min; 
		}
		//��ʼ��
		Node E;
		E.x = new int[n];
		for (int i = 0; i < n; ++i) E.x[i] = i + 1;	//��ʼ˳��1-n 
		E.s = 0;						  	//��ʼ�ߵ�0 
		E.cc = 0;						  	//��ʼ����0 
		E.rcost = MinSum;				 
		int bestc = NoEdge;
		//������ռ�		
		while(E.s < n-1) {	//��Ҷ���
			if(E.s == n-2) { //��ǰ��չ�����Ҷ���ĸ��� 
				//�����ɻ�·�Ƿ������Ž� 
				if(a[E.x[n-2]*(n+1)+E.x[n-1]]!=NoEdge && a[E.x[n-1]*(n+1)+1]!=NoEdge &&
					(E.cc+a[E.x[n-1]*(n+1)+1]+a[E.x[n-1]*(n+1)+1] <bestc || bestc==NoEdge)) {
						//���ø�С��· 
						bestc = E.cc +a[E.x[n-2]*(n+1)+E.x[n-1]] + a[E.x[n-1]*(n+1)+1];
						E.cc = bestc;
						E.lcost = bestc;
						E.s++;
						q.push(E);
				} 
				else { // ������չ�ڵ� 
					delete [] E.x; 
				}
			}
			else { // ������ǰ��չ�ڵ�ĺ��ӽڵ� 
				for(int i=E.s+1; i<n;i++) {
					if(a[E.x[E.s]*(n+1)+E.x[i]]!=NoEdge) {
						//���н�� 
						int cc = E.cc + a[E.x[E.s]*(n+1)+E.x[i]];
						int rcost = E.rcost - MinOut[E.x[E.s]];
						int b = cc+rcost;	//�½�
						if(b<bestc || bestc == NoEdge) {
							//�������ܺ������Ž�
							//��������С��
							Node N;
							N.x = new int[n];
							for(int j=0;j<n;++j) N.x[j]=E.x[j];
							N.x[E.s+1] =E.x[i];		
							N.x[i]  = E.x[E.s+1];	//�ڵ㽻��
							N.cc =cc;				//���ø��� 
							N.s = E.s+1;			//����+1 
							N.lcost = b;
							N.rcost =rcost;
							q.push(N); 
						}
					}
 				}
 				delete [] E.x;	
			}
			if(q.empty()) break;
			E = q.top();
			q.pop(); 
		}
		if(bestc == NoEdge) return NoEdge;	//�޻�· 
		
    	//�������Ž⵽v[1��n] 
    	for(int i = 0; i < n; ++i) v[i + 1] = E.x[i];
    	
    	//�ͷŶ����е�ʣ��Ԫ�� 
		while(!q.empty()) {
			E = q.top();
			if(E.x) delete [] E.x;
			q.pop();
		}
		return bestc; 
	}
};
/*
��������̻�··��11
·��: 1 3 4 2 1
*/
int main() {
 	int n = 4; 
 	int NoEdge = 0x3fffffff; 
 	int a[] = { //�ķѾ�����±�Ϊ1��ʼ��ֵΪ0��ʾ���ɴ� 
 		0, 0, 0, 0, 0, 
 		0, 0, 5, 2, 10, 
 		0, 2, 0, 5, 12, 
 		0, 3, 7, 0, 5, 
 		0, 8, 2, 4, 0 
 	};
 	int v[5]; 
 	TSP tsp; 
 	int c = tsp.Solve(n, a, NoEdge, v); 
 	cout << "���·��" << c <<endl; 
 	cout << "·��:"; 
 	for(int i = 1; i <= n; ++i) 
 		cout<< ' ' << v[i];
 	cout << " 1" << endl;
 	return 0;
} 
