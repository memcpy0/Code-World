//  C++程序设计，201902
//	密码本应用程序
//  将信息存储到文本文件中 
//	Created by ZhangPing @ 2019-10    

#include "PasswordNotebook.h" 
// 密码本程序的主控函数
int main()
{
	PasswordNotebook myNotebook; // 密码本对象
	// 加载密码本，并执行账号密码管理业务
	myNotebook.Load();

	return 0;
}
