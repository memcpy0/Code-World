#include <iostream>
#include <list>
using namespace std;

int main() {
    list<int> a;
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; ++i) a.push_back(i + 1); 
   
    int kth = 1;  // ����
    while (a.size() > 1) {
	    for (auto it = a.begin(); it != a.end(); ) { // ͨ��������ɾ��
	        if (kth++ % m == 0)	it = a.erase(it); //ɾ�����Ԫ��, ��������һ��Ԫ�صĵ�����
	        else ++it; //����ɾ����ʱ��, ֱ��ָ����һ��Ԫ��
	    }
	}
    cout << *(a.begin());
    return 0;
} 
