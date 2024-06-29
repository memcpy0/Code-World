#include <iostream>
#include <cstring>
using namespace std;

class AA {
	int i;
	public:
		AA(int n):i(n) {	};
		virtual ~AA() {	cout << "~AA������!" << endl;}
};

class BB:public AA{
	char *p;
	public:
		BB(int n, char *s):AA(n), p(s){
			if (!s) p=NULL;
			else {
				p=new char[strlen(s)]+1;
				strcpy(p, s);
			}
		}
		~BB() {
			// ͨ��ֻҪ�������ж���������������ר�ŵĶ���, ��������������Ӧ�ø�Ϊ�麯�� 
			delete [] p; cout<<"~BB�����ã�"<<endl; 
		}
};

int main()
{
	AA *p = new BB(5, NULL);
	delete p; // ��ָ������ָ������������Ķ��� 
	return 0;
}
