@[toc]

---
# 1. 字典树概述
## (1) Trie简介
字典树Trie，或者说前缀树，又称单词查找树，是一种**基于哈希表**和**Trie树**的快速内容查找算法，是哈希树的一类变种树形结构。它的最大优点是：<font color="red">**利用字符串的公共前缀来减少查询时间**</font>，最大限度地减少无谓的字符串比较，查询效率比哈希树高。

## (2) Trie性质
字典树的**基本性质**有3点：
1. **根结点不包含字符**，除根结点外的每一个结点都包含一个字符；
2. **从根结点到某一个结点，路径上字符连接起来就是该节点的前缀**，是该节点对应的字符串；
3. **每个结点的所有子结点包含的字符互不相同**，使用后代结点的指针目录，即**固定长度的指针数组**，对应可能出现的所有字符。

## (3) Trie用途和复杂度
字典树的用途：
- 基本用途：**字符串存储**。利用字符串的公共前缀节省存储空间（？）。比如加入 `"abc", "abcd", "abd", "b", "bcd", "efg", "hik"` 之后，字典树状况如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200829223640116.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)


- **字符串检索**。一般从字符串数组( `n` 个字符串)中暴力检索一个字符串，需要逐个匹配每个字符串，复杂度是 $\text{O(N＊M)}$ 的时间，$\text{N}$ 是数组长度，$\text{M}$ 是该字符串的长度，效率很低。原因在于比起搜索整数或实数(耗常数时间)来说，字符串比较需要正比于字符串长度的时间。而使用已经建立的字典树，可以降低到 $\text{O(M)}$ 的时间，**查找次数最多只需要这个单词的字符个数**；
- **字符串排序**，先序遍历Trie，可以得到其排序；
- **词频统计**，统计一个单词出现次数，需要允许重复添加；
- **前缀匹配**，字典树按照**公共前缀**建树，很适合前缀匹配；
- 和KMP搭配，在AC自动机中使用。

字典树的复杂度：
- 时间复杂度优秀；
- 如果使用指针，**需要的空间比较庞大**。只保存小写字母，每个结点 `26` 个指针，`64` 位指针为 `8` 位，那一个结点就是 `26bytes` ，100万长度的字符串，就需要200MB的内存空间。


---
# 2. 字典树实现
## (1) Trie主要功能和结点结构
假设组成所有单词的字符仅仅是小写字符 `a~z` ，实现以下 `4` 个功能：
```cpp
void insert(char s[]);		//insert(String word); 		添加word,可重复添加
void remove(char s[]);  	//remove(String word); 		删除word,如果添加word多次,仅删除一个
bool search(char s[]);		//search(String word); 		查询word算法在字典树中
int prefixNumber(char s[]); //prefixNumber(String pre); 返回以字符串pre为前缀的单词数量
```
字典树结点的Java代码：
```java
public class TrieNode {
	public int path; //表示有多少个单词共用这个结点(以当前字符串为前缀的单词数量)
	public int end;  //有多少个单词以这个单词结尾
	public TrieNode[] map;	//长度为26的数组
	//字符种类较多的情况下,可以选择真实的哈希表结构实现map
	
	public TrieNode() {
		path = 0;
		end = 0;
		map = new TrieNode[26];
	}
}
```
## (2) 插入操作
假设单词 `word` 的长度为 `N` ，从左到右遍历 `word` 中的每个字符，并依次从根结点开始根据每个 `word[i]` ，找到下一个结点：
 - 找的过程中该结点不存在，则建立新结点，即为 `a` ，`a.path = 1` 。
 - 如果结点存在，记为 `b` ，`b.path++` 。
 - 通过最后一个字符 `word[N - 1]` 找到最后一个结点时，记为 `e` ，令 `e.path++, e.end++` 。
 
 `Trie` 类和插入函数的代码如下：
