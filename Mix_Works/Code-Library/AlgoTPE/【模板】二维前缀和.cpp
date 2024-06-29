#include <bits/stdc++.h>
using namespace std;

class NumMatrix {
public:
    vector<vector<int>> mx;
    NumMatrix(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        mx.resize(m + 1);
        for (int i = 0; i <= m; ++i)  
            mx[i].resize(n + 1); 
        //二维vector的使用要注意 
        //sumMatrix = vector<vector<int>>(rw + 1, vector<int> (cl + 1, 0));  
        //int **p;
        //p = new int*[m + 1];
        //for (int i = 0; i <= m; ++i) p[i] = new int[n + 1](); 
        for (int i = 1; i <= m; ++i) 
            for (int j = 1; j <= n; ++j) 
                mx[i][j] = mx[i - 1][j] + mx[i][j - 1] - mx[i - 1][j - 1] + matrix[i - 1][j - 1];
    }
    
    int sumRegion(int row1, int col1, int row2, int col2) {
        return mx[row2 + 1][col2 + 1] - mx[row2 + 1][col1] - mx[row1][col2 + 1] + mx[row1][col1];
    }
};
 
int main() {
	vector<vector<int>> matrix = {
  		{3, 0, 1, 4, 2},
  		{5, 6, 3, 2, 1},
  		{1, 2, 0, 1, 5},
  		{4, 1, 0, 1, 7},
  		{1, 0, 3, 0, 5}
	};
    NumMatrix* obj = new NumMatrix(matrix);
    int param_1 = obj->sumRegion(1, 1, 2, 2);
    cout << param_1 << endl;
    param_1 = obj->sumRegion(2, 1, 4, 3);
    cout << param_1 << endl;  
    param_1 = obj->sumRegion(1, 2, 2, 4);
    cout << param_1 << endl;     
	return 0;
}
