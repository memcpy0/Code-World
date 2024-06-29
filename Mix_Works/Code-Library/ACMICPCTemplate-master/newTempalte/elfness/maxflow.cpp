/*
Author: elfness@UESTC
*/
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<algorithm>
#include<iostream>
#include<vector>
#include<string>
using namespace std;
typedef long long LL;
const int V=1500;
const int En=900000;
const int inf=0x3f3f3f3f;
struct Edge {
  int num,ne;
  int c;
} e[En];
int p[V],K;
void add(int x,int y,int c) {
  e[K].num=y;
  e[K].c=c;
  e[K].ne=p[x];
  p[x]=K++;
  e[K].num=x;
  e[K].c=0;
  e[K].ne=p[y];
  p[y]=K++;
}
int d[V],pre[V],pree[V],gap[V],cur[V];
int N,st,ed;
int low[V];
int sap() {
  int ret=0;
  bool fail;
  for(int i=0; i<=N; i++) {
    d[i]=0;
    gap[i]=0;
    cur[i]=p[i];
    low[i]=0;
  }
  low[st]=inf;
  gap[0]=N;
  int u=st;
  while(d[st]<N) {
    fail=true;
    for(int i=cur[u]; i!=-1; i=e[i].ne) {
      int v=e[i].num;
      cur[u]=i;
      if(e[i].c&&d[u]==d[v]+1) {
        pre[v]=u;
        pree[v]=i;
        low[v]=min(low[u],e[i].c);
        u=v;
        if(u==ed) {
          do {
            e[pree[u]].c-=low[ed];
            e[pree[u]^1].c+=low[ed];
            u=pre[u];
          } while(u!=st);
          ret+=low[ed];
        }
        fail=false;
        break;
      }
    }
    if(fail) {
      gap[d[u]]--;
      if(!gap[d[u]])return ret;
      d[u]=N;
      for(int i=p[u]; i!=-1; i=e[i].ne)
        if(e[i].c)d[u]=min(d[u],d[e[i].num]+1);
      gap[d[u]]++;
      cur[u]=p[u];
      if(u!=st)u=pre[u];
    }
  }
  return ret;
}
int n,m,s,t;
struct Elf {
  int u,v,lo,up;
} b[12000];
int lb[12000];
int doit() {
  int i;
  N=n+2;
  st=n;
  ed=n+1;
  for(i=0; i<N; i++)p[i]=-1;
  K=0;
  for(i=0; i<n; i++)lb[i]=0;
  for(i=0; i<m; i++) {
    lb[b[i].u]-=b[i].lo;
    lb[b[i].v]+=b[i].lo;
    add(b[i].u,b[i].v,b[i].up-b[i].lo);
  }
  for(i=0; i<n; i++) {
    if(lb[i]>0)add(st,i,lb[i]);
    else add(i,ed,-lb[i]);
  }
  add(t,s,inf);
  int te=sap();
  for(i=p[st]; i!=-1; i=e[i].ne)
    if(e[i].c!=0)return -1;
  st=s;
  ed=t;
  te=sap();
  return te;
}