//  C++程序设计，201902
//
//	列表类
//
//	Created by ZhangHua @ 2019-02   
//  Updated by ZhangPing @ 2019-10
//          实现了SetItem、GetItem和DeleteItem函数

#include "XList.h"

XList::XList()
{
	Initialize();
}

void XList::Initialize()
{
	m_items = 0;
}

bool XList::IsEmpty()
{
	if (m_items == 0)
		return true;
	else
		return false;
}

bool XList::IsFull()
{
	if (m_items >= LISTSIZE)
		return true;
	else
		return false;
}

int XList::ItemCount()
{
	return m_items;
}

bool XList::AppendItem(Item &item)
{
	if (IsFull()) return false;

	m_entries[m_items] = item;
	m_items++;

	return true;
}

void XList::Traverse(void (*pFun)(Item &item))
{
	for (int i = 0; i < m_items; i++) pFun(m_entries[i]);
}

void XList::Clear()
{
	m_items = 0;
}

bool XList::SetItem(int idx, Item &item)
{ 
	if (idx < 0 || idx >= m_items) return false;

	m_entries[idx] = item;
	return true;
}

bool XList::GetItem(int idx, Item &item)
{ 
	if (idx < 0 || idx >= m_items) return false;

	item = m_entries[idx];
	return true;
}

bool XList::DeleteItem(int idx)
{ 
	if (idx < 0 || idx >= m_items) return false;

	for (int i = idx; i < m_items - 1; ++i) {
		m_entries[i + 1].SetId(i);  //修改记录id
		m_entries[i] = m_entries[i + 1];
	}
	m_items--; // 记录数-1
	return true;
}
