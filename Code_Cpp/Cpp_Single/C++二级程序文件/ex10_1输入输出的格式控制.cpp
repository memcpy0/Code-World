#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <iomanip>
#include <cmath>
using namespace std;

class Person{
	public:
		char name[10];
		char date[12];
		bool isFemale;
};
Person staff[] = {"����", "1977.05.30", true,
                  "������", "1985.11.01", true,
				  "������", "1978.05.19", true,
				  "��ǿ", "1953.08.12", false,
				  "����ΰ", "1982.01.22", true}; 
int main()
{
	cout << 123 << endl;
	cout.width(10);   // ��ȵ�����ֻ��һ������������Ч��, ���ָ���0 
	cout << 123 << 456 << endl;
	cout << setw(10) << 123 << setw(10) << 456 << endl;  
	
	cout << 314.1596535 << endl;  // ������� resetiosflags(ios_base::floatfield)
	cout << fixed << 314.15926535 << endl; // ������� 
	cout << scientific << 314.15926535 << endl;  // ��ѧ������
	
	// С���㴦�� �Ƿ���ʾ��Ч0 showpoint
	cout << endl << resetiosflags(ios_base::floatfield) << 732.0 << endl;
	cout << showpoint << 732.0 << endl;
	
	// ��λ�����ڵ�������ƽ������ 
	cout << "    |";
	for (int i=0; i<10; i++)
	{
		cout << setw(5) << i << "  ";
    } 
    cout << endl << "---+";
    for (int i=0; i<10; i++)
    {
    	cout << "-------";
	}
	cout << fixed << setprecision(3);       // ������ȵĿ��� 
	for (int i=0; i<10; i++)
	{
		cout << endl << setw(2) << "   |";
		for (int j=0; j<10; j++) cout << setw(7) << sqrt(i*10.0+j);
	}
	cout << endl;
	// ���뷽ʽ�Ŀ��� left����� internal right �Ҷ��� 
	cout << internal << setw(5) << -123 << endl;
	cout << left << setw(10) << 123 << endl;
	cout << 123 << right << setw(5) << -123 << endl; 
	// ����ַ��Ŀ���
	cout << setw(10) << 12.34 << endl;
	cout << left << setfill('+') << setw(10) << 12.34 << endl; 
	// ���뻻�з�
	cout << endl;
	
	// ����������Ƶı任
	cout << 12 << '\t' << showbase << oct << 9 << '\t' << uppercase << hex << 16 
	     << nouppercase << '\t' << dec << 10 << noshowbase << '\t' << 100 << endl; 
	     
    // �ļ��� �� �ر� ��λ �б�״̬ 
	// �и�ʽ������� ������<< >> 
	ofstream table("C:\\SQRTABLE.TXT", ios_base::trunc); 
	table << "   |";
	for (int i=0; i<10; i++) table << setw(6) << i << "    "; 
	table << endl << "-----+";
	for (int i=0; i<10; i++) table << "----------";
	table << fixed << setprecision(6);
	for (int i=0; i<10; i++){
		table << endl << setw(2) << i << "   |";
		for (int j=0; j<10; j++) table << setw(10) << sqrt(i*10+j);
	} 
    table.close();
    // �޸�ʽ������� ������ ��������<< >> ��write��read������� 
    fstream infile, outfile;
    outfile.open("staff.dat", ios::out|ios::binary);
    if (!outfile) {
    	cout << "staff.dat can't open." << endl;
    	exit(1);
	}
    for (int i=0; i<5; i++) outfile.write((char *)&staff[i], sizeof(staff[i]));
	outfile.close();
	infile.open("staff.dat", ios::in|ios::binary); 
    if (!infile) {
    	cout << "staff.dat can't open." << endl;
    	exit(1);
	}
    for (int i=0; i<5; i++) {
    	infile.read((char *)&staff[i], sizeof(staff[i]));
    	cout << "���� " << staff[i].name << '\t' << (staff[i].isFemale? "Ů":"��") << "\t" << "�������� " << staff[i].date<<endl; 
	}
	return 0;
}
