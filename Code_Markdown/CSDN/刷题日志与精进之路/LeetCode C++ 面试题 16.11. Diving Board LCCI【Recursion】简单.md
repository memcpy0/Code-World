You are building a diving board by placing a bunch of planks of wood end-to-end. There are two types of planks, one of length `shorter` and one of length `longer` . You must use exactly `K` planks of wood. Write a method to generate all possible lengths for the diving board.

return all lengths in **non-decreasing order**.

Example:
```cpp
Input: 
shorter = 1
longer = 2
k = 3
Output:  {3,4,5,6}
```

Note:
-    `0 < shorter <= longer`
-    `0 <= k <= 100000`

 
题意：给出两种木板，一种长，一种短。使用K块木板，求出它们形成的所有可能的长度的非降序序列。

思路1：暴力递归，每次要么选择短木板，要么选择长木板。这是不可行的，复杂度为 $\text{O(2}^\text{K})$ 。

思路2：在2^k^个选择方案中，有大量的重复。比如，以题目给出的 `example` 为例：
```
0: 短木板,长为1 ; 1:长木板,长为2
000  -> len: 3
001  -> len: 4
010  -> len: 4
011  -> len: 5
100  -> len: 4
101  -> len: 5
110  -> len: 5
111  -> len: 6
```
其中，选择 `i` 块短木板和 `k - i` 块长木板的所有方案的结果是一样的；选择 `i` 块长木板和 `k - i` 块短木板的所有方案的结果是一样的。因此，从 `0` 搜索到 `k/2` ，真正的方案结果只有 `k + 1` 种。我们可以有更快的解法。
```cpp
class Solution {
public: 
    vector<int> divingBoard(int shorter, int longer, int k) {
        vector<int> lens;
        if (k == 0) return lens; 
        set<int> temp; //为了满足题意要求的顺序
        for (int i = 0; i <= (k >> 1); ++i) { //O(k/2 * logn)
            //对短木板选择i个,长木板选择k-i个的方案都是相同的
            //对短木板选择k-i个,长木板选择i个的方案都是相同的
            temp.insert(i * shorter + (k - i) * longer);
            temp.insert(i * longer + (k - i) * shorter);
        }
        return vector<int>(temp.begin(), temp.end()); 
    }
};
```
思路3：上面用到了 `set` ，插入时的复杂度稍微高一点，完全可以不用 `set` 就实现题意。从 `0` 到 `k` ，当长木板选择 `0` 块 + 短木板选择 `k` 时总长度最小，当长木板选择 `k` 块 + 短木板选择 `0` 块时总长度最大。需要注意**木板长短相等的特殊情况**。
```cpp
class Solution {
public: 
    vector<int> divingBoard(int shorter, int longer, int k) {
        vector<int> lens;
        if (k == 0) return lens;   
        if (shorter == longer) {
            lens.push_back(shorter * k);
            return lens;
        } 
        for (int i = 0; i <= k; ++i) 
            lens.push_back(i * longer + (k - i) * shorter);
        return lens;
    }
};
```
改进：在上面的基础上，还可以进一步改进。由于有 `k + 1` 种方案，则上面的代码会有 `2 * (k + 1)` 次乘法，效率不高，可以先算出最短的长度 `minLen = k * shorter` ，然后每次 `minLen` 减去一个 `shorter` ，加上一个 `longer` 。这样更快：
```cpp
class Solution {
public: 
    vector<int> divingBoard(int shorter, int longer, int k) {
        vector<int> lens;
        if (k == 0) return lens; 
        int minLen = k * shorter;
        lens.push_back(minLen);
        if (shorter == longer) return lens;
        for (int i = 1; i <= k; ++i) 
            lens.push_back(minLen = minLen - shorter + longer);
        return lens;
    }
};
```


