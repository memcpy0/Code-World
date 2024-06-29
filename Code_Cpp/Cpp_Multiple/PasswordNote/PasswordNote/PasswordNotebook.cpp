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