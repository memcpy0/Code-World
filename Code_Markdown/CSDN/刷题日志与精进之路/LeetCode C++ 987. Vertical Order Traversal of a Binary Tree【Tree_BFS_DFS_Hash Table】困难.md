<p>Given the <code>root</code> of a binary tree, calculate the <strong>vertical order traversal</strong> of the binary tree.</p>

<p>For each node at position <code>(row, col)</code>, its left and right children will be at positions <code>(row + 1, col - 1)</code> and <code>(row + 1, col + 1)</code> respectively. The root of the tree is at <code>(0, 0)</code>.</p>

<p>The <strong>vertical order traversal</strong> of a binary tree is a list of top-to-bottom orderings for each column index starting from the leftmost column and ending on the rightmost column. There may be multiple nodes in the same row and same column. In such a case, sort these nodes by their values.</p>

<p>Return <em>the <strong>vertical order traversal</strong> of the binary tree</em>.</p>
 
<p><strong>Example 1:</strong></p>
<img style="width: 431px; height: 304px;" src="https://assets.leetcode.com/uploads/2021/01/29/vtree1.jpg" alt="">

```css
Input: root = [3,9,20,null,null,15,7]
Output: [[9],[3,15],[20],[7]]
Explanation:
Column -1: Only node 9 is in this column.
Column 0: Nodes 3 and 15 are in this column in that order from top to bottom.
Column 1: Only node 20 is in this column.
Column 2: Only node 7 is in this column.
```

<p><strong>Example 2:</strong></p>
<img style="width: 512px; height: 304px;" src="https://assets.leetcode.com/uploads/2021/01/29/vtree2.jpg" alt="">

```css
Input: root = [1,2,3,4,5,6,7]
Output: [[4],[2],[1,5,6],[3],[7]]
Explanation:
Column -2: Only node 4 is in this column.
Column -1: Only node 2 is in this column.
Column 0: Nodes 1, 5, and 6 are in this column.
          1 is at the top, so it comes first.
          5 and 6 are at the same position (2, 0), so we order them by their value, 5 before 6.
Column 1: Only node 3 is in this column.
Column 2: Only node 7 is in this column.
```

 
<p><strong>Example 3:</strong></p>
<img style="width: 512px; height: 304px;" src="https://assets.leetcode.com/uploads/2021/01/29/vtree3.jpg" alt="">

