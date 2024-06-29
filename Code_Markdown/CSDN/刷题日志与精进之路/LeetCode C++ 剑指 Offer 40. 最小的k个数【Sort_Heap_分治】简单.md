

输入整数数组 `arr` ，找出其中最小的 `k` 个数。例如，输入4、5、1、6、2、7、3、8这8个数字，则最小的4个数字是1、2、3、4。

 

**示例 1：**

```clike
输入：arr = [3,2,1], k = 2
输出：[1,2] 或者 [2,1]
```

**示例 2：**

```clike
输入：arr = [0,1,2,1], k = 1
输出：[0]
```
**限制：**   
- `0 <= k <= arr.length <= 10000`
 - `0 <= arr[i] <= 10000`



题意：不用说明。

---
### 解法1 `sort` 排序
```cpp
class Solution {
public:
    vector<int> getLeastNumbers(vector<int>& arr, int k) {
        sort(arr.begin(), arr.end());
        return vector<int>(arr.begin(), arr.begin() + k);
    }
};
```
提交后结果如下：
```cpp
执行用时：76 ms, 在所有 C++ 提交中击败了69.90% 的用户
内存消耗：18.8 MB, 在所有 C++ 提交中击败了63.29% 的用户
```
---
### 解法2 计数排序
```cpp
class Solution {
public:
    vector<int> getLeastNumbers(vector<int>& arr, int k) {
        if (k == 0) return {};
        int cnt[10010] = {0}, num = 0;
        for (const int &v : arr) ++cnt[v]; 
        vector<int> ans;
        for (int i = 0; i <= 10000; ++i) { 
            for (int j = 0; j < cnt[i]; ++j) {
                ans.push_back(i);
                ++num;
                if (num >= k) return ans; //前面检查了k==0时的情况
            } 
        }
        return ans;
    }
};
```
执行效率如下：
```cpp
执行用时：44 ms, 在所有 C++ 提交中击败了98.95% 的用户
内存消耗：19 MB, 在所有 C++ 提交中击败了40.40% 的用户
```

