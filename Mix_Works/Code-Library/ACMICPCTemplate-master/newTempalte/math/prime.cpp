int N;
bool isPrime[10001];
int prime[10000];
void getPrime(int n) {
  memset(isPrime,1,++n);
  N=0;
  isPrime[0]=isPrime[1]=0;
  for (int i=2; i<n; i++) {
    if (isPrime[i])
      prime[N++]=i;
    for (int j=0; j<N && prime[j]*i<n; j++) {
      isPrime[i*prime[j]]=0;
      if (i%prime[j]==0)
        break;
    }
  }
}
