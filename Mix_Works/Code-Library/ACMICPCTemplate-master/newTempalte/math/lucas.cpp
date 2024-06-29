//`$num[i] = i!$`
int comLucus(int n,int m,int p) {
  int ans=1;
  for (; n && m && ans; n/=p,m/=p) {
    if (n%p>=m%p)
      ans = ans*num[n%p]%p*getInv(num[m%p]%p)%p
            *getInv(num[n%p-m%p])%p;
    else
      ans=0;
  }
  return ans;
}
