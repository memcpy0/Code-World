`Serialization` is the process of converting a data structure or object into a sequence of bits so that it can be stored in a file or memory buffer, or transmitted across a network connection link to be reconstructed later in the same or another computer environment.

Design an algorithm to serialize and deserialize a binary tree. There is no restriction on how your `serialization/deserialization` algorithm should work. You just need to ensure that a binary tree can be serialized to a string and this string can be deserialized to the original tree structure.

**Example**: 
```cpp
You may serialize the following tree:

    1
   / \
  2   3
     / \
    4   5

as "[1,2,3,null,null,4,5]"
```

**Clarification**: The above format is the same as `how LeetCode serializes a binary tree`. You do not necessarily need to follow this format, so please be creative and come up with different approaches yourself.

**Note**: Do not use class member/global/static variables to store states. Your serialize and deserialize algorithms should be stateless.


题目：序列化和反序列化一个二叉树。

思路：不一定要用 `LeetCode` 的序列化方法，可以自己写。我的做法：使用层序遍历，对每一个结点，都要序列化其左右指针(为空时字符串中是 `"null"` )。

反序列化时，先根据","分割字符串，然后层序建树。这个做法可以同样通过 `449. Serialize and Deserialize BST` 这一题。

其实，我是在做这一题的时候发现有个中等的题目 `449` ，然后发现几乎一样，就一并做了。

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
