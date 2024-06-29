#include <iostream>
#include <vector> 
using namespace std;
// ����
int data[] = {1, 2, 3, 4, 5, 6, 8, 9, 10, 32, 15, 18, 33}; // �����е����� 
#define Swap(a, b) { int temp = a; a = b; b = temp; } 
//////////////////////////////////////////////////////////////////////////////////////////////// 

//���� 
//�ݹ�-��������Ȼ�������ȫ���У��������õ��Ľ�����ǰ����ֵ���ģ���ʹ�������������С�����У����Ѿ���С��������
//��Կ��ؼ���,���Եݹ齻�����ȫ����,Ȼ������+ȥ�� 
namespace swapRecursion {
	int num = 0; // ���еĸ���
	/* ȫ���� */
	void fullPermutation(int begin, int end) {
		if (begin == n) { // ������а�������������
			++num;  // ���м��� 
		    for (int i = 0; i <= n; ++i)  // ���������� 
				cout << data[i] << " ";
			cout << endl;
		} else {
			for (int i = begin; i <= end; ++i) {
				Swap(data[begin], data[i]); // ��1����n�����������潻���γ���n�����...�Դ����� 
				fullPermutation(begin + 1, end);
				Swap(data[begin], data[i]);
			}
		}
	}
	/* ��ӡn����������m������ȫ����, ��n!/(n-m)!�� */
	int m = 3;
	void permutation(int begin, int end) {
		if (begin == m) {
			++num;
			for (int i = 0; i < m; ++i) 
				cout << data[i] << " ";
			cout << endl;
		} else {
			for (int i = begin; i <= end; ++i) {
				Swap(data[begin], data[i]);
				permutation(begin + 1, end);
				Swap(data[begin], data[i]);
			}
		}
	}
}

