Serialization is the process of converting a data structure or object into a sequence of bits so that it can be stored in a file or memory buffer, or transmitted across a network connection link to be reconstructed later in the same or another computer environment.

Design an algorithm to serialize and deserialize **a binary search tree**. There is no restriction on how your serialization/deserialization algorithm should work. You just need to ensure that a binary search tree can be serialized to a string and this string can be deserialized to the original tree structure.

**The encoded string should be as compact as possible.**

**Note**: Do not use class member/global/static variables to store states. Your serialize and deserialize algorithms should be stateless.

题意：将一棵BST序列化，然后反序列化。

思路：我的做法，将BST当做一棵普通的二叉树，使用层序遍历，对每一个结点，都要序列化其左右指针(为空时字符串中是 `"null"` )。

反序列化时，先根据 `","` 分割字符串，然后层序建树。这个做法可以同样通过 `297. Serialize and Deserialize Binary Tree` 这一题。

虽然可以通过BST的性质，优化代码，但是没什么时间，先这样写吧。以后再更新。

代码：
```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Codec {
public: 
    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        string ans; 
        if (!root) return ans;
        queue<TreeNode*> q;
        q.push(root); 
        while (!q.empty()) {
            TreeNode *t = q.front(); q.pop();
            if (t == nullptr) { ans += ",null"; continue; } 
            if (t != root) ans += ",";
            ans += to_string(t->val);
            q.push(t->left);
            q.push(t->right);
        }   
        return ans;
    }
    void buildtree(TreeNode *& root, const vector<string> &tokens) { //树非空
        int val = stoi(tokens[0]), idx = 1;
        root = new TreeNode(val);
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode *t = q.front(); q.pop(); 
            if (tokens[idx] == "null") ++idx;
            else {
                val = stoi(tokens[idx++]);  
                t->left = new TreeNode(val);
                q.push(t->left);
            }  
            if (tokens[idx] == "null") ++idx;
            else { 
                val = stoi(tokens[idx++]);   
                t->right = new TreeNode(val);
                q.push(t->right);
            }
        } 
    }

    void split(const string &s, vector<string> &tokens, const char &delim = ' ') {
        tokens.clear();
        size_t lastPos = s.find_first_not_of(delim, 0);
        size_t pos = s.find(delim, lastPos);
        while (lastPos != string::npos) {
            tokens.emplace_back(s.substr(lastPos, pos - lastPos));
            lastPos = s.find_first_not_of(delim, pos);
            pos = s.find(delim, lastPos);
        }
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        TreeNode *root = nullptr;
        if (data.empty()) return root;
        vector<string> tokens;
        split(data, tokens, ','); 
        buildtree(root, tokens);
        return root;
    }
};

// Your Codec object will be instantiated and called as such:
// Codec codec;
// codec.deserialize(codec.serialize(root));
```