```java
public class Trie {
	private TrieNode root;
	public Trie() {
		root = new TrieNode();
	}
	
	public void insert(String word) {
		if (word == null) return;
		char[] chs = word.toCharArray();
		int index = 0;
		TrieNode node = root;
		for (int i = 0; i < chs.length; ++i) {
			index = chs[i] - 'a';
			if (node.map[index] == null)
				node.map[index] = new TrieNode();
			node = node.map[index];
			++node.path;
		}
		++node.end;
	}
}
```
## (3) 查找操作
从左到右遍历 `word` 中的每个字符，并依次从根结点开始根据每个 `word[i]` ，找到下一个结点：
- 如果找的过程中结点不存在，说明这个单词的整个部分没有添加进Trie树，于是不存在该单词，直接返回 `false` ；
- 如果能够通过 `word[N - 1]` 找到最后一个结点，记作 `e` ，同时 `e.end != 0` ，说明存在单词通过 `word[N - 1]` 的字符路径，并且以结点 `e` 结尾，返回 `true` ；如果 `e.end == 0` ，返回 `false` 。

查找的代码如下：
```java
public class Trie {
	//...
	public boolean search(String word) {
		if (word == null) return false;
		char[] chs = word.toCharArray();
		TrieNode node = root;
		int index = 0;
		for (int i = 0; i < chs.length; ++i) {
			index = chs[i] - 'a';
			if (node.map[index] == null)
				return false;
			node = node.map[index];
		}
		return node.end != 0;
	}
}
```
## (4) 删除操作
这里先调用 `search(word)` ，看看该单词是否存在。如果不存在，则直接返回；存在则执行后续的过程：从左到右遍历 `word` 中的每个字符，并依次从根结点开始根据每个 `word[i]` 找到下一个结点，过程中把扫过的每个结点的 `path` 值减一。

如果发现下一个结点的 `path` 值减完之后为 `0` ，直接从当前结点的 `map` 中删除后续的所有路径，返回即可。如果扫到最后一个结点，记为 `e` ，`e.path--, e.end--;` 。
```java
public class Trie {
	//...
	public void remove(String word) {
		if (search(word)) {
			char[] chs = word.toCharArray();
			TrieNode node = root;
			int index = 0;
			for (int i = 0; i < chs.length; ++i) {
				index = chs[i] - 'a';
				if (node.map[index].path-- == 1) { //path--后等于0,删除后续路径
					node.map[index] = null;
					return;
				}
				node = node.map[index];
			}
			--node.end;
		}
	}
}
```

## (5) 前缀计数操作
和查找操作同理。根据 `pre` 不断找到结点，假设最后的结点记为 `e` ，返回 `e.path` 的值即可。代码如下：
```java
public class Trie {
	//...
	public int prefixNumber(String pre) {
		if (pre == null) return 0;
		char[] chs = pre.toCharArray();
		TrieNode node = root;
		int index = 0;
		for (int i = 0; i < chs.length; ++i) {
			index = chs[i] - 'a';
			if (node.map[index] == null)
				return 0;
			node = node.map[index];
		}
		return node.path;
	}
}
```

---
# 3. 动态字典树全代码(C++)
```cpp
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

struct Trie {
	Trie* next[26];
	int path; //以当前字符串为前缀的单词数量
	int end;  //有多少个单词以这个单词结尾
	Trie() { //构造函数
		for (int i = 0; i < 26; ++i) next[i] = nullptr; 
		//memset(next, 0, sizeof(next));
		path = end = 0;
	}
	~Trie() { // 析构
		for (int i = 0; i < 26; ++i) {
			if (next[i]) delete next[i];
		}
	}
}; 
Trie trie;
//插入一个字符串到字典树
void insert(char s[]) {
	Trie *p = &trie; 
	for (int i = 0; s[i]; ++i) { //遍历每一个字符 
		if (p->next[s[i] - 'a'] == nullptr) //结点不包括这个字符 
			p->next[s[i] - 'a'] = new Trie; //创建一个
		p = p->next[s[i] - 'a']; //进入下一层 
		++p->path; //包含该前缀的字符串+1
	}
	++p->end;
}
//搜索字符串
bool search(char s[]) {  
	Trie *p = &trie;
	for (int i = 0; s[i]; ++i) { //在字典树中找到该单词的结尾位置 
	    if (!p->next[s[i] - 'a']) return false;  
		p = p->next[s[i] - 'a'];
	}
	return p->end != 0;
}
//删除字符串一次
void remove(char s[]) {
	if (search(s)) {
		Trie *p = &trie;
		for (int i = 0; s[i]; ++i) {
			if (p->next[s[i] - 'a']->path-- == 1) {
				p->next[s[i] - 'a'] = nullptr; //实际设计中需要回收空间
				return;
			}
			p = p->next[s[i] - 'a'];
		}
		--p->end;
	}
}
//返回以该字符串为前缀的单词的数量 	
int prefixNumber(char s[]) { 
	Trie *p = &trie;
	for (int i = 0; s[i]; ++i) { //在字典树中找到该单词的结尾位置 
	    if (!p->next[s[i] - 'a']) return 0;  
		p = p->next[s[i] - 'a'];
	}
	return p->path;
}

int main() {
	//...
	return 0;
} 
```

