#include <bits/stdc++.h>
using namespace std;

//void split(string &s, vector<string> &tokens, char delim = ' ') { //遇到' '可能会出现空串 
//  stringstream ss(s);
//  string temp;
//  while (getline(ss, temp, delim)) tokens.push_back(temp); //慢  
//}
//void split(const string &s, vector<string> &tokens, char delim = ' ') {
//	tokens.clear();
//	size_t beginPos = s.find_first_not_of(delim, 0); 
//	size_t lastPos = s.find(delim, beginPos);
//	while (beginPos != string::npos) {
//		tokens.emplace_back(s.substr(beginPos, lastPos - beginPos));
//		beginPos = s.find_first_not_of(delim, lastPos);
//		lastPos = s.find(delim, beginPos);
//	}
//}
//string reverseWords(string &s) {
//    if (s.empty()) return "";
//    vector<string> v;
//    split(s, v, ' ');
//    if (v.empty()) return "";
//    if (v.size() == 1) return v.back();
//    string ans(v.back());
//    for (int i = v.size() - 2; i >= 0; --i) {
//        ans += " ";
//        ans += v[i];
//    }
//    return ans;
//}

//int main() {
//	string s(" ");
//	cout << reverseWords(s);
//	return 0;
//}
//(1)	
//istream& getline (istream&  is, string& str, char delim);
//istream& getline (istream&& is, string& str, char delim);
//
//(2)	
//
//istream& getline (istream&  is, string& str);
//istream& getline (istream&& is, string& str);
//
//Get line from stream into string
//Extracts characters from is and stores them into str until the delimitation character delim is found (or the newline character, '\n', for (2)).
//
//The extraction also stops if the end of file is reached in is or if some other error occurs during the input operation.
//
//If the delimiter is found, it is extracted and discarded (i.e. it is not stored and the next input operation will begin after it).
//
//Note that any content in str before the call is replaced by the newly extracted sequence.
//
//Each extracted character is appended to the string as if its member push_back was called.
class Solution {
public:
    /**
     * @param s: the article
     * @return: the number of letters that are illegal
     */
    void split(const string &s, vector<string> &tokens, char delim = ' ') {
        tokens.clear();
    	size_t beginPos = s.find_first_not_of(delim, 0); 
    	size_t lastPos = s.find(delim, beginPos);
    	while (beginPos != string::npos) {
    		tokens.emplace_back(s.substr(beginPos, lastPos - beginPos));
    		beginPos = s.find_first_not_of(delim, lastPos);
    		lastPos = s.find(delim, beginPos);
    	}
	}
}
    int count(string &s) {
        if (s.empty()) return 0;
        vector<string> sentence;
        split(s, sentence, '.');
        for (const auto t : sentence) cout << t << endl;
        vector<vector<string>> tokens(sentence.size());
        for (int i = 0; i < sentence.size(); ++i) 
            split(sentence[i], tokens[i], ' ');
        int ans = 0;
        for (int i = 0; i < tokens.size(); ++i) {
            for (int j = 0; j < tokens[i].size(); ++j) { //每句话
                if (j == 0) {
                	cout << tokens[i][j] << endl;
                    if (islower(tokens[i][j][0])) ++ans;
                    for (int k = 1; k < tokens[i][j].size(); ++k) 
                        if (isupper(tokens[i][j][k])) ++ans;
                } else {
                	cout << tokens[i][j] << endl;
                    for (int k = 0; k < tokens[i][j].size(); ++k) {
                        if (k == 0) continue;
                        if (isupper(tokens[i][j][k])) ++ans;
                    }
                }
            }
        }
        return ans;
    }
};

int main() {
	string s = "WfaOIUZeTuQhIArgJuSgFufHBDoONlOVkKzXNwbDNXwD,EemZNuUovYHqKIaQBTZWUJinpNm,OX.DQPfHLNgedBUlGrHMgvoVw,sRicWxN.uNmULoHkMumuA mtemWcWPoUeZZdclZDYpbWY.OpAIBAVtJWfvTYzZtJowzcGizConWSUmZQHfnivsIedejNMtdiBTLfepfz,KTXTodw zNiIzFYSPuwPZLkhPkyvuxJinQHsPRfqDJGEECWhOiE.FCfexqGIpdlTTXgLvBxeUIuN.LPjQZCnH GJUlhCKDSZ";
	Solution t;
	cout << t.count(s);
	return 0;
}
