HDU和PAT，很多时候都还需要使用C风格的字符串和输入输出函数，有点麻烦...

**Problem Description**
Ignatius最近遇到一个难题,老师交给他很多单词(**只有小写字母组成**,不会有重复的单词出现),现在老师要他统计出以某个字符串为前缀的单词数量(**单词本身也是自己的前缀**).
 
**Input**
输入数据的第一部分是一张单词表,每行一个单词,**单词的长度不超过10**,它们代表的是老师交给Ignatius统计的单词,一个空行代表单词表的结束.第二部分是一连串的提问,**每行一个提问**,**每个提问都是一个字符串**.
注意:本题**只有一组测试数据**,处理到文件结束.
 

**Output**
对于每个提问,给出**以该字符串为前缀的单词**的数量.
 

**Sample Input**
```
banana
band
bee
absolute
acm

ba
b
band
abc
```
题意：统计以某个字符串为前缀的字符串数量。

---

方法一：使用 `map` ，很简单。主要操作**在于分解一个单词的前缀**，这里是一个不错的方法。
```cpp
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <map>
using namespace std;

int main() {
	map<string, int> mp;  
	char str[13];
	while (gets(str)) {
		int len = strlen(str);  
		if (!len) break; // 为空行时退出循环，不再输入单词表 
		for (int i = len; i > 0; --i) { // 从后往前依次删除这个字符串的字符，得到所有的前缀 
			str[i] = '\0'; // 单词本身也是自己的前缀，""不是前缀 
			mp[str]++; // 统计相应前缀的数量 
		}
	}
	while (gets(str)) cout << mp[str] << endl; // 输出以该字符串为前缀的单词的数量 
	return 0;
}
```

---
方法二：字典树Trie，或者说前缀树，又称单词查找树，是一种**基于哈希表**和**Trie树**的快速内容查找算法，是一种哈希树的变种。

比起用链式结构，更简单、更常用的方法是**使用数组来实现字典树**，更加保险。使用 `pos` 给每个前缀标号，在 `num` 数组中使用对应前缀的 `pos` 值取其数量。

注意：这里开到100万的数组时也会MLE，如果只开10万的数组会Runtime Error，估计是数组越界，开到50万左右可以过。
```cpp
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

int trie[500010][26];  // 用数组定义字典树，存储下一个字符的位置
int num[500010] = {0}; // 以某一字符串为前缀的单词的数量
int pos = 1;           // 当前新分配的存储位置 

void insert(char s[]) { 
	int p = 0; // 字典树第几层
	for (int i = 0; s[i]; i++) {
		int n = s[i] - 'a';
		if (trie[p][n] == 0) // 对应字符没有值 
			trie[p][n] = pos++;
		p = trie[p][n]; 
		num[p]++; // 每个前缀的单词数量+1 
	}
}

int find(char s[]) { 
	int p = 0;
	for (int i = 0; s[i]; i++) {
		int n = s[i] - 'a';
		if (trie[p][n] == 0) // 对应字符没有值
		    return 0;
		p = trie[p][n];
	}
	return num[p];
}

int main() {
	memset(trie, 0, sizeof(trie));
    char strs[12];
	while (gets(strs)) {
		int len = strlen(strs);
		if (!len) break;
		insert(strs);
	}
	while (gets(strs)) cout << find(strs) << endl;
	return 0;
}
```
