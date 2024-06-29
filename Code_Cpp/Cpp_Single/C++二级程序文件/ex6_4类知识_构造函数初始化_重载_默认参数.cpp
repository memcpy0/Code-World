#include <iostream>
#include <algorithm>
using namespace std;

// 时间类的说明部分
class Time
{
	
	
};
 
// 日期类的说明部分 
class Date
{   // 访问修饰控制符 
	public:               // 同一类型的成员放在一起 
        /* 
        构造函数功能是对数据成员进行初始化; 
		构造函数的名字必须与类名相同；
        不指定返回类型, void也不行, 实际隐含有返回值, 由系统内部使用; 
		构造函数可以有一个或多个参数, 可以重载, 可以有默认参数值;
		构造函数会在创建对象时自动调用;
		*/ 
        Date(int y, int m, int d);
        /* 默认构造函数就是使用时不必为其提供参数的构造函数, 函数名与类名相同,
		有两种类型, 参数表为空, 不带参数, 或者它的所有参数使用默认参数值, 下面的就是一个默认构造函数;
		如果类中定义了一个默认构造函数, 就使用它; 如果类中没有定义任何构造函数, 就生成一个默认的不带参数的构造函数,
		如果类中定义了别的构造函数即有参和默认有参构造函数, 不会生成默认无参构造函数;
		同时定义默认有参构造函数和默认无参构造函数, 会引起二义性; 
		*/
        Date(int y=2000): year(y) {
        	month = 10;
        	day = 1;
        	count++;
        	cout << "Constructor of Date with 1 parameter." << endl;
		}
		/* 复制构造函数 
		复制构造函数也是一种构造函数, 函数名与类名相同, 能指定函数返回类型; 
		复制构造函数只有一参数, 是对同类的某个对象的引用； 
		每个类中必须有一个复制构造函数, 没有就会自动生成一个公有复制构造函数; 
		*/
		Date(const Date& date); 
		/*
		析构函数名是在类名前加~;
	    析构函数不指定返回类型, 不能有返回值;
		析构函数没有参数, 因此析构函数不能重载, 一个类中只能有一个析构函数;
		在撤销对象时, 系统会自动调用析构函数, 调用顺序与构造函数的调用顺序完全相反！ 
		如果类中没有定义析构函数, 编译器自动生成一个公有的析构函数, 即默认析构函数. 
		*/
		~Date();
		
		void SetDate(int y, int m, int d);  // 所有成员函数都必须在类体内声明, 但不一定要实现 
		int IsLeapYear()                    // 如果成员函数在类体内实现定义,就是内联函数,在体外则需要显示inline 
		{
			return (!(year%4) && year%100) || !(year%400);
		}
		static void Print(Date& d); 
	/* 类体内不允许对所定义的数据成员初始化, 所以静态数据成员也要到类体外进行初始化 */ 
	private:                              // 类中的数据成员和成员函数默认的访问权限是私有的  
		int year, month, day;             // 私有的数据成员, 一般都将数据作为私有的
		static int count;
}; 
int Date::count = 0;
void Date::Print(Date& d)
{
	cout << d.year << "." << d.month << "." << d.day << endl;  // 私有成员, 只能在类的成员函数内访问 
	cout << "There are " << count << " Date Objects." << endl;
}
// 日期类的实现部分             // 初始化列表 
Date::Date(int y, int m, int d):year(y), month(m), day(d) { 
    /* 初始化优先于赋值;
	先执行初始化列表对成员进行初始化, 再执行构造函数体;
	初始化列表各个初始化项的执行顺序取决于类成员在类中声明的顺序并与之一致, 而与初始化项的顺序无关;
	如果在初始化列表中没有对某个成员进行初始化, 表示对该成员进行了默认初始化, 使之有了不确定的初值, 就可以省略,
	如果对所有成员都进行了默认初始化, 那么就可以省略构造函数的初始化列表;  
	多数数据成员既可以使用初始化列表获得显式初值, 也可以在获得默认初值后在构造函数体中进行赋值, 从而获得确定的值, 
	前者除了使初始化情况更明显, 还有一定的效率优势; 
    */ 
    count++;
    cout << "Constructor of Date with 3 parameters." << endl;
}
Date::Date(const Date& date)
{
	year = date.year;
	month = date.month;
	day = date.day; 
	cout << "Copy Constructor of Date." << endl;
}
Date::~Date()
{
	cout << "Destrutor of Date: " << year << "." << month << "." << day << endl;
}  
/* 
   任何来自类外部(通过类对象或其引用)的访问都必须通过公有类型进行！
   公有类型的成员不仅可以在类内部被访问, 也可以在类外部通过类对象被访问 
   私有类型的成员只允许在本类(非类对象)的成员函数内部访问, 任何来自类外部(通过类对象)的访问都是非法的
   保护类型介于两者之间    
*/

