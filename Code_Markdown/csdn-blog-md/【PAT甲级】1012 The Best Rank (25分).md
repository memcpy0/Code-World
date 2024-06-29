To evaluate the performance of our first year CS majored students, we consider their grades of three courses only: `C` - C Programming Language, `M` - Mathematics (Calculus or Linear Algrbra), and `E` - English. At the mean time, we encourage students by emphasizing on their best ranks -- that is, among the four ranks with respect to the three courses and the average grade, we print the best rank for each student.

For example, The grades of `C`, `M`, `E` and `A` - Average of 4 students are given as the following:

```swift
StudentID  C  M  E  A
310101     98 85 88 90
310102     70 95 88 84
310103     82 87 94 88
310104     91 91 91 91
```

Then the best ranks for all the students are `No.1` since the 1st one has done the best in C Programming Language, while the 2nd one in Mathematics, the 3rd one in English, and the last one in average.
### Input Specification:

Each input file contains one test case. Each case starts with a line containing 2 numbers $N$ and $M (≤2000)$, which are the total number of students, and the number of students who would check their ranks, respectively. Then `N` lines follow, each contains a student ID which is a string of 6 digits, followed by the three integer grades (in the range of [0, 100]) of that student in the order of `C`, `M` and `E`. Then there are $M$ lines, each containing a student ID.
### Output Specification:

For each of the $M$ students, print in one line the best rank for him/her, and the symbol of the corresponding rank, separated by a space.

The priorities of the ranking methods are ordered as `A` > `C` > `M` > `E`. Hence if there are two or more ways for a student to obtain the same best rank, output the one with the highest priority.

If a student is not on the grading list, simply output `N/A`.
### Sample Input:

```swift
5 6
310101 98 85 88
310102 70 95 88
310103 82 87 94
310104 91 91 91
310105 85 90 90
310101
310102
310103
310104
310105
999999
```

### Sample Output:

```swift
1 C
1 M
1 E
1 A
3 A
N/A
```


---
### 解法 排序+哈希表
题目中没有说明相同分数的等级是否相同，不过一般来说PAT中都是这样排名的。此外还需要注意的是：
- 为了避免浮点误差，代码中使用总分而非平均分；
- 不用存储各个科目的排名数据，排序后直接计算每个人的该科排名，并比较更新每个人的最优排名;
- 直接用 `unordered_map` 索引最优名次和科目
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 2010;
int n, m, t;
char courses[4] = {'A', 'C', 'M', 'E'};
struct stu { //g[0:3]即ACME,best是最高的rank,idx是最高rank对应科目
    int id, g[4] = {0}, best = maxn, idx = -1; 
} arr[maxn];
using bi = pair<int, int>; //best, idx
unordered_map<int, bi> rec;
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &arr[i].id); //CME,A=C+M+E
        for (int j = 1; j <= 3; ++j) { scanf("%d", &arr[i].g[j]); arr[i].g[0] += arr[i].g[j]; } 
    }
    for (int i = 0; i < 4; ++i) { //按照每一科的成绩排序
        sort(arr, arr + n, [&](const stu& a, const stu& b) { return a.g[i] > b.g[i]; });
        int r = 1;
        for (int j = 0; j < n; ++j) {
            if (j && arr[j].g[i] == arr[j - 1].g[i]) ; //成绩相等,名次为r
            else r = j + 1; //j为0,或者成绩更低
            if (r < arr[j].best) arr[j].best = r, arr[j].idx = i; //更新最高等级及相应科目
            if (i == 3) rec[arr[j].id] = bi{arr[j].best, arr[j].idx}; //最后一科时,记录id:(最高名次,相应科目)
        }
    }
    for (int i = 0; i < m; ++i) {
        scanf("%d", &t);
        if (rec.find(t) == rec.end()) printf("N/A\n");
        else printf("%d %c\n", rec[t].first, courses[rec[t].second]);
    }
    return 0;
}
```
