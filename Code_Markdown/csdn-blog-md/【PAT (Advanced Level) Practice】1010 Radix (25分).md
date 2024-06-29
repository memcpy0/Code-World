Given a pair of positive integers, for example, 6 and 110, can this equation 6 = 110 be true? The answer is `yes`, if 6 is a decimal number and 110 is a binary number.

Now for any pair of positive integers $N_1$​​ and $N_2$​​, your task is to find the radix of one number while that of the other is given.
### Input Specification:

Each input file contains one test case. Each case occupies a line which contains 4 positive integers:
```cpp
N1 N2 tag radix
```
Here `N1` and `N2` each has no more than 10 digits. A digit is less than its radix and is chosen from the set `{0-9, a-z}` where `0-9` represent the decimal numbers `0-9`, and `a-z` represent the decimal numbers `10-35`. The last number radix is the radix of `N1` if `tag` is 1 , or of `N2` if `tag` is 2.
### Output Specification:

For each test case, print in one line the radix of the other number so that the equation `N1 = N2` is true. If the equation is impossible, print `Impossible`. If the solution is not unique, output the smallest possible radix.
### Sample Input 1:

```groovy
6 110 1 10
```

### Sample Output 1:

```groovy
2
```

---
```cpp
#include <bits/stdc++.h>
typedef long long ll;
using namespace std;
ll Map[256];
ll inf = ((ll)1 << 63) - 1; //long long的最大值, 注意类型转换和加括号
void init() {
    for (char c = '0'; c <= '9'; c++) {
        Map[c] = c - '0'; //将'0'-'9'映射到0-9
    }
    for (char c = 'a'; c <= 'z'; c++) {
        Map[c] = c - 'a' + 10; //将'a'-'z'映射到10-35
    }
}
ll convertNum10(char a[], ll radix, ll t) { //将radix进制的a转换为十进制, t为进制上界
    ll ans = 0;
    for (int i = 0; a[i]; i++) {
        ans = ans * radix + Map[a[i]]; //进制转换
        if (ans < 0 || ans > t) return -1; //未知进制的数溢出或超过N1的十进制
    }
    return ans;
}
int findMaxDigit(char n2[]) { //求最大的数位
    int ans = -1;
    for (int i = 0; n2[i]; i++) {
        if (Map[n2[i]] > ans) ans = Map[n2[i]];
    }
    return ans + 1; //最大的数位为ans, 说明进制数的底线是ans+1
}

int cmp(char n2[], ll radix, ll t) { //N2字符串的十进制与t比较
    ll num = convertNum10(n2, radix, t); //将N2转换为十进制
    if (num < 0) return 1; //溢出, 肯定N2 > t
    if (t > num) return -1; //t稍大, 返回-1
    else if (t == num) return 0; //相等, 返回0
    else return 1; //num稍大, 返回1
}
//二分查找N2合适的进制, 使得N1和N2转换为十进制后相等
ll binarySearch(char n2[], ll left, ll right, ll t) { //二分求解N2的进制
    ll mid;
    while (left <= right) {
        mid = (left + right) / 2;
        int flag = cmp(n2, mid, t); //判断N2转换为十进制后与t进制比较
        if (flag == 0) return mid;  //找到解, 返回mid
        else if (flag == -1) left = mid + 1; //往右子区间继续查找
        else right = mid - 1; //往左子区间继续查找
    }
    return -1; //不存在解
}

char n1[20], n2[20], temp[20];
int main() {
    init();
    int tag, radix;
    scanf("%s %s %d%d", n1, n2, &tag, &radix);
    if (tag == 2) {
        strcpy(temp, n1);
        strcpy(n1, n2);
        strcpy(n2, temp); //交换, 以确定radix为n1的进制
    }
    ll t = convertNum10(n1, radix, inf); //将N1从radix进制转换为十进制
    ll low = findMaxDigit(n2); //找到n2中数位最大的位+1, 作为二分下界
    ll high = max(low, t) + 1; //二分进制的上界
    ll ans = binarySearch(n2, low, high, t); //二分
    if (ans == -1) printf("Impossible\n");
    else printf("%lld\n", ans); //N1和N2相等时
    return 0;
}
```

---
题目给的测试用例太浅了，让我给点难的，不过通过了下面的用例也不能过，还有一些用例可能会在转换为十进制数时溢出，导致出现错误的结果（这里没有）：
```cpp
N1       N2  tag radix   N2进制
10       a   1   10   -> 11; 
35       z   1   10   -> 36; 
37       10  1   10   -> 37; 
100      10  1   10   -> 100; 
7fffffff 10  1   16   -> 2147483647
7ffffffe 20  1   16   -> 1073741823
7fffffff 20  1   16   -> Impossible
```

我新写的代码：

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
char N1[12], N2[12], N[12];//N存储未知进制的那个数的字符串
ll tag, radix, x, t, lo = 2, hi = 37;  //x存储已知进制的那个数的十进制值,t存储未知进制的那个数的十进制值; lo, hi二分搜索 
bool flag = false;
ll toDigit(char c) { return isdigit(c) ? c - '0' : c - 'a' + 10; }
ll convertToDecimal(char s[], ll radix) { //将s按照进制radix转换为十进制数值
    ll ans = 0;
    for (int i = 0; s[i]; ++i) {
        ans = ans * radix + toDigit(s[i]);
        if (ans >= x || ans < 0) flag = true; //此时s转换的进制数一定>=x
    }
    return ans;
}
void binarySearch() { //计算第一个进制,满足未知进制的那个数的十进制值>=x 
    while (lo < hi) {
        ll mid = lo + (hi - lo) / 2; //中间进制
        flag = false;           //flag作为标记
        t = convertToDecimal(N, mid); //按照mid进制计算未知进制的那个数的十进制值 
        if (flag) hi = mid; 
        else lo = mid + 1; 
    }
} 
int main() {
    scanf("%s%s%lld%lld", N1, N2, &tag, &radix);         //4个正整数
    x = convertToDecimal(tag == 1 ? N1 : N2, radix); //得到N1或N2(radix)的十进制值
    memcpy(N, tag == 1 ? N2 : N1, sizeof(N2)); //从N2或N1中拷贝内容到N中
    for (int i = 0; N[i]; ++i)  
        lo = max(lo, 1ll + toDigit(N[i])); //得到最低进制的准确值,避免N="a"时可以使用2进制这种情况
    hi = max(x, hi) + 1;
    binarySearch();
    t = convertToDecimal(N, lo);
    if (t == x) printf("%lld", lo); //找到这个进制,使得两者相等
    else printf("Impossible");
    return 0;
}
```

