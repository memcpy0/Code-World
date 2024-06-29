#include <bits/stdc++.h>
using namespace std;

//从暴力匹配->发现i不用回退,j回退即可,此时用的是next[j],表示p[0:j)中最长的真前缀和真后缀相同的长度,于是这部分长度不需要比较,直接跳过
//或者原本想的是,val[j]表示p[0:j]中最长的前缀和后缀的相同的长度,这样使用有什么区别呢？ -> 右移一个,变成了next[] 
//自我子串匹配 
//nextval[]
//循环节 
//--------------------------------------------
//namespace kmp1 {
//	void getNext(const string &p, int *next) {
//	    next[0] = 0, next[1] = 0;
//	    int plen = p.size();
//	    for (int i = 1; i < plen; ++i) {
//	        int j = next[i];
//	        while (j && p[i] != p[j]) j = next[j];
//	        next[i + 1] = (p[i] == p[j]) ? j + 1 : 0;
//	    }
//	}
//	int kmp(const string &s, const string &p) {
//	    if (s.size() < p.size()) return -1;
//	    int slen = s.size(), plen = p.size(), j = 0;
//	    int *next = new int[plen + 1];
//	    getNext(p, next); 
//	    for (int i = 0; i < slen; ++i) {
//	        while (j && s[i] != p[j]) j = next[j];
//	        if (s[i] == p[j]) ++j;
//	        if (j >= plen) return i + 1 - j;
//	    }
//	    return -1;
//	}
//}
//--------------------------------------------------------

//--------------------------------------------
//public int getIndexOf(String s, String m) {
//	if (s == null || m == null || m.length() < 1 || s.length() < m.length())
//		return -1;
//	char[] ss = s.toCharArray();
//	char[] ms = m.toCharArray();
//	int si = 0, mj = 0;
//	int[] next = getNextArray(ms);
//	while (si < ss.length && mj < ms.length) {
//		if (ss[si] == ms[mj]) {
//			++si;
//			++mj;
//		} 
//		else if (next[mj] == -1) ++si;
//		else mj = next[mj];
//		//else if (j != 0) j = next[j];
//    	//else ++i;
//	}
//	return mj == ms.length ? si - mj : -1;
//}
//
//public int[] getNextArray(char[] ms) {
//	if (ms.length == 1) return new int[] {-1};
//	int[] next = new int[ms.length];
//	next[0] = -1;
//	next[1] = 0;
//	int pos = 2;
//	int cn = 0;
//	while (pos < next.length) {
//		if (ms[pos - 1] == ms[cn])
//			next[pos++] = ++cn;
//		else if (cn > 0) cn = next[cn];
//		else next[pos++] = 0;
//	}
//	return next;
//}
namespace kmp2 {
	void getNext(const string &s, int *next) {
	    next[0] = -1, next[1] = 0;
	    int cn = 0, pos = 2, len = s.size();
	    while (pos <= len) {
	        if (s[pos - 1] == s[cn]) next[pos++] = ++cn;
	        else if (cn > 0) cn = next[cn];
	        else next[pos++] = 0;
	    }
	}
	int kmp(const string &s, const string &p) {
	    if (s.size() < p.size()) return -1;
	    int slen = s.size(), plen = p.size(), i = 0, j = 0;
	    int *next = new int[plen];
	    getNext(p, next);
	    while (i < slen && j < plen) {
	        if (s[i] == p[j]) ++i, ++j;
	        else if (next[j] == -1) ++i; //暗示j = 0 
	        else j = next[j];
	    }
	    return j >= plen ? i - j : -1;
	}
	//这里优化后的函数必须配上kmp3::kmp,而不是上面的两个kmp 
	void getNextVal(const string &s, int *next) {
	    next[0] = -1; //next[1]需要重新确定 
	    int cn = -1, pos = 1, len = s.size();
	    while (pos < len) {
	        if (cn == -1 || s[pos - 1] == s[cn]) {
	        	++cn;
				if (s[pos] != s[cn]) next[pos++] = cn;
				//出现了P[pos]==P[next[pos]]==P[cn]
				//于是next[pos]=next[next[pos]]=next[cn]
				else next[pos++] = next[cn];
			}
	        else cn = next[cn];
	    }
	} 
	int kmp(const string &s, const string &p) {
	    if (s.size() < p.size()) return -1;
	    int slen = s.size(), plen = p.size();
	    int *next = new int[plen];
	    getNextVal(p, next);		
		int i = 0, j = 0;
		while (i < slen && j < plen) {
			if (j == -1 || s[i] == p[j]) {
				++i;
				++j; 
			} else j = next[j];
		}
		return j >= plen ? i - j : -1; 
	}
}
//--------------------------------------------  
namespace kmp3 { //李春保版本 
	void getNext(const string &s, int *next) {
		int len = s.size();
		next[0] = -1;
		int k = -1, j = 0;
		while (j < len - 1) {
			if (k == -1 || s[j] == s[k]) next[++j] = ++k;
			else k = next[k]; //不相等,回退开始新的模式匹配(s的前缀子串和后缀子串的匹配) 
		}
	}  
	void getNextVal(const string &s, int *next) {
		int len = s.size();
		next[0] = -1;
		int k = -1, j = 0;
		while (j < len - 1) { //s[j]表示后缀,s[k]表示前缀 
			if (k == -1 || s[j] == s[k]) {
				++j;
				++k;
				if (s[j] != s[k]) next[j] = k; //之前只有这一行 
				else next[j] = next[k];
				//因为不能出现p[j]=p[next[j]],
				//所以当出现时需要继续递归,k=next[k]=next[next[k]]
			} else k = next[k];
		}	
	}
	int kmp(const string &s, const string &p) {
		if (s.size() < p.size()) return -1;
		int slen = s.size(), plen = p.size();
		int *next = new int[plen];
		getNext(p, next);	//getNextVal(p, next);
		int i = 0, j = 0;
		while (i < slen && j < plen) {
			//如果j=-1,或者当前字符匹配成功(即S[i]==P[j]),都令i++,j++    
			if (j == -1 || s[i] == p[j]) {
				++i;
				++j;
			} else j = next[j];
			//如果j!=-1,且当前字符匹配失败(即S[i]!=P[j]), 则令i不变,j=next[j]    
			//next[j]即为j所对应的next值      
		}
		return j >= plen ? i - j : -1;
	}
}
void printNext(int *next, int len) {
	for (int i = 0; i < len; ++i) 
		printf(" %d" + !i, next[i]);	
}
int main() { 
    string s = "abab";
	int next1[10], next2[10];
	kmp2::getNextVal(s, next1);
	printNext(next1, s.size());
	cout << endl;
	kmp3::getNextVal(s, next2);
	printNext(next2, s.size());
    return 0;
}
 