void Date::SetDate(int y, int m, int d)
{
	year = y;
	month = m;
	day = d; 
} 

void DatePrint(Date &d)
{	
	d.Print(d);         // 正确, 通过公共接口访问	
} 
Date fun(Date d)
{
	Date temp;
	temp = d;
	return temp;
} 
int main()
{ 
    /* 系统自动调用构造函数, 两个构造参数满足函数重载的条件, 调用时根据参数自动解析 */
	Date date1(2012, 10, 10), date2(0, 0, 0);  /* date2[5] 定义对象数组,可以进行初始化,每个数组元素根据构造函数构造,未初始化的使用默认构造函数 */ 
	Date date3(date1);                     
    date2 = fun(date3);                   
	// Date *pDate, &date4=date1;              // 定义对象指针和对象引用, 此时不会调用构造函数 
	/* 复制构造函数只在3种情况被调用, 用类的一个已知的对象去初始化一个正在创建的对象 
    传值调用时用对象作为函数实参，去初始化函数形参;
	对象作为函数返回值, 去初始化一个匿名对象, 再将匿名对象赋值给接收返回值的对象, 赋值完毕后匿名对象销毁
	其实, 后两者只是前者的特例而言, 归根到底只有一种情况; 
	只有初始化会调用, 赋值不会调用复制构造函数 
	*/ 
	cout << "date1: ";
	Date::Print(date1);          
	cout << "date2: ";
	Date::Print(date2);
	cout << "date3: ";
	Date::Print(date3);
	if (date2.IsLeapYear()) 
	   cout << "date2 is a leapyear." << endl;
    
    Date *p1, *p2, *p3;
    /* 通过new和delete显式创建和删除对象与前面有一些不同 
	1. new返回指针, delete删除指针
	2. 用new创建单个对象时，根据参数调用相应的构造函数, new创建对象数组的时候, 会调用默认构造函数, 但是由于在类中
	如果已经定义了一个有参构造函数或默认有参构造函数后, 不会生成默认无参构造函数, 这样就会使用默认有参构造函数(没有
	的话就会报错, 因为没有匹配的构造函数；用delete删除时会调用析构函数
	3. 对自由存储对象调用构造函数和析构函数时，由于调用是显式进行的, 调用顺序取决于new和delete运算符的顺序, 析构函数
	的调用顺序不一定与构造函数的执行顺序严格相反, 但是对象数组中各个元素的构造和析构顺序还是相反的  
	*/ 
	p1 = new Date(1111, 1, 1);   
	p2 = new Date(2222, 2, 2);
	p3 = new Date[2];
	/*使用.访问一般对象(对象数组元素)和引用对象，使用->访问指针对象的数据成员和成员函数*/ 
	p3[0].SetDate(0000, 0, 0);
	p3[1].SetDate(3333, 3, 3);
	Date::Print(*p1);
	Date::Print(*p2);
	Date::Print(p3[0]);
	Date::Print(p3[1]);
	delete p1;
	delete p2;
	delete [] p3;
	return 0;
}

  
