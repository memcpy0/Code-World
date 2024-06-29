#include <bits/stdc++.h>
using namespace std;
 
vector<int> colNote;    //记录之前的每行(以1开始)的列号
vector<bool> colVis;    //记录列号是否被使用过
vector<vector<string>> ans;    //记录答案
string rowstr;          //记录每行的字符串答案
int endGame;
bool check(int row, int col) {
    for (int j = 0; j < row; ++j) 
        if (abs(col - colNote[j]) == abs(row - j))  //和前几行是否成对角线
            return false;
    return true;
}
void dfs(int row) {
    if (row >= endGame) {
        //得到每行的列号
        vector<string> tmp;
        for (int i = 0; i < colNote.size(); ++i) {
            tmp.push_back(rowstr);
            tmp[i][colNote[i]] = 'Q'; //原来记录的行列都从1开始
        }
        ans.push_back(tmp);
        return;
    }
    for (int i = 0; i < endGame; ++i) {
        if (!colVis[i] && check(row, i)) { //和前几行的列号是否相等或者成对角线
            colNote[row] = i; 
            colVis[i] = true;
            dfs(row + 1);
            colVis[i] = false;
        }
    }
}
vector<vector<string>> solveNQueens(int n) {
    if (n == 0 || n == 2 || n == 3) return vector<vector<string>>();
    colNote.resize(n, 0);
    colVis.resize(n, false);
    endGame = n; 
    rowstr.resize(n, '.');
    dfs(0); //从第0行开始
    return ans;
}
int main() {
	int n = 15;
	solveNQueens(n);
	cout << n << "皇后的方案总数为" << ans.size() << endl; 
	for (auto scheme : ans) { // ans中每个vector<string>都是一个方案 
		for (int i = 0; i < scheme.size(); ++i) 
			cout << scheme[i] << endl;
		cout << endl; 
	} 
	return 0;
}
