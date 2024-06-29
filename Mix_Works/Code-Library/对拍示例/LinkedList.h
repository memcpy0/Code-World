#ifndef __LINKEDLIST_H_
#define __LINKEDLIST_H_

#include <iostream>

#include "ListNode.h"      						//结点类
#include "String.h" 							//字符串类 

class LinkedList {
protected:
	ListNode *head; 	   						//头结点指针
	int _size;		   							//单链表长度 
	ListNode* reverseList(ListNode *head); 	  	//迭代翻转链表
	ListNode* reverseListRec(ListNode *head); 	//递归翻转链表
public:
	LinkedList();								//构造函数 
	~LinkedList();								//析构函数 
	
	int size() const;							//返回单链表长度
	bool empty() const; 						//判断单链表是否为空
	
	void orderlyInsert(const String &s);		//递增有序插入
	void reverse();								//链表倒置的公共接口
	void remove(int k);							//删除第K个结点
	void removeFirst();							//删除第1个结点
	void removeLast();							//删除最后一个结点
	void printList();							//打印整个单链表的数据
};

//有序单链表类的实现部分
ListNode* LinkedList::reverseList(ListNode *head) { 	  	//迭代翻转链表
	if (head == nullptr || head->next == nullptr) return head;
	ListNode *newHead = nullptr;
	while (head) {
		ListNode *t = head->next;
		head->next = newHead;
		newHead = head;
		head = t;
	}
	return newHead;
}
ListNode* LinkedList::reverseListRec(ListNode *head) {		//递归翻转链表
	if (head == nullptr || head->next == nullptr) return head;
	ListNode *newHead = reverseListRec(head->next);
	head->next->next = head;
	head->next = nullptr;
	return newHead;
}

LinkedList::LinkedList() {						//构造空的头结点
	head = new ListNode();			
	_size = 0;
}
LinkedList::~LinkedList() {						//析构函数 
	ListNode *p = head->next;
	while (p != nullptr) {
		head->next = p->next;
		delete p;
		p = head->next;
	}
	delete head;
}

inline int LinkedList::size() const {
	return _size;
}
inline bool LinkedList::empty() const {
	return !_size;
}

void LinkedList::orderlyInsert(const String &str) {
	ListNode *pre = head;
	while (pre->next != nullptr && pre->next->data < str) {	//直到链表末尾或者str<=pre->next->data时
		pre = pre->next;
	}
	pre->next = new ListNode(str, pre->next);
	++_size;
}

void LinkedList::reverse() { 
	head->next = reverseList(head->next); //改变虚拟头结点之后的链表方向即可
	//head->next = reverseListRec(head->next);
}

void LinkedList::remove(int k) {
	if (k < 1 || k > _size) return;
	ListNode *p = head, *q;
	for (int i = 1; i < k; ++i) p = p->next;
	
	q = p->next; //要删除的结点  
	p->next = q->next; 
	--_size;
	delete q;
}
void LinkedList::removeFirst() {
	if (empty()) return;
	remove(1);
}
void LinkedList::removeLast() {
	if (empty()) return;
	remove(_size);
}

void LinkedList::printList() {
	ListNode *p = head->next;
	while (p != nullptr) {
		std::cout << p->data << std::endl;
		p = p->next;
	}
}
#endif
