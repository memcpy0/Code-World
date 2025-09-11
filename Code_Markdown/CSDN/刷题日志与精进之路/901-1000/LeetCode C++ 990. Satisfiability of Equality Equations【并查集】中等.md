Given an array `equations` of strings that represent relationships between variables, each string `equations[i]` has length $4$ and takes one of two different forms: `"a==b"` or `"a!=b"`.  Here, `a` and `b` are **lowercase** letters (not necessarily different) that represent one-letter variable names.

Return `true` if and only if it is possible to **assign integers** to variable names so as to satisfy all the given equations.

 

Example 1:

```cpp
Input: ["a==b","b!=a"]
Output: false
Explanation: If we assign say, a = 1 and b = 1, then the first equation is satisfied, but not the second.  There is no way to assign the variables to satisfy both equations.
```

Example 2:

```cpp
Input: ["b==a","a==b"]
Output: true
Explanation: We could assign a = 1 and b = 1 to satisfy both equations.
```

Example 3:

```cpp
Input: ["a==b","b==c","a==c"]
Output: true
```

Example 4:

```cpp
Input: ["a==b","b!=c","c==a"]
Output: false
```

Example 5:

```cpp
Input: ["c==c","b==d","x!=z"]
Output: true
```

 

Note:
- `1 <= equations.length <= 500`
- `equations[i].length == 4`
-  `equations[i][0]` and `equations[i][3]` are lowercase letters
-  `equations[i][1]` is either `'='` or `'!'`
-  `equations[i][2]` is `'='`


题意：可满足性问题——即是否存在一组值，可以使得一组逻辑表达式均为真。本题中限定为 `==` 和 `!=` 运算，简单很多。

思路：由于 `==` 是等价关系，因此套用并查集的板子就可以完成这道题。

代码：
```cpp
class Solution {
public:
    vector<int> father;
    int find(int x) { //路径压缩
        if (father[x] < 0) return x;
        else return father[x] = find(father[x]);
    }
    void merge(int r1, int r2) { //按大小归并
        if (father[r1] < father[r2]) {
            father[r1] += father[r2];
            father[r2] = r1;
        } else {
            father[r2] += father[r1];
            father[r1] = r2;
        }
    }
    bool equationsPossible(vector<string>& equations) {
        if (equations.empty()) return true;
        father.resize(30, -1);  
        for (const auto &s : equations) {
            int num1 = s[0] - 'a', num2 = s[3] - 'a';
            if (s[1] == '!' && num1 == num2) return false; //a!=a ; a==a不做动作
            if (s[1] == '=' && num1 != num2) { //a==b
                int r1 = find(num1), r2 = find(num2);
                if (r1 != r2) merge(r1, r2);
            } 
        } 
        for (const auto &s : equations)  
            if (s[1] == '!' && (find(s[0] - 'a') == find(s[3] - 'a'))) 
                return false;
        return true;
    }
};
```
效率：
```
执行用时 :8 ms, 在所有 C++ 提交中击败了86.19% 的用户
内存消耗 :11.1 MB, 在所有 C++ 提交中击败了50.00%的用户
```
