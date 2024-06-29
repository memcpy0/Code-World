#include <iostream>
#include <list>
using namespace std;

int main() {
    list<int> a;
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; ++i) a.push_back(i + 1); 
   
    int kth = 1;  // 数数
    while (a.size() > 1) {
	    for (auto it = a.begin(); it != a.end(); ) { // 通过迭代器删除
	        if (kth++ % m == 0)	it = a.erase(it); //删除这个元素, 并返回下一个元素的迭代器
	        else ++it; //不用删除的时候, 直接指向下一个元素
	    }
	}
    cout << *(a.begin());
    return 0;
} 
