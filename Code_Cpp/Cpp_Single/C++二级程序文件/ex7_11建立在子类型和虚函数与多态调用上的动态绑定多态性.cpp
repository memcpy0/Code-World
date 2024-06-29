#include <iostream>
#include <string>
using namespace std;

class Base{
	string base_name;
	public:
		Base():base_name("BASE"){	}
		virtual const string my_name() const { return base_name;};
		const string class_name() const { return base_name;}
};
class Derived:public Base {
	string derived_name;
	public:
		Derived(): derived_name("DERIVED") {	}
		virtual const string my_name() const { return derived_name;}
		const string class_name() const { return derived_name;}
};

void show_ptr(Base *p){
	cout << p->my_name() << '\t' << p->class_name();
}
void show_ref(Base &r){
	cout << r.my_name() << '\t' << r.class_name();
}

int main()
{
	Base bb;
	Derived dd;
	show_ptr(&bb); cout << " <== 应显示:Base   Base" << endl;
	show_ptr(&dd); cout << " <== 应显示:Derived   Derived" << endl;
	cout << endl;
	show_ref(bb); cout << " <== 应显示:Base   Base" << endl;
	show_ref(dd); cout << " <== 应显示:Derived   Derived" << endl;
}

