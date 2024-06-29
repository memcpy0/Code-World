#include <bits/stdc++.h>
using namespace std;
/*
`atoi` 用来把一个字符串转换成一个整数。其中，当输入字符串**非法**或者**是 `"0"`** 时，`atoi` 返回 `0` ，并用一个全局变量区分这两种情况：
- 非法输入时，返回 `0` 并将这个全局变量设为一个特殊标记；
- 输入是 `"0"` 时，返回 `0` ，不会设置全局变量。

当 `atoi` 返回 `0` 时，调用者可以通过检查全局变量得知，输入究竟是非法输入（`NULL` 指针、空字符串 `""` 、含有 `0-9, +, -` 之外的非法字符），还是字符串 `"0"` 。

当然，本题中条件不太一样：只要存在有效的数字部分，就可以返回其数值，除非**第一个非空格字符不是一个有效整数字符**、**字符串为空**或**字符串仅包含空白字符**时。

另外，我们还需要考虑溢出等情况。

> C++中成员变量的初始化顺序，只会和它们在类中声明的顺序有关，而与初始化列表中的顺序无关。
*/

/*
功能测试：输入的字符串表示正数、负数、0
边界值测试：最大的正整数、最小的负整数
特殊输入测试：NULL指针、输入字符串为空字符串、输入字符串含有非数字字符、输入字符串仅有一个正号或者负号等 
*/
namespace myAtoi { //49
//定义一个全部变量g_nStatus标记是否遇到了非法输入,如果输入的字符串指针是空指针
//则标记全局变量然后直接返回;
//接下来遍历字符串中的所有字符:
//正负号只有可能出现在字符串的第一个字符,先处理第一个字符,是负号则标记为负数
//处理后续字符,遇到0-9之外的字符时终止遍历;遇到数字时则累加上去 
	enum Status { kValid = 0, kInvaild};
	int g_nStatus = kValid;
	int strToInt(const char *str) {
		g_nStatus = kInvaild;
		int num = 0;
		if (*str == '\0') return num; //输入是空串"" 
		
		if (str != nullptr) { //输入是空指针NULL时直接跳过 
			const char *digit = str;
			bool minus = false; //是否是负数 
			if (*digit == '+') 
				++digit;	
			else if (*digit == '-') {
				++digit;
				minus = true;
			}
			//没有考虑只有一个正号或者负号,会设置g_nStatus = kValid!!!
			//if (*digit == '\0') return 0; 
			while (*digit) {
				if (*digit >= '0' && *digit <= '9') {
					num = num * 10 + (*digit - '0');	
					++digit;
				} else {
					num = 0;
					break;
				}
			}
			if (*digit == '\0') {
				g_nStatus = kValid;
				if (minus) 
					num = 0 - num;
			}
		}
		return num;
	} 
	//参考代码
	int strToInt(const char *str) {
		g_nStatus = kInvalid;
		long long num = 0;
		if (str != nullptr && *str != '\0') { //空指针或者空串 
			bool minus = false;
			if (*str == '+') 
				++str;
			else if (*str == '-') {
				++str;
				minus = true;
			}
			
			if (*str != '\0')
				num = StrToIntCore(str, minus);			
		}
		return (int)num;  
	}
	long long StrToIntCore(const char *digit, bool minus) {
		long long num = 0;
		while (*digit) {
			if (*digit >= '0' && *digit <= '9') {
				int flag = minus ? -1 : 1;
				num = num * 10 + flag * (*digit - '0');
				
				if ((!minus && num > 0x7FFFFFFF) || //分两种情况判断整数发生上溢出或者下溢出 
					(minus && num < (signed int)0x80000000)) { //负数下溢出
						num = 0;	//设置为0 
						break;
				}
				++digit;
			} else { //其他非法字符 
				num = 0;   
				break;	
			}
		}
		if (*digit == '\0')	g_nStatus = kValid;
		return num;
	}			 
}

int main() { 

    return 0;
}
 
