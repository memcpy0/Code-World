/*
给定任意的两个长度的字符串，求所有二者的最长公共子序列
张平 2017302360010 
*/
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

string A = "ABCBDAB", B = "BDCABA";
vector<vector<int> > dp; 
vector<string> allLCS;
int lcs(const string &A, const string &B) { 
	dp.resize(A.size() + 1);  //初始化dp数组 
	int ASize = static_cast<int>(A.size()), BSize = static_cast<int>(B.size());
	for (int i = 0; i <= ASize; ++i) dp[i].resize(B.size() + 1, 0);  
	
	for (int i = 1; i <= ASize; ++i) {
		for (int j = 1; j <= BSize; ++j) {
			if (A[i - 1] == B[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1; 
			else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);  
		}
	}
	return dp[ASize][BSize];
}
//递归回溯求出所有的LCS子序列 
void dfs(int i, int j, string curStr) {  
	while (i > 0 && j > 0) {
		if (A[i - 1] == B[j - 1]) {
			curStr.push_back(A[i - 1]);
			--i;
			--j;
		} 
		else if (dp[i - 1][j] > dp[i][j - 1]) --i;
		else if (dp[i - 1][j] < dp[i][j - 1]) --j;
		else {
			dfs(i - 1, j, curStr);
			dfs(i, j - 1, curStr);
			return;
		}
	}
	reverse(curStr.begin(), curStr.end()); 
	allLCS.push_back(curStr);
}

int main() {
	cout << lcs(A, B) << endl;
	string temp;
	dfs(A.size(), B.size(), temp);
	for (const auto s : allLCS) 
		cout << s << endl; 
	return 0;
}
