/*
** Problem: https://leetcode.cn/problems/majority-element-ii/
** Article: https://memcpy0.blog.csdn.net/article/details/133681538
** Author: memcpy0
*/
// 超过n/k出现次数的情况
class Solution {
public:
    vector<int> majorityElement(vector<int> & nums, int k){
        vector<int> cand(k-1, INT_MIN);
        vector<int> cnt(k-1, 0);
        for(int i = 0; i < nums.size(); i++){
            bool oldcand = false;
            for(int j = 0; j < k-1; j++){
                if(cand[j] == nums[i]){
                    cnt[j]++;
                    oldcand = true;
                    break;
                }
            }
            if(!oldcand){
                bool newcand = false;
                for(int j = 0; j < k-1; j++){
                    if(cnt[j] == 0){
                        cand[j] = nums[i];
                        cnt[j] = 1;
                        newcand = true;
                        break;
                    }
                }
                if(!newcand){
                    for(int j = 0; j < k-1; j++){
                        cnt[j]--;
                    }
                }
            }
        }
        vector<int> res; 
        int target = nums.size() / k;
        for(int i = 0; i < k-1; i++){
            int count = 0;
            for(int j = 0; j < nums.size(); j++){
                if(cand[i] == nums[j]){
                    count++;
                }
            }
            if(count > target){
                res.push_back(cand[i]); 
            }
        }
        return res;
    }
    vector<int> majorityElement(vector<int>& nums) {
        return majorityElement(nums, 3);    
    }
};