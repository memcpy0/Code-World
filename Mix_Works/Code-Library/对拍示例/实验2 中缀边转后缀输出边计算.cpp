#include <iostream>
#include <cstdio>
using namespace std;

/**************************************************************************************************************
类功能：堆栈类，用于存储运算符和运算数
成员变量：int mSize：栈中最多可存放的元素个数；int top：栈顶下标；T *st：指向存放栈元素的数组的指针
成员函数： arrStack(int size)：构造函数
 arrStack()：构造函数
 ~arrStack()：析构函数
 void clear()：清空堆栈
 bool push(const T item)：往堆栈中压入元素
 bool pop(T& item)：弹出栈顶元素
 bool returntop(T& item)：返回栈顶元素，并不弹出
 bool empty()：检查栈顶是否为空
**************************************************************************************************************/
template<class T>															//类模板
class arrStack {
private:
    int mSize;																//栈中最多可存放的元素个数
    int top;																//栈顶位置
    T *st;																	//指向存放栈元素的数组的指针
public:
    arrStack(int size) {													//含参构造函数
        mSize = size;												
        top = -1;
        st = new T[mSize];
    }
    arrStack() {															//无参构造函数
        top = -1;
    }
    ~arrStack() {															//析构函数
        delete [] st; 
    }
    void clear() {															//清空堆栈
        top = -1;
    }
    bool push(const T item) {												//往堆栈中压入元素
        if (top == mSize - 1) {
            cout << "栈满了" << endl;										//如果堆栈满了
            return false;
        }
        else {																//如果栈没有满
            st[++top] = item;												//把该元素放在栈顶
            return true;
        }
    }
    bool pop(T& item) {														//弹出栈顶元素
        if (top == -1) {													//空栈
            cout << "栈为空，不能执行出栈操作" << endl;
            return false;
        }
        else {																//不是空栈
            item = st[top--];
            return true;
        }
    }
    bool returntop(T& item) {												//返回栈顶元素，并不弹出
        if (top == -1) {													//空栈
            //cout << "栈为空" << endl;
            return false;
        }
        else {
            item = st[top];
            return true;
        }
    }
    bool empty() {															//检查栈顶是否为空
        if (top == -1) return true;
        else return false;
    }
/*****************************************************************************************************************************
函数功能：此函数只能在字符栈中使用，比较读入元素char c与目前栈顶元素的运算符的优先级
形式参数：const char c:读入的字符
返回值：布尔型，栈顶元素优先级是否高于读入元素
******************************************************************************************************************************/
    bool stackTopPrioritize(const char c) {                                                
        int zhanding, duqu;								                                   
        if (top == -1) return false;                                        //空栈时字符进栈
	    switch (c) {
	        case '+': 
	        case '-': {duqu = 1;break;}										//加减号优先级为1
	        case '*':
	        case '/': {duqu = 2;break;}														   
	        case '(' : {duqu = 0;}											//左括号不出栈只在有右括号才出栈，所以认为优先级小
	        default: duqu = 0;
        }
        
        char zhanTop = st[top];                                             //栈顶元素
	        switch (zhanTop) {
	        case'+':
	        case'-': {zhanding = 1;break;}
	        case'*':
	        case'/': {zhanding = 2;break;}
	        case'(' : {zhanding = 0;}
        }
        if (zhanding >= duqu) return true;									//出栈，栈顶字符优先级高于等于读取的优先级
        else return false;
    }
};
/**************************************************************************************************************
类功能：
成员变量：
成员函数： 
**************************************************************************************************************/
class TwoStack
{
private:
	arrStack<char> *charStack;											    //声明一个字符型堆栈，用于判断和存储运算符号的计算先后顺序
	arrStack<int> *intStack;											    //声明一个整型堆栈，用于存储操作数

public:
	TwoStack() {
		charStack = new arrStack<char>(100);
		intStack  = new arrStack<int>(100);
	}

	bool GetTwoOperands(int &opd1, int &opd2) {								//得到两个操作数 
		if (intStack->empty()) return false;
		intStack->pop(opd1);
		if (intStack->empty()) return false;
		intStack->pop(opd2);
	}

	int Compute(char oper, double opd1, double opd2) {						//计算两个操作数 
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
		int num = 0;																//计算数组中元素的数量
		int res, opd1, opd2, operand; 
		while ((cin >> readin) && readin != '=') {								    //读入字符，以"="为终止记号
			switch (readin) {														//readin是运算符
				case '(' : 															//是( 
					charStack->push(readin);										//无条件入栈 
					break;
				case ')' :  														//是) 
					while (charStack->pop(c2) && c2 != '(') {						//弹出栈顶元素直到弹出左括号 
						cout << c2 << " ";											//输出栈顶元素 
						GetTwoOperands(opd1, opd2);									//得到两个操作数 
						res = Compute(c2, opd1, opd2);		 
						intStack->push(res);										//将结果入栈 
					}
					break;
				case '+' :
				case '-' :
				case '*' :
				case '/' :  
					charStack->returntop(c2);										//得到栈顶元素 
					if (c2 == '(')  charStack->push(readin);						//如果栈顶是(则无条件入栈 
					else {
						if(!charStack->stackTopPrioritize(readin)) {				//如果readin优先级更高							
							charStack->push(readin);
						}
						else {
							do{
								charStack->pop(c2);									//弹出栈顶元素 
								cout << c2 << " ";									//输出栈顶元素 
								GetTwoOperands(opd1, opd2);							//得到两个操作数						
								res = Compute(c2, opd1, opd2);		 
								intStack->push(res);								//入栈 
							} while(charStack->stackTopPrioritize(readin));
							charStack->push(readin);								//弹出优先级更高的栈顶元素后入栈,保持栈的单调性 
						}
					}
					break;
				default:															//是数字则得到完整的操作数 
					cin.putback(readin);											//将已读的字符插入输入流中										
					cin >> operand;													//读出一个操作数 
					intStack->push(operand);										//操作数压栈 
					cout << operand << " ";											//输出操作数 
			}
		}
		while (!charStack->empty()) {												//运算符栈还有运算符 
			charStack->pop(c2);														//弹出栈顶元素 
			cout << c2 << " ";														//输出运算符 
			GetTwoOperands(opd1, opd2);												//从操作数栈得到两个操作数 
			res = Compute(c2, opd1, opd2);											//计算结果 
		    intStack->push(res);													//结果入栈 
		}
		int x; 
		intStack->returntop(x);
		return x;
	 }
};

int main() {
	TwoStack ts;
	int a;
	cout << "请输入表达式, 以'='为结束符：" << endl; 
    a = ts.Transform();
	cout << endl << "逆波兰表达式如上, 表达式运算结果为：" << endl;
	cout << a;
	//system("pause");
	return 0;
}      
