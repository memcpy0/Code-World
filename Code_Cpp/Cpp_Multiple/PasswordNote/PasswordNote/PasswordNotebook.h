//  C++������ƣ�201902
//
//	���뱾��
//  ���뱾�������ϲ��࣬���ڹ������м�¼����ɾ�Ĳ顣
//
//	Created by ZhangPing @ 2019-10   
//  Updated by ZhangPing @ 2019-10
//          ���AddRecord��SearchRecordById��DeleteRecordById��ChangeRecordById�Ⱥ���
//          ��Ӷ�д�ļ����ݵĺ���

#ifndef _XNotebook_H_
#define _XNotebook_H_

#include "XList.h"
#include "XRecord.h"
#include <fstream>
using namespace std;

class PasswordNotebook
{
private:
	void ReadData(fstream &finout); // ���ļ��ж�������
	void WriteData(fstream &finout); // ���ļ���д������
protected:
	XList m_record_list; // ���뱾�ļ�¼�б����б�����������˺�������Ϣ

public:
	// ���뱾�Ľ����ҵ���߼�����
	void Load();

	// ��ʾ���뱾����Ĺ��ܲ˵�������ȡ�û�ѡ��ִ�еĹ�����
	// ����ֵ���û�ѡ��Ĺ�������
	int Menu();

	// ��ʾȫ����¼��Ϣ
	void ShowAllRecords();
	// �����뱾������¼�¼
	bool AddRecord();
	// ���Ҽ�¼��Ϣ
	bool SearchRecordById();
	// ɾ����¼��Ϣ
	bool DeleteRecordById();
	// �޸ļ�¼��Ϣ
	bool ChangeRecordById();


};

#endif