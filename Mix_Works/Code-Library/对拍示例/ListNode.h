#ifndef __LINKEDNODE_H_ 
#define __LINKEDNODE_H_
 
#include "String.h"
class ListNode {
public:
	String data;	//�ַ��� 
	ListNode *next;		//ָ����
	
	ListNode();
	ListNode(const String &s, ListNode *link = nullptr);
};

ListNode::ListNode() {   //����ָ����Ϊ�յĽ��
	next = nullptr;
}

ListNode::ListNode(const String &s, ListNode *link) { //����һ������Ϊdata,ָ����Ϊnext�Ľ�� 
	data = s;
	next = link;
}
#endif
