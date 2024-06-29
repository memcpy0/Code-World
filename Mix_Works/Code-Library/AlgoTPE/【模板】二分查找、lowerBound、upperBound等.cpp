#include <bits/stdc++.h>
using namespace std;
//尽管可以用递归实现，但一般把二分查找写成非递归的
int bsearch(int *A, int x, int y, int v) { //[x,y) 一般二分查找
	while (x < y) {
		int m = x + (y - x) / 2;
		if (A[m] == v) return m;
		else if (v < A[m]) y = m;
		else x = m + 1;
	}
	return -1;
}
int bsearch(int *A, int lo, int hi, int v) { //[lo,hi] 一般二分查找
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
        function<ull(ull, ull)> getVals = [&mod](ull b, ull e) -> ull { //首项,末项都<=1e9
            ull m = b + e, n = e - b + 1; //m为首项和末项和,n为项数
            return m * n / 2 % mod; //m*n可能溢出int范围
        };
        if (inv.size() == 1) return getVals(inv[0] - orders + 1, inv[0]); //只有一种球时
        int lo = 0, hi = *max_element(inv.begin(), inv.end()); //lo和hi都是y轴的标尺
        while (lo < hi) { //找到满足横线之上球数<=orders的第一个下界
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
        function<ull(ull, ull)> getVals = [&mod](ull b, ull e) -> ull { //首项,末项都<=1e9
            ull m = b + e, n = e - b + 1; //m为首项和末项和,n为项数
            return m * n / 2 % mod; //m*n可能溢出int范围
        };
        if (inv.size() == 1) return getVals(inv[0] - orders + 1, inv[0]); //只有一种球时
        int lo = 0, hi = *max_element(inv.begin(), inv.end()), T; //lo和hi都是y轴的标尺
        while (lo <= hi) { //找到满足横线之上球数<=orders的第一个下界
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

//二分查找常常用在一些抽象的场合，没有数组A，也没有要查找的v，但是二分的思想仍然适用

/*如果数组中有多个元素都是v，上面的函数返回的是哪一个的下标呢？第一个？最后一个？都不是
如果所有元素都是要找的，它返回的是中间那一个。
这样的结果并不是很理想，能不能求出值等于v的完整区间呢

下面的函数，当v存在时，返回它出现的第一个位置。（即返回第一个>=v的数字出现的第一个位置）
如果不存在，返回这样一个下标i：
在此处插入v（原来的元素A[i], A[i+1],…全部往后移动一个位置）后序列仍然有序
*/
/*下面的函数，当v存在时，返回它出现的最后一个位置的后面一个位置。（返回第一个>v的数字出现的第一个位置）
如果不存在，返回这样一个下标i：
在此处插入v（原来的元素A[i], A[i+1],…全部往后移动一个位置）后序列仍然有序
不难看出，当v不存在时，lowerBound和upperBound的表现一致
*/
//第一个>=v的值位置 (升序) （v的后继） 向后扩展解空间
int lowerBound(int *A, int x, int y, int v) { //[x,y) 二分查找求下界
    //最后的返回值不仅可能是x, x+1, x+2,…, y-1，还可能是y——如果v大于A[y-1]，就只能插入这里了
    //尽管查找区间是左闭右开区间[x,y)，返回值的候选区间却是闭区间[x,y]
	while (x < y) { //x,y夹出来一个位置
		int m = x + (y - x) / 2;
		//A[m]和v的各种关系
		//- A[m]>v：所求位置不可能在后面，但有可能是m，因此区间变为[x,m]
		//- A[m]=v：至少已经找到一个，而左边可能还有，因此区间变为[x,m]
		//- A[m]>v：m和前面都不可行，因此区间变为[m+1,y]
		if (A[m] >= v) y = m; //A[m]≥v时新区间为[x,m] 满足条件
		else x = m + 1;  //A[m]＜v时新区间为[m+1,y] 不满足条件
	}
	return x;
}
//第一个>v的值位置 (升序) （v的真后继） 向后扩展解空间
int upperBound(int *A, int x, int y, int v) {
	//最后的返回值不仅可能是x, x+1, x+2,…, y-1，还可能是y——如果v大于A[y-1]，就只能插入这里了
    //尽管查找区间是左闭右开区间[x,y)，返回值的候选区间却是闭区间[x,y]
	while (x < y) {
		int m = x + (y - x) / 2;
		//A[m]和v的各种关系
		//- A[m]>v：所求位置不可能在后面，但有可能是m，因此区间变为[x,m]
		//- A[m]=v：m和前面都不可行，而右边还有可能，因此区间变为[m+1,y]
		//- A[m]<v：m和前面都不可行，因此区间变为[m+1,y]
		if (A[m] > v) y = m;
		else x = m + 1;
	}
	return x;
}
//最后一个>=v的值位置 (升序) 无意义 

//最后一个>v的值位置 (升序) 无意义 

//第一个<=v的值位置 (升序) 无意义 

//第一个<v的值位置 (升序) 无意义 

//最后一个<=v的值位置 (升序) 有意义 （v的前驱） 向前扩展解空间 

//最后一个<v的值位置 (升序) 有意义 （v的真前驱） 向前扩展解空间



//第一个>=v的值位置 (降序) 无意义 

//第一个>v的值位置 (降序) 无意义 

//最后一个>=v的值位置 (降序) 有意义 （v的前驱） 向前扩展解空间

//最后一个>v的值位置 (降序) 有意义 （v的真前驱） 向前扩展解空间

//第一个<=v的值位置 (降序) 有意义 （v的后继） 向后扩展解空间

//第一个<v的值位置 (降序) 有意义 （v的真后继） 向后扩展解空间

//最后一个<=v的值位置 (降序) 无意义 

//最后一个<v的值位置 (降序) 无意义 


/*对二分查找的讨论就相对比较完整了：
设lower_bound和upper_bound的返回值分别为L和R，则v出现的子序列为[L,R)。
这个结论当v不在时也成立：此时L=R，区间为空。

找第一个是因为 / 总是偏向区间起点
*****二分只关注单调性,从lo到hi是递增或者递减都无关紧要
*/
//------------------------------------------------------------------------------------------------
//依次类推，还能写出找到最后一个<=v的数字出现的位置 R
int upperCut(int *A, int x, int y, int v) {
	//最后的返回值不仅可能是x, x+1, x+2,…, y-1，还可能是y——如果v大于A[y-1]，就只能插入这里了
    // 尽管查找区间是左闭右开区间[x,y)，返回值的候选区间却是闭区间[x,y]
	while (x < y) {
		int m = x + (y - x) / 2;
		//A[m]和v的各种关系
		//- A[m]>v：所求位置不可能在后面，但有可能是m，因此区间变为[x,m]
		//- A[m]=v：m和前面都不可行，而右边还有可能，因此区间变为[m+1,y]
		//- A[m]<v：m和前面都不可行，因此区间变为[m+1,y]
		if (A[m] > v) y = m;
		else x = m + 1;
	}
	return x - 1; //第一个>v的值的位置-1
}
//int upperCut(int *A, int x, int y, int v) {
//	while (x < y) {
//		int m = (x + y + 1) / 2;
//		if (A[m] <= v) x = m; //关键！
//		else y = m - 1; //有bug,问题在于??? 0和1永远是0 0和1永远是1 
//	}
//	return x;
//}
//最后一个<v的数字出现的位置 L  (L,R]
int lowerCut(int *A, int x, int y, int v) { //[x,y) 二分查找求下界
    //最后的返回值不仅可能是x, x+1, x+2,…, y-1，还可能是y——如果v大于A[y-1]，就只能插入这里了
    //尽管查找区间是左闭右开区间[x,y)，返回值的候选区间却是闭区间[x,y]
	while (x < y) { //x,y夹出来一个位置
		int m = x + (y - x) / 2;
		//A[m]和v的各种关系
		//- A[m]>v：所求位置不可能在后面，但有可能是m，因此区间变为[x,m]
		//- A[m]=v：至少已经找到一个，而左边可能还有，因此区间变为[x,m]
		//- A[m]>v：m和前面都不可行，因此区间变为[m+1,y]
		if (A[m] >= v) y = m; //A[m]≥v时新区间为[x,m] 满足条件
		else x = m + 1;  //A[m]＜v时新区间为[m+1,y] 不满足条件
	}
	return x - 1; //第一个>=v的值的位置-1
}
//int lowerCut(int *A, int x, int y, int v) {
//	while (x < y) {
//		int m = (x + y + 1) / 2;
//		if (A[m] < v) x = m; //关键！//不靠谱!不正确!
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
