import java.math.BigInteger;
import java.util.*;
public class Main {
  public static class Fraction {
    public BigInteger num,den;
    public Fraction() {
      num=BigInteger.ZERO;
      den=BigInteger.ONE;
    }
    public Fraction(int _num,int _den) {
      num=BigInteger.valueOf(_num);
      den=BigInteger.valueOf(_den);
    }
    public Fraction(BigInteger _num,BigInteger _den) {
      num=_num;
      den=_den;
    }
    public Fraction gen() {
      BigInteger g=num.gcd(den);
      return new Fraction(num.divide(g),den.divide(g));
    }
    public Fraction add(Fraction x) {
      return new Fraction(x.num.multiply(den).add(num.multiply(x.den)),x.den.multiply(den)).gen();
    }
    public Fraction reciprocal() {
      return new Fraction(den,num);
    }
    public void out() {
      System.out.println(num+"/"+den);
    }
  }
  public static BigInteger sqrt(BigInteger a) {
    BigInteger b=a;
    while (a.compareTo(b.multiply(b))<0)
      b=b.multiply(b).add(a).divide(b.multiply(BigInteger.valueOf(2)));
    return b;
  }
  public static boolean check(Fraction x,int n) {
    return x.num.multiply(x.num).add(x.den.multiply(x.den.multiply(BigInteger.valueOf(n))).negate()).compareTo(BigInteger.ONE)==0;
  }
  static int p[]=new int[1000];
  static int l;
  public static void main(String[] args) {
    BigInteger ans=BigInteger.ZERO;
    int idx=0;
    for (int n=2,r=2; n<=1000; n++) {
      if (n==r*r) {
        r++;
        continue;
      }
      int tmp=calc(n,0,1),a=tmp,b=n-tmp*tmp;
      p[0]=tmp;
      l=1;
      while (true) {
        tmp=calc(n,a,b);
        p[l++]=tmp;
        a=a-tmp*b;
        Fraction x=getFrac();
        if (check(x,n)) {
          if (ans.compareTo(x.num)<0) {
            ans=x.num;
            idx=n;
          }
          break;
        }
        a=-a;
        b=(n-a*a)/b;
      }
    }
    System.out.println(idx);
  }
  private static Fraction getFrac() {
    Fraction ret=new Fraction(p[l-1],1);
    for (int i=l-2; i>=0; i--)
      ret=new Fraction(p[i],1).add(ret.reciprocal());
    return ret;
  }
  private static int calc(int n, int a, int b) {
    for (long i=2;; i++)
      if ((i*b-a)*(i*b-a)>n)
        return (int)i-1;
  }
}

