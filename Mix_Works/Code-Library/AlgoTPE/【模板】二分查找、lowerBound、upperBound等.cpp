#include <bits/stdc++.h>
using namespace std;
//���ܿ����õݹ�ʵ�֣���һ��Ѷ��ֲ���д�ɷǵݹ��
int bsearch(int *A, int x, int y, int v) { //[x,y) һ����ֲ���
	while (x < y) {
		int m = x + (y - x) / 2;
		if (A[m] == v) return m;
		else if (v < A[m]) y = m;
		else x = m + 1;
	}
	return -1;
}
int bsearch(int *A, int lo, int hi, int v) { //[lo,hi] һ����ֲ���
	while (lo <= hi) {
		int m = lo + (hi - lo) / 2;
		if (A[m] == v) return m;
		else if (v < A[m]) hi = m - 1;
		else lo = m + 1;
	}
	return -1;
}
//------------------------------------------------------------------------------------------- 
class Solution { 
public:
    int maxProfit(vector<int>& inventory, int orders) {
        const int mod = 1e9 + 7;
        using ull = unsigned long long;
        const vector<int>& inv = inventory;
        function<ull(ull, ull)> getVals = [&mod](ull b, ull e) -> ull { //����,ĩ�<=1e9
            ull m = b + e, n = e - b + 1; //mΪ�����ĩ���,nΪ����
            return m * n / 2 % mod; //m*n�������int��Χ
        };
        if (inv.size() == 1) return getVals(inv[0] - orders + 1, inv[0]); //ֻ��һ����ʱ
        int lo = 0, hi = *max_element(inv.begin(), inv.end()); //lo��hi����y��ı��
        while (lo < hi) { //�ҵ��������֮������<=orders�ĵ�һ���½�
            int mid = lo + (hi - lo) / 2, t = 0; 
            for (int v : inv) {
                if (v > mid) {
                    t += v - mid;
                    if (t > orders) break;
                }
            }
//            if (t <= orders) hi = mid;
//            else lo = mid + 1;
        } 
        ull T = lo, ans = 0, rest = orders;
        for (int v : inv) {
            if (v > T) {
                ans += getVals(T + 1, v);
                rest -= (v - T);
            }
        }
        ans += rest * T;
        return ans % mod;
    }
}; 
class Solution { 
public:
    int maxProfit(vector<int>& inventory, int orders) {
        const int mod = 1e9 + 7;
        using ull = unsigned long long;
        const vector<int>& inv = inventory;
        function<ull(ull, ull)> getVals = [&mod](ull b, ull e) -> ull { //����,ĩ�<=1e9
            ull m = b + e, n = e - b + 1; //mΪ�����ĩ���,nΪ����
            return m * n / 2 % mod; //m*n�������int��Χ
        };
        if (inv.size() == 1) return getVals(inv[0] - orders + 1, inv[0]); //ֻ��һ����ʱ
        int lo = 0, hi = *max_element(inv.begin(), inv.end()), T; //lo��hi����y��ı��
        while (lo <= hi) { //�ҵ��������֮������<=orders�ĵ�һ���½�
            int mid = lo + (hi - lo) / 2, t = 0; 
            for (int v : inv) {
                if (v > mid) {
                    t += v - mid;
                    if (t > orders) break;
                }
            }
//            if (t > orders) lo = mid + 1;
//            else {
//                T = mid;
//                hi = mid - 1;
//            }
        } 
        ull ans = 0, rest = orders;
        for (int v : inv) {
            if (v > T) {
                ans += getVals(T + 1, v);
                rest -= (v - T);
            }
        }
        ans += rest * T;
        return ans % mod;
    }
}; 
//------------------------------------------------------------------------------------------------------

//���ֲ��ҳ�������һЩ����ĳ��ϣ�û������A��Ҳû��Ҫ���ҵ�v�����Ƕ��ֵ�˼����Ȼ����

