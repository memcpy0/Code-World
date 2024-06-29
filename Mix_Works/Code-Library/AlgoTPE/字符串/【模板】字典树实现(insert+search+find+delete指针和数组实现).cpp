//#include <bits/stdc++.h>
//using namespace std;
//
////------------------------------------------------------------------------------------------
//public class TrieNode {
//	public int path; //表示有多少个单词共用这个结点(以当前字符串为前缀的单词数量)
//	public int end;  //有多少个单词以这个单词结尾
//	public TrieNode[] map;	//长度为26的数组
//	//字符种类较多的情况下,可以选择真实的哈希表结构实现map
//	
//	public TrieNode() {
//		path = 0;
//		end = 0;
//		map = new TrieNode[26];
//	}
//}
//
//public class Trie {
//	private TrieNode root;
//	public Trie() {
//		root = new TrieNode();
//	}
///*
//假设单词 `word` 的长度为 `N` ，从左到右遍历 `word` 中的每个字符，并依次从根结点开始根据每个 `word[i]` ，找到下一个结点：
// - 找的过程中该结点不存在，则建立新结点，即为 `a` ，`a.path = 1` 。
// - 如果结点存在，记为 `b` ，`b.path++` 。
// - 通过最后一个字符 `word[N - 1]` 找到最后一个结点时，记为 `e` ，令 `e.path++, e.end++` 。
//*/
//	public void insert(String word) {
//		if (word == null) return;
//		char[] chs = word.toCharArray();
//		int index = 0;
//		TrieNode node = root;
//		for (int i = 0; i < chs.length; ++i) {
//			index = chs[i] - 'a';
//			if (node.map[index] == null)
//				node.map[index] = new TrieNode();
//			node = node.map[index];
//			++node.path;
//		}
//		++node.end;
//	}
///*
//从左到右遍历 `word` 中的每个字符，并依次从根结点开始根据每个 `word[i]` ，找到下一个结点：
//- 如果找的过程中结点不存在，说明这个单词的整个部分没有添加进Trie树，于是不存在该单词，直接返回 `false` ；
//- 如果能够通过 `word[N - 1]` 找到最后一个结点，记作 `e` ，同时 `e.end != 0` ，说明存在单词通过 `word[N - 1]` 的字符路径，
//并且以结点 `e` 结尾，返回 `true` ；如果 `e.end == 0` ，返回 `false` 。
//*/
//	public boolean search(String word) {
//		if (word == null) return false;
//		char[] chs = word.toCharArray();
//		TrieNode node = root;
//		int index = 0;
//		for (int i = 0; i < chs.length; ++i) {
//			index = chs[i] - 'a';
//			if (node.map[index] == null)
//				return false;
//			node = node.map[index];
//		}
//		return node.end != 0;
//	}
///*
//这里先调用 `search(word)` ，看看该单词是否存在。如果不存在，则直接返回；
//存在则执行后续的过程：从左到右遍历 `word` 中的每个字符，并依次从根结点开始根据每个 `word[i]` 找到下一个结点，过程中把扫过的每个结点的 `path` 值减一。
//
//如果发现下一个结点的 `path` 值减完之后为 `0` ，直接从当前结点的 `map` 中删除后续的所有路径，返回即可。
//如果扫到最后一个结点，记为 `e` ，`e.path--, e.end--;` 。
//*/
//	public void remove(String word) {
//		if (search(word)) {
//			char[] chs = word.toCharArray();
//			TrieNode node = root;
//			int index = 0;
//			for (int i = 0; i < chs.length; ++i) {
//				index = chs[i] - 'a';
//				if (node.map[index].path-- == 1) { //path--后等于0,删除后续路径
//					node.map[index] = null;
//					return;
//				}
//				node = node.map[index];
//			}
//			--node.end;
//		}
//	}
///*
//和查找操作同理。根据 `pre` 不断找到结点，假设最后的结点记为 `e` ，返回 `e.path` 的值即可。
//*/
//	public int prefixNumber(String pre) {
//		if (pre == null) return 0;
//		char[] chs = pre.toCharArray();
//		TrieNode node = root;
//		int index = 0;
//		for (int i = 0; i < chs.length; ++i) {
//			index = chs[i] - 'a';
//			if (node.map[index] == null)
//				return 0;
//			node = node.map[index];
//		}
//		return node.path;
//	}
//}
//--------------------------------------------------------------------------------
//#include <cstdio>
//#include <cstring>
//#include <iostream>
//using namespace std;
//
//struct Trie {
//	Trie* next[26];
//	int path; //以当前字符串为前缀的单词数量
//	int end;  //有多少个单词以这个单词结尾
//	Trie() { //构造函数
//		for (int i = 0; i < 26; ++i) next[i] = nullptr; 
//		//memset(next, 0, sizeof(next));
//		path = end = 0;
//	}
//	~Trie() { // 析构
//		for (int i = 0; i < 26; ++i) {
//			if (next[i]) delete next[i];
//		}
//	}
//}; 
//Trie trie;
////插入一个字符串到字典树
//void insert(char s[]) {
//	Trie *p = &trie; 
//	for (int i = 0; s[i]; ++i) { //遍历每一个字符 
//		if (p->next[s[i] - 'a'] == nullptr) //结点不包括这个字符 
//			p->next[s[i] - 'a'] = new Trie; //创建一个
//		p = p->next[s[i] - 'a']; //进入下一层 
//		++p->path; //包含该前缀的字符串+1
//	}
//	++p->end;
//}
////搜索字符串
//bool search(char s[]) {  
//	Trie *p = &trie;
//	for (int i = 0; s[i]; ++i) { //在字典树中找到该单词的结尾位置 
//	    if (!p->next[s[i] - 'a']) return false;  
//		p = p->next[s[i] - 'a'];
//	}
//	return p->end != 0;
//}
////删除字符串一次
//void remove(char s[]) {
//	if (search(s)) {
//		Trie *p = &trie;
//		for (int i = 0; s[i]; ++i) {
//			if (p->next[s[i] - 'a']->path-- == 1) {
//				p->next[s[i] - 'a'] = nullptr; //实际设计中需要回收空间
//				return;
//			}
//			p = p->next[s[i] - 'a'];
//		}
//		--p->end;
//	}
//}
////返回以该字符串为前缀的单词的数量 	
//int prefixNumber(char s[]) { 
//	Trie *p = &trie;
//	for (int i = 0; s[i]; ++i) { //在字典树中找到该单词的结尾位置 
//	    if (!p->next[s[i] - 'a']) return 0;  
//		p = p->next[s[i] - 'a'];
//	}
//	return p->path;
//}
//
//int main() {
//	//...
//	return 0;
//} 

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


