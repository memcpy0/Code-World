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
    ����: 1 2 4 5 3 6 8 7
	����4 2 5 1 8 6 3 7
	����4 5 2 8 6 7 3 1
�ǵݹ�϶�Ҫʹ��ջ�ṹ������ʹ��ջҲ�в�ͬ�ķ���������������֣�
��һ��ͨ���Բ��ߣ���ȵڶ��ֿ������ɵĸĳ���Ӧ��������ͺ�������Ĵ��룩
*/
namespace TraverseIterMy {
	void preorder(TreeNode *root) {
		if (root == nullptr) return;
		stack<TreeNode*> st;
		//�ǵݹ��������
		st.push(root);
		while (!st.empty()) {
			TreeNode *t = st.top(); st.pop();
			printf("%d ", t->val);
			if (t->right) st.push(t->right); //��Ϊ�˺���������� 
			if (t->left) st.push(t->left);
		}
	}
	void inorder(TreeNode *root) {
		if (root == nullptr) return;
		stack<TreeNode*> st;
		//�ǵݹ��������
		st.push(root);
//		while (!st.empty()) {
//			TreeNode *t = st.top(); st.pop();
//			if (t->right) st.push(t->right);
//			
//		}
	}
	/*
���������������˫ջд��
��������ı���˳�������Ҹ��������Ƿ���Դ�������Ϥ�Ҽ򵥵��������ת����ȥ��������أ�
���ǿ��Եġ����ǿ��Թ۲쵽���������������˳���Ǹ�����Ľڵ����У��ٵ��򣬱�պ��Ǻ��������˳�����Ҹ���������˳���Ǹ�����Ļ����ܺð죬����������Ĵ����и����о����ˡ�
�������ǿ���ѡ������ջ��һ�����ڸ����������һ�����ڱ������У����Ĵ����ǰ�����ֻ�����������޸ļ��ɡ�
	*/ 
	//��������Ǹ�-��-��,�����������-��-��,�������������Ϊ��-��-��
	//Ȼ��ת���Ǻ��������˳�� 
	void postorder(TreeNode *root) {
		if (root == nullptr) return;
		stack<TreeNode*> st;
		vector<int> order;
		st.push(root);
		while (!st.empty()) {
			TreeNode *t = st.top(); st.pop();
			order.push_back(t->val); //��
			//����������෴
			if (t->left) st.push(t->left); //��Ϊ�˺���������� 
			if (t->right) st.push(t->right);
		}
		for (int i = order.size() - 1; i >= 0; --i) 
			printf("%d ", order[i]);
	}
}
//��ɫ��Ƿ�-һ��ͨ���Ҽ���������������
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
void preorderNR(TreeNode *root) { //����һ��ģ��ϵͳջ�ĵ��ù��̣��������Ǹ�������ʵ�����з�ʽ
	stack<TreeNode*> st; 
	TreeNode *temp = root; //�����preorder()��ʱ��һ������һ��ָ��ǰ���root��ָ��
	while (temp || !st.empty()) { //�����preorder()ʱһ��, �����ǰ��root����Ϊ�� && ϵͳջ�ĵ�����ȫ����/stack��û�н��ʱ���˳� 
		while (temp) { //�ǿ�ʱ���˳� 
			st.push(temp); //��������rootѹ��ϵͳջ��, �ݹ����preorder(root); ������stack�洢temp, ��ϵͳջ�ĵ���һ�� 
			printf("%d ", temp->val); //ͬ��, temp�ǿ�ʱ��ӡ��ֵ 
			temp = temp->left; //������һ��, �ݹ��ӡroot->left; ����ֱ��Ų��ָ��temp 
		} //tempΪ���˳�; ��preorder(root->left), root->leftΪ�շ����� 
		temp = st.top(); st.pop(); //root->leftΪ�շ����ϲ����; ������stack�ĵ���, �ҵ���һ�� 
		//���root->right�գ�preorder(root->right)��ֱ�ӷ���ԭ�أ�Ȼ�󷵻����ϲ㣻����ͬ����temp->rightΪ��, 
		//���ģ��ϵͳ����ջû����/stack�л���ֵ��tempΪ������while(temp), ��������stack�еĽ�㣬ֱ����������Ϊֹ��Ȼ����������ظ�����Ĵ������ 
		temp = temp->right; //���root->right�ǿ�, �ݹ����preorder(root->right), �µ�rootָ��ԭroot->right, Ȼ���ظ�: 
		//�ȼ��root�ǿ�; Ȼ����������ϵͳջ/ѹ��stack��, ��ӡ��ֵ; Ȼ��������������ݹ����/while(temp), temp=temp->left 
	}
}

