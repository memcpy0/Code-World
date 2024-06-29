//  C++������ƣ�201910 
//	���뱾��������뱾��
//
//	Created by ZhangPing @ 2019-10   
//  Updated by ZhangPing @ 2019-10
//          ʵ����AddRecord��SearchRecordById��DeleteRecordById��ChangeRecordById����
//          ʵ���˶�д�ļ����ݵĺ���

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
	// ��մ����е�����
	system("cls");

	// ��ʾ���뱾��������
	std::cout << "==*==============\n";
	std::cout << "| �����˺����뱾|\n";  
	std::cout << "| [1] ��ʾȫ��  |\n";
	std::cout << "| [2] ��Ӽ�¼  |\n";
	std::cout << "| [3] ���Ҽ�¼  |\n";
	std::cout << "| [4] ɾ����¼  |\n";
	std::cout << "| [5] �޸ļ�¼  |\n";
	std::cout << "| [0] �ر����뱾|\n";
	std::cout << "=================\n";
	
	//�����û�����Ҫִ�еĲ������
	std::cout << "����" << m_record_list.ItemCount() << "����¼��ID���㿪ʼ��\n";
	std::cout << "��ѡ��Ҫִ�еĹ��ܣ�������ù��ܵı�ţ�\n"; 
	std::string cstr;
	std::getline(cin, cstr);
	int choice = atoi(cstr.c_str()); 

	while (choice < 0 || choice > 5)
	{
		std::cout<<"��������Ч�Ĺ��ܱ�š�\n";
		std::cout<<"���������루1~5��������0�˳�����";
		std::cin>>choice;
	}
	// ������Ч�Ĳ������
	return choice;
} 

// ���ı��ļ��ж�������
void PasswordNotebook::ReadData(fstream &finout) {
	string temp;
	while (getline(finout, temp)) { // ����ID
		XRecord item; 
		item.SetId(stoi(temp)); 

		getline(finout, temp); // ����Ӧ����
		item.SetKeyname(temp); 

		getline(finout, temp); // �����˺�
		item.SetAccount(temp);  

		getline(finout, temp); // ��������
		item.SetPassword(temp); 

		getline(finout, temp); // ���봴��ʱ��
		item.SetAddDatetime(temp); 

		getline(finout, temp); // ����˵��
		item.SetDescr(temp);

		m_record_list.AppendItem(item);
	}
	finout.close();
}

// ������д���ı��ļ���
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
	finout.close(); // �ر��ļ���
}

void PasswordNotebook::Load()
{
	fstream finout("data.txt", ios::in);
	if (!finout.good()) cout << "�����ļ��޷��򿪣��������������ݣ�" << endl;
	else ReadData(finout);

	int choice; // �����û�ѡ��Ĺ�������
	// ���뱾�Ĳ�������ѭ����ֱ���û�ѡ���˳����Ž���
	do
	{
		// ��ʾ�˵�����ȡҪִ�еĹ�����
		choice = Menu(); 
		// ���ݹ�������ִ����Ӧ�Ĳ���
		switch (choice)
		{
			case 1: // [1]��ʾȫ����¼�б�
			{
				std::cout << "\nѡ��ִ��[1]����ʾȫ����¼\n";
				ShowAllRecords();
				break;
			}
			case 2: // [2]��Ӽ�¼
			{
				std::cout << "\nѡ��ִ��[2]����Ӽ�¼\n";
				AddRecord();
				break;
			}
			case 3: // [3]���Ҽ�¼
			{
				std::cout << "\nѡ��ִ��[3]�����Ҽ�¼\n";
				SearchRecordById();
				break;
			}
			case 4: // [4]ɾ����¼
			{
				std::cout << "\nѡ��ִ��[4]��ɾ����¼ \n";
				DeleteRecordById();
				break;
			}
			case 5: // [5]�޸ļ�¼
			{
				std::cout << "\nѡ��ִ��[5]���޸ļ�¼\n";
				ChangeRecordById();
				break;
			}
			case 0: 
			{
				finout.open("data.txt", ios::out);
				WriteData(finout);
				std::cout << "\n���������ѱ��棬��ӭ�´�ʹ�ã��ټ���\n" << endl; 
				break;
			}
		}
		 // ��ͣ���Ա��û��鿴�������
		system("pause");
	} while (choice != 0);
}

