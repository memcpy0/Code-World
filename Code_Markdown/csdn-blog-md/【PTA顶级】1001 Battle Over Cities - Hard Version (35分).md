
<p>It is vitally important to have all the cities connected by highways in a war.  If a city is conquered by the enemy, all the highways from/toward that city will be closed.  To keep the rest of the cities connected, we must repair some highways with the minimum cost.  On the other hand, if losing a city will cost us too much to rebuild the connection, we must pay more attention to that city.</p>
<p>Given the map of cities which have all the destroyed and remaining highways marked, you are supposed to point out the city to which we must pay the most attention.</p>
<h3 id="input-specification-">Input Specification:</h3>

Each input file contains one test case. Each case starts with a line containing 2 numbers `N` ($\le 500$) , and <code>M</code>, which are the total number of cities, and the number of highways, respectively.  Then <code>M</code> lines follow, each describes a highway by 4 integers: <code>City1 City2 Cost Status</code> where <code>City1</code> and <code>City2</code> are the numbers of the cities the highway connects (the cities are numbered from 1 to <code>N</code>), <code>Cost</code> is the effort taken to repair that highway if necessary, and <code>Status</code> is either 0, meaning that highway is destroyed, or 1, meaning that highway is in use.
<p>Note: It is guaranteed that the whole country was connected before the war.</p>
<h3 id="output-specification-">Output Specification:</h3>
<p>For each test case, just print in a line the city we must protest the most, that is, it will take us the maximum effort to rebuild the connection if that city is conquered by the enemy.</p>
<p>In case there is more than one city to be printed, output them in increasing order of the city numbers, separated by one space, but no extra space at the end of the line.  In case there is no need to repair any highway at all, simply output 0.</p>
<h3 id="sample-input-1-">Sample Input 1:</h3>

```swift
4 5
1 2 1 1
1 3 1 1
2 3 1 0
2 4 1 1
3 4 1 0
```

<h3>Sample Output 1:</h3>

```swift
1 2
```
<h3>Sample Input 2:

```swift
4 5
1 2 1 1
1 3 1 1
2 3 1 0
2 4 1 1
3 4 2 1
```

<h3 id="sample-output-2-">Sample Output 2:</h3>

```swift
0
```
题意：在一场战争中，将所有城市通过高速公路连通起来是很重要的。如果一座城市被敌人征服了，那么所有从它出发或者到达它的高速公路都必须被关闭。为了保证剩下的城市仍然能够连通，我们必须以最小的代价修复某些要道；此外，如果失去一座城市会付出很大代价去重建联系，就必须更加关注这座城市。

给出城市的地图，标记出所有已被摧毁或仍然存在的高速公路，指出我们应该最关注哪一座城市。

```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
	int n, m;
	scanf("%d%d", &n, &m);
	int city1, city2, cost, status;
	for (int i = 0; i < m; ++i) {
		scanf("%d%d%d%d", &city1, &city2, &cost, &status);
		
```
的题意是假设某个城市被占领后，要使剩下的城市保持联通可能会花钱修路，求最小花费里花费最多的那个被占领的城市。

这个题凭感觉就是最小生成树，最小生成树满足权值最小（最小花费），所以依次去掉某个城市的所有与其相接的路径，把剩下的路加入最小生成树，求最大值即可。

有一个地方写的时候没注意到，就是去掉某个城市后可能导致连通块的数量的增多，这种情况下算这个城市的花费无限大就可以了（自己真菜QAQ）。
技术图片

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <queue>
#include <vector>
#include <cmath>

using namespace std;

const int INF = 0x3f3f3f3f;
int n,m,max1;
int f[505];
vector<int> ans;

struct P{
    int u,v,c,s;
    bool operator<(const P &a)const{
        if(s != a.s) return s > a.s;//优先选择还能用的路，这种路径不增加花费
        else return c<a.c;//其实是处理需要修理的路径，先修花费少的
    }
}p[250005];

int Find(int x){
    if(x==f[x]) return x;
    else return f[x]=Find(f[x]);
}

int main(){
    scanf("%d%d",&n,&m);
    max1=0;
    for(int i=1;i<=m;i++)
        scanf("%d%d%d%d",&p[i].u,&p[i].v,&p[i].c,&p[i].s);
    sort(p+1,p+1+m);
    for(int i=1;i<=n;i++){
        int num=0,cnt=0;
        for(int i=1;i<=n;i++) f[i]=i;
        for(int j=1;j<=m;j++){
            if(num==n-2) break;
            int u=p[j].u;
            int v=p[j].v;
            if(u==i || v==i) continue ;//去掉某个城市
            u=Find(u);
            v=Find(v);
            if(u!=v){
                f[u]=v;
                num++;
                if(p[j].s==0) cnt+=p[j].c;
            }
        }
        if(num!=n-2){//先判断删除某个城市后是否连通
            if(max1!=INF){
                max1=INF;
                ans.clear();
            }
            ans.push_back(i);
        }else{
            if(cnt>max1){
                max1=cnt;
                ans.clear();
                ans.push_back(i);
            }else if(cnt==max1 && max1!=0)
                ans.push_back(i);
        }
    }
    if(ans.size()==0) printf("0\n");
    else{
        sort(ans.begin(),ans.end());
        printf("%d",ans[0]);
        for(int i=1;i<ans.size();i++)
            printf(" %d",ans[i]);
    }
    return 0;
}


