九九乘法表。
```java
public class MultipleTable {
	public static void printTable(int m) { //打印出m*m的乘法表
		for (int i = 1; i <= m; i++) {
			for (int j = 1; j <= i; j++) 
				System.out.print(j + "*" + i + "=" + i * j + "\t");
			System.out.println();
		}
	}
	
	public static void main(String[] args) {
		printTable(9);
		printTable(11);
	}
}
```
学生成绩统计。
```java
import java.util.*;

public class StudentGroup {
	private double avgScore = 0; //平均分
	private double maxScore = 0; //最高分
	private int goodNum = 0; //高于平均分的人数
	private int badNum = 0; //不及格人数
	private double[] score = new double[10];
	
	public void calAverage() {
		System.out.println("请输入10个学生的成绩(空格隔开): ");
		Scanner input = new Scanner(System.in);
		for (int i = 0; i <= 9; i++) {
			score[i] = input.nextInt();
			avgScore += score[i];
		}
		avgScore /= 10;
	}
	public void getNum() { 
		for (int i = 0; i <= 9; i++) {
			if (score[i] > avgScore) goodNum++;
			if (score[i] < 60) badNum++;
			if (score[i] > maxScore) maxScore = score[i];
		}
	}
	public static void main(String[] args) {
		StudentGroup t = new StudentGroup();
		t.calAverage();
		t.getNum(); 
		System.out.print("10个学生中最高分为" + t.maxScore + ", 平均分为" + t.avgScore);
		System.out.print(", 不及格人数为" + t.badNum + ", 高于平均分人数为" + t.goodNum);
		System.out.println();
	}
}
```