```css
Input: root = [1,2,3,4,6,5,7]
Output: [[4],[2],[1,5,6],[3],[7]]
Explanation:
This case is the exact same as example 2, but with nodes 5 and 6 swapped.
Note that the solution remains the same since 5 and 6 are in the same location and should be ordered by their values.
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li>The number of nodes in the tree is in the range <code>[1, 1000]</code>.</li>
	<li><code>0 &lt;= Node.val &lt;= 1000</code></li>
</ul>

题意：给出二叉树的根结点 `root` ，设计算法计算二叉树的**垂序遍历**序列。对位于 `(row, col)` 的每个结点而言，其左右子结点分别位于 `(row + 1, col - 1)` 和 `(row + 1, col + 1)` 。树的根结点位于 `(0, 0)` 。

二叉树的**垂序遍历**从最左边的列开始直到最右边的列结束，按列索引每一列上的所有结点，形成一个按出现位置**从上到下**排序的有序列表。如果**同行同列**上有多个结点，则按结点的值**从小到大**进行排序。

---
### 解法1 BFS 
使用BFS，分别处理每一层的各列结点，注意要对同层同列上的多个结点值进行特殊处理：
```cpp
class Solution {
private:
    struct node { int col; TreeNode *root; }; //列号,树指针
public:
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        if (root == nullptr) return {};
        map<int, vector<int>> rec; //按照列号从小到大记录二叉树垂序每一列的值
        queue<node> q;
        q.push(node{0, root});
        while (!q.empty()) {
            int size = q.size();
            unordered_map<int, vector<int>> tempRec; 
            for (int i = 0; i < size; ++i) { //同层同列上的多个结点要排序以后,再加入相同列号的哈希列表里
                node temp = q.front(); q.pop();
                int curCol = temp.col;
                TreeNode *curRoot = temp.root;
                tempRec[curCol].push_back(curRoot->val); //先将同层的各个结点按照列号加入tempRec
                if (curRoot->left) q.push(node{curCol - 1, curRoot->left});
                if (curRoot->right) q.push(node{curCol + 1, curRoot->right});
            } 
            for (auto &it : tempRec) { 
                sort(it.second.begin(), it.second.end()); //对同层同列的多个结点值进行排序
                rec[it.first].insert(rec[it.first].end(), it.second.begin(), it.second.end()); //按照列号分别插入到rec中
            }
        }
        vector<vector<int>> ans;
        for (auto &it : rec) ans.push_back(it.second);
        return ans;
    }
};
```
个人感觉写得太过冗长……运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：11.6 MB, 在所有 C++ 提交中击败了98.19% 的用户
```
Python对应代码如下：
```cpp
class Solution(object):
    def verticalTraversal(self, root):
        """
        :type root: TreeNode
        :rtype: List[List[int]]
        """
        if not root: return []
        d = {}
        q = [[0, root]]

        while q:
            tempd = {}
            tempq = []
            while q:
                curCol, curRoot = q.pop()
                tempd[curCol] = tempd.get(curCol, []) + [curRoot.val] # 同层同列的多个结点值
                if curRoot.left:
                    tempq.append([curCol - 1, curRoot.left])
                if curRoot.right:
                    tempq.append([curCol + 1, curRoot.right])
            for k, v in tempd.items():
                d[k] = d.get(k, []) + sorted(v) # 同层同列的结点需要先排序再加入
            q = tempq # 下一层的待处理结点列表
        
        return [d[k] for k in sorted(d.keys())]
```
运行效率如下：
```cpp
执行用时：8 ms, 在所有 Python 提交中击败了100.00% 的用户
内存消耗：13.5 MB, 在所有 Python 提交中击败了37.29% 的用户
```
考虑到树中结点数目总数在范围 `[1, 1000]` 内，列号变化范围则在 `(-1000, 1000)` 之间，因此可以用 `vector<int> rec[maxn]` 代替 `map<int, vector<int>>` 和 `unordered_map<int, vector<int>>` ：
```cpp
class Solution {
public:
    const int maxn = 2021;
    using node = pair<TreeNode*, int>;
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        if (root == nullptr) return {};
        vector<int> rec[maxn], tempRec[maxn];
        queue<node> q;
        q.push(node{root, 0});
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < maxn; ++i) tempRec[i].clear();
            for (int i = 0; i < size; ++i) {
                TreeNode *curRoot = q.front().first;
                int curCol = q.front().second;
                q.pop();
                tempRec[curCol + 1000].push_back(curRoot->val);
                if (curRoot->left) q.push(node{curRoot->left, curCol - 1});
                if (curRoot->right) q.push(node{curRoot->right, curCol + 1});
            }
            for (int i = 0; i < maxn; ++i) {
                if (!tempRec[i].empty()) {
                    sort(tempRec[i].begin(), tempRec[i].end());
                    rec[i].insert(rec[i].end(), tempRec[i].begin(), tempRec[i].end());
                }
            }
        }
        vector<vector<int>> ans;
        for (int i = 0; i < maxn; ++i) if (!rec[i].empty()) ans.push_back(rec[i]);
        return ans;
    }
};
```
然而时间效率大大降低：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了61.51% 的用户
内存消耗：11.5 MB, 在所有 C++ 提交中击败了99.28% 的用户 
```
 
---
### 解法2 DFS
运用DFS+map，可以很容易得到各列的值列表，但是碰到同层同列的多个结点值时，就有点麻烦。我们既要保证**层数越低的结点值在值列表中越靠前**，也要确保**同层同列的多个结点值按照从小到大的顺序排列**——由于 `0 <= Node.val <= 1000` ，我们扩大层数 `row` 占有的权重为 `row * 10000` ，同时令结点值的权重为值本身。这样使用multiset（因为一列中可能存在相同的值），存储 `row * 10000 + root->val` 这一加权值，即可按照规则排序。最后模 `10000` 取出对应的结点值。具体代码如下：
```cpp
class Solution { 
public:
    map<int, multiset<int>> rec;
    void dfs(TreeNode *root, int curCol, int curRow) {
        if (root == nullptr) return;
        rec[curCol].insert(curRow * 10000 + root->val); 
        dfs(root->left, curCol - 1, curRow + 1);
        dfs(root->right, curCol + 1, curRow + 1);
    }
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        dfs(root, 0, 0);
        vector<vector<int>> ans;
        for (const auto &it : rec) {
            vector<int> cur;
            for (auto t : it.second) cur.push_back(t % 10000);
            ans.push_back(cur);
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了89.93% 的用户
内存消耗：13.8 MB, 在所有 C++ 提交中击败了24.19% 的用户
```