void inorderNR(TreeNode *root) { //����һ��ģ��ϵͳջ�ĵ��ù��̣��������Ǹ�������ʵ�����з�ʽ
	stack<TreeNode*> st;
	TreeNode *temp = root; //�����inorder()��ʱ��һ������һ��ָ��ǰ���root��ָ��
	while (temp || !st.empty()) {  //�����inorder()ʱһ��, �����ǰ��root����Ϊ�� && ϵͳջ�ĵ�����ȫ����/stack��û�н��ʱ���˳� 
		while (temp) { //���rootΪ��, �˳� 
			st.push(temp);     //����ѹ��ϵͳջ�� 
			temp = temp->left; //������һ��, �ݹ������������inorder(root->left); temp=temp->left 
		} //��tempΪ��ʱ, �൱�ڵݹ��������root->leftΪ�յ�ʱ��, ��ʱ���root->left���ص�root
		temp = st.top(); st.pop(); //��stack�ĵ���ģ��ϵͳջ�ķ���
		printf("%d ", temp->val); //root->leftΪ�շ��ص�root��, ��ӡroot��ǰ����ֵ
		temp = temp->right; //������root->right���еݹ����, �µ�rootָ��ԭroot->right, Ȼ���ظ�...�����ж�root->right�Ƿ�Ϊ�վ����������������:
		//�ȼ��root�ǿ�; Ȼ����������ϵͳջ/ѹ��stack��; Ȼ��������������ݹ����/while(temp), temp=temp->left
	}
} 
 
void postorderNR(TreeNode *root) { //����һ��ģ��ϵͳջ�ĵ��ù��̣��������Ǹ�������ʵ�����з�ʽ
	stack<TreeNode*> st;
	TreeNode *temp = root; //�����postorder()��ʱ��һ������һ��ָ��ǰ���root��ָ��
	while (temp || !st.empty()) { //�����postorder()ʱһ��, �����ǰ��root����Ϊ�� && ϵͳջ�ĵ�����ȫ����/stack��û�н��ʱ���˳�
		while (temp) { 
			st.push(temp);      //����ѹ��ϵͳջ�� 
			temp = temp->left;  //������һ��, �ݹ������������postorder(root->left); temp=temp->left 
		}
		//tempΪ��, ������postorder(root->left)����Ϊ�ղ����ص�root��һ��, ��ʱ��Ҫ�ݹ�postorder(root->right)
		TreeNode *cur = st.top(); //�Ȼص�root��һ��cur=st.top() 
		temp = cur->right; //������������ 
		if (temp == nullptr) { //�����������Ƿ񻹴���������
			printf("%d ", cur->val); //������ʱ��˵��������ΪҶ�ӽ��, ��Ӧ����postorder(root->left)���յ�, Ȼ��postorder(root->right)Ҳ���յ�, root->rightΪ�յ�ʱ��ֱ�Ӵ�ӡroot->val
			//��ӡ����Ҫ����, �Ǵ�postorder(root->left)����postorder(root->right)�����أ����Ǹ����⣬Ҳ�������ѵ� 
			st.pop(); //��Ȼ��ӡ�ˣ�˵�������������㣬������ģ��ϵͳջ�ķ��� 
			while (!st.empty() && cur == st.top()->right) { //���curΪ�������ҽ�㣬���Ǵ�postorder(root->right)���أ�������ӡ 
				cur = st.top(); st.pop(); //���Ҹ������������������������������� 
				printf("%d ", cur->val); //��ӡ����� 
			} //���cur��Ϊ�������ҽ����������ˣ���Ҫ���ʸ��������������ٴ�ӡ����㣬���Ǵ�postorder(root->left)���أ���������postorder(root->left) 
		}
		//ǰ��tempָ����temp->right; //������������ʱ��temp����ָ��������������Ϊ�գ��������ظ�ǰ���ѭ��while(temp) 
	}
}
}
/* 
����������ǵݹ麯����ʵ�ֹؼ����ڣ�ģ��ϵͳջ�ĵ��÷�ʽ���ص����ڣ��������������ֱ�����ʽ�����ķ���˳����һ���ģ��������ں�ʱ��ӡ����ֵ
- �������ȴ�ӡ��������ݹ飬ʵ�ڲ������󣬾�ָ���������ݹ飻����������̣�
- ������һ·����ݹ飬ʵ�ڲ������󣬾ʹ�ӡ��Ȼ��ָ���������ݹ飻����������̣�
- ����Ҳ��һ·����ݹ飬ʵ�ڲ������󣬾�ָ��������������ʱ���ݹ飻����������̣� -> ���������ݹ�Ҳ����ʱ���ʹ�ӡ�����ȴ�ӡ��һ����һ��Ҷ�ӽ�㣩
  Ȼ�󷵻أ������ǰ���������Ǹ������ҽ�㣬��ô˵��Ҫ��ӡ������ˣ� ����Ӧ��postorder(root->right)����
            �����ǰ���������Ǹ��������㣬��ô���˸���㣬����Ҫ�Ը����������������ʼ�ģ�һ·����...) �Ǹ����� 
 
   	 1
   /   \
  2     3
/  \   / \
4   5 6   7
	 /
    8
    ����: 1 2 4 5 3 6 8 7
	����4 2 5 1 8 6 3 7
	����4 5 2 8 6 7 3 1
*/

