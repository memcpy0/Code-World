#include <iostream>
#include <vector>
using namespace std;
int n, m;

int main() {
	cin >> n >> m;
	vector<int> vi;
	for (int i = 0; i < n; ++i) vi.push_back(i + 1);
	int pos = 0; //��ʼ��Ϊ0����Ϊ�����0��ʼ; ȡ�������õ�0-(n-1)�����ķ�Χ 
	while (vi.size() > 1) {
		pos = (pos + m - 1) % vi.size(); //�õ�pos����ĵ�m-1�������±�, ȡ��  
		vi.erase(vi.begin() + pos);  //ɾ�������; posָ���λ�����ڷŵ�����һ���� 
	}
	cout << vi[0];
	return 0;
}
