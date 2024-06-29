#include <iostream>
#include <vector>
using namespace std;
int n, m;

int main() {
	cin >> n >> m;
	vector<int> vi;
	for (int i = 0; i < n; ++i) vi.push_back(i + 1);
	int pos = 0; //初始化为0是因为数组从0开始; 取余操作会得到0-(n-1)的数的范围 
	while (vi.size() > 1) {
		pos = (pos + m - 1) % vi.size(); //得到pos后面的第m-1个数的下标, 取余  
		vi.erase(vi.begin() + pos);  //删除这个数; pos指向的位置现在放的是下一个数 
	}
	cout << vi[0];
	return 0;
}
