> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an n-ary tree, return the *level* order traversal of its nodes' values.

*Nary-Tree input serialization is represented in their level order traversal, each group of children is separated by the null value (See examples).*

**Example 1:**
<img src="https://img-blog.csdnimg.cn/20200803124207127.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
```cpp
Input: root = [1,null,3,2,4,null,5,6]
Output: [[1],[3,2,4],[5,6]]
```
**Example 2:**
<img src="https://img-blog.csdnimg.cn/20200803124214315.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
```cpp
Input: root = [1,null,2,3,4,5,null,null,6,7,null,8,null,9,10,null,null,11,null,12,null,13,null,null,14]
Output: [[1],[2,3,4,5],[6,7,8,9,10],[11,12,13],[14]]
```
**Constraints:**
- The height of the n-ary tree is less than or equal `to 1000`
- The total number of nodes is between `[0, 10^4]`
 
### 解法1 

方法一：广度优先搜索
思路与算法

对于「层序遍历」的题目，我们一般使用广度优先搜索。在广度优先搜索的每一轮中，我们会遍历同一层的所有节点。

具体地，我们首先把根节点 root\textit{root}root 放入队列中，随后在广度优先搜索的每一轮中，我们首先记录下当前队列中包含的节点个数（记为 cnt\textit{cnt}cnt），即表示上一层的节点个数。在这之后，我们从队列中依次取出节点，直到取出了上一层的全部 cnt\textit{cnt}cnt 个节点为止。当取出节点 cur\textit{cur}cur 时，我们将 cur\textit{cur}cur 的值放入一个临时列表，再将 cur\textit{cur}cur 的所有子节点全部放入队列中。

当这一轮遍历完成后，临时列表中就存放了当前层所有节点的值。这样一来，当整个广度优先搜索完成后，我们就可以得到层序遍历的结果。

细节

需要特殊判断树为空的情况。

作者：力扣官方题解
链接：https://leetcode.cn/problems/n-ary-tree-level-order-traversal/solutions/1400623/n-cha-shu-de-ceng-xu-bian-li-by-leetcode-lxdr/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```cpp
class Solution:
    def levelOrder(self, root: 'Node') -> List[List[int]]:
        if not root:
            return []

        ans = list()
        q = deque([root])

        while q:
            cnt = len(q)
            level = list()
            for _ in range(cnt):
                cur = q.popleft()
                level.append(cur.val)
                for child in cur.children:
                    q.append(child)
            ans.append(level)

        return ans

作者：力扣官方题解
链接：https://leetcode.cn/problems/n-ary-tree-level-order-traversal/solutions/1400623/n-cha-shu-de-ceng-xu-bian-li-by-leetcode-lxdr/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

func levelOrder(root *Node) (ans [][]int) {
    if root == nil {
        return
    }
    q := []*Node{root}
    for q != nil {
        level := []int{}
        tmp := q
        q = nil
        for _, node := range tmp {
            level = append(level, node.Val)
            q = append(q, node.Children...)
        }
        ans = append(ans, level)
    }
    return
}

作者：力扣官方题解
链接：https://leetcode.cn/problems/n-ary-tree-level-order-traversal/solutions/1400623/n-cha-shu-de-ceng-xu-bian-li-by-leetcode-lxdr/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
public:
    vector<vector<int>> levelOrder(Node* root) {
        if (!root) {
            return {};
        }

        vector<vector<int>> ans;
        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            int cnt = q.size();
            vector<int> level;
            for (int i = 0; i < cnt; ++i) {
                Node* cur = q.front();
                q.pop();
                level.push_back(cur->val);
                for (Node* child: cur->children) {
                    q.push(child);
                }
            }
            ans.push_back(move(level));
        }

        return ans;
    }
};

class Solution {
    public List<List<Integer>> levelOrder(Node root) {
        if (root == null) {
            return new ArrayList<List<Integer>>();
        }

        List<List<Integer>> ans = new ArrayList<List<Integer>>();
        Queue<Node> queue = new ArrayDeque<Node>();
        queue.offer(root);

        while (!queue.isEmpty()) {
            int cnt = queue.size();
            List<Integer> level = new ArrayList<Integer>();
            for (int i = 0; i < cnt; ++i) {
                Node cur = queue.poll();
                level.add(cur.val);
                for (Node child : cur.children) {
                    queue.offer(child);
                }
            }
            ans.add(level);
        }

        return ans;
    }
}
```
思路1：N叉树层次遍历，并存储每层的结点值为一个 `vector` ，相当于按层打印。这是很基础的内容，按层次进行宽度遍历即可。

代码：
```cpp class Solution {
public:
    vector<vector<int>> levelOrder(Node* root) {
        vector<vector<int>> ans;
        if (!root) return ans;
        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();  //保存每层结点数目
            ans.emplace_back(vector<int>());
            for (int i = 0; i < size; ++i) {
                Node *t = q.front(); q.pop();
                ans.back().push_back(t->val);
                for (const auto p : t->children) q.push(p);
            }
        }   
        return ans;
    }
};
```
---
思路2：如果使用简单的宽度优先遍历，需要进行一些改进：用两个 `Node*` 变量 `last, nLast` 来表示**正在遍历的当前行的最右结点**、**下一行的最右结点**：
- 每层进行从左到右的宽度优先遍历，如果发现**遍历到的结点等于** `last` 并且**队列非空**，说明该换下个 `vector` 或者说该换行了。
- **换行**的做法是：令 `last = nLast` ，继续下一行的遍历过程；直到所有结点值都打印完。
- `nLast` 则一直跟踪BFS队列中的**最新加入的结点**，因为，**最新加入队列的结点一定是目前已经发现的下一行的最右结点**。

代码：
```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(Node* root) {
        vector<vector<int>> ans;
        if (!root) return ans;
        ans.emplace_back(vector<int>());
        Node *last = root, *nlast = nullptr;

        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            Node *t = q.front(); q.pop();
            ans.back().push_back(t->val);
            for (const auto p : t->children) {
                q.push(p);
                nlast = p; //一直跟踪最新加入队列的结点
            }
            if (t == last && !q.empty()) {
                ans.emplace_back(vector<int>());
                last = nlast;
            }
        }   
        return ans;
    }
};
```
甚至于我们可以不用 `nLast` 这个变量，只需要在遍历到的结点等于 `last` 的时候，令 `last = q.back()` —— 更新 `last` 为最新加入队列的结点指针即可。代码如下：
```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(Node* root) {
        vector<vector<int>> ans;
        if (!root) return ans;
        vector<int> v;
        Node *last = root;

        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            Node *t = q.front(); q.pop();
            v.push_back(t->val);

            for (const auto p : t->children) q.push(p);
            if (t == last) {
                if (!q.empty()) last = q.back();
                ans.emplace_back(v);
                v.clear();
            }
        }   
        return ans;
    }
};
```

---
思路3：DFS，从第 `0` 层向下深度遍历，每次如果 `depth >= ans.size()` ，就要添加一个 `vector` 进入 `ans` 。

代码：
```cpp
class Solution {
public:
    vector<vector<int>> ans;
    void DFS(const Node *root, int depth) {
        if (!root) return;
        if (ans.size() <= depth) ans.emplace_back(vector<int>());
        ans[depth].push_back(root->val);
        for (const Node *p : root->children)
            DFS(p, depth + 1);
    }

    vector<vector<int>> levelOrder(Node* root) {    
        if (!root) return ans;
        DFS(root, 0);
        return ans;
    }
};
```
