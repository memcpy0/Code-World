#include <iostream>
int main() {
	std::cout << "Enter two numbers:" << std::endl;
	int v1 = 0, v2 = 0;
	std::cin >> v1 >> v2;
	std::cout << "The sum of " << v1 << " and " << v2 << " is "
	          << v1 + v2 << std::endl;
	std::cerr << "你在错误的路上行走！" << std::endl;
	std::clog << "??一般性消息是什么?" << std::endl;
	return 0;
}
