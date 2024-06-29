#include <iostream>
#include <vector> 
using namespace std;
// 排列
int data[] = {1, 2, 3, 4, 5, 6, 8, 9, 10, 32, 15, 18, 33}; // 求排列的数据 
#define Swap(a, b) { int temp = a; a = b; b = temp; } 
//////////////////////////////////////////////////////////////////////////////////////////////// 

//排列 
//递归-交换法固然可以求出全排列，但是它得到的结果不是按照字典序的，即使输入的数据是最小的排列（即已经从小到大排序）
//针对可重集合,可以递归交换求出全排列,然后排序+去重 
namespace swapRecursion {
	int num = 0; // 排列的个数
	/* 全排列 */
	void fullPermutation(int begin, int end) {
		if (begin == n) { // 这个排列包含了所有数据
			++num;  // 排列计数 
		    for (int i = 0; i <= n; ++i)  // 输出这个排列 
				cout << data[i] << " ";
			cout << endl;
		} else {
			for (int i = begin; i <= end; ++i) {
				Swap(data[begin], data[i]); // 第1个数n种情况，与后面交换形成这n种情况...以此类推 
				fullPermutation(begin + 1, end);
				Swap(data[begin], data[i]);
			}
		}
	}
	/* 打印n个数中任意m个数的全排列, 共n!/(n-m)!个 */
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

namespace Algorithm { //算法竞赛入门经典
	//打印1-n的全排列 递归-选择法
	//如果输入的是最小的排列，那么它会按照字典序输出全排列
	//不然，也会输出全排列
	void print_permutation(int n, int *A, int cur) {
		if (cur == n) {
			for (int i = 0; i < n; ++i) printf("%d ", A[i]);
			printf("\n");
		} else {
			for (int i = 1; i <= n; ++i) { //尝试在A[cur]中填各种整数i
				int ok = 1;
				for (int j = 0; j < cur; ++j)
					if (A[j] == i) ok = 0; //如果i已经在A[0]~A[cur-1]中出现过,则不能再选
				if (ok) {
					A[cur] = i;
					print_permutation(n, A, cur + 1); //递归调用
				}
			}
		}
	}
}
/* 全排列 */
//递归-选择法
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
/* 全排列 */
//递归-选择法【字典序】，如果输入的是最小的排列，那么它会按照字典序输出全排列
//不然，也会输出全排列
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
		for (int i = 0; i <= end; ++i) { //每次从0开始是为了按照顺序找到最小的值，这样就会按照字典序 
			if (vis[i] == true) continue; //和算法竞赛的做法如出一辙,只是那里使用了一个循环检查i是否已经被使用
			
			vi.push_back(data[i]);
			vis[i] = true;
			
			allPermutation(k + 1, end);
			
			vi.pop_back();
			vis[i] = false;
		}
	}
/* 打印n个数中任意m个数的全排列, 共n!/(n-m)!个 */
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
//子集
{
	//递归求子集 
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
	//迭代求子集 
	void printSubset(int n) {
		int total = (1 << n);
		for (int i = 0; i < total; ++i) { //2^n个子集 
			for (int j = 0; j < n; ++j)  // 每个集合要遍历n个元素, 看是否含有这个元素 
				if (i & (1 << j)) cout << j << " ";
			cout << endl;
		} 
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//组合 
{ 
	int info[] = {2, 4, 5, 7, 9, 12}; // 15种情况 
	//递归求组合(子集修改) 
	vector<int> vi;  
	int k = 4;
	void recurCombine(int begin, int end) {
		if (vi.size() + (end - begin + 1) < t) return; // 数量不够
		if (vi.size() == k) { // 数量足够
			for (auto i : subset) 
				cout << i << " ";
			cout << endl;
			return;
		} 
		subset.push_back(begin); // 选择这一元素
		recurSubset(begin + 1, end);
		
		subset.pop_back(); // 不选
		recurSubset(begin + 1, end);  
	}

//递归DFS+排列+剪枝求组合 需要原数据自己有序？？？
	vector<int> c;
	int t = 4;
	void recurCombination(int begin, int end) {
		if (c.size() + (end - begin + 1) < t) return; // 数量不够
		if (c.size() == t) { // 数量足够
			for (auto i : c) 
				cout << i << " ";
			cout << endl;
			return;
		}
		for (int i = begin; i <= end; ++i) {
			if (!c.empty() && info[i] < c.back()) continue; //为什么? 太小了需要更大一点 
			c.push_back(info[i]);
			recurCombination(i + 1, end);
			c.pop_back();
		}
	} 
	
	//迭代求组合(各个组合间无序,组合内部有序)
	void Combination(int n, int k) {
		int tot = (1 << n);
		for (int i = 0; i < tot; ++i) {
			int num = 0, kk = i; //num统计i中1的个数;k用于处理i 
			while (kk) {
				kk = kk & (kk - 1); //清除k中最低位的1 
				++num;              //统计1的个数 
			}
			if (num == k) {         //二进制的1有k个, 符合要求 
				for (int j = 0; j < n; ++j) 
					if (i & (1 << j)) cout << info[j] << " ";
				cout << endl;
			}
		}
	}
}
int main() {
	//fullPermutation(0, 9); // 3628800种情况 10*9*8*7...*1 
	//fullPermutation(0, 3);       // 720种情况 10!/(10-3)!=10*9*8=720 
	//allPermutation(0, 3);
	//Permutation(0, 9);  
	//cout << num << endl;
	//recurCombination(0, 5);
	//recurSubset(0, 3); 
	//printSubset(4);
	//Combination(6, 4);
	return 0;
} 
