//#include <bits/stdc++.h>
//using namespace std;
//
////------------------------------------------------------------------------------------------
//public class TrieNode {
//	public int path; //��ʾ�ж��ٸ����ʹ���������(�Ե�ǰ�ַ���Ϊǰ׺�ĵ�������)
//	public int end;  //�ж��ٸ�������������ʽ�β
//	public TrieNode[] map;	//����Ϊ26������
//	//�ַ�����϶�������,����ѡ����ʵ�Ĺ�ϣ��ṹʵ��map
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
//���赥�� `word` �ĳ���Ϊ `N` �������ұ��� `word` �е�ÿ���ַ��������δӸ���㿪ʼ����ÿ�� `word[i]` ���ҵ���һ����㣺
// - �ҵĹ����иý�㲻���ڣ������½�㣬��Ϊ `a` ��`a.path = 1` ��
// - ��������ڣ���Ϊ `b` ��`b.path++` ��
// - ͨ�����һ���ַ� `word[N - 1]` �ҵ����һ�����ʱ����Ϊ `e` ���� `e.path++, e.end++` ��
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
//�����ұ��� `word` �е�ÿ���ַ��������δӸ���㿪ʼ����ÿ�� `word[i]` ���ҵ���һ����㣺
//- ����ҵĹ����н�㲻���ڣ�˵��������ʵ���������û����ӽ�Trie�������ǲ����ڸõ��ʣ�ֱ�ӷ��� `false` ��
//- ����ܹ�ͨ�� `word[N - 1]` �ҵ����һ����㣬���� `e` ��ͬʱ `e.end != 0` ��˵�����ڵ���ͨ�� `word[N - 1]` ���ַ�·����
//�����Խ�� `e` ��β������ `true` ����� `e.end == 0` ������ `false` ��
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
//�����ȵ��� `search(word)` �������õ����Ƿ���ڡ���������ڣ���ֱ�ӷ��أ�
//������ִ�к����Ĺ��̣������ұ��� `word` �е�ÿ���ַ��������δӸ���㿪ʼ����ÿ�� `word[i]` �ҵ���һ����㣬�����а�ɨ����ÿ������ `path` ֵ��һ��
//
//���������һ������ `path` ֵ����֮��Ϊ `0` ��ֱ�Ӵӵ�ǰ���� `map` ��ɾ������������·�������ؼ��ɡ�
//���ɨ�����һ����㣬��Ϊ `e` ��`e.path--, e.end--;` ��
//*/
//	public void remove(String word) {
//		if (search(word)) {
//			char[] chs = word.toCharArray();
//			TrieNode node = root;
//			int index = 0;
//			for (int i = 0; i < chs.length; ++i) {
//				index = chs[i] - 'a';
//				if (node.map[index].path-- == 1) { //path--�����0,ɾ������·��
//					node.map[index] = null;
//					return;
//				}
//				node = node.map[index];
//			}
//			--node.end;
//		}
//	}
///*
//�Ͳ��Ҳ���ͬ������ `pre` �����ҵ���㣬�������Ľ���Ϊ `e` ������ `e.path` ��ֵ���ɡ�
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
//	int path; //�Ե�ǰ�ַ���Ϊǰ׺�ĵ�������
//	int end;  //�ж��ٸ�������������ʽ�β
//	Trie() { //���캯��
//		for (int i = 0; i < 26; ++i) next[i] = nullptr; 
//		//memset(next, 0, sizeof(next));
//		path = end = 0;
//	}
//	~Trie() { // ����
//		for (int i = 0; i < 26; ++i) {
//			if (next[i]) delete next[i];
//		}
//	}
//}; 
//Trie trie;
////����һ���ַ������ֵ���
//void insert(char s[]) {
//	Trie *p = &trie; 
//	for (int i = 0; s[i]; ++i) { //����ÿһ���ַ� 
//		if (p->next[s[i] - 'a'] == nullptr) //��㲻��������ַ� 
//			p->next[s[i] - 'a'] = new Trie; //����һ��
//		p = p->next[s[i] - 'a']; //������һ�� 
//		++p->path; //������ǰ׺���ַ���+1
//	}
//	++p->end;
//}
////�����ַ���
//bool search(char s[]) {  
//	Trie *p = &trie;
//	for (int i = 0; s[i]; ++i) { //���ֵ������ҵ��õ��ʵĽ�βλ�� 
//	    if (!p->next[s[i] - 'a']) return false;  
//		p = p->next[s[i] - 'a'];
//	}
//	return p->end != 0;
//}
////ɾ���ַ���һ��
//void remove(char s[]) {
//	if (search(s)) {
//		Trie *p = &trie;
//		for (int i = 0; s[i]; ++i) {
//			if (p->next[s[i] - 'a']->path-- == 1) {
//				p->next[s[i] - 'a'] = nullptr; //ʵ���������Ҫ���տռ�
//				return;
//			}
//			p = p->next[s[i] - 'a'];
//		}
//		--p->end;
//	}
//}
////�����Ը��ַ���Ϊǰ׺�ĵ��ʵ����� 	
//int prefixNumber(char s[]) { 
//	Trie *p = &trie;
//	for (int i = 0; s[i]; ++i) { //���ֵ������ҵ��õ��ʵĽ�βλ�� 
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
//�����鶨���ֵ���,�洢��һ���ַ���λ��
//trie(���߸�Ϊnext)[i][c]��ʾi�ŵ��������洢�ַ�Ϊc+'a'�ĵ�ı��
int trie[500010][26];
int num[500010] = {0}; 		//��ĳһ�ַ���Ϊǰ׺�ĵ��ʵ�����
int endS[500010] = {0};		//��������Ϊĩβ�ĵ�������
int pos;	           		//��ǰ�·���Ĵ洢λ��(cnt:�·���Ľ������) 

