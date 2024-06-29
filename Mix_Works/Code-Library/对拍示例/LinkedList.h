#ifndef __LINKEDLIST_H_
#define __LINKEDLIST_H_

#include <iostream>

#include "ListNode.h"      						//�����
#include "String.h" 							//�ַ����� 

class LinkedList {
protected:
	ListNode *head; 	   						//ͷ���ָ��
	int _size;		   							//�������� 
	ListNode* reverseList(ListNode *head); 	  	//������ת����
	ListNode* reverseListRec(ListNode *head); 	//�ݹ鷭ת����
public:
	LinkedList();								//���캯�� 
	~LinkedList();								//�������� 
	
	int size() const;							//���ص�������
	bool empty() const; 						//�жϵ������Ƿ�Ϊ��
	
	void orderlyInsert(const String &s);		//�����������
	void reverse();								//�����õĹ����ӿ�
	void remove(int k);							//ɾ����K�����
	void removeFirst();							//ɾ����1�����
	void removeLast();							//ɾ�����һ�����
	void printList();							//��ӡ���������������
};

//�����������ʵ�ֲ���
ListNode* LinkedList::reverseList(ListNode *head) { 	  	//������ת����
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
ListNode* LinkedList::reverseListRec(ListNode *head) {		//�ݹ鷭ת����
	if (head == nullptr || head->next == nullptr) return head;
	ListNode *newHead = reverseListRec(head->next);
	head->next->next = head;
	head->next = nullptr;
	return newHead;
}

LinkedList::LinkedList() {						//����յ�ͷ���
	head = new ListNode();			
	_size = 0;
}
LinkedList::~LinkedList() {						//�������� 
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
	while (pre->next != nullptr && pre->next->data < str) {	//ֱ������ĩβ����str<=pre->next->dataʱ
		pre = pre->next;
	}
	pre->next = new ListNode(str, pre->next);
	++_size;
}

void LinkedList::reverse() { 
	head->next = reverseList(head->next); //�ı�����ͷ���֮��������򼴿�
	//head->next = reverseListRec(head->next);
}

void LinkedList::remove(int k) {
	if (k < 1 || k > _size) return;
	ListNode *p = head, *q;
	for (int i = 1; i < k; ++i) p = p->next;
	
	q = p->next; //Ҫɾ���Ľ��  
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
