#include <iostream>
#include <cstdio>
using namespace std;

/**************************************************************************************************************
�๦�ܣ���ջ�࣬���ڴ洢�������������
��Ա������int mSize��ջ�����ɴ�ŵ�Ԫ�ظ�����int top��ջ���±ꣻT *st��ָ����ջԪ�ص������ָ��
��Ա������ arrStack(int size)�����캯��
 arrStack()�����캯��
 ~arrStack()����������
 void clear()����ն�ջ
 bool push(const T item)������ջ��ѹ��Ԫ��
 bool pop(T& item)������ջ��Ԫ��
 bool returntop(T& item)������ջ��Ԫ�أ���������
 bool empty()�����ջ���Ƿ�Ϊ��
**************************************************************************************************************/
template<class T>															//��ģ��
class arrStack {
private:
    int mSize;																//ջ�����ɴ�ŵ�Ԫ�ظ���
    int top;																//ջ��λ��
    T *st;																	//ָ����ջԪ�ص������ָ��
public:
    arrStack(int size) {													//���ι��캯��
        mSize = size;												
        top = -1;
        st = new T[mSize];
    }
    arrStack() {															//�޲ι��캯��
        top = -1;
    }
    ~arrStack() {															//��������
        delete [] st; 
    }
    void clear() {															//��ն�ջ
        top = -1;
    }
    bool push(const T item) {												//����ջ��ѹ��Ԫ��
        if (top == mSize - 1) {
            cout << "ջ����" << endl;										//�����ջ����
            return false;
        }
        else {																//���ջû����
            st[++top] = item;												//�Ѹ�Ԫ�ط���ջ��
            return true;
        }
    }
    bool pop(T& item) {														//����ջ��Ԫ��
        if (top == -1) {													//��ջ
            cout << "ջΪ�գ�����ִ�г�ջ����" << endl;
            return false;
        }
        else {																//���ǿ�ջ
            item = st[top--];
            return true;
        }
    }
    bool returntop(T& item) {												//����ջ��Ԫ�أ���������
        if (top == -1) {													//��ջ
            //cout << "ջΪ��" << endl;
            return false;
        }
        else {
            item = st[top];
            return true;
        }
    }
    bool empty() {															//���ջ���Ƿ�Ϊ��
        if (top == -1) return true;
        else return false;
    }
/*****************************************************************************************************************************
�������ܣ��˺���ֻ�����ַ�ջ��ʹ�ã��Ƚ϶���Ԫ��char c��Ŀǰջ��Ԫ�ص�����������ȼ�
��ʽ������const char c:������ַ�
����ֵ�������ͣ�ջ��Ԫ�����ȼ��Ƿ���ڶ���Ԫ��
******************************************************************************************************************************/
    bool stackTopPrioritize(const char c) {                                                
        int zhanding, duqu;								                                   
        if (top == -1) return false;                                        //��ջʱ�ַ���ջ
	    switch (c) {
	        case '+': 
	        case '-': {duqu = 1;break;}										//�Ӽ������ȼ�Ϊ1
	        case '*':
	        case '/': {duqu = 2;break;}														   
	        case '(' : {duqu = 0;}											//�����Ų���ջֻ���������Ųų�ջ��������Ϊ���ȼ�С
	        default: duqu = 0;
        }
        
        char zhanTop = st[top];                                             //ջ��Ԫ��
	        switch (zhanTop) {
	        case'+':
	        case'-': {zhanding = 1;break;}
	        case'*':
	        case'/': {zhanding = 2;break;}
	        case'(' : {zhanding = 0;}
        }
        if (zhanding >= duqu) return true;									//��ջ��ջ���ַ����ȼ����ڵ��ڶ�ȡ�����ȼ�
        else return false;
    }
};
/**************************************************************************************************************
�๦�ܣ�
��Ա������
��Ա������ 
**************************************************************************************************************/
class TwoStack
{
private:
	arrStack<char> *charStack;											    //����һ���ַ��Ͷ�ջ�������жϺʹ洢������ŵļ����Ⱥ�˳��
	arrStack<int> *intStack;											    //����һ�����Ͷ�ջ�����ڴ洢������

public:
	TwoStack() {
		charStack = new arrStack<char>(100);
		intStack  = new arrStack<int>(100);
	}

	bool GetTwoOperands(int &opd1, int &opd2) {								//�õ����������� 
		if (intStack->empty()) return false;
		intStack->pop(opd1);
		if (intStack->empty()) return false;
		intStack->pop(opd2);
	}

	int Compute(char oper, double opd1, double opd2) {						//�������������� 
		int result;
		switch (oper) {
			case '+':
				result = opd2 + opd1;
				break;
			case '-':
				result = opd2 - opd1;
				break;
			case '/':
				result = opd2 / opd1;
				break;
			case '*':
				result = opd2 * opd1;
				break;
		}
		return result;
	}

	int Transform() {																
		char readin, c2;  																	
		int num = 0;																//����������Ԫ�ص�����
		int res, opd1, opd2, operand; 
		while ((cin >> readin) && readin != '=') {								    //�����ַ�����"="Ϊ��ֹ�Ǻ�
			switch (readin) {														//readin�������
				case '(' : 															//��( 
					charStack->push(readin);										//��������ջ 
					break;
				case ')' :  														//��) 
					while (charStack->pop(c2) && c2 != '(') {						//����ջ��Ԫ��ֱ������������ 
						cout << c2 << " ";											//���ջ��Ԫ�� 
						GetTwoOperands(opd1, opd2);									//�õ����������� 
						res = Compute(c2, opd1, opd2);		 
						intStack->push(res);										//�������ջ 
					}
					break;
				case '+' :
				case '-' :
				case '*' :
				case '/' :  
					charStack->returntop(c2);										//�õ�ջ��Ԫ�� 
					if (c2 == '(')  charStack->push(readin);						//���ջ����(����������ջ 
					else {
						if(!charStack->stackTopPrioritize(readin)) {				//���readin���ȼ�����							
							charStack->push(readin);
						}
						else {
							do{
								charStack->pop(c2);									//����ջ��Ԫ�� 
								cout << c2 << " ";									//���ջ��Ԫ�� 
								GetTwoOperands(opd1, opd2);							//�õ�����������						
								res = Compute(c2, opd1, opd2);		 
								intStack->push(res);								//��ջ 
							} while(charStack->stackTopPrioritize(readin));
							charStack->push(readin);								//�������ȼ����ߵ�ջ��Ԫ�غ���ջ,����ջ�ĵ����� 
						}
					}
					break;
				default:															//��������õ������Ĳ����� 
					cin.putback(readin);											//���Ѷ����ַ�������������										
					cin >> operand;													//����һ�������� 
					intStack->push(operand);										//������ѹջ 
					cout << operand << " ";											//��������� 
			}
		}
		while (!charStack->empty()) {												//�����ջ��������� 
			charStack->pop(c2);														//����ջ��Ԫ�� 
			cout << c2 << " ";														//�������� 
			GetTwoOperands(opd1, opd2);												//�Ӳ�����ջ�õ����������� 
			res = Compute(c2, opd1, opd2);											//������ 
		    intStack->push(res);													//�����ջ 
		}
		int x; 
		intStack->returntop(x);
		return x;
	 }
};

int main() {
	TwoStack ts;
	int a;
	cout << "��������ʽ, ��'='Ϊ��������" << endl; 
    a = ts.Transform();
	cout << endl << "�沨�����ʽ����, ���ʽ������Ϊ��" << endl;
	cout << a;
	//system("pause");
	return 0;
}      
