/*
** Problem: https://leetcode.cn/problems/combination-sum-ii/
** Article: https://memcpy0.blog.csdn.net/article/details/132867781
** Author: memcpy0
*/
// BEST: 排序+计数统计+回溯+剪枝
class Solution {
public:
    vector<pair<int, int>> freq;
    vector<vector<int>> ans;
    vector<int> seq;
    void dfs(int pos, int rest) {
        if (rest == 0) {
            ans.push_back(seq);
            return;
        }
        if (pos == freq.size() || rest < freq[pos].first) return;
        // 直接跳过
        dfs(pos + 1, rest);
        int most = min(rest / freq[pos].first, freq[pos].second);
        for (int i = 1; i <= most; ++i) {
            seq.push_back(freq[pos].first);
            dfs(pos + 1, rest - i * freq[pos].first);
        }
        for (int i = 1; i <= most; ++i) seq.pop_back();
    }
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        for (int num : candidates) {
            if (freq.empty() || num != freq.back().first)
                freq.emplace_back(num, 1);
            else ++freq.back().second;
        }
        dfs(0, target);
        return ans;
    }
};