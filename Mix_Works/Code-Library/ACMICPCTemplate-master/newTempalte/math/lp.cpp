#define MAXM 20  //max num of basic varibles
#define INF 1E200

double A[MAXM+5][MAXN+MAXM+5];
double b[MAXM+5],c[MAXN+MAXM+5];
int N[MAXN+5],B[MAXM+5];
double X[MAXN+MAXM+5],V;
int n,m,R,C,nCnt,bCnt;
int v1[MAXN],v2[MAXN];

int fcmp(double a,double b) {
  if(fabs(a-b)<1E-7) return 0;
  if(a>b) return 1;
  return -1;
}

void Pivot(int l,int e) {
  double t=A[l][e],p=c[e];
  b[l]=b[l]/t;
  for(int i=1; i<=C; i++)
    A[l][i]/=t;
  V=V-c[e]*b[l];
  for(int i=1; i<=R; i++) {
    if(i==l||fcmp(A[i][e],0.0)==0)
      continue;
    t=A[i][e];
    b[i]=b[i]-t*b[l];
    for(int j=1; j<=C; j++)
      A[i][j]=A[i][j]-t*A[l][j];
  }
  for(int i=1; i<=C; i++)
    c[i]=c[i]-p*A[l][i];
  for(int i=1; i<=nCnt; i++) {
    if(N[i]==e) {
      N[i]=B[l];
      break;
    }
  }
  B[l]=e;
}

bool Process(double P[]) {
  while(true) {
    int e=-1;
    double mV=-INF;
    for(int i=1; i<=nCnt; i++)
      if(fcmp(P[N[i]],mV)==1)
        mV=P[N[i]],e=N[i];

    if(fcmp(mV,0.0)<=0) break;
    int l=-1;
    mV=INF;
    for(int i=1; i<=bCnt; i++) {
      if(fcmp(A[i][e],0.0)==1) {
        double t=b[i]/A[i][e];
        if(fcmp(mV,t)==1||(fcmp(mV,t)==0&&(l==-1||B[l]>B[i])))
          mV=t,l=i;
      }
    }
    if(l==-1) return false;
    Pivot(l,e);
  }
  return true;
}

bool initSimplex() {
  nCnt=bCnt=0;
  for(int i=1; i<=n; i++)
    N[++nCnt]=i;
  for(int i=1; i<=m; i++)
    B[++bCnt]=i+n,A[i][n+i]=1.0;
  R=bCnt,C=bCnt+nCnt;
  double minV=INF;
  int p=-1;
  for(int i=1; i<=m; i++)
    if(fcmp(minV,b[i])==1)
      minV=b[i],p=i;
  if(fcmp(minV,0.0)>=0)
    return true;
  N[++nCnt]=n+m+1;
  R++,C++;
  for(int i=0; i<=C; i++)
    A[R][i]=0.0;
  for(int i=1; i<=R; i++)
    A[i][n+m+1]=-1.0;
  Pivot(p,n+m+1);
  if(!Process(A[R])) return false;
  if(fcmp(b[R],0.0)!=0)
    return false;
  p=-1;
  for(int i=1; i<=bCnt&&p==-1; i++)
    if(B[i]==n+m+1) p=i;
  if(p!=-1) {
    for(int i=1; i<=nCnt; i++) {
      if(fcmp(A[p][N[i]],0.0)!=0) {
        Pivot(p,N[i]);
        break;
      }
    }
  }
  bool f=false;
  for(int i=1; i<=nCnt; i++) {
    if(N[i]==n+m+1) f=true;
    if(f&&i+1<=nCnt)
      N[i]=N[i+1];
  }
  nCnt--;
  R--,C--;
  return true;
}

//-1: no solution 1: no bound 0: has a solution -V
int Simplex() {
  if(!initSimplex())
    return -1;
  if(!Process(c))
    return 1;
  for(int i=1; i<=nCnt; i++)
    X[N[i]]=0.0;
  for(int i=1; i<=bCnt; i++)
    X[B[i]]=b[i];
  return 0;
}

int main() {
  //n = 1;m=1;
  //V= 0.0;
  //c[1] = 1.0;
  //A[1][1] = 1.0;
  //b[1] = 5.0;
  //Simplex();
  //printf("V = %.3f\n",V);

  while(scanf("%d",&v1[1]) == 1) {
    for(int i = 2; i<=6; i++)
      scanf("%d",&v1[i]);
    n = 4;
    m = 6;
    for(int i = 0 ; i<=m+1; i++)
      for(int j=0; j<=n+m+2; j++)
        A[i][j] = c[j] = 0;
    memset(b,0,sizeof(b));
    V = 0.0;
    /*
    n 为未知数个数
    m 为约束个数
    目标：siama(c[i]*xi)
    约束：sigma(A[i][j]*xj) <=b[i]; j = 1 ... n
    解存在X里面
    */
    b[1] = v1[1] ;
    A[1][1] = 1;
    A[1][4] = 1;
    b[2] = v1[2] ;
    A[2][1] = 1;
    A[2][3] = 1;
    b[3] = v1[3] ;
    A[3][3] = 1;
    A[3][4] = 1;
    b[4] = v1[4] ;
    A[4][2] = 1;
    A[4][3] = 1;
    b[5] = v1[5] ;
    A[5][2] = 1;
    A[5][4] = 1;
    b[6] = v1[6] ;
    A[6][1] = 1;
    A[6][2] = 1;
    c[1] = 1;
    c[2] = 1;
    c[3] = 1;
    c[4] = 1;
    Simplex();
    //printf("V = %.3f\n",V);
    printf("%.3f %.3f %.3f %.3f\n",X[1],X[2],X[3],X[4]);

  }
  return 0;
}

