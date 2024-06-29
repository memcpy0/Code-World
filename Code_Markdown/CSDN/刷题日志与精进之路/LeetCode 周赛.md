5519. 重新排列单词间的空格 

给你一个字符串 `text` ，该字符串由若干被空格包围的单词组成。每个单词由一个或者多个小写英文字母组成，并且两个单词之间至少存在一个空格。题目测试用例保证 `text` 至少包含一个单词 。

请你重新排列空格，使每对相邻单词之间的空格数目都 相等 ，并尽可能 最大化 该数目。如果不能重新平均分配所有空格，请 将多余的空格放置在字符串末尾 ，这也意味着返回的字符串应当与原 text 字符串的长度相等。

返回 重新排列空格后的字符串 。

 

示例 1：

输入：text = "  this   is  a sentence "
输出："this   is   a   sentence"
解释：总共有 9 个空格和 4 个单词。可以将 9 个空格平均分配到相邻单词之间，相邻单词间空格数为：9 / (4-1) = 3 个。

示例 2：

输入：text = " practice   makes   perfect"
输出："practice   makes   perfect "
解释：总共有 7 个空格和 3 个单词。7 / (3-1) = 3 个空格加上 1 个多余的空格。多余的空格需要放在字符串的末尾。

示例 3：

输入：text = "hello   world"
输出："hello   world"

示例 4：

输入：text = "  walks  udp package   into  bar a"
输出："walks  udp  package  into  bar  a "

示例 5：

输入：text = "a"
输出："a"

 

提示：

    1 <= text.length <= 100
    text 由小写英文字母和 ' ' 组成
    text 中至少包含一个单词


代码：
```cpp
class Solution {
public:
    void split(const string &text, vector<string> &token_list) {
        token_list.clear();
        size_t beginPos = text.find_first_not_of(' ');
        size_t endPos = text.find_first_of(' ', beginPos);
        while (beginPos < text.size()){
            token_list.push_back(text.substr(beginPos, endPos - beginPos));
            beginPos = text.find_first_not_of(' ', endPos);
            endPos = text.find_first_of(' ', beginPos);
        }
    }
    string reorderSpaces(string text) {
        int spaceCount = 0;
        for (const char &c : text) if (c == ' ') ++spaceCount; 
        vector<string> token_list;
        split(text, token_list);
        int wordCount = token_list.size(); 
        string ans;
        if (wordCount == 1) {
            ans += token_list[0];
            ans.append(spaceCount, ' ');
            return ans;
        }
        int internalSpaces = spaceCount / (wordCount - 1); 
        for (int i = 0; i < wordCount; ++i) {
            ans += token_list[i]; 
            ans.append(spaceCount - internalSpaces >= 0 ? internalSpaces : spaceCount, ' '); 
            spaceCount -= internalSpaces;
        }
        return ans;
    }
}; 
```

---
5520. 拆分字符串使唯一子字符串的数目最大 

给你一个字符串 s ，请你拆分该字符串，并返回拆分后唯一子字符串的最大数目。

字符串 s 拆分后可以得到若干 非空子字符串 ，这些子字符串连接后应当能够还原为原字符串。但是拆分出来的每个子字符串都必须是 唯一的 。

注意：子字符串 是字符串中的一个连续字符序列。

 

示例 1：

输入：s = "ababccc"
输出：5
解释：一种最大拆分方法为 ['a', 'b', 'ab', 'c', 'cc'] 。像 ['a', 'b', 'a', 'b', 'c', 'cc'] 这样拆分不满足题目要求，因为其中的 'a' 和 'b' 都出现了不止一次。

示例 2：

输入：s = "aba"
输出：2
解释：一种最大拆分方法为 ['a', 'ba'] 。

示例 3：

输入：s = "aa"
输出：1
解释：无法进一步拆分字符串。

 

提示：

    1 <= s.length <= 16

    s 仅包含小写英文字母

