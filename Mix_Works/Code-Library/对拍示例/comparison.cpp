#include <bits/stdc++.h>
using namespace std;
/*
��STL��ɵ���ȷ����, ʹ�ñ���������ɳ���, ���ں����Ķ��Ĺ��� 
*/
void printList(const list<string>& lst) {
	for (auto it = lst.cbegin(); it != lst.cend(); ++it) cout << *it << endl;
}

int main() {
    //freopen("input1.txt", "r", stdin);  //40���ַ���,ɾ����25���ַ��� 
	//freopen("output1.txt", "w", stdout);
	freopen("data.in", "r", stdin);		//������� 
	freopen("data.out", "w", stdout);
	cout << "��������Ҫ������ַ�������: " << endl;
	 
	int N, M;
	cin >> N;
	list<string> t;
	string temp;
	//���ն���Ļ��з� 
	getline(cin, temp); 
	//����N���ַ��� 
	cout << "����Ҫ�����" << N << "���ַ���(�س����зָ��ַ���): " << endl; 
	while (N--) {		
		getline(cin, temp);
		t.push_back(temp);
	}
	
	//���ֵ���������� 
	t.sort();			
	//���ֵ������ȫ���ַ��� 
	cout << "�ַ������ֵ����������ĵ���������: " << endl; 
	printList(t);
	//���������� 
	reverse(t.begin(), t.end());
	//�����ֵ���ĵ������
	cout << "�����������ȫ���ַ�������: " << endl; 
	printList(t); 
	//����M,��������ɾ����M����ַ��� 
	cout << "������Ҫɾ���ĵڼ�����ַ���(�ֵ���): " << endl; 
	cin >> M;
	list<string>::iterator it = t.begin();
	advance(it, M - 1);
	t.erase(it); 
	//���յ������ 
	cout << "��������ɾ����" << M << "����ַ������������: " << endl; 
	printList(t);
	return 0;
}

	
	
