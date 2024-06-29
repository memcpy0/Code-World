2.³ÌĞò´úÂë#include<iostream>
using namespace std;
int main()
{
 char NUM[10]={'a','b','c','d','e','f','g','h','i','j'};
 char UNIT[10]={0,0,'S','B','Q','W','S','B','Q','Y'};
 char RESULT[17];
 int n,k=0,bitnum=0,cur,pre;
 cin>>n;
 if(!n)
 {
  cout<<NUM[0];
  return 0;
 }
 while(n)
 {
  cur=n%10;
  n/=10;
  bitnum++;
  if(cur)
  {
   if(bitnum>1) 
   RESULT[k++]=UNIT[bitnum]; 
   RESULT[k++]=NUM[cur];
  }
  else
  {
   if(bitnum==5)
   RESULT[k++]=UNIT[bitnum];
   else if(pre!=0&&bitnum!=4&&bitnum!=1)
   RESULT[k++]=NUM[cur];
  }
  pre=cur;
 }
 for(int i=k-1;i>=0;i--)
 cout<<RESULT[i];
 return 0;
}
