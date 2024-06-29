#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
	int val;
	TreeNode *left, *right;
	TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};
namespace TraverseRecur {
	void preorder(TreeNode *root) {
		if (root == nullptr) return;
		printf("%d ", root->val);
		preorder(root->left);
		preorder(root->right);
	}
	void inorder(TreeNode *root) {
		if (root == nullptr) return;
		inorder(root->left);
		printf("%d ", root->val);
		inorder(root->right);
	}
	void postorder(TreeNode *root) {
		if (root == nullptr) return;
		postorder(root->left);
		postorder(root->right);
		printf("%d ", root->val);
	}
}
/*
   	 1
   /   \
  2     3
/  \   / \
4   5 6   7
	 /
    8
    先序: 1 2 4 5 3 6 8 7
	中序：4 2 5 1 8 6 3 7
	后序：4 5 2 8 6 7 3 1
非递归肯定要使用栈结构。但是使用栈也有不同的方法，下面介绍两种：
第一种通用性不高（相比第二种可以轻松的改成适应中序遍历和后序遍历的代码）
*/
namespace TraverseIterMy {
	void preorder(TreeNode *root) {
		if (root == nullptr) return;
		stack<TreeNode*> st;
		//非递归先序遍历
		st.push(root);
		while (!st.empty()) {
			TreeNode *t = st.top(); st.pop();
			printf("%d ", t->val);
			if (t->right) st.push(t->right); //是为了后访问右子树 
			if (t->left) st.push(t->left);
		}
	}
	void inorder(TreeNode *root) {
		if (root == nullptr) return;
		stack<TreeNode*> st;
		//非递归中序遍历
		st.push(root);
//		while (!st.empty()) {
//			TreeNode *t = st.top(); st.pop();
//			if (t->right) st.push(t->right);
//			
//		}
	}
	/*
二叉树后序遍历：双栈写法
后序遍历的遍历顺序是左右根。我们是否可以从我们熟悉且简单的先序遍历转化过去后序遍历呢？
答案是可以的。我们可以观察到，可以先求出遍历顺序是根右左的节点序列，再倒序，便刚好是后序遍历的顺序：左右根。而遍历顺序是根右左的话，很好办，从先序遍历的代码中改两行就是了。
所以我们可以选用两个栈，一个用于根右左遍历，一个用于保存序列，最后的代码和前序遍历只是稍作几点修改即可。
	*/ 
	//先序遍历是根-左-右,后序遍历是左-右-根,于是先序遍历改为根-右-左
	//然后翻转就是后序遍历的顺序 
	void postorder(TreeNode *root) {
		if (root == nullptr) return;
		stack<TreeNode*> st;
		vector<int> order;
		st.push(root);
		while (!st.empty()) {
			TreeNode *t = st.top(); st.pop();
			order.push_back(t->val); //根
			//与先序遍历相反
			if (t->left) st.push(t->left); //是为了后访问左子树 
			if (t->right) st.push(t->right);
		}
		for (int i = order.size() - 1; i >= 0; --i) 
			printf("%d ", order[i]);
	}
}
//颜色标记法-一种通用且简明的树遍历方法
//https://leetcode-cn.com/problems/binary-tree-inorder-traversal/solution/yan-se-biao-ji-fa-yi-chong-tong-yong-qie-jian-ming/
namespace TraverseIterEasy {
	struct Command {
		string s;
		TreeNode *node;
		Command(const string &str, TreeNode *r) :
			s(str), node(r) { }
	};
	void preorder(TreeNode *root) {
		if (root == nullptr) return;
		stack<Command> st;
		st.push(Command("go", root));
		while (!st.empty()) {
			Command command = st.top(); st.pop();
			
			if (command.s == "print")
				printf("%d ", command.node->val);
			else {
				assert(command.s == "go");
				if (command.node->right)
					st.push(Command("go", command.node->right));
				if (command.node->left)
					st.push(Command("go", command.node->left));
				st.push(Command("print", command.node));
			}
		}
	}
	void inorder(TreeNode *root) {
		if (root == nullptr) return;
		stack<Command> st;
		st.push(Command("go", root));
		while (!st.empty()) {
			Command command = st.top(); st.pop();
			
			if (command.s == "print")
				printf("%d ", command.node->val);
			else {
				assert(command.s == "go");
				if (command.node->right)
					st.push(Command("go", command.node->right));
				st.push(Command("print", command.node));
				if (command.node->left)
					st.push(Command("go", command.node->left));
			}
		}
	}
	void postorder(TreeNode *root) {
		if (root == nullptr) return;
		stack<Command> st;
		st.push(Command("go", root));
		while (!st.empty()) {
			Command command = st.top(); st.pop();
			
			if (command.s == "print")
				printf("%d ", command.node->val);
			else {
				assert(command.s == "go");
				
				st.push(Command("print", command.node));
				if (command.node->right)
					st.push(Command("go", command.node->right));
				if (command.node->left)
					st.push(Command("go", command.node->left));
			}
		}
	}
}

