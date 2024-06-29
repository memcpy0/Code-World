/*
** Problem: https://leetcode.cn/problems/apply-operations-to-make-two-strings-equal/
** Article: https://memcpy0.blog.csdn.net/article/details/
** Author: memcpy0
*/
// BEST:记忆化搜索,好写! O(n^2)时间
class Solution {
public:
    int minOperations(string s1, string s2, int x) {
        vector<int> pos;
        for (int i = 0; i < s1.size(); ++i) if (s1[i] != s2[i]) pos.push_back(i);
        int n = pos.size();
        if (n & 1) return -1; // 不同位置数是奇数
        vector<vector<int>> dp(n, vector<int>(n, -1));
        function<int(int, int)> dfs = [&](int l, int r) {
            if (l >= r) return 0;
            if (dp[l][r] != -1) return dp[l][r];
            int a = min(x, pos[r] - pos[l]) + dfs(l + 1, r - 1);
            int b = min(x, pos[l + 1] - pos[l]) + dfs(l + 2, r);
            int c = min(x, pos[r] - pos[r - 1]) + dfs(l, r - 2);
            return dp[l][r] = min(a, min(b, c));
        };
        return dfs(0, n - 1);
    }
};

