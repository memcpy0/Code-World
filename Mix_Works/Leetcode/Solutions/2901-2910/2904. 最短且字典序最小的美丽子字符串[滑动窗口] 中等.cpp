/*
** Problem: https://leetcode.cn/problems/shortest-and-lexicographically-smallest-beautiful-string/
** Article: https://memcpy0.blog.csdn.net/article/details/
** Author: memcpy0
*/
// BEST
class Solution {
public:
    string shortestBeautifulSubstring(string s, int k) {
        if (count(s.begin(), s.end(), '1') < k) {
            return "";
        }
        string ans = s;
        int cnt1 = 0, left = 0; // cnt1为窗口内1的个数
        for (int right = 0; right < s.length(); right++) {
            cnt1 += s[right] - '0';            
            // 当窗口中1的个数>k 或 窗口左端点为0，就缩小窗口
            while (cnt1 > k || s[left] == '0') {
                cnt1 -= s[left++] - '0';
            }
            if (cnt1 == k) {
                string t = s.substr(left, right - left + 1);
                if (t.length() < ans.length() || t.length() == ans.length() && t < ans) {
                    ans = move(t);
                }
            }
        }
        return ans;
    }
};