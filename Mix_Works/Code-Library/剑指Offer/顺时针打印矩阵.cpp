#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> printMatrix(vector<vector<int> > matrix) {
        if (matrix.empty() || matrix[0].empty()) return vector<int>();
        int m = matrix.size(), n = matrix[0].size();
        int r = 0, c = 0, tot = n * m;
        int a[m][n];
        memset(a, 0, sizeof(a)); 
        vector<int> ans;
        a[r][c] = 1, ans.push_back(matrix[r][c]), --tot;
        while (true) {
            while (c + 1 < n && a[r][c + 1] == 0) a[r][c + 1] = 1, ans.push_back(matrix[r][++c]), --tot;
            while (r + 1 < m && a[r + 1][c] == 0) a[r + 1][c] = 1, ans.push_back(matrix[++r][c]), --tot;
            while (c - 1 >= 0 && a[r][c - 1] == 0) a[r][c - 1] = 1, ans.push_back(matrix[r][--c]), --tot;
            while (r - 1 >= 0 && a[r - 1][c] == 0) a[r - 1][c] = 1, ans.push_back(matrix[--r][c]), --tot;
            if (tot <= 0) break;
        }
        return ans;
    }
};

int main() {
	Solution s;
	vector<vector<int> > m = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
	vector<int> res = s.printMatrix(m); 
	for (int i = 0; i < res.size(); ++i) 
		cout <<res[i] << " ";
	return 0;
}
