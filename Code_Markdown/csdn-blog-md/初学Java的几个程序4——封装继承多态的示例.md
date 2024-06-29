涉及了抽象类，toString/hashCode/equals方法的覆盖等。
```java
 // 封装继承多态举例
class Date { // 日期类
	private int year;
	private int month;
	private int day;
	Date() {}
	Date(int y, int m, int d) {
		this.year = y;
		this.month = m;
		this.day = d;
	}
	public int getYear() {
		return year;
	}
	public int getMonth() {
		return month;
	}
	public int getDay() {
		return day;
	}
	public void setDate(int y, int m, int d) {
		year = y;
		month = m;
		day = d;
	}
	public String toString() { // 多态，覆盖Object类的toString方法
		return (year + "-" + month + "-" + day);
	}
	public boolean equals(Object otherObject) { // 多态，覆盖Object类的equals方法
		if (this == otherObject) return true;
	    if (otherObject == null) return false;
	    if (getClass() != otherObject.getClass()) return false;
	    Date other = (Date)otherObject;
	    return year == other.year && month == other.month && day == other.day;
	}
	public int hashCode() { // 多态，覆盖Object类的hashCode方法
		return Integer.parseInt(String.format("%04d%02d%02d", year, month, day));
	} 
}
//----------------------------------------------------------------------------
abstract class Person { // 人，抽象类
	private String name; // 姓名 
	public Person() {}
	public Person(String n) { // 重载默认的构造函数
		name = n;
	}
	public String getName() {
		return name;
	} 
	public abstract String getDescription(); // 抽象方法 
}

class Employee extends Person { // 雇员，继承抽象类的子类
	private double salary;
	private Date hireDate;
	public Employee() {}
	public Employee(double s) { // 重载默认的构造函数
		this("Employee #", s);
	}
	public Employee(String n, double s) { // 重载默认的构造函数
		super(n);
		salary = s;
	}
	public Employee(String n, double s, int year, int month, int day) { // 重载构造函数
		super(n);
		salary = s;
		hireDate = new Date(year, month, day);
	}
	public double getSalary() {
		return salary;
	}
	public Date getHiredate() {
		return hireDate;
	}
	public String getDescription() { // 实现抽象方法
		return String.format("公司雇员， 年薪是￥%.2f。", salary);
	}
	public boolean equals(Object otherObject) { // 多态，覆盖Object的方法
		if (this == otherObject) return true;
		if (otherObject == null) return false;
		if (getClass() != otherObject.getClass()) return false;
		Employee other = (Employee)otherObject;
		return getName().equals(other.getName()) && salary == other.salary && hireDate.equals(other.hireDate); 
	}
	public int hashCode() { // 多态，覆盖Object的方法
		return 7 * getName().hashCode() + 11 * new Double(salary).hashCode() + 13 * hireDate.hashCode();
	}
	public String toString() { // 多态，覆盖Object的方法
		return "Employee, [name=" + getName() + ", salary=" + salary + ", hireDate=" + hireDate.toString() + "]";
	}
}

class Manager extends Employee {
	private double bonus; // 经理津贴
	public Manager(String n, double s, int year, int month, int day) {
		super(n, s, year, month, day);
		bonus = 0;
	}
	public void setBonus(double b) {
		bonus = b;
	}
	public boolean equals(Object otherObject) { // 多态，覆盖Employee的方法
		if (!super.equals(otherObject)) return false; // 使用父类Employee的equals方法
		Manager other = (Manager)otherObject;
		return bonus == other.bonus; 
	}
	public int hashCode() { // 多态，覆盖Employee的方法 
		return super.hashCode() + 17 * new Double(bonus).hashCode();
	}
	public String toString() { // 多态，覆盖Employee的方法
		return super.toString() + ", is a Manager, [bonus=" + bonus + "]";
	}
}
// ----------------------------------------------------------------------------
class Teacher extends Person  { // 老师，继承抽象类的子类
	Date birthDate; // 出生日期
	public Teacher() {}
	public Teacher(String n, int year, int month, int day) {
		super(n);
		birthDate = new Date(year, month, day);
	}
	public String getDescription() { // 实现抽象方法
		return "老师， 出生日期是" + birthDate.toString();
	}
	public Date getBirthdate() {
		return birthDate;
	}
}

class Student extends Person { // 学生，继承抽象类的子类
	private String major; // 主修专业
	public Student() {}
	public Student(String n, String m) {
		super(n);
		major = m;
	}
	public String getDescription() { // 实现抽象方法
		return "大学生， 专业是" + major + "。";
	}
	public String getMajor() {
		return major;
	}
}
  
public class Examples {
    public static void main(String[] args) {
    	Person[] people = new Person[5];
    	people[0] = new Employee("张浩", 50000, 1989, 11, 1);
    	people[1] = new Manager("南浩", 100000, 1985, 5, 2);
    	((Manager)people[1]).setBonus(50000);
    	people[2] = new Teacher("刘晓玲", 1970, 8, 18);
    	people[3] = new Student("李明", "计算机科学");
    	people[4] = new Employee("刘闻", 60000, 1987, 9, 19);
    	// 多态
    	for (Person p : people) {
    		System.out.println(p.getName() + ", "  + p.getDescription());
    	}
    	// 覆盖的toString方法测试
    	for (Person p : people) {
    		if (p instanceof Employee) {
    			System.out.println(p);
    		}
    	}
    	// 覆盖的equals方法测试
    	Employee zhang = (Employee)people[0];
    	// ==测试，结果为true; 都指向同一个对象
    	System.out.println((Employee)people[0] == zhang);
    	// equals测试, 结果为true
    	System.out.println(((Employee)people[0]).equals(zhang));
    	
    	Employee zhang2 = new Employee("张浩", 50000, 1989, 11, 1);
    	// 非赋值操作，==测试，结果为false, 因为不是同一个对象
    	System.out.println((Employee)people[0] == zhang2);
    	// equals测试, 结果为true, 信息相同
    	System.out.println(((Employee)people[0]).equals(zhang2));
    }
}
```