namespace TraverseIterBook {
void preorderNR(TreeNode *root) { //这是一个模拟系统栈的调用过程，是上面那个函数的实际运行方式
	stack<TreeNode*> st; 
	TreeNode *temp = root; //像调用preorder()的时候一样，有一个指向当前结点root的指针
	while (temp || !st.empty()) { //像调用preorder()时一样, 如果当前的root子树为空 && 系统栈的调用完全结束/stack中没有结点时就退出 
		while (temp) { //非空时不退出 
			st.push(temp); //像上面是root压入系统栈中, 递归调用preorder(root); 这里用stack存储temp, 像系统栈的调用一样 
			printf("%d ", temp->val); //同样, temp非空时打印其值 
			temp = temp->left; //像上面一样, 递归打印root->left; 这里直接挪动指针temp 
		} //temp为空退出; 是preorder(root->left), root->left为空返回来 
		temp = st.top(); st.pop(); //root->left为空返回上层调用; 这里用stack的弹出, 找到上一层 
		//如果root->right空，preorder(root->right)会直接返回原地，然后返回上上层；这里同样，temp->right为空, 
		//如果模拟系统调用栈没结束/stack中还有值，temp为空跳过while(temp), 继续弹出stack中的结点，直到有右子树为止，然后对右子树重复上面的代码过程 
		temp = temp->right; //如果root->right非空, 递归调用preorder(root->right), 新的root指向原root->right, 然后重复: 
		//先检查root非空; 然后真正进入系统栈/压入stack中, 打印其值; 然后继续对左子树递归调用/while(temp), temp=temp->left 
	}
}

void inorderNR(TreeNode *root) { //这是一个模拟系统栈的调用过程，是上面那个函数的实际运行方式
	stack<TreeNode*> st;
	TreeNode *temp = root; //像调用inorder()的时候一样，有一个指向当前结点root的指针
	while (temp || !st.empty()) {  //像调用inorder()时一样, 如果当前的root子树为空 && 系统栈的调用完全结束/stack中没有结点时就退出 
		while (temp) { //检查root为空, 退出 
			st.push(temp);     //不断压入系统栈中 
			temp = temp->left; //像上面一样, 递归对左子树调用inorder(root->left); temp=temp->left 
		} //当temp为空时, 相当于递归遍历到了root->left为空的时候, 这时候从root->left返回到root
		temp = st.top(); st.pop(); //用stack的弹出模拟系统栈的返回
		printf("%d ", temp->val); //root->left为空返回到root后, 打印root当前结点的值
		temp = temp->right; //继续对root->right进行递归调用, 新的root指向原root->right, 然后重复...至于判断root->right是否为空就是上述代码的事了:
		//先检查root非空; 然后真正进入系统栈/压入stack中; 然后继续对左子树递归调用/while(temp), temp=temp->left
	}
} 
 
void postorderNR(TreeNode *root) { //这是一个模拟系统栈的调用过程，是上面那个函数的实际运行方式
	stack<TreeNode*> st;
	TreeNode *temp = root; //像调用postorder()的时候一样，有一个指向当前结点root的指针
	while (temp || !st.empty()) { //像调用postorder()时一样, 如果当前的root子树为空 && 系统栈的调用完全结束/stack中没有结点时就退出
		while (temp) { 
			st.push(temp);      //不断压入系统栈中 
			temp = temp->left;  //像上面一样, 递归对左子树调用postorder(root->left); temp=temp->left 
		}
		//temp为空, 类似于postorder(root->left)调用为空并返回到root这一层, 此时需要递归postorder(root->right)
		TreeNode *cur = st.top(); //先回到root这一层cur=st.top() 
		temp = cur->right; //并看看右子树 
		if (temp == nullptr) { //看看这个结点是否还存在右子树
			printf("%d ", cur->val); //不存在时就说明这个结点为叶子结点, 对应的是postorder(root->left)到终点, 然后postorder(root->right)也到终点, root->right为空的时候直接打印root->val
			//打印后需要返回, 是从postorder(root->left)还是postorder(root->right)返回呢？这是个问题，也是最大的难点 
			st.pop(); //既然打印了，说明访问了这个结点，弹出，模拟系统栈的返回 
			while (!st.empty() && cur == st.top()->right) { //如果cur为父结点的右结点，就是从postorder(root->right)返回，继续打印 
				cur = st.top(); st.pop(); //左右根，父结点的左子树和右子树都访问了 
				printf("%d ", cur->val); //打印父结点 
			} //如果cur不为父结点的右结点就是左结点了，需要访问父结点的右子树后再打印父结点，就是从postorder(root->left)返回，继续调用postorder(root->left) 
		}
		//前面temp指向了temp->right; //存在右子树的时候temp就是指向了右子树（不为空），这样重复前面的循环while(temp) 
	}
}
}
/* 
这里的三个非递归函数的实现关键在于：模拟系统栈的调用方式！重点在于：理解二叉树的三种遍历方式对树的访问顺序是一样的，区别在于何时打印结点的值
- 先序是先打印，再向左递归，实在不能向左，就指向右子树递归；继续这个过程；
- 中序是一路向左递归，实在不能向左，就打印，然后指向右子树递归；继续这个过程；
- 后序也是一路向左递归，实在不能向左，就指向右子树（存在时）递归；继续这个过程； -> 当右子树递归也结束时，就打印（最先打印的一定是一个叶子结点）
  然后返回，如果当前的这个结点是父结点的右结点，那么说明要打印父结点了！ （对应从postorder(root->right)返回
            如果当前的这个结点是父结点的左结点，那么到了父结点，还需要对父结点的右子树继续最开始的（一路向左...) 那个过程 
 
   	 1
   /   \
  2     3
/  \   / \
4   5 6   7
	 /
    8
    先序: 1 2 4 5 3 6 8 7
	中序：4 2 5 1 8 6 3 7
	后序：4 5 2 8 6 7 3 1
*/

