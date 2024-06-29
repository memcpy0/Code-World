@[toc]
## 货币的时间价值
现在的1000元的价值要大于未来的1000元，即使不考虑通货膨胀的影响，原因在于我们可以用手中的1000元进行投资并获得利息(interest)。因此，几乎所有的财务决策都必须考虑到货币的时间价值，而财务管理这门课程的许多内容也建立在对货币时间价值的理解和把握上。通往货币时间价值的关键则在于利率。
### 单利
单利：按照货币借贷的初始金额(本金)支付的利息。SI=PV~0~(i)(n)。SI为单利利息额，PV~0~为第0期的本金/现值，i为利率，n为期数。
对于任意的单利利率，账户第n期期末的终值为FV~n~=PV~0~+SI=PV~0~(1+(i)(n))。
```c
#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;

class SimpleInterest {
    private:
        double i, interest; //利率, 利息 
        int n;    //期数 
        double presentValue, futureValue; //现值, 终值 
    public:
    	SimpleInterest() { i = interest = n = presentValue = futureValue = 0; }
    	SimpleInterest(double _i, int _n): i(_i), n(_n) {	}
    	SimpleInterest(double _p, double _i, int _n): i(_i), n(_n), presentValue(_p) { 
		    interest = presentValue * i * n; //利息 
		    futureValue = presentValue + interest; 
	    }
		SimpleInterest(double _i, int _n, double _f): i(_i), n(_n), futureValue(_f) {	
            presentValue = futureValue / (1.0 + i * n);
            interest = presentValue * i * n;
		}   
		double getInterest() { return interest; }
        double getFutureValue() { return futureValue; }
		double getPresentValue() { return presentValue; }
}; 

int main() {
	SimpleInterest s = SimpleInterest(100, 0.08, 10);
	cout << "利息额:" << s.getInterest() << endl;
	cout << "第10期期末的终值为:" << s.getFutureValue() << endl; 
	return 0;
}
```