//��ʼ��,ʵ��������-1��Ϊ�����
void init() {
	memset(trie, -1, sizeof(trie)); //ȫ����ʼΪ-1,��ʾ��ǰ��û�д洢�ַ�
	pos = 0; 	//��ǰ�·���Ĵ洢λ��,��num[pos]���洢������ǰ�����ַ���������(pos>=1)
}
//�����ַ���
void insert(char s[]) { 
	int cur = 0; //�ֵ����ڼ���(-1��Ϊ�����)
	for (int i = 0; s[i]; i++) {
		int index = s[i] - 'a';
		if (trie[cur][index] == -1) // ��Ӧ�ַ�û��ֵ 
			trie[cur][index] = ++pos;
		cur = trie[cur][index]; //�������� 
		num[cur]++; //ÿ��ǰ׺�ĵ�������+1 
	}
	++endS[cur];	//�Ե�ǰ����β�ĵ�������+1
}
//��ѯĳ���ַ����Ƿ����
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
//��ѯ����ĳ��ǰ׺���ַ����Ƿ����,�������0��ʾ���ǰ׺������
int prefixNumber(char s[]) { //�����Ը��ַ���Ϊǰ׺�ĵ��ʵ�����
	int cur = 0; 
	for (int i = 0; s[i]; i++) {
		int index = s[i] - 'a';
		if (trie[cur][index] == -1) // ��Ӧ�ַ�û��ֵ
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


//Trie���������� "try"������˵ ǰ׺�� ��һ���������ݽṹ��
//���ڸ�Ч�ش洢�ͼ����ַ������ݼ��еļ�����һ���ݽṹ���൱���Ӧ���龰��
//�����Զ������ƴд��顣
//
//����ʵ�� Trie �ࣺ 
//    Trie() ��ʼ��ǰ׺������
//    void insert(String word) ��ǰ׺���в����ַ��� word ��
//    boolean search(String word) ����ַ��� word ��ǰ׺���У����� true�������ڼ���֮ǰ�Ѿ����룩�����򣬷��� false ��
//    boolean startsWith(String prefix) ���֮ǰ�Ѿ�������ַ��� word ��ǰ׺֮һΪ prefix ������ true �����򣬷��� false ��
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