/*
Morris遍历：
- Morris遍历时间复杂度为O(n),空间复杂度为O(1)。可以通过Morris遍历完成先序、中序、后序遍历。

- 在Morris遍历中，若当前节点有左孩子，则会访问该节点两次。若无左孩子，则会访问该节点一次。遍历顺序与递归遍历类似，由在第几次访问该节点时输出决定。

- 对于有左孩子的节点，第一次访问到就输出，为先序遍历；第二次访问时输出为中序遍历。
- 后序遍历为：第二次访问到某一节点时，逆序打印其左子树的最右边界。最后单独逆序打印整棵树的右边界。

Morris遍历规则：
1. 对于当前节点cur，若cur无左孩子，则cur=cur->right。
2. 如果cur有左孩子，找到cur左子树最右节点，记为mostright。
　2.1如果mostright右孩子为空，则mostright->right=cur。cur向左子树移动。(此时为第一次访问该节点)
　2.2如果mostright右孩子为cur，则mostright->right=null。cur向右子树移动(此时为第二次访问该节点)

对于下面的二叉树：
	  1
	/  \
   2    3
  / \  / \
 4  5 6   7
访问顺序为：1　　2　　4　　2　  5　　1　　3　　6　　3　　7
先序遍历顺序：1 2 4 5 3 6 7(所有节点均第一次访问时输出)
中序遍历顺序：4 2 5 1 6 3 7(有左孩子的节点第二次访问时输出)
后序遍历顺序：4 5 2 6 7 3 1(有左孩子的节点第二次访问时，逆序输出左子树的右边界，最后加上整棵树的逆序右边界。
对于2，输出4
对于1输出5,2 
对于3，输出6
最后输出7 3 1)
*/ 
namespace TraverseMorris {
	void preorder(TreeNode *root) {
		if (root == nullptr) return;
		TreeNode *cur = root, *mostRight;
		while (cur) {
			if (cur->left == nullptr) { //没有左子树的节点 
				printf("%d ", cur->val);
				cur = cur->right; //开始改变了叶子节点的right,使其指向要回到的节点 
			} else {
				mostRight = cur->left;
				//寻找左子树最右节点
				while (mostRight->right && mostRight->right != cur) 
					mostRight = mostRight->right;
				if (mostRight->right == nullptr) {
					//此时为第一次访问该节点
					printf("%d ", cur->val);
					mostRight->right = cur; //指向要回到的节点 
					cur = cur->left;		//访问左子树 
				} else {
					//此时为第二次访问该节点
					mostRight->right = nullptr; //恢复原状
					cur = cur->right;
				}
			}
		}
	}
	void inorder(TreeNode *root) {
		if (root == nullptr) return;
		TreeNode *cur = root, *mostRight;
		while (cur) {
			if (cur->left == nullptr) { //没有左子树的节点
				printf("%d ", cur->val);
				cur = cur->right;
			} else {
				mostRight = cur->left; 
				//寻找左子树最右节点
				while (mostRight->right && mostRight->right != cur) 
					mostRight = mostRight->right;
				if (mostRight->right == nullptr) {
					//此时为第一次访问该节点
					mostRight->right = cur; //方便回到之后要访问的该节点 
					cur = cur->left; //访问左子树 
				} else {
					//此时为第二次访问该节点
					printf("%d ", cur->val);
					mostRight->right = nullptr; //恢复原状 
					cur = cur->right;
				}
			}
		}
	}
	void sout(TreeNode *root) { //逆序输出左子树的右边界
		stack<int> st;
		while (root) {
			st.push(root->val);
			root = root->right;
		}
		while (!st.empty()) {
			printf("%d ", st.top());
			st.pop();
		}
	}	
	void postorder(TreeNode *root) {
		if (root == nullptr) return;
		TreeNode *cur = root, *mostRight;
		while (cur) {
			if (cur->left == nullptr) { //没有左子树的节点 
				cur = cur->right; //先访问右子树 
			} else {
				mostRight = cur->left; 
				//寻找左子树最右节点
				while (mostRight->right && mostRight->right != cur)
					mostRight = mostRight->right;
				if (mostRight->right == nullptr) {
				 	//此时为第一次访问该节点
					mostRight->right = cur;
					cur = cur->left;
				} else {
					//此时为第二次访问该节点
					mostRight->right = nullptr;
					sout(cur->left); 
					cur = cur->right;
				}
			}
		}
		sout(root); //最后逆序输出根节点的最右边界 
	}			
}

