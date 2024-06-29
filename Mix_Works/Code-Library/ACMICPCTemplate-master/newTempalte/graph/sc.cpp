int dfsnum[2000];
int low[2000];
int stack[2000];
int top;
int ans;
int an;
int be[2000];
int flag[2000];
void dfs(int x) {
  dfsnum[x] = low[x] = ans++;
  stack[++top] = x;
  flag[x] = 1;
  for (int i = head[x]; i != -1; i = edge[i].next) {
    int y = edge[i].to;
    if (dfsnum[y] == -1) {
      dfs(y);
      low[x] = min(low[x],low[y]);
    } else if (flag[y] == 1)
      low[x] = min(low[x],dfsnum[y]);
  }
  if (dfsnum[x] == low[x]) {
    while (stack[top] != x) {
      flag[stack[top]] = 0;
      be[stack[top]] = an;
      top--;
    }
    flag[x] = 0;
    be[x] = an++;
    top--;
  }
}
void SC() {
  memset(dfsnum,-1,sizeof(dfsnum));
  memset(flag,0,sizeof(flag));
  top = 0;
  an = 0;
  ans = 0;
  for (int i = 0; i < n; i++)
    if (dfsnum[i] == -1)
      dfs(i);
}

