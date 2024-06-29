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