/*
Morris������
- Morris����ʱ�临�Ӷ�ΪO(n),�ռ临�Ӷ�ΪO(1)������ͨ��Morris��������������򡢺��������

- ��Morris�����У�����ǰ�ڵ������ӣ������ʸýڵ����Ρ��������ӣ������ʸýڵ�һ�Ρ�����˳����ݹ�������ƣ����ڵڼ��η��ʸýڵ�ʱ���������

- ���������ӵĽڵ㣬��һ�η��ʵ��������Ϊ����������ڶ��η���ʱ���Ϊ���������
- �������Ϊ���ڶ��η��ʵ�ĳһ�ڵ�ʱ�������ӡ�������������ұ߽硣��󵥶������ӡ���������ұ߽硣

Morris��������
1. ���ڵ�ǰ�ڵ�cur����cur�����ӣ���cur=cur->right��
2. ���cur�����ӣ��ҵ�cur���������ҽڵ㣬��Ϊmostright��
��2.1���mostright�Һ���Ϊ�գ���mostright->right=cur��cur���������ƶ���(��ʱΪ��һ�η��ʸýڵ�)
��2.2���mostright�Һ���Ϊcur����mostright->right=null��cur���������ƶ�(��ʱΪ�ڶ��η��ʸýڵ�)

��������Ķ�������
	  1
	/  \
   2    3
  / \  / \
 4  5 6   7
����˳��Ϊ��1����2����4����2��  5����1����3����6����3����7
�������˳��1 2 4 5 3 6 7(���нڵ����һ�η���ʱ���)
�������˳��4 2 5 1 6 3 7(�����ӵĽڵ�ڶ��η���ʱ���)
�������˳��4 5 2 6 7 3 1(�����ӵĽڵ�ڶ��η���ʱ������������������ұ߽磬�������������������ұ߽硣
����2�����4
����1���5,2 
����3�����6
������7 3 1)
*/ 
namespace TraverseMorris {
	void preorder(TreeNode *root) {
		if (root == nullptr) return;
		TreeNode *cur = root, *mostRight;
		while (cur) {
			if (cur->left == nullptr) { //û���������Ľڵ� 
				printf("%d ", cur->val);
				cur = cur->right; //��ʼ�ı���Ҷ�ӽڵ��right,ʹ��ָ��Ҫ�ص��Ľڵ� 
			} else {
				mostRight = cur->left;
				//Ѱ�����������ҽڵ�
				while (mostRight->right && mostRight->right != cur) 
					mostRight = mostRight->right;
				if (mostRight->right == nullptr) {
					//��ʱΪ��һ�η��ʸýڵ�
					printf("%d ", cur->val);
					mostRight->right = cur; //ָ��Ҫ�ص��Ľڵ� 
					cur = cur->left;		//���������� 
				} else {
					//��ʱΪ�ڶ��η��ʸýڵ�
					mostRight->right = nullptr; //�ָ�ԭ״
					cur = cur->right;
				}
			}
		}
	}
	void inorder(TreeNode *root) {
		if (root == nullptr) return;
		TreeNode *cur = root, *mostRight;
		while (cur) {
			if (cur->left == nullptr) { //û���������Ľڵ�
				printf("%d ", cur->val);
				cur = cur->right;
			} else {
				mostRight = cur->left; 
				//Ѱ�����������ҽڵ�
				while (mostRight->right && mostRight->right != cur) 
					mostRight = mostRight->right;
				if (mostRight->right == nullptr) {
					//��ʱΪ��һ�η��ʸýڵ�
					mostRight->right = cur; //����ص�֮��Ҫ���ʵĸýڵ� 
					cur = cur->left; //���������� 
				} else {
					//��ʱΪ�ڶ��η��ʸýڵ�
					printf("%d ", cur->val);
					mostRight->right = nullptr; //�ָ�ԭ״ 
					cur = cur->right;
				}
			}
		}
	}
	void sout(TreeNode *root) { //����������������ұ߽�
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
			if (cur->left == nullptr) { //û���������Ľڵ� 
				cur = cur->right; //�ȷ��������� 
			} else {
				mostRight = cur->left; 
				//Ѱ�����������ҽڵ�
				while (mostRight->right && mostRight->right != cur)
					mostRight = mostRight->right;
				if (mostRight->right == nullptr) {
				 	//��ʱΪ��һ�η��ʸýڵ�
					mostRight->right = cur;
					cur = cur->left;
				} else {
					//��ʱΪ�ڶ��η��ʸýڵ�
					mostRight->right = nullptr;
					sout(cur->left); 
					cur = cur->right;
				}
			}
		}
		sout(root); //�������������ڵ�����ұ߽� 
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
