#include <bits/stdc++.h>
using namespace std;
/*
����һ�ö������ĸ��ڵ㣬���������ȡ��Ӹ��ڵ㵽Ҷ�ڵ����ξ����Ľڵ㣨������Ҷ�ڵ㣩�γ�����һ��·�����·���ĳ���Ϊ������ȡ�
���磺
���������� [3,9,20,null,null,15,7]��

    3
   / \
  9  20
    /  \
   15   7

�������������� 3 �� 

���ܲ��ԣ�������ͨ�Ķ������������������н�㶼û����/������
����������ԣ�ֻ��һ����������㣬������ͷ���ΪNULLָ��
�߽���ԣ��� 

--------------------------------------------------------------------------
����һ�ö������ĸ��ڵ㣬�жϸ����ǲ���ƽ������������ĳ������������ڵ���������������������1����ô������һ��ƽ���������

ʾ�� 1:
���������� [3,9,20,null,null,15,7]
    3
   / \
  9  20
    /  \
   15   7
���� true ��

ʾ�� 2:
���������� [1,2,2,3,3,null,null,4,4]
       1
      / \
     2   2
    / \
   3   3
  / \
 4   4
���� false ��

���ƣ�
    1 <= ���Ľ����� <= 10000
    
���ܲ��ԣ�����ƽ�����������ƽ��Ķ������������������н�㶼û����/������
����������ԣ�ֻ��һ����������㣬������ͷ���ΪNULLָ��
�߽���ԣ��� 
*/ 
//�ظ�������� 
bool isBalanced(BinaryTreeNode *pRoot) {
	if (pRoot == nullptr) return true;
	int left = treeDepth(pRoot->left);
	int right = TreeDepth(pRoot->right);
	int diff = left - right;
	if (diff > 1 || diff < -1) 
		return false;
	return isBalanced(pRoot->left) && isBalanced(pRoot->right);
}
//ֻ������һ�Σ��ú�������ķ���,������������ÿ����㣬�ڱ�����һ�����֮ǰ�����Ǿ��Ѿ�������������������
//��ôֻҪ�ڱ���ÿ������ʱ���¼������ȣ�ĳһ������ȵ�������Ҷ�ӽ���·�����ȣ������ǾͿ���һ�߱���
//һ���ж�ÿ������ǲ���ƽ���
bool isBalanced(BinaryTreeNode *pRoot, int *pDepth) {
	if (pRoot == nullptr) {
		*pDepth = 0; 
		return true; 
	}
	int left, right;
	if (isBalanced(pRoot->left, &left) && isBalanced(pRoot->right, &right)) {
		int diff = left - right;
		if (diff <= 1 && diff >= -1) {
			*pDepth = 1 + max(left, right);
			return true;
		}
	}
	return false;
}
//�ú�������ķ�ʽ����һ�ö��������ڱ���ĳ���������ӽ��֮�����ǿ��Ը������������ӽ�������ж����Ƿ���ƽ���
//���õ���ǰ�������
//�������������ĸ����ʱ�����ж������ö������Ƿ�ƽ��������� 
bool isBalanced(BinaryTreeNode *pRoot) {
	int depth = 0;
	return isBalanced(pRoot, &depth);
} 
 
int main() {
    freopen(".in","r",stdin);
    freopen(".out","w",stdout);

    return 0;
}