//--------------------------------------------
#include <bits/stdc++.h>
using namespace std;
//暴力算法 
int bf(string s, string t, int pos) { //在s串中查找t串,从s串的pos开始
	int i = pos, j = 0, sum = 0; //sum记录比较了多少次 
	int slen = s.size(), tlen = t.size();
	while (i < slen && j < tlen) {
		++sum; //比较次数加1 
		if (s[i] == t[j]) ++i, ++j; //相等就继续比较后面的字符
		else {
			i = i - j + 1; //i回退到上一轮开始比较的下一个字符 
			j = 0; //j回退到第0个字符 
		} 
	}
	cout << "一共比较了" << sum << "次" << endl;
	if (j >= tlen) return i - tlen;
	return 0;
}

int main() {
	string s, t;
	int pos;
	cin >> s >> t >> pos;
	cout << bf(s, t, pos) << endl; 
}
//---------------------------------------------------- 
//int bruteForce(const string &source, const string &pattern) {
//	int i = 0, j = 0, n = source.size(), m = pattern.size();
//	while (i < n && j < m) {
//		if (source[i] == pattern[j]) {
//			++i;
//			++j;
//		} else {
//			i = i - j + 1;
//			j = 0;
//		}
//	}
//	return j >= m ? i - j : -1;
//}
//
//
//int hash(const string &s, int b, int e) { 
//    int code = 0;
//    for (int i = b; i <= e; ++i) code += (s[i] - 'a');
//    return code;
//}
//bool strCmp(const string &a, int i, const string &b) {
//    int j = 0;
//    while (j < b.size() && a[i++] == b[j++]);
//    return j >= b.size();
//} 
//int RabinKarp(string haystack, string needle) {
//    int m = haystack.size(), n = needle.size();
//    if (m < n) return -1;
//    
//    int scode = hash(haystack, 0, n - 1);
//    int pcode = hash(needle, 0, n - 1);
//    
//    for (int i = 0; i <= m - n; ++i) { 
//        if (scode == pcode && strCmp(haystack, i, needle)) return i;
//        if (i < m - n) scode += (haystack[i + n] - haystack[i]);
//    }
//    return -1;
//}   
//
//int Sunday(string haystack, string needle) {
//    int n = haystack.size(), m = needle.size();
//    if (n < m) return -1;
//    bitset<30> bst;
//    for (const char c : needle) bst.set(c - 'a');
//    int i = 0, j = 0;
//    while (i < n && j < m) {
//        if (haystack[i] == needle[j]) {
//            ++i; ++j;
//            continue;
//        }
//        int nextPos = i - j + m;
//        if (nextPos >= n) return -1;
//        if (bst[haystack[nextPos] - 'a']) i = i - j + 1; 
//        else i = nextPos + 1;
//        j = 0;
//    }
//    return j >= m ? i - j : -1;
//}   
//
//int main() {
//
//
//	return 0;
//}