---
# 4. 字典树应用题目
简单的题目：
[HDU 1251 统计难题](http://acm.hdu.edu.cn/showproblem.php?pid=1251) ：统计以某个字符串为前缀的字符串数量。注意：如果按照这里给出的指针形式提交，会MLE。
[洛谷 P2580 于是他错误的点名开始了](https://www.luogu.com.cn/problem/P2580)

难一点的题目：
背单词（Remember the Word, LA 3942）
strcmp() Anyone?，UVA 11732

---
# 5. 静态字典树
**更简单、更常用的方法是使用数组来实现字典树**，更加保险。我们将**使用二维数组 `trie[i][j]` 保存结点 `i` 的编号为 `j` 的子结点**，其中编号指的是该字符在字符集中的顺序（从 `0` 开始）。如果这个子结点不存在，则 `trie[i][j] = 0` ，因为任何结点的子结点都不可能是根结点；或者写成 `-1` 也行。然后，我们使用 `pos` 给每个前缀标号，在 `num` 数组中使用对应前缀的 `pos` 值取其数量。
```cpp
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
//用数组定义字典树,存储下一个字符的位置
//trie(或者改为next)[i][c]表示i号点所连、存储字符为c+'a'的点的编号
int trie[500010][26];
int num[500010] = {0}; 		//以某一字符串为前缀的单词的数量
int endS[500010] = {0};		//以这个结点为末尾的单词数量
int pos;	           		//当前新分配的存储位置(cnt:新分配的结点数量) 

//初始化,实际上是以-1层为根结点
void init() {
	memset(trie, -1, sizeof(trie)); //全部初始为-1,表示当前点没有存储字符
	pos = 0; 	//当前新分配的存储位置,用num[pos]来存储经过当前结点的字符串的数量(pos>=1)
}
//插入字符串
void insert(char s[]) { 
	int cur = 0; //字典树第几层(-1层为根结点)
	for (int i = 0; s[i]; i++) {
		int index = s[i] - 'a';
		if (trie[cur][index] == -1) // 对应字符没有值 
			trie[cur][index] = ++pos;
		cur = trie[cur][index]; //继续向下 
		num[cur]++; //每个前缀的单词数量+1 
	}
	++endS[cur];	//以当前结点结尾的单词数量+1
}
//查询某个字符串是否存在
bool search(char s[]) {
	int cur = 0;
	for (int i = 0; s[i]; i++) {
		int index = s[i] - 'a';
		if (trie[cur][index] == -1) 
			return false;
		cur = trie[cur][index];
	}
	return endS[cur] != 0; 
}
//查询含有某个前缀的字符串是否存在,如果返回0表示这个前缀不存在
int prefixNumber(char s[]) { //返回以该字符串为前缀的单词的数量
	int cur = 0; 
	for (int i = 0; s[i]; i++) {
		int index = s[i] - 'a';
		if (trie[cur][index] == -1) // 对应字符没有值
		    return 0;
		cur = trie[cur][index];
	}
	return num[cur];
}

int main() {
	init();
	//...
	return 0;
}
```
之后，我们将介绍普通字典树的扩展——01字典树。
