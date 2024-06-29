/*
** Problem: https://leetcode.cn/problems/combination-sum
** Article: https://memcpy0.blog.csdn.net/article/details/132867097
** Author: memcpy0
*/
// 回溯+剪枝
class Solution {
public:
    vector<vector<int>> ans;
    void dfs(vector<int>& candidates, int target,  vector<int>& combine, int idx) {
        if (idx == candidates.size()) return;
        if (target == 0) {
            ans.emplace_back(combine);
            return;
        }
        // 直接跳过
        dfs(candidates, target, combine, idx + 1);
        // 选择当前数
        if (target - candidates[idx] >= 0) {
            combine.emplace_back(candidates[idx]);
            dfs(candidates, target - candidates[idx], combine, idx);
            combine.pop_back();
        }
    }
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<int> combine;
        dfs(candidates, target, combine, 0);
        return ans;
    }
};
// BEST: 回溯+剪枝+排序
class Solution {
public:
    vector<vector<int>> ans;
    void dfs(vector<int>& candidates, int target,  vector<int>& combine, int idx) { 
        if (target == 0) {
            ans.emplace_back(combine);
            return;
        }
        if (idx == candidates.size() || target - candidates[idx] < 0) return;
        // 直接跳过
        dfs(candidates, target, combine, idx + 1);
        // 选择当前数
        combine.emplace_back(candidates[idx]);
        dfs(candidates, target - candidates[idx], combine, idx);
        combine.pop_back();
    }
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<int> combine;
        sort(candidates.begin(), candidates.end());
        dfs(candidates, target, combine, 0);
        return ans;
    }
};