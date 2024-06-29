#ifndef __LINKEDNODE_H_ 
#define __LINKEDNODE_H_
 
#include "String.h"
class ListNode {
public:
	String data;	//字符串 
	ListNode *next;		//指针域
	
	ListNode();
	ListNode(const String &s, ListNode *link = nullptr);
};

ListNode::ListNode() {   //构造指针域为空的结点
	next = nullptr;
}

ListNode::ListNode(const String &s, ListNode *link) { //构造一个数据为data,指针域为next的结点 
	data = s;
	next = link;
}
#endif