//Trie（发音类似 "try"）或者说 前缀树 是一种树形数据结构，
//用于高效地存储和检索字符串数据集中的键。这一数据结构有相当多的应用情景，
//例如自动补完和拼写检查。
//
//请你实现 Trie 类： 
//    Trie() 初始化前缀树对象。
//    void insert(String word) 向前缀树中插入字符串 word 。
//    boolean search(String word) 如果字符串 word 在前缀树中，返回 true（即，在检索之前已经插入）；否则，返回 false 。
//    boolean startsWith(String prefix) 如果之前已经插入的字符串 word 的前缀之一为 prefix ，返回 true ；否则，返回 false 。
class Trie {
    struct node {
        bool isEnd;
        node *next[26];
        node() { isEnd = false; memset(next, 0, sizeof(next)); }
    } *root;
public:
    /** Initialize your data structure here. */
    Trie() { root = new node; }
    
    /** Inserts a word into the trie. */
    void insert(string word) {
        node *cur = root;
        int n = word.size();
        for (int i = 0; i < n; ++i) {
            int num = word[i] - 'a';
            if (cur->next[num] == nullptr) cur->next[num] = new node;
            cur = cur->next[num];
        }
        cur->isEnd = true;
    }
    
    /** Returns if the word is in the trie. */
    bool search(string word) {
        node *cur = root;
        int n = word.size();
        for (int i = 0; i < n; ++i) {
            int num = word[i] - 'a';
            if (cur->next[num] == nullptr) return false;
            cur = cur->next[num];
        }
        return cur->isEnd;
    }
    
    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) {
        node *cur = root;
        int n = prefix.size();
        for (int i = 0; i < n; ++i) {
            int num = prefix[i] - 'a';
            if (cur->next[num] == nullptr) return false;
            cur = cur->next[num];
        }       
        return true;
    }
}; 

//---------------------------------------------------------- 
