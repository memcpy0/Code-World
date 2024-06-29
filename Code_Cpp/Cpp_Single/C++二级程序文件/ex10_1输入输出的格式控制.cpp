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
Person staff[] = {"吴月", "1977.05.30", true,
                  "赵晓光", "1985.11.01", true,
				  "王如生", "1978.05.19", true,
				  "张强", "1953.08.12", false,
				  "李世伟", "1982.01.22", true}; 
int main()
{
	cout << 123 << endl;
	cout.width(10);   // 宽度的设置只对一次输入或输出有效果, 随后恢复成0 
	cout << 123 << 456 << endl;
	cout << setw(10) << 123 << setw(10) << 456 << endl;  
	
	cout << 314.1596535 << endl;  // 浮点输出 resetiosflags(ios_base::floatfield)
	cout << fixed << 314.15926535 << endl; // 定点输出 
	cout << scientific << 314.15926535 << endl;  // 科学计数法
	
	// 小数点处理 是否显示无效0 showpoint
	cout << endl << resetiosflags(ios_base::floatfield) << 732.0 << endl;
	cout << showpoint << 732.0 << endl;
	
	// 两位数以内的整数的平方根表 
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
	cout << fixed << setprecision(3);       // 输出精度的控制 
	for (int i=0; i<10; i++)
	{
		cout << endl << setw(2) << "   |";
		for (int j=0; j<10; j++) cout << setw(7) << sqrt(i*10.0+j);
	}
	cout << endl;
	// 对齐方式的控制 left左对齐 internal right 右对齐 
	cout << internal << setw(5) << -123 << endl;
	cout << left << setw(10) << 123 << endl;
	cout << 123 << right << setw(5) << -123 << endl; 
	// 填充字符的控制
	cout << setw(10) << 12.34 << endl;
	cout << left << setfill('+') << setw(10) << 12.34 << endl; 
	// 插入换行符
	cout << endl;
	
	// 输入输出数制的变换
	cout << 12 << '\t' << showbase << oct << 9 << '\t' << uppercase << hex << 16 
	     << nouppercase << '\t' << dec << 10 << noshowbase << '\t' << 100 << endl; 
	     
    // 文件流 打开 关闭 定位 判别状态 
	// 有格式输入输出 可以用<< >> 
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
    // 无格式输入输出 二进制 不可以用<< >> 用write和read输出输入 
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
    	cout << "姓名 " << staff[i].name << '\t' << (staff[i].isFemale? "女":"男") << "\t" << "出生日期 " << staff[i].date<<endl; 
	}
	return 0;
}
