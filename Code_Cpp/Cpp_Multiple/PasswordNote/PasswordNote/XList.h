//  C++������ƣ�201902
//
//	�б���
//  �б������ڹ��õĹ����࣬���������ά��һ����Ŀ��
//
//	Created by ZhangHua @ 2019-10   
//  Updated by ZhangPing @ 2019-10
//          ���SetItem��GetItem��DeleteItem���� 

#ifndef _XLIST_H_
#define _XLIST_H_

#include "XRecord.h"

typedef XRecord Item; // �����б��е���Ŀ����

class XList
{
public:
	const static int LISTSIZE = 1000; // �б������

private:
	Item m_entries[LISTSIZE]; // �þ�̬���鱣����Ŀ
	int m_items; // ��ǰ��Ŀ������

public:
	// ���캯��
	XList();

	// ��ʼ��
	void Initialize();

	// �ж��б�ǰ�Ƿ�Ϊ��
	bool IsEmpty();

	// �ж��б�ǰ�Ƿ�����
	bool IsFull();

	// ��ȡ�б�����Ŀ��ʵ������
	int ItemCount();

	// ���б��β�����һ����Ŀ
	bool AppendItem(Item &item);

	// ��һ�������������б��е�ÿ����Ŀ
	void Traverse(void (*pFun)(Item &item));

	// ����б�
	void Clear();

	// ���ݸ�����˳���idx����item��ֵ���б��е���Ŀ��������true
	// ���idx�����򷵻�false
	bool SetItem(int idx, Item &item);

	// ���ݸ�����˳���idx����ȡ�б��е���Ŀ����ֵ��item��������true
	// ���idx�����򷵻�false
	bool GetItem(int idx, Item &item);

	// ���ݸ�����˳���idx��ɾ���б��е���Ŀ��������true
	// ���idx�����򷵻�false
	bool DeleteItem(int idx);
};

#endif