/*����������ж��Ԫ�ض���v������ĺ������ص�����һ�����±��أ���һ�������һ����������
�������Ԫ�ض���Ҫ�ҵģ������ص����м���һ����
�����Ľ�������Ǻ����룬�ܲ������ֵ����v������������

����ĺ�������v����ʱ�����������ֵĵ�һ��λ�á��������ص�һ��>=v�����ֳ��ֵĵ�һ��λ�ã�
��������ڣ���������һ���±�i��
�ڴ˴�����v��ԭ����Ԫ��A[i], A[i+1],��ȫ�������ƶ�һ��λ�ã���������Ȼ����
*/
/*����ĺ�������v����ʱ�����������ֵ����һ��λ�õĺ���һ��λ�á������ص�һ��>v�����ֳ��ֵĵ�һ��λ�ã�
��������ڣ���������һ���±�i��
�ڴ˴�����v��ԭ����Ԫ��A[i], A[i+1],��ȫ�������ƶ�һ��λ�ã���������Ȼ����
���ѿ�������v������ʱ��lowerBound��upperBound�ı���һ��
*/
//��һ��>=v��ֵλ�� (����) ��v�ĺ�̣� �����չ��ռ�
int lowerBound(int *A, int x, int y, int v) { //[x,y) ���ֲ������½�
    //���ķ���ֵ����������x, x+1, x+2,��, y-1����������y�������v����A[y-1]����ֻ�ܲ���������
    //���ܲ�������������ҿ�����[x,y)������ֵ�ĺ�ѡ����ȴ�Ǳ�����[x,y]
	while (x < y) { //x,y�г���һ��λ��
		int m = x + (y - x) / 2;
		//A[m]��v�ĸ��ֹ�ϵ
		//- A[m]>v������λ�ò������ں��棬���п�����m����������Ϊ[x,m]
		//- A[m]=v�������Ѿ��ҵ�һ��������߿��ܻ��У���������Ϊ[x,m]
		//- A[m]>v��m��ǰ�涼�����У���������Ϊ[m+1,y]
		if (A[m] >= v) y = m; //A[m]��vʱ������Ϊ[x,m] ��������
		else x = m + 1;  //A[m]��vʱ������Ϊ[m+1,y] ����������
	}
	return x;
}
//��һ��>v��ֵλ�� (����) ��v�����̣� �����չ��ռ�
int upperBound(int *A, int x, int y, int v) {
	//���ķ���ֵ����������x, x+1, x+2,��, y-1����������y�������v����A[y-1]����ֻ�ܲ���������
    //���ܲ�������������ҿ�����[x,y)������ֵ�ĺ�ѡ����ȴ�Ǳ�����[x,y]
	while (x < y) {
		int m = x + (y - x) / 2;
		//A[m]��v�ĸ��ֹ�ϵ
		//- A[m]>v������λ�ò������ں��棬���п�����m����������Ϊ[x,m]
		//- A[m]=v��m��ǰ�涼�����У����ұ߻��п��ܣ���������Ϊ[m+1,y]
		//- A[m]<v��m��ǰ�涼�����У���������Ϊ[m+1,y]
		if (A[m] > v) y = m;
		else x = m + 1;
	}
	return x;
}
//���һ��>=v��ֵλ�� (����) ������ 

//���һ��>v��ֵλ�� (����) ������ 

//��һ��<=v��ֵλ�� (����) ������ 

//��һ��<v��ֵλ�� (����) ������ 

//���һ��<=v��ֵλ�� (����) ������ ��v��ǰ���� ��ǰ��չ��ռ� 

//���һ��<v��ֵλ�� (����) ������ ��v����ǰ���� ��ǰ��չ��ռ�



//��һ��>=v��ֵλ�� (����) ������ 

//��һ��>v��ֵλ�� (����) ������ 

//���һ��>=v��ֵλ�� (����) ������ ��v��ǰ���� ��ǰ��չ��ռ�

