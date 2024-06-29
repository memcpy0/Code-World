@[toc]

## 1、开始
管理好自己的账号密码，是一件很有必要的事情。因此我写了这样一个程序。
## 2、类结构
XApp作为具体的应用程序，密码本PasswordNotebook作为上层管理类。XUtility是工具，XRecord提供了一个记录项，存储具体的账号密码等信息。XList是XRecord的列表，提供了增删改查XRecord的方法。
## 3、具体实现
应用：
```cpp
//  C++程序设计，201910
//	密码本应用程序
//  将信息存储到文本文件中 
//	Created by ZhangPing @ 2019-10    

#include "PasswordNotebook.h" 
// 密码本程序的主控函数
int main()
{
	PasswordNotebook myNotebook; // 密码本对象
	// 加载密码本，并执行账号密码管理业务
	myNotebook.Load();

	return 0;
}
```
密码本：
```cpp
//  C++程序设计，201910
//
//	密码本类
//  密码本类属于上层类，用于管理所有记录的增删改查。
//
//	Created by ZhangPing @ 2019-10   
//  Updated by ZhangPing @ 2019-10
//          添加AddRecord、SearchRecordById、DeleteRecordById、ChangeRecordById等函数
//          添加读写文件数据的函数

#ifndef _XNotebook_H_
#define _XNotebook_H_

#include "XList.h"
#include "XRecord.h"
#include <fstream>
using namespace std;

class PasswordNotebook
{
private:
	void ReadData(fstream &finout); // 从文件中读入数据
	void WriteData(fstream &finout); // 向文件中写入数据
protected:
	XList m_record_list; // 密码本的记录列表，用列表对象来保存账号密码信息

public:
	// 密码本的界面和业务逻辑控制
	void Load();

	// 显示密码本程序的功能菜单，并获取用户选择执行的功能项
	// 返回值是用户选择的功能项编号
	int Menu();

	// 显示全部记录信息
	void ShowAllRecords();
	// 往密码本中添加新记录
	bool AddRecord();
	// 查找记录信息
	bool SearchRecordById();
	// 删除记录信息
	bool DeleteRecordById();
	// 修改记录信息
	bool ChangeRecordById();


};

#endif
```

