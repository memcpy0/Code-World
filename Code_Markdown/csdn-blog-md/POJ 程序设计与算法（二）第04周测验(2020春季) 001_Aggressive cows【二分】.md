```
总时间限制: 1000ms
内存限制:   65536kB
```
**描述**
    Farmer John has built a new long barn, with $N (2 <= N <= 100,000)$ stalls. The stalls are **located along a straight line** at positions $x_1,...,x_N (0 \leq x_i \leq 1,000,000,000)$.

 His $C (2 \leq C \leq N)$ cows don't like this barn layout and become **aggressive** towards **each other** once **put into a stall**. To prevent the cows from hurting each other, FJ want to assign the cows to the stalls, such that **the minimum distance between any two of them** is as **large** as possible. What is the **largest minimum distance**?

输入
 * Line 1: Two space-separated integers: $N$ and $C$

* Lines 2..N+1: Line i+1 contains an integer stall location, xi

输出
  * Line 1: One integer: the largest minimum distance

样例输入

    5 3
    1
    2
    8
    4
    9
    
样例输出

    3

提示  

    FJ can put his 3 cows in the stalls at positions 1, 4 and 8, resulting in a minimum distance of 3.
    Huge input data,scanf is recommended.
来源
 
     USACO 2005 February Gold 


### 一、我的代码
```cpp
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 101000;
int n, c;
int stalls[maxn];
int main() { 
	cin >> n >> c;
	for (int i = 0; i < n; ++i) 
		cin >> stalls[i];
	sort(stalls, stalls + n);
	int l = 1, r = (stalls[n - 1]) / c + 1; //[] //upper_bound模板 
	while (l < r) {  //找出第一个满足t>0的largestMinDistance 
		int largestMinDistance = (l + r) / 2;
		int preStall = 0, t = c - 1; 
		for (int i = 1; i < n; ++i) {
			if (stalls[i] - stalls[preStall] >= largestMinDistance) {
				preStall = i;
				--t; 
			}
		}  
		if (t > 0) { //还有牛放不下, 应该减少距离, LMD往左边找 
			r = largestMinDistance; 
		} else { //说明隔得距离太小了, 可以放更多的牛 //lMD可以往右边找 
			l = largestMinDistance + 1; 
		}
	}
	cout << l - 1; //l-1即最大的最小距离
	return 0;	
} 
```











