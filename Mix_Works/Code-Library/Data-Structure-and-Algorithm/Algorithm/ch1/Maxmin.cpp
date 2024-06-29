//例1.5
#include <bits/stdc++.h>
using namespace std;
//求解非空数组中的最大值和最小值
void Maxmin(int arr[], int n, int &mx, int &mn) {
    mx = mn = arr[0];
    for (int i = 1; i < n; ++i) {
        if (arr[i] > mx) mx = arr[i];
		else if (arr[i] < mn) mn = arr[i];
    }
}
int main() {
    int a[] = {2,1,5,6,4,3,8,9,7}, mx, mn;
    int n = 9;
    Maxmin(a, n, mx, mn);
    printf("Max=%d, Min=%d\n", mx, mn);
    return 0;
}
