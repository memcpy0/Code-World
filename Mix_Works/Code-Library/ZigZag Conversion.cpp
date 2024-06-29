#include <bits/stdc++.h>
#include <string>
using namespace std;

string convert(string s, int numRows) { 
    if (s.empty() || s.size() == 1) return s;
    vector<string> ss(numRows); 
    string ans;
    int size = s.size(), idx = -1, flag = 1;
    for (int i = 0; i < size; ++i) {
        if (flag) { 
            ss[++idx] += s[i];
            if (idx + 1 >= numRows) flag = 0;
        } else {
            ss[--idx] += s[i];
            if (idx - 1 < 0) flag = 1;
        }
    }
    for (int i = 0; i < numRows; ++i) ans += ss[i];
    return ans;
} 
        
int main() {
	cout << convert("PAYPALISHIRING", 4); 
	return 0;
} 

