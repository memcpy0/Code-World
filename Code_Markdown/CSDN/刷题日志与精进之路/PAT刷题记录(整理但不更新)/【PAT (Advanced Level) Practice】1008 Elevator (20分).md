The highest building in our city has only one elevator. A request list is made up with $N$ positive numbers. The numbers denote at which floors the elevator will stop, in specified order. It costs 6 seconds to move the elevator up one floor, and 4 seconds to move down one floor. The elevator will stay for 5 seconds at each stop.

For a given request list, you are to compute the total time spent to fulfill the requests on the list. The elevator is on the 0th floor at the beginning and does not have to return to the ground floor when the requests are fulfilled.
### Input Specification:

Each input file contains one test case. Each case contains a positive integer $N$, followed by $N$ positive numbers. All the numbers in the input are less than 100.
### Output Specification:

For each test case, print the total time on a single line.
### Sample Input:

```groovy
3 2 3 1
```

### Sample Output:

```groovy
41
```
以前的写法好生涩啊：
```groovy
#include <bits/stdc++.h>

int main() {
    int n; //All the numbers in the input are less than 100
    scanf("%d", &n); //n > 0
    int elev[n];
    for (int i = 0; i < n; i++) scanf("%d", &elev[i]);
    int time = 0;
    for (int i = 0; i < n; i++) {
        if (i == 0 && elev[i] == 0) time += 5; //一开始就要在第0层等5s
        else if (i == 0 && elev[i] != 0) time += (elev[i] - 0) * 6 + 5; //一开始直接上去, 然后停下5s
        else { //i > 0
            int t = elev[i] - elev[i - 1]; //层数
            time += t > 0 ? t * 6 + 5 : -t * 4 + 5; //上楼或下楼
        }
    }
    printf("%d\n", time);
    return 0;
}
```
现在的代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
int n, prenum = 0, curnum;
int main() {
    scanf("%d", &n); 
    int totalTime = n * 5;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &curnum);
        int diff = curnum - prenum;
        totalTime += diff > 0 ? 6 * diff : 4 * abs(diff);
        prenum = curnum;
    } 
    printf("%d", totalTime);
    return 0;
}
```