代码如下：
```cpp
class Solution {
public: 
    int maxSplits = 0;
    unordered_map<string, bool> record;
    void dfs(int pos, const string &s, int splits) {
        if (pos >= s.size()) {
            maxSplits = max(maxSplits, splits);
            return;
        }
        for (int len = 1; pos + len <= s.size(); ++len) {
            string str = s.substr(pos, len);
            if (record[str]) continue;
            record[str] = true;
            dfs(pos + len, s, splits + 1);
            record[str] = false;
        }
    }
    int maxUniqueSplit(string s) { 
        dfs(0, s, 0);
        return maxSplits;
    }
};
```

---
5521. 矩阵的最大非负积 

给你一个大小为 `rows x cols` 的矩阵 `grid` 。最初，你位于左上角 `(0, 0)` ，每一步，你可以在矩阵中 **向右** 或 **向下** 移动。

在从左上角 `(0, 0)` 开始到右下角 `(rows - 1, cols - 1)` 结束的所有路径中，找出具有 **最大非负积** 的路径。路径的积是沿路径访问的单元格中所有整数的乘积。

返回 最大非负积 对 <code>10<sup>9</sup> + 7</code> 取余 的结果。如果最大积为负数，则返回 `-1` 。

注意，取余是在得到最大积之后执行的。


**示例 1：**
```cpp
输入：grid = [[-1,-2,-3],
             [-2,-3,-3],
             [-3,-3,-2]]
输出：-1
解释：从 (0, 0) 到 (2, 2) 的路径中无法得到非负积，所以返回 -1
```

**示例 2：**
```cpp
输入：grid = [[1,-2,1],
             [1,-2,1],
             [3,-4,1]]
输出：8
解释：最大非负积对应的路径已经用粗体标出 (1 * 1 * -2 * -4 * 1 = 8)
```

**示例 3：**
```cpp
输入：grid = [[1, 3],
             [0,-4]]
输出：0
解释：最大非负积对应的路径已经用粗体标出 (1 * 0 * -4 = 0)
```

**示例 4：**
```cpp
输入：grid = [[ 1, 4,4,0],
             [-2, 0,0,1],
             [ 1,-1,1,1]]
输出：2
解释：最大非负积对应的路径已经用粗体标出 (1 * -2 * 1 * -1 * 1 * 1 = 2)
```

---
5522. 连通两组点的最小成本 

给你两组点，其中第一组中有 <code>size<sub>1</sub></code> 个点，第二组中有 <code>size<sub>2</sub></code> 个点，且 <code>size<sub>1</sub> >= size<sub>2</sub></code> 。

任意两点间的连接成本 `cost` 由大小为 <code>size<sub>1</sub> x size<sub>2</sub></code> 矩阵给出，其中 `cost[i][j]` 是第一组中的点 `i` 和第二组中的点 `j` 的连接成本。如果两个组中的每个点都与另一组中的一个或多个点连接，则称这两组点是连通的。换言之，第一组中的每个点必须至少与第二组中的一个点连接，且第二组中的每个点必须至少与第一组中的一个点连接。

返回连通两组点所需的最小成本。

 

**示例 1：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200920112225887.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
```cpp
输入：cost = [[15, 96], [36, 2]]
输出：17
解释：连通两组点的最佳方法是：
1--A
2--B
总成本为 17 。
```

**示例 2：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200920112252533.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
```cpp
输入：cost = [[1, 3, 5], [4, 1, 1], [1, 5, 3]]
输出：4
解释：连通两组点的最佳方法是：
1--A
2--B
2--C
3--A
最小成本为 4 。
请注意，虽然有多个点连接到第一组中的点 2 和第二组中的点 A ，但由于题目并不限制连接点的数目，所以只需要关心最低总成本。
```

**示例 3：**
```cpp
输入：cost = [[2, 5, 1], [3, 4, 7], [8, 1, 2], [6, 2, 4], [3, 8, 8]]
输出：10
```

 

**提示：**
- <code>size<sub>1</sub> == cost.length</code>
- <code>size<sub>2</sub> == cost[i].length</code>
- <code>1 <= size<sub>1</sub>, size<sub>2</sub> <= 12</code>
- <code>size<sub>1</sub> >= size<sub>2</sub></code>
-  <code>0 <= cost[i][j] <= 100



