/*
** Problem: https://leetcode.cn/problems/sum-of-total-strength-of-wizards/
** Article: https://memcpy0.blog.csdn.net/article/details/132397568
** Author: memcpy0
*/
class Solution {
public:
    int totalStrength(vector<int>& strength) {
        const int mod = 1e9 + 7;
        int n = strength.size();
        vector<int> left(n, -1); // left[i] 为左侧严格小于 strength[i] 的最近元素位置（不存在时为 -1）
        vector<int> right(n, n); // right[i] 为右侧小于等于 strength[i] 的最近元素位置（不存在时为 n）
        stack<int> st;
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && strength[st.top()] >= strength[i]) {
                right[st.top()] = i;
                st.pop();
            }
            if (!st.empty()) left[i] = st.top();
            st.push(i);
        }
        long s = 0L; // 前缀和
        vector<int> ss(n + 2); // 前缀和的前缀和
        for (int i = 1; i <= n; ++i) {
            s += strength[i - 1];
            ss[i + 1] = (ss[i] + s) % mod; // 注意取模后，下面计算两个 ss 相减，结果可能为负
        }

        int ans = 0;
        for (int i = 0; i < n; ++i) { // [l,r]左闭右闭
            long l = left[i] + 1, r = right[i] - 1;
            long tot = ((i - l + 1) * (ss[r + 2] - ss[i + 1]) - (r - i + 1) * (ss[i + 1] - ss[l])) % mod;
            ans = (ans + strength[i] * tot) % mod; // 累加贡献
        }
        return (ans + mod) % mod; // 防止算出负数
    }
};