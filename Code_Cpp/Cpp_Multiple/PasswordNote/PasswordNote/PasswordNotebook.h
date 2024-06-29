//  C++程序设计，201902
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