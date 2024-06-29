Implement the class `UndergroundSystem` that supports three methods:

1.`checkIn(int id, string stationName, int t)`

-    A customer with `id` card equal to `id`, gets in the station `stationName` at time `t`.
-    A customer can **only** be checked into one place **at a time**.

2.`checkOut(int id, string stationName, int t)`

-    A customer with id card equal to `id`, gets out from the station `stationName` at time t.

3.`getAverageTime(string startStation, string endStation)`

-    Returns the **average time** to travel between the `startStation and the endStation`.
-    The average time is computed from all the previous traveling from `startStation` to `endStation` that happened **directly**.
-    Call to `getAverageTime` is always valid.

You can assume all calls to `checkIn` and `checkOut` methods are **consistent**. That is, if a customer gets in at time $t_1$ at some station, then it gets out at time $t_2$ with $t_2 > t_1$. All events happen in chronological order.

 

Example 1:
```
Input
["UndergroundSystem","checkIn","checkIn","checkIn","checkOut","checkOut","checkOut","getAverageTime","getAverageTime","checkIn","getAverageTime","checkOut","getAverageTime"]
[[],[45,"Leyton",3],[32,"Paradise",8],[27,"Leyton",10],[45,"Waterloo",15],[27,"Waterloo",20],[32,"Cambridge",22],["Paradise","Cambridge"],["Leyton","Waterloo"],[10,"Leyton",24],["Leyton","Waterloo"],[10,"Waterloo",38],["Leyton","Waterloo"]]

Output
[null,null,null,null,null,null,null,14.0,11.0,null,11.0,null,12.0]

Explanation
UndergroundSystem undergroundSystem = new UndergroundSystem();
undergroundSystem.checkIn(45, "Leyton", 3);
undergroundSystem.checkIn(32, "Paradise", 8);
undergroundSystem.checkIn(27, "Leyton", 10);
undergroundSystem.checkOut(45, "Waterloo", 15);
undergroundSystem.checkOut(27, "Waterloo", 20);
undergroundSystem.checkOut(32, "Cambridge", 22);
undergroundSystem.getAverageTime("Paradise", "Cambridge");       // return 14.0. There was only one travel from "Paradise" (at time 8) to "Cambridge" (at time 22)
undergroundSystem.getAverageTime("Leyton", "Waterloo");          // return 11.0. There were two travels from "Leyton" to "Waterloo", a customer with id=45 from time=3 to time=15 and a customer with id=27 from time=10 to time=20. So the average time is ( (15-3) + (20-10) ) / 2 = 11.0
undergroundSystem.checkIn(10, "Leyton", 24);
undergroundSystem.getAverageTime("Leyton", "Waterloo");          // return 11.0
undergroundSystem.checkOut(10, "Waterloo", 38);
undergroundSystem.getAverageTime("Leyton", "Waterloo");          // return 12.0
```
 

Constraints:

  -  There will be at most $20000$ operations.
  -  $1 \le id, t \le10^6$
  -  All strings consist of **uppercase**, **lowercase** English letters and **digits**.
  -  `1 <= stationName.length <= 10`
  -  Answers within $10^{-5}$ of the actual value will be accepted as correct.

题意：设计一个`UndergroundSystem`类，提供三个方法，`checkIn`、`checkOut`、`getAverageTime`。其中，调用`checkIn`给出的参数是 乘客ID,起点站,起点时间，`chekOut`给出的参数是 乘客ID,终点站,终点时间。一个乘客只会一次从起点出发，到达终点。`getAverageTime`需要我们求出**从起点到终点站的所有乘客花的平均时间**。

思路：这个题目还是有点麻烦的，需要设计一下；如果能够直接一次性给出 `id,startStation, endStation, startTime, endtime`，会简单一些。麻烦的是，它是分两次给出的；而且这些操作最多有 $20000$ 次，需要想办法优化；一个字符串有大小写字母和数字，长度最长有 $10$，直接字符串哈希不可行。

因此必须先用 $id$ 哈希，保存起点站名和起点时间，然后再和终点站名和终点时间一起处理。

### 代码一 暴力循环(不通过)
暴力循环，求时间的和与次数。结果超出时间限制。
```cpp
struct node {
    string s, e;
    double t;
};
const int maxn = 1000010;
node arr[maxn];
class UndergroundSystem {
public:
    UndergroundSystem() { }
    
    void checkIn(int id, string stationName, int t) {
        arr[id].s = stationName;
        arr[id].t = t;
    }
    
    void checkOut(int id, string stationName, int t) {
        arr[id].e = stationName;
        arr[id].t = (t - arr[id].t);
    }
    
    double getAverageTime(string startStation, string endStation) {
        int cnt = 0;
        double sum = 0;
        for (int i = 1; i < maxn; ++i) {
            if (arr[i].s == startStation && arr[i].e == endStation) {
                ++cnt;
                sum += arr[i].t;
            }
        }
        return sum / cnt;
    }
};

/**
 * Your UndergroundSystem object will be instantiated and called as such:
 * UndergroundSystem* obj = new UndergroundSystem();
 * obj->checkIn(id,stationName,t);
 * obj->checkOut(id,stationName,t);
 * double param_3 = obj->getAverageTime(startStation,endStation);
 */
```


### 代码二 双层map
在使用双层的map之前，我是想着用 $map$ 把起点和终点的站名映射到数字上，用一个二维矩阵存储**所有同一起点和终点的耗时总和**与**乘客人次**。`getAverageTime`的时候，只需查询`matrix[v][w]`就可以了。不过这样做，容易爆空间。

因此，我改为了使用双层的map，也不用把字符串映射到数字上了。代码如下：
```cpp
struct type { double cnt = 0, total = 0; };

struct node { string t; int s; };
node arr[1000010];

class UndergroundSystem {
public:
    map<string, map<string, type> > dict; 
    UndergroundSystem() { }
    
    void checkIn(int id, string stationName, int t) {
        arr[id].t = stationName;
        arr[id].s = t;
    }
    
    void checkOut(int id, string stationName, int t) {
        dict[arr[id].t][stationName].total += (t - arr[id].s);
        dict[arr[id].t][stationName].cnt++;
    }
    
    double getAverageTime(string startStation, string endStation) {
        type temp = dict[startStation][endStation];
        return temp.total / temp.cnt;
    }
};
```
