#include <bits/stdc++.h>
using namespace std;

namespace Strstr {
class Solution {
public:
    int strStr(string haystack, string needle) { 
        int i = 0, j = 0, n = haystack.size(), m = needle.size();
	    while (i < n && j < m) {
            if (haystack[i] == needle[j]) {
                ++i;
                ++j;
            } else {
                i = i - j + 1;
                j = 0;
            }
        }
        return j >= m ? i - j : -1;
    }
    int strStr(string haystack, string needle) {
        int i = 0, j = 0, m = haystack.size(), n = needle.size();
        size_t pos = haystack.find(needle);
        return pos == string::npos ? -1 : pos; 
    }
    //Robin KarpÀ„∑® 
    int hash(const string &s, int b, int e) { 
        int code = 0;
        for (int i = b; i <= e; ++i) code += (s[i] - 'a');
        return code;
    }
    bool strCmp(const string &a, int i, const string &b) {
        int j = 0, len = b.size();
        while (j < len  && a[i++] == b[j++]);
        return j >= len;
    } 
    int strStr(string haystack, string needle) {
        int m = haystack.size(), n = needle.size();
        if (m < n) return -1;
        
        int scode = hash(haystack, 0, n - 1);
        int pcode = hash(needle, 0, n - 1);
        
        for (int i = 0; i <= m - n; ++i) { 
            if (scode == pcode && strCmp(haystack, i, needle)) return i;
            if (i < m - n) scode += (haystack[i + n] - haystack[i]);
        }
        return -1;
    }   
	//SundayÀ„∑®
	int strStr(string haystack, string needle) {
        int n = haystack.size(), m = needle.size();
        if (n < m) return -1;
        bitset<30> bst;
        for (const char c : needle) bst.set(c - 'a');
        int i = 0, j = 0;
        while (i < n && j < m) {
            if (haystack[i] == needle[j]) {
                ++i; ++j;
                continue;
            }
            int nextPos = i - j + m;
            if (nextPos >= n) return -1;
            if (bst[haystack[nextPos] - 'a']) i = i - j + 1; 
            else i = nextPos + 1;
            j = 0;
        }
        return j >= m ? i - j : -1;
    }   

};

}

int main() {
    freopen(".in","r",stdin);
    freopen(".out","w",stdout);

    return 0;
}

