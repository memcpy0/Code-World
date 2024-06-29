/*
从 1∼n 这 n 个整数中随机选出 m 个，输出所有可能的选择方案。
输入格式

两个整数 n,m,在同一行用空格隔开。
输出格式

按照从小到大的顺序输出所有方案，每行 1个。

首先，同一行内的数升序排列，相邻两个数用一个空格隔开。
其次，对于两个不同的行，对应下标的数一一比较，字典序较小的排在前面（例如 1 3 5 7 排在 1 3 6 8 前面）。

数据范围

n>0, 0≤m≤n , n+(n−m)≤25

输入样例：

5 3

输出样例：

1 2 3 
1 2 4 
1 2 5 
1 3 4 
1 3 5 
1 4 5 
2 3 4 
2 3 5 
2 4 5 
3 4 5 

思考题：如果要求使用非递归方法，该怎么做呢？
*/
//迭代
//不考虑生成组合数组（组合内有序，组合间无序）：

//普通递归
//考虑生成组合数组（从小到大枚举，组合间/内有序）：
#include <bits/stdc++.h>
using namespace std;
int n, m;
vector<int> chosen;
void dfs(int begin, int end) {
    if (chosen.size() + (end - begin + 1) < m) return; // 数量太少
    if (chosen.size() == m) {
        for (int i = 0; i < chosen.size(); ++i)
            printf("%d ", chosen[i]);
        printf("\n");
        return;
    }
    chosen.push_back(begin);
    dfs(begin + 1, end);
    chosen.pop_back();
    dfs(begin + 1, end);
}
int main() {
    scanf("%d%d", &n, &m);
    dfs(1, n);  
    return 0;   
}

//状态压缩递归
//不考虑生成组合数组（从小到大枚举，组合间/内有序）：
#include <bits/stdc++.h>
using namespace std;
int n, m, state = 0;
void dfs(int begin, int end, int cnt) {
    if (cnt + (end - begin + 1) < m) return; // 数量太少
    if (cnt == m) {
        for (int i = 0; i < 32; ++i)
            if ((state >> i) & 1)
                printf("%d ", i + 1);
        printf("\n");
        return;
    }
    state |= (1 << begin);
    dfs(begin + 1, end, cnt + 1);
    state &= ~(1 << begin);
    dfs(begin + 1, end, cnt);
}
int main() {
    scanf("%d%d", &n, &m);
    dfs(0, n - 1, 0);   
    return 0;   
}