```cpp
//  C++程序设计，201910 
//	密码本程序的密码本类
//
//	Created by ZhangPing @ 2019-10   
//  Updated by ZhangPing @ 2019-10
//          实现了AddRecord、SearchRecordById、DeleteRecordById、ChangeRecordById函数
//          实现了读写文件数据的函数

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <conio.h>
using namespace std;

#include "PasswordNotebook.h"
#include "XUtility.h"

int PasswordNotebook::Menu()
{
	// 清空窗口中的内容
	system("cls");

	// 显示密码本操作界面
	std::cout << "==*==============\n";
	std::cout << "| 个人账号密码本|\n";  
	std::cout << "| [1] 显示全部  |\n";
	std::cout << "| [2] 添加记录  |\n";
	std::cout << "| [3] 查找记录  |\n";
	std::cout << "| [4] 删除记录  |\n";
	std::cout << "| [5] 修改记录  |\n";
	std::cout << "| [0] 关闭密码本|\n";
	std::cout << "=================\n";
	
	//引导用户输入要执行的操作编号
	std::cout << "已有" << m_record_list.ItemCount() << "条记录！ID从零开始！\n";
	std::cout << "请选择要执行的功能，并输入该功能的编号：\n"; 
	std::string cstr;
	std::getline(cin, cstr);
	int choice = atoi(cstr.c_str()); 

	while (choice < 0 || choice > 5)
	{
		std::cout<<"输入了无效的功能编号。\n";
		std::cout<<"请重新输入（1~5），输入0退出程序：";
		std::cin>>choice;
	}
	// 返回有效的操作编号
	return choice;
} 

// 从文本文件中读入数据
void PasswordNotebook::ReadData(fstream &finout) {
	string temp;
	while (getline(finout, temp)) { // 读入ID
		XRecord item; 
		item.SetId(stoi(temp)); 

		getline(finout, temp); // 读入应用名
		item.SetKeyname(temp); 

		getline(finout, temp); // 读入账号
		item.SetAccount(temp);  

		getline(finout, temp); // 读入密码
		item.SetPassword(temp); 

		getline(finout, temp); // 读入创建时间
		item.SetAddDatetime(temp); 

		getline(finout, temp); // 读入说明
		item.SetDescr(temp);

		m_record_list.AppendItem(item);
	}
	finout.close();
}

// 将数据写到文本文件中
void PasswordNotebook::WriteData(fstream &finout) {
	XRecord record;
	for (int i = 0; i < m_record_list.ItemCount(); ++i) {
		m_record_list.GetItem(i, record);
		finout << record.GetId() << endl  
			<< record.GetKeyname() << endl
			<< record.GetAccount() << endl
			<< record.GetPassword() << endl
			<< record.GetAddDatetime() << endl
			<< record.GetDescr() << endl;
	}
	finout.close(); // 关闭文件流
}

void PasswordNotebook::Load()
{
	fstream finout("data.txt", ios::in);
	if (!finout.good()) cout << "数据文件无法打开！请重新输入数据！" << endl;
	else ReadData(finout);

	int choice; // 保存用户选择的功能项编号
	// 密码本的操作控制循环，直到用户选择“退出”才结束
	do
	{
		// 显示菜单，获取要执行的功能项
		choice = Menu(); 
		// 根据功能项编号执行相应的操作
		switch (choice)
		{
			case 1: // [1]显示全部记录列表
			{
				std::cout << "\n选择执行[1]，显示全部记录\n";
				ShowAllRecords();
				break;
			}
			case 2: // [2]添加记录
			{
				std::cout << "\n选择执行[2]，添加记录\n";
				AddRecord();
				break;
			}
			case 3: // [3]查找记录
			{
				std::cout << "\n选择执行[3]，查找记录\n";
				SearchRecordById();
				break;
			}
			case 4: // [4]删除记录
			{
				std::cout << "\n选择执行[4]，删除记录 \n";
				DeleteRecordById();
				break;
			}
			case 5: // [5]修改记录
			{
				std::cout << "\n选择执行[5]，修改记录\n";
				ChangeRecordById();
				break;
			}
			case 0: 
			{
				finout.open("data.txt", ios::out);
				WriteData(finout);
				std::cout << "\n您的数据已保存，欢迎下次使用！再见！\n" << endl; 
				break;
			}
		}
		 // 暂停，以便用户查看操作结果
		system("pause");
	} while (choice != 0);
}

bool PasswordNotebook::AddRecord()
{
	// 定义新记录对象
	XRecord record;
	// id号内部自动分配为Item的数量
	record.SetId(m_record_list.ItemCount()); 
	
	// 设置record对象的入册时间为当前系统时间
	record.SetAddDatetime(XUtility::GetCurDateTime());
 
	// 引导用户输入新记录的其他属性值
	cout << "输入要管理账号密码的应用名称：";
	string keyname;
	getline(cin, keyname);
	record.SetKeyname(keyname);

	cout << "输入账号：";
	string account;
	getline(cin, account);
	record.SetAccount(account);

	cout << "输入密码：";
	string password;
	getline(cin, password);
	record.SetPassword(password);

	cout << "输入说明: ";
	string descr;        
	getline(cin, descr);
	record.SetDescr(descr);

	// -把新记录添加到密码本的记录列表
	m_record_list.AppendItem(record);  

	return true;
}

void showRecord(XRecord &record) {
	cout << "-------------------------------------------------------------------\n"
		<< "ID：" << record.GetId() << endl
		<< "应用：" << record.GetKeyname() << "，\n"
		<< "**账号：" << record.GetAccount() << "，\n"
		<< "**密码：" << record.GetPassword() << "，\n"
		<< "**创建时间：" << record.GetAddDatetime() << "，\n"
		<< "**说明：" << record.GetDescr() << endl
		<< "-------------------------------------------------------------------\n";
}

// 显示全部记录
void PasswordNotebook::ShowAllRecords()
{
	m_record_list.Traverse(showRecord);
}

// 通过ID查找记录信息
bool PasswordNotebook::SearchRecordById() {
	int id;
	cout << "输入要查询的记录ID：";
	cin >> id;
	getchar();
	XRecord item;
	if (m_record_list.GetItem(id, item) == true) {
		showRecord(item);
		return true;
	} else {
		cout << "你所要查询的记录不存在，请重新查找！" << endl;
		return false;
	}
}

// 通过ID删除记录信息
bool PasswordNotebook::DeleteRecordById() {
	int id;
	cout << "输入要删除的记录ID：";
	cin >> id; 
	getchar();
	if (m_record_list.DeleteItem(id) == true) {
		cout << "该记录已经成功删除！" << endl;
		return true;
	} else {
		cout << "你所要删除的记录不存在！" << endl;
		return false;
	}
}

// 通过ID修改记录信息
bool PasswordNotebook::ChangeRecordById() {
	int id;
	cout << "输入要修改的记录ID：";
	cin >> id;
	getchar();
	XRecord item;
	if (m_record_list.GetItem(id, item) == true) { 
		cout << "输入修改的内容，如某一项输入为空，表示不进行修改。" << endl;
		cout << "修改应用名称为：";
		string keyname;
		getline(cin, keyname);
		if (keyname != "") item.SetKeyname(keyname);
		
		cout << "修改账号为：";
		string account;
		getline(cin, account);
		if (account != "") item.SetAccount(account);

		cout << "修改密码为：";
		string password;
		getline(cin, password);
		if (password != "") item.SetPassword(password);

		cout << "修改说明为: ";
		string descr;
		getline(cin, descr);
		if (descr != "") item.SetDescr(descr);

		m_record_list.SetItem(id, item);
		return true;
	}
	else {
		cout << "你所要修改的记录不存在！" << endl;
		return false;
	}
}
```
工具类：
```cpp
//
//	公用工具类
//  此类提供了一组实用的功能函数。 
#ifndef _XUTILITY_H_
#define _XUTILITY_H_

#include <string>

class XUtility
{
public:
	// 获取当前系统时间，转换为字符串（格式为 YYYY-MM-DD hh:mm:ss），返回该字符串
	static std::string GetCurDateTime();
};

#endif
```

