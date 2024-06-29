### 3233 Download Manager
- Problem Description
Jiajia downloads a lot, a lot more than you can even imagine. Some say that he starts downloading up to 20,000 files together. If 20,000 files try to share a limited bandwidth then it will be a big hazard and no files will be downloaded properly. That is why, he uses a download manager.

	If there are T files to download, the download manger uses the following policy while downloading files:

>1. The download manager gives the smaller files higher priority, so it starts downloading the smallest n files at startup. If there is a tie, download manager chooses the one with less bytes remaining (for download). We assume that with at least 50 Mega Bytes/sec of bandwidth, n files can be downloaded simultaneously without any problem.
>2. The available bandwidth is equally shared by the all the files that are being downloaded. When a file is completely downloaded its bandwidth is instantaneously given to the next file. If there are no more files left except the files that are being downloaded, this bandwidth is immediately shared equally by all remaining files that are being downloaded.

Given the size and completed percentage of each file, your task is to intelligently simulate the behavior of the download manager to find the total time required to download all the files.
- Input
The will be at most 10 test cases. Each case begins with three integers T (1 <= T <= 20000), n (1 <= n <= 2000 and 1 <= n <= T) and B (50 <= B <= 1000). Here B denotes the total bandwidth available to Jiajia (In Megabytes/sec). Please note that the download manager always downloads n files in parallel unless there are less than n files available for download. Each of next T lines contains one non-negative floating-point number S (less than 20,000, containing at most 2 digits after the decimal places) and one integer P (0 <= P <= 100). These two numbers denote a file whose size is S megabyte and which has been downloaded exactly P% already. Also note that although theoretically it is not possible that the size of a file or size of its remaining part is a fraction when expressed in bytes, for simplicity please assume that such thing is possible in this problem. The last test case is followed by T=n=B=0, which should not be processed.
- Output
For each case, print the case number and the total time required to download all the files, expressed in hours and rounded to 2 digits after the decimal point. Print a blank line after the output of each test case.
- Sample Input
	```
	6 3 90
	100.00 90
	40.40 70
	60.30 70
	40.40 80
	40.40 85
	40.40 88
	1 1 56
	12.34 100
	0 0 0
	```
- Sample Output
	```
	Case 1: 0.66
	
	Case 2: 0.00
	
	```
题意：最多有20000个文件要下载。给出要下载的文件数T，带宽B和并行下载的文件数n。然后给出T个文件的大小s和未下载的比例p。要求求出下载用时，精确到小数点后两位。下载器采用的策略如下：
 1. 越小的文件越早开始下；如果文件大小相等，就先下载p最小的文件；
 2. 带宽可以容纳n个文件同时下载，平均分配给每个文件带宽，一个文件下载后就立刻按照规则1下载下一个文件。

这是一道让人无语的题目。 
思路一：顺着题意写代码。先按照规则排序；再用一个静态带表头的双端链表，结点记录pre和next，和该文件剩余的字节；然后就是模拟，一次性下载n个，每次把第一个下载完了移除(修改pre和next)，一定是字节最少r_min的文件先下载完，累计这最小的时间(B带宽是同时平均分给n个文件的，所以`time_min = r_min/(B/n)`)，同时更新剩余正在下载的n-1个文件的字节，直到所有文件都下载完(T==0)；最后累加的时间即为输出结果。

思路二：如果顺着题意写不出来代码，会发现，因为**要下载的总量是一定的，总的下载速度及带宽也是一定的**，所以消耗时间不受文件下载的先后顺序影响。只需要理解成：用B的速度下载n个文件所有剩余的大小总和。因为题目要求，一个文件下载完成后，他所分得的流量马上分给其他的文件，也就是说在下载期间一直保持B的速度。
```c
#include <cstdio>
int main() {
	int t, n, b; //总文件数 并行文件下载数 带宽  
	int c = 1;
	while (~scanf("%d%d%d", &t, &n, &b) && (t || n || b)) {
		double s, p, sum = 0; //每个文件的大小和已经下了多少
	    while (t--) {
            scanf("%lf%lf", &s, &p);
            sum += s * (100 - p) * 0.01;
		}	
		printf("Case %d: %.2lf\n\n", c++, sum / (b * 1.0)); 
	} 
	return 0;
}
```