int main() {
	TreeNode root(1), a(2), b(3), d(4), e(5), f(6), g(7), h(8);
	root.left = &a, root.right = &b;
	a.left = &d, a.right = &e;
	b.left = &f, b.right = &g;
	f.left = &h;
	cout << "---------------Traverse Recursive---------------" << endl;
	TraverseRecur::preorder(&root); cout << endl; 
	TraverseRecur::inorder(&root); cout << endl; 
	TraverseRecur::postorder(&root); cout << endl; 

	cout << "---------------Traverse My function---------------" << endl;	
	TraverseIterMy::preorder(&root); cout << endl;
	TraverseIterMy::inorder(&root); cout << endl; 
	TraverseIterMy::postorder(&root); cout << endl; 

	cout << "---------------Traverse Easy---------------" << endl;	
	TraverseIterEasy::preorder(&root); cout << endl;
	TraverseIterEasy::inorder(&root); cout << endl; 
	TraverseIterEasy::postorder(&root); cout << endl; 
	 
	cout << "---------------Traverse On book---------------" << endl;
	TraverseIterBook::preorderNR(&root); cout << endl;
	TraverseIterBook::inorderNR(&root); cout << endl;
	TraverseIterBook::postorderNR(&root); cout << endl;
	
	cout << "---------------Traverse Morris---------------" << endl;
	TraverseMorris::preorder(&root); cout << endl;
	TraverseMorris::inorder(&root); cout << endl;
	TraverseMorris::postorder(&root); cout << endl;
	return 0;
}
