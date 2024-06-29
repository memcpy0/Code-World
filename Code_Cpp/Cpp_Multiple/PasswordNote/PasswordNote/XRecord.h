//  C++������ƣ�201910
//
//	���뱾����ļ�¼��
//	��¼�ඨ���¼�����ݳ�Ա�����ԣ��Ͳ������ݳ�Ա�ĺ�����������
//	Created by ZhangPing @ 2019-10   
//  Updated by ZhangPing @ 2019-10
//          ������µ����Ժ���ص�Getter��Setter����

#ifndef _XRECORD_H_
#define _XRECORD_H_

#include <string>
using std::string;

// ��Ƭ
class XRecord
{
private:
	unsigned int m_id;   // ��¼ID��ʵ���ϵ�ͬ�ڼ�¼���б��е�λ��
	string key_name;     // Ӧ��������վ��
	string m_account;    // �˺�
	string m_password; // ����
	string m_descr;        // ˵��
	string m_add_datetime; // ���ʱ��

public:
	// =======================================================
	// ˽�����Ե�Setter��Getter�������޸ĺͶ�ȡ������
	// �����XRecord�����������µ�����,����Ҫ������Ӧ��Setter��Getter����
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