```cpp
//
//	公用工具类
//  此类提供了一组实用的功能函数。 
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

#include "XUtility.h"

string XUtility::GetCurDateTime()
{
	time_t t = time(NULL);
    char tmp[20];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %X", localtime(&t));
	return string(tmp);
}
```
记录类：
```cpp
//  C++程序设计，201910
//
//	密码本程序的记录类
//	记录类定义记录的数据成员（属性）和操作数据成员的函数（方法）
//	Created by ZhangPing @ 2019-10   
//  Updated by ZhangPing @ 2019-10
//          添加了新的属性和相关的Getter和Setter函数

#ifndef _XRECORD_H_
#define _XRECORD_H_

#include <string>
using std::string;

// 照片
class XRecord
{
private:
	unsigned int m_id;   // 记录ID，实际上等同于记录在列表中的位序
	string key_name;     // 应用名或网站名
	string m_account;    // 账号
	string m_password; // 密码
	string m_descr;        // 说明
	string m_add_datetime; // 入册时间

public:
	// =======================================================
	// 私有属性的Setter和Getter函数（修改和读取函数）
	// 如果在XRecord类中增加了新的属性,则需要增加相应的Setter和Getter函数
	// -------------------------------------------------------
	void SetId(const int id) { m_id = id; }
	int GetId() { return m_id; }

	void SetKeyname(const string &kn) { key_name = kn; }
	string GetKeyname() { return key_name; }

	void SetAccount(const string &account) { m_account = account; }
	string GetAccount() { return m_account; }

	void SetPassword(const string &password) { m_password = password; }
	string GetPassword() { return m_password; }

	void SetDescr(const string &descr) { m_descr = descr; }
	string GetDescr() { return m_descr; }

	void SetAddDatetime(const string &add_datetime) { m_add_datetime = add_datetime; }
	string GetAddDatetime() { return m_add_datetime; }
};

#endif
```

```cpp
//  C++程序设计，201910
//
//	密码本程序的记录类

#include "XRecord.h"
```
列表类：
```cpp
//  C++程序设计，201902
//
//	列表类
//  列表类属于公用的工具类，用来保存和维护一组项目。

#ifndef _XLIST_H_
#define _XLIST_H_

#include "XRecord.h"

typedef XRecord Item; // 定义列表中的项目类型

class XList
{
public:
	const static int LISTSIZE = 1000; // 列表的容量

private:
	Item m_entries[LISTSIZE]; // 用静态数组保存项目
	int m_items; // 当前项目的数量

public:
	// 构造函数
	XList();

	// 初始化
	void Initialize();

	// 判断列表当前是否为空
	bool IsEmpty();

	// 判断列表当前是否已满
	bool IsFull();

	// 获取列表中项目的实际数量
	int ItemCount();

	// 在列表的尾部添加一个项目
	bool AppendItem(Item &item);

	// 把一个函数作用于列表中的每个项目
	void Traverse(void (*pFun)(Item &item));

	// 清空列表
	void Clear();

	// 根据给定的顺序号idx，把item赋值给列表中的项目，并返回true
	// 如果idx有误，则返回false
	bool SetItem(int idx, Item &item);

	// 根据给定的顺序号idx，获取列表中的项目，赋值给item，并返回true
	// 如果idx有误，则返回false
	bool GetItem(int idx, Item &item);

	// 根据给定的顺序号idx，删除列表中的项目，并返回true
	// 如果idx有误，则返回false
	bool DeleteItem(int idx);
};

#endif
```

```cpp
//  C++程序设计，201902
//
//	列表类
//
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
```
## 4、效果
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191019204308534.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