//���һ��>v��ֵλ�� (����) ������ ��v����ǰ���� ��ǰ��չ��ռ�

//��һ��<=v��ֵλ�� (����) ������ ��v�ĺ�̣� �����չ��ռ�

//��һ��<v��ֵλ�� (����) ������ ��v�����̣� �����չ��ռ�

//���һ��<=v��ֵλ�� (����) ������ 

//���һ��<v��ֵλ�� (����) ������ 


/*�Զ��ֲ��ҵ����۾���ԱȽ������ˣ�
��lower_bound��upper_bound�ķ���ֵ�ֱ�ΪL��R����v���ֵ�������Ϊ[L,R)��
������۵�v����ʱҲ��������ʱL=R������Ϊ�ա�

�ҵ�һ������Ϊ / ����ƫ���������
*****����ֻ��ע������,��lo��hi�ǵ������ߵݼ����޹ؽ�Ҫ
*/
//------------------------------------------------------------------------------------------------
//�������ƣ�����д���ҵ����һ��<=v�����ֳ��ֵ�λ�� R
int upperCut(int *A, int x, int y, int v) {
	//���ķ���ֵ����������x, x+1, x+2,��, y-1����������y�������v����A[y-1]����ֻ�ܲ���������
    // ���ܲ�������������ҿ�����[x,y)������ֵ�ĺ�ѡ����ȴ�Ǳ�����[x,y]
	while (x < y) {
		int m = x + (y - x) / 2;
		//A[m]��v�ĸ��ֹ�ϵ
		//- A[m]>v������λ�ò������ں��棬���п�����m����������Ϊ[x,m]
		//- A[m]=v��m��ǰ�涼�����У����ұ߻��п��ܣ���������Ϊ[m+1,y]
		//- A[m]<v��m��ǰ�涼�����У���������Ϊ[m+1,y]
		if (A[m] > v) y = m;
		else x = m + 1;
	}
	return x - 1; //��һ��>v��ֵ��λ��-1
}
//int upperCut(int *A, int x, int y, int v) {
//	while (x < y) {
//		int m = (x + y + 1) / 2;
//		if (A[m] <= v) x = m; //�ؼ���
//		else y = m - 1; //��bug,��������??? 0��1��Զ��0 0��1��Զ��1 
//	}
//	return x;
//}
//���һ��<v�����ֳ��ֵ�λ�� L  (L,R]
int lowerCut(int *A, int x, int y, int v) { //[x,y) ���ֲ������½�
    //���ķ���ֵ����������x, x+1, x+2,��, y-1����������y�������v����A[y-1]����ֻ�ܲ���������
    //���ܲ�������������ҿ�����[x,y)������ֵ�ĺ�ѡ����ȴ�Ǳ�����[x,y]
	while (x < y) { //x,y�г���һ��λ��
		int m = x + (y - x) / 2;
		//A[m]��v�ĸ��ֹ�ϵ
		//- A[m]>v������λ�ò������ں��棬���п�����m����������Ϊ[x,m]
		//- A[m]=v�������Ѿ��ҵ�һ��������߿��ܻ��У���������Ϊ[x,m]
		//- A[m]>v��m��ǰ�涼�����У���������Ϊ[m+1,y]
		if (A[m] >= v) y = m; //A[m]��vʱ������Ϊ[x,m] ��������
		else x = m + 1;  //A[m]��vʱ������Ϊ[m+1,y] ����������
	}
	return x - 1; //��һ��>=v��ֵ��λ��-1
}
//int lowerCut(int *A, int x, int y, int v) {
//	while (x < y) {
//		int m = (x + y + 1) / 2;
//		if (A[m] < v) x = m; //�ؼ���//������!����ȷ!
//		else y = m - 1;
//	}
//	return x;
//}

int main() {
	List L;
	ElementType X;
	Position P;
	
	L = ReadInput();
	scanf("%d", &X);
	P = BinarySearch(L, X);
	printf("%d\n", P);
	return 0;
}
