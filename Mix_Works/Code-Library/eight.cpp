#include <iostream> 
using namespace std; 

int fact[] = {1, 1, 2, 6, 24, 120}; //(i!)=factory[i]; 
int cantor(int perm[], int n) {
	int ans = 0;
	for (int i = 0; i < n; ++i) { //对排列的每个数
		int count = 0; //计算排列后面小于perm[i]的数 
		for (int j = i + 1; j < n; ++j) 
			if (perm[j] < perm[i]) 
				++count;
		//小于perm[i]的数放在perm[i]上，后面有fact[n-1-i]种排列可能 
		ans += count * fact[n - 1 - i]; 
	}
	return ans;
}

int main() {
//	permutation(0, 3);
	int perm[] = {2, 1, 4, 3}, perm1[] = {2, 4, 3, 1}, perm2[] = {4, 3, 2, 1};
	cout << cantor(perm, 4) << " ";
	cout << cantor(perm1, 4) << " ";
	cout << cantor(perm2, 4) << " ";
	return 0;	
}
