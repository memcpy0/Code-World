/*
** Problem: https://leetcode.cn/problems/add-strings/
** Article: https://memcpy0.blog.csdn.net/article/details/
** Author: memcpy0
*/
// BEST
class Solution {
    public String addStrings(String num1, String num2) {
        int c = 0;
        int i = num1.length() - 1, j = num2.length() - 1;
        var sb = new StringBuilder();
        while (i >= 0 || j >= 0 || c != 0) {
            c += i >= 0 ? (num1.charAt(i--) - '0') : 0;
            c += j >= 0 ? (num2.charAt(j--) - '0') : 0;
            sb.append(c % 10);
            c /= 10;
        }
        return sb.reverse().toString();
    }
}

// C++
class Solution {
public:
    string addStrings(string num1, string num2) {
        string ans;
        int i = num1.size() - 1, j = num2.size() - 1, c = 0;
        while (i >= 0 || j >= 0 || c != 0) {
            if (i >= 0) c += (num1[i--] - '0');
            if (j >= 0) c += (num2[j--] - '0'); 
            ans += char(c % 10 + '0'); 
            c /= 10;
        }  
        reverse(ans.begin(), ans.end());
        return ans;
    }
};