#include "ListNode.h"
#include "LinkedList.h"
#include "String.h"
using namespace std; 
/*
ָ��,��,����:
�Ӽ���������N���ַ���������һ�����ֵ������ĵ��������ֵ��������
�������������ã����ֵ���ĵ��������������һ��M�����У�N>=M��,��������ɾ����M����ַ������ٵ��������
(ע: ��������ַ������ö�ָ̬�����ռ䣬��������ַ����Ȳ�����1000) 
*/	
int main() {
    //freopen("input1.txt", "r", stdin);  //40���ַ���,ɾ����25���ַ��� 
	//freopen("output2.txt", "w", stdout);
	freopen("data.in", "r", stdin);		  //������� 
	freopen("solution_data.out", "w", stdout);
	cout << "��������Ҫ������ַ�������: " << endl;
	 
	int N, M;
	cin >> N;
	LinkedList lst;
	String temp;
	//���ն���Ļ��з� 
	getchar();
	//����N���ַ��� 
	cout << "����Ҫ�����" << N << "���ַ���(�س����зָ��ַ���): " << endl; 
	while (N--) {		
		temp.getline();
		lst.orderlyInsert(temp);  //���ֵ������˳�����,ȷ������������ 
	}		
	//���ֵ������ȫ���ַ��� 
	cout << "�ַ������ֵ����������ĵ���������: " << endl; 
	lst.printList();
	//���������� 
	lst.reverse();
	//�����ֵ���ĵ������
	cout << "�����������ȫ���ַ�������: " << endl; 
	lst.printList(); 
	//����M,��������ɾ����M����ַ��� 
	cout << "������Ҫɾ���ĵڼ�����ַ���(�ֵ���): " << endl; 
	cin >> M;
	lst.remove(M);
	//���յ������ 
	cout << "��������ɾ����" << M << "����ַ������������: " << endl; 
	lst.printList();
	return 0;
} 
