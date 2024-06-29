

Given a string `S` that only contains `"I"` (increase) or `"D"` (decrease), let `N = S.length` .

Return any permutation `A` of `[0, 1, ..., N]` such that for all `i = 0, ..., N-1` :
- If `S[i] == "I"` , then `A[i] < A[i+1]`
- If `S[i] == "D"` , then `A[i] > A[i+1]` 

 

**Example 1:**

```clike
Input: "IDID"
Output: [0,4,1,3,2]
```

**Example 2:**

```clike
Input: "III"
Output: [0,1,2,3]
```

**Example 3:**

```clike
Input: "DDI"
Output: [3,2,0,1]
```
**Note:**
- `1 <= S.length <= 10000`
- `S` only contains characters `"I"` or `"D"` .


题意：给定只含有 `I, D` 的字符串，`N = S.length` ，返回 `[0, 1, ..., N]` 的任意排列 `A` 使得对于所有的 `i = 0, ..., N - 1` ，都有：
- 如果 `S[i] == 'I'` ，那么 `A[i] < A[i + 1]` ；
- 如果 `S[i] == 'D'` ，那么 `A[i] > A[i + 1]` 。

---
### 解法 贪心
要求生成的是排列，意味着 `0 ~ S.length - 1` 范围内的每个数只能出现一次，即是一个**消耗性选择**的过程。当需要决定一个位置的数字时，根据的信息是“**当前位置和下一个位置的数字大小关系**”。如果要求是递增的，那么只要确保当前选择的是剩余可选范围内最小的那个，下一个位置上，在剩余的数字中不管选哪个都能保证比当前位置的更大。递减同理。

所以需要维护一个“剩余可选的数字范围”，**每次要么从中取出最小的那个，要么取出最大的那个**，然后记录和更新这个范围的最小值和最大值即可。具体代码很简单：
```cpp
class Solution {
public:
    vector<int> diStringMatch(string S) {
        int lo = 0, hi = S.size();
        vector<int> ans;
        for (const char &c : S) {
            if (c == 'I') ans.push_back(lo++);
            else ans.push_back(hi--); 
        }
        ans.push_back(lo);
        return ans;
    }
};
```
效果如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了38.80% 的用户
内存消耗：9.2 MB, 在所有 C++ 提交中击败了5.11% 的用户
```
