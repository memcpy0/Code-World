<p>Given a string S, we can transform every letter individually&nbsp;to be lowercase or uppercase to create another string.</p>

<p>Return <em>a list of all possible strings we could create</em>. You can return the output&nbsp;in <strong>any order</strong>.</p>

 
<p><strong>Example 1:</strong></p>

```clike
Input: S = "a1b2"
Output: ["a1b2","a1B2","A1b2","A1B2"] 
```

<p><strong>Example 2:</strong></p>

```clike
Input: S = "3z4"
Output: ["3z4","3Z4"] 
```

<p><strong>Example 3:</strong></p>

```clike
Input: S = "12345"
Output: ["12345"]
```

 

<p><strong>Example 4:</strong></p>

```clike
Input: S = "0"
Output: ["0"]
```

 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>S</code> will be a string with length between <code>1</code> and <code>12</code>.</li>
	<li><code>S</code> will consist only of letters or digits.</li>
</ul>




题意：给定一个字符串 `S` ，通过将字符串S中的每个字母转变大小写，我们可以获得一个新的字符串。返回所有可能得到的字符串集合。

---
### 解法1 迭代+位向量
先记录所有字母的位置 `idxs[]` 和个数 `m` ，然后进行二进制子集生成，每个子集都代表了对字符串 `s` 中所有字母进行的大小翻转操作。
```cpp
class Solution {
public:
    vector<string> letterCasePermutation(string s) {
        int n = s.size();
        vector<int> idxs;
        for (int i = 0; i < n; ++i) if (isalpha(s[i])) idxs.push_back(i);
        int m = idxs.size(), tot = (1 << m);
        vector<string> ans;
        for (int i = 0; i < tot; ++i) { 
            string cur = s;
            for (int j = 0, k = 0; j < m; ++j, ++k) {
                if ((i >> j) & 1) {
                    char &c = cur[idxs[k]];
                    if (islower(c)) c = toupper(c);
                    else c = tolower(c);
                }
            }
            ans.push_back(cur);
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了29.20% 的用户
内存消耗：9.1 MB, 在所有 C++ 提交中击败了95.35% 的用户
```
---
### 解法2 递归+回溯
用DFS实现回溯，`cur` 代表当前扫描到第几位，如果当前位是 `digit` ，直接加入，继续下一层递归；如果当前位是 `alpha` ，先将原形式加进去，然后下一层递归，再加对立的大/小写形式，再下一层递归。其实就是递归生成子集的写法。
```cpp
class Solution {
private:
    vector<string> ans;
    void dfs(const string& s, string& temp, int cur) {
        if (cur >= s.size()) { ans.push_back(temp); return; }
        if (isdigit(s[cur])) { //是数字
            temp.push_back(s[cur]);
            dfs(s, temp, cur + 1);
            temp.pop_back();
        } else { //添加字符与对立形式
            temp.push_back(s[cur]);
            dfs(s, temp, cur + 1);
            temp.pop_back();

            temp.push_back(islower(s[cur]) ? toupper(s[cur]) : tolower(s[cur]));
            dfs(s, temp, cur + 1);
            temp.pop_back();
        }
    }
public:
    vector<string> letterCasePermutation(string s) {
        string temp;
        dfs(s, temp, 0);
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了94.06% 的用户
内存消耗：12 MB, 在所有 C++ 提交中击败了13.96% 的用户
```
