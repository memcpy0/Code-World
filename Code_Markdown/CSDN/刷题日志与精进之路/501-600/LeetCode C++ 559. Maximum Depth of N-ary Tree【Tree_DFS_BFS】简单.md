Given a n-ary tree, find its maximum depth.

The maximum depth is the number of nodes along the longest path from the root node down to the farthest leaf node.

*Nary-Tree input serialization is represented in their level order traversal, each group of children is separated by the null value (See examples).*

 

Example 1:
<img src="https://img-blog.csdnimg.cn/20200728010548822.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
```clike
Input: root = [1,null,3,2,4,null,5,6]
Output: 3
```

Example 2:
<img src="https://img-blog.csdnimg.cn/20200728010603506.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">

```clike
Input: root = [1,null,2,3,4,5,null,null,6,7,null,8,null,9,10,null,null,11,null,12,null,13,null,null,14]
Output: 5
```
**Constraints**:
-    The depth of the n-ary tree is less than or equal to `1000` .
-    The total number of nodes is between `[0, 10^4]` .


题意：找到N叉树的最大深度。

---
思路1：BFS。

代码：
```cpp
/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> children;
    Node() {}
    Node(int _val) {
        val = _val;
    }
    Node(int _val, vector<Node*> _children) {
        val = _val;
        children = _children;
    }
};
*/

class Solution {
public:
    int maxDepth(Node* root) {
        if (!root) return 0;
        int treeMaxDepth = 0;
        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();
            ++treeMaxDepth;
            for (int i = 0; i < size; ++i) {
                Node *temp = q.front(); q.pop();
                for (const auto &t : temp->children) q.push(t);
            }
        }
        return treeMaxDepth;
    }
};
```


执行效率：
```
执行用时：44 ms, 在所有 C++ 提交中击败了55.94% 的用户
内存消耗：11.2 MB, 在所有 C++ 提交中击败了100.00% 的用户
```

---
思路2：DFS。

代码：
```cpp
class Solution {
public:
    int maxDepth(Node* root) {
        if (!root) return 0;
        int mx = 0;
        for (const auto &r : root->children) 
            mx = max(maxDepth(r), mx); 
        return ++mx; 
    }
};
```
效率：
```
执行用时：44 ms, 在所有 C++ 提交中击败了55.94% 的用户
内存消耗：10.8 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