namespace Algorithm { //�㷨�������ž���
	//��ӡ1-n��ȫ���� �ݹ�-ѡ��
	//������������С�����У���ô���ᰴ���ֵ������ȫ����
	//��Ȼ��Ҳ�����ȫ����
	void print_permutation(int n, int *A, int cur) {
		if (cur == n) {
			for (int i = 0; i < n; ++i) printf("%d ", A[i]);
			printf("\n");
		} else {
			for (int i = 1; i <= n; ++i) { //������A[cur]�����������i
				int ok = 1;
				for (int j = 0; j < cur; ++j)
					if (A[j] == i) ok = 0; //���i�Ѿ���A[0]~A[cur-1]�г��ֹ�,������ѡ
				if (ok) {
					A[cur] = i;
					print_permutation(n, A, cur + 1); //�ݹ����
				}
			}
		}
	}
}
/* ȫ���� */
//�ݹ�-ѡ��
class Solution {
private:
    vector<string> ans;
    void dfs(const string &s, int cur, int end, string& p) {
        if (cur >= end) { ans.emplace_back(p); return; }
        for (int i = 0; i < end; ++i) {
            bool ok = true;
            for (int j = 0; j < cur; ++j) if (s[i] == p[j]) { ok = false; break; }
            if (ok) {
                p[cur] = s[i];
                dfs(s, cur + 1, end, p);
            }
        }
    }
public:
    vector<string> permutation(string s) {
        string temp(s.size(), ' ');
        dfs(s, 0, s.size(), temp);
        return ans;
    }
};
/* ȫ���� */
//�ݹ�-ѡ�񷨡��ֵ��򡿣�������������С�����У���ô���ᰴ���ֵ������ȫ����
//��Ȼ��Ҳ�����ȫ����
namespace Solution {
	bool vis[10] = {0};
	vector<int> vi;
	void fullPermutation(int k, int end) {
		if (k > end) {
			++num;
			for (int i = 0; i < vi.size(); ++i)
			 	cout << vi[i] << " ";
			cout << endl;
			return;
		}
		for (int i = 0; i <= end; ++i) { //ÿ�δ�0��ʼ��Ϊ�˰���˳���ҵ���С��ֵ�������ͻᰴ���ֵ��� 
			if (vis[i] == true) continue; //���㷨�������������һ��,ֻ������ʹ����һ��ѭ�����i�Ƿ��Ѿ���ʹ��
			
			vi.push_back(data[i]);
			vis[i] = true;
			
			allPermutation(k + 1, end);
			
			vi.pop_back();
			vis[i] = false;
		}
	}
/* ��ӡn����������m������ȫ����, ��n!/(n-m)!�� */
	bool vis[10] = {0};
	vector<int> vi;
	int M = 3;
	void permutation(int k, int end) {
		if (k == M) {
			++num;
			for (int i = 0; i < vi.size(); ++i)
			 	cout << vi[i] << " ";
			cout << endl;
			return;
		}
		for (int i = 0; i <= end; ++i) {
			if (vis[i] == true) continue;
			
			vi.push_back(data[i]);
			vis[i] = true;
			
			Permutation(k + 1, end);
			
			vi.pop_back();
			vis[i] = false;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//�Ӽ�
{
	//�ݹ����Ӽ� 
	vector<int> subset;  
	void recurSubset(int begin, int end) {
		if (begin > end) {
			for (auto i : subset) 
				cout << i << " ";
			cout << endl;
			return;
		} 
		subset.push_back(begin);
		recurSubset(begin + 1, end);
		subset.pop_back();
		recurSubset(begin + 1, end);  
	}
	//�������Ӽ� 
	void printSubset(int n) {
		int total = (1 << n);
		for (int i = 0; i < total; ++i) { //2^n���Ӽ� 
			for (int j = 0; j < n; ++j)  // ÿ������Ҫ����n��Ԫ��, ���Ƿ������Ԫ�� 
				if (i & (1 << j)) cout << j << " ";
			cout << endl;
		} 
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//��� 
{ 
	int info[] = {2, 4, 5, 7, 9, 12}; // 15����� 
	//�ݹ������(�Ӽ��޸�) 
	vector<int> vi;  
	int k = 4;
	void recurCombine(int begin, int end) {
		if (vi.size() + (end - begin + 1) < t) return; // ��������
		if (vi.size() == k) { // �����㹻
			for (auto i : subset) 
				cout << i << " ";
			cout << endl;
			return;
		} 
		subset.push_back(begin); // ѡ����һԪ��
		recurSubset(begin + 1, end);
		
		subset.pop_back(); // ��ѡ
		recurSubset(begin + 1, end);  
	}

//�ݹ�DFS+����+��֦����� ��Ҫԭ�����Լ����򣿣���
	vector<int> c;
	int t = 4;
	void recurCombination(int begin, int end) {
		if (c.size() + (end - begin + 1) < t) return; // ��������
		if (c.size() == t) { // �����㹻
			for (auto i : c) 
				cout << i << " ";
			cout << endl;
			return;
		}
		for (int i = begin; i <= end; ++i) {
			if (!c.empty() && info[i] < c.back()) continue; //Ϊʲô? ̫С����Ҫ����һ�� 
			c.push_back(info[i]);
			recurCombination(i + 1, end);
			c.pop_back();
		}
	} 
	
	//���������(������ϼ�����,����ڲ�����)
	void Combination(int n, int k) {
		int tot = (1 << n);
		for (int i = 0; i < tot; ++i) {
			int num = 0, kk = i; //numͳ��i��1�ĸ���;k���ڴ���i 
			while (kk) {
				kk = kk & (kk - 1); //���k�����λ��1 
				++num;              //ͳ��1�ĸ��� 
			}
			if (num == k) {         //�����Ƶ�1��k��, ����Ҫ�� 
				for (int j = 0; j < n; ++j) 
					if (i & (1 << j)) cout << info[j] << " ";
				cout << endl;
			}
		}
	}
}
int main() {
	//fullPermutation(0, 9); // 3628800����� 10*9*8*7...*1 
	//fullPermutation(0, 3);       // 720����� 10!/(10-3)!=10*9*8=720 
	//allPermutation(0, 3);
	//Permutation(0, 9);  
	//cout << num << endl;
	//recurCombination(0, 5);
	//recurSubset(0, 3); 
	//printSubset(4);
	//Combination(6, 4);
	return 0;
} 