bool PasswordNotebook::AddRecord()
{
	// �����¼�¼����
	XRecord record;
	// id���ڲ��Զ�����ΪItem������
	record.SetId(m_record_list.ItemCount()); 
	
	// ����record��������ʱ��Ϊ��ǰϵͳʱ��
	record.SetAddDatetime(XUtility::GetCurDateTime());
 
	// �����û������¼�¼����������ֵ
	cout << "����Ҫ�����˺������Ӧ�����ƣ�";
	string keyname;
	getline(cin, keyname);
	record.SetKeyname(keyname);

	cout << "�����˺ţ�";
	string account;
	getline(cin, account);
	record.SetAccount(account);

	cout << "�������룺";
	string password;
	getline(cin, password);
	record.SetPassword(password);

	cout << "����˵��: ";
	string descr;        
	getline(cin, descr);
	record.SetDescr(descr);

	// -���¼�¼��ӵ����뱾�ļ�¼�б�
	m_record_list.AppendItem(record);  

	return true;
}

void showRecord(XRecord &record) {
	cout << "-------------------------------------------------------------------\n"
		<< "ID��" << record.GetId() << endl
		<< "Ӧ�ã�" << record.GetKeyname() << "��\n"
		<< "**�˺ţ�" << record.GetAccount() << "��\n"
		<< "**���룺" << record.GetPassword() << "��\n"
		<< "**����ʱ�䣺" << record.GetAddDatetime() << "��\n"
		<< "**˵����" << record.GetDescr() << endl
		<< "-------------------------------------------------------------------\n";
}

// ��ʾȫ����¼
void PasswordNotebook::ShowAllRecords()
{
	m_record_list.Traverse(showRecord);
}

// ͨ��ID���Ҽ�¼��Ϣ
bool PasswordNotebook::SearchRecordById() {
	int id;
	cout << "����Ҫ��ѯ�ļ�¼ID��";
	cin >> id;
	getchar();
	XRecord item;
	if (m_record_list.GetItem(id, item) == true) {
		showRecord(item);
		return true;
	} else {
		cout << "����Ҫ��ѯ�ļ�¼�����ڣ������²��ң�" << endl;
		return false;
	}
}

// ͨ��IDɾ����¼��Ϣ
bool PasswordNotebook::DeleteRecordById() {
	int id;
	cout << "����Ҫɾ���ļ�¼ID��";
	cin >> id; 
	getchar();
	if (m_record_list.DeleteItem(id) == true) {
		cout << "�ü�¼�Ѿ��ɹ�ɾ����" << endl;
		return true;
	} else {
		cout << "����Ҫɾ���ļ�¼�����ڣ�" << endl;
		return false;
	}
}

// ͨ��ID�޸ļ�¼��Ϣ
bool PasswordNotebook::ChangeRecordById() {
	int id;
	cout << "����Ҫ�޸ĵļ�¼ID��";
	cin >> id;
	getchar();
	XRecord item;
	if (m_record_list.GetItem(id, item) == true) { 
		cout << "�����޸ĵ����ݣ���ĳһ������Ϊ�գ���ʾ�������޸ġ�" << endl;
		cout << "�޸�Ӧ������Ϊ��";
		string keyname;
		getline(cin, keyname);
		if (keyname != "") item.SetKeyname(keyname);
		
		cout << "�޸��˺�Ϊ��";
		string account;
		getline(cin, account);
		if (account != "") item.SetAccount(account);

		cout << "�޸�����Ϊ��";
		string password;
		getline(cin, password);
		if (password != "") item.SetPassword(password);

		cout << "�޸�˵��Ϊ: ";
		string descr;
		getline(cin, descr);
		if (descr != "") item.SetDescr(descr);

		m_record_list.SetItem(id, item);
		return true;
	}
	else {
		cout << "����Ҫ�޸ĵļ�¼�����ڣ�" << endl;
		return false;
	}
}