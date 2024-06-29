#include <bits/stdc++.h>
using namespace std;
 
vector<int> colNote;    //��¼֮ǰ��ÿ��(��1��ʼ)���к�
vector<bool> colVis;    //��¼�к��Ƿ�ʹ�ù�
vector<vector<string>> ans;    //��¼��
string rowstr;          //��¼ÿ�е��ַ�����
int endGame;
bool check(int row, int col) {
    for (int j = 0; j < row; ++j) 
        if (abs(col - colNote[j]) == abs(row - j))  //��ǰ�����Ƿ�ɶԽ���
            return false;
    return true;
}
void dfs(int row) {
    if (row >= endGame) {
        //�õ�ÿ�е��к�
        vector<string> tmp;
        for (int i = 0; i < colNote.size(); ++i) {
            tmp.push_back(rowstr);
            tmp[i][colNote[i]] = 'Q'; //ԭ����¼�����ж���1��ʼ
        }
        ans.push_back(tmp);
        return;
    }
    for (int i = 0; i < endGame; ++i) {
        if (!colVis[i] && check(row, i)) { //��ǰ���е��к��Ƿ���Ȼ��߳ɶԽ���
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
    dfs(0); //�ӵ�0�п�ʼ
    return ans;
}
int main() {
	int n = 15;
	solveNQueens(n);
	cout << n << "�ʺ�ķ�������Ϊ" << ans.size() << endl; 
	for (auto scheme : ans) { // ans��ÿ��vector<string>����һ������ 
		for (int i = 0; i < scheme.size(); ++i) 
			cout << scheme[i] << endl;
		cout << endl; 
	} 
	return 0;
}
