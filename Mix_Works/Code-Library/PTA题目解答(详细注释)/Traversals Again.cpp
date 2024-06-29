#include <bits/stdc++.h>
using namespace std;
const int MaxSize = 35;

struct BTreeNode {
    int data;
    struct BTreeNode *left;
	struct BTreeNode *right;
};
BTreeNode *newNode(int v) {
    BTreeNode *Node = new BTreeNode;
    Node->data = v;
    Node->left = Node->right = NULL;
    return Node; //返回新建结点的地址 
}

int n;
int preSqu[35], len1 = 0, inSqu[35], len2 = 0;
void Init() {
    stack<int> s;
    char order[10]; int temp;
    for (int i = 0; i < 2 * n; i++) {
        scanf("%s", order);
        if (order[1] == 'u') {
            scanf("%d", &temp);
            preSqu[len1++] = temp;
            s.push(temp);
        } else if (order[1] == 'o') {
            temp = s.top(); s.pop();
            inSqu[len2++] = temp;
        }
    }
}

BTreeNode* InorderCreateTree(int preL, int preR, int inL, int inR) {
	if (preL > preR) { //空直接返回 
		return NULL;
	}
	BTreeNode* root = newNode(preSqu[preL]);
	int k; 
	for (k = inL; k <= inR; k++) { //在中序序列中找到根结点 
		if (inSqu[k] == preSqu[preL]) break;
	} 
	int numLeft = k - inL; //左子树的结点个数 
	//左子树的先序区间[preL+1, preL+numLeft], 中序区间[inL, k-1]
	//返回左子树的根结点地址, 赋值给根结点的左指针 
	root->left = InorderCreateTree(preL+1, preL+numLeft, inL, k-1);
	//右子树的先序区间[preL+numLeft+1, preR], 中序区间[k+1, inR]
	//返回左子树的根结点地址, 赋值给根结点的左指针 
	root->right = InorderCreateTree(preL+numLeft+1, preR, k+1, inR);
	
	return root; //返回根结点地址 
}

void Postorder(BTreeNode *root) {
	if (root) {
		Postorder(root->left);
		Postorder(root->right);
		printf(--n ? "%d " : "%d", root->data);
	}	
}

int main() {
    scanf("%d", &n);
    Init();
    BTreeNode *T = InorderCreateTree(0, len1 - 1, 0, len2 - 1);
    Postorder(T);
    return 0;
}
