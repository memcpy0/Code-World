Suppose a bank has $N$ windows open for service. There is a yellow line in front of the windows which devides the waiting area into two parts. The rules for the customers to wait in line are:

- The space inside the yellow line in front of each window is enough to contain a line with $M$ customers. Hence when all the $N$ lines are full, all the customers after (and including) the $(NM+1)$st one will have to wait in a line behind the yellow line.
- Each customer will choose the shortest line to wait in when crossing the yellow line. If there are two or more lines with the same length, the customer will always choose the window with the smallest number.
- $Customer_i$​​ will take $T_i$​​ minutes to have his/her transaction processed.
- The first $N$ customers are assumed to be served at 8:00am.

Now given the processing time of each customer, you are supposed to tell the exact time at which a customer has his/her business done.

For example, suppose that a bank has 2 windows and each window may have 2 customers waiting inside the yellow line. There are 5 customers waiting with transactions taking 1, 2, 6, 4 and 3 minutes, respectively. At 08:00 in the morning, $customer_1$​​ is served at $window_1$​​ while $customer_2$​​ is served at $window_2$​​. $Customer_3$​​ will wait in front of $window_1$​​ and $customer_4$​​ will wait in front of $window_2$​​. $Customer_5$​​ will wait behind the yellow line.

At 08:01, $customer​_1$​​ is done and $customer​_5$​​ enters the line in front of $window_1$​​ since that line seems shorter now. $Customer_2$​​ will leave at 08:02, $customer_4$​​ at 08:06, $customer_3$​​ at 08:07, and finally $customer_5$​​ at 08:10.
## Input Specification:

Each input file contains one test case. Each case starts with a line containing 4 positive integers: $N$ ($≤20$, number of windows), $M$ ($≤10$, the maximum capacity of each line inside the yellow line), $K$ ($≤1000$, number of customers), and $Q$ ($≤1000$, number of customer queries).

The next line contains $K$ positive integers, which are the processing time of the $K$ customers.

The last line contains $Q$ positive integers, which represent the customers who are asking about the time they can have their transactions done. The customers are numbered from 1 to $K$ .
## Output Specification:

For each of the $Q$ customers, print in one line the time at which his/her transaction is finished, in the format `HH:MM` where `HH` is in `[08, 17]` and `MM` is in `[00, 59]` . Note that since the bank is closed everyday after 17:00, for those customers who cannot be served before 17:00, you must output `Sorry` instead.
**Sample Input:**

```cpp
2 2 7 5
1 2 6 4 3 534 2
3 4 5 6 7
```

**Sample Output:**

```cpp
08:07
08:06
08:10
17:00
Sorry
```

---
### 解法 队列+模拟
这一题确实有点麻烦，关键在于突破这几个难点：
- 顾客如何找到人数最少且序号最小的窗口；
- 如果顾客在17:00之前没有被服务到，他就不会被服务了，查询时输出 `Sorry` ；
- 如果顾客在17:00之前开始被服务，即使过了17:00的银行关门时间，也应该继续服务下去

为此设置 `start[]` 和 `proc[]` ，`start[i]` 表示这个顾客开始被服务的时间点，`proc[i]` 表示顾客办理业务需要的时间长度，`start[i] + proc[i]` 表示顾客结束业务办理的时间点。然后设置一个计时器 `now` ，每过一分钟就看一下哪些窗口有顾客完成业务办理，一直循环到17:00（不包括）。最后针对查询输出结果。
```cpp
#include <bits/stdc++.h>
using namespace std;
const int MAXC = 1010, BEGIN = 8 * 60, END = 17 * 60;
queue<int> wins[22];
int n, m, k, q, t, proc[MAXC], start[MAXC], last[MAXC], now = BEGIN, cur = 0;
int main() {
    scanf("%d%d%d%d", &n, &m, &k, &q); //窗口,容量,顾客,查询
    for (int i = 0; i < k; ++i) scanf("%d", &proc[i]);
    for (int i = 0; i < m && cur < k; ++i) {
        for (int j = 0; j < n && cur < k; ++j) {
            start[cur] = wins[j].empty() ? now : start[wins[j].back()] + proc[wins[j].back()]; //记录开始时间 
            wins[j].push(cur++);
        }
    }
    while (now < END) {
        for (int i = 0; i < n; ++i) { //所有窗口
            if (wins[i].empty()) continue;
            int c = wins[i].front(); //每个窗口的首位顾客
            if (start[c] + proc[c] <= now) { //处理完一位顾客的事
                wins[i].pop(); 
                if (cur < k) {
                    start[cur] = wins[i].empty() ? now : start[wins[i].back()] + proc[wins[i].back()]; //记录开始时间
                    wins[i].push(cur++); 
                }
            }
        }
        ++now; //时间过渡到下一分钟
    }
    for (int i = 0; i < q; ++i) {
        scanf("%d", &t);
        if (start[t - 1] == 0 || start[t - 1] >= END) printf("Sorry\n"); //没有在17:00之前进入队列 或者 没有17:00之前被服务到
        else printf("%02d:%02d\n", (start[t - 1] + proc[t - 1]) / 60, (start[t - 1] + proc[t - 1]) % 60);
    }
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210529052723753.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

