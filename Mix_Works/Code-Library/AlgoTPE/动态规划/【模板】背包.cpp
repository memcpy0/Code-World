//01背包
int dp[SIZE]MAXV];
for (int i = 1; i <= n; ++i) { //n个物品
	for (int j = 0; j <= w[i]; ++j) //写成j<=w[i]也可以
		dp[i][j] = dp[i - 1][j];    //1~i个物品中,占用当前背包的体积为j(j <= w[i])时无法装入第i个物品
	for (int j = w[i]; j <= v; ++j) 
		dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - w[i]] + c[i]);
}
//滚动数组
int dp[2][MAXV]; //两倍空间
for (int i = 1; i <= n; ++i) { //n个物品
	for (int j = 0; j <= w[i]; ++j) //无法装入第i个物品时
		dp[i & 1][j] = dp[(i - 1) & 1][j]; //沿袭之前的值
	for (int j = w[i]; j <= v; ++j)
		dp[i & 1][j] = max(dp[(i - 1) & 1][j], dp[(i - 1) & 1][j - w[i]] + c[i]);
int dp[MAXV];
for (int i = 1; i <= n; ++i) //n个物品
	for (int j = v; j >= w[i]; --j) //逆序
		dp[j] = max(dp[j], dp[j - w[i]] + c[i]);
int dp[MAXM]; 
memset(dp, 0x7f, sizeof(dp)); //语义是最小,需要赋值为无穷大,以便后续更新
dp[0] = 0; //价值为0时,什么都没装
for (int i = 1; i <= n; ++i) //枚举所有物品
	for (int j = m; j >= c[i]; --j) //m等于所有物品的价值之和
		dp[j] = min(dp[j], dp[j - c[i]] + w[i]);
//完全背包
int ans = 0; 
for (int i = 1; i <= m; ++i) //枚举所有的价值
	if (dp[i] <= v)			 //得到可行的最大价值
		ans = i;
int dp[MAXV];
for (int i = 1; i <= n; ++i)
	for (int j = w[i]; j <= v; ++j) //顺序
		dp[j] = max(dp[j], dp[j - w[i]] + c[i]);
		
背包问题主要是背模板，这里收录了一些模板

一些复杂的背包问题（如泛化物品）未收录

01背包问题：

无优化

for(int i=1;i<=n;i++)
{
    for(int c=0;c<=m;c++)
    {
        f[i][c]=f[i-1][c];
        if(c>=w[i])
        f[i][c]=max(f[i][c],f[i-1][c-w[i]]+v[i]);
    }
}

一维数组优化：

for(int i=1;i<=n;i++)
{
    for(int c=m;c>=0;c--)
    {
        if(c>=w[i])
        f[c]=max(f[c],f[c-w[i]]+v[i]);
    }
}

更进一步的常数优化：

for(int i=1;i<=n;i++)
{
    sumw+=w[i];
    bound=max(m-sumw,w[i]);
    for(int c=m;c>=bound;c--)
    {
        if(c>=w[i])
        f[c]=max(f[c],f[c-w[i]]+v[i]);
    }
}

完全背包问题：

for(int i=1;i<=n;i++)
{
    for(int c=0;c<=m;c++)
    {
        if(c>=w[i])
        f[c]=max(f[c],f[c-w[i]]+v[i]);
    }
}

多重背包问题：

for(int i=1;i<=n;i++)
{
    if(w[i]*a[i]>m)
    {
        for(int c=0;c<=m;c++)
        {
        if(c>=w[i])
        f[c]=max(f[c],f[c-w[i]]+v[i]);
        }
    }
    else
    {
         k=1;amount=a[i];
         while(k<amount)
         {
             for(int c=k*w[i];c>=0;c--)
             {
                 if(c>=w[i])
                 f[c]=max(f[c],f[c-w[i]]+k*v[i]);
             }
             amount-=k;
             k<<=1;
         }
         for(int c=amount*w[i];c>=0;c--)
         {
             f[c]=max(f[c],f[c-w[i]]+amount*v[i]);
         }
    }
}
